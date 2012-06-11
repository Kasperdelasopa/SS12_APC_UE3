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
  std::atomic<int*> top = new Node();
  //Test


	protected:  bool tryPush(Node node){
	  Node oldTop = top.get();
	  node.next = oldTop;
	  return(NODE_CAS(node, oldTop, node));
	}

	public: void push(TT value) {
	  Node node = new Node(value);

	  {// Scope of backoff
	    Pheet::Backoff bo;
		  while (true) {
			if (tryPush(node)) {
			  return;
			} else {
			  bo.backoff();
			}
		  }
	  }
	}

	protected: Node tryPop() {
	  Node oldTop = top.get();
	  if (oldTop == NULL) {
		  return nullable_traits<TT>::null_value;
	  }
	  Node newTop = oldTop.next;
	  if (Node_CAS(top, oldTop, newTop)) {
		return oldTop;
	  } else {
		return NULL;
	  }
	}

	public: TT pop() {
		{// Scope of backoff
			Pheet::Backoff bo;
			  while (true) {
				Node returnNode = tryPop();
				if (returnNode != NULL) {
				  return returnNode.value;
				} else {
				  bo.backoff();
				}
			  }
		}
	}

};
template <class Pheet, typename TT>
class Node {
	public:
		TT value;
		Node next;

	public: Node(TT value) {
		this->value = value;
		next = NULL;
	}
};


} /* namespace pheet */
#endif /* GLOBALLOCKSTACK_H_ */
