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
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <collie/testing/doctest.h>
#include <sstream>

#include <collie/utility/status.h>

namespace collie {

    namespace {

        class TestStatusDetail : public StatusDetail {
        public:
            const char *type_id() const override { return "type_id"; }

            std::string to_string() const override { return "a specific detail message"; }
        };

    }  // namespace

    TEST_CASE("StatusTest, TestCodeAndMessage") {
        Status ok = Status::ok_status();
        REQUIRE_EQ(StatusCode::OK, ok.code());
        Status file_error = Status::io_error("file error");
        REQUIRE_EQ(StatusCode::IOError, file_error.code());
        REQUIRE_EQ("file error", file_error.message());
    }

    TEST_CASE("StatusTest, TestToString") {
        Status file_error = Status::io_error("file error");
        REQUIRE_EQ("IO_ERROR: file error", file_error.to_string());

        std::stringstream ss;
        ss << file_error;
        REQUIRE_EQ(file_error.to_string(), ss.str());
    }

    TEST_CASE("StatusTest, TestToStringWithDetail") {
        Status status(StatusCode::IOError, "summary", std::make_shared<TestStatusDetail>());
        REQUIRE_EQ("IO_ERROR: summary. Detail: a specific detail message", status.to_string());

        std::stringstream ss;
        ss << status;
        REQUIRE_EQ(status.to_string(), ss.str());
    }

    TEST_CASE("StatusTest, TestWithDetail") {
        Status status(StatusCode::IOError, "summary");
        auto detail = std::make_shared<TestStatusDetail>();
        Status new_status = status.with_detail(detail);

        REQUIRE_EQ(new_status.code(), status.code());
        REQUIRE_EQ(new_status.message(), status.message());
        REQUIRE_EQ(new_status.detail(), detail);
    }

    TEST_CASE("StatusTest, TestCoverageWarnNotOK") {
        COLLIE_WARN_NOT_OK(Status::invalid_argument("invalid"), "Expected warning");
    }

    TEST_CASE("StatusTest, AndStatus") {
        Status a = Status::ok_status();
        Status b = Status::ok_status();
        Status c = Status::invalid_argument("invalid value");
        Status d = Status::io_error("file error");

        Status res;
        res = a & b;
        REQUIRE(res.ok());
        res = a & c;
        REQUIRE(res.is_invalid_argument());
        res = d & c;
        REQUIRE(res.is_io_error());

        res = Status::ok_status();
        res &= c;
        REQUIRE(res.is_invalid_argument());
        res &= d;
        REQUIRE(res.is_invalid_argument());

    // With rvalues
        res = Status::ok_status() & Status::invalid_argument("foo");
        REQUIRE(res.is_invalid_argument());
        res = Status::invalid_argument("foo") & Status::ok_status();
        REQUIRE(res.is_invalid_argument());
        res = Status::invalid_argument("foo") & Status::io_error("bar");
        REQUIRE(res.is_invalid_argument());

        res = Status::ok_status();
        res &= Status::ok_status();
        REQUIRE(res.ok());
        res &= Status::invalid_argument("foo");
        REQUIRE(res.is_invalid_argument());
        res &= Status::io_error("bar");
        REQUIRE(res.is_invalid_argument());
    }

    TEST_CASE("StatusTest, TestEquality") {
        REQUIRE_EQ(Status(), Status::ok_status());
        REQUIRE_EQ(Status::invalid_argument("error"), Status::invalid_argument("error"));

        REQUIRE_NE(Status::invalid_argument("error"), Status::ok_status());
        REQUIRE_NE(Status::invalid_argument("error"), Status::invalid_argument("other error"));
    }

    TEST_CASE("StatusTest, TestDetailEquality") {
        const auto status_with_detail =
                Status(StatusCode::IOError, "", std::make_shared<TestStatusDetail>());
        const auto status_with_detail2 =
                Status(StatusCode::IOError, "", std::make_shared<TestStatusDetail>());
        const auto status_without_detail = Status::io_error("");

        REQUIRE_EQ(*status_with_detail.detail(), *status_with_detail2.detail());
        REQUIRE_EQ(status_with_detail, status_with_detail2);
        REQUIRE_NE(status_with_detail, status_without_detail);
        REQUIRE_NE(status_without_detail, status_with_detail);
    }

}  // namespace collie
