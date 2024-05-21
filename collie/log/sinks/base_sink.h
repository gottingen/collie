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
//
// base sink templated over a mutex (either dummy or real)
// concrete implementation should override the sink_it_() and flush_()  methods.
// locking is taken care of in this class - no locking needed by the
// implementers..
//

#include <collie/log/common.h>
#include <collie/log/details/log_msg.h>
#include <collie/log/sinks/sink.h>

namespace collie::log {
    namespace sinks {
        template<typename Mutex>
        class base_sink : public sink {
        public:
            base_sink();

            explicit base_sink(std::unique_ptr<collie::log::formatter> formatter);

            ~base_sink() override = default;

            base_sink(const base_sink &) = delete;

            base_sink(base_sink &&) = delete;

            base_sink &operator=(const base_sink &) = delete;

            base_sink &operator=(base_sink &&) = delete;

            void log(const details::log_msg &msg) final;

            void flush() final;

            void set_pattern(const std::string &pattern) final;

            void set_formatter(std::unique_ptr<collie::log::formatter> sink_formatter) final;

        protected:
            // sink formatter
            std::unique_ptr<collie::log::formatter> formatter_;
            Mutex mutex_;

            virtual void sink_it_(const details::log_msg &msg) = 0;

            virtual void flush_() = 0;

            virtual void set_pattern_(const std::string &pattern);

            virtual void set_formatter_(std::unique_ptr<collie::log::formatter> sink_formatter);
        };
    }  // namespace sinks
}  // namespace collie::log

#include <collie/log/sinks/base_sink-inl.h>
