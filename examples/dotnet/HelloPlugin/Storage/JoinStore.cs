using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;

namespace Hello.Storage
{
    public sealed class JoinStore
    {
        private static readonly char[] Separators = { ' ', '\t' };

        private readonly Dictionary<string, uint> _joins = new Dictionary<string, uint>();
        private string _path = string.Empty;

        public void Load(string path)
        {
            _path = path;
            _joins.Clear();
            if (!File.Exists(_path))
            {
                return;
            }

            foreach (string line in File.ReadAllLines(_path))
            {
                string[] parts = line.Split(Separators, StringSplitOptions.RemoveEmptyEntries);
                uint count;
                if (parts.Length == 2
                    && uint.TryParse(parts[1], NumberStyles.None, CultureInfo.InvariantCulture, out count))
                {
                    _joins[parts[0]] = count;
                }
            }
        }

        public void Save()
        {
            StringBuilder builder = new StringBuilder();
            foreach (KeyValuePair<string, uint> entry in _joins)
            {
                builder.Append(entry.Key).Append(' ').Append(entry.Value.ToString(CultureInfo.InvariantCulture));
                builder.Append('\n');
            }

            File.WriteAllText(_path, builder.ToString());
        }

        public uint RecordJoin(string playerName)
        {
            uint count = JoinsOf(playerName) + 1;
            _joins[playerName] = count;
            return count;
        }

        public uint JoinsOf(string playerName)
        {
            uint count;
            if (!_joins.TryGetValue(playerName, out count))
            {
                return 0;
            }

            return count;
        }
    }
}
