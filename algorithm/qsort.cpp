#include "stdio.h"

int partition(int* arr, int beg, int end)
{
    if (NULL == arr) {
        return -1;
    }
    if (end - beg < 1) {
        return 0;
    }
    int pivot = arr[beg];
    int replace_pos = beg;
    int b = beg;
    int e = end;
    bool on_end = true;
    for (;b < e;) {
        if (on_end) {
            if (arr[e] < pivot) {
                arr[replace_pos] = arr[e];
                on_end = false;
                replace_pos = e;
            } else {
                -- e;
            }
        } else {
            if (arr[b] > pivot) {
                arr[replace_pos] = arr[b];
                on_end = true;
                replace_pos = b;
            } else {
                ++ b;
            }
        }
    }
    arr[b] = pivot;

    return b;
}

int qsort(int* a, int beg, int end)
{
    if (end - beg < 2) {
        return -1;
    }
    int index = partition(a, beg, end);
    
    if (index - 1 - beg > 1) {
        qsort(a, beg, index - 1);
    }
    if (end - index -1 > 1) {
        qsort(a, index + 1, end);
    }

    return 0;
}

int main()
{
    int a[] = {38, 8, 16, 6, 79, 57, 24, 56, 2, 58, 4, 70, 45};

    qsort(a, 0, 12);
    for (int i =0; i < 13; ++ i) {
        printf("ret:%d\n", a[i]);
    }

    return 0;
}
