/*
 * ImprovedBranchBoundGraphBipartitioningDeltaNVLogic.h
 *
 *  Created on: Dec 21, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGDELTANVLOGIC_H_
#define IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGDELTANVLOGIC_H_

#include "pheet/pheet.h"
#include "../graph_helpers.h"

#include <string.h>
#include <algorithm>

namespace pheet {

template <class Pheet, class SubProblem>
class ImprovedBranchBoundGraphBipartitioningDeltaNVLogic {
public:
	typedef ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<Pheet, SubProblem> Self;
	typedef typename SubProblem::Set Set;

	ImprovedBranchBoundGraphBipartitioningDeltaNVLogic(Subproblem* sub_problem);
	ImprovedBranchBoundGraphBipartitioningDeltaNVLogic(Subproblem* sub_problem, Self const& other);
	~ImprovedBranchBoundGraphBipartitioningDeltaNVLogic();

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
	size_t nv;

	size_t* weights[2];
};

template <class Pheet, class SubProblem>
ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::ImprovedBranchBoundGraphBipartitioningDeltaNVLogic(Subproblem* sub_problem)
: sub_problem(sub_problem), cut(0), lb(0), nv(0) {
	weights[0] = new size_t[sub_problem->size];
	weights[1] = new size_t[sub_problem->size];
	memset(weights[0], 0, sizeof(size_t)*sub_problem->size);
	memset(weights[1], 0, sizeof(size_t)*sub_problem->size);
}

template <class Pheet, class SubProblem>
ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::ImprovedBranchBoundGraphBipartitioningDeltaNVLogic(Subproblem* sub_problem, Self const& other)
: sub_problem(sub_problem), cut(other.cut), lb(other.lb), nv(other.nv) {
	weights[0] = new size_t[sub_problem->size];
	weights[1] = new size_t[sub_problem->size];
	memcpy(weights[0], other.weights[0], sizeof(size_t)*sub_problem->size);
	memcpy(weights[1], other.weights[1], sizeof(size_t)*sub_problem->size);
}

template <class Pheet, class SubProblem>
ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::~ImprovedBranchBoundGraphBipartitioningDeltaNVLogic() {
	delete[] weights[0];
	delete[] weights[1];
}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::init_root() {

}

template <class Pheet, class SubProblem>
size_t ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::get_next_vertex() {
	pheet_assert(sub_problem->sets[2].test(nv));
	return nv;
}

template <class Pheet, class SubProblem>
size_t ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::get_cut() {
	return cut;
}

template <class Pheet, class SubProblem>
size_t ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::get_lower_bound() {
	return get_cut() + lb;
}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::update(uint8_t set, size_t pos) {
	cut += weights[set ^ 1][pos];

	for(size_t i = 0; i < sub_problem->graph[pos].num_edges; ++i) {
		weights[set][sub_problem->graph[pos].edges[i].target] += sub_problem->graph[pos].edges[i].weight;
	}

	ptrdiff_t* delta = new ptrdiff_t[sub_problem->sets[2].count()];

	size_t di = 0;
	lb = 0;
	size_t current_bit = sub_problem->sets[2]._Find_first();
	nv = current_bit;
	ptrdiff_t max_delta = 0;
	while(current_bit != sub_problem->sets[2].size()) {
		ptrdiff_t d = (ptrdiff_t)weights[1][current_bit] - (ptrdiff_t)weights[0][current_bit];
		if(std::abs(d) >= max_delta) {
			max_delta = std::abs(d);
			nv = current_bit;
		}
		delta[di++] = d;
		lb += std::min(weights[0][current_bit], weights[1][current_bit]);
		current_bit = sub_problem->sets[2]._Find_next(current_bit);
	}
	std::sort(delta, delta + di);

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
void ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::bulk_update(uint8_t set, Set positions) {
	size_t current_bit = positions._Find_first();
	while(current_bit != positions.size()) {
		update(set, current_bit);
		current_bit = positions._Find_next(current_bit);
	}
}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<Pheet, SubProblem>::print_name() {
	std::cout << "DeltaNVLogic";
}

}

#endif /* IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGDELTANVLOGIC_H_ */
