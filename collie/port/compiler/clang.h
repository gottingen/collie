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

#if defined(COLLIE_COMPILER_CLANG)
#   error "can not define COLLIE_COMPILER_GNUC manully"
#elif defined(__clang__) && !defined(_MSC_VER)
#   define COLLIE_COMPILER_CLANG   1
#   define COLLIE_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)
#   define COLLIE_COMPILER_NAME    "clang"
#   define COLLIE_CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
#else
#   define COLLIE_CLANG_VERSION (0)
#endif

