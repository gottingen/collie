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

#if defined(COLLIE_PLATFORM_APPLE) || defined(COLLIE_PLATFORM_IPHONE) || defined(COLLIE_PLATFORM_OSX)
#   error "can not define COLLIE_PLATFORM_APPLE manully"
#elif defined(__APPLE__) && __APPLE__
#include <TargetConditionals.h>

// Apple family of operating systems.
#   define COLLIE_PLATFORM_APPLE
#   define COLLIE_PLATFORM_POSIX 1

// iPhone
// TARGET_OS_IPHONE will be undefined on an unknown compiler, and will be defined on gcc.
#   if defined(COLLIE_PLATFORM_IPHONE) || defined(__IPHONE__) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR)
#       define COLLIE_PLATFORM_IPHONE 1
#       define COLLIE_PLATFORM_NAME "iPhone"
#       define COLLIE_ASM_STYLE_ATT 1
#       define COLLIE_POSIX_THREADS_AVAILABLE 1
#       if defined(__arm__)
#           define COLLIE_ABI_ARM_APPLE 1
#           define COLLIE_PROCESSOR_ARM32 1
#           define COLLIE_PLATFORM_DESCRIPTION "iPhone on ARM"
#       elif defined(__aarch64__) || defined(__AARCH64)
#           define COLLIE_ABI_ARM64_APPLE 1
#           define COLLIE_PROCESSOR_ARM64 1
#           define COLLIE_PLATFORM_DESCRIPTION "iPhone on ARM64"
#       elif defined(__i386__)
#           define COLLIE_PLATFORM_IPHONE_SIMULATOR 1
#           define COLLIE_PROCESSOR_X86 1
#           define COLLIE_PLATFORM_DESCRIPTION "iPhone simulator on x86"
#       elif defined(__x86_64) || defined(__amd64)
#           define COLLIE_PROCESSOR_X86_64 1
#           define COLLIE_PLATFORM_DESCRIPTION "iPhone simulator on x64"
#       else
#           error Unknown processor
#       endif
#   define COLLIE_PLATFORM_MOBILE 1

    // Macintosh OSX
    // TARGET_OS_MAC is defined by the Metrowerks and older AppleC compilers.
    // Howerver, TARGET_OS_MAC is defined to be 1 in all cases.
    // __i386__ and __intel__ are defined by the GCC compiler.
    // __dest_os is defined by the Metrowerks compiler.
    // __MACH__ is defined by the Metrowerks and GCC compilers.
    // powerc and __powerc are defined by the Metrowerks and GCC compilers.
#   elif defined(__MACH__) || (defined(__MSL__) && (__dest_os == __mac_os_x))
#       define COLLIE_PLATFORM_OSX 1
#       define COLLIE_PLATFORM_UNIX 1
#       define COLLIE_PLATFORM_POSIX 1
      //#define COLLIE_PLATFORM_BSD 1           We don't currently define this. OSX has some BSD history but a lot of the API is different.
#       define COLLIE_PLATFORM_NAME "OSX"
#       if defined(__i386__) || defined(__intel__)
#           define COLLIE_PROCESSOR_X86 1
#           define COLLIE_PLATFORM_DESCRIPTION "OSX on x86"
#       elif defined(__x86_64) || defined(__amd64)
#           define COLLIE_PROCESSOR_X86_64 1
#           define COLLIE_PLATFORM_DESCRIPTION "OSX on x64"
#       elif defined(__arm__)
#           define COLLIE_ABI_ARM_APPLE 1
#           define COLLIE_PROCESSOR_ARM32 1
#           define COLLIE_PLATFORM_DESCRIPTION "OSX on ARM"
#       elif defined(__aarch64__) || defined(__AARCH64)
#           define COLLIE_ABI_ARM64_APPLE 1
#           define COLLIE_PROCESSOR_ARM64 1
#           define COLLIE_PLATFORM_DESCRIPTION "OSX on ARM64"
#      elif defined(__POWERPC64__) || defined(__powerpc64__)
#           define COLLIE_PROCESSOR_POWERPC 1
#           define COLLIE_PROCESSOR_POWERPC_64 1
#           define COLLIE_PLATFORM_DESCRIPTION "OSX on PowerPC 64"
#      elif defined(__POWERPC__) || defined(__powerpc__)
#           define COLLIE_PROCESSOR_POWERPC 1
#           define COLLIE_PROCESSOR_POWERPC_32 1
#           define COLLIE_PLATFORM_DESCRIPTION "OSX on PowerPC"
#       else
#           error Unknown processor
#       endif
#       if defined(__GNUC__)
#           define COLLIE_ASM_STYLE_ATT 1
#       else
#           define COLLIE_ASM_STYLE_MOTOROLA 1
#       endif
#       define COLLIE_PLATFORM_DESKTOP 1
#   else
#       error Unknown Apple Platform
#   endif
#endif