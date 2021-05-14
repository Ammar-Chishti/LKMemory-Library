#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int under_malloc() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 4);
    if (ret != 0) { return ret; }

    buf -= 8;
    for (int i = 0; i < 8; i++) {
        if (buf[i] != 0x6b) {
            return 1;
        }
    }
    free(buf);
    return 0;
}

int main() {
    return under_malloc();
}