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
// 2020/08/28 - Created by netcan: https://github.com/netcan
#pragma once
#include <cstddef>
#include <tuple>

namespace collie::tf {
namespace dsl {
namespace detail {
// get tuple element index by f, if not exists then index >= tuple_size
template <typename TUP, template <typename> class F, typename = void>
struct TupleElementByF {
  constexpr static size_t Index = 0;
};

template <template <typename> class F, typename H, typename... Ts>
struct TupleElementByF<std::tuple<H, Ts...>, F, std::enable_if_t<F<H>::value>> {
  constexpr static size_t Index = 0;
};

template <template <typename> class F, typename H, typename... Ts>
struct TupleElementByF<std::tuple<H, Ts...>, F,
                       std::enable_if_t<!F<H>::value>> {
  constexpr static size_t Index =
      1 + TupleElementByF<std::tuple<Ts...>, F>::Index;
};

template <typename T, typename TUP, size_t... Is>
constexpr inline T AggregationByTupImpl(TUP &&tup, std::index_sequence<Is...>) {
  return T{std::get<Is>(tup)...};
}
} // namespace detail

template <typename TUP, template <typename> class F>
constexpr size_t TupleElementByF_v = detail::TupleElementByF<TUP, F>::Index;

template <typename T, typename TUP>
constexpr inline T AggregationByTup(TUP &&tup) {
  return detail::AggregationByTupImpl<T>(
      std::forward<TUP>(tup),
      std::make_index_sequence<std::tuple_size<std::decay_t<TUP>>::size>{});
}
} // namespace dsl
} // namespace collie::tf
