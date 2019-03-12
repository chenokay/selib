#include "circle_cache.h"

int main()
{
	int v1 = 5;
	int v2 = 99;
	int v3 = 100;
	LruCache<int> lc;
	lc.init(10, 50);

	lc.put(9u, &v1);
	lc.put(8u, NULL);
	lc.put(7u, NULL);
	int * ret1 = lc.get(9u);
	printf("get value1:%d\n", *ret1);
	lc.put(6u, &v1);
	lc.put(11u, NULL);
	lc.put(20u, &v2);
	lc.put(22u, NULL);
	lc.put(93u, NULL);
	int * ret2 = lc.get(20u);
	printf("get value1:%d\n", *ret2);
	lc.put(97u, NULL);
	lc.put(98u, NULL);
	lc.put(6u, &v3);
	//ret2 = lc.get(6u);

	//printf("get value4:%d\n", *ret2);
	lc.debug_print();

	return 0;
}
