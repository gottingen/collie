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

#include <collie/type_safe/floating_point.h>
#include <collie/type_safe/integer.h>

namespace collie::ts
{
/// \exclude
namespace detail
{
    template <typename T>
    struct get_target_underlying_integer
    {
        using type = T;
    };
    template <typename T, class Policy>
    struct get_target_underlying_integer<integer<T, Policy>>
    {
        using type = T;
    };

    template <typename T, class Policy>
    struct get_target_integer
    {
        using type = integer<T, Policy>;
    };
    template <typename T, class Policy>
    struct get_target_integer<integer<T, Policy>, Policy>
    {
        using type = integer<T, Policy>;
    };

    template <typename T>
    struct get_target_floating_point
    {
        using type = floating_point<T>;
    };
    template <typename T>
    struct get_target_floating_point<floating_point<T>>
    {
        using type = floating_point<T>;
    };

    template <typename Target, typename Source>
    TYPE_SAFE_FORCE_INLINE constexpr bool is_narrowing(
        const Source& source,
        typename std::enable_if<std::is_integral<Source>::value, int>::type = 0)
    {
        using limits = std::numeric_limits<Target>;
        return sizeof(Target) < sizeof(Source) // no narrowing possible
               && (source > Source(limits::max())
                   || source < Source(limits::min())); // otherwise check bounds
    }

    template <typename Target, typename Source>
    TYPE_SAFE_FORCE_INLINE constexpr bool is_narrowing(
        const Source& source,
        typename std::enable_if<std::is_floating_point<Source>::value, int>::type = 0)

    {
        return sizeof(Target) < sizeof(Source) // no narrowing possible
                                               // cast source -> underlying float -> target float ->
                                               // source and check if it changed the value
               && static_cast<Source>(static_cast<Target>(source)) != source;
    }
} // namespace detail

/// \returns An arithmetic type with the same value as in `source`,
/// but converted to to a different type.
/// \requires The value of `source` must be representable by the new target type.
/// \module types
/// \param 2
/// \exclude
template <typename Target, typename Source,
          typename = typename std::enable_if<std::is_arithmetic<Source>::value>::type>
TYPE_SAFE_FORCE_INLINE constexpr Target narrow_cast(const Source& source) noexcept
{
    using underlying = typename detail::get_target_underlying_integer<Target>::type;
    return detail::is_narrowing<underlying>(source)
               ? DEBUG_UNREACHABLE(detail::precondition_error_handler{},
                                   "conversion would truncate value")
               : static_cast<Target>(static_cast<underlying>(source));
}

/// \returns A [ts::integer]() with the same value as `source` but of a different type.
/// \requires The value of `source` must be representable by the new target type.
/// \notes `Target` can either be a specialization of the `integer` template itself
/// or a built-in integer type, the result will be wrapped if needed.
/// \module types
/// \exclude return
template <typename Target, typename Source, class Policy>
TYPE_SAFE_FORCE_INLINE constexpr auto narrow_cast(const integer<Source, Policy>& source) noexcept ->
    typename detail::get_target_integer<Target, Policy>::type
{
    using target_integer = typename detail::get_target_integer<Target, Policy>::type;
    using target_t       = typename target_integer::integer_type;
    return narrow_cast<target_t>(static_cast<Source>(source));
}

/// \returns A [ts::floating_point]() with the same value as `source` but of a different type.
/// \requires The value of `source` must be representable by the new target type.
/// \notes `Target` can either be a specialization of the `floating_point` template itself
/// or a built-in floating point type, the result will be wrapped if needed.
/// \module types
/// \exclude return
template <typename Target, typename Source>
TYPE_SAFE_FORCE_INLINE constexpr auto narrow_cast(const floating_point<Source>& source) noexcept ->
    typename detail::get_target_floating_point<Target>::type
{
    using target_float = typename detail::get_target_floating_point<Target>::type;
    using target_t     = typename target_float::floating_point_type;
    return narrow_cast<target_t>(static_cast<Source>(source));
}
} // namespace collie::ts
