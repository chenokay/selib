#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algo_util.h"

int main()
{
    srand(time(NULL));
    int a[50];

    for (int i = 0; i < 10; ++i) {
        a[i] = rand() % 20;
        printf("%d ", a[i]);
    }

    insert_sort<int>(a, 10);

    printf("\n");
    for (int i = 0; i < 10; ++i) {
        printf("%d ", a[i]);
    }

    return 0;
}
