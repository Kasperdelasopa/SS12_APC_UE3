/*
* UTSRun.h
*
*  Created on: 22.09.2011
*      Author: Daniel Cederman
*     License: Boost Software License 1.0 (BSL1.0)
*/

#ifndef UTSRUN_H_
#define UTSRUN_H_

#include <pheet/pheet.h>
#include "UTSTasks.h"

#include <iostream>

namespace pheet {

template <class Scheduler>
class UTSRun {
public:
	UTSRun(procs_t cpus);
	~UTSRun();

	void run();

	void print_results();
	void print_headers();

	static void print_name();
	static void print_scheduler_name();

	static procs_t const max_cpus;
	static char const name[];
private:

	typename Scheduler::CPUHierarchy cpu_hierarchy;
	Scheduler scheduler;
};

template <class Scheduler>
procs_t const UTSRun<Scheduler>::max_cpus = Scheduler::max_cpus;

template <class Scheduler>
char const UTSRun<Scheduler>::name[] = "RecursiveSearch";

template <class Scheduler>
UTSRun<Scheduler>::UTSRun(procs_t cpus): cpu_hierarchy(cpus), scheduler(&cpu_hierarchy) {

}

template <class Scheduler>
UTSRun<Scheduler>::~UTSRun() {

}

template <class Scheduler>
void UTSRun<Scheduler>::run() {
	
	Node root;
	uts_initRoot(&root, type);
	scheduler.template finish<UTSStartTask<typename Scheduler::Task> >(root);
}

template <class Scheduler>
void UTSRun<Scheduler>::print_results() {
	scheduler.print_performance_counter_values();
}

template <class Scheduler>
void UTSRun<Scheduler>::print_headers() {
	scheduler.print_performance_counter_headers();
}

template <class Scheduler>
void UTSRun<Scheduler>::print_name() {
	std::cout << name;
}

template <class Scheduler>
void UTSRun<Scheduler>::print_scheduler_name() {
	Scheduler::print_name();
}
}

#endif /* INAROWGAME_H_ */
