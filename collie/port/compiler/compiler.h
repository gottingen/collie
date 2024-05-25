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

#include <collie/port/compiler/gnuc.h>
#include <collie/port/compiler/clang.h>
#include <collie/port/compiler/msc.h>
#include <collie/port/compiler/nvcc.h>

#ifndef COLLIE_COMPILER_NAME
#   error "unknown compiler"
#endif

#ifndef COLLIE_HAVE_FEATURE
#   if defined(__has_feature)
#       define COLLIE_HAVE_FEATURE(x) __has_feature(x)
#   else
#       define COLLIE_HAVE_FEATURE(x) 0
#   endif
#endif

// COLLIE_HAVE_ATTRIBUTE
//
// A function-like feature checking macro that is a wrapper around
// `__has_attribute`, which is defined by GCC 5+ and Clang and evaluates to a
// nonzero constant integer if the attribute is supported or 0 if not.
//
// It evaluates to zero if `__has_attribute` is not defined by the compiler.
//
// GCC: https://gcc.gnu.org/gcc-5/changes.html
// Clang: https://clang.llvm.org/docs/LanguageExtensions.html
#ifndef COLLIE_HAVE_ATTRIBUTE
#   ifdef __has_attribute
#       define COLLIE_HAVE_ATTRIBUTE(x) __has_attribute(x)
#   else
#       define COLLIE_HAVE_ATTRIBUTE(x) 0
#   endif
#endif

// COLLIE_HAVE_BUILTIN()
//
// Checks whether the compiler supports a Clang Feature Checking Macro, and if
// so, checks whether it supports the provided builtin function "x" where x
// is one of the functions noted in
// https://clang.llvm.org/docs/LanguageExtensions.html
//
// Note: Use this macro to avoid an extra level of #ifdef __has_builtin check.
// http://releases.llvm.org/3.3/tools/clang/docs/LanguageExtensions.html
#ifndef COLLIE_HAVE_BUILTIN
#   ifdef __has_builtin
#       define COLLIE_HAVE_BUILTIN(x) __has_builtin(x)
#   else
#       define COLLIE_HAVE_BUILTIN(x) 0
#   endif
#endif

// COLLIE_HAVE_CPP_ATTRIBUTE
//
// A function-like feature checking macro that accepts C++11 style attributes.
// It's a wrapper around `__has_cpp_attribute`, defined by ISO C++ SD-6
// (https://en.cppreference.com/w/cpp/experimental/feature_test). If we don't
// find `__has_cpp_attribute`, will evaluate to 0.
#ifndef COLLIE_HAVE_CPP_ATTRIBUTE
#   if defined(__cplusplus) && defined(__has_cpp_attribute)
        // NOTE: requiring __cplusplus above should not be necessary, but
        // works around https://bugs.llvm.org/show_bug.cgi?id=23435.
#       define COLLIE_HAVE_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#   else
#       define COLLIE_HAVE_CPP_ATTRIBUTE(x) 0
#   endif
#endif

// ------------------------------------------------------------------------
// COLLIE_HAVE_INCLUDE
//
// May be used in #if and #elif expressions to test for the existence
// of the header referenced in the operand. If possible it evaluates to a
// non-zero value and zero otherwise. The operand is the same form as the file
// in a #include directive.
//
// Example usage:
//
// #if COLLIE_HAVE_INCLUDE("myinclude.h")
//     #include "myinclude.h"
// #endif
//
// #if COLLIE_HAVE_INCLUDE(<myinclude.h>)
//     #include <myinclude.h>
// #endif

#if !defined(COLLIE_HAVE_INCLUDE)
#   if defined(COLLIE_COMPILER_CLANG)
#       define COLLIE_HAVE_INCLUDE(x) __has_include(x)
#   elif COLLIE_GNUC_VERSION > 0
#       define COLLIE_HAVE_INCLUDE(x) __has_include(x)
#   else
#       define COLLIE_HAVE_INCLUDE(x) 0
#   endif
#endif