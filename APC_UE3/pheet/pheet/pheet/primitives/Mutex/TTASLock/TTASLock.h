/*
 * TASLock.h
 *
 *  Created on: 10.08.2011
 *      Author: Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef TTASLOCK_H_
#define TTASLOCK_H_

#include <iostream>
#include "../common/BasicLockGuard.h"

namespace pheet {

template <class Pheet>
class TTASLock {
public:
	typedef TTASLock<Pheet> Self;
	typedef BasicLockGuard<Pheet, Self> LockGuard;

	TTASLock();
	~TTASLock();

	void lock();
	bool try_lock();
	bool try_lock(long int time_ms);

	void unlock();

	static void print_name() {
		std::cout << "TTASLock";
	}
private:
	// Volatile needed to ensure the compiler doesn't optimize the while(locked) loop
	int volatile locked;
};

template <class Pheet>
TTASLock<Pheet>::TTASLock()
:locked(0) {

}

template <class Pheet>
TTASLock<Pheet>::~TTASLock() {
}

template <class Pheet>
void TTASLock<Pheet>::lock() {
	do {
		while(locked);
	}
	while(!INT_CAS(&locked, 0, 1));
}

template <class Pheet>
bool TTASLock<Pheet>::try_lock() {
	return (!locked) && INT_CAS(&locked, 0, 1);
}

template <class Pheet>
bool TTASLock<Pheet>::try_lock(long int time_ms) {
	struct timeval begin;
	gettimeofday(&begin, NULL);
	long int begin_ms = (begin.tv_usec / 1000) + (begin.tv_sec * 1000);
	while(!INT_CAS(&locked, 0, 1)) {
		do {
			struct timeval current;
			gettimeofday(&current, NULL);
			long int current_ms = (current.tv_usec / 1000) + (current.tv_sec * 1000);
			if(current_ms - begin_ms >= time_ms) {
				return false;
			}
		}while(locked);
	};
	return true;
}

template <class Pheet>
void TTASLock<Pheet>::unlock() {
	MEMORY_FENCE();
	locked = 0;
}

}

#endif /* TASLOCK_H_ */
