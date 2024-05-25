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

#ifndef COLLIE_SIMD_MEMORY_ALLOCATOR_H_
#define COLLIE_SIMD_MEMORY_ALLOCATOR_H_

#include <collie/memory/aligned_allocator.h>
#include <collie/simd/types/simd_utils.h>
#include <collie/simd/config/simd_arch.h>

namespace collie::simd {
    template <class T, class A = default_arch>
    using default_allocator = typename std::conditional<A::requires_alignment(),
                                                        collie::aligned_allocator<T, A::alignment()>,
                                                        std::allocator<T>>::type;

    /**
  * @struct aligned_mode
  * @brief tag for load and store of aligned memory.
  */
    struct aligned_mode {
    };

    /**
     * @struct unaligned_mode
     * @brief tag for load and store of unaligned memory.
     */
    struct unaligned_mode {
    };

    /***********************
     * Allocator alignment *
     ***********************/

    template<class A>
    struct allocator_alignment {
        using type = unaligned_mode;
    };

    template<class T, size_t N>
    struct allocator_alignment<collie::aligned_allocator<T, N>> {
        using type = aligned_mode;
    };

    template<class A>
    using allocator_alignment_t = typename allocator_alignment<A>::type;

    /***********************
     * container alignment *
     ***********************/

    template<class C, class = void>
    struct container_alignment {
        using type = unaligned_mode;
    };

    template<class C>
    struct container_alignment<C, detail::void_t<typename C::allocator_type>> {
        using type = allocator_alignment_t<typename C::allocator_type>;
    };

    template<class C>
    using container_alignment_t = typename container_alignment<C>::type;

    /*********************
     * alignment checker *
     *********************/

    /**
     * Checks whether pointer \c ptr is aligned according the alignment
     * requirements of \c Arch.
     * @return true if the alignment requirements are met
     */
    template<class Arch = default_arch>
    inline bool is_aligned(void const *ptr) {
        return (reinterpret_cast<uintptr_t>(ptr) % static_cast<uintptr_t>(Arch::alignment())) == 0;
    }
}  // namespace collie::simd

#endif  // COLLIE_SIMD_MEMORY_ALLOCATOR_H_
