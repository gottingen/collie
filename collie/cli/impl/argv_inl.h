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

// This include is only needed for IDEs to discover symbols
#include <collie/cli/argv.h>

#include <collie/cli/encoding.h>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#if defined(_WIN32)
#if !(defined(_AMD64_) || defined(_X86_) || defined(_ARM_))
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) ||           \
    defined(_M_AMD64)
#define _AMD64_
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__i386__) || defined(_M_IX86)
#define _X86_
#elif defined(__arm__) || defined(_M_ARM) || defined(_M_ARMT)
#define _ARM_
#elif defined(__aarch64__) || defined(_M_ARM64)
#define _ARM64_
#elif defined(_M_ARM64EC)
#define _ARM64EC_
#endif
#endif

// first
#ifndef NOMINMAX
// if NOMINMAX is already defined we don't want to mess with that either way
#define NOMINMAX
#include <windef.h>
#undef NOMINMAX
#else
#include <windef.h>
#endif

// second
#include <winbase.h>
// third
#include <processthreadsapi.h>
#include <shellapi.h>
#endif

namespace collie::detail {

#ifdef _WIN32
inline std::vector<std::string> compute_win32_argv() {
    std::vector<std::string> result;
    int argc = 0;

    auto deleter = [](wchar_t **ptr) { LocalFree(ptr); };
    // NOLINTBEGIN(*-avoid-c-arrays)
    auto wargv = std::unique_ptr<wchar_t *[], decltype(deleter)>(CommandLineToArgvW(GetCommandLineW(), &argc), deleter);
    // NOLINTEND(*-avoid-c-arrays)

    if(wargv == nullptr) {
        throw std::runtime_error("CommandLineToArgvW failed with code " + std::to_string(GetLastError()));
    }

    result.reserve(static_cast<size_t>(argc));
    for(size_t i = 0; i < static_cast<size_t>(argc); ++i) {
        result.push_back(narrow(wargv[i]));
    }

    return result;
}
#endif
}  // namespace collie::detail
