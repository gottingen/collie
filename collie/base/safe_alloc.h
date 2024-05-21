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


#ifndef COLLIE_BASE_SAFE_ALLOC_H_
#define COLLIE_BASE_SAFE_ALLOC_H_

#include <collie/base/macros.h>
#include <collie/base/debug_assert.h>
#include <memory>

#ifndef COLLIE_SAFE_ENABLE_ASSERTIONS
#define COLLIE_SAFE_ALLOC_ASSERTIONS 0
#endif

namespace collie {

    struct alloc_assert_handler : collie::debug_assert::set_level<COLLIE_SAFE_ALLOC_ASSERTIONS>,
                            collie::debug_assert::default_handler {
    };

    COLLIE_ATTRIBUTE_RETURNS_NONNULL inline void *safe_malloc(size_t Sz) {
        void *Result = std::malloc(Sz);
        if (Result == nullptr) {
            // It is implementation-defined whether allocation occurs if the space
            // requested is zero (ISO/IEC 9899:2018 7.22.3). Retry, requesting
            // non-zero, if the space requested was zero.
            if (Sz == 0) {
                return safe_malloc(1);
            }
            DEBUG_ASSERT(false,  alloc_assert_handler{}, "Allocation failed");
        }
        return Result;
    }

    COLLIE_ATTRIBUTE_RETURNS_NONNULL inline void *safe_calloc(size_t Count,
                                                            size_t Sz) {
        void *Result = std::calloc(Count, Sz);
        if (Result == nullptr) {
            // It is implementation-defined whether allocation occurs if the space
            // requested is zero (ISO/IEC 9899:2018 7.22.3). Retry, requesting
            // non-zero, if the space requested was zero.
            if (Count == 0 || Sz == 0)
                return safe_malloc(1);
            DEBUG_ASSERT(false,  alloc_assert_handler{}, "Allocation failed");
        }
        return Result;
    }

    COLLIE_ATTRIBUTE_RETURNS_NONNULL inline void *safe_realloc(void *Ptr, size_t Sz) {
        void *Result = std::realloc(Ptr, Sz);
        if (Result == nullptr) {
            // It is implementation-defined whether allocation occurs if the space
            // requested is zero (ISO/IEC 9899:2018 7.22.3). Retry, requesting
            // non-zero, if the space requested was zero.
            if (Sz == 0)
                return safe_malloc(1);
            DEBUG_ASSERT(false,  alloc_assert_handler{}, "Allocation failed");
        }
        return Result;
    }

}  // namespace collie

#endif  // COLLIE_BASE_SAFE_ALLOC_H_
