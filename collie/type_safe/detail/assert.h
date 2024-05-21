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
//

#pragma once

#include <collie/base/debug_assert.h>
#include <collie/type_safe/config.h>

namespace collie::ts::detail {

    struct assert_handler : collie::debug_assert::set_level<TYPE_SAFE_ENABLE_ASSERTIONS>,
                            collie::debug_assert::default_handler {
    };

    struct precondition_error_handler
            : collie::debug_assert::set_level<TYPE_SAFE_ENABLE_PRECONDITION_CHECKS>,
              collie::debug_assert::default_handler {
    };

    inline void on_disabled_exception() noexcept {
        struct handler : collie::debug_assert::set_level<1>, collie::debug_assert::default_handler {
        };
        DEBUG_UNREACHABLE(handler{}, "attempt to throw an exception but exceptions are disabled");
    }
} // namespace collie::ts::detail
