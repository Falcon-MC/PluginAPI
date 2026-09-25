#pragma once

#include "falcon/Api.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace falcon {
    class Scheduler {
    public:
        uint64_t delay(uint64_t ticks, std::function<void()> task) const {
            return schedule(ticks, 0, std::move(task));
        }

        uint64_t repeat(uint64_t periodTicks, std::function<void()> task) const {
            return schedule(periodTicks, periodTicks, std::move(task));
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

        uint64_t schedule(uint64_t delayTicks, uint64_t periodTicks, std::function<void()> task) const {
            auto *stored = detail::keep(std::make_unique<std::function<void()>>(std::move(task)));
            return detail::api().scheduleTask(detail::plugin(), &runTask, stored, delayTicks, periodTicks);
        }

        static void runTask(void *userData) {
            (*static_cast<std::function<void()> *>(userData))();
        }

        static void runWork(void *userData) {
            static_cast<AsyncJob *>(userData)->mWork();
        }

        static void runDone(void *userData) {
            std::unique_ptr<AsyncJob> job(static_cast<AsyncJob *>(userData));
            if (job->mDone)
                job->mDone();
        }
    };
}
