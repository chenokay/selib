#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heap_sort.h"

int main()
{
    srand(time(NULL));
    int a[50] = {14,7,15,2,8,8,12,12,10,17};

    for (int i = 0; i < 20; ++i) {
        a[i] = rand() % 20;
        printf("%d ", a[i]);
    }

    HeapSort<int> hs;
    if(0 != hs.build(a, 20)) {
        printf("fail to build heap");
        return -1;
    }

    printf("\n");

    printf("max is:%d\n", *(hs.max()));
    printf("min is:%d\n", *(hs.min()));

    return 0;
}
