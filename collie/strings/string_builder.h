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

#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <sstream>

namespace collie {

    namespace detail {
        class StringStreamWrapper {
        public:
            StringStreamWrapper() : sstream_(std::make_unique<std::ostringstream>()), ostream_(*sstream_) {}

            ~StringStreamWrapper() = default;

            std::ostream &stream() { return ostream_; }

            std::string str() { return sstream_->str(); }

        protected:
            std::unique_ptr<std::ostringstream> sstream_;
            std::ostream &ostream_;
        };

    }  // namespace detail

    template<typename Head>
    void StringBuilderRecursive(std::ostream &stream, Head &&head) {
        stream << head;
    }

    template<typename Head, typename... Tail>
    void StringBuilderRecursive(std::ostream &stream, Head &&head, Tail &&... tail) {
        StringBuilderRecursive(stream, std::forward<Head>(head));
        StringBuilderRecursive(stream, std::forward<Tail>(tail)...);
    }

    template<typename... Args>
    std::string StringBuilder(Args &&... args) {
        detail::StringStreamWrapper ss;
        StringBuilderRecursive(ss.stream(), std::forward<Args>(args)...);
        return ss.str();
    }

    /// CRTP helper for declaring string representation. Defines operator<<
    template<typename T>
    class ToStringOstreamable {
    public:
        ~ToStringOstreamable() {
            static_assert(
                    std::is_same<decltype(std::declval<const T>().to_string()), std::string>::value,
                    "ToStringOstreamable depends on the method T::to_string() const");
        }

    private:
        const T &cast() const { return static_cast<const T &>(*this); }

        friend inline std::ostream &operator<<(std::ostream &os, const ToStringOstreamable &t) {
            return os << t.cast().to_string();
        }
    };
}  // namespace collie
