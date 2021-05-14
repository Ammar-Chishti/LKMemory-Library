#include <stdio.h>
#include <stdlib.h>
#include "write_report.h"
#include "../../include/lkmalloc.h"

int orphan_free_report() {
    char* buf = NULL;
    lkfree((void*) &buf, 0);
    return write_report(8, "/home/achishti/hw2-achishti/tests/lkreports/orphan_free_report.txt");
}

int main() {
    return orphan_free_report();
}