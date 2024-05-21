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

#ifdef _WIN32
#include <collie/log/sinks/wincolor_sink.h>
#else

#include <collie/log/sinks/ansicolor_sink.h>

#endif

#include <collie/log/details/synchronous_factory.h>

namespace collie::log {
    namespace sinks {
#ifdef _WIN32
        using stdout_color_sink_mt = wincolor_stdout_sink_mt;
        using stdout_color_sink_st = wincolor_stdout_sink_st;
        using stderr_color_sink_mt = wincolor_stderr_sink_mt;
        using stderr_color_sink_st = wincolor_stderr_sink_st;
#else
        using stdout_color_sink_mt = ansicolor_stdout_sink_mt;
        using stdout_color_sink_st = ansicolor_stdout_sink_st;
        using stderr_color_sink_mt = ansicolor_stderr_sink_mt;
        using stderr_color_sink_st = ansicolor_stderr_sink_st;
#endif
    }  // namespace sinks

    template<typename Factory = collie::log::synchronous_factory>
    std::shared_ptr<logger> stdout_color_mt(const std::string &logger_name,
                                            color_mode mode = color_mode::automatic);

    template<typename Factory = collie::log::synchronous_factory>
    std::shared_ptr<logger> stdout_color_st(const std::string &logger_name,
                                            color_mode mode = color_mode::automatic);

    template<typename Factory = collie::log::synchronous_factory>
    std::shared_ptr<logger> stderr_color_mt(const std::string &logger_name,
                                            color_mode mode = color_mode::automatic);

    template<typename Factory = collie::log::synchronous_factory>
    std::shared_ptr<logger> stderr_color_st(const std::string &logger_name,
                                            color_mode mode = color_mode::automatic);

}  // namespace collie::log

#include <collie/log/sinks/stdout_color_sinks-inl.h>
