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


namespace collie::log {
namespace details {

inline log_msg_buffer::log_msg_buffer(const log_msg &orig_msg)
    : log_msg{orig_msg} {
    buffer.append(logger_name.begin(), logger_name.end());
    buffer.append(payload.begin(), payload.end());
    update_string_views();
}

inline log_msg_buffer::log_msg_buffer(const log_msg_buffer &other)
    : log_msg{other} {
    buffer.append(logger_name.begin(), logger_name.end());
    buffer.append(payload.begin(), payload.end());
    update_string_views();
}

inline log_msg_buffer::log_msg_buffer(log_msg_buffer &&other) noexcept
    : log_msg{other},
      buffer{std::move(other.buffer)} {
    update_string_views();
}

inline log_msg_buffer &log_msg_buffer::operator=(const log_msg_buffer &other) {
    log_msg::operator=(other);
    buffer.clear();
    buffer.append(other.buffer.data(), other.buffer.data() + other.buffer.size());
    update_string_views();
    return *this;
}

inline log_msg_buffer &log_msg_buffer::operator=(log_msg_buffer &&other) noexcept {
    log_msg::operator=(other);
    buffer = std::move(other.buffer);
    update_string_views();
    return *this;
}

inline void log_msg_buffer::update_string_views() {
    logger_name = string_view_t{buffer.data(), logger_name.size()};
    payload = string_view_t{buffer.data() + logger_name.size(), payload.size()};
}

}  // namespace details
}  // namespace collie::log
