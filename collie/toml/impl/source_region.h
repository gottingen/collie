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

#include <collie/toml/impl/std_optional.h>
#include <collie/toml/impl/std_string.h>
#include <collie/toml/impl/forward_declarations.h>
#include <collie/toml/impl/print_to_stream.h>
#include <collie/toml/impl/header_start.h>

TOML_NAMESPACE_START
{
	/// \brief	The integer type used to tally line numbers and columns.
	using source_index = uint32_t;

	/// \brief	A pointer to a shared string resource containing a source path.
	using source_path_ptr = std::shared_ptr<const std::string>;

	/// \brief	A source document line-and-column pair.
	///
	/// \detail \cpp
	/// auto table = toml::parse_file("config.toml"sv);
	/// std::cout << "The node 'description' was defined at "sv
	///		<< table.get("description")->source().begin()
	///		<< "\n";
	/// \ecpp
	///
	/// \out
	///	The value 'description' was defined at line 7, column 15
	/// \eout
	///
	/// \remarks toml++'s parser is unicode-aware insofar as it knows how to handle
	/// 		 non-ASCII whitespace and newline characters, but it doesn't give much thought
	///			 to combining marks, grapheme clusters vs. characters, et cetera.
	/// 		 If a TOML document contains lots of codepoints outside of the ASCII range
	/// 		 you may find that your source_positions don't match those given by a text editor
	/// 		 (typically the line numbers will be accurate but column numbers will be too high).
	/// 		 <strong>This is not an error.</strong> I've chosen this behaviour as a deliberate trade-off
	/// 		 between parser complexity and correctness.
	struct TOML_TRIVIAL_ABI source_position
	{
		/// \brief The line number.
		/// \remarks Valid line numbers start at 1.
		source_index line;

		/// \brief The column number.
		/// \remarks Valid column numbers start at 1.
		source_index column;

		/// \brief	Returns true if both line and column numbers are non-zero.
		TOML_PURE_GETTER
		explicit constexpr operator bool() const noexcept
		{
			return line > source_index{} //
				&& column > source_index{};
		}

		/// \brief	Equality operator.
		TOML_PURE_GETTER
		friend constexpr bool operator==(const source_position& lhs, const source_position& rhs) noexcept
		{
			return lhs.line == rhs.line //
				&& lhs.column == rhs.column;
		}

		/// \brief	Inequality operator.
		TOML_PURE_INLINE_GETTER
		friend constexpr bool operator!=(const source_position& lhs, const source_position& rhs) noexcept
		{
			return !(lhs == rhs);
		}

	  private:
		/// \cond

		TOML_PURE_GETTER
		static constexpr uint64_t pack(const source_position& pos) noexcept
		{
			return static_cast<uint64_t>(pos.line) << 32 | static_cast<uint64_t>(pos.column);
		}

		/// \endcond

	  public:
		/// \brief	Less-than operator.
		TOML_PURE_GETTER
		friend constexpr bool operator<(const source_position& lhs, const source_position& rhs) noexcept
		{
			return pack(lhs) < pack(rhs);
		}

		/// \brief	Less-than-or-equal-to operator.
		TOML_PURE_GETTER
		friend constexpr bool operator<=(const source_position& lhs, const source_position& rhs) noexcept
		{
			return pack(lhs) <= pack(rhs);
		}

		/// \brief	Greater-than operator.
		TOML_PURE_GETTER
		friend constexpr bool operator>(const source_position& lhs, const source_position& rhs) noexcept
		{
			return pack(lhs) > pack(rhs);
		}

		/// \brief	Greater-than-or-equal-to operator.
		TOML_PURE_GETTER
		friend constexpr bool operator>=(const source_position& lhs, const source_position& rhs) noexcept
		{
			return pack(lhs) >= pack(rhs);
		}

		/// \brief	Prints a source_position to a stream.
		///
		/// \detail \cpp
		/// auto tbl = toml::parse("bar = 42"sv);
		///
		/// std::cout << "The value for 'bar' was found on "sv
		///		<< tbl.get("bar")->source().begin()
		///		<< "\n";
		/// \ecpp
		///
		/// \out
		/// The value for 'bar' was found on line 1, column 7
		/// \eout
		///
		/// \param 	lhs	The stream.
		/// \param 	rhs	The source_position.
		///
		/// \returns	The input stream.
		friend std::ostream& operator<<(std::ostream& lhs, const source_position& rhs)
		{
			impl::print_to_stream(lhs, rhs);
			return lhs;
		}
	};

	/// \brief	A source document region.
	///
	/// \detail \cpp
	/// auto tbl = toml::parse_file("config.toml"sv);
	/// if (auto server = tbl.get("server"))
	/// {
	///		std::cout << "begin: "sv << server->source().begin << "\n";
	///		std::cout << "end: "sv << server->source().end << "\n";
	///		std::cout << "path: "sv << *server->source().path << "\n";
	///	}
	/// \ecpp
	///
	/// \out
	///	begin: line 3, column 1
	///	end: line 3, column 22
	///	path: config.toml
	/// \eout
	///
	/// \remarks toml++'s parser is unicode-aware insofar as it knows how to handle
	/// 		 non-ASCII whitespace and newline characters, but it doesn't give much thought
	///			 to combining marks, grapheme clusters vs. characters, et cetera.
	/// 		 If a TOML document contains lots of codepoints outside of the ASCII range
	/// 		 you may find that your source_positions don't match those given by a text editor
	/// 		 (typically the line numbers will be accurate but column numbers will be too high).
	/// 		 <strong>This is not an error.</strong> I've chosen this behaviour as a deliberate trade-off
	/// 		 between parser complexity and correctness.
	struct source_region
	{
		/// \brief The beginning of the region (inclusive).
		source_position begin;

		/// \brief The end of the region (exclusive).
		source_position end;

		/// \brief	The path to the corresponding source document.
		///
		/// \remarks This will be `nullptr` if no path was provided to toml::parse().
		source_path_ptr path;

#if TOML_ENABLE_WINDOWS_COMPAT

		/// \brief	The path to the corresponding source document as a wide-string.
		///
		/// \availability This function is only available when #TOML_ENABLE_WINDOWS_COMPAT is enabled.
		///
		/// \remarks This will return an empty optional if no path was provided to toml::parse().
		TOML_NODISCARD
		optional<std::wstring> wide_path() const
		{
			if (!path || path->empty())
				return {};
			return { impl::widen(*path) };
		}

#endif

		/// \brief	Prints a source_region to a stream.
		///
		/// \detail \cpp
		/// auto tbl = toml::parse("bar = 42", "config.toml");
		///
		/// std::cout << "The value for 'bar' was found on "sv
		///		<< tbl.get("bar")->source()
		///		<< "\n";
		/// \ecpp
		///
		/// \out
		/// The value for 'bar' was found on line 1, column 7 of 'config.toml'
		/// \eout
		///
		/// \param 	lhs	The stream.
		/// \param 	rhs	The source_position.
		///
		/// \returns	The input stream.
		friend std::ostream& operator<<(std::ostream& lhs, const source_region& rhs)
		{
			impl::print_to_stream(lhs, rhs);
			return lhs;
		}
	};
}
TOML_NAMESPACE_END;

#include <collie/toml/impl/header_end.h>
