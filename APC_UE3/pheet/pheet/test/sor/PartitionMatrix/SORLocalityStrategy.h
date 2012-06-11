/*
 * SORLocalityStrategy.h
 *
 *  Created on: Jan 4, 2012
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef SORLOCALITYSTRATEGY_H_
#define SORLOCALITYSTRATEGY_H_

#include <pheet/pheet.h>
#include <pheet/sched/strategies/BaseStrategy.h>

namespace pheet {

template <class Scheduler>
class SORLocalityStrategy : public BaseStrategy<Scheduler> {
public:
	SORLocalityStrategy(typename Scheduler::TaskExecutionContext* last_owner, prio_t base_pop_priority, prio_t base_steal_priority);
	SORLocalityStrategy(SORLocalityStrategy& other);
	SORLocalityStrategy(SORLocalityStrategy&& other);
	virtual ~SORLocalityStrategy();

	virtual prio_t get_pop_priority(size_t task_id);
	virtual prio_t get_steal_priority(size_t task_id, typename Scheduler::StealerDescriptor& desc);
	virtual BaseStrategy<Scheduler>* clone();

private:
	typename Scheduler::TaskExecutionContext* last_owner;
	prio_t base_pop_priority;
	prio_t base_steal_priority;
};

template <class Scheduler>
SORLocalityStrategy<Scheduler>::SORLocalityStrategy(typename Scheduler::TaskExecutionContext* last_owner, prio_t base_pop_priority, prio_t base_steal_priority)
: last_owner(last_owner), base_pop_priority(base_pop_priority), base_steal_priority(base_steal_priority) {
	pheet_assert(base_pop_priority < 16);
	pheet_assert(base_steal_priority < 16);
}

template <class Scheduler>
SORLocalityStrategy<Scheduler>::SORLocalityStrategy(SORLocalityStrategy& other)
: last_owner(other.last_owner), base_pop_priority(other.base_pop_priority), base_steal_priority(other.base_steal_priority) {

}

template <class Scheduler>
SORLocalityStrategy<Scheduler>::SORLocalityStrategy(SORLocalityStrategy&& other)
: last_owner(other.last_owner), base_pop_priority(other.base_pop_priority), base_steal_priority(other.base_steal_priority) {

}

template <class Scheduler>
SORLocalityStrategy<Scheduler>::~SORLocalityStrategy() {

}

template <class Scheduler>
inline prio_t SORLocalityStrategy<Scheduler>::get_pop_priority(size_t task_id) {
	
	typename Scheduler::TaskExecutionContext* owner = Scheduler::get_context();
		if(last_owner==0)
	{
	  //printf("x1");
		return 1;
	}
	
	/*		  	if(last_owner!=owner)
		return 0;
	else
		return 1;
		*/
	//printf("Dist1: %d\n",owner->get_distance(last_owner));

	/*	if(owner->get_distance(last_owner)!=0)
	{
	//	printf("no1");
		return 0;
	}
	else
	{
	//	printf("yes1");
		return 1;
		}*/
	////
	//typename Scheduler::TaskExecutionContext* owner = Scheduler::get_context();
	procs_t max_d = owner->get_max_distance();
	procs_t d = owner->get_distance(last_owner);
	return ((max_d - d) << 4) + base_pop_priority;
}

template <class Scheduler>
inline prio_t SORLocalityStrategy<Scheduler>::get_steal_priority(size_t task_id, typename Scheduler::StealerDescriptor& desc) {
  	if(last_owner==0)
	{
	  //	printf("x2");
		return 1;
	}
/*		typename Scheduler::TaskExecutionContext* owner = Scheduler::get_context();
		if(last_owner!=owner)
		return 0;
	else
		return 1;
	*/
	//	printf("Dist2: %d",desc.get_distance_to(last_owner));
	/*
	if(desc.get_distance_to(last_owner)==0)
	{
		//	printf("no2\n");
		return 10;
	}
	else
	{
		//	printf("yes2\n");
		return 0;
		}*/
	////
	procs_t max_d = desc.get_max_distance();
	procs_t d = desc.get_distance_to(last_owner);
	return ((max_d - d) << 4) + base_steal_priority;
}

template <class Scheduler>
inline BaseStrategy<Scheduler>* SORLocalityStrategy<Scheduler>::clone() {
	return new SORLocalityStrategy<Scheduler>(*this);
}

}

#endif /* SORLOCALITYSTRATEGY_H_ */
