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

#ifndef COLLIE_MACRO_MACROS_H_
#define COLLIE_MACRO_MACROS_H_

#ifndef COLLIE_LIKELY
#if defined(__GNUC__)
#define COLLIE_LIKELY(x) (__builtin_expect((x), 1))
#define COLLIE_UNLIKELY(x) (__builtin_expect((x), 0))
#else
#define COLLIE_LIKELY(x) (x)
  #define COLLIE_UNLIKELY(x) (x)
#endif
#endif

#endif  // COLLIE_MACRO_MACROS_H_
