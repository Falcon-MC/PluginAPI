#include "HelloPlugin/Storage/JoinStore.h"

#include <fstream>

void JoinStore::load(const std::string &path) {
    mPath = path;
    mJoins.clear();

    std::ifstream file(mPath);
    std::string name;
    uint32_t count = 0;
    while (file >> name >> count)
        mJoins[name] = count;
}

void JoinStore::save() const {
    std::ofstream file(mPath, std::ios::trunc);
    for (const auto &entry: mJoins)
        file << entry.first << ' ' << entry.second << '\n';
}

uint32_t JoinStore::recordJoin(const std::string &playerName) {
    return ++mJoins[playerName];
}

uint32_t JoinStore::joinsOf(const std::string &playerName) const {
    const auto it = mJoins.find(playerName);
    return it == mJoins.end() ? 0 : it->second;
}
