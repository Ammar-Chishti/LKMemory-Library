#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int error_warn_approx_free() {
    char *buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    buf += 5;
    ret = lkfree((void*) &buf, 11);
    return ret;  // Should return non-zero value with warning
}

int main() {
    return error_warn_approx_free();
}