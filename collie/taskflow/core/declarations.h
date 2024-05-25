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

namespace collie::tf {

// ----------------------------------------------------------------------------
// taskflow
// ----------------------------------------------------------------------------
class AsyncTopology;
class Node;
class Graph;
class FlowBuilder;
class Semaphore;
class Subflow;
class Runtime;
class Task;
class TaskView;
class Taskflow;
class Topology;
class TopologyBase;
class Executor;
class Worker;
class WorkerView;
class ObserverInterface;
class ChromeTracingObserver;
class TFProfObserver;
class TFProfManager;

template <typename T>
class Future;

template <typename...Fs>
class Pipeline;

// ----------------------------------------------------------------------------
// cudaFlow
// ----------------------------------------------------------------------------
class cudaFlowNode;
class cudaFlowGraph;
class cudaTask;
class cudaFlow;
class cudaFlowCapturer;
class cudaFlowOptimizerBase;
class cudaFlowLinearOptimizer;
class cudaFlowSequentialOptimizer;
class cudaFlowRoundRobinOptimizer;

// ----------------------------------------------------------------------------
// syclFlow
// ----------------------------------------------------------------------------
class syclNode;
class syclGraph;
class syclTask;
class syclFlow;

// ----------------------------------------------------------------------------
// struct 
// ----------------------------------------------------------------------------
struct TaskParams;
struct DefaultTaskParams;


}  // end of namespace collie::tf -----------------------------------------------------




