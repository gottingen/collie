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


#ifndef COLLIE_STRINGS_CAT_H_
#define COLLIE_STRINGS_CAT_H_

#include <string>
#include <vector>
#include <string_view>
#include <collie/strings/fmt/format.h>

namespace collie {

    template <typename T>
    std::string str_cat(const T &t) {
        return fmt::format("{}", t);
    }

    template <typename T, typename... Args>
    std::string str_cat(const T &t, const Args &... args) {
        return fmt::format("{}{}", t, str_cat(args...));
    }

    template <typename... Args>
    std::string str_cat(const std::vector<std::string> &vec, const Args &... args) {
        std::string result;
        for (const auto &s : vec) {
            result += s;
        }
        return str_cat(result, args...);
    }

    template <typename... Args>
    std::string str_cat(const std::vector<std::string_view> &vec, const Args &... args) {
        std::string result;
        for (const auto &s : vec) {
            result += s;
        }
        return str_cat(result, args...);
    }

    template <typename T>
    std::string &str_cat_append(std::string &result, const T &t) {
        result += fmt::format("{}", t);
        return result;
    }

    template <typename T, typename... Args>
    std::string &str_cat_append(std::string &result, const T &t, const Args &... args) {
        result += fmt::format("{}{}", t, str_cat(args...));
        return result;
    }

    template <typename... Args>
    std::string &str_cat_append(std::string &result, const std::vector<std::string> &vec, const Args &... args) {
        for (const auto &s : vec) {
            result += s;
        }
        return str_cat(result, args...);
    }

    template <typename... Args>
    std::string &str_cat_append(std::string &result, const std::vector<std::string_view> &vec, const Args &... args) {
        for (const auto &s : vec) {
            result += s;
        }
        return str_cat(result, args...);
    }
}  // namespace collie

#endif  // COLLIE_STRINGS_CAT_H_
