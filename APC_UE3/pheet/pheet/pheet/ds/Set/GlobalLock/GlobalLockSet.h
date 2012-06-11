/*
 * GlobalLockSet.h
 *
 *  Created on: May 30, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0
 */

#ifndef GLOBALLOCKSET_H_
#define GLOBALLOCKSET_H_

#include <set>
#include <iostream>
#include "../../../misc/type_traits.h"
//#include "../../PriorityQueue/SetWrapper/PriorityQueueSetWrapper.h"

namespace pheet {

template <class Pheet, typename TT, class Compare = std::less<TT>>
class GlobalLockSet {
public:
	typedef typename Pheet::Mutex Mutex;
	typedef typename Pheet::LockGuard LockGuard;

	GlobalLockSet()
	: length(0){}
	~GlobalLockSet() {}

	void put(TT const& item) {
		LockGuard g(m);

		data.insert(item);

		length = data.size();
	}

	bool contains(TT const& item) {
		LockGuard g(m);

		auto iter = data.find(item);
		if(iter == data.end()) {
			return false;
		}

		return true;
	}

	bool remove(TT const& item) {
		LockGuard g(m);

		auto iter = data.find(item);
		if(iter == data.end()) {
			return false;
		}

		data.erase(iter);
		length = data.size();

		return true;
	}

	TT peek_min() {
		LockGuard g(m);

		pheet_assert(length == data.size());
		if(data.empty()) {
			return nullable_traits<TT>::null_value;
		}

		auto iter = data.begin();
		TT ret = *iter;

		return ret;
	}

	TT pop_min() {
		LockGuard g(m);

		pheet_assert(length == data.size());
		if(data.empty()) {
			return nullable_traits<TT>::null_value;
		}

		auto iter = data.begin();
		TT ret = *iter;
		data.erase(iter);
		--length;
		pheet_assert(length == data.size());

		return ret;
	}

	TT peek_max() {
		LockGuard g(m);

		pheet_assert(length == data.size());
		if(data.empty()) {
			return nullable_traits<TT>::null_value;
		}

		auto iter = data.rbegin();
		TT ret = *iter;

		return ret;
	}

	TT pop_max() {
		LockGuard g(m);

		pheet_assert(length == data.size());
		if(data.empty()) {
			return nullable_traits<TT>::null_value;
		}

		auto iter = data.rbegin();
		pheet_assert(iter != data.rend());
		TT ret = *iter;
		data.erase(ret);
		--length;
		pheet_assert(length == data.size());

		return ret;
	}

	inline size_t get_length() {
		return length;
	}

	inline size_t size() {
		return get_length();
	}

	static void print_name() {
		std::cout << "GlobalLockSet<";
		Mutex::print_name();
		std::cout << ">";
	}

private:
	std::set<TT, Compare> data;
	size_t length;
	Mutex m;
};

//template <class Pheet, typename TT, class Compare = std::less<TT>>
//using GlobalLockSetPriorityQueue = PriorityQueueSetWrapper<Pheet, TT, Compare, GlobalLockSet>;

} /* namespace pheet */
#endif /* GLOBALLOCKSET_H_ */
