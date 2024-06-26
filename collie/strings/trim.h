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


#ifndef COLLIE_STRINGS_TRIM_H_
#define COLLIE_STRINGS_TRIM_H_

#include <collie/strings/ascii.h>
#include <collie/strings/case_conv.h>
#include <collie/strings/match.h>

namespace collie {

    /**
     * @ingroup collie_strings_trim
     * @brief operator()() is used to check if a character is in the given string.
     */
    struct by_any_of {
        explicit by_any_of(std::string_view str) : trimmer(str) {

        }

        bool operator()(char c) {
            return trimmer.find(c) != std::string_view::npos;
        }

        bool operator()(unsigned char c) {
            return trimmer.find(c) != std::string_view::npos;
        }

    private:
        std::string_view trimmer;
    };

    /**
     * @ingroup collie_strings_trim
     * @brief operator()() is used to check if a character is a whitespace.
     */
    struct by_white_space {
        by_white_space() = default;

        bool operator()(unsigned char c) {
            return ascii_is_space(c);
        }
    };

    /**
     * @ingroup collie_strings_trim
     * @brief trim_left() removes whitespace from the beginning of the given string.
     *         pred defaults to by_white_space.
     *         Example:
     *         @code
     *         std::string_view input("\t abc");
     *         EXPECT_EQ(collie::trim_left(input), "abc");
     *         EXPECT_EQ(collie::trim_left(input, collie::by_any_of("\t a")), "bc");
     *         @endcode
     * @param str The string to trim.
     * @param pred The predicate to use to determine if a character should be trimmed.
     *        Defaults to by_white_space.
     * @return A std::string_view with whitespace stripped from the beginning of the
     *         given std::string_view.
     */
    template<typename Pred = by_white_space>
    [[nodiscard]] inline std::string_view trim_left(std::string_view str, Pred pred = Pred()) {
        auto it = std::find_if_not(str.begin(), str.end(), pred);
        return str.substr(static_cast<size_t>(it - str.begin()));
    }

    /**
     * @ingroup collie_strings_trim
     * @brief trim_left() removes specified characters that match the predicate from the beginning of the given string.
     *        pred defaults to by_white_space.
     *        Example:
     *        @code
     *        std::string input("\t abc");
     *        collie::trim_left(&input);
     *        EXPECT_EQ(input, "abc");
     *        std::string input2("\t abc");
     *        collie::trim_left(&input2, collie::by_any_of("\t a"));
     *        EXPECT_EQ(input2, "bc");
     *        @endcode
     * @attention This function will modify the given string.
     * @param str The string to trim.
     * @param pred The predicate to use to determine if a character should be trimmed.
     *       Defaults to by_white_space.
     */
    template<typename Pred = by_white_space>
    inline void
    trim_left(std::string *str, Pred pred = Pred()) {
        auto it = std::find_if_not(str->begin(), str->end(), pred);
        str->erase(str->begin(), it);
    }

    /**
     * @ingroup collie_strings_trim
     * @brief trim_right() removes whitespace from the end of the given string.
     *        pred defaults to by_white_space.
     *        Example:
     *        @code
     *        std::string_view input("abc \t");
     *        EXPECT_EQ(collie::trim_right(input), "abc");
     *        EXPECT_EQ(collie::trim_right(input, collie::by_any_of("\t a")), "abc");
     *        @endcode
     * @param str The string to trim.
     * @param pred The predicate to use to determine if a character should be trimmed.
     *        Defaults to by_white_space.
     * @return A std::string_view with whitespace stripped from the end of the
     *         given std::string_view.
     */
    template<typename Pred = by_white_space>
    [[nodiscard]] inline std::string_view trim_right(std::string_view str, Pred pred = Pred()) {
        auto it = std::find_if_not(str.rbegin(), str.rend(), pred);
        return str.substr(0, static_cast<size_t>(str.rend() - it));
    }

    /**
     * @ingroup collie_strings_trim
     * @brief trim_right() removes specified characters that match the predicate from the end of the given string.
     *        pred defaults to by_white_space.
     *        Example:
     *        @code
     *        std::string input("abc \t");
     *        collie::trim_right(&input);
     *        EXPECT_EQ(input, "abc");
     *        std::string input2("abc \t");
     *        collie::trim_right(&input2, collie::by_any_of("\t a"));
     *        EXPECT_EQ(input2, "abc");
     *        @endcode
     * @attention This function will modify the given string.
     * @param str The string to trim.
     * @param pred The predicate to use to determine if a character should be trimmed.
     *        Defaults to by_white_space.
     */
    template<typename Pred = by_white_space>
    inline void trim_right(std::string *str, Pred pred = Pred()) {
        auto it = std::find_if_not(str->rbegin(), str->rend(), pred);
        str->erase(static_cast<size_t>(str->rend() - it));
    }

    /**
     * @ingroup collie_strings_trim
     * @brief trim_all() removes specified characters that match the predicate from both ends of the given string.
     *        pred defaults to by_white_space.
     *        Example:
     *        @code
     *        std::string_view input(" \t abc \t");
     *        EXPECT_EQ(collie::trim_all(input), "abc");
     *        EXPECT_EQ(collie::trim_all(input, collie::by_any_of("\t a")), "bc");
     *        @endcode
     * @param str The string to trim.
     * @param pred The predicate to use to determine if a character should be trimmed.
     *       Defaults to by_white_space.
     * @return A std::string_view with whitespace stripped from both ends of the
     *        given std::string_view.
     */
    template<typename Pred = by_white_space>
    [[nodiscard]] inline std::string_view trim_all(std::string_view str, Pred pred = Pred()) {
        return trim_right(trim_left(str, pred), pred);
    }

    /**
     * @ingroup collie_strings_trim
     * @brief trim_all() removes specified characters that match the predicate from both ends of the given string.
     *        pred defaults to by_white_space.
     *        Example:
     *        @code
     *        std::string input(" \t abc \t");
     *        collie::trim_all(&input);
     *        EXPECT_EQ(input, "abc");
     *        std::string input2(" \t abc \t");
     *        collie::trim_all(&input2, collie::by_any_of("\t a"));
     *        EXPECT_EQ(input2, "bc");
     *        @endcode
     * @attention This function will modify the given string.
     * @param str The string to trim.
     * @param pred The predicate to use to determine if a character should be trimmed.
     *        Defaults to by_white_space.
     */
    template<typename Pred = by_white_space>
    inline void
    trim_all(std::string *str, Pred pred = Pred()) {
        trim_right(str, pred);
        trim_left(str, pred);
    }

    /**
     * @ingroup collie_strings_trim
     * @brief trim_complete() removes leading, trailing, and consecutive internal whitespace.
     *        Example:
     *        @code
     *        std::string input(" \t ab c \t");
     *        collie::trim_complete(&input);
     *        EXPECT_EQ(input, "abc");
     *        @endcode
     * @attention This function will modify the given string.
     * @param str The string to trim.
     */
    inline void trim_complete(std::string *str) {
        auto stripped = trim_all(*str);

        if (stripped.empty()) {
            str->clear();
            return;
        }

        auto input_it = stripped.begin();
        auto input_end = stripped.end();
        auto output_it = &(*str)[0];
        bool is_ws = false;

        for (; input_it < input_end; ++input_it) {
            if (is_ws) {
                // Consecutive whitespace?  Keep only the last.
                is_ws = ascii_is_space(static_cast<unsigned char>(*input_it));
                if (is_ws) --output_it;
            } else {
                is_ws = ascii_is_space(static_cast<unsigned char>(*input_it));
            }

            *output_it = *input_it;
            ++output_it;
        }

        str->erase(static_cast<size_t>(output_it - &(*str)[0]));
    }

    inline bool consume_prefix(std::string_view *str, std::string_view expected) {
        if (!starts_with(*str, expected)) return false;
        str->remove_prefix(expected.size());
        return true;
    }

    inline bool consume_suffix(std::string_view *str, std::string_view expected) {
        if (!ends_with(*str, expected)) return false;
        str->remove_suffix(expected.size());
        return true;
    }

    [[nodiscard]] inline std::string_view strip_prefix(
            std::string_view str, std::string_view prefix) {
        if (starts_with(str, prefix)) str.remove_prefix(prefix.size());
        return str;
    }

    [[nodiscard]] inline std::string_view strip_suffix(
            std::string_view str, std::string_view suffix) {
        if (ends_with(str, suffix)) str.remove_suffix(suffix.size());
        return str;
    }

}  // namespace collie
#endif  // COLLIE_STRINGS_TRIM_H_
