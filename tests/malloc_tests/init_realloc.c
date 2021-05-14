#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int init_realloc() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 1);
    ret = lkmalloc(15, (void*) &buf, 17);
    if (ret != 0) { return ret; }
    for (int i = 0; i < 15; i++) {
        if (buf[i] != 0) {
            return 1;
        }
    }
    free(buf);
    return 0;
}

int main() {
    return init_realloc();
}