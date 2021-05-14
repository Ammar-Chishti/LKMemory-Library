#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int init_over_malloc() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 3);
    if (ret != 0) { return ret; }
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
    
    buf -= 10;
    free(buf);
    return 0;
}

int main() {
    return init_over_malloc();
}