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

#include <collie/log/details/log_msg.h>

namespace collie::log {
    namespace details {

// Extend log_msg with internal buffer to store its payload.
// This is needed since log_msg holds string_views that points to stack data.

        class log_msg_buffer : public log_msg {
            memory_buf_t buffer;

            void update_string_views();

        public:
            log_msg_buffer() = default;

            explicit log_msg_buffer(const log_msg &orig_msg);

            log_msg_buffer(const log_msg_buffer &other);

            log_msg_buffer(log_msg_buffer &&other) noexcept;

            log_msg_buffer &operator=(const log_msg_buffer &other);

            log_msg_buffer &operator=(log_msg_buffer &&other) noexcept;
        };

    }  // namespace details
}  // namespace collie::log

#include <collie/log/details/log_msg_buffer-inl.h>
