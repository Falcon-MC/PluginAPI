#pragma once

#include "falcon/Api.hpp"

#include <string>

namespace falcon {
    class Logger {
    public:
        void info(const std::string &message) const {
            detail::api().log(detail::plugin(), FALCON_LOG_INFO, message.c_str());
        }

        void warning(const std::string &message) const {
            detail::api().log(detail::plugin(), FALCON_LOG_WARNING, message.c_str());
        }

        void error(const std::string &message) const {
            detail::api().log(detail::plugin(), FALCON_LOG_ERROR, message.c_str());
        }
    };
}
