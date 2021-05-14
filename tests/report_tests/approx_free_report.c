#include <stdio.h>
#include <stdlib.h>
#include "write_report.h"
#include "../../include/lkmalloc.h"

int approx_free_report() {
    char* one = NULL;
    char* two = NULL;
    char* three = NULL;

    int ret = lkmalloc(10, (void*) &one, 0);
    ret = lkmalloc(10, (void*) &two, 0);
    ret = lkmalloc(10, (void*) &three, 0);

    one += 5;
    two += 5;
    ret = lkfree((void*) &one, 1);
    ret = lkfree((void*) &two, 1);
    ret = lkfree((void*) &three, 0);

    // Only the first two records should show because they are approximate frees
    return write_report(32, "/home/achishti/hw2-achishti/tests/lkreports/approx_free_report.txt");
}

int main() {
    return approx_free_report();
}