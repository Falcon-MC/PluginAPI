using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

public class GenerateFalconBindings : Task
{
    private static readonly Dictionary<string, string> Primitives = new Dictionary<string, string>
    {
        { "void", "void" },
        { "char", "byte" },
        { "signed char", "sbyte" },
        { "unsigned char", "byte" },
        { "short", "short" },
        { "unsigned short", "ushort" },
        { "int", "int" },
        { "unsigned", "uint" },
        { "unsigned int", "uint" },
        { "long long", "long" },
        { "unsigned long long", "ulong" },
        { "float", "float" },
        { "double", "double" },
        { "int8_t", "sbyte" },
        { "uint8_t", "byte" },
        { "int16_t", "short" },
        { "uint16_t", "ushort" },
        { "int32_t", "int" },
        { "uint32_t", "uint" },
        { "int64_t", "long" },
        { "uint64_t", "ulong" },
        { "size_t", "nuint" },
        { "intptr_t", "nint" },
        { "uintptr_t", "nuint" }
    };

    private static readonly HashSet<string> Keywords = new HashSet<string>
    {
        "abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const",
        "continue", "decimal", "default", "delegate", "do", "double", "else", "enum", "event", "explicit",
        "extern", "false", "finally", "fixed", "float", "for", "foreach", "goto", "if", "implicit", "in", "int",
        "interface", "internal", "is", "lock", "long", "namespace", "new", "null", "object", "operator", "out",
        "override", "params", "private", "protected", "public", "readonly", "ref", "return", "sbyte", "sealed",
        "short", "sizeof", "stackalloc", "static", "string", "struct", "switch", "this", "throw", "true", "try",
        "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "virtual", "void", "volatile", "while"
    };

    private readonly Dictionary<string, string> _aliases = new Dictionary<string, string>();
    private readonly Dictionary<string, string[]> _functionAliases = new Dictionary<string, string[]>();
    private readonly HashSet<string> _structs = new HashSet<string>();

    [Required]
    public string Header { get; set; }

    [Required]
    public string OutputFile { get; set; }

    [Required]
    public string Namespace { get; set; }

    public override bool Execute()
    {
        try
        {
            string source = File.ReadAllText(Header).Replace("\r\n", "\n");
            string generated = Translate(source);
            if (File.Exists(OutputFile) && File.ReadAllText(OutputFile) == generated)
            {
                return true;
            }

            string directory = Path.GetDirectoryName(OutputFile);
            if (!string.IsNullOrEmpty(directory))
            {
                Directory.CreateDirectory(directory);
            }

            File.WriteAllText(OutputFile, generated);
            return true;
        }
        catch (Exception exception)
        {
            Log.LogError("Could not generate the bindings from {0}: {1}", Header, exception.Message);
            return false;
        }
    }

    private string Translate(string source)
    {
        string text = Regex.Replace(source, @"/\*.*?\*/", " ", RegexOptions.Singleline);
        text = Regex.Replace(text, @"//[^\n]*", " ");

        List<KeyValuePair<string, string>> constants = ReadConstants(text);
        text = Regex.Replace(text, @"^[ \t]*#.*$", " ", RegexOptions.Multiline);

        List<string> statements = new List<string>();
        foreach (Match match in Regex.Matches(text, @"typedef\b[^;{}]*(?:\{[^{}]*\}[^;{}]*)?;"))
        {
            statements.Add(match.Value);
        }

        foreach (string statement in statements)
        {
            Collect(statement);
        }

        StringBuilder output = new StringBuilder();
        output.Append("using System.Runtime.InteropServices;\n\n");
        output.Append("namespace ").Append(Namespace).Append("\n{\n");
        WriteConstants(output, constants);

        foreach (string statement in statements)
        {
            WriteStatement(output, statement);
        }

        output.Append("}\n");
        return output.ToString();
    }

    private static List<KeyValuePair<string, string>> ReadConstants(string text)
    {
        List<KeyValuePair<string, string>> constants = new List<KeyValuePair<string, string>>();
        HashSet<string> seen = new HashSet<string>();
        string pattern = @"^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+(.+?)[ \t]*$";
        foreach (Match match in Regex.Matches(text, pattern, RegexOptions.Multiline))
        {
            string name = match.Groups[1].Value;
            string value = match.Groups[2].Value;
            string declaration = ConstantDeclaration(ConstantName(name), value);
            if (declaration == null || !seen.Add(name))
            {
                continue;
            }

            constants.Add(new KeyValuePair<string, string>(name, declaration));
        }

        return constants;
    }

    private static string ConstantDeclaration(string name, string value)
    {
        Match number = Regex.Match(value, @"^(0[xX][0-9a-fA-F]+|\d+)([uU]?)$");
        if (number.Success)
        {
            string type = number.Groups[2].Value.Length > 0 ? "uint" : "int";
            return "public const " + type + " " + name + " = " + number.Groups[1].Value + ";";
        }

        Match text = Regex.Match(value, "^\"([^\"\\\\]*)\"$");
        if (text.Success)
        {
            return "public const string " + name + " = \"" + text.Groups[1].Value + "\";";
        }

        return null;
    }

    private static string ConstantName(string name)
    {
        string trimmed = name.StartsWith("FALCON_", StringComparison.Ordinal) ? name.Substring(7) : name;
        StringBuilder result = new StringBuilder();
        foreach (string part in trimmed.Split(new[] { '_' }, StringSplitOptions.RemoveEmptyEntries))
        {
            result.Append(char.ToUpperInvariant(part[0]));
            result.Append(part.Substring(1).ToLowerInvariant());
        }

        if (result.Length == 0 || char.IsDigit(result[0]))
        {
            result.Insert(0, '_');
        }

        return result.ToString();
    }

    private void Collect(string statement)
    {
        string text = Normalize(statement);

        Match opaque = Regex.Match(text, @"^typedef struct (\w+) (\w+);$");
        if (opaque.Success)
        {
            _structs.Add(opaque.Groups[2].Value);
            return;
        }

        Match structure = Regex.Match(text, @"^typedef struct (\w+ )?\{(.*)\} ?(\w+);$");
        if (structure.Success)
        {
            _structs.Add(structure.Groups[3].Value);
            return;
        }

        Match function = Regex.Match(text, @"^typedef (.+?)\( ?\* ?(\w+) ?\) ?\((.*)\);$");
        if (function.Success)
        {
            _functionAliases[function.Groups[2].Value] = new[] { function.Groups[1].Value, function.Groups[3].Value };
            return;
        }

        Match alias = Regex.Match(text, @"^typedef (.+?[ \*])(\w+);$");
        if (alias.Success)
        {
            _aliases[alias.Groups[2].Value] = alias.Groups[1].Value.Trim();
            return;
        }

        throw new InvalidOperationException("Unsupported declaration: " + text);
    }

    private static void WriteConstants(StringBuilder output, List<KeyValuePair<string, string>> constants)
    {
        output.Append("    public static class FalconConstants\n    {\n");
        foreach (KeyValuePair<string, string> constant in constants)
        {
            output.Append("        ").Append(constant.Value).Append('\n');
        }

        output.Append("    }\n");
    }

    private void WriteStatement(StringBuilder output, string statement)
    {
        string text = Normalize(statement);

        Match opaque = Regex.Match(text, @"^typedef struct (\w+) (\w+);$");
        if (opaque.Success)
        {
            output.Append("\n    public struct ").Append(opaque.Groups[2].Value).Append("\n    {\n    }\n");
            return;
        }

        Match structure = Regex.Match(text, @"^typedef struct (\w+ )?\{(.*)\} ?(\w+);$");
        if (!structure.Success)
        {
            return;
        }

        output.Append("\n    [StructLayout(LayoutKind.Sequential)]\n");
        output.Append("    public unsafe struct ").Append(structure.Groups[3].Value).Append("\n    {\n");
        foreach (string field in structure.Groups[2].Value.Split(';'))
        {
            string declaration = field.Trim();
            if (declaration.Length == 0)
            {
                continue;
            }

            output.Append("        public ").Append(Field(declaration)).Append(";\n");
        }

        output.Append("    }\n");
    }

    private string Field(string declaration)
    {
        Match function = Regex.Match(declaration, @"^(.+?)\( ?\* ?(\w+) ?\) ?\((.*)\)$");
        if (function.Success)
        {
            string type = FunctionPointer(function.Groups[1].Value, function.Groups[3].Value);
            return type + " " + Identifier(function.Groups[2].Value);
        }

        if (declaration.IndexOf('[') >= 0 || declaration.IndexOf(',') >= 0)
        {
            throw new InvalidOperationException("Unsupported field: " + declaration);
        }

        Match field = Regex.Match(declaration, @"^(.+?[ \*])(\w+)$");
        if (!field.Success)
        {
            throw new InvalidOperationException("Unsupported field: " + declaration);
        }

        return MapType(field.Groups[1].Value) + " " + Identifier(field.Groups[2].Value);
    }

    private string FunctionPointer(string returnType, string parameters)
    {
        List<string> types = new List<string>();
        foreach (string parameter in SplitParameters(parameters))
        {
            string declaration = parameter.Trim();
            if (declaration.Length == 0 || declaration == "void")
            {
                continue;
            }

            if (declaration.IndexOf('(') >= 0)
            {
                throw new InvalidOperationException("Unsupported parameter: " + declaration);
            }

            types.Add(MapType(ParameterType(declaration)));
        }

        types.Add(MapType(returnType));
        return "delegate* unmanaged[Cdecl]<" + string.Join(", ", types) + ">";
    }

    private static string ParameterType(string declaration)
    {
        Match named = Regex.Match(declaration, @"^(.+?[ \*])(\w+)$");
        if (!named.Success)
        {
            return declaration;
        }

        string type = Regex.Replace(named.Groups[1].Value, @"\bconst\b", " ").Trim();
        string name = named.Groups[2].Value;
        if (type.Length == 0 || Primitives.ContainsKey(name) || type == "unsigned" || type == "signed")
        {
            return declaration;
        }

        return named.Groups[1].Value;
    }

    private static IEnumerable<string> SplitParameters(string parameters)
    {
        List<string> result = new List<string>();
        int depth = 0;
        int start = 0;
        for (int index = 0; index < parameters.Length; index++)
        {
            char character = parameters[index];
            if (character == '(')
            {
                depth++;
            }
            else if (character == ')')
            {
                depth--;
            }
            else if (character == ',' && depth == 0)
            {
                result.Add(parameters.Substring(start, index - start));
                start = index + 1;
            }
        }

        result.Add(parameters.Substring(start));
        return result;
    }

    private string MapType(string declaration)
    {
        string type = Regex.Replace(declaration, @"\bconst\b", " ");
        int pointers = 0;
        foreach (char character in type)
        {
            if (character == '*')
            {
                pointers++;
            }
        }

        string name = Regex.Replace(type.Replace("*", " "), @"\s+", " ").Trim();
        return Resolve(name) + new string('*', pointers);
    }

    private string Resolve(string name)
    {
        string primitive;
        if (Primitives.TryGetValue(name, out primitive))
        {
            return primitive;
        }

        string alias;
        if (_aliases.TryGetValue(name, out alias))
        {
            return MapType(alias);
        }

        string[] function;
        if (_functionAliases.TryGetValue(name, out function))
        {
            return FunctionPointer(function[0], function[1]);
        }

        if (_structs.Contains(name))
        {
            return name;
        }

        throw new InvalidOperationException("Unsupported type: " + name);
    }

    private static string Identifier(string name)
    {
        return Keywords.Contains(name) ? "@" + name : name;
    }

    private static string Normalize(string statement)
    {
        string text = Regex.Replace(statement, @"\s+", " ").Trim();
        text = Regex.Replace(text, @" ?\* ?", " * ");
        text = Regex.Replace(text, @" ?\( ?", "(");
        text = Regex.Replace(text, @" ?\) ?", ")");
        text = Regex.Replace(text, @" ?, ?", ", ");
        text = Regex.Replace(text, @" ?; ?", ";");
        text = Regex.Replace(text, @" ?\{ ?", " {");
        text = Regex.Replace(text, @" ?\} ?", "} ");
        text = Regex.Replace(text, @"\s+", " ").Trim();
        return text.Replace("} ;", "};");
    }
}
