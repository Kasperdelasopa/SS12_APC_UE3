/*
 * LinkedListStrategyTaskStorageStream.h
 *
 *  Created on: Apr 16, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef LINKEDLISTSTRATEGYTASKSTORAGESTREAM_H_
#define LINKEDLISTSTRATEGYTASKSTORAGESTREAM_H_

namespace pheet {

template <class Pheet>
class LinkedListStrategyTaskStorageStreamDataBlock {

	Self* get_next();
private:
	Self* prev;
	Self* next;

	size_t references;
	DataBlock* block;
	View* view;
	size_t* view_refs;
};

template <class Pheet, class StreamDataBlock>
class LinkedListStrategyTaskStorageStreamRef {

private:
	StreamDataBlock* block;
	size_t index;
};

template <class Pheet, class TaskStorage>
class LinkedListStrategyTaskStorageStream {
public:
	typedef typename TaskStorage::Item Item;

	LinkedListStrategyTaskStorageStream(TaskStorage* task_storage);
	~LinkedListStrategyTaskStorageStream();

	inline Item& deref(StreamRef const& ref) {
		return ref.block->block->get_data(ref.index);
	}

	bool has_next(StreamRef const& ref) {
		return (ref.index < (ref.block->block->get_size())) ||
				(ref.block->block->get_next() != nullptr);
	}
	StreamRef next() {

	}
	void next(StreamRef& ref) {
		pheet_assert(has_next(ref));
		if(ref.index < ref.block->block->get_max_size()) {
			++ref.index;
		}
		else {
			ref.index = 0;
			ref.block = ref.block->get_next();
		}
	}
private:
	void check_current_view();

	TaskStorage* task_storage;

	View* current_view;
	size_t* current_view_refs;
};

template <class Pheet, class TaskStorage>
LinkedListStrategyTaskStorageStream::LinkedListStrategyTaskStorageStream(TaskStorage* task_storage)
: task_storage(task_storage),
  current_view(task_storage->acquire_current_view()),
  current_view_refs(new size_t()) {

}

template <class Pheet, class TaskStorage>
LinkedListStrategyTaskStorageStream::~LinkedListStrategyTaskStorageStream() {
	pheet_assert((*current_view_refs) != 0);
	--(*current_view_refs);
	if((*current_view_refs) == 0) {
		current_view->deregister();
	}
}

template <class Pheet, class TaskStorage>
inline void
LinkedListStrategyTaskStorageStream::check_current_view() {
	if(task_storage->get_current_view() != current_view) {
		pheet_assert((*current_view_refs) != 0);
		--(*current_view_refs);
		if((*current_view_refs) == 0) {
			current_view->deregister();
		}

		current_view = task_storage->acquire_current_view();
		current_view_refs = new size_t();
	}
}

}

#endif /* LINKEDLISTSTRATEGYTASKSTORAGESTREAM_H_ */
