#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int bad_free() {
    char *buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    buf += 5;
    ret = lkfree((void*) &buf, 0);
    return ret;  // Should return non-zero value
}

int main() {
    return bad_free();
}