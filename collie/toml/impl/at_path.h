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

#include <collie/toml/impl/forward_declarations.h>

/// \cond
TOML_IMPL_NAMESPACE_START
{
	template <typename T>
	using parse_path_callback = bool(TOML_CALLCONV*)(void*, T);

	TOML_NODISCARD
	bool TOML_CALLCONV parse_path(std::string_view,
								  void*,
								  parse_path_callback<std::string_view>,
								  parse_path_callback<size_t>);
}
TOML_IMPL_NAMESPACE_END;
/// \endcond

TOML_NAMESPACE_START
{
	/// \brief Returns a view of the node matching a fully-qualified "TOML path".
	///
	/// \detail \cpp
	/// auto config = toml::parse(R"(
	///
	/// [foo]
	/// bar = [ 0, 1, 2, [ 3 ], { kek = 4 } ]
	///
	/// )"sv);
	///
	/// std::cout << toml::at_path(config, "foo.bar[2]") << "\n";
	/// std::cout << toml::at_path(config, "foo.bar[3][0]") << "\n";
	/// std::cout << toml::at_path(config, "foo.bar[4].kek") << "\n";
	/// \ecpp
	///
	/// \out
	/// 2
	/// 3
	/// 4
	/// \eout
	///
	///
	/// \note Keys in paths are interpreted literally, so whitespace (or lack thereof) matters:
	/// \cpp
	/// toml::at_path(config, "foo.bar")  // same as config["foo"]["bar"]
	/// toml::at_path(config, "foo. bar") // same as config["foo"][" bar"]
	/// toml::at_path(config, "foo..bar") // same as config["foo"][""]["bar"]
	/// toml::at_path(config, ".foo.bar") // same as config[""]["foo"]["bar"]
	/// \ecpp
	/// <br>
	/// Additionally, TOML allows '.' (period) characters to appear in keys if they are quoted strings.
	/// This function makes no allowance for this, instead treating all period characters as sub-table delimiters.
	/// If you have periods in your table keys, first consider:
	/// 1. Not doing that
	/// 2. Using node_view::operator[] instead.
	///
	/// \param root		The root node from which the path will be traversed.
	/// \param path		The "TOML path" to traverse.
	TOML_NODISCARD
	TOML_EXPORTED_FREE_FUNCTION
	node_view<node> TOML_CALLCONV at_path(node & root, std::string_view path) noexcept;

	/// \brief Returns a const view of the node matching a fully-qualified "TOML path".
	///
	/// \see #toml::at_path(node&, std::string_view)
	TOML_NODISCARD
	TOML_EXPORTED_FREE_FUNCTION
	node_view<const node> TOML_CALLCONV at_path(const node& root, std::string_view path) noexcept;

#if TOML_ENABLE_WINDOWS_COMPAT

	/// \brief Returns a view of the node matching a fully-qualified "TOML path".
	///
	/// \availability This overload is only available when #TOML_ENABLE_WINDOWS_COMPAT is enabled.
	///
	/// \see #toml::at_path(node&, std::string_view)
	TOML_NODISCARD
	TOML_EXPORTED_FREE_FUNCTION
	node_view<node> TOML_CALLCONV at_path(node & root, std::wstring_view path);

	/// \brief Returns a const view of the node matching a fully-qualified "TOML path".
	///
	/// \availability This overload is only available when #TOML_ENABLE_WINDOWS_COMPAT is enabled.
	///
	/// \see #toml::at_path(node&, std::string_view)
	TOML_NODISCARD
	TOML_EXPORTED_FREE_FUNCTION
	node_view<const node> TOML_CALLCONV at_path(const node& root, std::wstring_view path);

#endif
}
TOML_NAMESPACE_END;
