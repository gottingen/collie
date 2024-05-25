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
// 2020/08/28 - Created by netcan: https://github.com/netcan
#pragma once
#include <collie/taskflow/dsl/connection.h>
#include <collie/taskflow/dsl/type_list.h>
#include <type_traits>

namespace collie::tf {
namespace dsl {
template <typename... Links> class TaskAnalyzer {
  template <typename FROMs, typename TOs, typename = void>
  struct BuildOneToOneLink;

  template <typename... Fs, typename Ts>
  struct BuildOneToOneLink<TypeList<Fs...>, Ts> {
    using type = Concat_t<typename BuildOneToOneLink<Fs, Ts>::type...>;
  };

  template <typename F, typename... Ts>
  struct BuildOneToOneLink<F, TypeList<Ts...>,
                           std::enable_if_t<!IsTypeList_v<F>>> {
    using type = TypeList<OneToOneLink<F, Ts>...>;
  };

  template <typename Link> class OneToOneLinkSetF {
    using FromTaskList = typename Link::FromTaskList;
    using ToTaskList = typename Link::ToTaskList;

  public:
    using type = typename BuildOneToOneLink<FromTaskList, ToTaskList>::type;
  };

public:
  using AllTasks = Unique_t<
      Concat_t<typename Links::FromTaskList..., typename Links::ToTaskList...>>;
  using OneToOneLinkSet =
      Unique_t<Flatten_t<Map_t<TypeList<Links...>, OneToOneLinkSetF>>>;
};

} // namespace dsl
} // namespace collie::tf
