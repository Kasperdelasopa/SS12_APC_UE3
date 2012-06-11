/*
 * LockFreeQueue.h
 *
 *  Created on: May 29, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0
 */

#ifndef LOCKFREEQUEUE_H_
#define LOCKFREEQUEUE_H_

#include <queue>
#include <iostream>
#include "../../../misc/type_traits.h"

namespace pheet {

template <class Pheet, typename TT>
class LockFreeQueue {
public:
	typedef typename Pheet::Mutex Mutex;
	typedef typename Pheet::LockGuard LockGuard;

	LockFreeQueue()
	: length(0){}
	~LockFreeQueue() {}

	void push(TT const& item) {
		LockGuard g(m);

		data.push(item);
		++length;
	}

	TT pop() {
		LockGuard g(m);

		pheet_assert(length == data.size());
		if(data.empty()) {
			return nullable_traits<TT>::null_value;
		}

		TT ret = data.front();

		--length;
		data.pop();
		return ret;
	}

	inline size_t get_length() {
		return length;
	}

	inline size_t size() {
		return get_length();
	}

	static void print_name() {
		std::cout << "LockFreeQueue<";
		Mutex::print_name();
		std::cout << ">";
	}

private:
	std::queue<TT> data;
	size_t length;
	Mutex m;
};

} /* namespace pheet */
#endif /* LockFreeQueue_H_ */
