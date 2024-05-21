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

#ifndef COLLIE_MODULE_LIB_PATH_H_
#define COLLIE_MODULE_LIB_PATH_H_

#include <string>
#include <string_view>
#include <collie/filesystem/fs.h>
#include <collie/module/semver.h>
#include <collie/strings/fmt/format.h>

namespace collie {


    inline constexpr std::string_view lib_linux_suffix() {
        return ".so";
    }

    inline constexpr std::string_view lib_macos_suffix() {
        return ".dylib";
    }

    inline constexpr std::string_view lib_windows_suffix() {
        return ".dll";
    }

    inline constexpr std::string_view lib_suffix() {
#if defined(__linux__)
        return lib_linux_suffix();
#elif defined(__APPLE__)
        return lib_macos_suffix();
#elif defined(_WIN32)
        return lib_windows_suffix();
#else
        return "";
#endif
    }

    std::string make_full_version(ModuleVersion version) {
        return fmt::format("{}.{}.{}", version.major, version.minor, version.patch);
    }

    std::string make_major_minor_version(ModuleVersion version) {
        return fmt::format("{}.{}", version.major, version.minor);
    }

    std::string make_major_version(ModuleVersion version) {
        return fmt::format("{}", version.major);
    }

    /**
     * @brief Make a library name for the given module name and version.
     * @param name The module name.
     * @param version The module version.
     *        @code
     *        ModuleVersion version(1, 2, 3);
     *        auto libname = make_libname("my_module", version);
     *        // libname == "libmy_module.1.2.3.dylib"
     *        @endcode
     * @return The library name.
     */
    inline std::string make_mac_libname(std::string_view name, std::string_view version) {
        return fmt::format("lib{}.{}.dylib", name, version);
    }

    inline std::string make_linux_libname(std::string_view name, std::string_view version) {
        return fmt::format("lib{}.so.{}", name, version);
    }

    inline std::string make_windows_libname(std::string_view name, std::string_view version) {
        return fmt::format("{}.{}.dll", name, version);
    }

    inline std::string make_libname(std::string_view name, std::string_view version) {
#if defined(__linux__)
        return make_linux_libname(name, version);
#elif defined(__APPLE__)
        return make_mac_libname(name, version);
#elif defined(_WIN32)
        return make_windows_libname(name, version);
#else
        return "";
#endif
    }

    inline std::string make_full_libname(std::string_view name, ModuleVersion version) {
        return make_libname(name, make_full_version(version));
    }

    inline std::string make_major_minor_libname(std::string_view name, ModuleVersion version) {
        return make_libname(name, make_major_minor_version(version));
    }

    inline std::string make_major_libname(std::string_view name, ModuleVersion version) {
        return make_libname(name, make_major_version(version));
    }

    inline std::string make_libname(std::string_view name, ModuleVersion version) {
        return make_libname(name, make_full_version(version));
    }


}  // namespace collie
#endif  // COLLIE_MODULE_LIB_PATH_H_
