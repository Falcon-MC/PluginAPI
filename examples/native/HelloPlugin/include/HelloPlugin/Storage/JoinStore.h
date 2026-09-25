#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

class JoinStore {
public:
    void load(const std::string &path);

    void save() const;

    uint32_t recordJoin(const std::string &playerName);

    uint32_t joinsOf(const std::string &playerName) const;

private:
    std::string mPath;
    std::unordered_map<std::string, uint32_t> mJoins;
};
