#include <stdio.h>
#include "cpu_switch_monitor.h"

using namespace SELIB_UTIL;

void *routine(void *arg)
{
	for (int j = 0; j < 50; ++j) {
		float total = 0;
		//usleep(1000000);
		for (int i = 0; i < 1000000000; ++i) {
			total += i;
			total /= i;
		}
	}
}

// g++ monitor/test/cpu_switch_monitor.cpp -I./util/ -I./monitor/ -lpthread -lrt
int main()
{
	CpuLoad::start();
	static const int THREADS = 500;

	pthread_t tid[THREADS];

	for (int i = 0; i < THREADS; ++i) {
		pthread_create(&(tid[i]), NULL, routine, NULL);
	}

	while(true) {
		printf("cpu load:%f\n", CpuLoad::get_cpu_load());
		usleep(1000000);
	}
	CpuLoad::stop();
	CpuLoad::join();

	for (int i = 0; i < THREADS; ++i) {
		pthread_join(tid[i], NULL);
	}

	return 0;
}

