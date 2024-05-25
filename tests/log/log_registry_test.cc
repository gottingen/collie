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
static const char *const tested_logger_name = "null_logger";
static const char *const tested_logger_name2 = "null_logger2";

#ifndef TLOG_NO_EXCEPTIONS
TEST_CASE("register_drop [registry]")
{
    collie::log::drop_all();
    collie::log::create<collie::log::sinks::null_sink_mt>(tested_logger_name);
    REQUIRE(collie::log::get(tested_logger_name) != nullptr);
    // Throw if registering existing name
    REQUIRE_THROWS_AS(collie::log::create<collie::log::sinks::null_sink_mt>(tested_logger_name), collie::log::tlog_ex);
}

TEST_CASE("explicit register [registry]")
{
    collie::log::drop_all();
    auto logger = std::make_shared<collie::log::logger>(tested_logger_name, std::make_shared<collie::log::sinks::null_sink_st>());
    collie::log::register_logger(logger);
    REQUIRE(collie::log::get(tested_logger_name) != nullptr);
    // Throw if registering existing name
    REQUIRE_THROWS_AS(collie::log::create<collie::log::sinks::null_sink_mt>(tested_logger_name), collie::log::tlog_ex);
}
#endif

TEST_CASE("apply_all [registry]")
{
    collie::log::drop_all();
    auto logger = std::make_shared<collie::log::logger>(tested_logger_name, std::make_shared<collie::log::sinks::null_sink_st>());
    collie::log::register_logger(logger);
    auto logger2 = std::make_shared<collie::log::logger>(tested_logger_name2, std::make_shared<collie::log::sinks::null_sink_st>());
    collie::log::register_logger(logger2);

    int counter = 0;
    collie::log::apply_all([&counter](std::shared_ptr<collie::log::logger>) { counter++; });
    REQUIRE(counter == 2);

    counter = 0;
    collie::log::drop(tested_logger_name2);
    collie::log::apply_all([&counter](std::shared_ptr<collie::log::logger> l) {
        REQUIRE(l->name() == tested_logger_name);
        counter++;
    });
    REQUIRE(counter == 1);
}

TEST_CASE("drop [registry]")
{
    collie::log::drop_all();
    collie::log::create<collie::log::sinks::null_sink_mt>(tested_logger_name);
    collie::log::drop(tested_logger_name);
    REQUIRE_FALSE(collie::log::get(tested_logger_name));
}

TEST_CASE("drop-default [registry]")
{
    collie::log::set_default_logger(collie::log::null_logger_st(tested_logger_name));
    collie::log::drop(tested_logger_name);
    REQUIRE_FALSE(collie::log::default_logger());
    REQUIRE_FALSE(collie::log::get(tested_logger_name));
}

TEST_CASE("drop_all [registry]")
{
    collie::log::drop_all();
    collie::log::create<collie::log::sinks::null_sink_mt>(tested_logger_name);
    collie::log::create<collie::log::sinks::null_sink_mt>(tested_logger_name2);
    collie::log::drop_all();
    REQUIRE_FALSE(collie::log::get(tested_logger_name));
    REQUIRE_FALSE(collie::log::get(tested_logger_name2));
    REQUIRE_FALSE(collie::log::default_logger());
}

TEST_CASE("drop non existing [registry]")
{
    collie::log::drop_all();
    collie::log::create<collie::log::sinks::null_sink_mt>(tested_logger_name);
    collie::log::drop("some_name");
    REQUIRE_FALSE(collie::log::get("some_name"));
    REQUIRE(collie::log::get(tested_logger_name));
    collie::log::drop_all();
}

TEST_CASE("default logger [registry]")
{
    collie::log::drop_all();
    collie::log::set_default_logger(collie::log::null_logger_st(tested_logger_name));
    REQUIRE(collie::log::get(tested_logger_name) == collie::log::default_logger());
    collie::log::drop_all();
}

TEST_CASE("set_default_logger(nullptr) [registry]")
{
    collie::log::set_default_logger(nullptr);
    REQUIRE_FALSE(collie::log::default_logger());
}

TEST_CASE("disable automatic registration [registry]")
{
    // set some global parameters
    collie::log::level::level_enum log_level = collie::log::level::level_enum::warn;
    collie::log::set_level(log_level);
    // but disable automatic registration
    collie::log::set_automatic_registration(false);
    auto logger1 = collie::log::create<collie::log::sinks::daily_file_sink_st>(tested_logger_name, TLOG_FILENAME_T("filename"), 11, 59);
    auto logger2 = collie::log::create_async<collie::log::sinks::stdout_color_sink_mt>(tested_logger_name2);
    // loggers should not be part of the registry
    REQUIRE_FALSE(collie::log::get(tested_logger_name));
    REQUIRE_FALSE(collie::log::get(tested_logger_name2));
    // but make sure they are still initialized according to global defaults
    REQUIRE(logger1->level() == log_level);
    REQUIRE(logger2->level() == log_level);
    collie::log::set_level(collie::log::level::info);
    collie::log::set_automatic_registration(true);
}
