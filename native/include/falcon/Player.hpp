#pragma once

#include "falcon/Api.hpp"

#include <string>

namespace falcon {
    class Player {
    public:
        explicit Player(FalconPlayer *handle) : mHandle(handle) {
        }

        bool valid() const {
            return mHandle != nullptr;
        }

        std::string name() const {
            return detail::api().playerName(mHandle);
        }

        bool isOperator() const {
            return detail::api().playerIsOperator(mHandle) != 0;
        }

        void sendMessage(const std::string &message) const {
            detail::api().playerSendMessage(mHandle, message.c_str());
        }

        void kick(const std::string &reason) const {
            detail::api().playerKick(mHandle, reason.c_str());
        }

        FalconPlayer *handle() const {
            return mHandle;
        }

    private:
        FalconPlayer *mHandle;
    };
}
