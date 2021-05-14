#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int unknown_free() {
    char* buf = NULL;
    int ret = lkfree((void*) &buf, 4);
    return ret;   // should be non-zero value with warning
}

int main() {
    return unknown_free();
}