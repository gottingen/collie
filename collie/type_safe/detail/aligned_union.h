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

#include <cstddef>
#include <type_traits>

namespace collie::ts::detail {
    // max for variadic number of types.
    template<typename T>
    constexpr const T &max(const T &a) {
        return a;
    }

    template<typename T>
    constexpr const T &max(const T &a, const T &b) {
        return a < b ? b : a;
    }

    template<typename T, typename... Ts>
    constexpr const T &max(const T &t, const Ts &... ts) {
        return max(t, max(ts...));
    }

    template<typename... Types>
    class aligned_union {
    public:
        static constexpr auto size_value = detail::max(sizeof(Types)...);
        static constexpr auto alignment_value = detail::max(alignof(Types)...);

        void *get() noexcept {
            return &storage_;
        }

        const void *get() const noexcept {
            return &storage_;
        }

    private:
        alignas(alignment_value) unsigned char storage_[size_value];
    };
} // namespace collie::ts::detail

