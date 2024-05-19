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

#include <cstring>
#include <type_traits>
#include <utility>
#include <new>
#include <collie/base/macros.h>

namespace collie {
    namespace internal {
#if __cpp_lib_launder
        using std::launder;
#else
        template <class T>
        constexpr T* launder(T* p) noexcept {
            return p;
        }
#endif

        template<typename T>
        class AlignedStorage {
        public:
            static constexpr bool can_memcpy = std::is_trivial<T>::value;

            constexpr T *get() noexcept {
                return collie::internal::launder(reinterpret_cast<T *>(&data_));
            }

            constexpr const T *get() const noexcept {
                return collie::internal::launder(reinterpret_cast<const T *>(&data_));
            }

            void destroy() noexcept {
                if (!std::is_trivially_destructible<T>::value) {
                    get()->~T();
                }
            }

            template<typename... A>
            void construct(A &&... args) noexcept {
                new(&data_) T(std::forward<A>(args)...);
            }

            template<typename V>
            void assign(V &&v) noexcept {
                *get() = std::forward<V>(v);
            }

            void move_construct(AlignedStorage *other) noexcept {
                new(&data_) T(std::move(*other->get()));
            }

            void move_assign(AlignedStorage *other) noexcept { *get() = std::move(*other->get()); }

            template<bool CanMemcpy = can_memcpy>
            static typename std::enable_if<CanMemcpy>::type move_construct_several(
                    AlignedStorage *COLLIE_RESTRICT src, AlignedStorage *COLLIE_RESTRICT dest, size_t n,
                    size_t memcpy_length) noexcept {
                memcpy(dest->get(), src->get(), memcpy_length * sizeof(T));
            }

            template<bool CanMemcpy = can_memcpy>
            static typename std::enable_if<CanMemcpy>::type
            move_construct_several_and_destroy_source(AlignedStorage *COLLIE_RESTRICT src,
                                                      AlignedStorage *COLLIE_RESTRICT dest, size_t n,
                                                      size_t memcpy_length) noexcept {
                memcpy(dest->get(), src->get(), memcpy_length * sizeof(T));
            }

            template<bool CanMemcpy = can_memcpy>
            static typename std::enable_if<!CanMemcpy>::type move_construct_several(
                    AlignedStorage *COLLIE_RESTRICT src, AlignedStorage *COLLIE_RESTRICT dest, size_t n,
                    size_t memcpy_length) noexcept {
                for (size_t i = 0; i < n; ++i) {
                    new(dest[i].get()) T(std::move(*src[i].get()));
                }
            }

            template<bool CanMemcpy = can_memcpy>
            static typename std::enable_if<!CanMemcpy>::type
            move_construct_several_and_destroy_source(AlignedStorage *COLLIE_RESTRICT src,
                                                      AlignedStorage *COLLIE_RESTRICT dest, size_t n,
                                                      size_t memcpy_length) noexcept {
                for (size_t i = 0; i < n; ++i) {
                    new(dest[i].get()) T(std::move(*src[i].get()));
                    src[i].destroy();
                }
            }

            static void move_construct_several(AlignedStorage *COLLIE_RESTRICT src,
                                               AlignedStorage *COLLIE_RESTRICT dest,
                                               size_t n) noexcept {
                move_construct_several(src, dest, n, n);
            }

            static void move_construct_several_and_destroy_source(
                    AlignedStorage *COLLIE_RESTRICT src, AlignedStorage *COLLIE_RESTRICT dest,
                    size_t n) noexcept {
                move_construct_several_and_destroy_source(src, dest, n, n);
            }

            static void destroy_several(AlignedStorage *p, size_t n) noexcept {
                if (!std::is_trivially_destructible<T>::value) {
                    for (size_t i = 0; i < n; ++i) {
                        p[i].destroy();
                    }
                }
            }

        private:
#if !defined(__clang__) && defined(__GNUC__) && defined(__i386__)
            // Workaround for GCC bug on i386:
  //   alignof(int64 | float64) can give different results depending on the
  //   compilation context, leading to internal ABI mismatch manifesting
  //   in incorrect propagation of Result<int64 | float64> between
  //   compilation units.
  // (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=88115)
  static constexpr size_t alignment() {
    if (std::is_integral_v<T> && sizeof(T) == 8) {
      return 4;
    } else if (std::is_floating_point_v<T> && sizeof(T) == 8) {
      return 4;
    }
    return alignof(T);
  }

  typename std::aligned_storage<sizeof(T), alignment()>::type data_;
#else
            typename std::aligned_storage<sizeof(T), alignof(T)>::type data_;
#endif
        };

    }  // namespace internal
}  // namespace collie
