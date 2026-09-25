using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;

namespace Falcon
{
    public sealed class Config
    {
        private static readonly char[] Blank = { ' ', '\t', '\r', '\n' };

        private readonly SortedDictionary<string, string> _values =
            new SortedDictionary<string, string>(StringComparer.Ordinal);

        public Config(string path)
        {
            Path = path;
        }

        public string Path { get; }

        public IReadOnlyList<string> Keys
        {
            get
            {
                return new List<string>(_values.Keys);
            }
        }

        public bool Load()
        {
            _values.Clear();
            string[] lines;
            try
            {
                lines = File.ReadAllLines(Path);
            }
            catch (IOException)
            {
                return false;
            }
            catch (UnauthorizedAccessException)
            {
                return false;
            }

            foreach (string line in lines)
            {
                string trimmed = line.Trim(Blank);
                if (trimmed.Length == 0 || trimmed[0] == '#')
                {
                    continue;
                }

                int separator = trimmed.IndexOf(':');
                if (separator < 0)
                {
                    continue;
                }

                string key = trimmed.Substring(0, separator).Trim(Blank);
                if (key.Length > 0)
                {
                    _values[key] = Unquote(trimmed.Substring(separator + 1).Trim(Blank));
                }
            }

            return true;
        }

        public bool Save()
        {
            StringBuilder builder = new StringBuilder();
            foreach (KeyValuePair<string, string> entry in _values)
            {
                builder.Append(entry.Key).Append(": ").Append(Quote(entry.Value)).Append('\n');
            }

            try
            {
                File.WriteAllText(Path, builder.ToString());
                return true;
            }
            catch (IOException)
            {
                return false;
            }
            catch (UnauthorizedAccessException)
            {
                return false;
            }
        }

        public bool Has(string key)
        {
            return _values.ContainsKey(key);
        }

        public string GetString(string key, string fallback = "")
        {
            string? value;
            if (!_values.TryGetValue(key, out value))
            {
                return fallback;
            }

            return value;
        }

        public long GetInt(string key, long fallback = 0)
        {
            string? value;
            long result;
            if (!_values.TryGetValue(key, out value)
                || !long.TryParse(value, NumberStyles.Integer, CultureInfo.InvariantCulture, out result))
            {
                return fallback;
            }

            return result;
        }

        public double GetDouble(string key, double fallback = 0.0)
        {
            string? value;
            double result;
            if (!_values.TryGetValue(key, out value)
                || !double.TryParse(value, NumberStyles.Float, CultureInfo.InvariantCulture, out result))
            {
                return fallback;
            }

            return result;
        }

        public bool GetBool(string key, bool fallback = false)
        {
            string? value;
            if (!_values.TryGetValue(key, out value))
            {
                return fallback;
            }

            if (value == "true" || value == "yes" || value == "on")
            {
                return true;
            }

            if (value == "false" || value == "no" || value == "off")
            {
                return false;
            }

            return fallback;
        }

        public void Set(string key, string? value)
        {
            _values[key] = value ?? string.Empty;
        }

        public void Set(string key, long value)
        {
            _values[key] = value.ToString(CultureInfo.InvariantCulture);
        }

        public void Set(string key, int value)
        {
            _values[key] = value.ToString(CultureInfo.InvariantCulture);
        }

        public void Set(string key, double value)
        {
            _values[key] = value.ToString("R", CultureInfo.InvariantCulture);
        }

        public void Set(string key, bool value)
        {
            _values[key] = value ? "true" : "false";
        }

        public void SetDefault(string key, string value)
        {
            if (!Has(key))
            {
                Set(key, value);
            }
        }

        public void SetDefault(string key, long value)
        {
            if (!Has(key))
            {
                Set(key, value);
            }
        }

        public void SetDefault(string key, int value)
        {
            if (!Has(key))
            {
                Set(key, value);
            }
        }

        public void SetDefault(string key, double value)
        {
            if (!Has(key))
            {
                Set(key, value);
            }
        }

        public void SetDefault(string key, bool value)
        {
            if (!Has(key))
            {
                Set(key, value);
            }
        }

        public void Remove(string key)
        {
            _values.Remove(key);
        }

        private static string Unquote(string value)
        {
            if (value.Length >= 2 && value[0] == '"' && value[value.Length - 1] == '"')
            {
                return value.Substring(1, value.Length - 2);
            }

            return value;
        }

        private static string Quote(string value)
        {
            bool plain = value.Length > 0 && value.IndexOfAny(new[] { ':', '#', '"' }) < 0 && value[0] != ' '
                         && value[value.Length - 1] != ' ';
            return plain ? value : "\"" + value + "\"";
        }
    }
}
