#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int exist_malloc() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 8);
    ret = lkmalloc(10, (void*) &buf, 8);
    if (ret == 0) { return 1; }
    free(buf);
    return 0;
}

int main() {
    return exist_malloc();
}