// This program demonstrates how to create a pipeline scheduling framework
// that propagates a series of integers and adds one to the result at each
// stage, using a range of pipes provided by the application.
//
// The pipeline has the following structure:
//
// o -> o -> o
// |    |    |
// v    v    v
// o -> o -> o
// |    |    |
// v    v    v
// o -> o -> o
// |    |    |
// v    v    v
// o -> o -> o
//
// Then, the program resets the pipeline to a new range of five pipes.
//
// o -> o -> o -> o -> o
// |    |    |    |    |
// v    v    v    v    v
// o -> o -> o -> o -> o
// |    |    |    |    |
// v    v    v    v    v
// o -> o -> o -> o -> o
// |    |    |    |    |
// v    v    v    v    v
// o -> o -> o -> o -> o

#include <collie/taskflow/taskflow.h>
#include <collie/taskflow/algorithm/pipeline.h>

int main() {

  collie::tf::Taskflow taskflow("pipeline");
  collie::tf::Executor executor;

  const size_t num_lines = 4;

    //1. How can I put a placeholder in the first pipe, i.e. [] (void, collie::tf::Pipeflow&) in order to match the pipe vector?
    auto pipe_callable1 = [] (collie::tf::Pipeflow& pf) mutable -> int {
        if(pf.token() == 5) {
          pf.stop();
          return 0;
        }
        else {
          printf("stage 1: input token = %zu\n", pf.token());
          return pf.token();
        }
    };
    auto pipe_callable2 = [] (int input, collie::tf::Pipeflow& pf) mutable -> float {
        return input + 1.0;
    };
    auto pipe_callable3 = [] (float input, collie::tf::Pipeflow& pf) mutable -> int {
        return input + 1;
    };

  //2. Is this ok when the type in vector definition is different from the exact types of emplaced elements?
  std::vector< ScalableDataPipeBase* > pipes;

  pipes.emplace_back(collie::tf::make_scalable_datapipe<void, int>(collie::tf::PipeType::SERIAL, pipe_callable1));
  pipes.emplace_back(collie::tf::make_scalable_datapipe<int, float>(collie::tf::PipeType::SERIAL, pipe_callable2));
  pipes.emplace_back(collie::tf::make_scalable_datapipe<float, int>(collie::tf::PipeType::SERIAL, pipe_callable3));

  // create a pipeline of four parallel lines using the given vector of pipes
  collie::tf::ScalablePipeline<decltype(pipes)::iterator> pl(num_lines, pipes.begin(), pipes.end());

  // build the pipeline graph using composition
  collie::tf::Task init = taskflow.emplace([](){ std::cout << "ready\n"; })
                          .name("starting pipeline");
  collie::tf::Task task = taskflow.composed_of(pl)
                          .name("pipeline");
  collie::tf::Task stop = taskflow.emplace([](){ std::cout << "stopped\n"; })
                          .name("pipeline stopped");

  // create task dependency
  init.precede(task);
  task.precede(stop);

  // dump the pipeline graph structure (with composition)
  taskflow.dump(std::cout);

  // run the pipeline
  executor.run(taskflow).wait();

  // reset the pipeline to a new range of five pipes and starts from
  // the initial state (i.e., token counts from zero)
  pipes.emplace_back(collie::tf::make_scalable_datapipe<int, float>(collie::tf::PipeType::SERIAL, pipe_callable1));
  pipes.emplace_back(collie::tf::make_scalable_datapipe<float, int>(collie::tf::PipeType::SERIAL, pipe_callable1));
  pl.reset(pipes.begin(), pipes.end());

  executor.run(taskflow).wait();

  return 0;
}



