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

#pragma once

#include <utility>

namespace collie::ts::detail {
    template<typename T>
    struct is_nothrow_swappable {
        template<typename U>
        static auto adl_swap(int, U &a, U &b) noexcept(noexcept(swap(a, b)))
        -> decltype(swap(a, b));

        template<typename U>
        static auto adl_swap(short, U &a, U &b) noexcept(noexcept(std::swap(a, b)))
        -> decltype(std::swap(a, b));

        static void adl_swap(...) noexcept(false);

        static constexpr bool value = noexcept(adl_swap(0, std::declval<T &>(), std::declval<T &>()));
    };
} // namespace collie::ts::detail
