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
#include <collie/log/logger.h>

namespace collie::log {

template <typename Factory>
inline std::shared_ptr<logger> stdout_color_mt(const std::string &logger_name,
                                                      color_mode mode) {
    return Factory::template create<sinks::stdout_color_sink_mt>(logger_name, mode);
}

template <typename Factory>
inline std::shared_ptr<logger> stdout_color_st(const std::string &logger_name,
                                                      color_mode mode) {
    return Factory::template create<sinks::stdout_color_sink_st>(logger_name, mode);
}

template <typename Factory>
inline std::shared_ptr<logger> stderr_color_mt(const std::string &logger_name,
                                                      color_mode mode) {
    return Factory::template create<sinks::stderr_color_sink_mt>(logger_name, mode);
}

template <typename Factory>
inline std::shared_ptr<logger> stderr_color_st(const std::string &logger_name,
                                                      color_mode mode) {
    return Factory::template create<sinks::stderr_color_sink_st>(logger_name, mode);
}
}  // namespace collie::log
