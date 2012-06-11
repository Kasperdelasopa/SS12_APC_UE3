/*
 * GraphBipartitioningTests.cpp
 *
 *  Created on: 07.09.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#include "GraphBipartitioningTests.h"

#ifdef GRAPH_BIPARTITIONING_TEST
/*
#include "BranchBound/BranchBoundGraphBipartitioning.h"
#include "BranchBound/BasicLowerBound.h"
#include "BranchBound/BasicNextVertex.h"
#include "StrategyBranchBound/StrategyBranchBoundGraphBipartitioning.h"
#include "StrategyBranchBound/BranchBoundGraphBipartitioningAutoStrategy.h"
#include "StrategyBranchBound/BranchBoundGraphBipartitioningBestFirstStrategy.h"

#include "ImprovedBranchBound/ImprovedBranchBoundGraphBipartitioningBasicLogic.h"
#include "ImprovedBranchBound/ImprovedBranchBoundGraphBipartitioningImprovedLogic.h"
#include "ImprovedBranchBound/ImprovedBranchBoundGraphBipartitioningDeltaLogic.h"
#include "ImprovedBranchBound/ImprovedBranchBoundGraphBipartitioningDeltaNVLogic.h"
#include "ImprovedBranchBound/ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic.h"
*/

#include <pheet/sched/Basic/BasicScheduler.h>
#include <pheet/sched/Centralized/CentralizedScheduler.h>

#include <pheet/ds/Queue/GlobalLock/GlobalLockQueue.h>
#include <pheet/ds/MultiSet/GlobalLock/GlobalLockMultiSet.h>

#include <pheet/primitives/Mutex/TASLock/TASLock.h>
#include <pheet/primitives/Mutex/TTASLock/TTASLock.h>

#include "ImprovedBranchBound/ImprovedBranchBoundGraphBipartitioning.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioning.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundFifoStrategy.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioningUpperLowerBoundStrategy.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioningAutoStrategy.h"
#include "ImprovedStrategyBranchBound/ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategy.h"
/*
#include "../../sched/strategies/Fifo/FifoStrategy.h"
#include "../../sched/strategies/Lifo/LifoStrategy.h"
#include "../../sched/strategies/LifoFifo/LifoFifoStrategy.h"

#include "../test_schedulers.h"*/
#endif

namespace pheet {

GraphBipartitioningTests::GraphBipartitioningTests() {

}

GraphBipartitioningTests::~GraphBipartitioningTests() {

}

void GraphBipartitioningTests::run_test() {
#ifdef GRAPH_BIPARTITIONING_TEST
	std::cout << "----" << std::endl;

#ifdef AMP_STEALING_DEQUE_TEST
//	this->run_partitioner<	Pheet::WithScheduler<BasicScheduler>::WithTaskStorage<YourImplementation>,
//							ImprovedBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet::WithScheduler<BasicScheduler>,
							ImprovedBranchBoundGraphBipartitioning>();

#elif AMP_QUEUE_STACK_TEST

	this->run_partitioner<	Pheet::WithScheduler<CentralizedScheduler>,
						ImprovedBranchBoundGraphBipartitioning>();

	this->run_partitioner<	Pheet::WithScheduler<CentralizedScheduler>::WithTaskStorage<GlobalLockQueue>,
						ImprovedBranchBoundGraphBipartitioning>();

	this->run_partitioner<	Pheet::WithScheduler<BasicScheduler>,
						ImprovedBranchBoundGraphBipartitioning>();

#elif AMP_SKIPLIST_TEST
	this->run_partitioner<	Pheet::WithScheduler<CentralizedScheduler>,
						ImprovedBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet::WithScheduler<CentralizedScheduler>::WithPriorityTaskStorage<GlobalLockMultiSetPriorityQueue>,
						ImprovedBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet,
						ImprovedBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet::WithScheduler<BasicScheduler>,
						ImprovedBranchBoundGraphBipartitioning>();


#elif AMP_LOCK_TEST

	this->run_partitioner<	Pheet::WithScheduler<CentralizedScheduler>,
						ImprovedBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet::WithScheduler<CentralizedScheduler>::WithMutex<TASLock>,
						ImprovedBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet::WithScheduler<CentralizedScheduler>::WithMutex<TTASLock>,
						ImprovedBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet::WithScheduler<BasicScheduler>,
						ImprovedBranchBoundGraphBipartitioning>();

#else

	this->run_partitioner<	Pheet,
							ImprovedStrategyBranchBoundGraphBipartitioning<>::WithSchedulingStrategy<ImprovedStrategyBranchBoundGraphBipartitioningDynamicStrategy>::T >();
	this->run_partitioner<	Pheet,
							ImprovedStrategyBranchBoundGraphBipartitioning<>::WithSchedulingStrategy<ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy>::T >();
	this->run_partitioner<	Pheet,
							ImprovedStrategyBranchBoundGraphBipartitioning<>::WithSchedulingStrategy<ImprovedStrategyBranchBoundGraphBipartitioningUpperLowerBoundStrategy>::T >();
	this->run_partitioner<	Pheet,
							ImprovedStrategyBranchBoundGraphBipartitioning<>::WithSchedulingStrategy<ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy>::T >();
	this->run_partitioner<	Pheet,
							ImprovedStrategyBranchBoundGraphBipartitioning>(); // strategy: ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy
	this->run_partitioner<	Pheet,
							ImprovedStrategyBranchBoundGraphBipartitioning<>::WithSchedulingStrategy<ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy>::T >();
	this->run_partitioner<	Pheet,
							ImprovedStrategyBranchBoundGraphBipartitioning<>::WithSchedulingStrategy<ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy>::T >();
	this->run_partitioner<	Pheet,
							ImprovedStrategyBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet,
							ImprovedStrategyBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet,
							ImprovedBranchBoundGraphBipartitioning>();
	this->run_partitioner<	Pheet::WithScheduler<BasicScheduler>,
							ImprovedBranchBoundGraphBipartitioning>();

#endif

#endif
		/*
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPriorityScheduler, 64>, DynamicUpperBoundFifoStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningUpperBoundStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningUpperLowerBoundStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPrioritySchedulerSlightlyLongerQueues, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPrioritySchedulerSlightlyLongerQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningUpperLowerBoundStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningUpperLowerBoundStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
	//	this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapMultiStealPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstBestStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		this->run_partitioner<ImprovedBranchBoundGraphBipartitioning<ArrayListHeapMultiStealPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<ArrayListHeapMultiStealPriorityScheduler, 64>, 64 > >();
		this->run_partitioner<ImprovedBranchBoundGraphBipartitioning<DefaultBasicScheduler, ImprovedBranchBoundGraphBipartitioningDeltaContribNVLogic<DefaultBasicScheduler, 64>, 64 > >();
*/
		/*
		// 1. All data-structures, with prio, DeltaNV logic, all strategies, all queue lengths except very long (very long only for single case)

		// ArrayListHeap, prio, DeltaNV, BestFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, DepthFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, LowerBound, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, FIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerShortQueues, 64>, AutoFifoStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, LIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerShortQueues, 64>, AutoLifoStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, LIFOFIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerShortQueues, 64>, AutoLifoFifoStrategy, 64 > >();

		// ArrayListHeap, prio, DeltaNV, BestFirst, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, DepthFirst, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, LowerBound, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, FIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPriorityScheduler, 64>, AutoFifoStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, LIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPriorityScheduler, 64>, AutoLifoStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, LIFOFIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPriorityScheduler, 64>, AutoLifoFifoStrategy, 64 > >();

		// ArrayListHeap, prio, DeltaNV, BestFirst, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, DepthFirst, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, LowerBound, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, FIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, AutoFifoStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, LIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, AutoLifoStrategy, 64 > >();
		// ArrayListHeap, prio, DeltaNV, LIFOFIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, AutoLifoFifoStrategy, 64 > >();

		// ArrayListHeap, prio, DeltaNV, BestFirst, 16
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerVeryLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListHeapPrioritySchedulerVeryLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();

		// ArrayList, prio, DeltaNV, BestFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, DepthFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, LowerBound, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, FIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerShortQueues, 64>, AutoFifoStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, LIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerShortQueues, 64>, AutoLifoStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, LIFOFIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerShortQueues, 64>, AutoLifoFifoStrategy, 64 > >();

		// ArrayList, prio, DeltaNV, BestFirst, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, DepthFirst, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, LowerBound, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, FIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, AutoFifoStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, LIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, AutoLifoStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, LIFOFIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, AutoLifoFifoStrategy, 64 > >();

		// ArrayList, prio, DeltaNV, BestFirst, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, DepthFirst, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, LowerBound, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, FIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerLongQueues, 64>, AutoFifoStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, LIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerLongQueues, 64>, AutoLifoStrategy, 64 > >();
		// ArrayList, prio, DeltaNV, LIFOFIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPrioritySchedulerLongQueues, 64>, AutoLifoFifoStrategy, 64 > >();


		// PrimitivePheetHeap, prio, DeltaNV, BestFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, DepthFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, LowerBound, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, FIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerShortQueues, 64>, AutoFifoStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, LIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerShortQueues, 64>, AutoLifoStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, LIFOFIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerShortQueues, 64>, AutoLifoFifoStrategy, 64 > >();

		// PrimitivePheetHeap, prio, DeltaNV, BestFirst, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, DepthFirst, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, LowerBound, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, FIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, AutoFifoStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, LIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, AutoLifoStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, LIFOFIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, AutoLifoFifoStrategy, 64 > >();

		// PrimitivePheetHeap, prio, DeltaNV, BestFirst, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, DepthFirst, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, LowerBound, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, FIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerLongQueues, 64>, AutoFifoStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, LIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerLongQueues, 64>, AutoLifoStrategy, 64 > >();
		// PrimitivePheetHeap, prio, DeltaNV, LIFOFIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePheetHeapPrioritySchedulerLongQueues, 64>, AutoLifoFifoStrategy, 64 > >();


		// Primitive, prio, DeltaNV, BestFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// Primitive, prio, DeltaNV, DepthFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// Primitive, prio, DeltaNV, LowerBound, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerShortQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// Primitive, prio, DeltaNV, FIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerShortQueues, 64>, AutoFifoStrategy, 64 > >();
		// Primitive, prio, DeltaNV, LIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerShortQueues, 64>, AutoLifoStrategy, 64 > >();
		// Primitive, prio, DeltaNV, LIFOFIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerShortQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerShortQueues, 64>, AutoLifoFifoStrategy, 64 > >();

		// Primitive, prio, DeltaNV, BestFirst, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// Primitive, prio, DeltaNV, DepthFirst, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// Primitive, prio, DeltaNV, LowerBound, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// Primitive, prio, DeltaNV, FIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, AutoFifoStrategy, 64 > >();
		// Primitive, prio, DeltaNV, LIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, AutoLifoStrategy, 64 > >();
		// Primitive, prio, DeltaNV, LIFOFIFO, 4
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<ArrayListPriorityScheduler, 64>, AutoLifoFifoStrategy, 64 > >();

		// Primitive, prio, DeltaNV, BestFirst, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// Primitive, prio, DeltaNV, DepthFirst, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// Primitive, prio, DeltaNV, LowerBound, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// Primitive, prio, DeltaNV, FIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerLongQueues, 64>, AutoFifoStrategy, 64 > >();
		// Primitive, prio, DeltaNV, LIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerLongQueues, 64>, AutoLifoStrategy, 64 > >();
		// Primitive, prio, DeltaNV, LIFOFIFO, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<PrimitivePrioritySchedulerLongQueues, 64>, AutoLifoFifoStrategy, 64 > >();

		// 2. ArrayListHeap, with prio, Delta logic, all strategies, 8

		// ArrayListHeap, prio, Delta, BestFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();
		// ArrayListHeap, prio, Delta, DepthFirst, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningDepthFirstStrategy, 64 > >();
		// ArrayListHeap, prio, Delta, LowerBound, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningLowerBoundStrategy, 64 > >();
		// ArrayListHeap, prio, Delta, FIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, AutoFifoStrategy, 64 > >();
		// ArrayListHeap, prio, Delta, LIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, AutoLifoStrategy, 64 > >();
		// ArrayListHeap, prio, Delta, LIFOFIFO, 1
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningDeltaLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, AutoLifoFifoStrategy, 64 > >();

		// 3. ArrayListHeap, with prio, improved logic, BestFirst strategy, 8

		// ArrayListHeap, prio, Improved, BestFirst, 8
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListHeapPrioritySchedulerLongQueues, ImprovedBranchBoundGraphBipartitioningImprovedLogic<ArrayListHeapPrioritySchedulerLongQueues, 64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();

		// 4. -, basic sched, all logics, -, .

		// -, basic, DeltaNV, -, -
		this->run_partitioner<ImprovedBranchBoundGraphBipartitioning<DefaultBasicScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<DefaultBasicScheduler, 64>, 64 > >();
		// -, basic, Delta, -, -
		this->run_partitioner<ImprovedBranchBoundGraphBipartitioning<DefaultBasicScheduler, ImprovedBranchBoundGraphBipartitioningDeltaLogic<DefaultBasicScheduler, 64>, 64 > >();
		// -, basic, Improved, -, -
		this->run_partitioner<ImprovedBranchBoundGraphBipartitioning<DefaultBasicScheduler, ImprovedBranchBoundGraphBipartitioningImprovedLogic<DefaultBasicScheduler, 64>, 64 > >();

		// 5. -, synch sched, DeltaNV logic, -, .

		// -, synch, DeltaNV, -, -
		this->run_partitioner<ImprovedBranchBoundGraphBipartitioning<DefaultSynchroneousScheduler, ImprovedBranchBoundGraphBipartitioningDeltaNVLogic<DefaultSynchroneousScheduler, 64>, 64 > >();
*/
		// All to slow compared to newer implementations
/*		this->run_partitioner<ImprovedBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningBasicLogic<64>, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningBasicLogic<64>, AutoFifoStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningBasicLogic<64>, AutoLifoStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningBasicLogic<64>, AutoLifoFifoStrategy, 64 > >();
		this->run_partitioner<ImprovedStrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, ImprovedBranchBoundGraphBipartitioningBasicLogic<64>, ImprovedStrategyBranchBoundGraphBipartitioningBestFirstStrategy, 64 > >();

		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerShortQueues, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<ArrayListPrioritySchedulerLongQueues, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();

		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitivePriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerShortQueues, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerLongQueues, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();
		// Too slow...
		// this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitivePrioritySchedulerVeryLongQueues, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitiveHeapPriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitiveHeapPrioritySchedulerShortQueues, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitiveHeapPrioritySchedulerLongQueues, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitiveHeapPrioritySchedulerVeryLongQueues, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningBestFirstStrategy > >();

		this->run_partitioner<BranchBoundGraphBipartitioning<PrimitivePriorityScheduler, BasicLowerBound, BasicNextVertex> >();
		this->run_partitioner<BranchBoundGraphBipartitioning<PrimitiveHeapPriorityScheduler, BasicLowerBound, BasicNextVertex> >();
		this->run_partitioner<BranchBoundGraphBipartitioning<FallbackPriorityScheduler, BasicLowerBound, BasicNextVertex> >();
		this->run_partitioner<BranchBoundGraphBipartitioning<DefaultMixedModeScheduler, BasicLowerBound, BasicNextVertex> >();
		this->run_partitioner<BranchBoundGraphBipartitioning<DefaultBasicScheduler, BasicLowerBound, BasicNextVertex> >();
		this->run_partitioner<BranchBoundGraphBipartitioning<DefaultSynchroneousScheduler, BasicLowerBound, BasicNextVertex> >();

		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitivePriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningAutoStrategy<FifoStrategy> > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitiveHeapPriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningAutoStrategy<FifoStrategy> > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitivePheetHeapPriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningAutoStrategy<FifoStrategy> > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitiveSortedArrayHeapPriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningAutoStrategy<FifoStrategy> > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningAutoStrategy<FifoStrategy> > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitiveHeapPriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningAutoStrategy<LifoStrategy> > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<PrimitiveHeapPriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningAutoStrategy<LifoFifoStrategy> > >();
		this->run_partitioner<StrategyBranchBoundGraphBipartitioning<ArrayListPriorityScheduler, BasicLowerBound, BasicNextVertex, BranchBoundGraphBipartitioningAutoStrategy<LifoFifoStrategy> > >();
		*/
}

}

