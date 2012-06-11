/*
 * ImprovedBranchBoundGraphBipartitioningDeltaLogic.h
 *
 *  Created on: Dec 21, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGDELTALOGIC_H_
#define IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGDELTALOGIC_H_

#include "pheet/pheet.h"
#include "../graph_helpers.h"

#include <string.h>
#include <algorithm>

namespace pheet {

template <class Pheet, class SubProblem>
class ImprovedBranchBoundGraphBipartitioningDeltaLogic {
public:
	typedef ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<Pheet, SubProblem> Self;
	typedef typename SubProblem::Set Set;

	ImprovedBranchBoundGraphBipartitioningDeltaLogic(Subproblem* sub_problem);
	ImprovedBranchBoundGraphBipartitioningDeltaLogic(Subproblem* sub_problem, Self const& other);
	~ImprovedBranchBoundGraphBipartitioningDeltaLogic();

	void init_root();
	size_t get_next_vertex();
	size_t get_cut();
	size_t get_lower_bound();
	void update(uint8_t set, size_t pos);
	void bulk_update(uint8_t set, Set positions);

	static void print_name();
private:
	Subproblem* sub_problem;

	size_t cut;
	size_t lb;

	size_t* weights[2];
};

template <class Pheet, class SubProblem>
ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::ImprovedBranchBoundGraphBipartitioningDeltaLogic(Subproblem* sub_problem)
: sub_problem(sub_problem), cut(0), lb(0) {
	weights[0] = new size_t[sub_problem->size];
	weights[1] = new size_t[sub_problem->size];
	memset(weights[0], 0, sizeof(size_t)*sub_problem->size);
	memset(weights[1], 0, sizeof(size_t)*sub_problem->size);
}

template <class Pheet, class SubProblem>
ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::ImprovedBranchBoundGraphBipartitioningDeltaLogic(Subproblem* sub_problem, Self const& other)
: sub_problem(sub_problem), cut(other.cut), lb(other.lb) {
	weights[0] = new size_t[sub_problem->size];
	weights[1] = new size_t[sub_problem->size];
	memcpy(weights[0], other.weights[0], sizeof(size_t)*sub_problem->size);
	memcpy(weights[1], other.weights[1], sizeof(size_t)*sub_problem->size);
}

template <class Pheet, class SubProblem>
ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::~ImprovedBranchBoundGraphBipartitioningDeltaLogic() {
	delete[] weights[0];
	delete[] weights[1];
}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::init_root() {

}

template <class Pheet, class SubProblem>
size_t ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::get_next_vertex() {
	return sub_problem->sets[2]._Find_first();
}

template <class Pheet, class SubProblem>
size_t ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::get_cut() {
	return cut;
}

template <class Pheet, class SubProblem>
size_t ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::get_lower_bound() {
	return get_cut() + lb;
}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::update(uint8_t set, size_t pos) {
	cut += weights[set ^ 1][pos];

	for(size_t i = 0; i < sub_problem->graph[pos].num_edges; ++i) {
		weights[set][sub_problem->graph[pos].edges[i].target] += sub_problem->graph[pos].edges[i].weight;
	}

	ptrdiff_t* delta = new ptrdiff_t[sub_problem->sets[2].count()];
	size_t di = 0;
	lb = 0;
	size_t current_bit = sub_problem->sets[2]._Find_first();
	while(current_bit != sub_problem->sets[2].size()) {
		delta[di++] = (ptrdiff_t)weights[1][current_bit] - (ptrdiff_t)weights[0][current_bit];
		lb += std::min(weights[0][current_bit], weights[1][current_bit]);
		current_bit = sub_problem->sets[2]._Find_next(current_bit);
	}
	std::sort(delta, delta + di);

	pheet_assert(di == sub_problem->sets[2].count());
	size_t pivot = (/*sub_problem->size - */sub_problem->k) - sub_problem->sets[0].count();
	if(pivot < di && delta[pivot] < 0) {
		do {
			lb += -delta[pivot];
			++pivot;
		} while(pivot < di && delta[pivot] < 0);
	}
	else if(pivot > 0 && delta[pivot - 1] > 0) {
		do {
			--pivot;
			lb += delta[pivot];
		} while(pivot > 0 && delta[pivot - 1] > 0);
	}

	delete[] delta;
}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::bulk_update(uint8_t set, Set positions) {
	size_t current_bit = positions._Find_first();
	while(current_bit != positions.size()) {
		update(set, current_bit);
		current_bit = positions._Find_next(current_bit);
	}
}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningDeltaLogic<Pheet, SubProblem>::print_name() {
	std::cout << "DeltaLogic";
}

}

#endif /* IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGDELTALOGIC_H_ */
