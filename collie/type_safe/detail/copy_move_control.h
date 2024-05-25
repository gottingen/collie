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
//

#pragma once

namespace collie::ts::detail {
    template<bool AllowCopy>
    struct copy_control;

    template<>
    struct copy_control<true> {
        copy_control() noexcept = default;

        copy_control(const copy_control &) noexcept = default;

        copy_control &operator=(const copy_control &) noexcept = default;

        copy_control(copy_control &&) noexcept = default;

        copy_control &operator=(copy_control &&) noexcept = default;
    };

    template<>
    struct copy_control<false> {
        copy_control() noexcept = default;

        copy_control(const copy_control &) noexcept = delete;

        copy_control &operator=(const copy_control &) noexcept = delete;

        copy_control(copy_control &&) noexcept = default;

        copy_control &operator=(copy_control &&) noexcept = default;
    };

    template<bool AllowCopy>
    struct move_control;

    template<>
    struct move_control<true> {
        move_control() noexcept = default;

        move_control(const move_control &) noexcept = default;

        move_control &operator=(const move_control &) noexcept = default;

        move_control(move_control &&) noexcept = default;

        move_control &operator=(move_control &&) noexcept = default;
    };

    template<>
    struct move_control<false> {
        move_control() noexcept = default;

        move_control(const move_control &) noexcept = default;

        move_control &operator=(const move_control &) noexcept = default;

        move_control(move_control &&) noexcept = delete;

        move_control &operator=(move_control &&) noexcept = delete;
    };
} // namespace collie::ts::detail
