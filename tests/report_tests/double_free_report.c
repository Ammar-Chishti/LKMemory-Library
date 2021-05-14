#include <stdio.h>
#include <stdlib.h>
#include "write_report.h"
#include "../../include/lkmalloc.h"

int double_free_report() {
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    ret = lkfree((void*) &buf, 0);
    ret = lkfree((void*) &buf, 0);
    return write_report(16, "/home/achishti/hw2-achishti/tests/lkreports/double_free_report.txt");
}

int main() {
    return double_free_report();
}