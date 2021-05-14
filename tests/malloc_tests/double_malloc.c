#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int double_malloc() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    ret = lkmalloc(10, (void*) &buf, 0);
    free(buf);
    return ret;  // should return non zero value
}

int main() {
    return double_malloc();
}