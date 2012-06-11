/*
 * GlobalLockStack.h
 *
 *  Created on: May 29, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0
 */

#ifndef GLOBALLOCKSTACK_H_
#define GLOBALLOCKSTACK_H_

#include <vector>
#include <iostream>
#include "../../../misc/type_traits.h"

namespace pheet {

template <class Pheet, typename TT>
class GlobalLockStack {
public:
	typedef typename Pheet::Mutex Mutex;
	typedef typename Pheet::LockGuard LockGuard;

	GlobalLockStack()
	: length(0){}
	~GlobalLockStack() {}

	void push(TT const& item) {
		LockGuard g(m);

		data.push_back(item);
		++length;
	}

	TT pop() {
		LockGuard g(m);

		pheet_assert(length == data.size());
		if(data.empty()) {
			return nullable_traits<TT>::null_value;
		}

		TT ret = data.back();

		--length;
		data.pop_back();
		return ret;
	}

	inline size_t get_length() {
		return length;
	}

	inline size_t size() {
		return get_length();
	}

	static void print_name() {
		std::cout << "GlobalLockStack<";
		Mutex::print_name();
		std::cout << ">";
	}

private:
	std::vector<TT> data;
	size_t length;
	Mutex m;
};

} /* namespace pheet */
#endif /* GLOBALLOCKSTACK_H_ */
