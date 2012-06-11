/*
 * StrategyScheduler.h
 *
 *  Created on: Mar 7, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef STRATEGYSCHEDULER2_H_
#define STRATEGYSCHEDULER2_H_

#include "StrategySchedulerPlace.h"
#include "StrategySchedulerPerformanceCounters.h"
#include "StrategySchedulerTaskStorageItem.h"

//#include "../../ds/StrategyTaskStorage/Local/LocalStrategyTaskStorage.h"
#include "../../ds/StrategyTaskStorage/LinkedList/LinkedListStrategyTaskStorage.h"
#include "../../ds/StrategyStealer/Dummy/DummyStrategyStealer.h"
#include "base_strategies/LifoFifo/LifoFifoBaseStrategy.h"

namespace pheet {

template <class Pheet>
struct StrategySchedulerState {
	StrategySchedulerState();

	uint8_t current_state;
	typename Pheet::Barrier state_barrier;
	typename Pheet::Scheduler::Task* startup_task;
};

template <class Pheet>
StrategySchedulerState<Pheet>::StrategySchedulerState()
: current_state(0), startup_task(NULL) {

}


template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
class StrategySchedulerImpl {
public:
	typedef typename Pheet::Backoff Backoff;
	typedef typename Pheet::MachineModel MachineModel;
	typedef BinaryTreeMachineModel<Pheet, MachineModel> InternalMachineModel;
	typedef StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT> Self;
	typedef SchedulerTask<Pheet> Task;
	template <typename F>
	using FunctorTask = SchedulerFunctorTask<Pheet, F>;
	typedef StrategySchedulerTaskStorageItem<Pheet, Task, StrategySchedulerPlaceStackElement> TaskStorageItem;
	typedef TaskStorageT<Pheet, TaskStorageItem> TaskStorage;
	typedef StealerT<Pheet, TaskStorage> Stealer;
	typedef StrategySchedulerPlace<Pheet, 4> Place;
	typedef StrategySchedulerState<Pheet> State;
	typedef FinishRegion<Pheet> Finish;
//	typedef StrategySchedulerPlaceDescriptor<Pheet> PlaceDesc;
//	template <class Strategy>
//	using TaskDesc = StrategySchedulerTaskDescriptor<Pheet, Strategy>;
	typedef BaseStrategyT<Pheet> BaseStrategy;
	typedef typename Place::PerformanceCounters PerformanceCounters;

	template <class NP>
	using BT = StrategySchedulerImpl<NP, TaskStorageT, StealerT, BaseStrategyT>;
	template <template <class, typename> class NewTS>
		using WithTaskStorage = StrategySchedulerImpl<Pheet, NewTS, StealerT, BaseStrategyT>;
	template <template <class, typename, typename> class NewTS>
		using WithPriorityTaskStorage = Self;


	/*
	 * Uses complete machine
	 */
	StrategySchedulerImpl();
	StrategySchedulerImpl(PerformanceCounters& performance_counters);

	/*
	 * Only uses the given number of places
	 * (Currently no direct support for oversubscription)
	 */
	StrategySchedulerImpl(procs_t num_places);
	StrategySchedulerImpl(procs_t num_places, PerformanceCounters& performance_counters);
	~StrategySchedulerImpl();

	static void print_name();

//	static void print_performance_counter_values(Place::PerformanceCounters performance_counters);

//	static void print_performance_counter_headers();

	static Place* get_place();
	static procs_t get_place_id();
	Place* get_place_at(procs_t place_id);

	template<class CallTaskType, typename ... TaskParams>
		void finish(TaskParams&& ... params);

	template<typename F, typename ... TaskParams>
		void finish(F&& f, TaskParams&& ... params);

	template<class CallTaskType, typename ... TaskParams>
		void call(TaskParams&& ... params);

	template<typename F, typename ... TaskParams>
		void call(F&& f, TaskParams&& ... params);

	template<class CallTaskType, typename ... TaskParams>
		void spawn(TaskParams&& ... params);

	template<typename F, typename ... TaskParams>
		void spawn(F&& f, TaskParams&& ... params);

	template<class CallTaskType, class Strategy, typename ... TaskParams>
		void spawn_s(Strategy s, TaskParams&& ... params);

	template<class Strategy, typename F, typename ... TaskParams>
		void spawn_s(Strategy s, F&& f, TaskParams&& ... params);
/*
	template<class CallTaskType, class Strategy, typename ... TaskParams>
		void spawn_s(Strategy&& s, TaskParams&& ... params);

	template<class Strategy, typename F, typename ... TaskParams>
		void spawn_s(Strategy&& s, F&& f, TaskParams&& ... params);
*/
	static char const name[];
	static procs_t const max_cpus;

private:
	InternalMachineModel machine_model;
	Place** places;
	procs_t num_places;

	State state;

	PerformanceCounters performance_counters;
};


template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
char const StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::name[] = "StrategyScheduler";

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
procs_t const StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::max_cpus = std::numeric_limits<procs_t>::max() >> 1;

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::StrategySchedulerImpl()
: num_places(machine_model.get_num_leaves()) {

	places = new Place*[num_places];
	places[0] = new Place(machine_model, places, num_places, &state, performance_counters);
	places[0]->prepare_root();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::StrategySchedulerImpl(typename Place::PerformanceCounters& performance_counters)
: num_places(machine_model.get_num_leaves()) {

	places = new Place*[num_places];
	places[0] = new Place(machine_model, places, num_places, &state, performance_counters);
	places[0]->prepare_root();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::StrategySchedulerImpl(procs_t num_places)
: num_places(num_places) {

	places = new Place*[num_places];
	places[0] = new Place(machine_model, places, num_places, &state, performance_counters);
	places[0]->prepare_root();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::StrategySchedulerImpl(procs_t num_places, typename Place::PerformanceCounters& performance_counters)
: num_places(num_places) {

	places = new Place*[num_places];
	places[0] = new Place(machine_model, places, num_places, &state, performance_counters);
	places[0]->prepare_root();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::~StrategySchedulerImpl() {
	delete places[0];
	delete[] places;
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::print_name() {
	std::cout << name << "<";
	Place::TaskStorage::print_name();
	std::cout /*<< ", " << (int)CallThreshold*/ << ">";
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
typename StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::Place* StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::get_place() {
	return Place::get();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
procs_t StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::get_place_id() {
	return Place::get()->get_id();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
typename StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::Place* StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::get_place_at(procs_t place_id) {
	pheet_assert(place_id < num_places);
	return places[place_id];
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<class CallTaskType, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::finish(TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->finish<CallTaskType>(std::forward<TaskParams&&>(params) ...);
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<typename F, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::finish(F&& f, TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->finish(f, std::forward<TaskParams&&>(params) ...);
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<class CallTaskType, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::spawn(TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->spawn<CallTaskType>(std::forward<TaskParams&&>(params) ...);
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<typename F, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::spawn(F&& f, TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->spawn(f, std::forward<TaskParams&&>(params) ...);
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<class CallTaskType, class Strategy, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::spawn_s(Strategy s, TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->spawn_s<CallTaskType>(std::move(s), std::forward<TaskParams&&>(params) ...);
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<class Strategy, typename F, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::spawn_s(Strategy s, F&& f, TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->spawn_s(std::move(s), f, std::forward<TaskParams&&>(params) ...);
}
/*
template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<class CallTaskType, class Strategy, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::spawn_s(Strategy&& s, TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->spawn_s<CallTaskType>(std::forward<Strategy&&>(s), std::forward<TaskParams&&>(params) ...);
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<class Strategy, typename F, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::spawn_s(Strategy&& s, F&& f, TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->spawn_s(std::forward<Strategy&&>(s), f, std::forward<TaskParams&&>(params) ...);
}*/

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<class CallTaskType, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::call(TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->call<CallTaskType>(std::forward<TaskParams&&>(params) ...);
}

template <class Pheet, template <class P, typename T> class TaskStorageT, template <class P, class TS> class StealerT, template <class P> class BaseStrategyT>
template<typename F, typename ... TaskParams>
inline void StrategySchedulerImpl<Pheet, TaskStorageT, StealerT, BaseStrategyT>::call(F&& f, TaskParams&& ... params) {
	Place* p = get_place();
	pheet_assert(p != NULL);
	p->call(f, std::forward<TaskParams&&>(params) ...);
}


template<class Pheet>
using StrategyScheduler = StrategySchedulerImpl<Pheet, /*LocalStrategyTaskStorage*/ LinkedListStrategyTaskStorage, DummyStrategyStealer, LifoFifoBaseStrategy>;

}

#endif /* STRATEGYSCHEDULER2_H_ */
