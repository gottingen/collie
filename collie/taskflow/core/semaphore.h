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

#include <vector>
#include <mutex>

#include <collie/taskflow/core/declarations.h>

/**
@file semaphore.hpp
@brief semaphore include file
*/

namespace collie::tf {

// ----------------------------------------------------------------------------
// Semaphore
// ----------------------------------------------------------------------------

/**
@class Semaphore

@brief class to create a semophore object for building a concurrency constraint

A semaphore creates a constraint that limits the maximum concurrency,
i.e., the number of workers, in a set of tasks.
You can let a task acquire/release one or multiple semaphores before/after
executing its work.
A task can acquire and release a semaphore,
or just acquire or just release it.
A collie::tf::Semaphore object starts with an initial count.
As long as that count is above 0, tasks can acquire the semaphore and do
their work.
If the count is 0 or less, a task trying to acquire the semaphore will not run
but goes to a waiting list of that semaphore.
When the semaphore is released by another task,
it reschedules all tasks on that waiting list.

@code{.cpp}
collie::tf::Executor executor(8);   // create an executor of 8 workers
collie::tf::Taskflow taskflow;

collie::tf::Semaphore semaphore(1); // create a semaphore with initial count 1

std::vector<collie::tf::Task> tasks {
  taskflow.emplace([](){ std::cout << "A" << std::endl; }),
  taskflow.emplace([](){ std::cout << "B" << std::endl; }),
  taskflow.emplace([](){ std::cout << "C" << std::endl; }),
  taskflow.emplace([](){ std::cout << "D" << std::endl; }),
  taskflow.emplace([](){ std::cout << "E" << std::endl; })
};

for(auto & task : tasks) {  // each task acquires and release the semaphore
  task.acquire(semaphore);
  task.release(semaphore);
}

executor.run(taskflow).wait();
@endcode

The above example creates five tasks with no dependencies between them.
Under normal circumstances, the five tasks would be executed concurrently.
However, this example has a semaphore with initial count 1,
and all tasks need to acquire that semaphore before running and release that
semaphore after they are done.
This arrangement limits the number of concurrently running tasks to only one.

*/
class Semaphore {

  friend class Node;

  public:

    /**
    @brief constructs a semaphore with the given counter

    A semaphore creates a constraint that limits the maximum concurrency,
    i.e., the number of workers, in a set of tasks.

    @code{.cpp}
    collie::tf::Semaphore semaphore(4);  // concurrency constraint of 4 workers
    @endcode
    */
    explicit Semaphore(size_t max_workers);

    /**
    @brief queries the counter value (not thread-safe during the run)
    */
    size_t count() const;

  private:

    std::mutex _mtx;

    size_t _counter;

    std::vector<Node*> _waiters;

    bool _try_acquire_or_wait(Node*);

    std::vector<Node*> _release();
};

inline Semaphore::Semaphore(size_t max_workers) :
  _counter(max_workers) {
}

inline bool Semaphore::_try_acquire_or_wait(Node* me) {
  std::lock_guard<std::mutex> lock(_mtx);
  if(_counter > 0) {
    --_counter;
    return true;
  }
  else {
    _waiters.push_back(me);
    return false;
  }
}

inline std::vector<Node*> Semaphore::_release() {
  std::lock_guard<std::mutex> lock(_mtx);
  ++_counter;
  std::vector<Node*> r{std::move(_waiters)};
  return r;
}

inline size_t Semaphore::count() const {
  return _counter;
}

}  // end of namespace collie::tf. ---------------------------------------------------

