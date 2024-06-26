// Copyright 2023 The Turbo Authors.
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

#include "includes.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "collie/testing/test.h"

using std::chrono::milliseconds;
using test_clock = std::chrono::high_resolution_clock;

static milliseconds millis_from(const test_clock::time_point &tp0)
{
    return std::chrono::duration_cast<milliseconds>(test_clock::now() - tp0);
}
TEST_CASE("dequeue-empty-nowait [mpmc_blocking_q]")
{
    size_t q_size = 100;
    milliseconds tolerance_wait(20);
    collie::log::details::mpmc_blocking_queue<int> q(q_size);
    int popped_item = 0;

    auto start = test_clock::now();
    auto rv = q.dequeue_for(popped_item, milliseconds::zero());
    auto delta_ms = millis_from(start);

    REQUIRE(rv == false);
    INFO("Delta " << delta_ms.count() << " millis");
    REQUIRE(delta_ms <= tolerance_wait);
}

TEST_CASE("dequeue-empty-wait [mpmc_blocking_q]")
{

    size_t q_size = 100;
    milliseconds wait_ms(250);
    milliseconds tolerance_wait(250);

    collie::log::details::mpmc_blocking_queue<int> q(q_size);
    int popped_item = 0;
    auto start = test_clock::now();
    auto rv = q.dequeue_for(popped_item, wait_ms);
    auto delta_ms = millis_from(start);

    REQUIRE(rv == false);

    INFO("Delta " << delta_ms.count() << " millis");
    REQUIRE(delta_ms >= wait_ms - tolerance_wait);
    REQUIRE(delta_ms <= wait_ms + tolerance_wait);
}

TEST_CASE("enqueue_nowait [mpmc_blocking_q]")
{

    size_t q_size = 1;
    collie::log::details::mpmc_blocking_queue<int> q(q_size);
    milliseconds tolerance_wait(10);

    q.enqueue(1);
    REQUIRE(q.overrun_counter() == 0);

    auto start = test_clock::now();
    q.enqueue_nowait(2);
    auto delta_ms = millis_from(start);

    INFO("Delta " << delta_ms.count() << " millis");
    REQUIRE(delta_ms <= tolerance_wait);
    REQUIRE(q.overrun_counter() == 1);
}

TEST_CASE("bad_queue [mpmc_blocking_q]")
{
    size_t q_size = 0;
    collie::log::details::mpmc_blocking_queue<int> q(q_size);
    q.enqueue_nowait(1);
    REQUIRE(q.overrun_counter() == 1);
    int i = 0;
    REQUIRE(q.dequeue_for(i, milliseconds(0)) == false);
}

TEST_CASE("empty_queue [mpmc_blocking_q]")
{
    size_t q_size = 10;
    collie::log::details::mpmc_blocking_queue<int> q(q_size);
    int i = 0;
    REQUIRE(q.dequeue_for(i, milliseconds(10)) == false);
}

TEST_CASE("full_queue [mpmc_blocking_q]")
{
    size_t q_size = 100;
    collie::log::details::mpmc_blocking_queue<int> q(q_size);
    for (int i = 0; i < static_cast<int>(q_size); i++)
    {
        q.enqueue(i + 0); // i+0 to force rvalue and avoid tidy warnings on the same time if we std::move(i) instead
    }

    q.enqueue_nowait(123456);
    REQUIRE(q.overrun_counter() == 1);

    for (int i = 1; i < static_cast<int>(q_size); i++)
    {
        int item = -1;
        q.dequeue_for(item, milliseconds(0));
        REQUIRE(item == i);
    }

    // last item pushed has overridden the oldest.
    int item = -1;
    q.dequeue_for(item, milliseconds(0));
    REQUIRE(item == 123456);
}
