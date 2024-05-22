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

#include <collie/strings/format.h>
#include <atomic>
#include <memory>

int main() {

    collie::println("hello");
    collie::println("{} {}","hello", "Jeff");
    collie::println(collie::format_range("[{}]", {1,2,3}, ", "));
    std::string h = "hello";
    collie::format_append(&h, " {}", "Jeff");
    collie::println(h);

    std::string h1 = "hello";
    collie::format_append(&h1, " Jeff");
    collie::println(h1);

    std::vector<int> abc = {1,4,7};
    collie::println("v: {}", abc);

    std::atomic<int> a{1};
    collie::println("a: {}",a);

    std::unique_ptr<int> iptr = std::make_unique<int>(10);
    collie::println("ptr: {}", collie::ptr(iptr.get()));

    collie::println(collie::Color::green, "green");
    collie::println(collie::Color::yellow, "yellow {}", 42);
    collie::println(collie::Color::yellow, 42);
    collie::println(42);

    collie::to_str(42);
    return 0;
}