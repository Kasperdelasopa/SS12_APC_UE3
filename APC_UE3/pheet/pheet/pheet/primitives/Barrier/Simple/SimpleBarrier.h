/*
 * SimpleBarrier.h
 *
 *  Created on: 18.04.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef SIMPLEBARRIER_H_
#define SIMPLEBARRIER_H_

#include "../../../misc/types.h"
#include "../../../misc/atomics.h"

/*
 *
 */
namespace pheet {

template <class Pheet>
class SimpleBarrier {
public:
	SimpleBarrier();
	~SimpleBarrier();

	void wait(size_t i, procs_t p);
	void signal(size_t i);
	void barrier(size_t i, procs_t p);
	void reset();

private:
	typedef typename Pheet::Backoff Backoff;

	procs_t barriers[4];
};

template <class Pheet>
SimpleBarrier<Pheet>::SimpleBarrier() {
	reset();
}

template <class Pheet>
SimpleBarrier<Pheet>::~SimpleBarrier() {

}

template <class Pheet>
void SimpleBarrier<Pheet>::signal(size_t i) {
	INT_ATOMIC_ADD(&(barriers[i&3]), 1);
}

template <class Pheet>
void SimpleBarrier<Pheet>::wait(size_t i, procs_t p) {
	Backoff bo;
	while(barriers[i&3] != p) {
		bo.backoff();
	}
	barriers[(i+2) & 3] = 0;
}

template <class Pheet>
void SimpleBarrier<Pheet>::barrier(size_t i, procs_t p) {
	signal(i);
	wait(i, p);
}

template <class Pheet>
void SimpleBarrier<Pheet>::reset() {
	barriers[0] = 0;
	barriers[1] = 0;
	barriers[2] = 0;
	barriers[3] = 0;
}

}

#endif /* SIMPLEBARRIER_H_ */
