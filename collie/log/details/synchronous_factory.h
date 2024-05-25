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

#include <collie/log/details/registry.h>

namespace collie::log {

    // Default logger factory-  creates synchronous loggers
    class logger;

    struct synchronous_factory {
        template<typename Sink, typename... SinkArgs>
        static std::shared_ptr<collie::log::logger> create(std::string logger_name, SinkArgs &&...args) {
            auto sink = std::make_shared<Sink>(std::forward<SinkArgs>(args)...);
            auto new_logger = std::make_shared<collie::log::logger>(std::move(logger_name), std::move(sink));
            details::registry::instance().initialize_logger(new_logger);
            return new_logger;
        }
    };
}  // namespace collie::log
