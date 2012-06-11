/*
* SORRun.h
*
*  Created on: 22.09.2011
*      Author: Daniel Cederman
*     License: Boost Software License 1.0 (BSL1.0)
*/

#ifndef SORRUN_H_
#define SORRUN_H_

#include <pheet/pheet.h>
#include "SORTasks.h"
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <iostream>
#include <fstream>

namespace pheet {

template <class Scheduler>
class SORRun {
public:
  SORRun(procs_t cpus, int M, int N, int slices, double omega, int iterations, bool prio);
	~SORRun();

	void run();

	void print_results();
	void print_headers();

	static void print_name();
	static void print_scheduler_name();

	static procs_t const max_cpus;
	static char const name[];

	double getTotal();

private:
	typename Scheduler::CPUHierarchy cpu_hierarchy;
	Scheduler scheduler;
	int iterations;
	SORParams sp;
	double* backend;
};

template <class Scheduler>
procs_t const SORRun<Scheduler>::max_cpus = Scheduler::max_cpus;

template <class Scheduler>
char const SORRun<Scheduler>::name[] = "PartitionMatrix";

template <class Scheduler>
double SORRun<Scheduler>::getTotal()
{
	return sp.total;
}

template <class Scheduler>
  SORRun<Scheduler>::SORRun(procs_t cpus, int M, int N, int slices, double omega, int iterations, bool prio):
cpu_hierarchy(cpus), scheduler(&cpu_hierarchy), iterations(iterations) {
	
	sp.M=M;
	sp.N=N;
	sp.slices=slices;
	sp.omega=omega;
	  sp.prio = prio;

	typedef boost::mt19937 base_generator_type;
	base_generator_type generator(42);
	boost::uniform_real<> uni_dist(0,1);
	boost::variate_generator<base_generator_type&, boost::uniform_real<> > uni(generator, uni_dist);

	sp.G = new double*[M];
	backend = new double[M*N];
	for(int i=0;i<M;i++)
		sp.G[i]=&backend[N*i];

	for (int i=0; i<M; i++)
		for (int j=0; j<N; j++)
		{
			sp.G[i][j] = uni() * 1e-6;
		}      

/*	
	//Compare with reference random values
	ifstream file ("bin/ref/randvals.bin", ios::in|ios::binary);
	file.read((char*)backend,N*M*sizeof(double));
	file.close();
	std::cout << sp.G[621][154] << endl;
*/

}

template <class Scheduler>
SORRun<Scheduler>::~SORRun() {
	delete[] sp.G;
	delete[] backend;

}

template <class Scheduler>
void SORRun<Scheduler>::run() {
	// Start here
	scheduler.template finish<SORStartTask<typename Scheduler::Task> >(sp, iterations);
}

template <class Scheduler>
void SORRun<Scheduler>::print_results() {
	scheduler.print_performance_counter_values();
}

template <class Scheduler>
void SORRun<Scheduler>::print_headers() {
	scheduler.print_performance_counter_headers();
}

template <class Scheduler>
void SORRun<Scheduler>::print_name() {
	std::cout << name;
}

template <class Scheduler>
void SORRun<Scheduler>::print_scheduler_name() {
	Scheduler::print_name();
}
}

#endif /* SORRUN_H_ */
