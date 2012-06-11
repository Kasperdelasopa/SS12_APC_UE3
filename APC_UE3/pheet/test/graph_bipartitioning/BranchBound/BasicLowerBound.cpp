/*
 * BasicLowerBound.cpp
 *
 *  Created on: 09.09.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#include "BasicLowerBound.h"

#include <set>

namespace pheet {

BasicLowerBound::BasicLowerBound() {

}

BasicLowerBound::~BasicLowerBound() {

}

size_t BasicLowerBound::operator()(GraphVertex* graph, size_t size, size_t k, size_t const* set1, size_t set1_size, size_t const* set2, size_t set2_size) {
	size_t lb = 0;
	for(size_t i = 0; i < set1_size; ++i) {
		size_t node = set1[i];
		size_t i2 = 0;
		for(size_t j = 0; j < graph[node].num_edges; ++j) {
			size_t target = graph[node].edges[j].target;
			if(target > node) {
				while(i2 < set2_size && set2[i2] < target) {
					++i2;
				}
				if(i2 == set2_size) {
					break;
				}
				if(set2[i2] == target) {
					lb += graph[node].edges[j].weight;
				}
			}
		}
	}
	for(size_t i = 0; i < set2_size; ++i) {
		size_t node = set2[i];
		size_t i2 = 0;
		for(size_t j = 0; j < graph[node].num_edges; ++j) {
			size_t target = graph[node].edges[j].target;
			if(target > node) {
				while(i2 < set1_size && set1[i2] < target) {
					++i2;
				}
				if(i2 == set1_size) {
					break;
				}
				if(set1[i2] == target) {
					lb += graph[node].edges[j].weight;
				}
			}
		}
	}

	return lb;
}

char const BasicLowerBound::name[] = "BasicLowerBound";

}
