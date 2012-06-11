/*
 * ParallelRecursiveNQueens.h Parallel and recursive N-queens solver
 *
 *  Created on: 2011-10-11
 *      Author: Anders Gidenstam
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef PARALLELRECURSIVENQUEENS_H_
#define PARALLELRECURSIVENQUEENS_H_

#include <algorithm>

#include "../../../misc/types.h"
#include "ParallelRecursiveNQueensTask.h"

namespace pheet {

template <class Scheduler>
class ParallelRecursiveNQueens {
public:
  ParallelRecursiveNQueens(procs_t cpus, size_t N);
  ~ParallelRecursiveNQueens();

  int* solve();
  void print_results();
  void print_headers();

  static void print_scheduler_name();

  static const procs_t max_cpus;
  static const char name[];
  static const char * const scheduler_name;

private:
  size_t N;
  typename Scheduler::CPUHierarchy cpu_hierarchy;
  Scheduler scheduler;
};

template <class Scheduler>
procs_t const ParallelRecursiveNQueens<Scheduler>::max_cpus = Scheduler::max_cpus;

template <class Scheduler>
char const ParallelRecursiveNQueens<Scheduler>::name[] = "Parallel Recursive N-Queens";

template <class Scheduler>
char const * const ParallelRecursiveNQueens<Scheduler>::scheduler_name = Scheduler::name;

template <class Scheduler>
ParallelRecursiveNQueens<Scheduler>::ParallelRecursiveNQueens(procs_t cpus, size_t n)
  : N(n), cpu_hierarchy(cpus), scheduler(&cpu_hierarchy) {

}

template <class Scheduler>
ParallelRecursiveNQueens<Scheduler>::~ParallelRecursiveNQueens() {

}

template <class Scheduler>
int* ParallelRecursiveNQueens<Scheduler>::solve() {
  typename ParallelRecursiveNQueensTask<typename Scheduler::Task>::subproblem_t* problem;
  int* result = 0;

  problem = (typename ParallelRecursiveNQueensTask<typename Scheduler::Task>::subproblem_t*)malloc(sizeof(typename ParallelRecursiveNQueensTask<typename Scheduler::Task>::subproblem_t));

  problem->N        = N;
  problem->column   = 0;
  problem->solution = (int*)calloc(N, sizeof(int));
  problem->result   = &result;

  scheduler.template finish<ParallelRecursiveNQueensTask<typename Scheduler::Task> >(problem);
  return result;
}

template <class Scheduler>
void ParallelRecursiveNQueens<Scheduler>::print_results() {
  scheduler.print_performance_counter_values();
}

template <class Scheduler>
void ParallelRecursiveNQueens<Scheduler>::print_headers() {
  scheduler.print_performance_counter_headers();
}

template <class Scheduler>
void ParallelRecursiveNQueens<Scheduler>::print_scheduler_name() {
  Scheduler::print_name();
}

}

#endif /* PARALLELRECURSIVENQUEENS_H_ */
