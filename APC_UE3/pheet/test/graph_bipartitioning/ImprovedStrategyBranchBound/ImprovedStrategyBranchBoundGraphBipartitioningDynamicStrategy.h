/*
 * ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategy.h
 *
 *  Created on: Jan 13, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGDYNAMICSTRATEGY_H_
#define IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGDYNAMICSTRATEGY_H_

#include <pheet/pheet.h>
#include "ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy.h"

namespace pheet {

template <class Pheet, template <class P, class SP> class Strategy, class SubProblem>
class ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl {
public:
	template <template <class P, class SP> class NewStrat>
	using WithStrategy = ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl<Pheet, NewStrat, SubProblem>;

//	template <class Pheet, class SubProblem>
//	using T = ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl<Pheet, SubProblem>;

	ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl();
	~ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl();

	Strategy<Pheet, SubProblem> operator()(SubProblem* sub_problem, size_t* upper_bound);

	static void print_name();
};

template <class Pheet, template <class P, class SP> class Strategy, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl<Pheet, Strategy, SubProblem>::ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl() {

}

template <class Pheet, template <class P, class SP> class Strategy, class SubProblem>
inline ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl<Pheet, Strategy, SubProblem>::~ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl() {

}

template <class Pheet, template <class P, class SP> class Strategy, class SubProblem>
inline Strategy<Pheet, SubProblem> ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl<Pheet, Strategy, SubProblem>::operator()(SubProblem* sub_problem, size_t* upper_bound) {
	return Strategy<Pheet, SubProblem>(sub_problem, upper_bound);
}

template <class Pheet, template <class P, class SP> class Strategy, class SubProblem>
void ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl<Pheet, Strategy, SubProblem>::print_name() {
	std::cout << "DynamicStrategy<";
	Strategy<Pheet, SubProblem>::print_name();
	std::cout << ">";
}

template <class Pheet, class SubProblem>
using ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategy = ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategyImpl<Pheet, ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy, SubProblem>;

}

#endif /* IMPROVEDSTRATEGYBRANCHBOUNDGRAPHBIPARTITIONINGDYNAMICSTRATEGY_H_ */
