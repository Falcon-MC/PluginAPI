#pragma once

#include <cstdint>
#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace falcon {
    class Config {
    public:
        Config() = default;

        explicit Config(std::string path) : mPath(std::move(path)) {
        }

        bool load() {
            mValues.clear();
            std::ifstream file(mPath);
            if (!file)
                return false;

            std::string line;
            while (std::getline(file, line)) {
                const std::string trimmed = trim(line);
                if (trimmed.empty() || trimmed[0] == '#')
                    continue;

                const size_t separator = trimmed.find(':');
                if (separator == std::string::npos)
                    continue;

                const std::string key = trim(trimmed.substr(0, separator));
                if (!key.empty())
                    mValues[key] = unquote(trim(trimmed.substr(separator + 1)));
            }
            return true;
        }

        bool save() const {
            std::ofstream file(mPath, std::ios::trunc);
            if (!file)
                return false;

            for (const auto &entry: mValues)
                file << entry.first << ": " << quote(entry.second) << '\n';
            return true;
        }

        bool has(const std::string &key) const {
            return mValues.count(key) != 0;
        }

        std::vector<std::string> keys() const {
            std::vector<std::string> result;
            result.reserve(mValues.size());
            for (const auto &entry: mValues)
                result.push_back(entry.first);
            return result;
        }

        std::string getString(const std::string &key, const std::string &fallback = "") const {
            const auto it = mValues.find(key);
            return it == mValues.end() ? fallback : it->second;
        }

        int64_t getInt(const std::string &key, int64_t fallback = 0) const {
            const auto it = mValues.find(key);
            if (it == mValues.end())
                return fallback;

            try {
                return std::stoll(it->second);
            } catch (...) {
                return fallback;
            }
        }

        double getDouble(const std::string &key, double fallback = 0.0) const {
            const auto it = mValues.find(key);
            if (it == mValues.end())
                return fallback;

            try {
                return std::stod(it->second);
            } catch (...) {
                return fallback;
            }
        }

        bool getBool(const std::string &key, bool fallback = false) const {
            const auto it = mValues.find(key);
            if (it == mValues.end())
                return fallback;
            if (it->second == "true" || it->second == "yes" || it->second == "on")
                return true;
            if (it->second == "false" || it->second == "no" || it->second == "off")
                return false;
            return fallback;
        }

        void set(const std::string &key, const std::string &value) {
            mValues[key] = value;
        }

        void set(const std::string &key, const char *value) {
            mValues[key] = value == nullptr ? std::string() : std::string(value);
        }

        void set(const std::string &key, int64_t value) {
            mValues[key] = std::to_string(value);
        }

        void set(const std::string &key, int value) {
            mValues[key] = std::to_string(value);
        }

        void set(const std::string &key, double value) {
            mValues[key] = std::to_string(value);
        }

        void set(const std::string &key, bool value) {
            mValues[key] = value ? "true" : "false";
        }

        template<typename T>
        void setDefault(const std::string &key, T value) {
            if (!has(key))
                set(key, value);
        }

        void remove(const std::string &key) {
            mValues.erase(key);
        }

        const std::string &path() const {
            return mPath;
        }

    private:
        static std::string trim(const std::string &value) {
            const size_t first = value.find_first_not_of(" \t\r\n");
            if (first == std::string::npos)
                return std::string();
            const size_t last = value.find_last_not_of(" \t\r\n");
            return value.substr(first, last - first + 1);
        }

        static std::string unquote(const std::string &value) {
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
                return value.substr(1, value.size() - 2);
            return value;
        }

        static std::string quote(const std::string &value) {
            const bool plain = !value.empty() && value.find_first_of(":#\"") == std::string::npos
                               && value.front() != ' ' && value.back() != ' ';
            return plain ? value : "\"" + value + "\"";
        }

        std::string mPath;
        std::map<std::string, std::string> mValues;
    };
}
