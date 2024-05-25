//
// Copyright (C) 2024 EA group inc.
// Author: Jeff.li lijippy@163.com
// All rights reserved.
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//


#pragma once

// periodic worker thread - periodically executes the given callback function.
//
// RAII over the owned thread:
//    creates the thread on construction.
//    stops and joins the thread on destruction (if the thread is executing a callback, wait for it
//    to finish first).

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace collie::log {
    namespace details {

        class periodic_worker {
        public:
            template<typename Rep, typename Period>
            periodic_worker(const std::function<void()> &callback_fun,
                            std::chrono::duration<Rep, Period> interval) {
                active_ = (interval > std::chrono::duration<Rep, Period>::zero());
                if (!active_) {
                    return;
                }

                worker_thread_ = std::thread([this, callback_fun, interval]() {
                    for (;;) {
                        std::unique_lock<std::mutex> lock(this->mutex_);
                        if (this->cv_.wait_for(lock, interval, [this] { return !this->active_; })) {
                            return;  // active_ == false, so exit this thread
                        }
                        callback_fun();
                    }
                });
            }

            periodic_worker(const periodic_worker &) = delete;

            periodic_worker &operator=(const periodic_worker &) = delete;

            // stop the worker thread and join it
            ~periodic_worker();

        private:
            bool active_;
            std::thread worker_thread_;
            std::mutex mutex_;
            std::condition_variable cv_;
        };
    }  // namespace details
}  // namespace collie::log

#include <collie/log/details/periodic_worker-inl.h>
