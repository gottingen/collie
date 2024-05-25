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
#include <collie/taskflow/core/flow_builder.h>
#include <collie/taskflow/core/task.h>
#include <collie/taskflow/dsl/type_list.h>
#include <type_traits>

namespace collie::tf {
namespace dsl {
struct TaskSignature {};

template <typename TASK, typename CONTEXT> struct TaskCb {
  using TaskType = TASK;
  void build(FlowBuilder &build, const CONTEXT &context) {
    task_ = build.emplace(TaskType{context}());
  }

  Task task_;
};

template <typename TASK> struct IsTask {
  template <typename TaskCb> struct apply {
    constexpr static bool value =
        std::is_same<typename TaskCb::TaskType, TASK>::value;
  };
};

template <typename TASK, typename = void> struct TaskTrait;

template <typename... TASK> struct SomeTask {
  using TaskList =
      Unique_t<Flatten_t<TypeList<typename TaskTrait<TASK>::TaskList...>>>;
};

// a task self
template <typename TASK>
struct TaskTrait<
    TASK, std::enable_if_t<std::is_base_of<TaskSignature, TASK>::value>> {
  using TaskList = TypeList<TASK>;
};

template <typename... TASK> struct TaskTrait<SomeTask<TASK...>> {
  using TaskList = typename SomeTask<TASK...>::TaskList;
};
} // namespace dsl
} // namespace collie::tf
