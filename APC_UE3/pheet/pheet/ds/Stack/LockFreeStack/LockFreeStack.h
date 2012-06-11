/*
 * GlobalLockStack.h
 *
 *  Created on: May 29, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0
 */

#ifndef LOCKFREESTACK_H_
#define LOCKFREESTACK_H_

#include <vector>
#include <iostream>
#include "../../../misc/type_traits.h"
#include "../../../misc/atomics.h"
#include <atomic>

namespace pheet {

template <class Pheet, typename TT>
class LockFreeStack {
public:
	typedef typename Pheet::Mutex Mutex;
	typedef typename Pheet::LockGuard LockGuard;

	LockFreeStack()
	: length(0){}
	~LockFreeStack() {}

	atomic<TT*> top = Node(nullable_traits<TT>::null_value);


	protected:  bool tryPush(TT node){
	  TT oldTop = std::atomic_load(top);

	  node.next = oldTop;
	  return(std::atomic_compare_exchange_weak(node, oldTop, node));
	}

	public: void push(TT value) {
	  TT node = new Node(value);

	  {// Scope of backoff
		  while (true) {
			if (tryPush(node)) {
			  return;
			} else {
			  typename Pheet::Backoff bo;
			  bo.backoff();
			}
		  }
	  }
	}

	protected: TT tryPop() {
	  TT oldTop = std::atomic_load(top);
	  if (oldTop == NULL) {
		  return nullable_traits<TT>::null_value;
	  }
	  TT newTop = oldTop.next;
	  if (std::atomic_compare_exchange_weak(top, oldTop, newTop)) {
		return oldTop;
	  } else {
		return NULL;
	  }
	}

	public: TT pop() {
		  while (true) {
			TT returnNode = tryPop();
			if (returnNode != NULL) {
			  return returnNode.value;
			} else {
				{
				  typename Pheet::Backoff bo;
				  bo.backoff();
				}
			}
		  }
	}

	class Node {
		public:
			TT value;
			Node next;

		public: Node(TT value) {
			this->value = value;
			next = NULL;
		}
	};

	inline size_t get_length() {
		return length;
	}

	inline size_t size() {
		return get_length();
	}

	static void print_name() {
		std::cout << "LockFreeStack<";
		Mutex::print_name();
		std::cout << ">";
	}

	private:
		std::vector<TT> data;
		size_t length;
		Mutex m;

};





} /* namespace pheet */
#endif /* LOCKFREESTACK_H_ */
