/*
 * LifoStrategy2.h
 *
 *  Created on: Mar 29, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef LIFOSTRATEGY2_H_
#define LIFOSTRATEGY2_H_

#include <time.h>

namespace pheet {

template <class Pheet>
class LifoStrategy2 {
public:
	typedef LifoStrategy2<Pheet> Self;

	LifoStrategy2()
	:time(0/*clock()*/) {}

	LifoStrategy2(Self& other)
	:time(other.time) {}

	LifoStrategy2(Self&& other)
	:time(other.time) {}

	~LifoStrategy2() {}

	inline bool prioritize(Self& other) {
		// Should work with wraparound (clock_t is unsigned)
		return (other.time - time) > 0;
	}

	inline size_t get_transitive_weight() {
		return 32;
	}

	inline size_t get_memory_footprint() {
		return 32;
	}

private:
	clock_t time;
};

}

#endif /* LIFOSTRATEGY2_H_ */
