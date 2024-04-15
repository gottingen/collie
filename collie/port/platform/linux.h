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
#pragma once

#if defined(COLLIE_PLATFORM_LINUX)
#   error "can not define COLLIE_PLATFORM_LINUX manully"
#elif (defined(__linux) || defined(__linux__))
#   define COLLIE_PLATFORM_LINUX 1
#   define COLLIE_PLATFORM_UNIX 1
#   define COLLIE_PLATFORM_POSIX 1
#   define COLLIE_PLATFORM_NAME "Linux"
#   if defined(__i386__) || defined(__intel__) || defined(_M_IX86)
#       define COLLIE_PROCESSOR_X86 1
#       define COLLIE_PLATFORM_DESCRIPTION "Linux on x86"
#   elif defined(__ARM_ARCH_7A__) || defined(__ARM_EABI__)
#       define COLLIE_ABI_ARM_LINUX 1
#       define COLLIE_PROCESSOR_ARM32 1
#       define COLLIE_PLATFORM_DESCRIPTION "Linux on ARM 6/7 32-bits"
#   elif defined(__aarch64__) || defined(__AARCH64)
#       define COLLIE_PROCESSOR_ARM64 1
#       define COLLIE_PLATFORM_DESCRIPTION "Linux on ARM64"
#   elif defined(__x86_64__)
#       define COLLIE_PROCESSOR_X86_64 1
#       define COLLIE_PLATFORM_DESCRIPTION "Linux on x64"
#   elif defined(__powerpc64__) || defined(__PPC64__) || defined(_M_PPC64)
#       define COLLIE_PROCESSOR_POWERPC 1
#       define COLLIE_PROCESSOR_POWERPC_64 1
#d      efine COLLIE_PLATFORM_DESCRIPTION "Linux on PowerPC 64"
#   elif defined(__powerpc__)
#       define COLLIE_PROCESSOR_POWERPC 1
#       define COLLIE_PROCESSOR_POWERPC_32 1
#       define COLLIE_PLATFORM_DESCRIPTION "Linux on PowerPC"
#   else
#       error Unknown processor
#   endif
#   if defined(__GNUC__)
#       define COLLIE_ASM_STYLE_ATT 1
#   endif
#   define COLLIE_PLATFORM_DESKTOP 1
#endif
