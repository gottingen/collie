// Copyright 2024 The Elastic AI Search Authors.
//
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
