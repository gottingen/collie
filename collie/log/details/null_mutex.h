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

#include <atomic>
#include <utility>
// null, no cost dummy "mutex" and dummy "atomic" int

namespace collie::log {
namespace details {
struct null_mutex {
    void lock() const {}
    void unlock() const {}
};

struct null_atomic_int {
    int value;
    null_atomic_int() = default;

    explicit null_atomic_int(int new_value)
        : value(new_value) {}

    int load(std::memory_order = std::memory_order_relaxed) const { return value; }

    void store(int new_value, std::memory_order = std::memory_order_relaxed) { value = new_value; }

    int exchange(int new_value, std::memory_order = std::memory_order_relaxed) {
        std::swap(new_value, value);
        return new_value;  // return value before the call
    }
};

}  // namespace details
}  // namespace collie::log
