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
#include <collie/log/pattern_formatter.h>

namespace collie::log {

    inline void initialize_logger(std::shared_ptr<logger> logger) {
        details::registry::instance().initialize_logger(std::move(logger));
    }

    inline std::shared_ptr<logger> get(const std::string &name) {
        return details::registry::instance().get(name);
    }

    inline void set_formatter(std::unique_ptr<collie::log::formatter> formatter) {
        details::registry::instance().set_formatter(std::move(formatter));
    }

    inline void set_pattern(std::string pattern, pattern_time_type time_type) {
        set_formatter(
                std::unique_ptr<collie::log::formatter>(new pattern_formatter(std::move(pattern), time_type)));
    }

    inline void enable_backtrace(size_t n_messages) {
        details::registry::instance().enable_backtrace(n_messages);
    }

    inline void disable_backtrace() { details::registry::instance().disable_backtrace(); }

    inline void dump_backtrace() { default_logger_raw()->dump_backtrace(); }

    inline level::level_enum get_level() { return default_logger_raw()->level(); }

    inline bool should_log(level::level_enum log_level) {
        return default_logger_raw()->should_log(log_level);
    }

    inline void set_level(level::level_enum log_level) {
        details::registry::instance().set_level(log_level);
    }

    inline void set_vlog_level(int vlevel) {
        details::registry::instance().set_vlog_level(vlevel);
    }

    inline void flush_on(level::level_enum log_level) {
        details::registry::instance().flush_on(log_level);
    }

    inline void set_error_handler(void (*handler)(const std::string &msg)) {
        details::registry::instance().set_error_handler(handler);
    }

    inline void register_logger(std::shared_ptr<logger> logger) {
        details::registry::instance().register_logger(std::move(logger));
    }

    inline void apply_all(const std::function<void(std::shared_ptr<logger>)> &fun) {
        details::registry::instance().apply_all(fun);
    }

    inline void drop(const std::string &name) { details::registry::instance().drop(name); }

    inline void drop_all() { details::registry::instance().drop_all(); }

    inline void shutdown() { details::registry::instance().shutdown(); }

    inline void set_automatic_registration(bool automatic_registration) {
        details::registry::instance().set_automatic_registration(automatic_registration);
    }

    inline std::shared_ptr<collie::log::logger> default_logger() {
        return details::registry::instance().default_logger();
    }

    inline collie::log::logger *default_logger_raw() {
        return details::registry::instance().get_default_raw();
    }

    inline void set_default_logger(std::shared_ptr<collie::log::logger> default_logger) {
        details::registry::instance().set_default_logger(std::move(default_logger));
    }

    inline void apply_logger_env_levels(std::shared_ptr<logger> logger) {
        details::registry::instance().apply_logger_env_levels(std::move(logger));
    }

}  // namespace collie::log
