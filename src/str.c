#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "str.h"

int str2int(const char *str,int *res){
    if(NULL == str || 0 == strlen(str)) return -1;
    int len = strlen(str);
    if(len > 12) return -1;
    int i = 0;
    while(i < len){
        if(str[i] < '0' || str[i] > '9') return 1;
        i++;
    }
    if(NULL != res) *res = atoi(str);
    return 0;
}
