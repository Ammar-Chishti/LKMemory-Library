#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stdio.h>
#include <gmodule.h>
#include <unistd.h>
#include <stdlib.h>

// key: ptr returned by malloc
// value: tree_record* to record of malloc
GTree* malloc_tree;

// key: tree_record* to record of malloc
// value: tree_record* to record of free
GTree* perfect_match_tree;

// key: tree_record* to record of malloc
// value: tree_record* to record of approx_free
GTree* approx_match_tree;

// key: void** ptr passed in
// val: tree_record* to record bad_free
GTree* bad_free_tree;

// key: void** ptr passed in
// val: tree_record* to record orphan_free
GTree* orphan_free_tree;

// key: void** ptr passed in
// val: tree_record* to record double_free
GTree* double_free_tree;


void init_trees();

void free_all_trees();


struct tree_record {
    int record_type;    // 0 for malloc, 1 for free
    char* filename;
    char* func_name;
    int line_num;
    long int seconds;
    long int microseconds;
    void** ptr_passed;
    int retval;
    long int size_or_flags;       // requested size if malloc or flags if free
    void* alloc_addr_returned;  // NULL if record_type is 1 else addr returned by malloc
    int is_paired;     // boolean int to let us know if a malloc record has been paired with a free. -1 if this record is free
    int is_approx;     // boolean int to let us know if this free record is an approx or an actual. -1 if this record is malloc
    int redzones;      // 0 means none, 1 means under, 2 means over, 3 means both. -1 if free record
};

struct tree_record* new_record(int record_type, char* filename, const char* func_name, int line_num,
                            long int seconds, long int microseconds, void** ptr_passed, int retval,
                            long int size_or_flags, void* alloc_addr_returned, int paired, int approx, int redzones);

char* tree_record_to_string(struct tree_record* record);

int free_record(struct tree_record* record);

void print_tree_record(struct tree_record* record);

#endif