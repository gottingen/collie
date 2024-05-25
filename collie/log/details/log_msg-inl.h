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


#include <collie/log/details/os.h>

namespace collie::log {
namespace details {

inline log_msg::log_msg(collie::log::log_clock::time_point log_time,
                               collie::log::source_loc loc,
                               string_view_t a_logger_name,
                               collie::log::level::level_enum lvl,
                               collie::log::string_view_t msg)
    : logger_name(a_logger_name),
      level(lvl),
      time(log_time)
#ifndef CLOG_NO_THREAD_ID
      ,
      thread_id(os::thread_id())
#endif
      ,
      source(loc),
      payload(msg) {
}

inline log_msg::log_msg(collie::log::source_loc loc,
                               string_view_t a_logger_name,
                               collie::log::level::level_enum lvl,
                               collie::log::string_view_t msg)
    : log_msg(os::now(), loc, a_logger_name, lvl, msg) {}

inline log_msg::log_msg(string_view_t a_logger_name,
                               collie::log::level::level_enum lvl,
                               collie::log::string_view_t msg)
    : log_msg(os::now(), source_loc{}, a_logger_name, lvl, msg) {}

}  // namespace details
}  // namespace collie::log
