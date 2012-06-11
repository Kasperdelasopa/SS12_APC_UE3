/*
 * ArrayListStrategyTaskStorage.h
 *
 *  Created on: Mar 29, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef ARRAYLISTSTRATEGYTASKSTORAGE_H_
#define ARRAYLISTSTRATEGYTASKSTORAGE_H_


#include "../../../../../settings.h"
#include "../../../../../sched/strategies/BaseStrategy.h"
#include "../../../../../primitives/Backoff/Exponential/ExponentialBackoff.h"

#include "ArrayListStrategyTaskStoragePerformanceCounters.h"
#include "ArrayListStrategyTaskStorageIterator.h"
#include "ArrayListStrategyTaskStorageControlBlock.h"

#include "../../../../StrategyHeap/Heap/Heap.h"

#include <vector>
#include <algorithm>

namespace pheet {

template <class Pheet, typename TT>
struct ArrayListStrategyTaskStorageItem {
	TT data;
	BaseStrategy<Pheet>* s;
	size_t index;
};
/*
struct HeapElement {
	prio_t pop_prio;
	size_t index;
};*/
/*
class ArrayListStrategyTaskStorageComparator {
public:
	ArrayListStrategyTaskStorageComparator() {};

	bool operator() (HeapElement const& lhs, HeapElement const& rhs) const
	{
		return lhs.pop_prio < rhs.pop_prio;
	}
};
*/
// BLOCK_SIZE has to be a power of 2 to work with wrap-around of index
template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Ret> class StrategyHeapT>
class ArrayListStrategyTaskStorageImpl {
public:
	typedef TT T;
	typedef ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT> ThisType;
//	typedef typename Pheet::Scheduler Scheduler;
	// Not completely a standard iterator, as it doesn't support a dereference operation, but this makes implementation simpler for now (and even more lightweight)
	typedef ArrayListStrategyTaskStorageIterator<ThisType> iterator;
	typedef ArrayListStrategyTaskStoragePerformanceCounters<Pheet> PerformanceCounters;
	typedef ArrayListStrategyTaskStorageControlBlock<ThisType> ControlBlock;
	typedef ArrayListStrategyTaskStorageItem<Pheet, T> Item;
	typedef typename Pheet::Backoff Backoff;
	typedef size_t HeapElement;
	typedef StrategyHeapT<HeapElement, ArrayListStrategyTaskStorageComparator> StrategyHeap;

	ArrayListStrategyTaskStorageImpl(size_t expected_capacity);
//	ArrayListStrategyTaskStorageImpl(size_t expected_capacity, PerformanceCounters& perf_count);
	~ArrayListStrategyTaskStorageImpl();

	iterator begin(PerformanceCounters& pc);
	iterator end(PerformanceCounters& pc);

	T take(iterator item, PerformanceCounters& pc);
	void transfer(ThisType& other, iterator* items, size_t num_items, PerformanceCounters& pc);
	bool is_taken(iterator item, PerformanceCounters& pc);
	prio_t get_steal_priority(iterator item, typename Pheet::Scheduler::StealerDescriptor& sd, PerformanceCounters& pc);
	size_t get_task_id(iterator item, PerformanceCounters& pc);
	void deactivate_iterator(iterator item, PerformanceCounters& pc);

	template <class Strategy>
	void push(Strategy& s, T item, PerformanceCounters& pc);
	T pop(PerformanceCounters& pc);
	T peek(PerformanceCounters& pc);

	size_t get_length(PerformanceCounters& pc);
	bool is_empty(PerformanceCounters& pc);
	bool is_full(PerformanceCounters& pc);

	// Can be called by the scheduler every time it is idle to perform some routine maintenance
	void perform_maintenance(PerformanceCounters& pc);

	static void print_name();

	static size_t const block_size;
protected:
	// can be overridden by child
//	T local_take_best(PerformanceCounters& pc);

private:
	T local_take(size_t block, size_t block_index, PerformanceCounters& pc);
	void clean(PerformanceCounters& pc);
//	void push_internal(BaseStrategy<Pheet>* s, T item, PerformanceCounters& pc);

	ControlBlock* acquire_control_block();
	void create_next_control_block(PerformanceCounters& pc);

	size_t find_block_for_index(size_t index);

	enum {num_control_blocks = 128};
	ControlBlock control_blocks[num_control_blocks];

	// Although we could just use the first element of the current control block to find out the start index instead,
	// there is a race condition that prevents us to do this safely
	size_t start_index;
	size_t end_index;
	/*
	 * Updated on pushes and pops, but doesn't reflect steals, therefore this number is an estimate.
	 * If estimated length of current control block is smaller, use this number instead
	 * When clean() is called and queue is already empty, length is guaranteed to be 0
	 */
	size_t length;

	size_t current_control_block_id;
	size_t cleanup_control_block_id;
	ControlBlock* current_control_block;
	size_t current_control_block_item_index;

	std::vector<Item*> block_reuse;

	StrategyHeap pq;

	PerformanceCounters perf_count;

	static const T null_element;

	friend class ArrayListStrategyTaskStorageIterator<ThisType>;
};

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
const TT ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::null_element = nullable_traits<T>::null_value;

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
const size_t ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::block_size = BLOCK_SIZE;

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::ArrayListStrategyTaskStorageImpl(size_t expected_capacity)
: start_index(0), end_index(0), length(0), current_control_block_id(0), cleanup_control_block_id(0), current_control_block(control_blocks), current_control_block_item_index(0) {
	current_control_block->init_empty(0, block_reuse);
}
/*
template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::ArrayListStrategyTaskStorageImpl(size_t expected_capacity, PerformanceCounters& perf_count)
: top(0), bottom(0), data(expected_capacity), perf_count(perf_count) {

}*/

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::~ArrayListStrategyTaskStorageImpl() {
	Backoff bo;
	while(cleanup_control_block_id != current_control_block_id) {
		if(!control_blocks[cleanup_control_block_id].try_cleanup(block_reuse, 0)) {
			bo.backoff();
		}
		else {
			cleanup_control_block_id = (cleanup_control_block_id + 1) % num_control_blocks;
		}
	}

	size_t l = current_control_block->get_length();
	typename ControlBlock::Item* ccb_data = current_control_block->get_data();
	bool pre_end = true;
	for(size_t i = 0; i < l; ++i) {
		size_t end_pos = end_index - ccb_data[i].offset;
		if(end_pos < block_size) {
		//	current_control_block->clean_item_until(i, end_index);
			current_control_block->finalize_item_until(i, end_index, block_reuse, 0);
			pre_end = false;
		}
		else if(pre_end) {
		//	current_control_block->clean_item(i);
			current_control_block->finalize_item(i, block_reuse, 0);
		}
		else {
			current_control_block->finalize_unused_item(i, block_reuse, 0);
		}
	}
	current_control_block->finalize();

	while(!block_reuse.empty()) {
		delete[] block_reuse.back();
		block_reuse.pop_back();
	}
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
typename ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::iterator ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::begin(PerformanceCounters& pc) {
	return iterator(start_index);
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
typename ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::iterator ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::end(PerformanceCounters& pc) {
	return iterator(end_index);
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
TT ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::local_take(size_t block, size_t block_index, PerformanceCounters& pc) {
	pheet_assert(block < current_control_block->get_length());
	typename ControlBlock::Item* ccb_data = current_control_block->get_data();
	pheet_assert(block_index - ccb_data[block].offset < block_size);

	Item& ptsi = ccb_data[block].data[block_index - ccb_data[block].offset];

	if(ptsi.index != block_index) {
		pc.num_unsuccessful_takes.incr();
		return null_element;
	}
	if(!SIZET_CAS(&(ptsi.index), block_index, block_index + 1)) {
		pc.num_unsuccessful_takes.incr();
		return null_element;
	}

	pc.num_successful_takes.incr();
	return ptsi.data;
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
TT ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::take(iterator item, PerformanceCounters& pc) {
	pheet_assert(item != end(pc));

	Item* ptsi = item.dereference(this);

	if(ptsi == NULL || ptsi->index != item.get_index()) {
		pc.num_unsuccessful_takes.incr();
		return null_element;
	}
	if(!SIZET_CAS(&(ptsi->index), item.get_index(), item.get_index() + 1)) {
		pc.num_unsuccessful_takes.incr();
		return null_element;
	}

	// No danger in using the pointer. As long as the iterator is active, it may not be overwritten
	pc.num_successful_takes.incr();
	return ptsi->data;
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
void ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::transfer(ThisType& other, iterator* items, size_t num_items, PerformanceCounters& pc) {
	pheet_assert(other.is_empty(pc));

	for(size_t i = 0; i < num_items; ++i) {
		// Items have to be sorted by iterator index
		pheet_assert(i == 0 || items[i].get_index() > items[i-1].get_index());

		pheet_assert(items[i] != end(pc));
		Item* ptsi = items[i].dereference(this);
		if(ptsi == NULL || ptsi->index != items[i].get_index()) {
			pc.num_unsuccessful_takes.incr();
		}
		else {
			if(!SIZET_CAS(&(ptsi->index), items[i].get_index(), items[i].get_index() + 1)) {
				pc.num_unsuccessful_takes.incr();
			}
			else {
				pc.num_successful_takes.incr();
				other.push_internal(ptsi->s->clone(), ptsi->data, pc);
			}
		}
	}
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
bool ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::is_taken(iterator item, PerformanceCounters& pc) {
	Item* deref = item.dereference(this);
	return deref == NULL || deref->index != item.get_index();
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
prio_t ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::get_steal_priority(iterator item, typename Pheet::Scheduler::StealerDescriptor& sd, PerformanceCounters& pc) {
	Item* deref = item.dereference(this);
	if(deref == NULL) {
		return 0;
	}
	return deref->s->get_steal_priority(item.get_index(), sd);
}

/*
 * Returns a task id specifying the insertion order. some strategies use the insertion order
 * to prioritize tasks.
 * The task_id is local per queue. When a task is stolen it is assigned a new task id
 * (therefore automatically rebasing the strategy)
 * Warning! If multiple tasks are stolen and inserted into another queue, they have to be
 * inserted in the previous insertion order! (by increasing task_id)
 */
template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline size_t ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::get_task_id(iterator item, PerformanceCounters& pc) {
	return item.get_index();
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline void ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::deactivate_iterator(iterator item, PerformanceCounters& pc) {
	return item.deactivate();
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
template <class Strategy>
inline void ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::push(Strategy& s, T item, PerformanceCounters& pc) {
	pc.push_time.start_timer();

	size_t offset = current_control_block->get_data()[current_control_block_item_index].offset;
	if(end_index - offset == block_size) {
		++current_control_block_item_index;
		if(current_control_block_item_index == current_control_block->get_length()) {
			create_next_control_block(pc);
		}
		offset = current_control_block->get_data()[current_control_block_item_index].offset;
	}
	size_t item_index = end_index - offset;
	pheet_assert(item_index < block_size);

	pc.put_time.start_timer();
	Item to_put;
	to_put.data = item;
	pc.strategy_alloc_time.start_timer();
	// TODO: custom memory management for strategies with placement new. This would allow for bulk deallocations
	// as strategies are always deallocated with their control_block_item (meaning we can deallocation BLOCK_SIZE strategies in one step)
	to_put.s = new Strategy(s);
	pc.strategy_alloc_time.stop_timer();
	to_put.index = end_index;

	current_control_block->get_data()[current_control_block_item_index].data[item_index] = to_put;

	pc.put_time.stop_timer();

	pc.heap_push_time.start_timer();

	/*
	HeapElement he;
	he.pop_prio = s.get_pop_priority(end_index);
	if(he.pop_prio != 0) {
		he.index = end_index;
		pq.push(he);
	}*/
	// TODO: allow forbidding execution of tasks again
	pq.push<Strategy>(end_index);

	pc.heap_push_time.stop_timer();
	pc.max_heap_length.add_value(pq.get_length());

	MEMORY_FENCE();

	++length;
	++end_index;
	pc.push_time.stop_timer();
}
/*
template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline void ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::push_internal(BaseStrategy<Pheet>* s, T item, PerformanceCounters& pc) {
	pc.push_time.start_timer();

	size_t offset = current_control_block->get_data()[current_control_block_item_index].offset;
	if(end_index - offset == block_size) {
		++current_control_block_item_index;
		if(current_control_block_item_index == current_control_block->get_length()) {
			create_next_control_block(pc);
		}
		offset = current_control_block->get_data()[current_control_block_item_index].offset;
	}
	size_t item_index = end_index - offset;
	pheet_assert(item_index < block_size);

	pc.put_time.start_timer();
	Item to_put;
	to_put.data = item;
	// TODO: custom memory management for strategies with placement new. This would allow for bulk deallocations
	// as strategies are always deallocated with their control_block_item (meaning we can deallocation BLOCK_SIZE strategies in one step)
	to_put.s = s;
	to_put.index = end_index;

	current_control_block->get_data()[current_control_block_item_index].data[item_index] = to_put;

	pc.put_time.stop_timer();

	// TODO: allow forbidding execution of tasks again
	pq.push(end_index);

	pc.heap_push_time.stop_timer();
	pc.max_heap_length.add_value(pq.get_length());

	MEMORY_FENCE();

	++length;
	++end_index;
	pc.push_time.stop_timer();
}*/

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
TT ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::pop(PerformanceCounters& pc) {
	pc.total_size_pop.add(length);
	pc.pop_time.start_timer();
	T ret;

	while(!pq.is_empty()) {
		size_t he_index = pq.pop();

		size_t block = find_block_for_index(he_index);
		if(block != current_control_block->get_length()) {
			ret = local_take(block, he_index, pc);

			if(ret != null_element) {
				pheet_assert(length > 0);
				length = std::min(length - 1, pq.get_length());
				pc.num_successful_pops.incr();
				pc.pop_time.stop_timer();
				return ret;
			}
		}
	}

	length = 0;
	pc.num_unsuccessful_pops.incr();
	pc.pop_time.stop_timer();
	return null_element;
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline size_t ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::find_block_for_index(size_t index) {
	size_t l = current_control_block->get_length();
	typename ControlBlock::Item* ccb_data = current_control_block->get_data();

	pheet_assert(l > 0);
	size_t min = 0;
	size_t max = l - 1;

	while(true) {
		size_t curr = (min + max) >> 1;
		ptrdiff_t diff = index - ccb_data[curr].offset;
		if(diff >= 0 && (size_t)diff < block_size) {
			return curr;
		}
		else if(diff < 0) {
			if(curr == min) {
				return l;
			}
			max = curr - 1;
		}
		else {
			if(curr == max) {
				return l;
			}
			min = curr + 1;
		}
	}
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline TT ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::peek(PerformanceCounters& pc) {
	if(pq.empty()) {
		return null_element;
	}
	HeapElement he_index = pq.peek().index;

	size_t block = find_block_for_index(he_index);
	while(block == current_control_block->get_length()) {
		pq.pop();
		he_index = pq.peek().index;
		block = find_block_for_index(he_index);
	}
	typename ControlBlock::Item* ccb_data = current_control_block->get_data();
	return ccb_data[block].data[he_index - ccb_data[block].offset].data;
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline size_t ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::get_length(PerformanceCounters& pc) {
	return length;
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline bool ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::is_empty(PerformanceCounters& pc) {
	clean(pc);
	return length == 0;
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline bool ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::is_full(PerformanceCounters& pc) {
	return false;
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
void ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::clean(PerformanceCounters& pc) {
	pc.clean_time.start_timer();

	while(cleanup_control_block_id != current_control_block_id) {
		if(!control_blocks[cleanup_control_block_id].try_cleanup(block_reuse, current_control_block->get_length())) {
			break;
		}
		else {
			cleanup_control_block_id = (cleanup_control_block_id + 1) % num_control_blocks;
		}
	}

	size_t upper_bound_length = 0;
	size_t l = current_control_block->get_length();
	typename ControlBlock::Item* ccb_data = current_control_block->get_data();
	for(size_t i = 0; i < l; ++i) {
		size_t end_pos = end_index - ccb_data[i].offset;
		if(end_pos < block_size) {
			current_control_block->clean_item_until(i, end_index);
			upper_bound_length += end_index - ccb_data[i].first;
			break;
		}
		else {
			current_control_block->clean_item(i);
			upper_bound_length += (ccb_data[i].offset + block_size) - ccb_data[i].first;
		}
	}
	if(upper_bound_length < length) {
		length = upper_bound_length;
	}
	start_index = current_control_block->get_data()[0].first;

	pc.clean_time.stop_timer();
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
inline void ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::perform_maintenance(PerformanceCounters& pc) {
	clean(pc);
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
void ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::print_name() {
	std::cout << "ArrayListStrategyTaskStorage";
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
typename ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::ControlBlock* ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::acquire_control_block() {
	Backoff bo;
	while(true) {
		ControlBlock* cb = current_control_block;
		// Control block is active. Try acquiring it
		if(cb->try_register_iterator()) {
			return cb;
		}
		bo.backoff();
	}
}

template <class Pheet, typename TT, size_t BLOCK_SIZE, template <typename S, typename Comp> class StrategyHeapT>
void ArrayListStrategyTaskStorageImpl<Pheet, TT, BLOCK_SIZE, StrategyHeapT>::create_next_control_block(PerformanceCounters& pc) {
	pc.create_control_block_time.start_timer();

	clean(pc);

	size_t new_id = (current_control_block_id + 1) % num_control_blocks;
	while(new_id == cleanup_control_block_id) {
		// The next control block we want to use is still blocked - skip it
		// Will be cleaned up in the next round
		pc.num_skipped_cleanups.incr();
		cleanup_control_block_id = (cleanup_control_block_id + 1) % num_control_blocks;
		clean(pc);
		new_id = (new_id + 1) % num_control_blocks;
	}
	pheet_assert(new_id != cleanup_control_block_id);
	pc.configure_successor_time.start_timer();
	size_t upper_bound_length = control_blocks[new_id].configure_as_successor(current_control_block, block_reuse);
	pc.configure_successor_time.stop_timer();
	if(upper_bound_length < length) {
		length = upper_bound_length;
	}

	MEMORY_FENCE();

	current_control_block_id = new_id;
	current_control_block = control_blocks + new_id;
	current_control_block_item_index = current_control_block->get_new_item_index();
	start_index = current_control_block->get_data()[0].first;

	pc.max_control_block_items.add_value(current_control_block->get_length());

	pc.create_control_block_time.stop_timer();
}

template<class Pheet, typename TT>
using ArrayListStrategyTaskStorage = ArrayListStrategyTaskStorageImpl<Pheet, TT, 256, Pheet::DS::template PQ>;

}


#endif /* ARRAYLISTSTRATEGYTASKSTORAGE_H_ */
