/*
 * LockFreeExchanger.h
 *
 *  Created on: 27.06.2012
 *      Author: kasperdelasopa
 */

#ifndef LOCKFREEEXCHANGER_H_
#define LOCKFREEEXCHANGER_H_

#include <vector>
#include <iostream>
#include <sys/time.h>
#include "../../../misc/type_traits.h"
#include "../../../misc/atomics.h"
#include <pheet/misc/atomics.h>
#include <pheet/ds/Stack/EliminationBackoffStack/AtomicStampedReference.h>

namespace pheet {

template<typename TT>
class LockFreeExchanger
{
	public:
		int EMPTY, WAITING, BUSY;
		timespec ts;
		TT *value;

		LockFreeExchanger(): EMPTY(1), WAITING(2), BUSY(3) {}
		~LockFreeExchanger() {}

	public:
		TT exchange(TT *myItem, long timeout)
		{
			timeval tp;
			gettimeofday(&tp, 0);

			long timeBound = timeout + tp.tv_usec/1000;

			int stampHolder[] = {EMPTY};

			while (true)
			{
			  gettimeofday(&tp, 0);
			  if (tp.tv_usec/1000 > timeBound)
			  {
				  throw std::exception();
			  }
			  TT *yrItem = value;
			  int stamp = stampHolder[0];

			  switch(stamp)
			  {
				  case 1:
				  {
					if (PTR_CAS(&value, yrItem, myItem))
					{
					  gettimeofday(&tp, 0);
					  while (tp.tv_usec/1000 < timeBound)
					  {
						yrItem = myItem;
						if (stampHolder[0] == BUSY)
						{
						  value = NULL;
						  return *yrItem;
						}
					  }
					  if (PTR_CAS(&value, myItem, NULL))
					  {
						  throw std::exception();
					  }
					  else
					  {
						yrItem = myItem;
						value = NULL;
						return *yrItem;
					  }
					break;
				  }
				  case 2:
				  {
					if (PTR_CAS(&value, yrItem, myItem))
					  return *yrItem;
					break;
				  }
				  case 3:
				  {
					break;
				  }
				  default:
				  {
					  break;
				  }
				}
			  }
		}
	}

};

}
#endif /* LOCKFREEEXCHANGER_H_ */
