//selib is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//	author chenokay@gmail.com
//

#include <new>
#include <pthread.h>
#include "resource_stack.h"
#include "log_print.h"

namespace SELIB_UTIL {

// process elements in stack with multi-thread just like epool
// flollowing is the model:
//
//                              || ele_1 ||
//                              || ele_2 ||
//  PRODUCER_STACK:             || ele_3 ||
//                              || ele_4 ||
//                              || ele_5 ||
//                    _         ||  ...  ||           _            _
//                    //|       ||_______||         |\            |\
//                   read            /|\            write        write
//                   //              read             \              \
//              _________         ____|_____      _____\___      _____\____ 
//              |        |       |        |      |        |      |        |
//     THREADS: |thread_1|  ...  |thread_n|      |thread_1| ...  |thread_n|
//              |________|       |________|      |________|      |________|
//                   \               |                /              /             
//                    write         write           read           read         
//                     \             |              /              /          
//                     _\|          \|/           |/_            |/_              
//
//                              || ele_1 ||
//                              || ele_2 ||
//  CONSUMER_STACK:             || ele_3 ||
//                              || ele_4 ||
//                              || ele_5 ||
//                              ||  ...  ||
//                              ||_______||
// use case:
//
// class MyStruct {
//	...
// }
//
// class MyTest : public PoolMgr<MyStruct> {
//		virtual int ele_process(MyStruct * ms)
//		{
//			...
//		}
// }
//
// // main function
// 
// MyTest mt;
// // init thread number and pool size
// mt.init(thread_size, pool_size);
// // start processing with threads
// mt.start();
// // push elements in threads 
// mt.push(MyStruct);
// // wait, wait 5 seconds when without data
// mt.wait();
//

template <class POOL_ELE>
class PoolMgr {

public:
	PoolMgr() : _producer_pool(NULL), _consumer_pool(NULL), 
	_thread_ids(NULL), _th_size(0), _alive(false), _p_size(0) {}
	virtual ~PoolMgr() 
	{

		if (NULL != _thread_ids) {
			delete [] _thread_ids;
		}

		if (NULL != _producer_pool) {
			delete _producer_pool;
		}

		if (NULL != _consumer_pool) {
			delete _consumer_pool;
		}
	}

	// thread_size : thread count for process pool elements
	// pool_size : pool size to contain objects
	bool push(POOL_ELE * in)
	{
		return _consumer_pool->push(in);
	}

	POOL_ELE * pop()
	{
		POOL_ELE * p = _producer_pool->pop();

		// avoid the memory extention 
		if (NULL == p && _p_size > 0) {
			p = _producer_pool->force_pop();
			-- _p_size;
		} 

		return p;
	}

	int init(int thread_size, int pool_size)
	{
		_th_size = thread_size;
		_p_size = pool_size;
		_producer_pool = new (std::nothrow)ResourceStack<POOL_ELE, EleAllocator>();
		if (NULL == _producer_pool) {
			SELIB_WRITE_FATAL_LOG("fail to malloc producer stack");
			return -1;
		}

		_consumer_pool = new (std::nothrow)ResourceStack<POOL_ELE, EleAllocator>();
		if (NULL == _consumer_pool) {
			SELIB_WRITE_FATAL_LOG("fail to malloc consumer stack");
			return -1;
		}

		if (!_consumer_pool->create(_p_size)) {
			SELIB_WRITE_FATAL_LOG("fail to create consumer pool size");
			return -1;
		}

		_thread_ids = new(std::nothrow) pthread_t[_th_size];
		if (NULL == _thread_ids) {
			SELIB_WRITE_FATAL_LOG("fail to malloc thread ids");
			return -1;
		}

		return 0;
	}

	bool start()
	{
		_alive = true;
		for (int i= 0; i < _th_size; ++i) {
			if (0 != pthread_create(&_thread_ids[i], NULL, thread_fun, (void*)this)) {
				SELIB_WRITE_FATAL_LOG("fail to create thread");
			}
		}

		return 0;
	}

	bool wait()
	{
		for (int i= 0; i < _th_size; ++i) {
			if (0 != pthread_join(_thread_ids[i], NULL)) {
				SELIB_WRITE_FATAL_LOG("fail to join thread");
			}
		}
	}

	bool stop()
	{
		_alive = false;

		return true;
	}

	// thread process
	virtual int ele_process(POOL_ELE * pe, void *) = 0;

	virtual void* make_thread_data() = 0;

protected:

	struct EleAllocator {
		POOL_ELE * operator() () {
			return new (std::nothrow) POOL_ELE();
		}

		void destroy(POOL_ELE * p) {
			delete p;
		}
	};
	
	inline static void * thread_fun(void *param)
	{
		PoolMgr * pm = (PoolMgr *)param;
		void * th_data = pm->make_thread_data();

		if (NULL == th_data) {
			SELIB_WRITE_FATAL_LOG("fail to init redis pool");
			return NULL;
		}
		
		while(pm->_alive) {

			POOL_ELE * ce = pm->_consumer_pool->pop();
			if (NULL == ce) {
				pthread_yield();
				continue;
			}

			if (0 != pm->ele_process(ce, th_data)) {
				SELIB_WRITE_WARN_LOG("fail to process element");
			}

			pm->_producer_pool->push(ce);
		}
	}

	// memory buffer
	ResourceStack<POOL_ELE, EleAllocator> * _producer_pool;
	// data element thread will process
	ResourceStack<POOL_ELE, EleAllocator> * _consumer_pool;
	pthread_t * _thread_ids;
	int	_th_size;
	volatile bool _alive;
	volatile int _p_size;
};

}
