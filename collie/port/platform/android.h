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

#if defined(COLLIE_PLATFORM_ANDROID)
#   error "can not define COLLIE_PLATFORM_ANDROID manully"
#elif defined(__ANDROID__)
#   define COLLIE_PLATFORM_ANDROID 1
#   define COLLIE_PLATFORM_LINUX 1
#   define COLLIE_PLATFORM_UNIX 1
#   define COLLIE_PLATFORM_POSIX 1
#   define COLLIE_PLATFORM_NAME "Android"
#   define COLLIE_ASM_STYLE_ATT 1
#   if defined(__arm__)
#       define COLLIE_ABI_ARM_LINUX 1  // a.k.a. "ARM eabi"
#       define COLLIE_PROCESSOR_ARM32 1
#       define COLLIE_PLATFORM_DESCRIPTION "Android on ARM"
#   elif defined(__aarch64__)
#       define COLLIE_PROCESSOR_ARM64 1
#       define COLLIE_PLATFORM_DESCRIPTION "Android on ARM64"
#   elif defined(__i386__)
#       define COLLIE_PROCESSOR_X86 1
#       define COLLIE_PLATFORM_DESCRIPTION "Android on x86"
#   elif defined(__x86_64)
#       define COLLIE_PROCESSOR_X86_64 1
#       define COLLIE_PLATFORM_DESCRIPTION "Android on x64"
#   else
#       error Unknown processor
#   endif
#   define COLLIE_PLATFORM_MOBILE 1
#endif