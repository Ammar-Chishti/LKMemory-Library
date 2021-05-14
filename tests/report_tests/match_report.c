#include <stdio.h>
#include <stdlib.h>
#include "write_report.h"
#include "../../include/lkmalloc.h"

int match_report() {
    char* one = NULL;
    char* two = NULL;
    char* doubleMalloc = NULL;
    char* orphanFree = NULL;

    // malloc and free one and two normally
    int ret = lkmalloc(10, (void*) &one, 0);
    ret = lkmalloc(10, (void*) &two, 0);
    ret = lkfree((void*) &one, 0);
    ret = lkfree((void*) &two, 0);

    // The orphan free should not show up in the report but the freeing of one of the mallocs should
    ret = lkmalloc(10, (void*) &doubleMalloc, 0);
    ret = lkmalloc(10, (void*) &doubleMalloc, 0);
    ret = lkfree((void*) &doubleMalloc, 0);
    ret = lkfree((void*) &orphanFree, 0);
    return write_report(2, "/home/achishti/hw2-achishti/tests/lkreports/match_report.txt");
}

int main() {
    return match_report();
}