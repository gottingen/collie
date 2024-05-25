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

#ifdef __ANDROID__

    #include <collie/log/details/fmt_helper.h>
    #include <collie/log/details/null_mutex.h>
    #include <collie/log/details/os.h>
    #include <collie/log/details/synchronous_factory.h>
    #include <collie/log/sinks/base_sink.h>

    #include <android/log.h>
    #include <chrono>
    #include <mutex>
    #include <string>
    #include <thread>
    #include <type_traits>

    #if !defined(CLOG_ANDROID_RETRIES)
        #define CLOG_ANDROID_RETRIES 2
    #endif

namespace collie::log {
namespace sinks {

/*
 * Android sink
 * (logging using __android_log_write or __android_log_buf_write depending on the specified
 * BufferID)
 */
template <typename Mutex, int BufferID = log_id::LOG_ID_MAIN>
class android_sink final : public base_sink<Mutex> {
public:
    explicit android_sink(std::string tag = "clog", bool use_raw_msg = false)
        : tag_(std::move(tag)),
          use_raw_msg_(use_raw_msg) {}

protected:
    void sink_it_(const details::log_msg &msg) override {
        const android_LogPriority priority = convert_to_android_(msg.level);
        memory_buf_t formatted;
        if (use_raw_msg_) {
            details::fmt_helper::append_string_view(msg.payload, formatted);
        } else {
            base_sink<Mutex>::formatter_->format(msg, formatted);
        }
        formatted.push_back('\0');
        const char *msg_output = formatted.data();

        // See system/core/liblog/logger_write.c for explanation of return value
        int ret = android_log(priority, tag_.c_str(), msg_output);
        if (ret == -EPERM) {
            return;  // !__android_log_is_loggable
        }
        int retry_count = 0;
        while ((ret == -11 /*EAGAIN*/) && (retry_count < CLOG_ANDROID_RETRIES)) {
            details::os::sleep_for_millis(5);
            ret = android_log(priority, tag_.c_str(), msg_output);
            retry_count++;
        }

        if (ret < 0) {
            throw_clog_ex("logging to Android failed", ret);
        }
    }

    void flush_() override {}

private:
    // There might be liblog versions used, that do not support __android_log_buf_write. So we only
    // compile and link against
    // __android_log_buf_write, if user explicitly provides a non-default log buffer. Otherwise,
    // when using the default log buffer, always log via __android_log_write.
    template <int ID = BufferID>
    typename std::enable_if<ID == static_cast<int>(log_id::LOG_ID_MAIN), int>::type android_log(
        int prio, const char *tag, const char *text) {
        return __android_log_write(prio, tag, text);
    }

    template <int ID = BufferID>
    typename std::enable_if<ID != static_cast<int>(log_id::LOG_ID_MAIN), int>::type android_log(
        int prio, const char *tag, const char *text) {
        return __android_log_buf_write(ID, prio, tag, text);
    }

    static android_LogPriority convert_to_android_(collie::log::level::level_enum level) {
        switch (level) {
            case collie::log::level::trace:
                return ANDROID_LOG_VERBOSE;
            case collie::log::level::debug:
                return ANDROID_LOG_DEBUG;
            case collie::log::level::info:
                return ANDROID_LOG_INFO;
            case collie::log::level::warn:
                return ANDROID_LOG_WARN;
            case collie::log::level::error:
                return ANDROID_LOG_ERROR;
            case collie::log::level::fatal:
                return ANDROID_LOG_FATAL;
            default:
                return ANDROID_LOG_DEFAULT;
        }
    }

    std::string tag_;
    bool use_raw_msg_;
};

using android_sink_mt = android_sink<std::mutex>;
using android_sink_st = android_sink<details::null_mutex>;

template <int BufferId = log_id::LOG_ID_MAIN>
using android_sink_buf_mt = android_sink<std::mutex, BufferId>;
template <int BufferId = log_id::LOG_ID_MAIN>
using android_sink_buf_st = android_sink<details::null_mutex, BufferId>;

}  // namespace sinks

// Create and register android syslog logger

template <typename Factory = collie::log::synchronous_factory>
inline std::shared_ptr<logger> android_logger_mt(const std::string &logger_name,
                                                 const std::string &tag = "clog") {
    return Factory::template create<sinks::android_sink_mt>(logger_name, tag);
}

template <typename Factory = collie::log::synchronous_factory>
inline std::shared_ptr<logger> android_logger_st(const std::string &logger_name,
                                                 const std::string &tag = "clog") {
    return Factory::template create<sinks::android_sink_st>(logger_name, tag);
}

}  // namespace collie::log

#endif  // __ANDROID__
