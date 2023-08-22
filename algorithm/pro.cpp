#include "stdio.h"
#include "string.h"

int swap(char* p, int beg, int end)
{
    char tmp = '\0';

    for (-- end;beg <= end;++beg, --end) {
        tmp = p[end];
        p[end] = p[beg];
        p[beg] = tmp;
        //printf("ret is:%s; beg:%d; end:%d\n", p, beg, end);
    }

    return 0;
}

int reverse_string(char* p)
{
    //i am a cat
    //cat a am  i
    //tac a ma i
    //cat a am i
    int beg = 0;
    int last = 0;
    int end = strlen(p);
    swap(p, beg, end);   

    for (;beg < end; ++ beg) {
        if (p[beg] == ' ' && last != beg) {
            swap(p, last, beg);
            last = beg + 1;
        } else if(p[beg] == ' ' && last == beg) {
            last = beg + 1;
            continue;
        }
    }
    if (beg > end) {
        swap(p, last, beg);
    }

    return 0;
}

int main() 
{
    char p[] = "i am a cat";
    reverse_string(p);
    printf("ret is:%s\n", p);

    return 0;
}
