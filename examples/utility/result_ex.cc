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

#include <collie/utility/result.h>

collie::StatusCode kErrorCode = collie::StatusCode::InvalidArgument;
constexpr const char* kErrorMessage = "Invalid argument";
const int kIntElement = 42;
constexpr const char* kStringElement =
        "The Answer to the Ultimate Question of Life, the Universe, and Everything";
// A data type without a default constructor.
struct Foo {
    int bar;
    std::string baz;
    Foo() = default;
    explicit Foo(int value) : bar(value), baz(kStringElement) {}

    bool operator==(const Foo& other) const {
        return (bar == other.bar) && (baz == other.baz);
    }
};

collie::Result<Foo> GetFoo() {
    return Foo{42};
}

collie::Result<int> abort_on_error() {
    return collie::Status(kErrorCode, kErrorMessage);
}

int main() {
    // Create a result with an error code and message.
    auto r = abort_on_error();
    auto a = r.value_or_die();
       collie::Result<Foo> result_error = GetFoo();
         if (!result_error.ok()) {
              std::cerr << "Error message: " << result_error.status().message() << std::endl;
         } else {
             Foo foo;
             auto r = std::move(result_error).value<Foo>(&foo);
                std::cout << "Foo.bar: " << foo.bar << std::endl;
         }
    return 0;
}