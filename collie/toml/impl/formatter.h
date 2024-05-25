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

#include <collie/toml/impl/preprocessor.h>
#if TOML_ENABLE_FORMATTERS

#include <collie/toml/impl/forward_declarations.h>
#include <collie/toml/impl/print_to_stream.h>
#include <collie/toml/impl/header_start.h>
/// \cond

TOML_IMPL_NAMESPACE_START
{
	struct formatter_constants
	{
		format_flags mandatory_flags;
		format_flags ignored_flags;

		std::string_view float_pos_inf;
		std::string_view float_neg_inf;
		std::string_view float_nan;

		std::string_view bool_true;
		std::string_view bool_false;
	};

	struct formatter_config
	{
		format_flags flags;
		std::string_view indent;
	};

	class TOML_EXPORTED_CLASS formatter
	{
	  private:
		const node* source_;
#if TOML_ENABLE_PARSER && !TOML_EXCEPTIONS
		const parse_result* result_;
#endif
		const formatter_constants* constants_;
		formatter_config config_;
		size_t indent_columns_;
		format_flags int_format_mask_;
		std::ostream* stream_; //
		int indent_;		   // these are set in attach()
		bool naked_newline_;   //

	  protected:
		TOML_PURE_INLINE_GETTER
		const node& source() const noexcept
		{
			return *source_;
		}

		TOML_PURE_INLINE_GETTER
		std::ostream& stream() const noexcept
		{
			return *stream_;
		}

		TOML_PURE_INLINE_GETTER
		int indent() const noexcept
		{
			return indent_;
		}

		void indent(int level) noexcept
		{
			indent_ = level;
		}

		void increase_indent() noexcept
		{
			indent_++;
		}

		void decrease_indent() noexcept
		{
			indent_--;
		}

		TOML_PURE_INLINE_GETTER
		size_t indent_columns() const noexcept
		{
			return indent_columns_;
		}

		TOML_PURE_INLINE_GETTER
		bool indent_array_elements() const noexcept
		{
			return !!(config_.flags & format_flags::indent_array_elements);
		}

		TOML_PURE_INLINE_GETTER
		bool indent_sub_tables() const noexcept
		{
			return !!(config_.flags & format_flags::indent_sub_tables);
		}

		TOML_PURE_INLINE_GETTER
		bool literal_strings_allowed() const noexcept
		{
			return !!(config_.flags & format_flags::allow_literal_strings);
		}

		TOML_PURE_INLINE_GETTER
		bool multi_line_strings_allowed() const noexcept
		{
			return !!(config_.flags & format_flags::allow_multi_line_strings);
		}

		TOML_PURE_INLINE_GETTER
		bool real_tabs_in_strings_allowed() const noexcept
		{
			return !!(config_.flags & format_flags::allow_real_tabs_in_strings);
		}

		TOML_PURE_INLINE_GETTER
		bool unicode_strings_allowed() const noexcept
		{
			return !!(config_.flags & format_flags::allow_unicode_strings);
		}

		TOML_EXPORTED_MEMBER_FUNCTION
		void attach(std::ostream& stream) noexcept;

		TOML_EXPORTED_MEMBER_FUNCTION
		void detach() noexcept;

		TOML_EXPORTED_MEMBER_FUNCTION
		void print_newline(bool force = false);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print_indent();

		TOML_EXPORTED_MEMBER_FUNCTION
		void print_unformatted(char);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print_unformatted(std::string_view);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print_string(std::string_view str, bool allow_multi_line = true, bool allow_bare = false);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print(const value<std::string>&);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print(const value<int64_t>&);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print(const value<double>&);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print(const value<bool>&);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print(const value<date>&);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print(const value<time>&);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print(const value<date_time>&);

		TOML_EXPORTED_MEMBER_FUNCTION
		void print_value(const node&, node_type);

		TOML_NODISCARD
		TOML_EXPORTED_MEMBER_FUNCTION
		bool dump_failed_parse_result();

		TOML_NODISCARD_CTOR
		TOML_EXPORTED_MEMBER_FUNCTION
		formatter(const node*, const parse_result*, const formatter_constants&, const formatter_config&) noexcept;
	};
}
TOML_IMPL_NAMESPACE_END;

/// \endcond
#include <collie/toml/impl/header_end.h>
#endif // TOML_ENABLE_FORMATTERS
