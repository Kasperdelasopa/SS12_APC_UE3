/*
 * EliminationArray.h
 *
 *  Created on: 27.06.2012
 *      Author: kasperdelasopa
 */
#ifndef ELIMINATIONARRAY_H_
#define ELIMINATIONARRAY_H_

#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <pheet/ds/Stack/EliminationBackoffStack/LockFreeExchanger.h>

namespace pheet {

template<typename TT>
class EliminationArray{
public:
	typedef LockFreeExchanger<TT>  LFE;

	EliminationArray() : duration(std::numeric_limits<int>::max()) {}
	~EliminationArray() {}

private:
	int duration;
	LFE *exchanger;

public:
	EliminationArray(int capacity) {

       exchanger = (LFE[]) new LFE[capacity];

      for (int i = 0; i < capacity; i++) {

         exchanger[i] = new LFE();

       }

     }

public:
	TT visit(TT value, int range) {

       int slot = rand() % range +1;

       return ((exchanger[slot]).exchange(&value, duration));

     }

   };

}

#endif /* ELIMINATIONARRAY_H_ */
