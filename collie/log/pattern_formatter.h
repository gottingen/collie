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
#include <collie/log/details/log_msg.h>
#include <collie/log/details/os.h>
#include <collie/log/formatter.h>

#include <chrono>
#include <ctime>
#include <memory>

#include <string>
#include <unordered_map>
#include <vector>

namespace collie::log {
namespace details {

// padding information.
struct padding_info {
    enum class pad_side { left, right, center };

    padding_info() = default;
    padding_info(size_t width, padding_info::pad_side side, bool truncate)
        : width_(width),
          side_(side),
          truncate_(truncate),
          enabled_(true) {}

    bool enabled() const { return enabled_; }
    size_t width_ = 0;
    pad_side side_ = pad_side::left;
    bool truncate_ = false;
    bool enabled_ = false;
};

class  flag_formatter {
public:
    explicit flag_formatter(padding_info padinfo)
        : padinfo_(padinfo) {}
    flag_formatter() = default;
    virtual ~flag_formatter() = default;
    virtual void format(const details::log_msg &msg,
                        const std::tm &tm_time,
                        memory_buf_t &dest) = 0;

protected:
    padding_info padinfo_;
};

}  // namespace details

class  custom_flag_formatter : public details::flag_formatter {
public:
    virtual std::unique_ptr<custom_flag_formatter> clone() const = 0;

    void set_padding_info(const details::padding_info &padding) {
        flag_formatter::padinfo_ = padding;
    }
};

class  pattern_formatter final : public formatter {
public:
    using custom_flags = std::unordered_map<char, std::unique_ptr<custom_flag_formatter>>;

    explicit pattern_formatter(std::string pattern,
                               pattern_time_type time_type = pattern_time_type::local,
                               std::string eol = collie::log::details::os::default_eol,
                               custom_flags custom_user_flags = custom_flags());

    // use default pattern is not given
    explicit pattern_formatter(pattern_time_type time_type = pattern_time_type::local,
                               std::string eol = collie::log::details::os::default_eol);

    pattern_formatter(const pattern_formatter &other) = delete;
    pattern_formatter &operator=(const pattern_formatter &other) = delete;

    std::unique_ptr<formatter> clone() const override;
    void format(const details::log_msg &msg, memory_buf_t &dest) override;

    template <typename T, typename... Args>
    pattern_formatter &add_flag(char flag, Args &&...args) {
        custom_handlers_[flag] = details::make_unique<T>(std::forward<Args>(args)...);
        return *this;
    }
    void set_pattern(std::string pattern);
    void need_localtime(bool need = true);

private:
    std::string pattern_;
    std::string eol_;
    pattern_time_type pattern_time_type_;
    bool need_localtime_;
    std::tm cached_tm_;
    std::chrono::seconds last_log_secs_;
    std::vector<std::unique_ptr<details::flag_formatter>> formatters_;
    custom_flags custom_handlers_;

    std::tm get_time_(const details::log_msg &msg);
    template <typename Padder>
    void handle_flag_(char flag, details::padding_info padding);

    // Extract given pad spec (e.g. %8X)
    // Advance the given it pass the end of the padding spec found (if any)
    // Return padding.
    static details::padding_info handle_padspec_(std::string::const_iterator &it,
                                                 std::string::const_iterator end);

    void compile_pattern_(const std::string &pattern);
};
}  // namespace collie::log

#include <collie/log/pattern_formatter-inl.h>

