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

#include <collie/log/sinks/base_sink.h>
#include <collie/log/details/log_msg.h>
#include <collie/log/details/null_mutex.h>
#include <collie/log/pattern_formatter.h>

#include <algorithm>
#include <memory>
#include <mutex>
#include <vector>

// Distribution sink (mux). Stores a vector of sinks which get called when log
// is called

namespace collie::log::sinks {

    template<typename Mutex>
    class dist_sink : public base_sink<Mutex> {
    public:
        dist_sink() = default;

        explicit dist_sink(std::vector<std::shared_ptr<sink>> sinks)
                : sinks_(sinks) {}

        dist_sink(const dist_sink &) = delete;

        dist_sink &operator=(const dist_sink &) = delete;

        void add_sink(std::shared_ptr<sink> sub_sink) {
            std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
            sinks_.push_back(sub_sink);
        }

        void remove_sink(std::shared_ptr<sink> sub_sink) {
            std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
            sinks_.erase(std::remove(sinks_.begin(), sinks_.end(), sub_sink), sinks_.end());
        }

        void set_sinks(std::vector<std::shared_ptr<sink>> sinks) {
            std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
            sinks_ = std::move(sinks);
        }

        std::vector<std::shared_ptr<sink>> &sinks() { return sinks_; }

    protected:
        void sink_it_(const details::log_msg &msg) override {
            for (auto &sub_sink: sinks_) {
                if (sub_sink->should_log(msg.level)) {
                    sub_sink->log(msg);
                }
            }
        }

        void flush_() override {
            for (auto &sub_sink: sinks_) {
                sub_sink->flush();
            }
        }

        void set_pattern_(const std::string &pattern) override {
            set_formatter_(details::make_unique<collie::log::pattern_formatter>(pattern));
        }

        void set_formatter_(std::unique_ptr<collie::log::formatter> sink_formatter) override {
            base_sink<Mutex>::formatter_ = std::move(sink_formatter);
            for (auto &sub_sink: sinks_) {
                sub_sink->set_formatter(base_sink<Mutex>::formatter_->clone());
            }
        }

        std::vector<std::shared_ptr<sink>> sinks_;
    };

    using dist_sink_mt = dist_sink<std::mutex>;
    using dist_sink_st = dist_sink<details::null_mutex>;

}  // namespace collie::log::sinks
