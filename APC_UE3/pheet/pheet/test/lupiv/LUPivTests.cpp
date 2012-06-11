/*
 * LUPivTests.cpp
 *
 *  Created on: Dec 20, 2011
 *      Author: Martin Wimmer
 *	   License: Boost Software License 1.0 (BSL1.0)
 */

#include "LUPivTests.h"
#include "Simple/SimpleLUPiv.h"
#include "LocalityStrategy/LocalityStrategyLUPiv.h"

#include <pheet/pheet.h>
#include <pheet/sched/Basic/BasicScheduler.h>
#include <pheet/sched/Finisher/FinisherScheduler.h>
#include <pheet/models/MachineModel/HWLoc/HWLocSMTMachineModel.h>

#include <iostream>

namespace pheet {

LUPivTests::LUPivTests() {

}

LUPivTests::~LUPivTests() {

}

void LUPivTests::run_test() {
#ifdef LUPIV_TEST
	std::cout << "----" << std::endl;

	this->run_kernel<	Pheet,
						LocalityStrategyLUPiv>();
	this->run_kernel<	Pheet::WithMachineModel<HWLocSMTMachineModel>,
						LocalityStrategyLUPiv>();
	this->run_kernel<	Pheet,
						SimpleLUPiv>();
	this->run_kernel<	Pheet::WithScheduler<BasicScheduler>,
						SimpleLUPiv>();
	this->run_kernel<	Pheet::WithScheduler<FinisherScheduler>,
						SimpleLUPiv>();
#endif
}

}
