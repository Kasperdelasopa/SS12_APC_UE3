/*
 * CentralizedSchedulerPlace.h
 *
 *  Created on: May 29, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0
 */

#ifndef CENTRALIZEDSCHEDULERPLACE_H_
#define CENTRALIZEDSCHEDULERPLACE_H_

#include "../../settings.h"
#include "../common/CPUThreadExecutor.h"
#include "../common/FinishRegion.h"
#include "../common/PlaceBase.h"
#include "../../misc/atomics.h"
#include "../../misc/bitops.h"
#include "../../misc/type_traits.h"
#include "CentralizedSchedulerPerformanceCounters.h"

#include <functional>

namespace pheet {

struct CentralizedSchedulerPlaceStackElement {
	// Modified by local thread. Incremented when task is spawned, decremented when finished
	size_t num_spawned;

	// Only modified by other threads. Stolen tasks that were finished (including spawned tasks)
	size_t num_finished_remote;

	// Pointer to num_finished_remote of another thread (the one we stole tasks from)
	CentralizedSchedulerPlaceStackElement* parent;

	// Counter to update atomically when finalizing this element (ABA problem)
	size_t version;
};

template <class Place>
struct CentralizedSchedulerPlaceLevelDescription {
	Place** partners;
	procs_t num_partners;
	procs_t local_id;
	procs_t size;
	procs_t memory_level;
	procs_t global_id_offset;
};

template <class Pheet>
struct CentralizedSchedulerPlaceDequeItem {
	typedef CentralizedSchedulerPlaceDequeItem<Pheet> Self;
	CentralizedSchedulerPlaceDequeItem();

	typename Pheet::Environment::Place::Task* task;
	typename Pheet::Environment::Place::StackElement* stack_element;

	bool operator==(Self const& other) const;
	bool operator!=(Self const& other) const;
};

template <class Pheet>
CentralizedSchedulerPlaceDequeItem<Pheet>::CentralizedSchedulerPlaceDequeItem()
: task(NULL), stack_element(NULL) {

}

template <class Pheet>
bool CentralizedSchedulerPlaceDequeItem<Pheet>::operator==(Self const& other) const {
	return other.task == task;
}

template <class Pheet>
bool CentralizedSchedulerPlaceDequeItem<Pheet>::operator!=(Self const& other) const {
	return other.task != task;
}


template <class Pheet>
class nullable_traits<CentralizedSchedulerPlaceDequeItem<Pheet> > {
public:
	static CentralizedSchedulerPlaceDequeItem<Pheet> const null_value;
};

template <class Pheet>
CentralizedSchedulerPlaceDequeItem<Pheet> const nullable_traits<CentralizedSchedulerPlaceDequeItem<Pheet> >::null_value;

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
class CentralizedSchedulerPlace : public PlaceBase<Pheet> {
public:
	typedef CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold> Self;
	typedef Self Place;
	typedef CentralizedSchedulerPlaceLevelDescription<Self> LevelDescription;
	typedef typename Pheet::Backoff Backoff;
	typedef typename Pheet::Scheduler::Task Task;
	template <typename F>
		using FunctorTask = typename Pheet::Scheduler::template FunctorTask<F>;
	typedef CentralizedSchedulerPlaceStackElement StackElement;
	typedef CentralizedSchedulerPlaceDequeItem<Pheet> DequeItem;
	typedef TaskStorageT<Pheet, DequeItem> TaskStorage;
	typedef CentralizedSchedulerPerformanceCounters<Pheet, void/*, typename TaskStorage::PerformanceCounters*/> PerformanceCounters;
	typedef typename Pheet::Scheduler::InternalMachineModel InternalMachineModel;

	CentralizedSchedulerPlace(TaskStorage& task_storage, InternalMachineModel model, Place** places, procs_t num_places, typename Pheet::Scheduler::State* scheduler_state, PerformanceCounters& perf_count);
	CentralizedSchedulerPlace(TaskStorage& task_storage, LevelDescription* levels, procs_t num_initialized_levels, InternalMachineModel model, typename Pheet::Scheduler::State* scheduler_state, PerformanceCounters& perf_count);
	~CentralizedSchedulerPlace();

	void prepare_root();
	void join();

	procs_t get_id();

	static Self* get();

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

	procs_t get_distance(Self* other);

	void start_finish_region();
	void end_finish_region();

private:
	void initialize_levels();
	void grow_levels_structure();
	void run();
	void execute_task(Task* task, StackElement* parent);
	void main_loop();
	void wait_for_finish(StackElement* parent);

	void empty_stack();
	StackElement* create_non_blocking_finish_region(StackElement* parent);
	void signal_task_completion(StackElement* stack_element);
	void finalize_stack_element(StackElement* element, StackElement* parent, size_t version, bool local);

	TaskStorage& task_storage;

	InternalMachineModel machine_model;
	procs_t num_initialized_levels;
	procs_t num_levels;
	LevelDescription* levels;

	static size_t const stack_size;
	StackElement* stack;
	StackElement* current_task_parent;
	size_t stack_filled_left;
	size_t stack_filled_right;
	size_t stack_init_left;

	typename Pheet::Scheduler::State* scheduler_state;

	PerformanceCounters performance_counters;

	size_t preferred_queue_length;
	size_t max_queue_length;
	bool call_mode;

	CPUThreadExecutor<Self> thread_executor;

	static thread_local Self* local_place;

	template <class T>
		friend void* execute_cpu_thread(void* param);
};

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
size_t const CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::stack_size = 8192;

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
thread_local CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>* CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::local_place = NULL;


template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::CentralizedSchedulerPlace(TaskStorage& task_storage, InternalMachineModel model, Place** places, procs_t num_places, typename Pheet::Scheduler::State* scheduler_state, PerformanceCounters& perf_count)
: task_storage(task_storage),
  machine_model(model),
  num_initialized_levels(1), num_levels(find_last_bit_set(num_places)), levels(new LevelDescription[num_levels]),
  current_task_parent(nullptr),
  stack_filled_left(0), stack_filled_right(stack_size), stack_init_left(0),
  scheduler_state(scheduler_state),
  performance_counters(perf_count),
  preferred_queue_length(find_last_bit_set(num_places) << CallThreshold),
  max_queue_length(preferred_queue_length << 1),
  call_mode(false),
  thread_executor(this) {

	// This is the root task execution context. It differs from the others in that it reuses the existing thread instead of creating a new one

	performance_counters.total_time.start_timer();

	pheet_assert(num_places <= model.get_num_leaves());
	levels[0].global_id_offset = 0;
	levels[0].memory_level = model.get_memory_level();
	levels[0].size = num_places;
	levels[0].partners = places;
	levels[0].num_partners = 0;
	// We have to initialize this now, as the value is already used by performance counters during initialization
	levels[0].local_id = 0;

	initialize_levels();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::CentralizedSchedulerPlace(TaskStorage& task_storage, LevelDescription* levels, procs_t num_initialized_levels, InternalMachineModel model, typename Pheet::Scheduler::State* scheduler_state, PerformanceCounters& perf_count)
: task_storage(task_storage),
  machine_model(model),
  num_initialized_levels(num_initialized_levels), num_levels(num_initialized_levels + find_last_bit_set(levels[num_initialized_levels - 1].size >> 1)),
  levels(new LevelDescription[num_levels]),
  current_task_parent(nullptr),
  stack_filled_left(0), stack_filled_right(stack_size), stack_init_left(0),
  scheduler_state(scheduler_state),
  performance_counters(perf_count),
  preferred_queue_length(find_last_bit_set(levels[0].size) << CallThreshold),
  max_queue_length(preferred_queue_length << 1),
  call_mode(false),
  thread_executor(this) {

	memcpy(this->levels, levels, sizeof(LevelDescription) * num_initialized_levels);
	// We have to initialize this now, as the value is already used by performance counters during initialization
	this->levels[0].local_id = this->levels[num_initialized_levels - 1].global_id_offset;

	thread_executor.run();
}
/*
template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::CentralizedSchedulerPlace(std::vector<LevelDescription*> const* levels, std::vector<typename CPUHierarchy::CPUDescriptor*> const* cpus, typename Scheduler::State* scheduler_state, PerformanceCounters& perf_count)
: performance_counters(perf_count), stack_filled_left(0), stack_filled_right(stack_size), stack_init_left(0), num_levels(levels->size()), thread_executor(cpus, this), scheduler_state(scheduler_state), preferred_queue_length(find_last_bit_set((*levels)[0]->total_size - 1) << 4), max_queue_length(preferred_queue_length << 1), call_mode(false), task_storage(max_queue_length, performance_counters.num_steals, performance_counters.num_task_storage_pop_cas) {
	performance_counters.total_time.start_timer();

	stack = new StackElement[stack_size];
	this->levels = new LevelDescription[num_levels];
	procs_t local_id = 0;
	for(ptrdiff_t i = num_levels - 1; i >= 0; i--) {
		this->levels[i].partners = (*levels)[i]->partners;
		this->levels[i].num_partners = (*levels)[i]->num_partners;
		local_id += (*levels)[i]->local_id;
		this->levels[i].local_id = local_id;
		this->levels[i].total_size = (*levels)[i]->total_size;
	}

	performance_counters.finish_stack_blocking_min.add_value(stack_size);
	performance_counters.finish_stack_nonblocking_max.add_value(0);

	thread_executor.run();
}*/

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::~CentralizedSchedulerPlace() {
	if(get_id() == 0) {
		end_finish_region();
		// we can shut down the scheduler
		scheduler_state->current_state = 2;

		performance_counters.task_time.stop_timer();
		performance_counters.total_time.stop_timer();

		scheduler_state->state_barrier.barrier(1, levels[0].size);

		for(procs_t i = num_levels - 1; i >= 1; --i) {
			levels[i].partners[0]->join();
			delete levels[i].partners[0];
		}

		machine_model.unbind();
		local_place = nullptr;
	}
	delete[] stack;
	delete[] levels;
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::prepare_root() {
	machine_model.bind();
	stack = new StackElement[stack_size];

	scheduler_state->state_barrier.signal(0);

	pheet_assert(local_place == NULL);
	local_place = this;


	performance_counters.finish_stack_blocking_min.add_value(stack_size);
	performance_counters.finish_stack_nonblocking_max.add_value(0);

	scheduler_state->state_barrier.wait(0, levels[0].size);

	performance_counters.task_time.start_timer();
	start_finish_region();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::initialize_levels() {
	procs_t base_offset;
	procs_t size;

	pheet_assert(num_initialized_levels > 0);
	base_offset = levels[num_initialized_levels - 1].global_id_offset;
	size = levels[num_initialized_levels - 1].size;

	Place** places = levels[0].partners;

	while((size > 1) && (!machine_model.is_leaf())) {
		pheet_assert(machine_model.get_num_children() == 2);
		grow_levels_structure();

		InternalMachineModel child(machine_model.get_child(1));
		procs_t offset = child.get_node_offset();
		pheet_assert(offset > base_offset);
		if((offset - base_offset) < size) {
			levels[num_initialized_levels].size = size - (offset - base_offset);
			levels[num_initialized_levels].global_id_offset = offset;
			levels[num_initialized_levels].num_partners = offset - base_offset;
			levels[num_initialized_levels].partners = places + base_offset;
			levels[num_initialized_levels].memory_level = child.get_memory_level();

			places[offset] = new Place(task_storage, levels, num_initialized_levels + 1, child, scheduler_state, performance_counters);

			machine_model = machine_model.get_child(0);
			levels[num_initialized_levels].size = offset - base_offset;
			levels[num_initialized_levels].global_id_offset = base_offset;
			levels[num_initialized_levels].num_partners = size - (offset - base_offset);
			levels[num_initialized_levels].partners = places + offset;
			levels[num_initialized_levels].memory_level = machine_model.get_memory_level();

			size = offset - base_offset;
			++num_initialized_levels;
		}
		else {
			machine_model = machine_model.get_child(0);
		}
	}

	pheet_assert(levels[num_initialized_levels - 1].size == 1);
	procs_t global_id = levels[num_initialized_levels - 1].global_id_offset;
	// Level 0 is already initialized in the constructor
	for(procs_t i = 1; i < num_initialized_levels; ++i) {
		levels[i].local_id = global_id - levels[i].global_id_offset;
	}
	num_levels = num_initialized_levels;
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::grow_levels_structure() {
	if(num_initialized_levels == num_levels) {
		// We have allocated to little levels
		procs_t new_size = num_levels + find_last_bit_set(levels[num_levels - 1].size >> 1);
		pheet_assert(new_size > num_levels);
		LevelDescription* tmp = new LevelDescription[new_size];
		memcpy(tmp, levels, num_levels * sizeof(LevelDescription));
		delete[] levels;
		levels = tmp;
		num_levels = new_size;
	}
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::join() {
	thread_executor.join();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>* CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::get() {
	return local_place;
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
procs_t
CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::get_id() {
	return levels[0].local_id;
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::run() {
	local_place = this;
	initialize_levels();
	stack = new StackElement[stack_size];

	scheduler_state->state_barrier.signal(0);

	performance_counters.total_time.start_timer();
	performance_counters.finish_stack_blocking_min.add_value(stack_size);
	performance_counters.finish_stack_nonblocking_max.add_value(0);

	scheduler_state->state_barrier.wait(0, levels[0].size);

	// Make sure the original initialization by all places is visible
	MEMORY_FENCE();

	main_loop();

	scheduler_state->state_barrier.barrier(1, levels[0].size);
	local_place = NULL;

	// Join all partners that this thread created
	for(procs_t i = num_levels - 1; i >= 1; --i) {
		if(levels[i].global_id_offset != levels[i-1].global_id_offset) {
			break;
		}
		levels[i].partners[0]->join();
		delete levels[i].partners[0];
	}

	performance_counters.total_time.stop_timer();

	// Now we can safely finish execution
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::execute_task(Task* task, StackElement* parent) {
	if(parent < stack || (parent >= (stack + stack_size))) {
		// to prevent thrashing on the parent finish block (owned by another thread), we create a new finish block local to the thread

		// Create new stack element for finish
		parent = create_non_blocking_finish_region(parent);
	}

	// Store parent (needed for spawns inside the task)
	current_task_parent = parent;

	// Execute task
	performance_counters.task_time.start_timer();
	(*task)();
	performance_counters.task_time.stop_timer();

	// Signal that we finished executing this task
	signal_task_completion(parent);
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::main_loop() {
	DequeItem di = task_storage.pop();
	while(true) {

		while(di.task != NULL) {
//			performance_counters.num_dequeued_tasks.incr();

			// Warning, no distinction between locally spawned tasks and remote tasks
			// But this makes it easier with the finish construct, etc.
			// Otherwise we would have to empty our deque on the next finish call
			// which is bad for balancing
			execute_task(di.task, di.stack_element);
			delete di.task;
			di = task_storage.pop();
		}
		{Backoff bo;
			do {
				if(scheduler_state->current_state >= 2) {
					return;
				}
				bo.backoff();
				di = task_storage.pop();
			}while(di.task == nullptr);
		}
	}
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::wait_for_finish(StackElement* parent) {
	DequeItem di = task_storage.pop();
	while(true) {
		while(di.task != NULL) {
//			performance_counters.num_dequeued_tasks.incr();

			// Warning, no distinction between locally spawned tasks and remote tasks
			// But this makes it easier with the finish construct, etc.
			// Otherwise we would have to empty our deque on the next finish call
			// which is bad for balancing
			execute_task(di.task, di.stack_element);
			delete di.task;
			if(parent->num_spawned == parent->num_finished_remote + 1) {
				return;
			}
			di = task_storage.pop();
		}
		{Backoff bo;
			do {
				if(parent->num_finished_remote + 1 == parent->num_spawned) {
					return;
				}
				bo.backoff();
				di = task_storage.pop();
			}while(di.task == nullptr);
		}
	}
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
typename CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::StackElement*
CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::create_non_blocking_finish_region(StackElement* parent) {
	performance_counters.num_non_blocking_finish_regions.incr();

	// Perform cleanup on finish stack
	empty_stack();

	pheet_assert(stack_filled_left < stack_filled_right);

	stack[stack_filled_left].num_finished_remote = 0;
	// As we create it out of a parent region that is waiting for completion of a single task, we can already add this one task here
	stack[stack_filled_left].num_spawned = 1;
	stack[stack_filled_left].parent = parent;

	if(stack_filled_left >= stack_init_left/* && stack_filled_left < stack_init_right*/) {
		stack[stack_filled_left].version = 0;
		++stack_init_left;
	}
	else {
		++(stack[stack_filled_left].version);
	}
	pheet_assert((stack[stack_filled_left].version & 1) == 0);

	++stack_filled_left;
	performance_counters.finish_stack_nonblocking_max.add_value(stack_filled_left);

	return &(stack[stack_filled_left - 1]);
}

/*
 * empty stack but not below limit
 */
template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::empty_stack() {
	while(stack_filled_left > 0) {
		size_t se = stack_filled_left - 1;
		if(stack[se].num_spawned == stack[se].num_finished_remote
				&& (stack[se].version & 1)) {
		//	finalize_stack_element(&(stack[se]), stack[se].parent);

			stack_filled_left = se;

			// When parent is set to NULL, some thread is finalizing/has finalized this stack element (otherwise we would have an error)
			// Actually we have to check before whether parent has already been set to NULL, or we might have a race
		//	pheet_assert(stack[stack_filled_left].parent == NULL);
		}
		else {
			break;
		}
	}
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::signal_task_completion(StackElement* stack_element) {
	performance_counters.num_completion_signals.incr();
	StackElement* parent = stack_element->parent;
	size_t version = stack_element->version;

	bool local = stack_element >= stack && (stack_element < (stack + stack_size));
	if(local) {
		--(stack_element->num_spawned);

		// Memory fence is unfortunately required to guarantee that some thread finalizes the stack_element
		// TODO: prove that the fence is enough
		MEMORY_FENCE();
	}
	else {
		// Increment num_finished_remote of parent
		SIZET_ATOMIC_ADD(&(stack_element->num_finished_remote), 1);
	}
	if(stack_element->num_spawned == stack_element->num_finished_remote) {
		finalize_stack_element(stack_element, parent, version, local);
	}
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
inline void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::finalize_stack_element(StackElement* element, StackElement* parent, size_t version, bool local) {
	if(parent != NULL) {
		// We have to check if we are local too!
		// (otherwise the owner might already have modified element, and then num_spawned might be 0)
		// Rarely happens, but it happens!
		if(local && element->num_spawned == 0) {
			pheet_assert(element >= stack && element < (stack + stack_size));
			// No tasks processed remotely - no need for atomic ops
		//	element->parent = NULL;
			++(element->version);
			performance_counters.num_chained_completion_signals.incr();
			signal_task_completion(parent);
		}
		else {
			if(SIZET_CAS(&(element->version), version, version + 1)) {
				performance_counters.num_chained_completion_signals.incr();
				performance_counters.num_remote_chained_completion_signals.incr();
				signal_task_completion(parent);
			}
		}
	}
/*	else {
		// Root element - we can shut down the scheduler
		scheduler_state->current_state = 2;
	}*/
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::start_finish_region() {
	performance_counters.task_time.stop_timer();
	performance_counters.num_finishes.incr();

	// Perform cleanup on left side of finish stack
	empty_stack();

	pheet_assert(stack_filled_left < stack_filled_right);
	--stack_filled_right;
	performance_counters.finish_stack_blocking_min.add_value(stack_filled_right);

	stack[stack_filled_right].num_finished_remote = 0;
	// We add 1 to make sure finishes are not propagated to the parent (as we wait manually and then execute a continuation)
	stack[stack_filled_right].num_spawned = 1;
	stack[stack_filled_right].parent = current_task_parent;

	current_task_parent = stack + stack_filled_right;
	performance_counters.task_time.start_timer();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::end_finish_region() {
	performance_counters.task_time.stop_timer();
	pheet_assert(current_task_parent == &(stack[stack_filled_right]));

//	if(current_task_parent->num_spawned > current_task_parent->num_finished_remote + 1) {
		// There exist some non-executed or stolen tasks

		// Process other tasks until this task has been finished
		wait_for_finish(current_task_parent);
//	}

	// Restore old parent
	current_task_parent = stack[stack_filled_right].parent;

	// Remove stack element
	++stack_filled_right;
	performance_counters.task_time.start_timer();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
template<class CallTaskType, typename ... TaskParams>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::finish(TaskParams&& ... params) {
	start_finish_region();

	call<CallTaskType>(std::forward<TaskParams&&>(params) ...);

	end_finish_region();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
template<typename F, typename ... TaskParams>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::finish(F&& f, TaskParams&& ... params) {
	start_finish_region();

	call(f, std::forward<TaskParams&&>(params) ...);

	end_finish_region();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
template<class CallTaskType, typename ... TaskParams>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::spawn(TaskParams&& ... params) {
	performance_counters.num_spawns.incr();

	size_t limit = call_mode?preferred_queue_length:max_queue_length;
	if(task_storage.get_length() >= limit) {
		call_mode = true;
		performance_counters.num_spawns_to_call.incr();
		call<CallTaskType>(std::forward<TaskParams&&>(params) ...);
	}
	else {
		call_mode = false;
		performance_counters.num_actual_spawns.incr();

		CallTaskType* task = new CallTaskType(params ...);
		pheet_assert(current_task_parent != NULL);
		++(current_task_parent->num_spawned);
		DequeItem di;
		di.task = task;
		di.stack_element = current_task_parent;
		task_storage.push(di);
	}
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
template<typename F, typename ... TaskParams>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::spawn(F&& f, TaskParams&& ... params) {
	performance_counters.num_spawns.incr();

	size_t limit = call_mode?preferred_queue_length:max_queue_length;
	if(task_storage.get_length() >= limit) {
		call_mode = true;
		performance_counters.num_spawns_to_call.incr();
		call(f, std::forward<TaskParams&&>(params) ...);
	}
	else {
		call_mode = false;
		performance_counters.num_actual_spawns.incr();

		auto bound = std::bind(f, params ...);

		FunctorTask<decltype(bound)>* task = new FunctorTask<decltype(bound)>(bound);
		pheet_assert(current_task_parent != NULL);
		++(current_task_parent->num_spawned);
		DequeItem di;
		di.task = task;
		di.stack_element = current_task_parent;
		task_storage.push(di);
	}
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
template<class CallTaskType, typename ... TaskParams>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::call(TaskParams&& ... params) {
	performance_counters.num_calls.incr();
	// Create task
	CallTaskType task(std::forward<TaskParams&&>(params) ...);
	// Execute task
	task();
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
template<typename F, typename ... TaskParams>
void CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::call(F&& f, TaskParams&& ... params) {
	performance_counters.num_calls.incr();
	// Execute task
	f(std::forward<TaskParams&&>(params) ...);
}

template <class Pheet, template <class P, typename T> class TaskStorageT, uint8_t CallThreshold>
procs_t CentralizedSchedulerPlace<Pheet, TaskStorageT, CallThreshold>::get_distance(Self* other) {
	if(other == this) {
		return 0;
	}

	procs_t offset = std::max(levels[num_levels - 1].memory_level, other->levels[other->num_levels - 1].memory_level);
	procs_t i = min(num_levels - 1, other->num_levels - 1);
	while(levels[i].global_id_offset != other->levels[i].global_id_offset) {
		pheet_assert(i > 0);
		--i;
	}
	pheet_assert(levels[i].memory_level <= offset);
	return offset - levels[i].memory_level;
}
}

#endif /* CENTRALIZEDSCHEDULERPLACE_H_ */
