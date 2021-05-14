#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int init_under_malloc() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 5);
    if (ret != 0) { return ret; }

    buf -= 8;
    for (int i = 0; i < 8; i++) {
        if (buf[i] != 0x6b) {
            return 1;
        }
    }

    buf += 8;
    for (int i = 0; i < 10; i++) {
        if (buf[i] != 0) {
            return 1;
        }
    }

    buf -= 8;
    free(buf);
    return 0;
}

int main() {
    return init_under_malloc();
}