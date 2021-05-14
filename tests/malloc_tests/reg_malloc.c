#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int reg_malloc() {
    char *buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    free(buf);
    return ret;
}

int main() {
    return reg_malloc();
}