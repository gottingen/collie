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

#ifndef COLLIE_SIMD_TYPES_GENERIC_ARCH_H_
#define COLLIE_SIMD_TYPES_GENERIC_ARCH_H_

#include <collie/port/simd_config.h>

/**
 * @defgroup architectures Architecture description
 * */
namespace collie::simd {
    /**
     * @ingroup architectures
     *
     * Base class for all architectures.
     */
    struct generic {
        /// Whether this architecture is supported at compile-time.
        static constexpr bool supported() noexcept { return true; }

        /// Whether this architecture is available at run-time.
        static constexpr bool available() noexcept { return true; }

        /// If this architectures supports aligned memory accesses, the required
        /// alignment.
        static constexpr std::size_t alignment() noexcept { return 0; }

        /// Whether this architecture requires aligned memory access.
        static constexpr bool requires_alignment() noexcept { return false; }

        /// Unique identifier for this architecture.
        static constexpr unsigned version() noexcept { return generic::version(0, 0, 0); }

        /// Name of the architecture.
        static constexpr char const *name() noexcept { return "generic"; }

    protected:
        static constexpr unsigned
        version(unsigned major, unsigned minor, unsigned patch, unsigned multiplier = 100u) noexcept {
            return major * multiplier * multiplier + minor * multiplier + patch;
        }
    };
}

#endif  // COLLIE_SIMD_TYPES_GENERIC_ARCH_H_
