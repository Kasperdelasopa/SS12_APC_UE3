/*
 * GlobalLockStack.h
 *
 *  Created on: May 29, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0
 */

#ifndef ELIMINATIONBACKOFFSTACK_H_
#define ELIMINATIONBACKOFFSTACK_H_

#include <vector>
#include <iostream>
#include <exception>
#include "../../../misc/type_traits.h"
#include "../../../misc/atomics.h"
#include <pheet/misc/atomics.h>
//#include <pheet/ds/Stack/EliminationBackoffStack/AtomicStampedReference.h>
#include <pheet/ds/Stack/EliminationBackoffStack/EliminationArray.h>
#include <pheet/ds/Stack/LockFreeStack/LockFreeStack.h>

namespace pheet {

template <class Pheet, typename TT>
class EliminationBackoffStack : LockFreeStack<Pheet, TT> {
	typedef EliminationArray<TT>  EA;
	typedef typename Pheet::Mutex Mutex;
	typedef typename Pheet::LockGuard LockGuard;
	typedef Node<TT>  LFSNode;

private:
	  int capacity;
	  EA *eliminationArray;

public:
	EliminationBackoffStack()
	: LockFreeStack<Pheet, TT>(), capacity(std::numeric_limits<int>::max()),eliminationArray(new EA()),length(0) {}
	~EliminationBackoffStack() {}

public:
    void push(TT value) {
      LFSNode *node = new LFSNode(value);

	  while (true) {
		  if(this->tryPush(node)) {
		  		  return;
		  		}
		  		else try {
		  		  if (eliminationArray->visit(value, capacity) == nullable_traits<TT>::null_value) {
		  			return;
		  		  }
		  		} catch (exception & ex) {
		  }
	  }
    }

public:
  TT pop(){

  while (true) {

	LFSNode *returnNode = this->tryPop();

    if (returnNode != NULL) {

      return returnNode->value;

    } else try {

      TT otherValue = eliminationArray->visit(nullable_traits<TT>::null_value, capacity);

      if (otherValue != nullable_traits<TT>::null_value) {

        return otherValue;
      }

    } catch (exception & ex) {

    }
  }
}

public:
	inline size_t get_length() {
		return length;
	}

	inline size_t size() {
		return get_length();
	}

	static void print_name() {
		std::cout << "<EliminationBackoffStack<";
		Mutex::print_name();
		std::cout << ">";
	}

	private:
		size_t length;


};



} /* namespace pheet */
#endif /* ELIMINATIONBACKOFFSTACK_H_ */
