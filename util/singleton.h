
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

#ifndef SELIB_SINGLETON
#define SELIB_SINGLETON

#include <pthread.h>
#include <new>
#include "autolock.h"

namespace SELIB_UTIL {

// YourClass* instance = ProcessSingleton<YourClass>::get();
template <class TYPE>
class ProcessSingleton {
public:
    // Get the singleton instance.
    // Caller should NOT delete the returned object.
    static TYPE* get_instance() 
    {
        if (_instance == NULL) {
            autolock<TYPE> lock;
            if (_instance == NULL) {
                _instance = new (std::nothrow) TYPE();
            }
        }
        return _instance;
    }
	
	// bind in a instance which is generated outside
	static void bind(TYPE * inst)
	{
		autolock<TYPE> lock;
		if (_instance == NULL) {
			_instance = inst;
		}
	}

private:
    // disallow constructors
    ProcessSingleton();
    ~ProcessSingleton();
    ProcessSingleton(const ProcessSingleton&);
    ProcessSingleton& operator=(const ProcessSingleton&);

    static TYPE* _instance;
};

template <class TYPE>
TYPE* ProcessSingleton<TYPE>::_instance = NULL;

// thread Singleton class which manage instance:
// 1. only one instance in life time of thread
// 2. create and destroy instance automatically

typedef void *(*init_routine_t) (void *);

template<class TYPE>
class ThreadSingleton
{
	public:
		// get instance method 
		// call in the thread which use the instance pratically
		static TYPE* get_instance(init_routine_t init = NULL, void *arg = NULL)
		{
			pthread_once(&Key_once, ThreadSingleton::make_key);

			TYPE * ret = (TYPE*)pthread_getspecific(key);
			if (NULL == ret) {

				// if there a routine 
				if (NULL != init) {
					ret = (TYPE*)init(arg);
				} else {
					ret = new (std::nothrow)TYPE();
				}

				pthread_setspecific(key, (void *)ret);
			}

			return ret;
		}

	private:

		static void make_key()
		{
			pthread_key_create(&ThreadSingleton::key, ThreadSingleton::destructor);
		}

		// call this destructor as thread exit 
		static void destructor(void* obj)
		{
			delete ((TYPE*)obj);
			pthread_setspecific(ThreadSingleton::key, NULL); 
		}
		static pthread_key_t key;
		static pthread_once_t Key_once;

};

template <class TYPE>
pthread_key_t ThreadSingleton<TYPE>::key;

template <class TYPE>
pthread_once_t ThreadSingleton<TYPE>::Key_once = PTHREAD_ONCE_INIT;

}  // end namespace

using SELIB_UTIL::ProcessSingleton;
using SELIB_UTIL::ThreadSingleton;

#endif  
