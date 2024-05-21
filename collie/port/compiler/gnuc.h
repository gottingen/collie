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


#if defined(COLLIE_COMPILER_GNUC)
#   error "can not define COLLIE_COMPILER_GNUC manully"
#elif defined(__GNUC__) // GCC compilers exist for many platforms.
#   define COLLIE_COMPILER_GNUC    1
#   define COLLIE_COMPILER_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#   define COLLIE_COMPILER_NAME    "GCC"
#   define COLLIE_GNUC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#else
#   define COLLIE_GNUC_VERSION (0)
#endif