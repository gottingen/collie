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


#include <collie/log/common.h>
#include <collie/log/details/os.h>

namespace collie::log {
namespace sinks {

template <typename Mutex>
inline basic_file_sink<Mutex>::basic_file_sink(const filename_t &filename,
                                                      bool truncate,
                                                      const file_event_handlers &event_handlers)
    : file_helper_{event_handlers} {
    file_helper_.open(filename, truncate);
}

template <typename Mutex>
inline const filename_t &basic_file_sink<Mutex>::filename() const {
    return file_helper_.filename();
}

template <typename Mutex>
inline void basic_file_sink<Mutex>::sink_it_(const details::log_msg &msg) {
    memory_buf_t formatted;
    base_sink<Mutex>::formatter_->format(msg, formatted);
    file_helper_.write(formatted);
}

template <typename Mutex>
inline void basic_file_sink<Mutex>::flush_() {
    file_helper_.flush();
}

}  // namespace sinks
}  // namespace collie::log
