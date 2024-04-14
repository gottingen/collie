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

// Cygwin
// This is a pseudo-platform which will be defined along with TURBO_PLATFORM_LINUX when
// using the Cygwin build environment.
#if defined(__CYGWIN__)
#   define COLLIE_PLATFORM_CYGWIN 1
#   define COLLIE_PLATFORM_DESKTOP 1
#endif

// MinGW
// This is a pseudo-platform which will be defined along with TURBO_PLATFORM_WINDOWS when
// using the MinGW Windows build environment.
#if defined(__MINGW32__) || defined(__MINGW64__)
#   define COLLIE_PLATFORM_MINGW 1
#   define COLLIE_PLATFORM_DESKTOP 1
#endif