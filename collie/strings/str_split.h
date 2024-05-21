//
// Copyright 2022 The Turbo Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// -----------------------------------------------------------------------------
// File: str_split.h
// -----------------------------------------------------------------------------
//
// This file contains functions for splitting strings. It defines the main
// `str_split()` function, several delimiters for determining the boundaries on
// which to split the string, and predicates for filtering delimited results.
// `str_split()` adapts the returned collection to the type specified by the
// caller.
//
// Example:
//
//   // Splits the given string on commas. Returns the results in a
//   // vector of strings.
//   std::vector<std::string> v = collie::str_split("a,b,c", ',');
//   // Can also use ","
//   // v[0] == "a", v[1] == "b", v[2] == "c"
//
// See str_split() below for more information.

#pragma once

#include <algorithm>
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <string_view>
#include <collie/strings/internal/str_split_internal.h>
#include <collie/strings/trim.h>

namespace collie {

    //------------------------------------------------------------------------------
    // Delimiters
    //------------------------------------------------------------------------------
    //
    // `str_split()` uses delimiters to define the boundaries between elements in the
    // provided input. Several `Delimiter` types are defined below. If a string
    // (`const char*`, `std::string`, or `std::string_view`) is passed in place of
    // an explicit `Delimiter` object, `str_split()` treats it the same way as if it
    // were passed a `ByString` delimiter.
    //
    // A `Delimiter` is an object with a `Find()` function that knows how to find
    // the first occurrence of itself in a given `std::string_view`.
    //
    // The following `Delimiter` types are available for use within `str_split()`:
    //
    //   - `ByString` (default for string arguments)
    //   - `ByChar` (default for a char argument)
    //   - `ByAnyChar`
    //   - `ByLength`
    //   - `max_splits`
    //
    // A Delimiter's `Find()` member function will be passed an input `text` that is
    // to be split and a position (`pos`) to begin searching for the next delimiter
    // in `text`. The returned std::string_view should refer to the next occurrence
    // (after `pos`) of the represented delimiter; this returned std::string_view
    // represents the next location where the input `text` should be broken.
    //
    // The returned std::string_view may be zero-length if the Delimiter does not
    // represent a part of the string (e.g., a fixed-length delimiter). If no
    // delimiter is found in the input `text`, a zero-length std::string_view
    // referring to `text.end()` should be returned (e.g.,
    // `text.substr(text.size())`). It is important that the returned
    // std::string_view always be within the bounds of the input `text` given as an
    // argument--it must not refer to a string that is physically located outside of
    // the given string.
    //
    // The following example is a simple Delimiter object that is created with a
    // single char and will look for that char in the text passed to the `Find()`
    // function:
    //
    //   struct SimpleDelimiter {
    //     const char c_;
    //     explicit SimpleDelimiter(char c) : c_(c) {}
    //     std::string_view Find(std::string_view text, size_t pos) {
    //       auto found = text.find(c_, pos);
    //       if (found == std::string_view::npos)
    //         return text.substr(text.size());
    //
    //       return text.substr(found, 1);
    //     }
    //   };

    /**
     * @ingroup collie_strings_split
     * @file str_split.h
     * @brief Contains functions for splitting strings.
     */

    /**
     * @ingroup collie_strings_split
     * @brief A sub-string delimiter.
     * @details If `str_split()` is passed a string in place of a
     *             `Delimiter` object, the string will be implicitly converted into a
     *             `ByString` delimiter.
     *             Example:
     *             @code
     *             // Because a string literal is converted to an `collie::ByString`,
     *             // the following two splits are equivalent.
     *             std::vector<std::string> v1 = collie::str_split("a, b, c", ", ");
     *             using collie::ByString;
     *             std::vector<std::string> v2 = collie::str_split("a, b, c", ByString(", "));
     *             // v[0] == "a", v[1] == "b", v[2] == "c"
     *             @endcode
     *             @see ByChar
     *             @see ByAnyChar
     *             @see ByLength
     *             @see max_splits
     * @param sp The delimiter string.
     *
     */
    class ByString {
    public:
        explicit ByString(std::string_view sp);

        [[nodiscard]] std::string_view Find(std::string_view text, size_t pos) const;

    private:
        const std::string delimiter_;
    };

    /**
     * @ingroup collie_strings_split
     * @brief A single character delimiter.
     * @details `ByChar` is functionally equivalent to a
     *             1-char string within a `ByString` delimiter, but slightly more efficient.
     *             Example:
     *             @code
     *             // Because a char literal is converted to a collie::ByChar,
     *             // the following two splits are equivalent.
     *             std::vector<std::string> v1 = collie::str_split("a,b,c", ',');
     *             using collie::ByChar;
     *             std::vector<std::string> v2 = collie::str_split("a,b,c", ByChar(','));
     *             // v[0] == "a", v[1] == "b", v[2] == "c"
     *             @endcode
     *             `ByChar` is also the default delimiter if a single character is given
     *             as the delimiter to `str_split()`. For example, the following calls are
     *             equivalent:
     *             @code
     *             std::vector<std::string> v = collie::str_split("a-b", '-');
     *             using collie::ByChar;
     *             std::vector<std::string> v = collie::str_split("a-b", ByChar('-'));
     *             @endcode
     * @param c The delimiter character.
     *
     */
    class ByChar {
    public:
        explicit ByChar(char c) : c_(c) {}

        [[nodiscard]] std::string_view Find(std::string_view text, size_t pos) const;

    private:
        char c_;
    };

    /**
     * @ingroup collie_strings_split
     * @brief A delimiter that will match any of the given byte-sized characters within
     *        its provided string.
     *        Example:
     *        @code
     *        using collie::ByAnyChar;
     *        std::vector<std::string> v = collie::str_split("a,b=c", ByAnyChar(",="));
     *        // v[0] == "a", v[1] == "b", v[2] == "c"
     *        @endcode
     *        If `ByAnyChar` is given the empty string, it behaves exactly like
     *        `ByString` and matches each individual character in the input string.
     *        @see ByString
     * @note This delimiter works with single-byte string data, but does not work
     *       with variable-width encodings, such as UTF-8.
     *       if `ByAnyChar` is given the empty string, it behaves exactly like
     *       `ByString` and matches each individual character in the input string.
    * @param sp The delimiter string.
    */
    class ByAnyChar {
    public:
        explicit ByAnyChar(std::string_view sp);

        std::string_view Find(std::string_view text, size_t pos) const;

    private:
        const std::string delimiters_;
    };

    /**
     * @ingroup collie_strings_split
     * @brief A delimiter for splitting into equal-length strings.
     * @details The length argument to the constructor must be greater than 0.
     *          Note: this delimiter works with single-byte string data, but does not work
     *          with variable-width encodings, such as UTF-8.
     *          Example:
     *          @code
     *          using collie::ByLength;
     *          std::vector<std::string> v = collie::str_split("123456789", ByLength(3));
     *          // v[0] == "123", v[1] == "456", v[2] == "789"
     *          @endcode
     *          Note that the string does not have to be a multiple of the fixed split
     *          length. In such a case, the last substring will be shorter.
     *          @code
     *          using collie::ByLength;
     *          std::vector<std::string> v = collie::str_split("12345", ByLength(2));
     *          // v[0] == "12", v[1] == "34", v[2] == "5"
     *          @endcode
     * @param length The length of the delimiter.
     *
     */
    class ByLength {
    public:
        explicit ByLength(ptrdiff_t length);

        [[nodiscard]] std::string_view Find(std::string_view text, size_t pos) const;

    private:
        const ptrdiff_t length_;
    };

    namespace strings_internal {

        // A traits-like metafunction for selecting the default Delimiter object type
        // for a particular Delimiter type. The base case simply exposes type Delimiter
        // itself as the delimiter's Type. However, there are specializations for
        // string-like objects that map them to the ByString delimiter object.
        // This allows functions like collie::str_split() and collie::max_splits() to accept
        // string-like objects (e.g., ',') as delimiter arguments but they will be
        // treated as if a ByString delimiter was given.
        template<typename Delimiter>
        struct SelectDelimiter {
            using type = Delimiter;
        };

        template<>
        struct SelectDelimiter<char> {
            using type = ByChar;
        };
        template<>
        struct SelectDelimiter<char *> {
            using type = ByString;
        };
        template<>
        struct SelectDelimiter<const char *> {
            using type = ByString;
        };
        template<>
        struct SelectDelimiter<std::string_view> {
            using type = ByString;
        };
        template<>
        struct SelectDelimiter<std::string> {
            using type = ByString;
        };

        // Wraps another delimiter and sets a max number of matches for that delimiter.
        template<typename Delimiter>
        class MaxSplitsImpl {
        public:
            MaxSplitsImpl(Delimiter delimiter, int limit)
                    : delimiter_(delimiter), limit_(limit), count_(0) {}

            std::string_view Find(std::string_view text, size_t pos) {
                if (count_++ == limit_) {
                    return std::string_view(text.data() + text.size(),
                                            0);  // No more matches.
                }
                return delimiter_.Find(text, pos);
            }

        private:
            Delimiter delimiter_;
            const int limit_;
            int count_;
        };

    }  // namespace strings_internal

    /**
     * @ingroup collie_strings_split
     * @brief A delimiter that limits the number of matches which can occur to the passed
     *        `limit`. The last element in the returned collection will contain all
     *        remaining unsplit pieces, which may contain instances of the delimiter.
     *        The collection will contain at most `limit` + 1 elements.
     *        Example:
     *        @code
     *        using collie::max_splits;
     *        std::vector<std::string> v = collie::str_split("a,b,c", max_splits(',', 1));
     *        // v[0] == "a", v[1] == "b,c"
     *        @endcode
     * @param delimiter The delimiter.
     * @param limit The maximum number of splits.
     *
     */
    template<typename Delimiter>
    inline strings_internal::MaxSplitsImpl<
            typename strings_internal::SelectDelimiter<Delimiter>::type>
    max_splits(Delimiter delimiter, int limit) {
        typedef
        typename strings_internal::SelectDelimiter<Delimiter>::type DelimiterType;
        return strings_internal::MaxSplitsImpl<DelimiterType>(
                DelimiterType(delimiter), limit);
    }

    //------------------------------------------------------------------------------
    // Predicates
    //------------------------------------------------------------------------------
    //
    // Predicates filter the results of a `str_split()` by determining whether or not
    // a resultant element is included in the result set. A predicate may be passed
    // as an optional third argument to the `str_split()` function.
    //
    // Predicates are unary functions (or functors) that take a single
    // `std::string_view` argument and return a bool indicating whether the
    // argument should be included (`true`) or excluded (`false`).
    //
    // Predicates are useful when filtering out empty substrings. By default, empty
    // substrings may be returned by `str_split()`, which is similar to the way split
    // functions work in other programming languages.
    /**
     * @ingroup collie_strings_split
     * @brief A predicate that always returns `true`, indicating that all strings--
     *        including empty strings--should be included in the split output.
     *        This predicate is not strictly needed because this is the default
     *        behavior of `str_split()`; however, it might be useful at some call
     *        sites to make the intent explicit.
     *        Example:
     *        @code
     *        std::vector<std::string> v = collie::str_split(" a , ,,b,", ',', AllowEmpty());
     *        // v[0] == " a ", v[1] == " ", v[2] == "", v[3] = "b", v[4] == ""
     *        @endcode
     */
    struct AllowEmpty {
        bool operator()(std::string_view) const { return true; }
    };

    /**
     * @ingroup collie_strings_split
     * @brief A predicate that returns `false` if the given `std::string_view` is
     *        empty, indicating that `str_split()` should omit the empty string.
     *        Example:
     *        @code
     *        std::vector<std::string> v = collie::str_split(",a,,b,", ',', SkipEmpty());
     *        // v[0] == "a", v[1] == "b"
     *        @endcode
     * @note `SkipEmpty()` does not consider a string containing only whitespace
     *       to be empty. To skip such whitespace as well, use the `SkipWhitespace()`
     *       predicate.
     */
    struct SkipEmpty {
        bool operator()(std::string_view sp) const { return !sp.empty(); }
    };

    /**
     * @ingroup collie_strings_split
     * @brief A predicate that returns `false` if the given `std::string_view` is
     *        empty or contains only whitespace, indicating that `str_split()` should
     *        omit the string.
     *        Example:
     *        @code
     *        std::vector<std::string> v = collie::str_split(" a , ,,b,", ',', SkipWhitespace());
     *        // v[0] == " a ", v[1] == "b"
     *        @endcode
     *        `SkipEmpty()` would return whitespace elements.
     * @note `SkipWhitespace()` does not consider a string containing only whitespace
     *       to be empty. To skip such whitespace as well, use the `SkipWhitespace()`
     *       predicate.
     */
    struct SkipWhitespace {
        bool operator()(std::string_view sp) const {
            sp = collie::trim_all(sp);
            return !sp.empty();
        }
    };

    template<typename T>
    using EnableSplitIfString =
            typename std::enable_if<std::is_same<T, std::string>::value ||
                                    std::is_same<T, const std::string>::value,
                    int>::type;/**
     * @ingroup collie_strings_split
     * @brief Splits the given string on the given delimiter.
     * @details Split a given string based on the provided `Delimiter` object,
     *          returning the elements within the type specified by the caller.
     *          Optionally, you may pass a `Predicate` to `str_split()` indicating
     *          whether to include or exclude the resulting element within the final
     *          result set.
     *          Example:
     *          @code
     *          std::vector<std::string> v = collie::str_split("a,b,c,d", ',');
     *          // v[0] == "a", v[1] == "b", v[2] == "c", v[3] == "d"
     *          @endcode
     *          You can also provide an explicit `Delimiter` object:
     *          Example:
     *          @code
     *          using collie::ByAnyChar;
     *          std::vector<std::string> v = collie::str_split("a,b=c", ByAnyChar(",="));
     *          // v[0] == "a", v[1] == "b", v[2] == "c"
     *          @endcode
     *          See above for more information on delimiters.
     *          By default, empty strings are included in the result set. You can
     *          optionally include a third `Predicate` argument to apply a test for
     *          whether the resultant element should be included in the result set:
     *          Example:
     *          @code
     *          std::vector<std::string> v = collie::str_split(" a , ,,b,", ',', SkipWhitespace());
     *          // v[0] == " a ", v[1] == "b"
     *          @endcode
     *          See above for more information on predicates.
     *          str_split() Return Types
     *          The `str_split()` function adapts the returned collection to the
     *          collection specified by the caller (e.g. `std::vector` above). The
     *          returned collections may contain `std::string`, `std::string_view` (in
     *          which case the original string being split must ensure that it outlives
     *          the collection), or any object that can be explicitly created from an
     *          `std::string_view`. This behavior works for:
     *          1) All standard STL containers including `std::vector`, `std::list`,
     *          `std::deque`, `std::set`,`std::multiset`, 'std::map`, and `std::multimap`
     *          2) `std::pair` (which is not actually a container). See below.
     *          Example:
     *          @code
     *          // The results are returned as `std::string_view` objects. Note that we
     *          // have to ensure that the input string outlives any results.
     *          std::vector<std::string_view> v = collie::str_split("a,b,c", ',');
     *          // Stores results in a std::set<std::string>, which also performs
     *          // de-duplication and orders the elements in ascending order.
     *          std::set<std::string> a = collie::str_split("b,a,c,a,b", ',');
     *          // v[0] == "a", v[1] == "b", v[2] = "c"
     *          // `str_split()` can be used within a range-based for loop, in which case
     *          // each element will be of type `std::string_view`.
     *          std::vector<std::string> v;
     *          for (const auto sv : collie::str_split("a,b,c", ',')) {
     *             if (sv != "b") v.emplace_back(sv);
     *          }
     *          // v[0] == "a", v[1] == "c"
     *          // Stores results in a map. The map implementation assumes that the input
     *          // is provided as a series of key/value pairs. For example, the 0th element
     *          // resulting from the split will be stored as a key to the 1st element. If
     *          // an odd number of elements are resolved, the last element is paired with
     *          // a default-constructed value (e.g., empty string).
     *          std::map<std::string, std::string> m = collie::str_split("a,b,c", ',');
     *          // m["a"] == "b", m["c"] == ""     // last component value equals ""
     *          // Splitting to `std::pair` is an interesting case because it can hold only two
     *          // elements and is not a collection type. When splitting to a `std::pair` the
     *          // first two split strings become the `std::pair` `.first` and `.second`
     *          // members, respectively. The remaining split substrings are discarded. If there
     *          // are less than two split substrings, the empty string is used for the
     *          // corresponding `std::pair` member.
     *          @endcode
     *          // Example:
     *          @code
     *          // Stores first two split strings as the members in a std::pair.
     *          std::pair<std::string, std::string> p = collie::str_split("a,b,c", ',');
     *          // p.first == "a", p.second == "b"       // "c" is omitted.
     *          @endcode
     *          The `str_split()` function can be used multiple times to perform more
     *          complicated splitting logic, such as intelligently parsing key-value pairs.
     *          Example:
     *          @code
     *          // The input string "a=b=c,d=e,f=,g" becomes
     *          // { "a" => "b=c", "d" => "e", "f" => "", "g" => "" }
     *          std::map<std::string, std::string> m;
     *          for (std::string_view sp : collie::str_split("a=b=c,d=e,f=,g", ',')) {
     *              m.insert(collie::str_split(sp, collie::max_splits('=', 1)));
     *          }
     *          EXPECT_EQ("b=c", m.find("a")->second);
     *          EXPECT_EQ("e", m.find("d")->second);
     *          EXPECT_EQ("", m.find("f")->second);
     *          EXPECT_EQ("", m.find("g")->second);
     *          @endcode
     *          WARNING: Due to a legacy bug that is maintained for backward compatibility,
     *          splitting the following empty string_views produces different results:
     *          @code
     *          collie::str_split(std::string_view(""), '-');  // {""}
     *          collie::str_split(std::string_view(), '-');    // {}, but should be {""}
     *          @endcode
     *          Try not to depend on this distinction because the bug may one day be fixed.
     * @param text [input] The string to split.
     * @param d [input] The delimiter.
     * @param p [input] The predicate.
     * @return The split string.
     */
    template<typename Delimiter>
    strings_internal::Splitter<
            typename strings_internal::SelectDelimiter<Delimiter>::type, AllowEmpty,
            std::string_view>
    str_split(strings_internal::ConvertibleToStringView text, Delimiter d) {
        using DelimiterType =
                typename strings_internal::SelectDelimiter<Delimiter>::type;
        return strings_internal::Splitter<DelimiterType, AllowEmpty,
                std::string_view>(
                text.value(), DelimiterType(d), AllowEmpty());
    }

    template<typename Delimiter, typename StringType,
            EnableSplitIfString<StringType> = 0>
    strings_internal::Splitter<
            typename strings_internal::SelectDelimiter<Delimiter>::type, AllowEmpty,
            std::string>
    str_split(StringType &&text, Delimiter d) {
        using DelimiterType =
                typename strings_internal::SelectDelimiter<Delimiter>::type;
        return strings_internal::Splitter<DelimiterType, AllowEmpty, std::string>(
                std::move(text), DelimiterType(d), AllowEmpty());
    }

    template<typename Delimiter, typename Predicate>
    strings_internal::Splitter<
            typename strings_internal::SelectDelimiter<Delimiter>::type, Predicate,
            std::string_view>
    str_split(strings_internal::ConvertibleToStringView text, Delimiter d,
             Predicate p) {
        using DelimiterType =
                typename strings_internal::SelectDelimiter<Delimiter>::type;
        return strings_internal::Splitter<DelimiterType, Predicate,
                std::string_view>(
                text.value(), DelimiterType(d), std::move(p));
    }

    template<typename Delimiter, typename Predicate, typename StringType,
            EnableSplitIfString<StringType> = 0>
    strings_internal::Splitter<
            typename strings_internal::SelectDelimiter<Delimiter>::type, Predicate,
            std::string>
    str_split(StringType &&text, Delimiter d, Predicate p) {
        using DelimiterType =
                typename strings_internal::SelectDelimiter<Delimiter>::type;
        return strings_internal::Splitter<DelimiterType, Predicate, std::string>(
                std::move(text), DelimiterType(d), std::move(p));
    }
}  // namespace collie

/// inlines

namespace collie {
    namespace {

        // This GenericFind() template function encapsulates the finding algorithm
        // shared between the ByString and ByAnyChar delimiters. The FindPolicy
        // template parameter allows each delimiter to customize the actual find
        // function to use and the length of the found delimiter. For example, the
        // Literal delimiter will ultimately use std::string_view::find(), and the
        // AnyOf delimiter will use std::string_view::find_first_of().
        template<typename FindPolicy>
        inline std::string_view GenericFind(std::string_view text,
                                     std::string_view delimiter, size_t pos,
                                     FindPolicy find_policy) {
            if (delimiter.empty() && text.length() > 0) {
                // Special case for empty string delimiters: always return a zero-length
                // std::string_view referring to the item at position 1 past pos.
                return std::string_view(text.data() + pos + 1, 0);
            }
            size_t found_pos = std::string_view::npos;
            std::string_view found(text.data() + text.size(),
                                   0);  // By default, not found
            found_pos = find_policy.Find(text, delimiter, pos);
            if (found_pos != std::string_view::npos) {
                found = std::string_view(text.data() + found_pos,
                                         find_policy.Length(delimiter));
            }
            return found;
        }

        // Finds using std::string_view::find(), therefore the length of the found
        // delimiter is delimiter.length().
        struct LiteralPolicy {
            size_t Find(std::string_view text, std::string_view delimiter, size_t pos) {
                return text.find(delimiter, pos);
            }

            size_t Length(std::string_view delimiter) { return delimiter.length(); }
        };

        // Finds using std::string_view::find_first_of(), therefore the length of the
        // found delimiter is 1.
        struct AnyOfPolicy {
            size_t Find(std::string_view text, std::string_view delimiter, size_t pos) {
                return text.find_first_of(delimiter, pos);
            }

            size_t Length(std::string_view /* delimiter */) { return 1; }
        };

    }  // namespace

    //
    // ByString
    //

    inline ByString::ByString(std::string_view sp) : delimiter_(sp) {}

    inline std::string_view ByString::Find(std::string_view text, size_t pos) const {
        if (delimiter_.length() == 1) {
            // Much faster to call find on a single character than on an
            // std::string_view.
            size_t found_pos = text.find(delimiter_[0], pos);
            if (found_pos == std::string_view::npos)
                return std::string_view(text.data() + text.size(), 0);
            return text.substr(found_pos, 1);
        }
        return GenericFind(text, delimiter_, pos, LiteralPolicy());
    }

    //
    // ByChar
    //

    inline std::string_view ByChar::Find(std::string_view text, size_t pos) const {
        size_t found_pos = text.find(c_, pos);
        if (found_pos == std::string_view::npos)
            return std::string_view(text.data() + text.size(), 0);
        return text.substr(found_pos, 1);
    }

    //
    // ByAnyChar
    //

    inline ByAnyChar::ByAnyChar(std::string_view sp) : delimiters_(sp) {}

    inline std::string_view ByAnyChar::Find(std::string_view text, size_t pos) const {
        return GenericFind(text, delimiters_, pos, AnyOfPolicy());
    }

    //
    // ByLength
    //
    inline ByLength::ByLength(ptrdiff_t length) : length_(length) {
        assert(length > 0);
    }

    inline std::string_view ByLength::Find(std::string_view text,
                                     size_t pos) const {
        pos = std::min(pos, text.size());  // truncate `pos`
        std::string_view substr = text.substr(pos);
        // If the string is shorter than the chunk size we say we
        // "can't find the delimiter" so this will be the last chunk.
        if (substr.length() <= static_cast<size_t>(length_))
            return std::string_view(text.data() + text.size(), 0);

        return std::string_view(substr.data() + length_, 0);
    }
}
