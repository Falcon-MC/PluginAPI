#pragma once

#include "falcon/Api.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace falcon {
    class Scheduler {
    public:
        uint64_t delay(uint64_t ticks, std::function<void()> task) const {
            auto *once = new std::function<void()>(std::move(task));
            return detail::api().scheduleTask(detail::plugin(), &runOnce, once, ticks, 0);
        }

        uint64_t repeat(uint64_t periodTicks, std::function<void()> task) const {
            auto *stored = detail::keep(std::make_unique<std::function<void()>>(std::move(task)));
            return detail::api().scheduleTask(detail::plugin(), &runRepeating, stored, periodTicks, periodTicks);
        }

        uint64_t async(std::function<void()> work, std::function<void()> done = nullptr) const {
            auto *job = new AsyncJob{std::move(work), std::move(done)};
            return detail::api().runAsync(detail::plugin(), &runWork, &runDone, job);
        }

        void cancel(uint64_t task) const {
            detail::api().cancelTask(task);
        }

    private:
        struct AsyncJob {
            std::function<void()> mWork;
            std::function<void()> mDone;
        };

        static void runOnce(void *userData) {
            std::unique_ptr<std::function<void()>> task(static_cast<std::function<void()> *>(userData));
            detail::guarded("A task threw an exception", [&] {
                (*task)();
            });
        }

        static void runRepeating(void *userData) {
            detail::guarded("A task threw an exception", [&] {
                (*static_cast<std::function<void()> *>(userData))();
            });
        }

        static void runWork(void *userData) {
            detail::guarded("An asynchronous task threw an exception", [&] {
                static_cast<AsyncJob *>(userData)->mWork();
            });
        }

        static void runDone(void *userData) {
            std::unique_ptr<AsyncJob> job(static_cast<AsyncJob *>(userData));
            if (!job->mDone)
                return;

            detail::guarded("An asynchronous callback threw an exception", [&] {
                job->mDone();
            });
        }
    };
}
