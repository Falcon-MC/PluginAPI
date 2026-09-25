import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public final class BindingGenerator {
    private static final int MAX_LINE = 120;
    private static final String PACKAGE = "falcon.api.internal";
    private static final String SERVER_API = "FalconServerApi";
    private static final Pattern DEFINE = Pattern.compile("^[ \\t]*#[ \\t]*define[ \\t]+(\\w+)[ \\t]+(.+?)[ \\t]*$",
            Pattern.MULTILINE);
    private static final Pattern NUMBER = Pattern.compile("(-?\\d+)[uUlL]*");
    private static final Pattern STRING = Pattern.compile("\"((?:[^\"\\\\]|\\\\.)*)\"");
    private static final Pattern TOKEN = Pattern.compile("[A-Za-z_]\\w*|\\d+\\w*|\"(?:[^\"\\\\]|\\\\.)*\"|\\S");
    private static final Set<String> QUALIFIERS = Set.of("const", "volatile", "struct", "signed");
    private static final Set<String> KEYWORDS = Set.of("abstract", "assert", "boolean", "break", "byte", "case",
            "catch", "char", "class", "const", "continue", "default", "do", "double", "else", "enum", "extends",
            "final", "finally", "float", "for", "goto", "if", "implements", "import", "instanceof", "int",
            "interface", "long", "native", "new", "package", "private", "protected", "public", "return", "short",
            "static", "strictfp", "super", "switch", "synchronized", "this", "throw", "throws", "transient", "try",
            "void", "volatile", "while", "var", "record", "yield", "sealed", "permits", "true", "false", "null");

    private final Map<String, Constant> mConstants = new LinkedHashMap<>();
    private final Map<String, Type> mTypes = new LinkedHashMap<>();
    private final Map<String, Struct> mStructs = new LinkedHashMap<>();
    private final Map<String, Signature> mFunctionTypes = new LinkedHashMap<>();

    private BindingGenerator() {
        primitive("void", null, "void", 0);
        primitive("char", "JAVA_BYTE", "byte", 1);
        primitive("int8_t", "JAVA_BYTE", "byte", 1);
        primitive("uint8_t", "JAVA_BYTE", "byte", 1);
        primitive("int16_t", "JAVA_SHORT", "short", 2);
        primitive("uint16_t", "JAVA_SHORT", "short", 2);
        primitive("short", "JAVA_SHORT", "short", 2);
        primitive("int", "JAVA_INT", "int", 4);
        primitive("unsigned", "JAVA_INT", "int", 4);
        primitive("unsigned int", "JAVA_INT", "int", 4);
        primitive("int32_t", "JAVA_INT", "int", 4);
        primitive("uint32_t", "JAVA_INT", "int", 4);
        primitive("int64_t", "JAVA_LONG", "long", 8);
        primitive("uint64_t", "JAVA_LONG", "long", 8);
        primitive("size_t", "JAVA_LONG", "long", 8);
        primitive("uintptr_t", "JAVA_LONG", "long", 8);
        primitive("intptr_t", "JAVA_LONG", "long", 8);
        primitive("float", "JAVA_FLOAT", "float", 4);
        primitive("double", "JAVA_DOUBLE", "double", 8);
    }

    public static void main(String[] arguments) throws IOException {
        if (arguments.length != 3) {
            System.err.println("Usage: BindingGenerator <falcon_api.h> <output directory> <project version>");
            System.exit(2);
        }

        BindingGenerator generator = new BindingGenerator();
        generator.parse(Files.readString(Path.of(arguments[0]), StandardCharsets.UTF_8));
        generator.checkVersion(arguments[2]);

        Path directory = Path.of(arguments[1]).resolve(PACKAGE.replace('.', '/'));
        Files.createDirectories(directory);
        write(directory.resolve("FalconAbi.java"), generator.abiSource());
        write(directory.resolve("NativeApi.java"), generator.apiSource());
    }

    private static void write(Path path, String content) throws IOException {
        if (Files.exists(path) && Files.readString(path, StandardCharsets.UTF_8).equals(content)) {
            return;
        }
        Files.writeString(path, content, StandardCharsets.UTF_8);
    }

    private void primitive(String name, String layout, String javaType, long size) {
        mTypes.put(name, new Type(Kind.PRIMITIVE, layout, javaType, size, Math.max(size, 1), null));
    }

    private static Type pointer(Signature signature) {
        return new Type(Kind.POINTER, "ADDRESS", "MemorySegment", 8, 8, signature);
    }

    private void parse(String source) {
        String text = source.replaceAll("(?s)/\\*.*?\\*/", " ").replaceAll("//[^\\n]*", " ");
        readConstants(text);

        text = text.replaceAll("(?s)#\\s*ifdef\\s+__cplusplus.*?#\\s*endif", " ");
        text = text.replaceAll("(?m)^\\s*#.*$", " ");

        List<String> tokens = new ArrayList<>();
        Matcher matcher = TOKEN.matcher(text);
        while (matcher.find()) {
            tokens.add(matcher.group());
        }

        int depth = 0;
        List<String> statement = new ArrayList<>();
        for (String token : tokens) {
            if (token.equals("{")) {
                depth++;
            } else if (token.equals("}")) {
                depth--;
            }

            if (token.equals(";") && depth == 0) {
                declaration(statement);
                statement = new ArrayList<>();
            } else {
                statement.add(token);
            }
        }

        if (!statement.isEmpty()) {
            throw new IllegalStateException("Unterminated declaration: " + String.join(" ", statement));
        }
        if (!mStructs.containsKey(SERVER_API)) {
            throw new IllegalStateException("falcon_api.h does not declare " + SERVER_API);
        }
    }

    private void readConstants(String text) {
        Matcher matcher = DEFINE.matcher(text);
        while (matcher.find()) {
            String name = matcher.group(1);
            String value = matcher.group(2);
            if (!name.startsWith("FALCON_")) {
                continue;
            }

            String constant = name.substring("FALCON_".length());
            Matcher number = NUMBER.matcher(value);
            Matcher string = STRING.matcher(value);
            if (number.matches()) {
                long parsed = Long.parseLong(number.group(1));
                boolean wide = parsed > Integer.MAX_VALUE || parsed < Integer.MIN_VALUE;
                mConstants.put(constant, new Constant(wide ? "long" : "int", parsed + (wide ? "L" : "")));
            } else if (string.matches()) {
                mConstants.put(constant, new Constant("String", "\"" + string.group(1) + "\""));
            }
        }
    }

    private void declaration(List<String> tokens) {
        if (tokens.isEmpty()) {
            return;
        }
        if (!tokens.get(0).equals("typedef")) {
            throw new IllegalStateException("Unsupported declaration: " + String.join(" ", tokens));
        }

        List<String> body = tokens.subList(1, tokens.size());
        if (body.size() == 3 && body.get(0).equals("struct")) {
            mTypes.put(body.get(2), new Type(Kind.OPAQUE, null, null, 0, 1, null));
            return;
        }

        if (body.get(0).equals("struct") && body.contains("{")) {
            struct(body);
            return;
        }

        Member member = member(body);
        if (member.type().signature() != null) {
            mFunctionTypes.put(member.name(), member.type().signature());
        }
        mTypes.put(member.name(), member.type());
    }

    private void struct(List<String> body) {
        int open = body.indexOf("{");
        int close = body.lastIndexOf("}");
        if (close != body.size() - 2) {
            throw new IllegalStateException("Unsupported struct: " + String.join(" ", body));
        }

        String name = body.get(close + 1);
        List<Member> members = new ArrayList<>();
        List<String> current = new ArrayList<>();
        for (String token : body.subList(open + 1, close)) {
            if (token.equals(";")) {
                members.add(member(current));
                current = new ArrayList<>();
            } else {
                current.add(token);
            }
        }

        Struct struct = new Struct(name, members);
        mStructs.put(name, struct);
        mTypes.put(name, new Type(Kind.STRUCT, "FalconAbi." + className(name) + ".LAYOUT", "MemorySegment",
                struct.size(), struct.alignment(), null));
    }

    private Member member(List<String> tokens) {
        int open = functionStart(tokens);
        if (open < 0) {
            return new Member(tokens.get(tokens.size() - 1), type(tokens.subList(0, tokens.size() - 1)));
        }

        int nameClose = tokens.indexOf(")");
        String name = tokens.get(nameClose - 1);
        Type returnType = type(tokens.subList(0, open));
        List<Member> parameters = parameters(tokens.subList(nameClose + 2, tokens.size() - 1));
        return new Member(name, pointer(new Signature(returnType, parameters)));
    }

    private static int functionStart(List<String> tokens) {
        for (int index = 0; index + 1 < tokens.size(); index++) {
            if (tokens.get(index).equals("(") && tokens.get(index + 1).equals("*")) {
                return index;
            }
        }
        return -1;
    }

    private List<Member> parameters(List<String> tokens) {
        List<Member> parameters = new ArrayList<>();
        if (tokens.isEmpty() || (tokens.size() == 1 && tokens.get(0).equals("void"))) {
            return parameters;
        }

        int depth = 0;
        List<String> current = new ArrayList<>();
        for (String token : tokens) {
            if (token.equals("(")) {
                depth++;
            } else if (token.equals(")")) {
                depth--;
            }

            if (token.equals(",") && depth == 0) {
                parameters.add(parameter(current, parameters.size()));
                current = new ArrayList<>();
            } else {
                current.add(token);
            }
        }
        parameters.add(parameter(current, parameters.size()));
        return parameters;
    }

    private Member parameter(List<String> tokens, int index) {
        if (functionStart(tokens) >= 0) {
            Member function = member(tokens);
            return new Member(safeName(function.name()), function.type());
        }

        List<String> plain = tokens.stream().filter(token -> !QUALIFIERS.contains(token)).toList();
        String last = plain.get(plain.size() - 1);
        boolean named = plain.size() > 1 && Character.isJavaIdentifierStart(last.charAt(0))
                        && !mTypes.containsKey(String.join(" ", plain));
        if (!named) {
            return new Member("argument" + index, type(tokens));
        }
        return new Member(safeName(last), type(tokens.subList(0, tokens.lastIndexOf(last))));
    }

    private static String safeName(String name) {
        return KEYWORDS.contains(name) ? name + "Value" : name;
    }

    private Type type(List<String> tokens) {
        if (tokens.contains("*")) {
            return pointer(null);
        }

        String base = String.join(" ", tokens.stream().filter(token -> !QUALIFIERS.contains(token)).toList());
        Type type = mTypes.get(base);
        if (type == null || type.kind() == Kind.OPAQUE) {
            throw new IllegalStateException("Unsupported type: " + String.join(" ", tokens));
        }
        return type;
    }

    private void checkVersion(String version) {
        Constant major = mConstants.get("API_VERSION_MAJOR");
        Constant minor = mConstants.get("API_VERSION_MINOR");
        if (major == null || minor == null) {
            throw new IllegalStateException("falcon_api.h does not define the API version");
        }

        String expected = major.value() + "." + minor.value();
        if (!version.equals(expected) && !version.startsWith(expected + ".") && !version.startsWith(expected + "-")) {
            throw new IllegalStateException("The project version " + version + " does not match API " + expected
                                            + " in falcon_api.h");
        }
    }

    private static String className(String cName) {
        String stripped = cName.startsWith("Falcon") && cName.length() > 6 ? cName.substring(6) : cName;
        return Character.toUpperCase(stripped.charAt(0)) + stripped.substring(1);
    }

    private static String constantName(String name) {
        StringBuilder result = new StringBuilder();
        for (int index = 0; index < name.length(); index++) {
            char character = name.charAt(index);
            boolean upper = Character.isUpperCase(character);
            boolean afterLower = index > 0 && (Character.isLowerCase(name.charAt(index - 1))
                                               || Character.isDigit(name.charAt(index - 1)));
            boolean acronymEnd = index > 0 && index + 1 < name.length() && Character.isUpperCase(name.charAt(index - 1))
                                 && Character.isLowerCase(name.charAt(index + 1));
            if (upper && (afterLower || acronymEnd)) {
                result.append('_');
            }
            result.append(Character.toUpperCase(character));
        }
        return result.toString();
    }

    private static String descriptor(Signature signature) {
        List<String> layouts = new ArrayList<>();
        for (Member parameter : signature.parameters()) {
            layouts.add(parameter.type().layout());
        }
        if (signature.returnType().layout() == null) {
            return "FunctionDescriptor.ofVoid(" + String.join(", ", layouts) + ")";
        }
        layouts.add(0, signature.returnType().layout());
        return "FunctionDescriptor.of(" + String.join(", ", layouts) + ")";
    }

    private String abiSource() {
        Source out = new Source();
        out.line("package " + PACKAGE + ";");
        out.line("");
        out.line("import java.lang.foreign.FunctionDescriptor;");
        out.line("import java.lang.foreign.MemoryLayout;");
        out.line("import java.lang.foreign.StructLayout;");
        out.line("");
        out.line("import static java.lang.foreign.ValueLayout.ADDRESS;");
        out.line("import static java.lang.foreign.ValueLayout.JAVA_BYTE;");
        out.line("import static java.lang.foreign.ValueLayout.JAVA_DOUBLE;");
        out.line("import static java.lang.foreign.ValueLayout.JAVA_FLOAT;");
        out.line("import static java.lang.foreign.ValueLayout.JAVA_INT;");
        out.line("import static java.lang.foreign.ValueLayout.JAVA_LONG;");
        out.line("import static java.lang.foreign.ValueLayout.JAVA_SHORT;");
        out.line("");
        out.line("@SuppressWarnings(\"unused\")");
        out.line("public final class FalconAbi {");

        for (Map.Entry<String, Constant> entry : mConstants.entrySet()) {
            Constant constant = entry.getValue();
            out.wrapped("    public static final " + constant.type() + " " + entry.getKey() + " = ",
                    constant.value() + ";");
        }

        for (Map.Entry<String, Signature> entry : mFunctionTypes.entrySet()) {
            out.line("");
            out.wrapped("    public static final FunctionDescriptor " + constantName(className(entry.getKey())) + " = ",
                    descriptor(entry.getValue()) + ";");
        }

        for (Struct struct : mStructs.values()) {
            out.line("");
            structSource(out, struct);
        }

        out.line("");
        out.line("    private FalconAbi() {");
        out.line("    }");
        out.line("}");
        return out.text();
    }

    private void structSource(Source out, Struct struct) {
        String name = className(struct.name());
        out.line("    public static final class " + name + " {");

        List<String> elements = new ArrayList<>();
        long offset = 0;
        for (Member member : struct.members()) {
            long aligned = align(offset, member.type().alignment());
            if (aligned > offset) {
                elements.add("MemoryLayout.paddingLayout(" + (aligned - offset) + ")");
            }
            elements.add(member.type().layout() + ".withName(\"" + member.name() + "\")");
            offset = aligned + member.type().size();
        }
        if (struct.size() > offset) {
            elements.add("MemoryLayout.paddingLayout(" + (struct.size() - offset) + ")");
        }

        out.call("        public static final StructLayout LAYOUT = MemoryLayout.structLayout(", elements,
                ").withName(\"" + struct.name() + "\");");

        for (Member member : struct.members()) {
            out.line("");
            String constant = constantName(member.name());
            out.wrapped("        public static final long " + constant + " = ",
                    "LAYOUT.byteOffset(MemoryLayout.PathElement.groupElement(\"" + member.name() + "\"));");
            if (member.type().signature() != null) {
                out.wrapped("        public static final FunctionDescriptor " + constant + "_FUNCTION = ",
                        descriptor(member.type().signature()) + ";");
            }
        }

        out.line("");
        out.line("        private " + name + "() {");
        out.line("        }");
        out.line("    }");
    }

    private String apiSource() {
        Struct api = mStructs.get(SERVER_API);
        String abi = "FalconAbi." + className(SERVER_API);

        Source out = new Source();
        out.line("package " + PACKAGE + ";");
        out.line("");
        out.line("import java.lang.foreign.Linker;");
        out.line("import java.lang.foreign.MemorySegment;");
        out.line("import java.lang.foreign.SegmentAllocator;");
        out.line("import java.lang.foreign.ValueLayout;");
        out.line("import java.lang.invoke.MethodHandle;");
        out.line("");
        out.line("@SuppressWarnings(\"unused\")");
        out.line("public final class NativeApi {");
        out.line("    private static final Linker LINKER = Linker.nativeLinker();");

        for (Member member : api.members()) {
            if (member.type().signature() == null) {
                continue;
            }
            out.line("");
            out.line("    private static final MethodHandle " + constantName(member.name()) + " =");
            out.line("            LINKER.downcallHandle(" + abi + "." + constantName(member.name()) + "_FUNCTION);");
        }

        out.line("");
        out.line("    private final MemorySegment mTable;");
        for (Member member : api.members()) {
            if (member.type().signature() != null) {
                out.line("    private final MemorySegment m" + capitalize(member.name()) + ";");
            }
        }

        out.line("");
        out.line("    NativeApi(MemorySegment table) {");
        out.line("        mTable = table;");
        for (Member member : api.members()) {
            if (member.type().signature() != null) {
                out.wrapped("        m" + capitalize(member.name()) + " = ",
                        "table.get(ValueLayout.ADDRESS, " + abi + "." + constantName(member.name()) + ");");
            }
        }
        out.line("    }");

        for (Member member : api.members()) {
            out.line("");
            if (member.type().signature() == null) {
                fieldGetter(out, abi, member);
            } else {
                functionWrapper(out, member);
            }
        }

        out.line("");
        out.line("    private static RuntimeException failure(String function, Throwable throwable) {");
        out.line("        if (throwable instanceof RuntimeException runtime) {");
        out.line("            return runtime;");
        out.line("        }");
        out.line("        if (throwable instanceof Error error) {");
        out.line("            throw error;");
        out.line("        }");
        out.line("        return new IllegalStateException(function + \" failed\", throwable);");
        out.line("    }");
        out.line("}");
        return out.text();
    }

    private static void fieldGetter(Source out, String abi, Member member) {
        Type type = member.type();
        if (type.kind() != Kind.PRIMITIVE) {
            throw new IllegalStateException("Unsupported server API field: " + member.name());
        }
        out.line("    public " + type.javaType() + " " + member.name() + "() {");
        out.line("        return mTable.get(ValueLayout." + type.layout() + ", " + abi + "."
                 + constantName(member.name()) + ");");
        out.line("    }");
    }

    private static void functionWrapper(Source out, Member member) {
        Signature signature = member.type().signature();
        Type returnType = signature.returnType();
        boolean returnsStruct = returnType.kind() == Kind.STRUCT;

        List<String> declared = new ArrayList<>();
        List<String> passed = new ArrayList<>();
        passed.add("m" + capitalize(member.name()));
        if (returnsStruct) {
            declared.add("SegmentAllocator allocator");
            passed.add("allocator");
        }
        for (Member parameter : signature.parameters()) {
            declared.add(parameter.type().javaType() + " " + parameter.name());
            passed.add(parameter.name());
        }

        out.call("    public " + returnType.javaType() + " " + member.name() + "(", declared, ") {");
        out.line("        try {");
        String invoke = constantName(member.name()) + ".invokeExact(";
        if (returnType.layout() == null) {
            out.call("            " + invoke, passed, ");");
        } else {
            out.call("            return (" + returnType.javaType() + ") " + invoke, passed, ");");
        }
        out.line("        } catch (Throwable throwable) {");
        out.line("            throw failure(\"" + member.name() + "\", throwable);");
        out.line("        }");
        out.line("    }");
    }

    private static String capitalize(String name) {
        return Character.toUpperCase(name.charAt(0)) + name.substring(1);
    }

    private static long align(long offset, long alignment) {
        if (alignment <= 1) {
            return offset;
        }
        return (offset + alignment - 1) / alignment * alignment;
    }

    private enum Kind {
        PRIMITIVE,
        POINTER,
        STRUCT,
        OPAQUE
    }

    private record Constant(String type, String value) {
    }

    private record Type(Kind kind, String layout, String javaType, long size, long alignment, Signature signature) {
    }

    private record Member(String name, Type type) {
    }

    private record Signature(Type returnType, List<Member> parameters) {
    }

    private record Struct(String name, List<Member> members) {
        long alignment() {
            long alignment = 1;
            for (Member member : members) {
                alignment = Math.max(alignment, member.type().alignment());
            }
            return alignment;
        }

        long size() {
            long offset = 0;
            for (Member member : members) {
                offset = align(offset, member.type().alignment()) + member.type().size();
            }
            return align(offset, alignment());
        }
    }

    private static final class Source {
        private final StringBuilder mText = new StringBuilder();

        void line(String line) {
            mText.append(line).append('\n');
        }

        void wrapped(String head, String tail) {
            if (head.length() + tail.length() <= MAX_LINE) {
                line(head + tail);
                return;
            }
            line(head.stripTrailing());
            line(indentOf(head) + "        " + tail);
        }

        void call(String head, List<String> items, String tail) {
            String single = head + String.join(", ", items) + tail;
            if (single.length() <= MAX_LINE) {
                line(single);
                return;
            }

            String indent = indentOf(head) + "        ";
            line(head.stripTrailing());
            StringBuilder current = new StringBuilder(indent);
            for (int index = 0; index < items.size(); index++) {
                boolean last = index == items.size() - 1;
                String item = items.get(index) + (last ? tail : ",");
                boolean empty = current.length() == indent.length();
                if (!empty && current.length() + 1 + item.length() > MAX_LINE) {
                    line(current.toString());
                    current = new StringBuilder(indent);
                    empty = true;
                }
                if (!empty) {
                    current.append(' ');
                }
                current.append(item);
            }
            line(current.toString());
        }

        String text() {
            return mText.toString();
        }

        private static String indentOf(String text) {
            int count = 0;
            while (count < text.length() && text.charAt(count) == ' ') {
                count++;
            }
            return " ".repeat(count);
        }
    }
}
