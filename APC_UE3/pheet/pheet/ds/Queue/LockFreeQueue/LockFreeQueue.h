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
#include<pheet/misc/atomics.h>

namespace pheet {

template<typename TT>
class LockFreeQueueNode {

public:
	TT item;
	LockFreeQueueNode* next;

public:
	void setItem(TT i) {
		item = i;
	}
	;
	int compare_exchange(LockFreeQueueNode* last, LockFreeQueueNode* next) {
		return PTR_CAS(&(this->next), last, next);
	}
};

template<class Pheet, typename TT>

class LockFreeQueue {
public:
	typedef typename Pheet::Mutex Mutex;
	typedef typename Pheet::LockGuard LockGuard;
	typedef LockFreeQueueNode<TT> LFQNode;

	LFQNode* head = new LFQNode();
	LFQNode* tail = head;

	LockFreeQueue() :
		length(0) {}
	~LockFreeQueue() {
	}

	void push(TT const& item) {

		//std::cout << "push" << std::endl;
		LFQNode* node = new LFQNode();
		node->item = item;
		while (true) {
			LFQNode *last = tail;
			LFQNode *next = tail->next;
			if (next == nullptr) {

				if (last->compare_exchange(nullptr, node)) {

					//	tail->compare_exchange(last,node);
					PTR_CAS(&(tail), tail, node);
					length++;
					//print(head);
					return;
				}
			} else {
			tail->compare_exchange(last, next);
			}
		}

	}

	TT pop() {
		//std::cout << "pop" << std::endl;
		while (true) {
			LFQNode* first = head;
			LFQNode* last = tail;
			LFQNode* next = first->next;

			if (first == last) {

				if (next == nullptr) {//the queue is empty

						return nullable_traits<TT>::null_value; //return null  value like in GlobalLockQueue
				}
				tail->compare_exchange(last, next);
			} else {
				//std::cout << "first != last" << std::endl;

				TT i = next->item;
				if (PTR_CAS(&(head), head, head->next)) {
					length--;
					return i;



				}


			}

		}
	}

	inline size_t get_length() {
		return length;
	}
	;

	inline size_t size() {
		return get_length();
	}
	;

	static void print_name() {
		std::cout << "LockFreeQueue<";
		Mutex::print_name();
		std::cout << ">";
	}
	;


private:
	void print(LFQNode* node) {
		std::cout << "->node";
		if (head == node) {
			std::cout << "(head)";
		}
		if (tail == node) {
			std::cout << "(tail)";
		}

		if (node->next != nullptr) {
			print(node->next);
		} else {
			std::cout << "->null\n";
		}

	}

	size_t length;

};

} /* namespace pheet */
#endif /* LockFreeQueue_H_ */
