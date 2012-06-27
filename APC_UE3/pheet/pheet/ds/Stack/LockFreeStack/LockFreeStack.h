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
#include <pheet/misc/atomics.h>

namespace pheet {

template<typename TT>
	class Node {
	typedef Node<TT>  LFSNode;
		public:
			TT value;
			LFSNode *next;

		public: Node(TT value) {
			this->value = value;
			next = NULL;
		}
	};

template <class Pheet, typename TT>
class LockFreeStack {
public:
	typedef typename Pheet::Mutex Mutex;
	typedef typename Pheet::LockGuard LockGuard;
	typedef Node<TT>  LFSNode;
	LFSNode *top;


	LockFreeStack()
	: top(new LFSNode(nullable_traits<TT>::null_value)), length(0){}
	~LockFreeStack() {}

protected:
	bool tryPush(LFSNode *node){
		LFSNode * oldTop = top;

		node->next = oldTop;

		return(PTR_CAS(&top, oldTop, node));
	}

public:
	void push(TT value) {
		LFSNode *node = new LFSNode(value);
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

protected:
		LFSNode* tryPop() {
			LFSNode *oldTop = top;
		  if (oldTop == NULL) {
			  return NULL;
		  }
		  LFSNode *newTop = oldTop->next;
		  if (PTR_CAS(&top, oldTop, newTop)) {
			return oldTop;
		  } else {
			return NULL;
		  }
	}

	public:
	  TT pop() {
		  while (true) {
			  LFSNode * returnNode = tryPop();
			if (returnNode != NULL) {
			  return returnNode->value;
			} else {
				{
				  typename Pheet::Backoff bo;
				  bo.backoff();
				}
			}
		  }
	}


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
		size_t length;
};

} /* namespace pheet */
#endif /* LOCKFREESTACK_H_ */
