#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int double_free() {
    char *buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    ret = lkfree((void *) &buf, 0);
    ret = lkfree((void *) &buf, 0);
    return ret;     // Should be non-zero value
}

int main() {
    return double_free();
}