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

#include <cassert>
#include <collie/log/common.h>

namespace collie::log {
namespace details {

inline thread_pool::thread_pool(size_t q_max_items,
                                       size_t threads_n,
                                       std::function<void()> on_thread_start,
                                       std::function<void()> on_thread_stop)
    : q_(q_max_items) {
    if (threads_n == 0 || threads_n > 1000) {
        throw_clog_ex(
            "collie::log::thread_pool(): invalid threads_n param (valid "
            "range is 1-1000)");
    }
    for (size_t i = 0; i < threads_n; i++) {
        threads_.emplace_back([this, on_thread_start, on_thread_stop] {
            on_thread_start();
            this->thread_pool::worker_loop_();
            on_thread_stop();
        });
    }
}

inline thread_pool::thread_pool(size_t q_max_items,
                                       size_t threads_n,
                                       std::function<void()> on_thread_start)
    : thread_pool(q_max_items, threads_n, on_thread_start, [] {}) {}

inline thread_pool::thread_pool(size_t q_max_items, size_t threads_n)
    : thread_pool(
          q_max_items, threads_n, [] {}, [] {}) {}

// message all threads to terminate gracefully join them
inline thread_pool::~thread_pool() {
    CLOG_TRY {
        for (size_t i = 0; i < threads_.size(); i++) {
            post_async_msg_(async_msg(async_msg_type::terminate), async_overflow_policy::block);
        }

        for (auto &t : threads_) {
            t.join();
        }
    }
    CLOG_CATCH_STD
}

void inline thread_pool::post_log(async_logger_ptr &&worker_ptr,
                                         const details::log_msg &msg,
                                         async_overflow_policy overflow_policy) {
    async_msg async_m(std::move(worker_ptr), async_msg_type::log, msg);
    post_async_msg_(std::move(async_m), overflow_policy);
}

void inline thread_pool::post_flush(async_logger_ptr &&worker_ptr,
                                           async_overflow_policy overflow_policy) {
    post_async_msg_(async_msg(std::move(worker_ptr), async_msg_type::flush), overflow_policy);
}

size_t inline thread_pool::overrun_counter() { return q_.overrun_counter(); }

void inline thread_pool::reset_overrun_counter() { q_.reset_overrun_counter(); }

size_t inline thread_pool::discard_counter() { return q_.discard_counter(); }

void inline thread_pool::reset_discard_counter() { q_.reset_discard_counter(); }

size_t inline thread_pool::queue_size() { return q_.size(); }

void inline thread_pool::post_async_msg_(async_msg &&new_msg,
                                                async_overflow_policy overflow_policy) {
    if (overflow_policy == async_overflow_policy::block) {
        q_.enqueue(std::move(new_msg));
    } else if (overflow_policy == async_overflow_policy::overrun_oldest) {
        q_.enqueue_nowait(std::move(new_msg));
    } else {
        assert(overflow_policy == async_overflow_policy::discard_new);
        q_.enqueue_if_have_room(std::move(new_msg));
    }
}

void inline thread_pool::worker_loop_() {
    while (process_next_msg_()) {
    }
}

// process next message in the queue
// return true if this thread should still be active (while no terminate msg
// was received)
bool inline thread_pool::process_next_msg_() {
    async_msg incoming_async_msg;
    q_.dequeue(incoming_async_msg);

    switch (incoming_async_msg.msg_type) {
        case async_msg_type::log: {
            incoming_async_msg.worker_ptr->backend_sink_it_(incoming_async_msg);
            return true;
        }
        case async_msg_type::flush: {
            incoming_async_msg.worker_ptr->backend_flush_();
            return true;
        }

        case async_msg_type::terminate: {
            return false;
        }

        default: {
            assert(false);
        }
    }

    return true;
}

}  // namespace details
}  // namespace collie::log
