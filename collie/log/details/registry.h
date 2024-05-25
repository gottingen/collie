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

// Loggers registry of unique name->logger pointer
// An attempt to create a logger with an already existing name will result with CLogEx exception.
// If user requests a non existing logger, nullptr will be returned
// This class is thread safe

#include <collie/log/common.h>
#include <collie/log/details/periodic_worker.h>

#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

namespace collie::log {
    class logger;
}  // namespace collie::log
namespace collie::log::details {
    class thread_pool;

    class registry {
    public:
        using log_levels = std::unordered_map<std::string, level::level_enum>;

        registry(const registry &) = delete;

        registry &operator=(const registry &) = delete;

        void register_logger(std::shared_ptr<logger> new_logger);

        void initialize_logger(std::shared_ptr<logger> new_logger);

        std::shared_ptr<logger> get(const std::string &logger_name);

        std::shared_ptr<logger> default_logger();

        // Return raw ptr to the default logger.
        // To be used directly by the clog default api (e.g. collie::log::info)
        // This make the default API faster, but cannot be used concurrently with set_default_logger().
        // e.g do not call set_default_logger() from one thread while calling collie::log::info() from
        // another.
        logger *get_default_raw();

        // set default logger.
        // default logger is stored in default_logger_ (for faster retrieval) and in the loggers_ map.
        void set_default_logger(std::shared_ptr<logger> new_default_logger);

        void set_tp(std::shared_ptr<thread_pool> tp);

        std::shared_ptr<thread_pool> get_tp();

        // Set global formatter. Each sink in each logger will get a clone of this object
        void set_formatter(std::unique_ptr<formatter> formatter);

        void enable_backtrace(size_t n_messages);

        void disable_backtrace();

        void set_level(level::level_enum log_level);

        void set_vlog_level(int vlevel);

        void flush_on(level::level_enum log_level);

        template<typename Rep, typename Period>
        void flush_every(std::chrono::duration<Rep, Period> interval) {
            std::lock_guard<std::mutex> lock(flusher_mutex_);
            auto clbk = [this]() { this->flush_all(); };
            periodic_flusher_ = details::make_unique<periodic_worker>(clbk, interval);
        }

        void set_error_handler(err_handler handler);

        void apply_all(const std::function<void(const std::shared_ptr<logger>)> &fun);

        void flush_all();

        void drop(const std::string &logger_name);

        void drop_all();

        // clean all resources and threads started by the registry
        void shutdown();

        std::recursive_mutex &tp_mutex();

        void set_automatic_registration(bool automatic_registration);

        // set levels for all existing/future loggers. global_level can be null if should not set.
        void set_levels(log_levels levels, level::level_enum *global_level);

        static registry &instance();

        void apply_logger_env_levels(std::shared_ptr<logger> new_logger);

    private:
        registry();

        ~registry();

        void throw_if_exists_(const std::string &logger_name);

        void register_logger_(std::shared_ptr<logger> new_logger);

        bool set_level_from_cfg_(logger *logger);

        std::mutex logger_map_mutex_, flusher_mutex_;
        std::recursive_mutex tp_mutex_;
        std::unordered_map<std::string, std::shared_ptr<logger>> loggers_;
        log_levels log_levels_;
        std::unique_ptr<formatter> formatter_;
        collie::log::level::level_enum global_log_level_ = level::info;
        collie::log::level_t global_vlog_leve_{0};
        level::level_enum flush_level_ = level::off;
        err_handler err_handler_;
        std::shared_ptr<thread_pool> tp_;
        std::unique_ptr<periodic_worker> periodic_flusher_;
        std::shared_ptr<logger> default_logger_;
        bool automatic_registration_ = true;
        size_t backtrace_n_messages_ = 0;
    };

}  // namespace collie::log::details

#include <collie/log/details/registry-inl.h>
