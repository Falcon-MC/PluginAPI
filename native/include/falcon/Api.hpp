#pragma once

#include "falcon/falcon_api.h"

#include <functional>
#include <memory>
#include <vector>

namespace falcon::detail {
    inline const FalconServerApi *gApi = nullptr;
    inline FalconPlugin *gPlugin = nullptr;

    inline const FalconServerApi &api() {
        return *gApi;
    }

    inline FalconPlugin *plugin() {
        return gPlugin;
    }

    template<typename T>
    T *keep(std::unique_ptr<T> value) {
        static std::vector<std::unique_ptr<T>> storage;
        storage.push_back(std::move(value));
        return storage.back().get();
    }
}
