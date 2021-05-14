#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int approx_free() {
    char *buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    buf += 5;
    ret = lkfree((void*) &buf, 1);
    return ret;  // Should return 0
}

int main() {
    return approx_free();
}