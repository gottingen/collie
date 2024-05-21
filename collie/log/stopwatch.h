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

#include <chrono>
#include <collie/strings/fmt/format.h>

// Stopwatch support for clog  (using std::chrono::steady_clock).
// Displays elapsed seconds since construction as double.
//
// Usage:
//
// collie::log::stopwatch sw;
// ...
// collie::log::debug("Elapsed: {} seconds", sw);    =>  "Elapsed 0.005116733 seconds"
// collie::log::info("Elapsed: {:.6} seconds", sw);  =>  "Elapsed 0.005163 seconds"
//
//
// If other units are needed (e.g. millis instead of double), include <collie/strings/fmt/chrono.h> and use
// "duration_cast<..>(sw.elapsed())":
//
// #include <collie/log/fmt/chrono.h>
//..
// using std::chrono::duration_cast;
// using std::chrono::milliseconds;
// collie::log::info("Elapsed {}", duration_cast<milliseconds>(sw.elapsed())); => "Elapsed 5ms"

namespace collie::log {
    class stopwatch {
        using clock = std::chrono::steady_clock;
        std::chrono::time_point<clock> start_tp_;

    public:
        stopwatch()
                : start_tp_{clock::now()} {}

        std::chrono::duration<double> elapsed() const {
            return std::chrono::duration<double>(clock::now() - start_tp_);
        }

        void reset() { start_tp_ = clock::now(); }
    };
}  // namespace collie::log

// Support for fmt formatting  (e.g. "{:012.9}" or just "{}")
namespace fmt {

    template<>
    struct formatter<collie::log::stopwatch> : formatter<double> {
        template<typename FormatContext>
        auto format(const collie::log::stopwatch &sw, FormatContext &ctx) const -> decltype(ctx.out()) {
            return formatter<double>::format(sw.elapsed().count(), ctx);
        }
    };
}  // namespace std
