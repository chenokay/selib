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
//	reference from other engineer
//
#ifndef SELIB_TIMER
#define SELIB_TIMER

#include <time.h>
#include <errno.h>

namespace SELIB_UTIL {

class Timer
{
	public:
		Timer()
		{
			clear();
			restart();
		}

		void clear()
		{
			start_time.tv_sec = 0;
			start_time.tv_nsec = 0;
			check_point_time.tv_sec = 0;
			check_point_time.tv_nsec = 0;
		}

		void restart()
		{
			clock_gettime(CLOCK_MONOTONIC, &start_time);
			check_point_time = start_time;
		}
		
		long nsec_elapsed()
		{
			check();
			return (check_point_time.tv_sec - start_time.tv_sec) * 1000000000L +
				(check_point_time.tv_nsec - start_time.tv_nsec);
		}
		
		long usec_elapsed()
		{
			return nsec_elapsed() / 1000L;
		}
		
		long msec_elapsed()
		{
			return usec_elapsed() / 1000L;
		}
		
		long sec_elapsed()
		{
			return msec_elapsed() / 1000l;
		}
	private:
		
		void check()
		{
			clock_gettime(CLOCK_MONOTONIC, &check_point_time);
		}

		struct timespec start_time;  
		struct timespec check_point_time;  
};

}  

#endif
