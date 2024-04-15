// Copyright 2023 The Elastic-AI Authors.
// part of Elastic AI Search
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//
#pragma once

#include <collie/port/compiler/compiler.h>

// COLLIE_ATTRIBUTE_NO_TAIL_CALL
//
// Prevents the compiler from optimizing away stack frames for functions which
// end in a call to another function.
#ifndef COLLIE_HAVE_ATTRIBUTE_NO_TAIL_CALL
#   if COLLIE_HAVE_ATTRIBUTE(disable_tail_calls)
#       define COLLIE_HAVE_ATTRIBUTE_NO_TAIL_CALL 1
#       define COLLIE_ATTRIBUTE_NO_TAIL_CALL __attribute__((disable_tail_calls))
#   elif defined(__GNUC__) && !defined(__clang__) && !defined(__e2k__)
#       define COLLIE_HAVE_ATTRIBUTE_NO_TAIL_CALL 1
#       define COLLIE_ATTRIBUTE_NO_TAIL_CALL                                           \
            __attribute__((optimize("no-optimize-sibling-calls")))
#   else
#       define COLLIE_ATTRIBUTE_NO_TAIL_CALL
#       define COLLIE_HAVE_ATTRIBUTE_NO_TAIL_CALL 0
#   endif
#endif // COLLIE_HAVE_ATTRIBUTE_NO_TAIL_CALL

// ------------------------------------------------------------------------
// COLLIE_WEAK
// COLLIE_WEAK_SUPPORTED -- defined as 0 or 1.
//
// GCC
// The weak attribute causes the declaration to be emitted as a weak
// symbol rather than a global. This is primarily useful in defining
// library functions which can be overridden in user code, though it
// can also be used with non-function declarations.
//
// VC++
// At link time, if multiple definitions of a COMDAT are seen, the linker
// picks one and discards the rest. If the linker option /OPT:REF
// is selected, then COMDAT elimination will occur to remove all the
// unreferenced data items in the linker output.
//
// Example usage:
//    COLLIE_WEAK void Function();
//
#ifndef COLLIE_WEAK
#   if defined(_MSC_VER) && (_MSC_VER >= 1300) // If VC7.0 and later
#       define COLLIE_WEAK __declspec(selectany)
#       define COLLIE_WEAK_SUPPORTED 1
#   elif defined(_MSC_VER) || (defined(__GNUC__) && defined(__CYGWIN__))
#       define COLLIE_WEAK
#       define COLLIE_WEAK_SUPPORTED 0
#   elif defined(COLLIE_COMPILER_ARM)  // Arm brand compiler for ARM CPU
#       define COLLIE_WEAK __weak
#       define COLLIE_WEAK_SUPPORTED 1
#   else                           // GCC and IBM compilers, others.
#       define COLLIE_WEAK __attribute__((weak))
#   define COLLIE_WEAK_SUPPORTED 1
#   endif
#endif  // COLLIE_WEAK

// ------------------------------------------------------------------------
// COLLIE_PURE
//
// This acts the same as the GCC __attribute__ ((pure)) directive and is
// implemented simply as a wrapper around it to allow portable usage of
// it and to take advantage of it if and when it appears in other compilers.
//
// A "pure" function is one that has no effects except its return value and
// its return value is a function of only the function's parameters or
// non-volatile global variables. Any parameter or global variable access
// must be read-only. Loop optimization and subexpression elimination can be
// applied to such functions. A common example is strlen(): Given identical
// inputs, the function's return value (its only effect) is invariant across
// multiple invocations and thus can be pulled out of a loop and called but once.
//
// Example usage:
//    COLLIE_PURE void Function();
//
#ifndef COLLIE_PURE
#   if defined(COLLIE_COMPILER_GNUC)
#       define COLLIE_PURE __attribute__((pure))
#   else
#       define COLLIE_PURE
#   endif
#endif

// COLLIE_DLL
//
// When building Turbo as a DLL, this macro expands to `__declspec(dllexport)`
// so we can annotate symbols appropriately as being exported. When used in
// headers consuming a DLL, this macro expands to `__declspec(dllimport)` so
// that consumers know the symbol is defined inside the DLL. In all other cases,
// the macro expands to nothing.
#if defined(_MSC_VER)
#   if defined(COLLIE_BUILD_DLL)
#       define COLLIE_DLL __declspec(dllexport)
#   elif defined(COLLIE_CONSUME_DLL)
#       define COLLIE_DLL __declspec(dllimport)
#   else
#       define COLLIE_DLL
#   endif
#else
#   define COLLIE_DLL
//#define COLLIE_DLL __attribute__ ((visibility("default")))
#endif  // defined(_MSC_VER)

#ifndef COLLIE_HIDDEN
#   if defined(_MSC_VER)
#       define COLLIE_HIDDEN
#   elif defined(__CYGWIN__)
#       define COLLIE_HIDDEN
#   elif COLLIE_GNUC_VERSION > 400 || COLLIE_HAVE_ATTRIBUTE(visibility)
#       define COLLIE_HIDDEN    __attribute__ ((visibility("hidden")))
#   else
#       define COLLIE_HIDDEN
#   endif
#endif  // COLLIE_HIDDEN

#ifndef COLLIE_HOT
#   if COLLIE_HAVE_ATTRIBUTE(hot) || (defined(__GNUC__) && !defined(__clang__))
#       define COLLIE_HOT __attribute__((hot))
#   else
#       define COLLIE_HOT
#   endif
#endif

#ifndef COLLIE_COLD
#   if COLLIE_HAVE_ATTRIBUTE(cold) || (defined(__GNUC__) && !defined(__clang__))
#       define COLLIE_COLD __attribute__((cold))
#   else
#       define COLLIE_COLD
#   endif
#endif


// COLLIE_ATTRIBUTE_TRIVIAL_ABI
// Indicates that a type is "trivially relocatable" -- meaning it can be
// relocated without invoking the constructor/destructor, using a form of move
// elision.
//
// From a memory safety point of view, putting aside destructor ordering, it's
// safe to apply COLLIE_ATTRIBUTE_TRIVIAL_ABI if an object's location
// can change over the course of its lifetime: if a constructor can be run one
// place, and then the object magically teleports to another place where some
// methods are run, and then the object teleports to yet another place where it
// is destroyed. This is notably not true for self-referential types, where the
// move-constructor must keep the self-reference up to date. If the type changed
// location without invoking the move constructor, it would have a dangling
// self-reference.
//
// The use of this teleporting machinery means that the number of paired
// move/destroy operations can change, and so it is a bad idea to apply this to
// a type meant to count the number of moves.
//
// Warning: applying this can, rarely, break callers. Objects passed by value
// will be destroyed at the end of the call, instead of the end of the
// full-expression containing the call. In addition, it changes the ABI
// of functions accepting this type by value (e.g. to pass in registers).
//
// See also the upstream documentation:
// https://clang.llvm.org/docs/AttributeReference.html#trivial-abi
//
#ifndef COLLIE_ATTRIBUTE_TRIVIAL_ABI
#   if COLLIE_HAVE_CPP_ATTRIBUTE(clang::trivial_abi)
#       define COLLIE_ATTRIBUTE_TRIVIAL_ABI [[clang::trivial_abi]]
#       define COLLIE_HAVE_ATTRIBUTE_TRIVIAL_ABI 1
#   elif COLLIE_HAVE_ATTRIBUTE(trivial_abi)
#       define COLLIE_ATTRIBUTE_TRIVIAL_ABI __attribute__((trivial_abi))
#       define COLLIE_HAVE_ATTRIBUTE_TRIVIAL_ABI 1
#   else
#       define COLLIE_ATTRIBUTE_TRIVIAL_ABI
#   endif
#endif


// COLLIE_CONST_INIT
//
// A variable declaration annotated with the `COLLIE_CONST_INIT` attribute will
// not compile (on supported platforms) unless the variable has a constant
// initializer. This is useful for variables with static and thread storage
// duration, because it guarantees that they will not suffer from the so-called
// "static init order fiasco".
//
// This attribute must be placed on the initializing declaration of the
// variable. Some compilers will give a -Wmissing-constinit warning when this
// attribute is placed on some other declaration but missing from the
// initializing declaration.
//
// In some cases (notably with thread_local variables), `COLLIE_CONST_INIT` can
// also be used in a non-initializing declaration to tell the compiler that a
// variable is already initialized, reducing overhead that would otherwise be
// incurred by a hidden guard variable. Thus annotating all declarations with
// this attribute is recommended to potentially enhance optimization.
//
// Example:
//
//   class MyClass {
//    public:
//     COLLIE_CONST_INIT static MyType my_var;
//   };
//
//   COLLIE_CONST_INIT MyType MyClass::my_var = MakeMyType(...);
//
// For code or headers that are assured to only build with C++20 and up, prefer
// just using the standard `constinit` keyword directly over this macro.
//
// Note that this attribute is redundant if the variable is declared constexpr.
#if defined(__cpp_constinit) && __cpp_constinit >= 201907L
#   define COLLIE_CONST_INIT constinit
#elif COLLIE_HAVE_CPP_ATTRIBUTE(clang::require_constant_initialization)
#   define COLLIE_CONST_INIT [[clang::require_constant_initialization]]
#else
#   define COLLIE_CONST_INIT
#endif

#ifndef COLLIE_MAYBE_UNUSED
#   if COLLIE_HAVE_ATTRIBUTE(unused)
#       define COLLIE_MAYBE_UNUSED __attribute__((unused))
#   else
#       define COLLIE_MAYBE_UNUSED
#   endif
#endif // COLLIE_MAYBE_UNUSED

// COLLIE_FUNC_ALIGN
//
// Tells the compiler to align the function start at least to certain
// alignment boundary
#if COLLIE_HAVE_ATTRIBUTE(aligned) || (defined(__GNUC__) && !defined(__clang__))
#   define COLLIE_FUNC_ALIGN(bytes) __attribute__((aligned(bytes)))
#else
#   define COLLIE_FUNC_ALIGN(bytes)
#endif

/**
 * @ingroup turbo_function_macro
 * @brief This macro is used to force inline a function.
 * @details The `COLLIE_FORCE_INLINE` macro is used to force inline a function.
 *          This attribute is used to inform the compiler that a function is a
 *          hot spot in the program. The function is optimized more aggressively
 *          and on many platforms special calling sequences are used. For
 *          example, on ARM targets a function marked as hot is expected to
 *          be called more often than a cold function. This attribute is
 *          often used to mark functions that are called frequently inside
 *          loops.
 * @code
 *          TUROB_FORCE_INLINE int foo();
 * @endcode
 * @note    This attribute is not implemented in GCC versions earlier than
 *         3.1.0. This attribute is not implemented in Clang versions earlier
 *         than 3.0.0. This attribute is not implemented in MSVC. This
 *         attribute is not implemented in ICC. This attribute is not
 *         implemented in IBM XL C/C++. This attribute is not implemented in
 *         TI C/C++. This attribute is not implemented in ARM C/C++. This
 *         attribute is not implemented in TI C/C++. This attribute is not
 *         implemented in TI C/C++.
 */
#ifndef COLLIE_FORCE_INLINE
#   if defined(COLLIE_COMPILER_MSVC)
#       define COLLIE_FORCE_INLINE __forceinline
#   elif COLLIE_GNUC_VERSION > 300 || COLLIE_CLANG_VERSION > 0
#       if defined(__cplusplus)
#           define COLLIE_FORCE_INLINE inline __attribute__((always_inline))
#       else
#           define COLLIE_FORCE_INLINE __inline__ __attribute__((always_inline))
#       endif
#   else
#       if defined(__cplusplus)
#           define COLLIE_FORCE_INLINE inline
#       else
#           define COLLIE_FORCE_INLINE __inline
#       endif
#   endif
#endif


/**
 * @ingroup turbo_function_macro
 * @brief This macro is used to force inline a lambda function. Force inlining
 *        a lambda can be useful to reduce overhead in situations where a lambda
 *        may may only be called once, or inlining allows the compiler to apply
 *        other optimizations that wouldn't otherwise be possible.
 *
 *        The ability to force inline a lambda is currently only available on a
 *        subset of compilers.
 *
 *        Example usage:
 *        @code {.cpp}
 *        auto lambdaFunction = []() COLLIE_FORCE_INLINE_LAMBDA
 *        {
 *        };
 *        @endcode
 */
#ifndef COLLIE_FORCE_INLINE_LAMBDA
#   if defined(COLLIE_COMPILER_GNUC) || defined(COLLIE_COMPILER_CLANG)
#       define COLLIE_FORCE_INLINE_LAMBDA __attribute__((always_inline))
#   else
#       define COLLIE_FORCE_INLINE_LAMBDA
#   endif
#endif


// ------------------------------------------------------------------------
// COLLIE_NO_INLINE             // Used as a prefix.
// unusual compilers.
//
// Example usage:
//     COLLIE_NO_INLINE        void Foo();                       //
//     Implementation elsewhere. COLLIE_PREFIX_NO_INLINE void Foo()
//     COLLIE_POSTFIX_NO_INLINE;  // Implementation elsewhere.
//
// That this declaration is incompatbile with C++ 'inline' and any
// variant of COLLIE_FORCE_INLINE.
//
// To disable inline usage under VC++ priof to VS2005, you need to use this:
//    #pragma inline_depth(0) // Disable inlining.
//    void Foo() { ... }
//    #pragma inline_depth()  // Restore to default.
//
// Since there is no easy way to disable inlining on a function-by-function
// basis in VC++ prior to VS2005, the best strategy is to write
// platform-specific #ifdefs in the code or to disable inlining for a given
// module and enable functions individually with COLLIE_FORCE_INLINE.
//
#ifndef COLLIE_NO_INLINE
#   if defined(COLLIE_COMPILER_MSVC) && (COLLIE_COMPILER_VERSION >= 1400) // If VC8 (VS2005) or later...
#       define COLLIE_NO_INLINE __declspec(noinline)
#       define COLLIE_NO_INLINE_SUPPORTED 1
#   elif defined(COLLIE_COMPILER_MSVC)
#       define COLLIE_NO_INLINE
#   else
#       define COLLIE_NO_INLINE_SUPPORTED 1
#       define COLLIE_NO_INLINE __attribute__((noinline))
#   endif
#endif

#ifndef COLLIE_INLINE_VISIBILITY
#   define COLLIE_INLINE_VISIBILITY  COLLIE_HIDDEN COLLIE_FORCE_INLINE
#endif  // COLLIE_INLINE_VISIBILITY


// ------------------------------------------------------------------------
// COLLIE_CARRIES_DEPENDENCY
//
// Wraps the C++11 carries_dependency attribute
// http://en.cppreference.com/w/cpp/language/attributes
// http://blog.aaronballman.com/2011/09/understanding-attributes/
//
// Example usage:
//     COLLIE_CARRIES_DEPENDENCY int* SomeFunction()
//         { return &mX; }
//
//
#if !defined(COLLIE_CARRIES_DEPENDENCY)
#   if defined(COLLIE_COMPILER_NO_CARRIES_DEPENDENCY)
#       define COLLIE_CARRIES_DEPENDENCY
#   else
#       define COLLIE_CARRIES_DEPENDENCY [[carries_dependency]]
#   endif
#endif


// COLLIE_NONNULL
//
// Tells the compiler either (a) that a particular function parameter
// should be a non-null pointer, or (b) that all pointer arguments should
// be non-null.
//
// Note: As the GCC manual states, "[s]ince non-static C++ methods
// have an implicit 'this' argument, the arguments of such methods
// should be counted from two, not one."
//
// Args are indexed starting at 1.
//
// For non-static class member functions, the implicit `this` argument
// is arg 1, and the first explicit argument is arg 2. For static class member
// functions, there is no implicit `this`, and the first explicit argument is
// arg 1.
//
// Example:
//
//   /* arg_a cannot be null, but arg_b can */
//   void Function(void* arg_a, void* arg_b) COLLIE_NONNULL(1);
//
//   class C {
//     /* arg_a cannot be null, but arg_b can */
//     void Method(void* arg_a, void* arg_b) COLLIE_NONNULL(2);
//
//     /* arg_a cannot be null, but arg_b can */
//     static void StaticMethod(void* arg_a, void* arg_b)
//     COLLIE_NONNULL(1);
//   };
//
// If no arguments are provided, then all pointer arguments should be non-null.
//
//  /* No pointer arguments may be null. */
//  void Function(void* arg_a, void* arg_b, int arg_c) COLLIE_NONNULL();
//
// NOTE: The GCC nonnull attribute actually accepts a list of arguments, but
// COLLIE_NONNULL does not.
#ifndef COLLIE_NONNULL
#   if COLLIE_HAVE_ATTRIBUTE(nonnull) || (defined(__GNUC__) && !defined(__clang__))
#       define COLLIE_NONNULL(arg_index) __attribute__((nonnull(arg_index)))
#   else
#       define COLLIE_NONNULL(...)
#   endif
#endif


// ------------------------------------------------------------------------
// COLLIE_NORETURN
//
// Wraps the C++11 noreturn attribute. See COLLIE_COMPILER_NO_NORETURN
// http://en.cppreference.com/w/cpp/language/attributes
// http://msdn.microsoft.com/en-us/library/k6ktzx3s%28v=vs.80%29.aspx
// http://blog.aaronballman.com/2011/09/understanding-attributes/
//
// Example usage:
//     COLLIE_NORETURN void SomeFunction()
//         { throw "error"; }
//
// Disable [[noreturn]] on MSVC/NVCC because of bogus unreachable code warnings.
#if FMT_EXCEPTIONS && COLLIE_HAVE_CPP_ATTRIBUTE(noreturn) && !FMT_MSC_VERSION && !defined(__NVCC__)
#  define FMT_NORETURN [[noreturn]]
#else
#  define FMT_NORETURN
#endif