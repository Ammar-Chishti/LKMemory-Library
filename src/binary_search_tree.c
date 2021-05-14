#include "binary_search_tree.h"

gint treecmp(gconstpointer a, gconstpointer b) {
    if (a < b) {
        return -1;
    } else if (a > b) {
        return 1; 
    } else {
        return 0;
    }
}

void init_trees() {
    malloc_tree = g_tree_new(treecmp);
    perfect_match_tree = g_tree_new(treecmp);
    approx_match_tree = g_tree_new(treecmp);
    bad_free_tree = g_tree_new(treecmp);
    orphan_free_tree = g_tree_new(treecmp);
    double_free_tree = g_tree_new(treecmp);
}

struct tree_record* new_record(int record_type, char* filename, const char* func_name, int line_num,
                                long int seconds, long int microseconds, void** ptr_passed, int retval,
                                long int size_or_flags, void* alloc_addr_returned, int is_paired, int is_approx, int redzones) {
    
    struct tree_record* record = malloc(sizeof(struct tree_record));
    // if (record == NULL) { return NULL; }
    
    record->record_type = record_type;
    record->filename = malloc(strlen(filename));
    record->func_name = malloc(strlen(func_name));
    record->line_num = line_num;
    record->seconds = seconds;
    record->microseconds = microseconds;
    record->ptr_passed = ptr_passed;
    record->retval = retval;
    record->size_or_flags = size_or_flags;
    record->alloc_addr_returned = alloc_addr_returned;
    record->is_paired = is_paired;
    record->is_approx = is_approx;
    record->redzones = redzones;
    strcpy(record->filename, filename);
    strcpy(record->func_name, func_name);
    return record;
}

char* tree_record_to_string(struct tree_record* record) {
    char* buffer = malloc(getpagesize());
    sprintf(buffer, "%d,%s,%s,%d,%ld seconds %ld microseconds,%p,%d,", record->record_type, record->filename, 
    record->func_name, record->line_num, record->seconds, record->microseconds, record->ptr_passed, record->retval);

    if (record->record_type == 0) {
        sprintf(buffer + strlen(buffer), "%ld,%p\n", record->size_or_flags, record->alloc_addr_returned);
    } else {
        sprintf(buffer + strlen(buffer), "%ld,\n", record->size_or_flags);
    }
    return buffer;
}

void print_tree_record(struct tree_record* record) {
    printf("record_type: %d\n", record->record_type);
    printf("filename: %s\n", record->filename);
    printf("func_name: %s\n", record->func_name);
    printf("line_num: %d\n", record->line_num);
    printf("seconds: %ld\n", record->seconds);
    printf("microseconds: %ld\n", record->microseconds);
    printf("ptr_passed: %p\n", record->ptr_passed);
    printf("retval: %d\n", record->retval);
    printf("size_or_flags: %ld\n", record->size_or_flags);
    printf("alloc_addr_returned: %p\n", record->alloc_addr_returned);
    printf("is_paired: %d\n", record->is_paired);
    printf("is_approx: %d\n", record->is_approx);
    printf("redzone_val: %d\n", record->redzones);
    printf("\n");
}