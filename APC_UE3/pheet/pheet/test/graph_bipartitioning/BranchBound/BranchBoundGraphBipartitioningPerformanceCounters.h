/*
 * BranchBoundGraphBipartitioningPerformanceCounters.h
 *
 *  Created on: Feb 15, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef BRANCHBOUNDGRAPHBIPARTITIONINGPERFORMANCECOUNTERS_H_
#define BRANCHBOUNDGRAPHBIPARTITIONINGPERFORMANCECOUNTERS_H_

namespace pheet {

template <class Pheet>
class BranchBoundGraphBipartitioningPerformanceCounters {
public:
	typedef BranchBoundGraphBipartitioningPerformanceCounters<Pheet> Self;

	BranchBoundGraphBipartitioningPerformanceCounters() {}
	BranchBoundGraphBipartitioningPerformanceCounters(Self& other) {}
	~BranchBoundGraphBipartitioningPerformanceCounters() {}

	static void print_headers() {}
	void print_values() {}
};

}

#endif /* BRANCHBOUNDGRAPHBIPARTITIONINGPERFORMANCECOUNTERS_H_ */
