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

#ifndef COLLIE_BASE_MACROS_H_
#define COLLIE_BASE_MACROS_H_

#ifndef COLLIE_LIKELY
#if defined(__GNUC__)
#define COLLIE_LIKELY(x) (__builtin_expect((x), 1))
#define COLLIE_UNLIKELY(x) (__builtin_expect((x), 0))
#else
#define COLLIE_LIKELY(x) (x)
#define COLLIE_UNLIKELY(x) (x)
#endif
#endif

#ifdef __has_builtin
#  define COLLIE_HAS_BUILTIN(x) __has_builtin(x)
#else
#  define COLLIE_HAS_BUILTIN(x) 0
#endif

/// COLLIE_BUILTIN_UNREACHABLE - On compilers which support it, expands
/// to an expression which states that it is undefined behavior for the
/// compiler to reach this point.  Otherwise is not defined.
///
/// '#else' is intentionally left out so that other macro logic (e.g.,
/// COLLIE_ASSUME_ALIGNED and llvm_unreachable()) can detect whether
/// COLLIE_BUILTIN_UNREACHABLE has a definition.
#if COLLIE_HAS_BUILTIN(__builtin_unreachable) || defined(__GNUC__)
# define COLLIE_BUILTIN_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
# define COLLIE_BUILTIN_UNREACHABLE __assume(false)
#endif

/// \macro COLLIE_ASSUME_ALIGNED
/// Returns a pointer with an assumed alignment.
#if COLLIE_HAS_BUILTIN(__builtin_assume_aligned) || defined(__GNUC__)
# define COLLIE_ASSUME_ALIGNED(p, a) __builtin_assume_aligned(p, a)
#elif defined(COLLIE_BUILTIN_UNREACHABLE)
# define COLLIE_ASSUME_ALIGNED(p, a) \
           (((uintptr_t(p) % (a)) == 0) ? (p) : (COLLIE_BUILTIN_UNREACHABLE, (p)))
#else
# define COLLIE_ASSUME_ALIGNED(p, a) (p)
#endif

#ifdef __has_attribute
#define COLLIE_HAS_ATTR(attr) __has_attribute(attr)
#else
#define COLLIE_HAS_ATTR(attr) 0
#endif
#if defined(_MSC_VER)
#define COLLIE_ATTRIBUTE_RETURNS_NONNULL _Ret_notnull_
#elif COLLIE_HAS_ATTR(returns_nonnull)
#define COLLIE_ATTRIBUTE_RETURNS_NONNULL __attribute__((returns_nonnull))
#else
#define COLLIE_ATTRIBUTE_RETURNS_NONNULL
#endif

// ------------------------------------------------------------------------
// COLLIE_CONCAT
//
// This macro joins the two arguments together, even when one of
// the arguments is itself a macro (see 16.3.1 in C++98 standard).
// This is often used to create a unique name with __LINE__.
//
// For example, this declaration:
//    char COLLIE_CONCAT(unique_, __LINE__);
// expands to this:
//    char unique_73;
//
// Note that all versions of MSVC++ up to at least version 7.1
// fail to properly compile macros that use __LINE__ in them
// when the "program database for edit and continue" option
// is enabled. The result is that __LINE__ gets converted to
// something like __LINE__(Var+37).
//
#ifndef COLLIE_CONCAT
#define COLLIE_CONCAT(a, b)  TURBO_CONCAT1(a, b)
#define TURBO_CONCAT1(a, b) TURBO_CONCAT2(a, b)
#define TURBO_CONCAT2(a, b) a##b
#endif

#define COLLIE_STRINGIFY(x) #x

#if defined(_MSC_VER)
#  define COLLIE_MSVC_PUSH_DISABLE_WARNING(n) \
    __pragma(warning(push)) __pragma(warning(disable : n))
#  define COLLIE_MSVC_POP_WARNING() __pragma(warning(pop))
#else
#  define COLLIE_MSVC_PUSH_DISABLE_WARNING(n)
#  define COLLIE_MSVC_POP_WARNING()
#endif

#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
#define COLLIE_RESTRICT __restrict
#else
#define COLLIE_RESTRICT
#endif

#endif  // COLLIE_BASE_MACROS_H_
