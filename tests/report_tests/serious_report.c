#include <stdio.h>
#include <stdlib.h>
#include "write_report.h"
#include "../../include/lkmalloc.h"

int serious_report() {
    char* one = NULL;
    char* two = NULL;
    char* three = NULL;

    int ret = lkmalloc(10, (void*) &one, 0);
    ret = lkmalloc(10, (void*) &two, 0);
    ret = lkmalloc(10, (void*) &three, 0);
    ret = lkfree((void*) &three, 0);    // Freeing one to check if records are updated properly
    return write_report(1, "/home/achishti/hw2-achishti/tests/lkreports/serious_report.txt");
}

int main() {
    return serious_report();
}