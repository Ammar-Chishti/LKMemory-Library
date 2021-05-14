#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "write_report.h"
#include "../../include/lkmalloc.h"

int write_report(int flags, char* reportPath) {
    int fd = open(reportPath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Unable to create outfile in report tests");
        exit(EXIT_FAILURE);
    }
    int ret = lkreport(fd, flags);
    close(fd);
    return ret;     // Should be 0
}