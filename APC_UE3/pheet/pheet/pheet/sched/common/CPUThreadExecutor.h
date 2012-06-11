/*
 * CPUThreadExecutor.h
 *
 *  Created on: 13.04.2011
 *      Author: Martin Wimmer
 *     License: Boost Software License 1.0 (BSL1.0)
 */

#ifndef CPUTHREADEXECUTOR_H_
#define CPUTHREADEXECUTOR_H_

#include <pthread.h>
#include <vector>
#include <iostream>

namespace pheet {

template <class Executor>
void* execute_cpu_thread(void *param);

template <class T>
class CPUThreadExecutor {
public:
	CPUThreadExecutor(T* work);
	~CPUThreadExecutor();

	void run();
	void join();

private:
	void execute();

	T* work;
	pthread_t thread;

	template <class Executor>
	friend void* execute_cpu_thread(void* param);
};

template <class T>
CPUThreadExecutor<T>::CPUThreadExecutor(T* work)
: work(work) {

}

template <class T>
CPUThreadExecutor<T>::~CPUThreadExecutor() {

}

template <class T>
void CPUThreadExecutor<T>::run() {
//	std::cout << "calling run" << std::endl;
	pthread_create(&thread, NULL, execute_cpu_thread<T>, work);
}

template <class T>
void CPUThreadExecutor<T>::join() {
	pthread_join(thread, NULL);
}

template <class T>
void *execute_cpu_thread(void *param) {
//	std::cout << "exec cpu thread " << param << std::endl;
	T* exec = (T*) param;
	exec->run();
	return NULL;
}

}

#endif /* CPUTHREADEXECUTOR_H_ */
