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
// Created by jeff on 24-1-10.
//

#pragma once

#include <algorithm>
#include <functional>
#include <type_traits>
#include <utility>

namespace collie::container_internal {

    template <typename T, typename SFINAE = void>
    struct MapKeyType : std::false_type {};

    template <typename T>
    struct MapKeyType<T, std::void_t<typename T::key_type>> : std::true_type {};

}  // namespace turbo::container_internal

namespace collie {

    template <typename Container, typename Value, typename std::enable_if_t<container_internal::MapKeyType<Container>::value, int> =0>
    constexpr bool contains(const Container& container, const Value& value) {
        return container.find(value) != container.end();
    }

    // Overload that allows to provide an additional projection invocable. This
    // projection will be applied to every element in `container` before comparing
    // it with `value`. This will always perform a linear search.
    template <typename Container, typename Value, typename std::enable_if_t<!container_internal::MapKeyType<Container>::value, int> =0>
    constexpr bool contains(const Container& container,
                            const Value& value, bool sorted = false) {
        if(sorted) {
            return std::binary_search(container.begin(), container.end(), value);
        }
        return std::find(container.begin(), container.end(), value) != container.end();
    }
}  // namespace collie
