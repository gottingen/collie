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

//
// Custom sink for kafka
// Building and using requires librdkafka library.
// For building librdkafka library check the url below
// https://github.com/confluentinc/librdkafka
//

#include <collie/log/async.h>
#include <collie/log/details/log_msg.h>
#include <collie/log/details/null_mutex.h>
#include <collie/log/details/synchronous_factory.h>
#include <collie/log/sinks/base_sink.h>
#include <mutex>
#include <collie/log/common.h>

// kafka header
#include <librdkafka/rdkafkacpp.h>

namespace collie::log {
namespace sinks {

struct kafka_sink_config {
    std::string server_addr;
    std::string produce_topic;
    int32_t flush_timeout_ms = 1000;

    kafka_sink_config(std::string addr, std::string topic, int flush_timeout_ms = 1000)
        : server_addr{std::move(addr)},
          produce_topic{std::move(topic)},
          flush_timeout_ms(flush_timeout_ms) {}
};

template <typename Mutex>
class kafka_sink : public base_sink<Mutex> {
public:
    kafka_sink(kafka_sink_config config)
        : config_{std::move(config)} {
        try {
            std::string errstr;
            conf_.reset(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
            RdKafka::Conf::ConfResult confRes =
                conf_->set("bootstrap.servers", config_.server_addr, errstr);
            if (confRes != RdKafka::Conf::CONF_OK) {
                throw_clog_ex(
                    fmt_lib::format("conf set bootstrap.servers failed err:{}", errstr));
            }

            tconf_.reset(RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC));
            if (tconf_ == nullptr) {
                throw_clog_ex(fmt_lib::format("create topic config failed"));
            }

            producer_.reset(RdKafka::Producer::create(conf_.get(), errstr));
            if (producer_ == nullptr) {
                throw_clog_ex(fmt_lib::format("create producer failed err:{}", errstr));
            }
            topic_.reset(RdKafka::Topic::create(producer_.get(), config_.produce_topic,
                                                tconf_.get(), errstr));
            if (topic_ == nullptr) {
                throw_clog_ex(fmt_lib::format("create topic failed err:{}", errstr));
            }
        } catch (const std::exception &e) {
            throw_clog_ex(fmt_lib::format("error create kafka instance: {}", e.what()));
        }
    }

    ~kafka_sink() { producer_->flush(config_.flush_timeout_ms); }

protected:
    void sink_it_(const details::log_msg &msg) override {
        producer_->produce(topic_.get(), 0, RdKafka::Producer::RK_MSG_COPY,
                           (void *)msg.payload.data(), msg.payload.size(), NULL, NULL);
    }

    void flush_() override { producer_->flush(config_.flush_timeout_ms); }

private:
    kafka_sink_config config_;
    std::unique_ptr<RdKafka::Producer> producer_ = nullptr;
    std::unique_ptr<RdKafka::Conf> conf_ = nullptr;
    std::unique_ptr<RdKafka::Conf> tconf_ = nullptr;
    std::unique_ptr<RdKafka::Topic> topic_ = nullptr;
};

using kafka_sink_mt = kafka_sink<std::mutex>;
using kafka_sink_st = kafka_sink<collie::log::details::null_mutex>;

}  // namespace sinks

template <typename Factory = collie::log::synchronous_factory>
inline std::shared_ptr<logger> kafka_logger_mt(const std::string &logger_name,
                                               collie::log::sinks::kafka_sink_config config) {
    return Factory::template create<sinks::kafka_sink_mt>(logger_name, config);
}

template <typename Factory = collie::log::synchronous_factory>
inline std::shared_ptr<logger> kafka_logger_st(const std::string &logger_name,
                                               collie::log::sinks::kafka_sink_config config) {
    return Factory::template create<sinks::kafka_sink_st>(logger_name, config);
}

template <typename Factory = collie::log::async_factory>
inline std::shared_ptr<collie::log::logger> kafka_logger_async_mt(
    std::string logger_name, collie::log::sinks::kafka_sink_config config) {
    return Factory::template create<sinks::kafka_sink_mt>(logger_name, config);
}

template <typename Factory = collie::log::async_factory>
inline std::shared_ptr<collie::log::logger> kafka_logger_async_st(
    std::string logger_name, collie::log::sinks::kafka_sink_config config) {
    return Factory::template create<sinks::kafka_sink_st>(logger_name, config);
}

}  // namespace collie::log
