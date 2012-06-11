/*
 * BasicLowerBound.h
 *
 *  Created on: 09.09.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef BASICLOWERBOUND_H_
#define BASICLOWERBOUND_H_

#include "../graph_helpers.h"

/*
 *
 */
namespace pheet {

class BasicLowerBound {
public:
	BasicLowerBound();
	~BasicLowerBound();

	size_t operator()(GraphVertex* graph, size_t size, size_t k, size_t const* set1, size_t set1_size, size_t const* set2, size_t set2_size);

	static char const name[];
};

}

#endif /* BASICLOWERBOUND_H_ */
