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

#include <collie/toml/impl/source_region.h>
#include <collie/toml/impl/std_utility.h>
#include <collie/toml/impl/print_to_stream.h>
#include <collie/toml/impl/header_start.h>

TOML_NAMESPACE_START
{
	/// \brief A key parsed from a TOML document.
	///
	/// \detail These are used as the internal keys for a toml::table: \cpp
	/// const toml::table tbl = R"(
	///     a = 1
	///       b = 2
	///         c = 3
	/// )"_toml;
	///
	/// for (auto&& [k, v] : tbl)
	/// 	std::cout << "key '"sv << k << "' defined at "sv << k.source() << "\n";
	/// \ecpp
	/// \out
	/// key 'a' defined at line 2, column 5
	/// key 'b' defined at line 3, column 7
	/// key 'c' defined at line 4, column 9
	/// \eout
	class key
	{
	  private:
		std::string key_;
		source_region source_;

	  public:
		/// \brief	Default constructor.
		TOML_NODISCARD_CTOR
		key() noexcept = default;

		/// \brief	Constructs a key from a string view and source region.
		TOML_NODISCARD_CTOR
		explicit key(std::string_view k, source_region&& src = {}) //
			: key_{ k },
			  source_{ std::move(src) }
		{}

		/// \brief	Constructs a key from a string view and source region.
		TOML_NODISCARD_CTOR
		explicit key(std::string_view k, const source_region& src) //
			: key_{ k },
			  source_{ src }
		{}

		/// \brief	Constructs a key from a string and source region.
		TOML_NODISCARD_CTOR
		explicit key(std::string&& k, source_region&& src = {}) noexcept //
			: key_{ std::move(k) },
			  source_{ std::move(src) }
		{}

		/// \brief	Constructs a key from a string and source region.
		TOML_NODISCARD_CTOR
		explicit key(std::string&& k, const source_region& src) noexcept //
			: key_{ std::move(k) },
			  source_{ src }
		{}

		/// \brief	Constructs a key from a c-string and source region.
		TOML_NODISCARD_CTOR
		explicit key(const char* k, source_region&& src = {}) //
			: key_{ k },
			  source_{ std::move(src) }
		{}

		/// \brief	Constructs a key from a c-string view and source region.
		TOML_NODISCARD_CTOR
		explicit key(const char* k, const source_region& src) //
			: key_{ k },
			  source_{ src }
		{}

#if TOML_ENABLE_WINDOWS_COMPAT

		/// \brief	Constructs a key from a wide string view and source region.
		///
		/// \availability This constructor is only available when #TOML_ENABLE_WINDOWS_COMPAT is enabled.
		TOML_NODISCARD_CTOR
		explicit key(std::wstring_view k, source_region&& src = {}) //
			: key_{ impl::narrow(k) },
			  source_{ std::move(src) }
		{}

		/// \brief	Constructs a key from a wide string and source region.
		///
		/// \availability This constructor is only available when #TOML_ENABLE_WINDOWS_COMPAT is enabled.
		TOML_NODISCARD_CTOR
		explicit key(std::wstring_view k, const source_region& src) //
			: key_{ impl::narrow(k) },
			  source_{ src }
		{}

#endif

		/// \name String operations
		/// @{

		/// \brief	Returns a view of the key's underlying string.
		TOML_PURE_INLINE_GETTER
		std::string_view str() const noexcept
		{
			return std::string_view{ key_ };
		}

		/// \brief	Returns a view of the key's underlying string.
		TOML_PURE_INLINE_GETTER
		/*implicit*/ operator std::string_view() const noexcept
		{
			return str();
		}

		/// \brief	Returns true if the key's underlying string is empty.
		TOML_PURE_INLINE_GETTER
		bool empty() const noexcept
		{
			return key_.empty();
		}

		/// \brief	Returns a pointer to the start of the key's underlying string.
		TOML_PURE_INLINE_GETTER
		const char* data() const noexcept
		{
			return key_.data();
		}

		/// \brief	Returns the length of the key's underlying string.
		TOML_PURE_INLINE_GETTER
		size_t length() const noexcept
		{
			return key_.length();
		}

		/// @}

		/// \name Metadata
		/// @{

		/// \brief	Returns the source region responsible for specifying this key during parsing.
		TOML_PURE_INLINE_GETTER
		const source_region& source() const noexcept
		{
			return source_;
		}

		/// @}

		/// \name Equality and Comparison
		/// \attention These operations only compare the underlying strings; source regions are ignored for the purposes of all comparison!
		/// @{

		/// \brief	Returns true if `lhs.str() == rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator==(const key& lhs, const key& rhs) noexcept
		{
			return lhs.key_ == rhs.key_;
		}

		/// \brief	Returns true if `lhs.str() != rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator!=(const key& lhs, const key& rhs) noexcept
		{
			return lhs.key_ != rhs.key_;
		}

		/// \brief	Returns true if `lhs.str() < rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator<(const key& lhs, const key& rhs) noexcept
		{
			return lhs.key_ < rhs.key_;
		}

		/// \brief	Returns true if `lhs.str() <= rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator<=(const key& lhs, const key& rhs) noexcept
		{
			return lhs.key_ <= rhs.key_;
		}

		/// \brief	Returns true if `lhs.str() > rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator>(const key& lhs, const key& rhs) noexcept
		{
			return lhs.key_ > rhs.key_;
		}

		/// \brief	Returns true if `lhs.str() >= rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator>=(const key& lhs, const key& rhs) noexcept
		{
			return lhs.key_ >= rhs.key_;
		}

		/// \brief	Returns true if `lhs.str() == rhs`.
		TOML_PURE_INLINE_GETTER
		friend bool operator==(const key& lhs, std::string_view rhs) noexcept
		{
			return lhs.key_ == rhs;
		}

		/// \brief	Returns true if `lhs.str() != rhs`.
		TOML_PURE_INLINE_GETTER
		friend bool operator!=(const key& lhs, std::string_view rhs) noexcept
		{
			return lhs.key_ != rhs;
		}

		/// \brief	Returns true if `lhs.str() < rhs`.
		TOML_PURE_INLINE_GETTER
		friend bool operator<(const key& lhs, std::string_view rhs) noexcept
		{
			return lhs.key_ < rhs;
		}

		/// \brief	Returns true if `lhs.str() <= rhs`.
		TOML_PURE_INLINE_GETTER
		friend bool operator<=(const key& lhs, std::string_view rhs) noexcept
		{
			return lhs.key_ <= rhs;
		}

		/// \brief	Returns true if `lhs.str() > rhs`.
		TOML_PURE_INLINE_GETTER
		friend bool operator>(const key& lhs, std::string_view rhs) noexcept
		{
			return lhs.key_ > rhs;
		}

		/// \brief	Returns true if `lhs.str() >= rhs`.
		TOML_PURE_INLINE_GETTER
		friend bool operator>=(const key& lhs, std::string_view rhs) noexcept
		{
			return lhs.key_ >= rhs;
		}

		/// \brief	Returns true if `lhs == rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator==(std::string_view lhs, const key& rhs) noexcept
		{
			return lhs == rhs.key_;
		}

		/// \brief	Returns true if `lhs != rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator!=(std::string_view lhs, const key& rhs) noexcept
		{
			return lhs != rhs.key_;
		}

		/// \brief	Returns true if `lhs < rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator<(std::string_view lhs, const key& rhs) noexcept
		{
			return lhs < rhs.key_;
		}

		/// \brief	Returns true if `lhs <= rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator<=(std::string_view lhs, const key& rhs) noexcept
		{
			return lhs <= rhs.key_;
		}

		/// \brief	Returns true if `lhs > rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator>(std::string_view lhs, const key& rhs) noexcept
		{
			return lhs > rhs.key_;
		}

		/// \brief	Returns true if `lhs >= rhs.str()`.
		TOML_PURE_INLINE_GETTER
		friend bool operator>=(std::string_view lhs, const key& rhs) noexcept
		{
			return lhs >= rhs.key_;
		}

		/// @}

		/// \name Iteration
		/// @{

		/// \brief A const iterator for iterating over the characters in the key.
		using const_iterator = const char*;

		/// \brief A const iterator for iterating over the characters in the key.
		using iterator = const_iterator;

		/// \brief Returns an iterator to the first character in the key's backing string.
		TOML_PURE_INLINE_GETTER
		const_iterator begin() const noexcept
		{
			return key_.data();
		}

		/// \brief Returns an iterator to one-past-the-last character in the key's backing string.
		TOML_PURE_INLINE_GETTER
		const_iterator end() const noexcept
		{
			return key_.data() + key_.length();
		}

		/// @}

		/// \brief	Prints the key's underlying string out to the stream.
		friend std::ostream& operator<<(std::ostream& lhs, const key& rhs)
		{
			impl::print_to_stream(lhs, rhs.key_);
			return lhs;
		}
	};

	/// \brief	Metafunction for determining if a type is, or is a reference to, a toml::key.
	template <typename T>
	inline constexpr bool is_key = std::is_same_v<impl::remove_cvref<T>, toml::key>;

	/// \brief	Metafunction for determining if a type is, or is a reference to, a toml::key,
	///			or is implicitly or explicitly convertible to one.
	template <typename T>
	inline constexpr bool is_key_or_convertible = is_key<T> //
											   || impl::is_constructible_or_convertible<toml::key, T>;
}
TOML_NAMESPACE_END;

#include <collie/toml/impl/header_end.h>
