/*
 * ImprovedBranchBoundGraphBipartitioningBasicLogic.h
 *
 *  Created on: Dec 2, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGBASICLOGIC_H_
#define IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGBASICLOGIC_H_

#include "pheet/pheet.h"
#include "../graph_helpers.h"

namespace pheet {

template <class Pheet, class SubProblem>
class ImprovedBranchBoundGraphBipartitioningBasicLogic {
public:
	typedef ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<Pheet, SubProblem> Self;
	typedef typename SubProblem::Set Set;

	ImprovedBranchBoundGraphBipartitioningBasicLogic(Subproblem* sub_problem);
	ImprovedBranchBoundGraphBipartitioningBasicLogic(Subproblem* sub_problem, Self const& other);
	~ImprovedBranchBoundGraphBipartitioningBasicLogic();

	void init_root();
	size_t get_next_vertex();
	size_t get_cut();
	size_t get_lower_bound();
	void update(uint8_t set, size_t pos);
	void bulk_update(uint8_t set, Set positions);

	static void print_name();
private:
	Subproblem* sub_problem;
};

template <class Pheet, class SubProblem>
ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::ImprovedBranchBoundGraphBipartitioningBasicLogic(Subproblem* sub_problem)
: sub_problem(sub_problem) {

}

template <class Pheet, class SubProblem>
ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::ImprovedBranchBoundGraphBipartitioningBasicLogic(Subproblem* sub_problem, Self const& other)
: sub_problem(sub_problem) {

}

template <class Pheet, class SubProblem>
ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::~ImprovedBranchBoundGraphBipartitioningBasicLogic() {

}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::init_root() {

}

template <class Pheet, class SubProblem>
size_t ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::get_next_vertex() {
	return sub_problem->sets[2]._Find_first();
}

template <class Pheet, class SubProblem>
size_t ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::get_cut() {
	size_t cut = 0;
	size_t current_bit = sub_problem->sets[0]._Find_first();
	while(current_bit != sub_problem->sets[0].size()) {
		for(size_t i = 0; i < sub_problem->graph[current_bit].num_edges; ++i) {
			if(sub_problem->sets[1].test(sub_problem->graph[current_bit].edges[i].target)) {
				cut += sub_problem->graph[current_bit].edges[i].weight;
			}
		}
		current_bit = sub_problem->sets[0]._Find_next(current_bit);
	}
	return cut;
}

template <class Pheet, class SubProblem>
size_t ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::get_lower_bound() {
	return get_cut();
}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::update(uint8_t set, size_t pos) {

}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::bulk_update(uint8_t set, Set positions) {
	size_t current_bit = positions._Find_first();
	while(current_bit != positions.size()) {
		update(set, current_bit);
		current_bit = positions._Find_next(current_bit);
	}
}

template <class Pheet, class SubProblem>
void ImprovedBranchBoundGraphBipartitioningBasicLogic<Pheet, SubProblem>::print_name() {
	std::cout << "BasicLogic";
}

}

#endif /* IMPROVEDBRANCHBOUNDGRAPHBIPARTITIONINGBASICLOGIC_H_ */
