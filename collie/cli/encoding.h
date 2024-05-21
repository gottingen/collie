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

#include <collie/cli/macros.h>
#include <collie/filesystem/fs.h>
#include <string>
#include <string_view>

namespace collie {

    /// Convert a wide string to a narrow string.
    inline std::string narrow(const std::wstring &str);
    inline std::string narrow(const wchar_t *str);
    inline std::string narrow(const wchar_t *str, std::size_t size);

    /// Convert a narrow string to a wide string.
    inline std::wstring widen(const std::string &str);
    inline std::wstring widen(const char *str);
    inline std::wstring widen(const char *str, std::size_t size);

    inline std::string narrow(std::wstring_view str);
    inline std::wstring widen(std::string_view str);

    /// Convert a char-string to a native path correctly.
    inline collie::filesystem::path to_path(std::string_view str);

}  // namespace collie

#include <collie/cli/impl/encoding_inl.h>
