#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int warn_approx_free() {
    char *buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    buf += 5;
    ret = lkfree((void*) &buf, 3);
    return ret;  // Should return 0 with warning message
}

int main() {
    return warn_approx_free();
}