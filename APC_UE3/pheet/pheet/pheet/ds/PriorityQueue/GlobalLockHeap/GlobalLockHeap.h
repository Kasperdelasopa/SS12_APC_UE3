/*
 * GlobalLockHeap.h
 *
 *  Created on: Nov 30, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef GLOBALLOCKHEAP_H_
#define GLOBALLOCKHEAP_H_

#include "../../../settings.h"

#include <string.h>
#include <iostream>

namespace pheet {

/*
 * MaxGlobalLockHeap
 */
template <class Pheet, typename TT, class Comparator = std::less<TT> >
class GlobalLockHeap {
public:
	typedef typename Pheet::Mutex Mutex;
	typedef typename Pheet::LockGuard LockGuard;
	typedef TT T;

	GlobalLockHeap();
	GlobalLockHeap(Comparator const& comp);
	~GlobalLockHeap();

	void push(T item);
	T peek();
	T pop();

	size_t get_length();
	bool is_empty();

	static void print_name();

private:
	void bubble_up(size_t index);
	void bubble_down(size_t index);

	size_t capacity;
	size_t length;
	TT* data;
	Comparator is_less;
	Mutex m;
};

template <class Pheet, typename TT, class Comparator>
GlobalLockHeap<Pheet, TT, Comparator>::GlobalLockHeap()
: capacity(4), length(0), data(new TT[capacity]) {

}

template <class Pheet, typename TT, class Comparator>
GlobalLockHeap<Pheet, TT, Comparator>::GlobalLockHeap(Comparator const& comp)
: capacity(4), length(0), data(new TT[capacity]), is_less(comp) {

}

template <class Pheet, typename TT, class Comparator>
GlobalLockHeap<Pheet, TT, Comparator>::~GlobalLockHeap() {
	delete[] data;
}

template <class Pheet, typename TT, class Comparator>
void GlobalLockHeap<Pheet, TT, Comparator>::push(T item) {
	LockGuard g(m);

	if(length == capacity) {
		size_t new_capacity = capacity << 1;
		T* new_data = new T[new_capacity];
		memcpy(new_data, data, sizeof(T) * capacity);
		delete[] data;
		data = new_data;
		capacity = new_capacity;
	}

	data[length] = item;
	bubble_up(length);
	++length;
}

template <class Pheet, typename TT, class Comparator>
TT GlobalLockHeap<Pheet, TT, Comparator>::peek() {
	LockGuard g(m);

	if(length == 0) {
		return nullable_traits<TT>::null_value;
	}

	pheet_assert(length > 0);
	TT ret = data[0];
	return ret;
}


template <class Pheet, typename TT, class Comparator>
TT GlobalLockHeap<Pheet, TT, Comparator>::pop() {
	LockGuard g(m);

	if(length == 0) {
		return nullable_traits<TT>::null_value;
	}
	T ret = data[0];
	--length;
	data[0] = data[length];
	bubble_down(0);
	return ret;
}

template <class Pheet, typename TT, class Comparator>
size_t GlobalLockHeap<Pheet, TT, Comparator>::get_length() {
	return length;
}

template <class Pheet, typename TT, class Comparator>
bool GlobalLockHeap<Pheet, TT, Comparator>::is_empty() {
	return length == 0;
}

template <class Pheet, typename TT, class Comparator>
void GlobalLockHeap<Pheet, TT, Comparator>::bubble_up(size_t index) {
	size_t next = (index - 1) >> 1;
	while(index > 0 && is_less(data[next], data[index])) {
		std::swap(data[next], data[index]);
		index = next;
		next = (index - 1) >> 1;
	}
}

template <class Pheet, typename TT, class Comparator>
void GlobalLockHeap<Pheet, TT, Comparator>::bubble_down(size_t index) {
	size_t next = (index << 1) + 1;
	while(next < length) {
		size_t nnext = next + 1;
		if(nnext < length && is_less(data[next], data[nnext])) {
			next = nnext;
		}
		if(!is_less(data[index], data[next])) {
			break;
		}
		std::swap(data[index], data[next]);
		index = next;
		next = (index << 1) + 1;
	}
}

template <class Pheet, typename TT, class Comparator>
void GlobalLockHeap<Pheet, TT, Comparator>::print_name() {
	std::cout << "GlobalLockHeap<";
	Mutex::print_name();
	std::cout << ">";
}

}

#endif /* GLOBALLOCKHEAP_H_ */
