#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <collie/testing/doctest.h>
#include <collie/taskflow/taskflow.h>

TEST_CASE("SimplePriority.Sequential" * doctest::timeout(300)) {
  
  collie::tf::Executor executor(1);
  collie::tf::Taskflow taskflow;

  int counter = 0;

  auto [A, B, C, D, E] = taskflow.emplace(
    [&] () { counter = 0; },
    [&] () { REQUIRE(counter == 0); counter++; },
    [&] () { REQUIRE(counter == 2); counter++; },
    [&] () { REQUIRE(counter == 1); counter++; },
    [&] () { }
  );

  A.precede(B, C, D); 
  E.succeed(B, C, D);
  
  REQUIRE(B.priority() == collie::tf::TaskPriority::HIGH);
  REQUIRE(C.priority() == collie::tf::TaskPriority::HIGH);
  REQUIRE(D.priority() == collie::tf::TaskPriority::HIGH);

  B.priority(collie::tf::TaskPriority::HIGH);
  C.priority(collie::tf::TaskPriority::LOW);
  D.priority(collie::tf::TaskPriority::NORMAL);

  REQUIRE(B.priority() == collie::tf::TaskPriority::HIGH);
  REQUIRE(C.priority() == collie::tf::TaskPriority::LOW);
  REQUIRE(D.priority() == collie::tf::TaskPriority::NORMAL);

  executor.run_n(taskflow, 100).wait();
}

TEST_CASE("RandomPriority.Sequential" * doctest::timeout(300)) {
  
  collie::tf::Executor executor(1);
  collie::tf::Taskflow taskflow;

  const auto MAX_P = static_cast<unsigned>(collie::tf::TaskPriority::MAX);

  auto beg = taskflow.emplace([](){});
  auto end = taskflow.emplace([](){});

  size_t counters[MAX_P];
  size_t priorities[MAX_P];

  for(unsigned p=0; p<MAX_P; p++) {
    counters[p] = 0;
    priorities[p] = 0;
  }

  for(size_t i=0; i<10000; i++) {
    unsigned p = ::rand() % MAX_P;
    taskflow.emplace([p, &counters](){ counters[p]++; })
            .priority(static_cast<collie::tf::TaskPriority>(p))
            .succeed(beg)
            .precede(end);
    priorities[p]++;
  }

  executor.run(taskflow).wait();

  for(unsigned p=0; p<MAX_P; p++) {
    REQUIRE(priorities[p] == counters[p]);
  }

}

TEST_CASE("RandomPriority.Parallel" * doctest::timeout(300)) {
  
  collie::tf::Executor executor;
  collie::tf::Taskflow taskflow;

  const auto MAX_P = static_cast<unsigned>(collie::tf::TaskPriority::MAX);

  auto beg = taskflow.emplace([](){});
  auto end = taskflow.emplace([](){});

  std::atomic<size_t> counters[MAX_P];
  size_t priorities[MAX_P];

  for(unsigned p=0; p<MAX_P; p++) {
    counters[p] = 0;
    priorities[p] = 0;
  }

  for(size_t i=0; i<10000; i++) {
    unsigned p = ::rand() % MAX_P;
    taskflow.emplace([p, &counters](){ counters[p]++; })
            .priority(static_cast<collie::tf::TaskPriority>(p))
            .succeed(beg)
            .precede(end);
    priorities[p]++;
  }

  executor.run_n(taskflow, 2).wait();

  for(unsigned p=0; p<MAX_P; p++) {
    REQUIRE(counters[p]!=0);
    //std::cout << priorities[p] << ' ' << counters[p] << '\n';
  }

}





