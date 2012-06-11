/*
 * ImprovedImprovedBranchBoundGraphBipartitioningTask.h
 *
 *  Created on: Dec 2, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGTASK_H_
#define IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGTASK_H_

#include "ImprovedBranchBoundGraphBipartitioningSubproblem.h"
#include "ImprovedBranchBoundGraphBipartitioningPerformanceCounters.h"

namespace pheet {

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize = 64>
class ImprovedBranchBoundGraphBipartitioningTask : public Pheet::Task {
public:
	typedef ImprovedBranchBoundGraphBipartitioningTask<Pheet, Logic, MaxSize> Self;
	typedef GraphBipartitioningSolution<MaxSize> Solution;
	typedef MaxReducer<Pheet, Solution> SolutionReducer;
	typedef ImprovedBranchBoundGraphBipartitioningPerformanceCounters<Pheet> PerformanceCounters;
	typedef ImprovedBranchBoundGraphBipartitioningSubproblem<Pheet, Logic, MaxSize> SubProblem;

	ImprovedBranchBoundGraphBipartitioningTask(SubProblem* sub_problem, size_t* upper_bound, SolutionReducer& best, PerformanceCounters& pc);
	virtual ~ImprovedBranchBoundGraphBipartitioningTask();

	virtual void operator()();

private:
	SubProblem* sub_problem;
	size_t* upper_bound;
	SolutionReducer best;
	PerformanceCounters pc;
};

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize>
ImprovedBranchBoundGraphBipartitioningTask<Pheet, Logic, MaxSize>::ImprovedBranchBoundGraphBipartitioningTask(SubProblem* sub_problem, size_t* upper_bound, SolutionReducer& best, PerformanceCounters& pc)
: sub_problem(sub_problem), upper_bound(upper_bound), best(best), pc(pc) {

}

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize>
ImprovedBranchBoundGraphBipartitioningTask<Pheet, Logic, MaxSize>::~ImprovedBranchBoundGraphBipartitioningTask() {
	if(sub_problem != NULL) {
		delete sub_problem;
	}
}

template <class Pheet, template <class P, class SP> class Logic, size_t MaxSize>
void ImprovedBranchBoundGraphBipartitioningTask<Pheet, Logic, MaxSize>::operator()() {
	if(sub_problem->get_lower_bound() >= *upper_bound) {
		pc.num_irrelevant_tasks.incr();
		return;
	}

	SubProblem* sub_problem2 =
			sub_problem->split(pc.subproblem_pc);

	if(sub_problem->is_solution()) {
		sub_problem->update_solution(upper_bound, best, pc.subproblem_pc);
	}
	else if(sub_problem->get_lower_bound() < *upper_bound) {
		Pheet::template
			spawn<Self>(sub_problem, upper_bound, best, pc);
		// Make sure subproblem doesn't get deleted in destructor
		sub_problem = NULL;
	}

	if(sub_problem2->is_solution()) {
		sub_problem2->update_solution(upper_bound, best, pc.subproblem_pc);
		delete sub_problem2;
	}
	else if(sub_problem2->get_lower_bound() < *upper_bound) {
		Pheet::template
			call<Self>(sub_problem2, upper_bound, best, pc);
	}
	else {
		delete sub_problem2;
	}
}

}

#endif /* IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGTASK_H_ */
