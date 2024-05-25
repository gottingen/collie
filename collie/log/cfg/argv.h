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
#include <collie/log/cfg/helpers.h>
#include <collie/log/details/registry.h>

//
// Init log levels using each argv entry that starts with "CLOG_LEVEL="
//
// set all loggers to debug level:
// example.exe "CLOG_LEVEL=debug"

// set logger1 to trace level
// example.exe "CLOG_LEVEL=logger1=trace"

// turn off all logging except for logger1 and logger2:
// example.exe "CLOG_LEVEL=off,logger1=debug,logger2=info"

namespace collie::log {
namespace cfg {

// search for CLOG_LEVEL= in the args and use it to init the levels
inline void load_argv_levels(int argc, const char **argv) {
    const std::string clog_level_prefix = "CLOG_LEVEL=";
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.find(clog_level_prefix) == 0) {
            auto levels_string = arg.substr(clog_level_prefix.size());
            helpers::load_levels(levels_string);
        }
    }
}

inline void load_argv_levels(int argc, char **argv) {
    load_argv_levels(argc, const_cast<const char **>(argv));
}

}  // namespace cfg
}  // namespace collie::log
