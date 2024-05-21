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


#include <collie/log/details/thread_pool.h>
#include <collie/log/sinks/sink.h>

#include <memory>
#include <string>

inline collie::log::async_logger::async_logger(std::string logger_name,
                                                 sinks_init_list sinks_list,
                                                 std::weak_ptr<details::thread_pool> tp,
                                                 async_overflow_policy overflow_policy)
    : async_logger(std::move(logger_name),
                   sinks_list.begin(),
                   sinks_list.end(),
                   std::move(tp),
                   overflow_policy) {}

inline collie::log::async_logger::async_logger(std::string logger_name,
                                                 sink_ptr single_sink,
                                                 std::weak_ptr<details::thread_pool> tp,
                                                 async_overflow_policy overflow_policy)
    : async_logger(
          std::move(logger_name), {std::move(single_sink)}, std::move(tp), overflow_policy) {}

// send the log message to the thread pool
inline void collie::log::async_logger::sink_it_(const details::log_msg &msg){
    CLOG_TRY{if (auto pool_ptr = thread_pool_.lock()){
        pool_ptr->post_log(shared_from_this(), msg, overflow_policy_);
}
else {
    throw_clog_ex("async log: thread pool doesn't exist anymore");
}
}
CLOG_LOGGER_CATCH(msg.source)
}

// send flush request to the thread pool
inline void collie::log::async_logger::flush_(){
    CLOG_TRY{if (auto pool_ptr = thread_pool_.lock()){
        pool_ptr->post_flush(shared_from_this(), overflow_policy_);
}
else {
    throw_clog_ex("async flush: thread pool doesn't exist anymore");
}
}
CLOG_LOGGER_CATCH(source_loc())
}

//
// backend functions - called from the thread pool to do the actual job
//
inline void collie::log::async_logger::backend_sink_it_(const details::log_msg &msg) {
    for (auto &sink : sinks_) {
        if (sink->should_log(msg.level)) {
            CLOG_TRY { sink->log(msg); }
            CLOG_LOGGER_CATCH(msg.source)
        }
    }

    if (should_flush_(msg)) {
        backend_flush_();
    }
}

inline void collie::log::async_logger::backend_flush_() {
    for (auto &sink : sinks_) {
        CLOG_TRY { sink->flush(); }
        CLOG_LOGGER_CATCH(source_loc())
    }
}

inline std::shared_ptr<collie::log::logger> collie::log::async_logger::clone(std::string new_name) {
    auto cloned = std::make_shared<collie::log::async_logger>(*this);
    cloned->name_ = std::move(new_name);
    return cloned;
}
