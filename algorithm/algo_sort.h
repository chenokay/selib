template<class T>
void quick_sort_v1(T * ar, int len)
{
    T pivot = ar[0];
    int index_low = 0;
    int index_high = len;
    bool on_high= true;

    for (int i = 0; i < len - 1; ++i) {
        if (on_high) {
            -- index_high;
            if (ar[index_high] < pivot)  {
                ar[index_low] = ar[index_high];
                on_high = false;
            }
        } else {
            ++ index_low;
            if (ar[index_low] > pivot) {
                ar[index_high] = ar[index_low];
                on_high = true;
            }
        }
    }

    int mid = 0;
    if (on_high) {
        ar[index_low] = pivot; 
        mid = index_low;
    } else {
        ar[index_high] = pivot;
        mid = index_high;
    }

    if (mid+1 > 1) {
        quick_sort_v1<T>(ar, mid + 1);
    }

    if ((len - mid -1) > 1) {
        quick_sort_v1<T>(ar + mid + 1, len - mid -1);
    }
}

// TODO to debug
template<class T>
void quick_sort_v2(T * ar, int len)
{
	printf("-----------------------------\n");
	int index_low = 0;
	int index_high = len - 1;
	T pivot = ar[0];

	for (;index_low < index_high;) {

		//printf("index_low is:%d; index_high is:%d\n", index_low, index_high);
		while(ar[index_low] < pivot && index_low < index_high) {
			++ index_low;
		}

		while (ar[index_high] > pivot && index_low < index_high) {
			-- index_high;
		}

		//printf("index_low is:%d; index_high is:%d\n", index_low, index_high);

		// swap
		int tmp = ar[index_low];
		ar[index_low] = ar[index_high];
		ar[index_high] = tmp;

		if (index_low + 1 <= index_high) {
			++ index_low;
		}

		if (index_high - 1 >= index_low) {
			-- index_high;
		}

		for (int i = 0; i < len; ++i) {
			printf("%d ", ar[i]);
		}
		printf("\n");

	}

	int mid = 0;
	if (ar[index_low] > pivot) {
		mid = index_low;
	} else if(ar[index_high] < pivot) {
		mid = index_high + 1;
	} else {
		mid = index_low;
	}

	printf("mid is:%d\n", mid);

	quick_sort_v2<T>(ar, mid);
	quick_sort_v2<T>(ar + mid, len - mid);
}

template<class T>
int bin_find(T * arr, int len, int val)
{
	int mid = len / 2;

	if (len < 1) {
		return -1;
	}

	if (val == arr[mid]) {
		return mid;
	}

	if (val > arr[mid]) {
		return mid+ 1+bin_find<T>(arr + mid + 1, len - mid - 1, val);
	}

	if (val < arr[mid]) {
		return bin_find<T>(arr, mid, val);
	}
}

template<class T>
void insert_sort(T * arr, int len)
{
    for (int i = 1; i < len; ++i) {
        int j = i - 1;
        T key = arr[i];

        for (;j >= 0; --j) {

            if (key < arr[j]) {
                arr[j + 1] = arr[j];
            } else {
                break;
            }

        }
        arr[j+1] = key;
    }
}

template<class T>
void merge_sort(T * arr, int len)
{
    if (2 == len) {
        if (arr[0] > arr[1]) {
            arr[0] = arr[0] ^ arr[1];
            arr[1] = arr[0] ^ arr[1];
            arr[0] = arr[0] ^ arr[1];
        }
        return ;
    } else if (len < 2) {
        return ;
    } 
    int mid = len/2;

    merge_sort(arr, mid);
    merge_sort(arr + mid, len - mid);

    T a1[mid + 1];
    T a2[len - mid + 1];
    
    for (int i = 0;i < mid; ++i) {
        a1[i] = arr[i];
    }
    for (int i = mid;i < len; ++i) {
        a2[i-mid] = arr[i];
    }
    int i = 0;
    int j = 0;
    int k = 0;

    for (;i < mid && j < len - mid; ++k) {
        if (a1[i] < a2[j]) {
            arr[k] = a1[i];
            ++ i;
        } else {
            arr[k] = a2[j];
            ++ j;
        }
    }

    if (i < mid) {
        for (;i < mid; ++i, ++k) {
            arr[k] = a1[i];
        }
    }

    if (j < len - mid) {
        for (;j < len -mid; ++j, ++k) {
            arr[k] = a2[j];
        }
    }

 }

