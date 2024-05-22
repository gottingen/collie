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

#ifndef COLLIE_STRINGS_FORMAT_H_
#define COLLIE_STRINGS_FORMAT_H_

#include <collie/strings/fmt/format.h>
#include <collie/strings/fmt/ostream.h>
#include <collie/strings/fmt/printf.h>
#include <collie/strings/fmt/ranges.h>
#include <collie/strings/fmt/chrono.h>
#include <collie/strings/fmt/color.h>
#include <collie/strings/fmt/compile.h>
#include <collie/strings/fmt/os.h>
#include <collie/strings/fmt/args.h>
#include <collie/strings/fmt/std.h>
#include <collie/strings/text_style.h>

namespace collie {

    template<typename String = std::string, typename T>
    [[nodiscard]] inline String to_str(const T &t) {
        String result;
        fmt::memory_buffer buf;
        fmt::format_to(std::back_inserter(buf), "{}", t);
        return String(buf.data(), buf.size());
    }

    using namespace fmt;

    template<typename String = std::string, typename ...Args>
    void format_append(String *dst, std::string_view fmt, Args &&... args) {
        fmt::memory_buffer buf;
        fmt::format_to(std::back_inserter(buf), fmt, std::forward<Args>(args)...);
        dst->append(buf.data(), buf.size());
    }

    template<typename String = std::string, typename T>
    void format_append(String *dst, const T &t) {
        fmt::memory_buffer buf;
        fmt::format_to(std::back_inserter(buf), "{}", t);
        dst->append(buf.data(), buf.size());
    }

    template<typename String = std::string, typename ...Args>
    String format_range(std::string_view fmt, const std::tuple<Args...> &tuple, std::string_view sep) {
        fmt::memory_buffer view_buf;
        fmt::format_to(std::back_inserter(view_buf), fmt, fmt::join(tuple, sep));
        return String(view_buf.data(), view_buf.size());
    }

    template<typename String = std::string, typename T>

    String format_range(std::string_view fmt, std::initializer_list<T> list, std::string_view sep) {
        fmt::memory_buffer view_buf;
        fmt::format_to(std::back_inserter(view_buf), fmt, fmt::join(list, sep));
        return String(view_buf.data(), view_buf.size());
    }

    template<typename It, typename Sentinel, typename String = std::string>
    String format_range(std::string_view fmt, It begin, Sentinel end, std::string_view sep) {
        fmt::memory_buffer view_buf;
        fmt::format_to(std::back_inserter(view_buf), fmt,
                       fmt::join(std::forward<It>(begin), std::forward<Sentinel>(end), sep));
        return String(view_buf.data(), view_buf.size());
    }

    template<typename String = std::string, typename Range>
    String format_range(std::string_view fmt, Range &&range, std::string_view sep) {
        fmt::memory_buffer view_buf;
        fmt::format_to(std::back_inserter(view_buf), fmt, fmt::join(std::forward<Range>(range), sep));
        return String(view_buf.data(), view_buf.size());
    }

    /// format_range_append
    template<typename String = std::string, typename ...Args>
    void
    format_range_append(String *dst, std::string_view fmt, const std::tuple<Args...> &tuple, std::string_view sep) {
        fmt::memory_buffer view_buf;
        fmt::format_to(std::back_inserter(view_buf), fmt, fmt::join(tuple, sep));
        dst->append(view_buf.data(), view_buf.size());
    }

    template<typename String = std::string, typename T>

    void format_range_append(String *dst, std::string_view fmt, std::initializer_list<T> list, std::string_view sep) {
        fmt::memory_buffer view_buf;
        fmt::format_to(std::back_inserter(view_buf), fmt, fmt::join(list, sep));
        dst->append(view_buf.data(), view_buf.size());
    }

    template<typename String = std::string, typename It, typename Sentinel>
    void format_range_append(String *dst, std::string_view fmt, It begin, Sentinel end, std::string_view sep) {
        fmt::memory_buffer view_buf;
        fmt::format_to(std::back_inserter(view_buf), fmt, fmt::join(begin, end, sep));
        dst->append(view_buf.data(), view_buf.size());
    }

    template<typename String = std::string, typename Range>
    void format_range_append(String *dst, std::string_view fmt, Range &&range, std::string_view sep) {
        fmt::memory_buffer view_buf;
        fmt::format_to(std::back_inserter(view_buf), fmt, fmt::join(std::forward<Range>(range), sep));
        dst->append(view_buf.data(), view_buf.size());
    }

    template<typename T>
    void println(std::FILE *file, const T &arg) {
        fmt::print(file, "{}\n", arg);
    }

    template<typename T>
    void println(std::ostream &os, const T &arg) {
        fmt::print(os, "{}\n", arg);
    }

    template<typename ...Args>
    void println(std::FILE *file, std::string_view fmt, Args &&... args) {
        fmt::print(file, "{}\n", format(fmt, std::forward<Args>(args)...));
    }

    template<typename ...Args>
    void println(std::ostream &os, std::string_view fmt, Args &&... args) {
        fmt::print(os, "{}\n", format(fmt, std::forward<Args>(args)...));
    }

    template<typename T>
    void println(const T &arg) {
        fmt::print(stdout, "{}\n", arg);
    }

    template<typename ...Args>
    inline void println(std::string_view fmt, Args &&... args) {
        fmt::print(stdout, "{}\n", format(fmt, std::forward<Args>(args)...));
    }

    template<typename ...Args>
    inline void println(const Color &c, std::string_view fmt, Args &&... args) {
        TextStyle ts;
        ts.with_foreground_color(c);
        TermPrinter::apply_text_style(std::cout, ts);
        fmt::print(std::cout, "{}\n", format(fmt, std::forward<Args>(args)...));
        TermPrinter::reset_element_style(std::cout);
    }

    template<typename ...Args>
    inline void println(const TextStyle &ts, std::string_view fmt, Args &&... args) {
        TermPrinter::apply_text_style(std::cout, ts);
        fmt::print(std::cout, "{}\n", format(fmt, std::forward<Args>(args)...));
        TermPrinter::reset_element_style(std::cout);
    }

    template<typename T>
    inline void println(const Color &c, const T &arg) {
        TextStyle ts;
        ts.with_foreground_color(c);
        TermPrinter::apply_text_style(std::cout, ts);
        fmt::print(std::cout, "{}\n", arg);
        TermPrinter::reset_element_style(std::cout);
    }

    template<typename T>
    inline void println(const TextStyle &ts, const T &arg) {
        TermPrinter::apply_text_style(std::cout, ts);
        fmt::print(std::cout, "{}\n", arg);
        TermPrinter::reset_element_style(std::cout);
    }


}

#endif  // COLLIE_STRINGS_FORMAT_H_
