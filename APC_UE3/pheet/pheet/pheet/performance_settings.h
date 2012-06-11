/*
 * performance_settings.h
 *
 *  Created on: 11.08.2011
 *   Author(s): Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef PHEET_PERFORMANCE_SETTINGS_H_
#define PHEET_PERFORMANCE_SETTINGS_H_

namespace pheet {

#ifndef PHEET_ALL_PERFORMANCE_COUNTERS
bool const pc_all = false;
#else
bool const pc_all = true;
#endif

bool const scheduler_count_tasks_at_level = pc_all | false;
bool const scheduler_count_steal_calls_per_thread = pc_all | false;
bool const scheduler_count_unsuccessful_steal_calls_per_thread = pc_all | false;
bool const scheduler_count_spawns = pc_all | false;
bool const scheduler_count_actual_spawns = pc_all | false;
bool const scheduler_count_spawns_to_call = pc_all | false;
bool const scheduler_count_calls = pc_all | false;
bool const scheduler_count_finishes = pc_all | false;
bool const scheduler_count_completion_signals = pc_all | false;
bool const scheduler_count_chained_completion_signals = pc_all | false;
bool const scheduler_count_remote_chained_completion_signals = pc_all | false;
bool const scheduler_count_non_blocking_finish_regions = pc_all | false;
bool const stealing_deque_count_steals = pc_all | false;
bool const stealing_deque_count_steal_calls = pc_all | false;
bool const stealing_deque_count_unsuccessful_steal_calls = pc_all | false;
bool const stealing_deque_count_pop_cas = pc_all | false;

bool const stealing_deque_count_dequeued_tasks = pc_all | false;
bool const stealing_deque_count_steal_executed_tasks = pc_all | false;

bool const scheduler_measure_total_time = pc_all | false;
bool const scheduler_measure_task_time = pc_all | false;
bool const scheduler_measure_sync_time = pc_all | false;
bool const scheduler_measure_idle_time = pc_all | false;
bool const scheduler_measure_steal_time = pc_all | false;
bool const scheduler_measure_queue_processing_time = pc_all | false;
bool const scheduler_measure_visit_partners_time = pc_all | false;
bool const scheduler_measure_wait_for_finish_time = pc_all | false;
bool const scheduler_measure_wait_for_coordinator_time = pc_all | false;

bool const task_storage_count_steals = pc_all | false;
bool const task_storage_count_unsuccessful_pops = pc_all | false;
bool const task_storage_count_successful_pops = pc_all | false;
bool const task_storage_count_unsuccessful_takes = pc_all | false;
bool const task_storage_count_successful_takes = pc_all | false;
bool const task_storage_count_unsuccessful_steals = pc_all | false;
bool const task_storage_count_successful_steals = pc_all | false;
bool const task_storage_count_size_pop = pc_all | false;
bool const task_storage_count_size_steal = pc_all | false;
bool const task_storage_measure_push_time = pc_all | false;
bool const task_storage_measure_pop_time = pc_all | false;
bool const task_storage_measure_clean_time = pc_all | false;
bool const task_storage_measure_create_control_block_time = pc_all | false;
bool const task_storage_measure_configure_successor_time = pc_all | false;
bool const task_storage_measure_heap_push_time = pc_all | false;
bool const task_storage_measure_put_time = pc_all | false;
bool const task_storage_measure_strategy_alloc_time = pc_all | false;
bool const task_storage_measure_steal_time = pc_all | false;
bool const task_storage_count_skipped_cleanups = pc_all | false;
bool const task_storage_measure_max_control_block_items = pc_all | 		false;
bool const task_storage_measure_max_heap_length = pc_all | 				false;

bool const scheduler_measure_finish_stack_nonblocking_max = pc_all | 	false;
bool const scheduler_measure_finish_stack_blocking_min = pc_all | 		false;

}

#endif /* PHEET_PERFORMANCE_SETTINGS_H_ */
