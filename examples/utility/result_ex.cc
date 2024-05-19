// Copyright 2023 The Elastic-AI Authors.
// part of Elastic AI Search
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