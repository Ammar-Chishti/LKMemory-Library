#include <stdio.h>
#include <stdlib.h>
#include "../../include/lkmalloc.h"

int init_malloc() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 1);
    if (ret != 0) { return ret; }
    for (int i = 0; i < 10; i++) {
        if (buf[i] != 0) { 
            return 1;   // Return value of 1 is a test case error, not ERRNO failure
        }  
    }
    free(buf);
    return 0;
}

int main() {
    return init_malloc();
}