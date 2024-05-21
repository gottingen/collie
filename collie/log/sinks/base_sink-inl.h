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

#include <collie/log/common.h>
#include <collie/log/pattern_formatter.h>

#include <memory>
#include <mutex>

template<typename Mutex>
inline collie::log::sinks::base_sink<Mutex>::base_sink()
        : formatter_{details::make_unique<collie::log::pattern_formatter>()} {}

template<typename Mutex>
inline collie::log::sinks::base_sink<Mutex>::base_sink(
        std::unique_ptr<collie::log::formatter> formatter)
        : formatter_{std::move(formatter)} {}

template<typename Mutex>
void inline collie::log::sinks::base_sink<Mutex>::log(const details::log_msg &msg) {
    std::lock_guard<Mutex> lock(mutex_);
    sink_it_(msg);
}

template<typename Mutex>
void inline collie::log::sinks::base_sink<Mutex>::flush() {
    std::lock_guard<Mutex> lock(mutex_);
    flush_();
}

template<typename Mutex>
void inline collie::log::sinks::base_sink<Mutex>::set_pattern(const std::string &pattern) {
    std::lock_guard<Mutex> lock(mutex_);
    set_pattern_(pattern);
}

template<typename Mutex>
void inline
collie::log::sinks::base_sink<Mutex>::set_formatter(std::unique_ptr<collie::log::formatter> sink_formatter) {
    std::lock_guard<Mutex> lock(mutex_);
    set_formatter_(std::move(sink_formatter));
}

template<typename Mutex>
void inline collie::log::sinks::base_sink<Mutex>::set_pattern_(const std::string &pattern) {
    set_formatter_(details::make_unique<collie::log::pattern_formatter>(pattern));
}

template<typename Mutex>
void inline
collie::log::sinks::base_sink<Mutex>::set_formatter_(std::unique_ptr<collie::log::formatter> sink_formatter) {
    formatter_ = std::move(sink_formatter);
}
