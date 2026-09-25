package falcon.api;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.TreeMap;

public final class Config {
    private static final String WHITESPACE = " \t\r\n";

    private final Path mPath;
    private final Map<String, String> mValues = new TreeMap<>();

    public Config(Path path) {
        mPath = path;
    }

    public boolean load() {
        mValues.clear();
        if (!Files.isRegularFile(mPath)) {
            return false;
        }

        try (BufferedReader reader = Files.newBufferedReader(mPath, StandardCharsets.UTF_8)) {
            String line;
            while ((line = reader.readLine()) != null) {
                String trimmed = trim(line);
                if (trimmed.isEmpty() || trimmed.charAt(0) == '#') {
                    continue;
                }

                int separator = trimmed.indexOf(':');
                if (separator < 0) {
                    continue;
                }

                String key = trim(trimmed.substring(0, separator));
                if (!key.isEmpty()) {
                    mValues.put(key, unquote(trim(trimmed.substring(separator + 1))));
                }
            }
            return true;
        } catch (IOException exception) {
            return false;
        }
    }

    public boolean save() {
        try {
            Path parent = mPath.toAbsolutePath().getParent();
            if (parent != null) {
                Files.createDirectories(parent);
            }
            try (BufferedWriter writer = Files.newBufferedWriter(mPath, StandardCharsets.UTF_8)) {
                for (Map.Entry<String, String> entry : mValues.entrySet()) {
                    writer.write(entry.getKey() + ": " + quote(entry.getValue()) + "\n");
                }
            }
            return true;
        } catch (IOException exception) {
            return false;
        }
    }

    public boolean has(String key) {
        return mValues.containsKey(key);
    }

    public List<String> keys() {
        return new ArrayList<>(mValues.keySet());
    }

    public String getString(String key) {
        return getString(key, "");
    }

    public String getString(String key, String fallback) {
        return mValues.getOrDefault(key, fallback);
    }

    public long getInt(String key) {
        return getInt(key, 0);
    }

    public long getInt(String key, long fallback) {
        String value = mValues.get(key);
        if (value == null) {
            return fallback;
        }
        try {
            return Long.parseLong(value.trim());
        } catch (NumberFormatException exception) {
            return fallback;
        }
    }

    public double getDouble(String key) {
        return getDouble(key, 0.0);
    }

    public double getDouble(String key, double fallback) {
        String value = mValues.get(key);
        if (value == null) {
            return fallback;
        }
        try {
            return Double.parseDouble(value.trim());
        } catch (NumberFormatException exception) {
            return fallback;
        }
    }

    public boolean getBool(String key) {
        return getBool(key, false);
    }

    public boolean getBool(String key, boolean fallback) {
        String value = mValues.get(key);
        if (value == null) {
            return fallback;
        }
        return switch (value) {
            case "true", "yes", "on" -> true;
            case "false", "no", "off" -> false;
            default -> fallback;
        };
    }

    public void set(String key, String value) {
        mValues.put(key, value == null ? "" : value);
    }

    public void set(String key, long value) {
        mValues.put(key, Long.toString(value));
    }

    public void set(String key, int value) {
        mValues.put(key, Integer.toString(value));
    }

    public void set(String key, double value) {
        mValues.put(key, String.format(Locale.ROOT, "%f", value));
    }

    public void set(String key, boolean value) {
        mValues.put(key, value ? "true" : "false");
    }

    public void setDefault(String key, String value) {
        if (!has(key)) {
            set(key, value);
        }
    }

    public void setDefault(String key, long value) {
        if (!has(key)) {
            set(key, value);
        }
    }

    public void setDefault(String key, int value) {
        if (!has(key)) {
            set(key, value);
        }
    }

    public void setDefault(String key, double value) {
        if (!has(key)) {
            set(key, value);
        }
    }

    public void setDefault(String key, boolean value) {
        if (!has(key)) {
            set(key, value);
        }
    }

    public void remove(String key) {
        mValues.remove(key);
    }

    public Path path() {
        return mPath;
    }

    private static String trim(String value) {
        int first = 0;
        int last = value.length();
        while (first < last && WHITESPACE.indexOf(value.charAt(first)) >= 0) {
            first++;
        }
        while (last > first && WHITESPACE.indexOf(value.charAt(last - 1)) >= 0) {
            last--;
        }
        return value.substring(first, last);
    }

    private static String unquote(String value) {
        if (value.length() >= 2 && value.charAt(0) == '"' && value.charAt(value.length() - 1) == '"') {
            return value.substring(1, value.length() - 1);
        }
        return value;
    }

    private static String quote(String value) {
        boolean plain = !value.isEmpty() && value.chars().noneMatch(character -> ":#\"".indexOf(character) >= 0)
                        && value.charAt(0) != ' ' && value.charAt(value.length() - 1) != ' ';
        return plain ? value : "\"" + value + "\"";
    }
}
