/*
 * AutoStrategy.h
 *
 *  Created on: Oct 10, 2011
 *      Author: Martin Wimmer
 */

#ifndef AUTOSTRATEGY_H_
#define AUTOSTRATEGY_H_

#include "pheet/sched/strategies/LifoFifo/LifoFifoStrategy.h"

#include <iostream>

namespace pheet {

template <class Pheet, template <class P> class Strategy>
class BranchBoundGraphBipartitioningAutoStrategyImpl {
public:
	BranchBoundGraphBipartitioningAutoStrategyImpl();
	~BranchBoundGraphBipartitioningAutoStrategyImpl();

	Strategy<Pheet> operator()(GraphVertex* graph, size_t size, size_t k, size_t* set1, size_t set1_size, size_t* set2, size_t set2_size, size_t* ub, size_t lb);

	static void print_name();
};

template <class Pheet, template <class P> class Strategy>
inline BranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, Strategy>::BranchBoundGraphBipartitioningAutoStrategyImpl() {

}

template <class Pheet, template <class P> class Strategy>
inline BranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, Strategy>::~BranchBoundGraphBipartitioningAutoStrategyImpl() {

}

template <class Pheet, template <class P> class Strategy>
inline Strategy<Pheet> BranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, Strategy>::operator()(GraphVertex* graph, size_t size, size_t k, size_t* set1, size_t set1_size, size_t* set2, size_t set2_size, size_t* ub, size_t lb) {
	return Strategy<Pheet>();
}

template <class Pheet, template <class P> class Strategy>
void BranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, Strategy>::print_name() {
	std::cout << "AutoStrategy<";
	Strategy<Pheet>::print_name();
	std::cout << ">";
}

template <class Pheet>
using BranchBoundGraphBipartitioningAutoStrategy = BranchBoundGraphBipartitioningAutoStrategyImpl<Pheet, LifoFifoStrategy>;

}

#endif /* AUTOSTRATEGY_H_ */
