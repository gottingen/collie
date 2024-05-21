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

#include <collie/log/details/circular_q.h>
#include <collie/log/details/log_msg_buffer.h>
#include <collie/log/details/null_mutex.h>
#include <collie/log/sinks/base_sink.h>

#include <mutex>
#include <string>
#include <vector>

namespace collie::log {
    namespace sinks {
        /*
         * Ring buffer sink
         */
        template<typename Mutex>
        class ringbuffer_sink final : public base_sink<Mutex> {
        public:
            explicit ringbuffer_sink(size_t n_items)
                    : q_{n_items} {}

            std::vector<details::log_msg_buffer> last_raw(size_t lim = 0) {
                std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
                auto items_available = q_.size();
                auto n_items = lim > 0 ? (std::min)(lim, items_available) : items_available;
                std::vector<details::log_msg_buffer> ret;
                ret.reserve(n_items);
                for (size_t i = (items_available - n_items); i < items_available; i++) {
                    ret.push_back(q_.at(i));
                }
                return ret;
            }

            std::vector<std::string> last_formatted(size_t lim = 0) {
                std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
                auto items_available = q_.size();
                auto n_items = lim > 0 ? (std::min)(lim, items_available) : items_available;
                std::vector<std::string> ret;
                ret.reserve(n_items);
                for (size_t i = (items_available - n_items); i < items_available; i++) {
                    memory_buf_t formatted;
                    base_sink<Mutex>::formatter_->format(q_.at(i), formatted);
                    ret.push_back(fmt::to_string(formatted));
                }
                return ret;
            }

        protected:
            void sink_it_(const details::log_msg &msg) override {
                q_.push_back(details::log_msg_buffer{msg});
            }

            void flush_() override {}

        private:
            details::circular_q<details::log_msg_buffer> q_;
        };

        using ringbuffer_sink_mt = ringbuffer_sink<std::mutex>;
        using ringbuffer_sink_st = ringbuffer_sink<details::null_mutex>;

    }  // namespace sinks

}  // namespace collie::log
