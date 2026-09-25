#pragma once

#include "falcon/Api.hpp"

#include <cstdint>
#include <string>

namespace falcon {
    enum class PermissionDefault : uint32_t {
        False = FALCON_PERMISSION_DEFAULT_FALSE,
        True = FALCON_PERMISSION_DEFAULT_TRUE,
        Operator = FALCON_PERMISSION_DEFAULT_OPERATOR
    };

    class Permissions {
    public:
        bool add(const std::string &node, PermissionDefault defaultValue = PermissionDefault::Operator) const {
            return detail::api().registerPermission(detail::plugin(), node.c_str(),
                                                    static_cast<FalconPermissionDefault>(defaultValue)) != 0;
        }
    };
}
