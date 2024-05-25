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

#include <collie/log/details/null_mutex.h>
#include <collie/log/sinks/base_sink.h>

#include <mutex>
#include <ostream>

namespace collie::log {
namespace sinks {
template <typename Mutex>
class ostream_sink final : public base_sink<Mutex> {
public:
    explicit ostream_sink(std::ostream &os, bool force_flush = false)
        : ostream_(os),
          force_flush_(force_flush) {}
    ostream_sink(const ostream_sink &) = delete;
    ostream_sink &operator=(const ostream_sink &) = delete;

protected:
    void sink_it_(const details::log_msg &msg) override {
        memory_buf_t formatted;
        base_sink<Mutex>::formatter_->format(msg, formatted);
        ostream_.write(formatted.data(), static_cast<std::streamsize>(formatted.size()));
        if (force_flush_) {
            ostream_.flush();
        }
    }

    void flush_() override { ostream_.flush(); }

    std::ostream &ostream_;
    bool force_flush_;
};

using ostream_sink_mt = ostream_sink<std::mutex>;
using ostream_sink_st = ostream_sink<details::null_mutex>;

}  // namespace sinks
}  // namespace collie::log
