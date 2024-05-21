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

#pragma once

#include <functional>
#include <collie/taskflow/core/async.h>

namespace collie::tf {

    // Function: launch_loop
    template<typename P, typename Loop>
    TF_FORCE_INLINE void launch_loop(P part, Loop loop) {

        constexpr bool is_default_wrapper_v = std::is_same_v<
                typename std::decay_t<P>::closure_wrapper_type, DefaultClosureWrapper
        >;

        if constexpr (is_default_wrapper_v) {
            (void)part;
            loop();
        } else {
            std::invoke(part.closure_wrapper(), loop);
        }
    }

    // Function: launch_loop
    template<typename P, typename Loop>
    TF_FORCE_INLINE void launch_loop(
            size_t N,
            size_t W,
            Runtime &rt,
            std::atomic<size_t> &next,
            P part,
            Loop loop
    ) {

        //static_assert(std::is_lvalue_reference_v<Loop>, "");

        using namespace std::string_literals;

        for (size_t w = 0; w < W; w++) {
            auto r = N - next.load(std::memory_order_relaxed);
            // no more loop work to do - finished by previous async tasks
            if (!r) {
                break;
            }
            // tail optimization
            if (r <= part.chunk_size() || w == W - 1) {
                launch_loop(part, loop);
                break;
            } else {
                rt.silent_async_unchecked([=]() { launch_loop(part, loop); });
            }
        }

        rt.corun_all();
    }

    // Function: launch_loop
    template<typename P, typename Loop>
    TF_FORCE_INLINE void launch_loop(
            size_t W,
            size_t w,
            Runtime &rt,
            P part,
            Loop loop
    ) {
        using namespace std::string_literals;
        if (w == W - 1) {
            launch_loop(part, loop);
        } else {
            rt.silent_async_unchecked([=]() { launch_loop(part, loop); });
        }
    }

}  // end of namespace collie::tf -----------------------------------------------------
