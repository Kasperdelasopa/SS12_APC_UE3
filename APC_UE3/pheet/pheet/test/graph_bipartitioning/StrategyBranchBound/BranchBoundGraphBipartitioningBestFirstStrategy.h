/*
 * BranchBoundGraphBipartitioningBestFirstStrategy.h
 *
 *  Created on: Nov 15, 2011
 *      Author: Martin Wimmer
 */

#ifndef BRANCHBOUNDGRAPHBIPARTITIONINGBESTFIRSTSTRATEGY_H_
#define BRANCHBOUNDGRAPHBIPARTITIONINGBESTFIRSTSTRATEGY_H_

#include <pheet/pheet.h>
#include <pheet/sched/strategies/UserDefinedPriority/UserDefinedPriority.h>

namespace pheet {

template <class Pheet>
class BranchBoundGraphBipartitioningBestFirstStrategy {
public:
	BranchBoundGraphBipartitioningBestFirstStrategy();
	~BranchBoundGraphBipartitioningBestFirstStrategy();

	UserDefinedPriority<Pheet> operator()(GraphVertex* graph, size_t size, size_t k, size_t* set1, size_t set1_size, size_t* set2, size_t set2_size, size_t* ub, size_t lb);

	static void print_name();
};

template <class Pheet>
inline BranchBoundGraphBipartitioningBestFirstStrategy<Pheet>::BranchBoundGraphBipartitioningBestFirstStrategy() {

}

template <class Pheet>
inline BranchBoundGraphBipartitioningBestFirstStrategy<Pheet>::~BranchBoundGraphBipartitioningBestFirstStrategy() {

}

template <class Pheet>
inline UserDefinedPriority<Pheet> BranchBoundGraphBipartitioningBestFirstStrategy<Pheet>::operator()(GraphVertex* graph, size_t size, size_t k, size_t* set1, size_t set1_size, size_t* set2, size_t set2_size, size_t* ub, size_t lb) {
	size_t ubc = *ub;
	if(ubc < lb) {
		// Prioritize this task, as it will immediatly terminate anyway
		// Do not allow it to be stolen (not worth it)
		return UserDefinedPriority<Pheet>(std::numeric_limits< prio_t >::max(), 0);
	}

	// Depth: assumption: good for pop, bad for steal
	size_t depth = set1_size + set2_size;

	// Remaining elements in sets. Presumably lb is more realistic in balanced sets?
	// (ignored for now)
//	size_t set1_remaining = k - set1_size;
//	size_t set2_remaining = (size - k) - set2_size;

	// Difference to upper bound - larger is better
	// One maybe good, maybe bad decision: Always use the value relative to the current upper bound
	// Why: gives older elements a slight advantage. They will be rechecked and maybe dropped altogether
	// Also problematic if upper bound is near the limits of size_t as we may have an overflow (shouldn't be a problem in our case)
	size_t bound_diff = ubc - lb;

	// Make sure we don't overflow
	size_t limit = (std::numeric_limits< prio_t >::max() - 1) / size;
	if(bound_diff > limit) {
		// No upper bound available yet
		bound_diff = limit;
	}

	prio_t prio_pop = 1 + depth * bound_diff;
	prio_t prio_steal = 1 + (size - depth) * bound_diff;

	return UserDefinedPriority<Pheet>(prio_pop, prio_steal);
}

template <class Pheet>
inline void BranchBoundGraphBipartitioningBestFirstStrategy<Pheet>::print_name() {
	std::cout << "BestFirstStrategy";
}

}

#endif /* BRANCHBOUNDGRAPHBIPARTITIONINGBESTFIRSTSTRATEGY_H_ */
