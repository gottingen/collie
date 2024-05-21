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

#ifndef COLLIE_STRINGS_CASE_CONV_H_
#define COLLIE_STRINGS_CASE_CONV_H_

#include <string>
#include <string_view>
#include <collie/strings/ascii.h>
#include <collie/strings/inlined_string.h>

namespace collie {

    template<typename String>
    struct is_string_type : std::false_type {};

    template<>
    struct is_string_type<std::string> : std::true_type {};

    template<unsigned N>
    struct is_string_type<collie::InlinedString<N>> : std::true_type {};

    /**
     * @ingroup collie_string_convert
     * @brief Converts the characters in `s` to lowercase, changing the contents of `s`.
     *        This function is a no-op if `String` is not a string type.
     *        Example:
     *        @code
     *        std::string s = "Hello World!";
     *        collie::str_to_lower(&s);
     *        assert(s == "hello world!");
     *        @endcode
     * @param s The string to convert to lowercase.
     */
    template<typename String>
    inline typename std::enable_if_t<is_string_type<String>::value> str_to_lower(String *s) {
        if (s->empty()) {
            return;
        }
        for (auto &c : *s) {
            c = collie::ascii_to_lower(c);
        }
    }

    /**
     * @ingroup collie_string_convert
     * @brief Creates a lowercase string from a given std::string_view.
     *        String is a string type.
     *        Example:
     *        @code
     *        std::string s = "Hello World!";
     *        std::string s1 = collie::str_to_lower(s);
     *        assert(s1 == "hello world!");
     *        @endcode
     * @param s The string to convert to lowercase.
     */
    inline std::string str_to_lower(std::string_view s) {
        std::string result;
        result.reserve(s.size());
        for (auto c : s) {
            result.append(1, collie::ascii_to_lower(c));
        }
        return result;
    }

    /**
     * @ingroup collie_string_convert
     * @brief str_to_upper Converts the characters in `s` to uppercase, changing the contents of `s`.
     *        This function is a no-op if `String` is not a string type.
     *        Example:
     *        @code
     *        std::string s = "Hello World!";
     *        collie::str_to_upper(&s);
     *        assert(s == "HELLO WORLD!");
     *        @endcode
     * @param s The string to convert to uppercase.
     */
    template<typename String>
    typename std::enable_if_t<is_string_type<String>::value> str_to_upper(String *s) {
        if (s->empty()) {
            return;
        }
        for (auto &c : *s) {
            c = collie::ascii_to_upper(c);
        }
    }

    /**
     * @ingroup collie_string_convert
     * @brief Creates an uppercase string from a given std::string_view.
     *        String is a string type.
     *        Example:
     *        @code
     *        std::string s = "Hello World!";
     *        std::string s1 = collie::str_to_upper(s);
     *        assert(s1 == "HELLO WORLD!");
     *        @endcode
     * @param s The string to convert to uppercase.
     */
    inline std::string str_to_upper(std::string_view s) {
          std::string result;
            result.reserve(s.size());
            for (auto c : s) {
                result.append(1, collie::ascii_to_upper(c));
            }
            return result;
    }

}  // namespace collie

#endif  // COLLIE_STRINGS_CASE_CONV_H_
