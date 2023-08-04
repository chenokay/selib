#include <stdio.h>
 #include <string.h>

 struct Pattern_t {
     char pattern[128];
     int skip[128];
     int len;
 };

int make_skip(Pattern_t* pat)
 {
     if (pat == NULL) {
         return -1;
     }
     pat->skip[0] = -1;
     for (int i = 1; i < pat->len; ++i) {
         // 以前一个位置skip作为计算起点
         int j = pat->skip[i - 1];
         for (;j >= 0;) {
             // 找到匹配模式，退出，后续基于此累加
             if (pat->pattern[j] == pat->pattern[i - 1]){
                 break;
             }
             // 当前skip下不匹配，使用上一个位置已经计算过的位置进行匹配
             j = pat->skip[j];
         }
         // 在匹配串基础上加1
         pat->skip[i] = j + 1;
     }

     return 0;
 }

 int main()
 {
     char* ptr = "ABACABABE";
     Pattern_t pt;
     snprintf(pt.pattern, 128, "%s", ptr);
     pt.len = strlen(pt.pattern);
     make_skip(&pt);

     for (int i = 0; i < pt.len; ++i) {
         printf("%c %d\n", pt.pattern[i], pt.skip[i]);
     }

     return 0;
 }
