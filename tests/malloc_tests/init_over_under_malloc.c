#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int init_over_under_malloc() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 7);
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

    buf += 10;
    for (int i = 0; i < 8; i++) {
        if (buf[i] != 0x5a) {
            return 1;
        }
    }
    buf -= 18;
    free(buf);
    return 0;
}

int main() {
    return init_over_under_malloc();
}