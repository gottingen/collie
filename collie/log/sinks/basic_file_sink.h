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

#include <collie/log/details/file_helper.h>
#include <collie/log/details/null_mutex.h>
#include <collie/log/details/synchronous_factory.h>
#include <collie/log/sinks/base_sink.h>

#include <mutex>
#include <string>

namespace collie::log {
namespace sinks {
/*
 * Trivial file sink with single file as target
 */
template <typename Mutex>
class basic_file_sink final : public base_sink<Mutex> {
public:
    explicit basic_file_sink(const filename_t &filename,
                             bool truncate = false,
                             const file_event_handlers &event_handlers = {});
    const filename_t &filename() const;

protected:
    void sink_it_(const details::log_msg &msg) override;
    void flush_() override;

private:
    details::file_helper file_helper_;
};

using basic_file_sink_mt = basic_file_sink<std::mutex>;
using basic_file_sink_st = basic_file_sink<details::null_mutex>;

}  // namespace sinks

//
// factory functions
//
template <typename Factory = collie::log::synchronous_factory>
inline std::shared_ptr<logger> basic_logger_mt(const std::string &logger_name,
                                               const filename_t &filename,
                                               bool truncate = false,
                                               const file_event_handlers &event_handlers = {}) {
    return Factory::template create<sinks::basic_file_sink_mt>(logger_name, filename, truncate,
                                                               event_handlers);
}

template <typename Factory = collie::log::synchronous_factory>
inline std::shared_ptr<logger> basic_logger_st(const std::string &logger_name,
                                               const filename_t &filename,
                                               bool truncate = false,
                                               const file_event_handlers &event_handlers = {}) {
    return Factory::template create<sinks::basic_file_sink_st>(logger_name, filename, truncate,
                                                               event_handlers);
}

}  // namespace collie::log

#include <collie/log/sinks/basic_file_sink-inl.h>
