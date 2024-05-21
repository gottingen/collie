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

#include <algorithm>
#include <iterator>

namespace collie::log {
    namespace level {

        constexpr static string_view_t level_string_views[]CLOG_LEVEL_NAMES;

        static const char *short_level_names[]CLOG_SHORT_LEVEL_NAMES;

        inline const string_view_t &to_string_view(collie::log::level::level_enum l) noexcept {
            return level_string_views[l];
        }

        inline const char *to_short_c_str(collie::log::level::level_enum l) noexcept {
            return short_level_names[l];
        }

        inline collie::log::level::level_enum from_str(const std::string &name) noexcept {
            auto it = std::find(std::begin(level_string_views), std::end(level_string_views), name);
            if (it != std::end(level_string_views))
                return static_cast<level::level_enum>(std::distance(std::begin(level_string_views), it));

            // check also for "warn" and "err" before giving up..
            if (name == "warn") {
                return level::warn;
            }
            if (name == "err") {
                return level::error;
            }
            return level::off;
        }
    }  // namespace level

    inline CLogEx::CLogEx(std::string msg)
            : msg_(std::move(msg)) {}

    inline CLogEx::CLogEx(const std::string &msg, int last_errno) {
        memory_buf_t outbuf;
        fmt::format_system_error(outbuf, last_errno, msg.c_str());
        msg_ = fmt::to_string(outbuf);
    }

    inline const char *CLogEx::what() const noexcept { return msg_.c_str(); }

    inline void throw_clog_ex(const std::string &msg, int last_errno) {
        CLOG_THROW(CLogEx(msg, last_errno));
    }

    inline void throw_clog_ex(std::string msg) { CLOG_THROW(CLogEx(std::move(msg))); }

}  // namespace collie::log
