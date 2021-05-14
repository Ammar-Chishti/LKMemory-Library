#include <stdio.h>
#include <stdlib.h>
#include "write_report.h"
#include "../../include/lkmalloc.h"

int bad_free_report() {
    // It doesn't matter if lkf_approx is enabled when we free, we should still see it in the report
    char* buf = NULL;
    int ret = lkmalloc(10, (void*) &buf, 0);
    buf += 5;
    ret = lkfree((void*) &buf, 0);

    char* approx_free = NULL;
    ret = lkmalloc(10, (void*) &approx_free, 0);
    approx_free += 5;
    ret = lkfree((void*) &approx_free, 1);
    return write_report(4, "/home/achishti/hw2-achishti/tests/lkreports/bad_free_report.txt");
}

int main() {
    return bad_free_report();
}