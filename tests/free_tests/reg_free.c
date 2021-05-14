#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int reg_free() {
    char *buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    ret = lkfree((void*) &buf, 0);
    return ret;  // Should be zero
}

int main() {
    return reg_free();
}