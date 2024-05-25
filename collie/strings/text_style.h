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

#include <collie/port/port.h>
// the following snippet of code detects the current OS and
// defines the appropriate macro that is used to wrap some
// platform specific things
// This headers provides the `isatty()`/`fileno()` functions,
// which are used for testing whether a standart stream refers
// to the terminal. As for Windows, we also need WinApi funcs
// for changing colors attributes of the terminal.
#if defined(COLLIE_PLATFORM_LINUX) || defined(COLLIE_PLATFORM_OSX)

#include <unistd.h>

#elif defined(COLLIE_PLATFORM_WINDOWS)
#include <io.h>
#include <windows.h>
#endif

#include <cstdio>
#include <iostream>

namespace collie::termcolor {
    // Forward declaration of the `_internal` namespace.
    // All comments are below.
    namespace _internal {
        // An index to be used to access a private storage of I/O streams. See
        // colorize / nocolorize I/O manipulators for details.
        static int colorize_index = std::ios_base::xalloc();

        inline FILE *get_standard_stream(const std::ostream &stream);

        inline bool is_colorized(std::ostream &stream);

        inline bool is_atty(const std::ostream &stream);

#if defined(COLLIE_PLATFORM_WINDOWS)
        inline void win_change_attributes(std::ostream &stream, int foreground, int background = -1);
#endif
    } // namespace _internal

    inline std::ostream &colorize(std::ostream &stream) {
        stream.iword(_internal::colorize_index) = 1L;
        return stream;
    }

    inline std::ostream &nocolorize(std::ostream &stream) {
        stream.iword(_internal::colorize_index) = 0L;
        return stream;
    }

    inline std::ostream &reset(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[00m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, -1, -1);
#endif
        }
        return stream;
    }

    inline std::ostream &bold(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[1m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
#endif
        }
        return stream;
    }

    inline std::ostream &dark(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[2m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
#endif
        }
        return stream;
    }

    inline std::ostream &italic(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[3m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
#endif
        }
        return stream;
    }

    inline std::ostream &underline(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[4m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
#endif
        }
        return stream;
    }

    inline std::ostream &blink(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[5m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
#endif
        }
        return stream;
    }

    inline std::ostream &reverse(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[7m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
#endif
        }
        return stream;
    }

    inline std::ostream &concealed(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[8m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
#endif
        }
        return stream;
    }

    inline std::ostream &crossed(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[9m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
#endif
        }
        return stream;
    }

    inline std::ostream &grey(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[30m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream,
                                             0 // grey (black)
            );
#endif
        }
        return stream;
    }

    inline std::ostream &red(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[31m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, FOREGROUND_RED);
#endif
        }
        return stream;
    }

    inline std::ostream &green(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[32m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, FOREGROUND_GREEN);
#endif
        }
        return stream;
    }

    inline std::ostream &yellow(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[33m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, FOREGROUND_GREEN | FOREGROUND_RED);
#endif
        }
        return stream;
    }

    inline std::ostream &blue(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[34m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, FOREGROUND_BLUE);
#endif
        }
        return stream;
    }

    inline std::ostream &magenta(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[35m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_RED);
#endif
        }
        return stream;
    }

    inline std::ostream &cyan(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[36m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_GREEN);
#endif
        }
        return stream;
    }

    inline std::ostream &white(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[37m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#endif
        }
        return stream;
    }

    inline std::ostream &on_grey(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[40m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                                             0 // grey (black)
            );
#endif
        }
        return stream;
    }

    inline std::ostream &on_red(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[41m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, -1, BACKGROUND_RED);
#endif
        }
        return stream;
    }

    inline std::ostream &on_green(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[42m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, -1, BACKGROUND_GREEN);
#endif
        }
        return stream;
    }

    inline std::ostream &on_yellow(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[43m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, -1, BACKGROUND_GREEN | BACKGROUND_RED);
#endif
        }
        return stream;
    }

    inline std::ostream &on_blue(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[44m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, -1, BACKGROUND_BLUE);
#endif
        }
        return stream;
    }

    inline std::ostream &on_magenta(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[45m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, -1, BACKGROUND_BLUE | BACKGROUND_RED);
#endif
        }
        return stream;
    }

    inline std::ostream &on_cyan(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[46m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, -1, BACKGROUND_GREEN | BACKGROUND_BLUE);
#endif
        }
        return stream;
    }

    inline std::ostream &on_white(std::ostream &stream) {
        if (_internal::is_colorized(stream)) {
#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            stream << "\033[47m";
#elif defined(COLLIE_PLATFORM_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                                             BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED);
#endif
        }

        return stream;
    }

    //! Since C++ hasn't a way to hide something in the header from
    //! the outer access, I have to introduce this namespace which
    //! is used for internal purpose and should't be access from
    //! the user code.
    namespace _internal {
        //! Since C++ hasn't a true way to extract stream handler
        //! from the a given `std::ostream` object, I have to write
        //! this kind of hack.
        inline FILE *get_standard_stream(const std::ostream &stream) {
            if (&stream == &std::cout)
                return stdout;
            else if ((&stream == &std::cerr) || (&stream == &std::clog))
                return stderr;

            return 0;
        }

        // Say whether a given stream should be colorized or not. It's always
        // true for ATTY streams and may be true for streams marked with
        // colorize flag.
        inline bool is_colorized(std::ostream &stream) {
            return is_atty(stream) || static_cast<bool>(stream.iword(colorize_index));
        }

        //! Test whether a given `std::ostream` object refers to
        //! a terminal.
        inline bool is_atty(const std::ostream &stream) {
            FILE *std_stream = get_standard_stream(stream);

            // Unfortunately, fileno() ends with segmentation fault
            // if invalid file descriptor is passed. So we need to
            // handle this case gracefully and assume it's not a tty
            // if standard stream is not detected, and 0 is returned.
            if (!std_stream)
                return false;

#if defined(COLLIE_PLATFORM_OSX) || defined(COLLIE_PLATFORM_LINUX)
            return ::isatty(fileno(std_stream));
#elif defined(COLLIE_PLATFORM_WINDOWS)
            return ::_isatty(_fileno(std_stream));
#endif
        }

#if defined(COLLIE_PLATFORM_WINDOWS)
        //! Change Windows Terminal colors attribute. If some
        //! parameter is `-1` then attribute won't changed.
        inline void win_change_attributes(std::ostream &stream, int foreground, int background) {
          // yeah, i know.. it's ugly, it's windows.
          static WORD defaultAttributes = 0;

          // Windows doesn't have ANSI escape sequences and so we use special
          // API to change Terminal output color. That means we can't
          // manipulate colors by means of "std::stringstream" and hence
          // should do nothing in this case.
          if (!_internal::is_atty(stream))
            return;

          // get terminal handle
          HANDLE hTerminal = INVALID_HANDLE_VALUE;
          if (&stream == &std::cout)
            hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
          else if (&stream == &std::cerr)
            hTerminal = GetStdHandle(STD_ERROR_HANDLE);

          // save default terminal attributes if it unsaved
          if (!defaultAttributes) {
            CONSOLE_SCREEN_BUFFER_INFO info;
            if (!GetConsoleScreenBufferInfo(hTerminal, &info))
              return;
            defaultAttributes = info.wAttributes;
          }

          // restore all default settings
          if (foreground == -1 && background == -1) {
            SetConsoleTextAttribute(hTerminal, defaultAttributes);
            return;
          }

          // get current settings
          CONSOLE_SCREEN_BUFFER_INFO info;
          if (!GetConsoleScreenBufferInfo(hTerminal, &info))
            return;

          if (foreground != -1) {
            info.wAttributes &= ~(info.wAttributes & 0x0F);
            info.wAttributes |= static_cast<WORD>(foreground);
          }

          if (background != -1) {
            info.wAttributes &= ~(info.wAttributes & 0xF0);
            info.wAttributes |= static_cast<WORD>(background);
          }

          SetConsoleTextAttribute(hTerminal, info.wAttributes);
        }
#endif // COLLIE_PLATFORM_WINDOWS

    } // namespace _internal

} // namespace collie::termcolor

namespace collie {

    enum class Color {
        none, grey, red, green, yellow, blue, magenta, cyan, white
    };
    enum class FontStyle {
        bold, dark, italic, underline, blink, reverse, concealed, crossed
    };

    struct TextStyle {
        std::vector<FontStyle> style_list{};
        Color foreground_color{Color::none};
        Color background_color{Color::none};
        TextStyle& with_style(FontStyle style) {
            this->style_list.push_back(style);
            return *this;
        }
        TextStyle& with_foreground_color(Color color) {
            this->foreground_color = color;
            return *this;
        }
        TextStyle& with_background_color(Color color) {
            this->background_color = color;
            return *this;
        }
    };

    class TermPrinter {
    public:
        static void apply_text_style(std::ostream &stream, const TextStyle &style) {
            apply_foreground_color(stream, style.foreground_color);
            apply_background_color(stream, style.background_color);
            for (const auto &s : style.style_list) {
                apply_font_style(stream, s);
            }
        }
        static void apply_font_style(std::ostream &stream, FontStyle style) {
            switch (style) {
                case FontStyle::bold:
                    stream << termcolor::bold;
                    break;
                case FontStyle::dark:
                    stream << termcolor::dark;
                    break;
                case FontStyle::italic:
                    stream << termcolor::italic;
                    break;
                case FontStyle::underline:
                    stream << termcolor::underline;
                    break;
                case FontStyle::blink:
                    stream << termcolor::blink;
                    break;
                case FontStyle::reverse:
                    stream << termcolor::reverse;
                    break;
                case FontStyle::concealed:
                    stream << termcolor::concealed;
                    break;
                case FontStyle::crossed:
                    stream << termcolor::crossed;
                    break;
                default:
                    break;
            }
        }

        static void apply_foreground_color(std::ostream &stream, Color foreground_color) {
            switch (foreground_color) {
                case Color::grey:
                    stream << termcolor::grey;
                    break;
                case Color::red:
                    stream << termcolor::red;
                    break;
                case Color::green:
                    stream << termcolor::green;
                    break;
                case Color::yellow:
                    stream << termcolor::yellow;
                    break;
                case Color::blue:
                    stream << termcolor::blue;
                    break;
                case Color::magenta:
                    stream << termcolor::magenta;
                    break;
                case Color::cyan:
                    stream << termcolor::cyan;
                    break;
                case Color::white:
                    stream << termcolor::white;
                    break;
                case Color::none:
                default:
                    break;
            }
        }

        static void apply_background_color(std::ostream &stream, Color background_color) {
            switch (background_color) {
                case Color::grey:
                    stream << termcolor::on_grey;
                    break;
                case Color::red:
                    stream << termcolor::on_red;
                    break;
                case Color::green:
                    stream << termcolor::on_green;
                    break;
                case Color::yellow:
                    stream << termcolor::on_yellow;
                    break;
                case Color::blue:
                    stream << termcolor::on_blue;
                    break;
                case Color::magenta:
                    stream << termcolor::on_magenta;
                    break;
                case Color::cyan:
                    stream << termcolor::on_cyan;
                    break;
                case Color::white:
                    stream << termcolor::on_white;
                    break;
                case Color::none:
                default:
                    break;
            }
        }

        static void reset_element_style(std::ostream &stream) { stream << termcolor::reset; }
    };
}  // namespace collie

