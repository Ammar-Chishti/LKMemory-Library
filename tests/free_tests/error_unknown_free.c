#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int error_unknown_free() {
    char* buf = NULL;
    int ret = lkfree((void*) &buf, 4);
    return ret; // Should be non-zero val with warning
}

int main() {
    return error_unknown_free();
}