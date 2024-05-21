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
