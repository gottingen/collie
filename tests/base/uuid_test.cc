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
// Created by jeff on 24-3-29.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <collie/testing/doctest.h>
#include <collie/base/uuid.h>
#include <mutex>
#include <thread>
#include <algorithm>

void test_threaded_uuid(size_t N) {

    std::vector<collie::UUID> uuids(65536);

    // threaded
    std::mutex mutex;
    std::vector<std::thread> threads;

    for(size_t i=0; i<N; ++i) {
        threads.emplace_back([&](){
            for(int j=0; j<1000; ++j) {
                std::lock_guard<std::mutex> lock(mutex);
                uuids.push_back(collie::UUID());
            }
        });
    }

    for(auto& t : threads) {
        t.join();
    }

    auto size = uuids.size();
    std::sort(uuids.begin(), uuids.end());
    std::unique(uuids.begin(), uuids.end());
    REQUIRE(uuids.size() == size);
}

TEST_CASE("uuid") {

collie::UUID u1, u2, u3, u4;

// Comparator.
REQUIRE(u1 == u1);

// Copy
u2 = u1;
REQUIRE(u1 == u2);

// Move
u3 = std::move(u1);
REQUIRE(u2 == u3);

// Copy constructor
collie::UUID u5(u4);
REQUIRE(u5 == u4);

// Move constructor.
collie::UUID u6(std::move(u4));
REQUIRE(u5 == u6);

// Uniqueness
std::vector<collie::UUID> uuids(65536);
std::sort(uuids.begin(), uuids.end());
std::unique(uuids.begin(), uuids.end());
REQUIRE(uuids.size() == 65536);

}

TEST_CASE("uuid.10threads") {
test_threaded_uuid(10);
}

TEST_CASE("uuid.100threads") {
test_threaded_uuid(100);
}