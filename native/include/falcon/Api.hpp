#pragma once

#include "falcon/falcon_api.h"

#include <exception>
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

    inline void reportException(const char *where) {
        if (gApi != nullptr && gPlugin != nullptr)
            gApi->log(gPlugin, FALCON_LOG_ERROR, where);
    }

    template<typename Function>
    void guarded(const char *where, Function &&function) {
        try {
            function();
        } catch (const std::exception &exception) {
            reportException(exception.what());
        } catch (...) {
            reportException(where);
        }
    }

    template<typename T>
    T *keep(std::unique_ptr<T> value) {
        static std::vector<std::unique_ptr<T>> storage;
        storage.push_back(std::move(value));
        return storage.back().get();
    }
}
