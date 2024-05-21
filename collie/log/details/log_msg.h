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
#include <string>

namespace collie::log {
    namespace details {
        struct log_msg {
            log_msg() = default;

            log_msg(log_clock::time_point log_time,
                    source_loc loc,
                    string_view_t logger_name,
                    level::level_enum lvl,
                    string_view_t msg);

            log_msg(source_loc loc, string_view_t logger_name, level::level_enum lvl, string_view_t msg);

            log_msg(string_view_t logger_name, level::level_enum lvl, string_view_t msg);

            log_msg(const log_msg &other) = default;

            log_msg &operator=(const log_msg &other) = default;

            string_view_t logger_name;
            level::level_enum level{level::off};
            log_clock::time_point time;
            size_t thread_id{0};

            // wrapping the formatted text with color (updated by pattern_formatter).
            mutable size_t color_range_start{0};
            mutable size_t color_range_end{0};

            source_loc source;
            string_view_t payload;
        };
    }  // namespace details
}  // namespace collie::log

#include <collie/log/details/log_msg-inl.h>
