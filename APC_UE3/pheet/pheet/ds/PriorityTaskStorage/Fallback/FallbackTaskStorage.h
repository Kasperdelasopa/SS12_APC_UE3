/*
 * FallbackTaskStorage.h
 *
 *  Created on: 21.09.2011
 *      Author: Martin Wimmer
 */

#ifndef FALLBACKTASKSTORAGE_H_
#define FALLBACKTASKSTORAGE_H_

#include "../../../misc/type_traits.h"
#include "../../../misc/atomics.h"

#include <limits>
#include <iostream>

namespace pheet {

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
class FallbackTaskStorage {
public:
	typedef TT T;
	typedef typename BaseType::PerformanceCounters PerformanceCounters;
	typedef typename Pheet::Environment::StealerDescriptor StealerDescriptor;

	FallbackTaskStorage(size_t expected_capacity);
//	FallbackTaskStorage(size_t expected_capacity, PerformanceCounters& perf_count);
	~FallbackTaskStorage();

	template <class Strategy>
	void push(Strategy s, T item);
	template <class Strategy>
	void push(Strategy s, T item, PerformanceCounters& pc);
	T pop();
	T pop(PerformanceCounters& pc);
	T peek();
	T peek(PerformanceCounters& pc);
	T steal(StealerDescriptor& sd);
	T steal(StealerDescriptor& sd, PerformanceCounters& pc);

	T steal_push(FallbackTaskStorage<Pheet, TT, BaseType> &other, StealerDescriptor& sd);
	T steal_push(FallbackTaskStorage<Pheet, TT, BaseType> &other, StealerDescriptor& sd, PerformanceCounters& pc);

	size_t get_length();
	size_t get_length(PerformanceCounters& pc);
	bool is_empty();
	bool is_empty(PerformanceCounters& pc);
	bool is_full();
	bool is_full(PerformanceCounters& pc);

	// Can be called by the scheduler every time it is idle to perform some routine maintenance
	void perform_maintenance(PerformanceCounters& pc);

	static void print_name();

private:
	BaseType<Pheet, TT> data;

	PerformanceCounters perf_count;
};

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline FallbackTaskStorage<Pheet, TT, BaseType>::FallbackTaskStorage(size_t expected_capacity)
: data(expected_capacity) {

}
/*
template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline FallbackTaskStorage<Pheet, TT, BaseType>::FallbackTaskStorage(size_t expected_capacity, PerformanceCounters& perf_count)
: data(expected_capacity), perf_count(perf_count) {

}*/

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline FallbackTaskStorage<Pheet, TT, BaseType>::~FallbackTaskStorage() {

}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
template <class Strategy>
inline void FallbackTaskStorage<Pheet, TT, BaseType>::push(Strategy s, T item) {
	data.push(item);
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
template <class Strategy>
inline void FallbackTaskStorage<Pheet, TT, BaseType>::push(Strategy s, T item, PerformanceCounters& pc) {
	data.push(item);
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline TT FallbackTaskStorage<Pheet, TT, BaseType>::pop() {
	return data.pop();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline TT FallbackTaskStorage<Pheet, TT, BaseType>::pop(PerformanceCounters& pc) {
	return data.pop();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline TT FallbackTaskStorage<Pheet, TT, BaseType>::peek() {
	return data.peek();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline TT FallbackTaskStorage<Pheet, TT, BaseType>::peek(PerformanceCounters& pc) {
	return data.peek();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline TT FallbackTaskStorage<Pheet, TT, BaseType>::steal(StealerDescriptor& sd) {
	return data.steal();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline TT FallbackTaskStorage<Pheet, TT, BaseType>::steal(StealerDescriptor& sd, PerformanceCounters& pc) {
	return data.steal();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline TT FallbackTaskStorage<Pheet, TT, BaseType>::steal_push(FallbackTaskStorage<Pheet, TT, BaseType>& other, StealerDescriptor& sd) {
	return data.steal_push(other.data);
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline TT FallbackTaskStorage<Pheet, TT, BaseType>::steal_push(FallbackTaskStorage<Pheet, TT, BaseType>& other, StealerDescriptor& sd, PerformanceCounters& pc) {
	return data.steal_push(other.data);
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline size_t FallbackTaskStorage<Pheet, TT, BaseType>::get_length() {
	return data.get_length();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline size_t FallbackTaskStorage<Pheet, TT, BaseType>::get_length(PerformanceCounters& pc) {
	return data.get_length();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline bool FallbackTaskStorage<Pheet, TT, BaseType>::is_empty() {
	return data.is_empty();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline bool FallbackTaskStorage<Pheet, TT, BaseType>::is_empty(PerformanceCounters& pc) {
	return data.is_empty();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline bool FallbackTaskStorage<Pheet, TT, BaseType>::is_full() {
	return data.is_full();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline bool FallbackTaskStorage<Pheet, TT, BaseType>::is_full(PerformanceCounters& pc) {
	return data.is_full();
}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
inline void FallbackTaskStorage<Pheet, TT, BaseType>::perform_maintenance(PerformanceCounters& pc) {

}

template <class Pheet, typename TT, template <class EPheet, typename S> class BaseType>
void FallbackTaskStorage<Pheet, TT, BaseType>::print_name() {
	std::cout << "FallbackTaskStorage";
}

}

#endif /* FALLBACKTASKSTORAGE_H_ */
