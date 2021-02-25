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
//	chenokay@gmail.com
//
#ifndef SELIB_CPU_LOAD
#define SELIB_CPU_LOAD

#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "timer.h"
#include "singleton.h"

namespace SELIB_UTIL {

// monitor process cpu load with sleep
class CpuLoad
{
	public:
		static float get_cpu_load()
		{
			CpuLoad *cl = ProcessSingleton<CpuLoad>::get_instance();
			if (cl != NULL) {
				return cl->cpu_load();
			}

			return -1.0;
		}

		static int start()
		{
			CpuLoad *cl = ProcessSingleton<CpuLoad>::get_instance();
			if (cl != NULL) {
				return pthread_create(&(cl->_tid), NULL, CpuLoad::start_routine, cl);
			}

			return -1;
		}

		static int stop()
		{
			CpuLoad *cl = ProcessSingleton<CpuLoad>::get_instance();
			if (cl != NULL) {
				return cl->_stop();
			}

			return -1;
		}

		static int join()
		{
			CpuLoad *cl = ProcessSingleton<CpuLoad>::get_instance();
			if (cl != NULL) {
				pthread_join(cl->_tid, NULL);
			}

			return -1;
		}

		CpuLoad()
		{init();}
		~CpuLoad() { };

		int cpu_load()
		{
			return _cpu_delay;
		}
		bool run()
		{
			return _run;
		}
		int _stop()
		{
			_run = false;
			return 0;
		}
	private:
		unsigned int _seed;
		// avg cpu delay
		float _cpu_delay;
		// test
		bool _run;
		pthread_t _tid;

		CpuLoad(const CpuLoad&);
		CpuLoad& operator=(const CpuLoad&);

		int init()
		{
			_seed = time(NULL);
			_cpu_delay = 0;
			_run = true;
		}

		static void *start_routine(void *arg)
		{
			CpuLoad *cl = (CpuLoad*)arg;
			static int ARR_LEN = 128;
			static int MAX_SLEEP_US = 1000;

			float *arr = new float[ARR_LEN];
			uint32_t cur = 0;
			Timer tm;
			while (cl->run()) {
				int us = rand_r(&(cl->_seed)) % MAX_SLEEP_US;
				tm.restart();
				usleep(us);
				int consume_us = tm.usec_elapsed();

				arr[cur] = (consume_us - us) >= 0 ? (consume_us - us) : 0; 
				cur = (cur + 1) & (ARR_LEN - 1);

				if (cur == 0) {
					float total_consume  = 0;
					for (int i = 0; i < ARR_LEN; ++i) {
						total_consume += arr[i];
					}

					cl->_cpu_delay = total_consume/ARR_LEN;
				}
			}

			delete [] arr;
		}
};

}  

#endif
