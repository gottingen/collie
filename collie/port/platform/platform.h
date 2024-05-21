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

#include <collie/port/platform/cygwin.h>
#include <collie/port/platform/android.h>
#include <collie/port/platform/linux.h>
#include <collie/port/platform/apple.h>
#include <collie/port/platform/windows.h>

#ifndef COLLIE_PLATFORM_NAME
#   error "unknown platform"
#endif

/**
 * @def COLLIE_PLATFORM_PTR_SIZE
 * @brief Platform pointer size; same as sizeof(void*).
 * This is not the same as sizeof(int), as int is usually 32 bits on
 * even 64 bit platforms.
 */
#ifndef COLLIE_PLATFORM_PTR_SIZE
#   if defined(__WORDSIZE) // Defined by some variations of GCC.
#       define COLLIE_PLATFORM_PTR_SIZE ((__WORDSIZE) / 8)
#   elif defined(_WIN64) || defined(__LP64__) || defined(_LP64) || defined(_M_IA64) || defined(__ia64__) || defined(__arch64__) || defined(__aarch64__) || defined(__mips64__) || defined(__64BIT__) || defined(__Ptr_Is_64)
#       define COLLIE_PLATFORM_PTR_SIZE 8
#   elif defined(__CC_ARM) && (__sizeof_ptr == 8)
#       define COLLIE_PLATFORM_PTR_SIZE 8
#   else
#       define COLLIE_PLATFORM_PTR_SIZE 4
#   endif
#endif

#ifndef COLLIE_PLATFORM_WORD_SIZE
#   define COLLIE_PLATFORM_WORD_SIZE COLLIE_PLATFORM_PTR_SIZE
#endif


// COLLIE_PLATFORM_MIN_MALLOC_ALIGNMENT
// This defines the minimal alignment that the platform's malloc 
// implementation will return. This should be used when writing custom
// allocators to ensure that the alignment matches that of malloc
#ifndef COLLIE_PLATFORM_MIN_MALLOC_ALIGNMENT
#   if defined(COLLIE_PLATFORM_APPLE)
#       define COLLIE_PLATFORM_MIN_MALLOC_ALIGNMENT 16
#   elif defined(COLLIE_PLATFORM_ANDROID) && defined(COLLIE_PROCESSOR_ARM)
#       define COLLIE_PLATFORM_MIN_MALLOC_ALIGNMENT 8
#   elif defined(COLLIE_PLATFORM_ANDROID) && defined(COLLIE_PROCESSOR_X86_64)
#       define COLLIE_PLATFORM_MIN_MALLOC_ALIGNMENT 8
#   else
#       define COLLIE_PLATFORM_MIN_MALLOC_ALIGNMENT (COLLIE_PLATFORM_PTR_SIZE * 2)
#   endif
#endif

// COLLIE_MISALIGNED_SUPPORT_LEVEL
// Specifies if the processor can read and write built-in types that aren't
// naturally aligned.
//    0 - not supported. Likely causes an exception.
//    1 - supported but slow.
//    2 - supported and fast.
//
#ifndef COLLIE_MISALIGNED_SUPPORT_LEVEL
#   if defined(COLLIE_PROCESSOR_X86_64)
#       define COLLIE_MISALIGNED_SUPPORT_LEVEL 2
#   else
#       define COLLIE_MISALIGNED_SUPPORT_LEVEL 0
#   endif
#endif

// Macro to determine if a Windows API partition is enabled. Always false on non Microsoft platforms.
#if !defined(COLLIE_WINAPI_FAMILY_PARTITION)
#   define COLLIE_WINAPI_FAMILY_PARTITION(Partition) (0)
#endif

/**
 * @def COLLIE_CACHE_LINE_SIZE
 * @brief Platform cache line size.
 *        This is the size of the cache line in bytes. This is the size of the
 *        cache line in bytes. This is the minimum possible value. This the
 *        expected best guess values for the targets that we can make at compilation time.
 */
#ifndef COLLIE_CACHE_LINE_SIZE
#   if   defined(COLLIE_PROCESSOR_X86)
#       define COLLIE_CACHE_LINE_SIZE 32    // This is the minimum possible value.
#   elif defined(COLLIE_PROCESSOR_X86_64)
#       define COLLIE_CACHE_LINE_SIZE 64    // This is the minimum possible value
#   elif defined(COLLIE_PROCESSOR_ARM32)
#       define COLLIE_CACHE_LINE_SIZE 32    // This varies between implementations and is usually 32 or 64.
#   elif defined(COLLIE_PROCESSOR_ARM64)
#       define COLLIE_CACHE_LINE_SIZE 64    // Cache line Cortex-A8  (64 bytes) http://shervinemami.info/armAssembly.html however this remains to be mostly an assumption at this stage
#   elif (COLLIE_PLATFORM_WORD_SIZE == 4)
#       define COLLIE_CACHE_LINE_SIZE 32    // This is the minimum possible value
#   else
#       define COLLIE_CACHE_LINE_SIZE 64    // This is the minimum possible value
#endif
#endif
