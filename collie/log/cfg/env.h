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
#include <collie/log/details/os.h>
#include <collie/log/details/registry.h>

//
// Init levels and patterns from env variables CLOG_LEVEL
// Inspired from Rust's "env_logger" crate (https://crates.io/crates/env_logger).
// Note - fallback to "info" level on unrecognized levels
//
// Examples:
//
// set global level to debug:
// export CLOG_LEVEL=debug
//
// turn off all logging except for logger1:
// export CLOG_LEVEL="*=off,logger1=debug"
//

// turn off all logging except for logger1 and logger2:
// export CLOG_LEVEL="off,logger1=debug,logger2=info"

namespace collie::log {
namespace cfg {
inline void load_env_levels() {
    auto env_val = details::os::getenv("CLOG_LEVEL");
    if (!env_val.empty()) {
        helpers::load_levels(env_val);
    }
}

}  // namespace cfg
}  // namespace collie::log
