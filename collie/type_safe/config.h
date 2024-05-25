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

#if defined(TYPE_SAFE_IMPORT_STD_MODULE)
import std;
#else
#include <cstddef>
#include <cstdlib>
#endif

#ifndef TYPE_SAFE_ENABLE_ASSERTIONS
/// Controls whether internal assertions are enabled.
///
/// It is disabled by default.
#    define TYPE_SAFE_ENABLE_ASSERTIONS 0
#endif

#ifndef TYPE_SAFE_ENABLE_PRECONDITION_CHECKS
/// Controls whether preconditions are checked.
///
/// It is enabled by default.
#    define TYPE_SAFE_ENABLE_PRECONDITION_CHECKS 1
#endif

#ifndef TYPE_SAFE_ENABLE_WRAPPER
/// Controls whether the typedefs in [types.hpp]() use the type safe wrapper types.
///
/// It is enabled by default.
#    define TYPE_SAFE_ENABLE_WRAPPER 1
#endif

#ifndef TYPE_SAFE_ARITHMETIC_POLICY
/// Controls whether [ts::arithmetic_policy_default]() is [ts::undefined_behavior_arithmetic](),
/// [ts::checked_arithmetic](), or [ts::default_arithmetic]().
///
/// It is [ts::undefined_behavior_arithmetic]() by default.
#    define TYPE_SAFE_ARITHMETIC_POLICY 1
#endif

#ifndef TYPE_SAFE_DELETE_FUNCTIONS
#    if defined(_MSC_VER) && _MSC_VER < 1914
#        define TYPE_SAFE_DELETE_FUNCTIONS 0
#    else
/// \exclude
#        define TYPE_SAFE_DELETE_FUNCTIONS 0
#    endif
#endif

#ifndef TYPE_SAFE_USE_REF_QUALIFIERS
#    if defined(__cpp_ref_qualifiers) && __cpp_ref_qualifiers >= 200710
/// \exclude
#        define TYPE_SAFE_USE_REF_QUALIFIERS 1
#    elif defined(_MSC_VER) && _MSC_VER >= 1900
#        define TYPE_SAFE_USE_REF_QUALIFIERS 1
#    else
#        define TYPE_SAFE_USE_REF_QUALIFIERS 0
#    endif
#endif

#if TYPE_SAFE_USE_REF_QUALIFIERS
/// \exclude
#    define TYPE_SAFE_LVALUE_REF &
/// \exclude
#    define TYPE_SAFE_RVALUE_REF &&
#else
#    define TYPE_SAFE_LVALUE_REF
#    define TYPE_SAFE_RVALUE_REF
#endif

#ifndef TYPE_SAFE_USE_RETURN_TYPE_DEDUCTION
#    if defined(__cpp_return_type_deduction) && __cpp_return_type_deduction >= 201304
/// \exclude
#        define TYPE_SAFE_USE_RETURN_TYPE_DEDUCTION 1
#    elif defined(_MSC_VER) && _MSC_VER >= 1900
#        define TYPE_SAFE_USE_RETURN_TYPE_DEDUCTION 1
#    else
#        define TYPE_SAFE_USE_RETURN_TYPE_DEDUCTION 0
#    endif
#endif

#ifndef TYPE_SAFE_USE_NOEXCEPT_DEFAULT

#    if defined(__GNUC__) && __GNUC__ < 5
// GCC before 5.0 doesn't handle noexcept and = default properly
/// \exclude
#        define TYPE_SAFE_USE_NOEXCEPT_DEFAULT 0
#    else
/// \exclude
#        define TYPE_SAFE_USE_NOEXCEPT_DEFAULT 1
#    endif

#endif

#if TYPE_SAFE_USE_NOEXCEPT_DEFAULT
/// \exclude
#    define TYPE_SAFE_NOEXCEPT_DEFAULT(Val) noexcept(Val)
#else
/// \exclude
#    define TYPE_SAFE_NOEXCEPT_DEFAULT(Val)
#endif

#ifndef TYPE_SAFE_USE_CONSTEXPR14

#    if defined(__cpp_constexpr) && __cpp_constexpr >= 201304
/// \exclude
#        define TYPE_SAFE_USE_CONSTEXPR14 1
#    else
/// \exclude
#        define TYPE_SAFE_USE_CONSTEXPR14 0
#    endif

#endif

#if TYPE_SAFE_USE_CONSTEXPR14
/// \exclude
#    define TYPE_SAFE_CONSTEXPR14 constexpr
#else
/// \exclude
#    define TYPE_SAFE_CONSTEXPR14
#endif

#ifndef TYPE_SAFE_USE_EXCEPTIONS

#    if __cpp_exceptions
/// \exclude
#        define TYPE_SAFE_USE_EXCEPTIONS 1
#    elif defined(__GNUC__) && defined(__EXCEPTIONS)
/// \exclude
#        define TYPE_SAFE_USE_EXCEPTIONS 1
#    elif defined(_MSC_VER) && defined(_CPPUNWIND)
/// \exclude
#        define TYPE_SAFE_USE_EXCEPTIONS 1
#    else
/// \exclude
#        define TYPE_SAFE_USE_EXCEPTIONS 0
#    endif

#endif

#if TYPE_SAFE_USE_EXCEPTIONS
/// \exclude
#    define TYPE_SAFE_THROW(Ex) throw Ex
/// \exclude
#    define TYPE_SAFE_TRY try
/// \exclude
#    define TYPE_SAFE_CATCH_ALL catch (...)
/// \exclude
#    define TYPE_SAFE_RETHROW throw
#else

/// \exclude
namespace collie::ts
{
namespace detail
{
    void on_disabled_exception() noexcept;
}
} // namespace collie::ts

#    define TYPE_SAFE_THROW(Ex) (Ex, collie::ts::detail::on_disabled_exception())
#    define TYPE_SAFE_TRY if (true)
#    define TYPE_SAFE_CATCH_ALL if (false)
#    define TYPE_SAFE_RETHROW collie::ts::detail::on_disabled_exception()
#endif

#ifndef TYPE_SAFE_USE_RTTI

#    ifdef __GXX_RTTI
#        define TYPE_SAFE_USE_RTTI 1
#    elif defined(_CPPRTTI_)
#        define TYPE_SAFE_USE_RTTI 1
#    else
#        define TYPE_SAFE_USE_RTTI 0
#    endif

#endif

/// \entity type_safe
/// \unique_name ts
