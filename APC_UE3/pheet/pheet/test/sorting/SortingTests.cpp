/*
 * SortingTests.cpp
 *
 *  Created on: 07.04.2011
 *      Author: Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#include "SortingTests.h"

#ifdef SORTING_TEST
#include "Reference/ReferenceSTLSort.h"
#include "Reference/ReferenceQuicksort.h"
//#include "Strategy/StrategyQuicksort.h"
#include "Dag/DagQuicksort.h"
#include "MixedMode/MixedModeQuicksort.h"
#include "Reference/ReferenceHeapSort.h"

//#include <pheet/ds/StealingDeque/CircularArray11/CircularArrayStealingDeque11.h>
#include <pheet/ds/PriorityQueue/Heap/Heap.h>
#include <pheet/ds/PriorityQueue/STLPriorityQueueWrapper/STLPriorityQueueWrapper.h>
#include <pheet/ds/PriorityQueue/SortedArrayHeap/SortedArrayHeap.h>
#include <pheet/ds/Queue/GlobalLock/GlobalLockQueue.h>
#include <pheet/ds/MultiSet/GlobalLock/GlobalLockMultiSet.h>

#include <pheet/primitives/Mutex/TASLock/TASLock.h>
#include <pheet/primitives/Mutex/TTASLock/TTASLock.h>

#include <pheet/pheet.h>
#include <pheet/sched/Basic/BasicScheduler.h>
#include <pheet/sched/Finisher/FinisherScheduler.h>
#include <pheet/sched/Centralized/CentralizedScheduler.h>
#include <pheet/sched/CentralizedPriority/CentralizedPriorityScheduler.h>
//#include <pheet/sched/Strategy/StrategyScheduler.h>
#include <pheet/sched/Synchroneous/SynchroneousScheduler.h>
#include <pheet/sched/MixedMode/MixedModeScheduler.h>

#include <iostream>
#endif

namespace pheet {


SortingTests::SortingTests() {

}

SortingTests::~SortingTests() {

}

void SortingTests::run_test() {
#ifdef SORTING_TEST
	std::cout << "----" << std::endl;

#ifdef AMP_STEALING_DEQUE_TEST
//	this->run_sorter<	Pheet::WithScheduler<BasicScheduler>::WithTaskStorage<YourImplementation>,
//						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<BasicScheduler>,
						DagQuicksort>();

#elif AMP_QUEUE_STACK_TEST

	this->run_sorter<	Pheet::WithScheduler<CentralizedScheduler>,
						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<CentralizedScheduler>::WithTaskStorage<GlobalLockQueue>,
						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<BasicScheduler>,
						DagQuicksort>();
#elif AMP_SKIPLIST_TEST
	this->run_sorter<	Pheet::WithScheduler<CentralizedPriorityScheduler>,
						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<CentralizedPriorityScheduler>::WithPriorityTaskStorage<GlobalLockMultiSetPriorityQueue>,
						DagQuicksort>();
	this->run_sorter<	Pheet,
						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<BasicScheduler>,
						DagQuicksort>();

#elif AMP_LOCK_TEST

	this->run_sorter<	Pheet::WithScheduler<CentralizedScheduler>,
						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<CentralizedScheduler>::WithMutex<TASLock>,
						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<CentralizedScheduler>::WithMutex<TTASLock>,
						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<BasicScheduler>,
						DagQuicksort>();
#else
	// default tests
	this->run_sorter<	Pheet,
						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<BasicScheduler>,
						DagQuicksort>();
//	this->run_sorter<	Pheet::WithScheduler<BasicScheduler>::WithStealingDeque<CircularArrayStealingDeque11>,
//						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<SynchroneousScheduler>,
						DagQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<SynchroneousScheduler>,
						ReferenceSTLSort>();
	this->run_sorter<	Pheet::WithScheduler<SynchroneousScheduler>,
						ReferenceQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<SynchroneousScheduler>,
						ReferenceHeapSort>();
	this->run_sorter<	Pheet::WithScheduler<MixedModeScheduler>,
						MixedModeQuicksort>();
	this->run_sorter<	Pheet::WithScheduler<FinisherScheduler>,
						DagQuicksort>();
#endif

#endif
}


}
