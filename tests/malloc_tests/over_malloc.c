#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int over_malloc() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 2);
    if (ret != 0) { return ret; }
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
    return over_malloc();
}