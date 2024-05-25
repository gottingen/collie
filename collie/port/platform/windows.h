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

#if defined(COLLIE_PLATFORM_WINDOWS)
#   error "can not define COLLIE_PLATFORM_WINDOWS manully"
#elif (defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)) && !defined(CS_UNDEFINED_STRING)
#   define COLLIE_PLATFORM_WINDOWS 1
#   define COLLIE_PLATFORM_NAME "Windows"
#   ifdef _WIN64 // VC++ defines both _WIN32 and _WIN64 when compiling for Win64.
#       define COLLIE_PLATFORM_WIN64 1
#   else
#       define COLLIE_PLATFORM_WIN32 1
#   endif
#   if defined(_M_AMD64) || defined(_AMD64_) || defined(__x86_64__)
#       define COLLIE_PROCESSOR_X86_64 1
#       define COLLIE_SYSTEM_LITTLE_ENDIAN 1
#       define COLLIE_PLATFORM_DESCRIPTION "Windows on x64"
#   elif defined(_M_IX86) || defined(_X86_)
#       define COLLIE_PROCESSOR_X86 1
#       define COLLIE_SYSTEM_LITTLE_ENDIAN 1
#       define COLLIE_PLATFORM_DESCRIPTION "Windows on X86"
#   elif defined(_M_IA64) || defined(_IA64_)
#       define COLLIE_PROCESSOR_IA64 1
#       define COLLIE_SYSTEM_LITTLE_ENDIAN 1
#d      efine COLLIE_PLATFORM_DESCRIPTION "Windows on IA-64"
#   elif defined(_M_ARM)
#       define COLLIE_ABI_ARM_WINCE 1
#       define COLLIE_PROCESSOR_ARM32 1
#       define COLLIE_SYSTEM_LITTLE_ENDIAN 1
#       define COLLIE_PLATFORM_DESCRIPTION "Windows on ARM"
#   elif defined(_M_ARM64)
#       define COLLIE_PROCESSOR_ARM64 1
#       define COLLIE_SYSTEM_LITTLE_ENDIAN 1
#       define COLLIE_PLATFORM_DESCRIPTION "Windows on ARM64"
#   else //Possibly other Windows CE variants
#       error Unknown processor
#   endif
#   if defined(__GNUC__)
#       define COLLIE_ASM_STYLE_ATT 1
#   elif defined(_MSC_VER) || defined(__BORLANDC__) || defined(__ICL)
#       define COLLIE_ASM_STYLE_INTEL 1
#   endif
#   define COLLIE_PLATFORM_DESKTOP 1
#   define COLLIE_PLATFORM_MICROSOFT 1

// WINAPI_FAMILY defines to support Windows 8 Metro Apps - mirroring winapifamily.h in the Windows 8 SDK
#   define COLLIE_WINAPI_FAMILY_APP         1000
#   define COLLIE_WINAPI_FAMILY_DESKTOP_APP 1001
#   define COLLIE_WINAPI_FAMILY_GAMES       1006

#   if defined(WINAPI_FAMILY)
#       if defined(_MSC_VER)
#           pragma warning(push, 0)
#       endif
#       include <winapifamily.h>
#       if defined(_MSC_VER)
#           pragma warning(pop)
#       endif
#       if defined(WINAPI_FAMILY_DESKTOP_APP) && WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#           define COLLIE_WINAPI_FAMILY COLLIE_WINAPI_FAMILY_DESKTOP_APP
#       elif defined(WINAPI_FAMILY_APP) && WINAPI_FAMILY == WINAPI_FAMILY_APP
#           define COLLIE_WINAPI_FAMILY COLLIE_WINAPI_FAMILY_APP
#       elif defined(WINAPI_FAMILY_GAMES) && WINAPI_FAMILY == WINAPI_FAMILY_GAMES
#           define COLLIE_WINAPI_FAMILY COLLIE_WINAPI_FAMILY_GAMES
#       else
#           error Unsupported WINAPI_FAMILY
#       endif
#   else
#       define COLLIE_WINAPI_FAMILY COLLIE_WINAPI_FAMILY_DESKTOP_APP
#   endif

#   define COLLIE_WINAPI_PARTITION_DESKTOP   1
#   define COLLIE_WINAPI_PARTITION_APP       1
#   define COLLIE_WINAPI_PARTITION_GAMES    (COLLIE_WINAPI_FAMILY == COLLIE_WINAPI_FAMILY_GAMES)

#   define COLLIE_WINAPI_FAMILY_PARTITION(Partition)   (Partition)

// COLLIE_PLATFORM_WINRT
// This is a subset of Windows which is used for tablets and the "Metro" (restricted) Windows user interface.
// WinRT doesn't doesn't have access to the Windows "desktop" API, but WinRT can nevertheless run on
// desktop computers in addition to tablets. The Windows Phone API is a subset of WinRT and is not included
// in it due to it being only a part of the API.
#   if defined(__cplusplus_winrt)
#       define COLLIE_PLATFORM_WINRT 1
#   endif
#endif