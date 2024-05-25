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

inline bool collie::log::sinks::sink::should_log(collie::log::level::level_enum msg_level) const {
    return msg_level >= level_.load(std::memory_order_relaxed);
}

inline void collie::log::sinks::sink::set_level(level::level_enum log_level) {
    level_.store(log_level, std::memory_order_relaxed);
}

inline collie::log::level::level_enum collie::log::sinks::sink::level() const {
    return static_cast<collie::log::level::level_enum>(level_.load(std::memory_order_relaxed));
}
