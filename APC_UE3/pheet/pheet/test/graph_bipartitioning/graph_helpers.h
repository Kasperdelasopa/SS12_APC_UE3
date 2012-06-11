/*
 * graph_helpers.h
 *
 *  Created on: 07.09.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef GRAPH_HELPERS_H_
#define GRAPH_HELPERS_H_

#include "../init.h"
#include "pheet/misc/types.h"
#include "pheet/primitives/Reducer/Max/MaxReducer.h"

#include <bitset>

namespace pheet {

struct GraphEdge {
	size_t target;
	size_t weight;
};

struct GraphVertex {
	GraphEdge* edges;
	size_t num_edges;
};

template <size_t MAX_SIZE = 64>
struct GraphBipartitioningSolution {
	GraphBipartitioningSolution();

	GraphBipartitioningSolution<MAX_SIZE>& operator=(GraphBipartitioningSolution<MAX_SIZE> const& other);
	GraphBipartitioningSolution<MAX_SIZE>& operator=(GraphBipartitioningSolution<MAX_SIZE>& other);

	size_t weight;
	std::bitset<MAX_SIZE> sets[2];
};

template <size_t MAX_SIZE>
GraphBipartitioningSolution<MAX_SIZE>::GraphBipartitioningSolution() {
	weight = 0;
}

template <size_t MAX_SIZE>
GraphBipartitioningSolution<MAX_SIZE>& GraphBipartitioningSolution<MAX_SIZE>::operator=(GraphBipartitioningSolution<MAX_SIZE> const& other) {
	weight = other.weight;
	sets[0] = other.sets[0];
	sets[1] = other.sets[1];
	return *this;
}

template <size_t MAX_SIZE>
GraphBipartitioningSolution<MAX_SIZE>& GraphBipartitioningSolution<MAX_SIZE>::operator=(GraphBipartitioningSolution<MAX_SIZE>& other) {
	weight = other.weight;
	sets[0] = other.sets[0];
	sets[1] = other.sets[1];
	return *this;
}

template <size_t MAX_SIZE>
struct MaxOperation<GraphBipartitioningSolution<MAX_SIZE> > {
	GraphBipartitioningSolution<MAX_SIZE>& operator()(GraphBipartitioningSolution<MAX_SIZE>& x, GraphBipartitioningSolution<MAX_SIZE>& y);

	GraphBipartitioningSolution<MAX_SIZE> get_identity();
};

template <size_t MAX_SIZE>
GraphBipartitioningSolution<MAX_SIZE>& MaxOperation<GraphBipartitioningSolution<MAX_SIZE> >::operator()(GraphBipartitioningSolution<MAX_SIZE>& x, GraphBipartitioningSolution<MAX_SIZE>& y) {
	if(x.weight <= y.weight)
		return x;
	return y;
}

template <size_t MAX_SIZE>
GraphBipartitioningSolution<MAX_SIZE> MaxOperation<GraphBipartitioningSolution<MAX_SIZE> >::get_identity() {
	GraphBipartitioningSolution<MAX_SIZE> ret;
	ret.weight = std::numeric_limits<size_t>::max();
	return ret;
}

}

#endif /* GRAPH_HELPERS_H_ */
