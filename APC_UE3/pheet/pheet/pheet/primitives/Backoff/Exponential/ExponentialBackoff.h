/*
 * ExponentialBackoff.h
 *
 *  Created on: 18.04.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef EXPONENTIALBACKOFF_H_
#define EXPONENTIALBACKOFF_H_

#include <stdlib.h>
#include <time.h>
#include <algorithm>

/*
 *
 */
namespace pheet {

template <class Pheet, unsigned int MIN_BACKOFF = 100, unsigned int MAX_BACKOFF = 100000>
class ExponentialBackoffImpl {
public:
	ExponentialBackoffImpl();
	~ExponentialBackoffImpl();

	void backoff();
private:
	unsigned int limit;

//	static thread_local boost::mt19937 rng;
};
/*
template <unsigned int MIN_BACKOFF, unsigned int MAX_BACKOFF>
thread_local boost::mt19937 ExponentialBackoffImpl<MIN_BACKOFF, MAX_BACKOFF>::rng;
*/
template <class Pheet, unsigned int MIN_BACKOFF, unsigned int MAX_BACKOFF>
ExponentialBackoffImpl<Pheet, MIN_BACKOFF, MAX_BACKOFF>::ExponentialBackoffImpl() {
	limit = MIN_BACKOFF;
}

template <class Pheet, unsigned int MIN_BACKOFF, unsigned int MAX_BACKOFF>
ExponentialBackoffImpl<Pheet, MIN_BACKOFF, MAX_BACKOFF>::~ExponentialBackoffImpl() {
}

template <class Pheet, unsigned int MIN_BACKOFF, unsigned int MAX_BACKOFF>
void ExponentialBackoffImpl<Pheet, MIN_BACKOFF, MAX_BACKOFF>::backoff() {
//	boost::uniform_int<unsigned int> rnd_gen(0, limit);
	Pheet p;
	unsigned int sleep = p.rand_int(limit); //rnd_gen(rng);

	timespec delay;
	delay.tv_sec = (time_t)0;
	delay.tv_nsec = sleep;
	// let's sleep
	(void)nanosleep(&delay, (timespec *)NULL);

	limit = std::min(limit + sleep, MAX_BACKOFF);
}

template <class Pheet>
using ExponentialBackoff = ExponentialBackoffImpl<Pheet, 100, 100000>;

}

#endif /* EXPONENTIALBACKOFF_H_ */
