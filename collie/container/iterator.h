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

#include <cstddef>
#include <type_traits>

namespace collie {

    template<typename T>
    constexpr std::enable_if_t<std::is_integral<std::decay_t<T>>::value, bool>
    is_range_invalid(T beg, T end, T step) {
        return ((step == 0 && beg != end) ||
                (beg < end && step <= 0) ||  // positive range
                (beg > end && step >= 0));   // negative range
    }

    template<typename T>
    constexpr std::enable_if_t<std::is_integral<std::decay_t<T>>::value, size_t>
    distance(T beg, T end, T step) {
        return (end - beg + step + (step > 0 ? -1 : 1)) / step;
    }

}  // end of namespace collie
