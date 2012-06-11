/*
 * BasicNextVertex.cpp
 *
 *  Created on: 09.09.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#include "BasicNextVertex.h"

namespace pheet {

BasicNextVertex::BasicNextVertex() {

}

BasicNextVertex::~BasicNextVertex() {

}

size_t BasicNextVertex::operator()(GraphVertex* graph, size_t size, size_t k, size_t const* set1, size_t set1_size, size_t const* set2, size_t set2_size) {
	return set1_size + set2_size;
}

char const BasicNextVertex::name[] = "BasicNextVertex";

}
