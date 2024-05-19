// Copyright 2024 The Elastic-AI Authors.
// part of Elastic AI Search
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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
