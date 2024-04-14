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

#if defined(COLLIE_PLATFORM_ANDROID)
#   error "can not define COLLIE_PLATFORM_ANDROID manully"
#elif defined(__ANDROID__)
#   define COLLIE_PLATFORM_ANDROID 1
#   define COLLIE_PLATFORM_LINUX 1
#   define COLLIE_PLATFORM_UNIX 1
#   define COLLIE_PLATFORM_POSIX 1
#   define COLLIE_PLATFORM_NAME "Android"
#   define COLLIE_ASM_STYLE_ATT 1
#   if defined(__arm__)
#       define COLLIE_ABI_ARM_LINUX 1  // a.k.a. "ARM eabi"
#       define COLLIE_PROCESSOR_ARM32 1
#       define COLLIE_PLATFORM_DESCRIPTION "Android on ARM"
#   elif defined(__aarch64__)
#       define COLLIE_PROCESSOR_ARM64 1
#       define COLLIE_PLATFORM_DESCRIPTION "Android on ARM64"
#   elif defined(__i386__)
#       define COLLIE_PROCESSOR_X86 1
#       define COLLIE_PLATFORM_DESCRIPTION "Android on x86"
#   elif defined(__x86_64)
#       define COLLIE_PROCESSOR_X86_64 1
#       define COLLIE_PLATFORM_DESCRIPTION "Android on x64"
#   else
#       error Unknown processor
#   endif
#   define COLLIE_PLATFORM_MOBILE 1
#endif