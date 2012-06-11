/*
 * BasicNextVertex.h
 *
 *  Created on: 09.09.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef BASICNEXTVERTEX_H_
#define BASICNEXTVERTEX_H_

#include "../graph_helpers.h"

/*
 *
 */
namespace pheet {

class BasicNextVertex {
public:
	BasicNextVertex();
	~BasicNextVertex();

	size_t operator()(GraphVertex* graph, size_t size, size_t k, size_t const* set1, size_t set1_size, size_t const* set2, size_t set2_size);

	static char const name[];
};

}

#endif /* BASICNEXTVERTEX_H_ */
