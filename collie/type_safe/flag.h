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

#include <collie/type_safe/boolean.h>
#include <collie/type_safe/detail/assert.h>

namespace collie::ts {
    class flag;

    /// \exclude
    template<typename T, typename = detail::enable_boolean<T>>
    constexpr bool operator==(flag lhs, T rhs) noexcept;

    /// A type safe flag, it can either be `true` or `false`.
    ///
    /// Consider the following snippet:
    /// ```cpp
    /// auto was_newl = false;
    /// for (auto x : …)
    /// {
    ///     if (x == '\n')
    ///     {
    ///          assert(!was_newl); // want to change value here
    ///          was_newl = true;
    ///     }
    ///     else if (was_newl)
    ///     {
    ///         do_sth(c);
    ///         was_newl = false; // separate step, easy to forget (I did here originally!)
    ///     }
    /// }
    /// ```
    ///
    /// With flag, it is better:
    /// ```cpp
    /// collie::ts::flag was_newl(false);
    /// for (auto x : …)
    /// {
    ///      if (x == '\n')
    ///          was_newl.change(true); // asserts that value is changed
    ///      else if (was_newl.try_reset()) // resets flag and returns whether value changed
    ///          do_sth(x); // no way to forget
    /// }
    /// ```
    /// \notes It is named `flag` for consistency with [std::atomic_flag](),
    /// even though this one can provide an extended interface as it is not atomic.
    /// `flag` has nothing to do with [ts::flag_set]().
    /// \module types
    class flag {
    public:
        flag() = delete;

        /// \effects Gives the flag the initial state.
        /// \notes This function does not participate in overload resolution if `T` is not a boolean
        /// type. \param 1 \exclude
        template<typename T, typename = detail::enable_boolean<T>>
        constexpr flag(T initial_state) noexcept : state_(static_cast<bool>(initial_state)) {}

        /// \effects Flips the state of the flag.
        /// \returns The old value.
        bool toggle() noexcept {
            auto old = state_;
            state_ = !state_;
            return old;
        }

        /// \effects Sets its state to the new one.
        /// \requires The new state must be different than the old one.
        /// \param 1
        /// \exclude
        template<typename T, typename = detail::enable_boolean<T>>
        void change(T new_state) noexcept {
            DEBUG_ASSERT(state_ != new_state, detail::precondition_error_handler{});
            state_ = new_state;
        }

        /// \effects Sets its state to `true`.
        void set() noexcept {
            state_ = true;
        }

        /// \effects Sets its state to `true`.
        /// \returns `true` if the previous state was `false`, `false` otherwise,
        /// i.e. whether or not the state was changed.
        bool try_set() noexcept {
            auto old = state_;
            state_ = true;
            return old != state_;
        }

        /// \effects Sets its state to `false`.
        void reset() noexcept {
            state_ = false;
        }

        /// \effects Sets its state to `false`.
        /// \returns `true` if the previous state was `true`, `false` otherwise,
        /// i.e. whether or not the state was changed.
        bool try_reset() noexcept {
            auto old = state_;
            state_ = false;
            return old != state_;
        }

    private:
        bool state_;

        friend constexpr bool operator==(flag lhs, flag rhs) noexcept;

        template<typename T, typename>
        friend constexpr bool operator==(flag lhs, T rhs) noexcept;
    };

    /// [ts::flag]() equality comparison.
    /// \returns `true` if (1) both [ts::flag]() objects are in the same state,
    /// (2)/(3) the flag is in the given state.
    /// \notes (2)/(3) do not participate in overload resolution unless `T` is a boolean type.
    /// \group flag_equal
    /// \module types
    constexpr bool operator==(flag lhs, flag rhs) noexcept {
        return lhs.state_ == rhs.state_;
    }

    /// \group flag_equal
    /// \param 1
    /// \exclude
    template<typename T, typename>
    constexpr bool operator==(flag lhs, T rhs) noexcept {
        return lhs.state_ == static_cast<bool>(rhs);
    }

    /// \group flag_equal
    /// \param 1
    /// \exclude
    template<typename T, typename = detail::enable_boolean<T>>
    constexpr bool operator==(T lhs, flag rhs) noexcept {
        return rhs == lhs;
    }

    /// [ts::flag]() in-equality comparison.
    /// \returns `true` if (1) both [ts::flag]() objects are in the same state,
    /// (2)/(3) the flag is in the given state.
    /// \notes (2)/(3) do not participate in overload resolution unless `T` is a boolean type.
    /// \group flag_unequal
    /// \module types
    constexpr bool operator!=(flag lhs, flag rhs) noexcept {
        return !(lhs == rhs);
    }

    /// \group flag_unequal
    /// \param 1
    /// \exclude
    template<typename T, typename = detail::enable_boolean<T>>
    constexpr bool operator!=(flag lhs, T rhs) noexcept {
        return !(lhs == rhs);
    }

    /// \group flag_unequal
    /// \param 1
    /// \exclude
    template<typename T, typename = detail::enable_boolean<T>>
    constexpr bool operator!=(T lhs, flag rhs) noexcept {
        return !(lhs == rhs);
    }
} // namespace collie::ts
