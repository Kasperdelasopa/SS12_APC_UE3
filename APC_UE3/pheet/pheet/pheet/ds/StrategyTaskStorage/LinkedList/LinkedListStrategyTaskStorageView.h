/*
 * LinkedListStrategyTaskStorageView.h
 *
 *  Created on: 07.04.2012
 *      Author: Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef LINKEDLISTSTRATEGYTASKSTORAGEVIEW_H_
#define LINKEDLISTSTRATEGYTASKSTORAGEVIEW_H_

#include <vector>
#include "../../../misc/atomics.h"

#include "LinkedListStrategyTaskStorageDataBlock.h"

namespace pheet {

template <class Pheet, typename TT, size_t BlockSize>
class LinkedListStrategyTaskStorageView {
public:
	typedef LinkedListStrategyTaskStorageView<Pheet, TT, BlockSize> Self;
	typedef LinkedListStrategyTaskStorageDataBlock<Pheet, TT, Self, BlockSize> DataBlock;

	LinkedListStrategyTaskStorageView(DataBlock*& front);
	LinkedListStrategyTaskStorageView(DataBlock*& front, Self* prev);
	~LinkedListStrategyTaskStorageView();

	void mark_empty(DataBlock* block);

	void clean(std::vector<Self*>& view_reuse);

	bool try_register();
	void deregister();

	bool is_reusable() { return created_blocks == 0; }
	bool needs_cleanup() { return created_blocks >= (freed_blocks.size() << 1); }

	bool try_lock();

	void reset(Self* prev);
	void update_front();
private:
	DataBlock*& front;
	Self* prev;
	Self* next;

	size_t created_blocks;
	std::vector<DataBlock*> freed_blocks;

	ptrdiff_t reg;
};

template <class Pheet, typename TT, size_t BlockSize>
LinkedListStrategyTaskStorageView<Pheet, TT, BlockSize>::LinkedListStrategyTaskStorageView(DataBlock*& front)
:front(front), prev(nullptr), next(nullptr), created_blocks(0), reg(0) {

}

template <class Pheet, typename TT, size_t BlockSize>
LinkedListStrategyTaskStorageView<Pheet, TT, BlockSize>::LinkedListStrategyTaskStorageView(DataBlock*& front, Self* prev)
:front(front), prev(prev), next(nullptr), created_blocks(0), reg(0) {
	pheet_assert(prev->next == nullptr);
	prev->next = this;
}

template <class Pheet, typename TT, size_t BlockSize>
LinkedListStrategyTaskStorageView<Pheet, TT, BlockSize>::~LinkedListStrategyTaskStorageView() {
	pheet_assert(reg <= 0);

	for(auto i = freed_blocks.begin(); i != freed_blocks.end(); ++i) {
		delete *i;
	}
}

template <class Pheet, typename TT, size_t BlockSize>
void LinkedListStrategyTaskStorageView<Pheet, TT, BlockSize>::mark_empty(DataBlock* block) {
	freed_blocks.push_back(block);
	if(front == block) {
		block = front->get_next();
	}
}

template <class Pheet, typename TT, size_t BlockSize>
bool LinkedListStrategyTaskStorageView<Pheet, TT, BlockSize>::try_lock() {
	if(reg == 0) {
		if(PTRDIFFT_CAS(&reg, 0, -1)) {
			return true;
		}
	}
	return reg == -1;
}

template <class Pheet, typename TT, size_t BlockSize>
void LinkedListStrategyTaskStorageView<Pheet, TT, BlockSize>::clean(std::vector<Self*>& view_reuse) {
	pheet_assert(reg == -1);
	for(auto i = freed_blocks.begin(); i != freed_blocks.end(); ++i) {
		if((*i)->get_first_view() == this) {
			--created_blocks;
			delete *i;
		}
		else {
			pheet_assert(prev != nullptr);
			prev->freed_blocks.push_back(*i);
		}
	}
	freed_blocks.clear();
	if(prev != nullptr && prev->reg == -1) {
		prev->clean(view_reuse);
		if(created_blocks == 0) {
			prev->next = next;
		}
	}
	pheet_assert(next != nullptr);
	if(created_blocks == 0) {
		next->prev = prev;
		view_reuse.push_back(this);
	}
}

template <class Pheet, typename TT, size_t BlockSize>
void LinkedListStrategyTaskStorageView<Pheet, TT, BlockSize>::reset(Self* prev) {
	pheet_assert(reg == -1);
	pheet_assert(prev != nullptr);
	pheet_assert(freed_blocks.empty());

	this->prev = prev;
	next = nullptr;
	created_blocks = prev->created_blocks;
	pheet_assert(prev->next == nullptr);
	prev->next = this;

	MEMORY_FENCE();

	reg = 0;
}

template <class Pheet, typename TT, size_t BlockSize>
void LinkedListStrategyTaskStorageView<Pheet, TT, BlockSize>::update_front() {
	while(!front->is_active()) {
		front = front->get_next();
	}
}


}

#endif /* LINKEDLISTSTRATEGYTASKSTORAGEVIEW_H_ */
