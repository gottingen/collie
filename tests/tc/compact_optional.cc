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
#include <collie/type_safe/compact_optional.h>

#include <collie/testing/doctest.h>

using namespace collie::ts;

namespace collie::ts
{
template class basic_optional<compact_optional_storage<compact_bool_policy<bool>>>;
template class basic_optional<compact_optional_storage<compact_integer_policy<int, -1>>>;
template class basic_optional<compact_optional_storage<compact_floating_point_policy<float>>>;
} // namespace collie::ts

TEST_CASE("compact_bool")
{
    using storage = compact_optional_storage<compact_bool_policy<bool>>;

    storage s;
    REQUIRE(!s.has_value());

    s.create_value(true);
    REQUIRE(s.has_value());
    REQUIRE(s.get_value() == true);

    s.destroy_value();
    REQUIRE(!s.has_value());

    s.create_value(false);
    REQUIRE(s.has_value());
    REQUIRE(s.get_value() == false);
}

TEST_CASE("compact_integer")
{
    using storage = compact_optional_storage<compact_integer_policy<int, -1>>;

    storage s;
    REQUIRE(!s.has_value());

    s.create_value(0);
    REQUIRE(s.has_value());
    REQUIRE(s.get_value() == 0);

    s.destroy_value();
    REQUIRE(!s.has_value());

    s.create_value(1);
    REQUIRE(s.has_value());
    REQUIRE(s.get_value() == 1);
}

TEST_CASE("compact_floating_point")
{
    using storage = compact_optional_storage<compact_floating_point_policy<float>>;

    storage s;
    REQUIRE(!s.has_value());

    s.create_value(0.1);
    REQUIRE(s.has_value());
    REQUIRE(s.get_value() == 0.1f);

    s.destroy_value();
    REQUIRE(!s.has_value());

    s.create_value(1.0);
    REQUIRE(s.has_value());
    REQUIRE(s.get_value() == 1.0);
}

enum class test_compact_enum
{
    a,
    b,
};

template class collie::ts::basic_optional<
    compact_optional_storage<compact_enum_policy<test_compact_enum, 2>>>;

TEST_CASE("compact_enum")
{
    using storage = compact_optional_storage<compact_enum_policy<test_compact_enum, 2>>;

    storage s;
    REQUIRE(!s.has_value());

    s.create_value(test_compact_enum::a);
    REQUIRE(s.has_value());
    REQUIRE(s.get_value() == test_compact_enum::a);

    s.destroy_value();
    REQUIRE(!s.has_value());

    s.create_value(test_compact_enum::b);
    REQUIRE(s.has_value());
    REQUIRE(s.get_value() == test_compact_enum::b);
}

struct test_compact_container
{
    bool empty_ = true;

    test_compact_container() {}

    test_compact_container(int) : empty_(false) {}

    bool empty() const
    {
        return empty_;
    }
};
template class collie::ts::basic_optional<
    compact_optional_storage<compact_container_policy<test_compact_container>>>;

TEST_CASE("compact_container")
{
    using storage = compact_optional_storage<compact_container_policy<test_compact_container>>;

    storage s;
    REQUIRE(!s.has_value());

    s.create_value(0);
    REQUIRE(s.has_value());
    REQUIRE(!s.get_value().empty());

    s.destroy_value();
    REQUIRE(!s.has_value());
}
