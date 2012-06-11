/*
 * LinkedListStrategyTaskStorageDataBlock.h
 *
 *  Created on: 07.04.2012
 *      Author: Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef LINKEDLISTSTRATEGYTASKSTORAGEDATABLOCK_H_
#define LINKEDLISTSTRATEGYTASKSTORAGEDATABLOCK_H_

namespace pheet {

template <class Pheet, typename TT, class View, size_t BlockSize>
class LinkedListStrategyTaskStorageDataBlock {
public:
	typedef TT T;
	typedef LinkedListStrategyTaskStorageDataBlock<Pheet, TT, View, BlockSize> Self;

	LinkedListStrategyTaskStorageDataBlock(size_t id, View* first_view, Self* prev);
	~LinkedListStrategyTaskStorageDataBlock() {}

	bool take(size_t index, typename T::Item& ret, View* current_view);
	void mark_removed(size_t index, View* current_view);
	T& peek(size_t index);

	size_t push(T&& item, View* current_view);

	inline T& get_data(size_t index) {
		pheet_assert(index < filled);
		return data[index];
	}
	inline Self* get_next() { return next; }
	inline View* get_first_view() { return first_view; }
	inline bool is_active() { return active != 0; }
	inline size_t get_size() { return filled; }
	inline size_t get_max_size() { return BlockSize; }

private:
	void clean(View* current_view);

	T data[BlockSize];

	size_t id;
	View* first_view;
	Self* prev;
	Self* next;
	size_t filled;

	size_t active;
};

template <class Pheet, typename TT, class View, size_t BlockSize>
LinkedListStrategyTaskStorageDataBlock<Pheet, TT, View, BlockSize>::LinkedListStrategyTaskStorageDataBlock(size_t id, View* first_view, Self* prev)
:id(id), first_view(first_view), prev(prev), next(nullptr), filled(0), active(BlockSize) {

}

template <class Pheet, typename TT, class View, size_t BlockSize>
bool LinkedListStrategyTaskStorageDataBlock<Pheet, TT, View, BlockSize>::take(size_t index, typename T::Item& ret, View* current_view) {
	pheet_assert(active > 0);
	pheet_assert(index < filled);
	--active;

	if(data[index].taken == 0) {
		if(INT_CAS(&(data[index].taken), 0, 1)) {
			ret = data[index].item;
			clean(current_view);
			return true;
		}
	}
	clean(current_view);
	return false;
}

template <class Pheet, typename TT, class View, size_t BlockSize>
void LinkedListStrategyTaskStorageDataBlock<Pheet, TT, View, BlockSize>::mark_removed(size_t index, View* current_view) {
	pheet_assert(active > 0);
	pheet_assert(index < filled);
	--active;
	clean(current_view);
}

template <class Pheet, typename TT, class View, size_t BlockSize>
TT& LinkedListStrategyTaskStorageDataBlock<Pheet, TT, View, BlockSize>::peek(size_t index) {
	pheet_assert(active > 0);
	pheet_assert(index < filled);

	return data[index].item;
}

template <class Pheet, typename TT, class View, size_t BlockSize>
void LinkedListStrategyTaskStorageDataBlock<Pheet, TT, View, BlockSize>::clean(View* current_view) {
	// There should be no problems with id wraparound, except that block with id 0 ignores rules
	// This means O(log(n)) is not satisfied in for every predecessor of block 0
	// This leads to a complexity of O((n/(size_t_max + 1))*log(size_t_max)) for n > size_t max which isn't really bad
	// and rare in any case (it's linear to the number of wraparounds and not logarithmic)

	if(active == 0 // Check if block is not used any more
			&& (id == 0 // Either free block if has id 0 (special case)
					// Or make sure the next block is a higher power of two (to guarantee O(log(n)) access times
					// for other threads missing elements (don't want them to have O(n) for elements they never need)
					|| ((next->id & id) != next->id))) {
		 // Only free blocks if they have a successor (I believe as it may only be inactive if there is a successor)
		pheet_assert(next != nullptr);
		next->prev = prev;
		current_view->mark_empty(this);
		if(prev != nullptr) {
			prev->next = next;
			// Tail recursion
			prev->clean(current_view);
		}
	}
	current_view->update_front();
}

template <class Pheet, typename TT, class View, size_t BlockSize>
size_t LinkedListStrategyTaskStorageDataBlock<Pheet, TT, View, BlockSize>::push(T&& item, View* current_view) {
	pheet_assert(filled < BlockSize);

	data[filled] = item;

	if(filled == BlockSize - 1) {
		Self* tmp = new Self(id + 1, current_view, this);
		MEMORY_FENCE();
		next = tmp;
	}
	else {
		MEMORY_FENCE();
	}
	++filled;
	return filled - 1;
}

}

#endif /* LINKEDLISTSTRATEGYTASKSTORAGEDATABLOCK_H_ */
