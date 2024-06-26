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

#include <cctype>
#include <iostream>
#include <string>

#include <collie/type_safe/optional.h>
#include <collie/type_safe/optional_ref.h>
#include <collie/type_safe/visitor.h>

namespace ts = collie::ts;

// type safe back function
// impossible to forget precondition
ts::optional<char> back(const std::string& str)
{
    return str.empty() ? ts::nullopt : ts::make_optional(str.back());
}

// some imaginary lookup function
ts::optional<int> lookup(int c)
{
    // simulate lookup
    return c == 'T' ? ts::nullopt : ts::make_optional(c + 1);
}

// task: get last character of string,
// convert it to upper case
// look it up
// and return the integer or 0 if there is no integer

// this is how'd you do it with std::optional
int task_std(const std::string& str)
{
    auto c = back(str);
    if (!c)
        return 0;
    auto upper_case = std::toupper(c.value());
    auto result     = lookup(upper_case);
    return result.value_or(0);
}

// this is how'd you do it with the monadic functionality
// there is no need for branches and no errors possible
// it generates identical assembler code
int task_monadic(const std::string& str)
{
    return back(str)
        // map takes a functor and applies it to the stored value, if there is any
        // the result is another optional with possibly different type
        .map(static_cast<int (*)(int)>(&std::toupper))
        // now we map lookup
        // as lookup returns already an optional itself,
        // it won't wrap the result in another optional
        .map(lookup)
        // value_or() as usual
        .value_or(0);
}

// a visitor for an optional
// this again makes branches unnecessary
struct visitor
{
    template <typename T>
    void operator()(const T& val)
    {
        std::cout << val << '\n';
    }

    void operator()(ts::nullopt_t)
    {
        std::cout << "nothing :(\n";
    }
};

int main()
{
    std::cout << task_std("Hello World") << ' ' << task_monadic("Hello World") << '\n';
    std::cout << task_std("Hallo Welt") << ' ' << task_monadic("Hallo Welt") << '\n';
    std::cout << task_std("") << ' ' << task_monadic("") << '\n';

    // visit an optional
    ts::optional<int> opt(45);
    ts::visit(visitor{}, opt);
    opt.reset();
    ts::visit(visitor{}, opt);

    // safely manipulate the value if there is one
    // with() is an inplace map()
    // and thus more efficient if you do not need to change the type
    ts::with(opt, [](int& i) {
        std::cout << "got: " << i << '\n';
        ++i;
    });

    // an optional reference
    // basically a pointer, but provides the functionality of optional
    ts::optional_ref<int> ref;

    int a = 42;
    ref   = ts::ref(a); // assignment rebinds
    std::cout << ref.value() << '\n';

    ref.value() = 0;
    std::cout << a << '\n';

    int b = 5;
    ref.value_or(b)++;
    std::cout << a << ' ' << b << '\n';

    ref = nullptr; // assign nullptr as additional way to reset

    // an optional reference to const
    ts::optional_ref<const int> ref_const(ref);

    // create optional_ref from pointer
    auto ptr       = ts::opt_ref(&a);
    auto ptr_const = ts::opt_cref(&a);

    /// transform an optional_ref to an optional by copying
    auto ptr_transformed = ts::copy(ptr); // there is also ts::move() to move the value
    std::cout << ptr_transformed.value() << '\n';
}
