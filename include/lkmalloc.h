#ifndef LKMALLOC_H
#define LKMALLOC_H

#include <stdio.h>
#include <gmodule.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include "binary_search_tree.h"

#define LKM_REG 0x0
#define LKM_INIT 0x1
#define LKM_OVER 0x2
#define LKM_UNDER 0x4
#define LKM_EXIST 0x8
#define LKM_REALLOC 0x10

#define LKF_REG 0x0
#define LKF_APPROX 0x1
#define LKF_WARN 0x2
#define LKF_UNKNOWN 0x4
#define LKF_ERROR 0x8

#define LKR_NONE 0x0
#define LKR_SERIOUS 0x1
#define LKR_MATCH 0x2
#define LKR_BAD_FREE 0x4
#define LKR_ORPHAN_FREE 0x8
#define LKR_DOUBLE_FREE 0x10
#define LKR_APPROX 0x20

#define lkmalloc(size, ptr, flags) lkmalloc_helper(size, ptr, flags, __FILE__, __LINE__, __func__)
#define lkfree(ptr, flags) lkfree_helper(ptr, flags, __FILE__, __LINE__, __func__)

// We need to create helper functions so that we can have access to the file name,
// function name, and line number
int lkmalloc_helper(u_int size, void** ptr, u_int flags, char* filename, int line_num, const char* func_name);
int lkfree_helper(void** ptr, u_int flags, char* filename, int line_num, const char* func_name);
int lkreport(int fd, u_int flags);

int lktest();

struct lkfree_match_malloc_args {
    void* free_ptr_passed;
    struct tree_record* malloc_match;
};

struct lkreport_args {
    int fd;
    int record_count;
};

struct timeval cur_time;

void exit_gracefully();

#endif