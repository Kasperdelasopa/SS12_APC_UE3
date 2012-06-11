/*
 * InARowGame.h
 *
 *  Created on: Nov 23, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef INAROWGAME_H_
#define INAROWGAME_H_

#include <pheet/pheet.h>
#include "InARowGameTask.h"

#include <iostream>

namespace pheet {

template <class Scheduler>
class InARowGame {
public:
	InARowGame(procs_t cpus, unsigned int width, unsigned int height, unsigned int rowlength, unsigned int lookahead, unsigned int* scenario);
	~InARowGame();

	void run();

	void print_results();
	void print_headers();

	static void print_name();
	static void print_scheduler_name();

	static procs_t const max_cpus;
	static char const name[];
private:
	unsigned int width;
	unsigned int height;
	unsigned int rowlength;
	unsigned int lookahead;
	unsigned int* scenario;
	typename Scheduler::CPUHierarchy cpu_hierarchy;
	Scheduler scheduler;
};

template <class Scheduler>
procs_t const InARowGame<Scheduler>::max_cpus = Scheduler::max_cpus;

template <class Scheduler>
char const InARowGame<Scheduler>::name[] = "RecursiveSearch";

template <class Scheduler>
InARowGame<Scheduler>::InARowGame(procs_t cpus, unsigned int width, unsigned int height, unsigned int rowlength, unsigned int lookahead, unsigned int* scenario)
: width(width), height(height), rowlength(rowlength), lookahead(lookahead), scenario(scenario), cpu_hierarchy(cpus), scheduler(&cpu_hierarchy) {

}

template <class Scheduler>
InARowGame<Scheduler>::~InARowGame() {

}

template <class Scheduler>
void InARowGame<Scheduler>::run() {
	scheduler.template finish<InARowGameTask<typename Scheduler::Task> >(width, height, rowlength, lookahead, scenario);
}

template <class Scheduler>
void InARowGame<Scheduler>::print_results() {
	scheduler.print_performance_counter_values();
}

template <class Scheduler>
void InARowGame<Scheduler>::print_headers() {
	scheduler.print_performance_counter_headers();
}

template <class Scheduler>
void InARowGame<Scheduler>::print_name() {
	std::cout << name;
}

template <class Scheduler>
void InARowGame<Scheduler>::print_scheduler_name() {
	Scheduler::print_name();
}
}

#endif /* INAROWGAME_H_ */
