/*
 * ImprovedStrategyBranchBoundGraphBipartitioningTask.h
 *
 *  Created on: Dec 5, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGTASK_H_
#define IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGTASK_H_

#include "../ImprovedBranchBound/ImprovedBranchBoundGraphBipartitioningSubproblem.h"
#include "ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters.h"

namespace pheet {

template <class Pheet, template <class P, class SP> class Logic, template <class P, class SubProblem> class SchedulingStrategy, size_t MaxSize = 64>
class ImprovedStrategyBranchBoundGraphBipartitioningTask : public Pheet::Task {
public:
	typedef ImprovedStrategyBranchBoundGraphBipartitioningTask<Pheet, Logic, SchedulingStrategy, MaxSize> Self;
	typedef GraphBipartitioningSolution<MaxSize> Solution;
	typedef MaxReducer<Pheet, Solution> SolutionReducer;
	typedef ImprovedStrategyBranchBoundGraphBipartitioningPerformanceCounters<Pheet> PerformanceCounters;
	typedef ImprovedBranchBoundGraphBipartitioningSubproblem<Pheet, Logic, MaxSize> SubProblem;

	ImprovedStrategyBranchBoundGraphBipartitioningTask(SubProblem* sub_problem, size_t* upper_bound, SolutionReducer& best, PerformanceCounters& pc);
	virtual ~ImprovedStrategyBranchBoundGraphBipartitioningTask();

	virtual void operator()();

private:
	SubProblem* sub_problem;
	size_t* upper_bound;
	SolutionReducer best;
	SchedulingStrategy<Pheet, SubProblem> strategy;
	PerformanceCounters pc;
};

template <class Pheet, template <class P, class SubProblem> class Logic, template <class P, class SubProblem> class SchedulingStrategy, size_t MaxSize>
ImprovedStrategyBranchBoundGraphBipartitioningTask<Pheet, Logic, SchedulingStrategy, MaxSize>::ImprovedStrategyBranchBoundGraphBipartitioningTask(SubProblem* sub_problem, size_t* upper_bound, SolutionReducer& best, PerformanceCounters& pc)
: sub_problem(sub_problem), upper_bound(upper_bound), best(best), pc(pc) {

}

template <class Pheet, template <class P, class SubProblem> class Logic, template <class P, class SubProblem> class SchedulingStrategy, size_t MaxSize>
ImprovedStrategyBranchBoundGraphBipartitioningTask<Pheet, Logic, SchedulingStrategy, MaxSize>::~ImprovedStrategyBranchBoundGraphBipartitioningTask() {
	if(sub_problem != NULL) {
		delete sub_problem;
	}
}

template <class Pheet, template <class P, class SubProblem> class Logic, template <class P, class SubProblem> class SchedulingStrategy, size_t MaxSize>
void ImprovedStrategyBranchBoundGraphBipartitioningTask<Pheet, Logic, SchedulingStrategy, MaxSize>::operator()() {
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
			spawn_prio<Self>(strategy(sub_problem, upper_bound),
				sub_problem, upper_bound, best, pc);
		// Make sure subproblem doesn't get deleted in destructor
		sub_problem = NULL;
	}

	if(sub_problem2->is_solution()) {
		sub_problem2->update_solution(upper_bound, best, pc.subproblem_pc);
		delete sub_problem2;
	}
	else if(sub_problem2->get_lower_bound() < *upper_bound) {
		Pheet::template
			spawn_prio<Self>(strategy(sub_problem2, upper_bound),
				sub_problem2, upper_bound, best, pc);
	}
	else {
		delete sub_problem2;
	}
}

}

#endif /* IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGTASK_H_ */
