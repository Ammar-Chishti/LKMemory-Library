#include "lkmalloc.h"

gboolean lkreport_serious_write(gpointer key, gpointer value, gpointer data) {
    struct tree_record* malloc_record = (struct tree_record*) value;
    struct lkreport_args* args = (struct lkreport_args*) data;

    if (!(malloc_record->is_paired)) {
        
        char* malloc_string = tree_record_to_string(malloc_record);
        if (write(args->fd, malloc_string, strlen(malloc_string)) < 0) {
            perror("Unable to write memory leak record to outfile");
            return -1;
        }
        args->record_count += 1;
        free(malloc_string);
    }
    return FALSE;
}

gboolean lkreport_match_write(gpointer key, gpointer value, gpointer data) {
    struct tree_record* malloc_record = (struct tree_record*) key;
    struct tree_record* free_record = (struct tree_record*) value;
    struct lkreport_args* args = (struct lkreport_args*) data;

    char* malloc_string = tree_record_to_string(malloc_record);
    char* free_string = tree_record_to_string(free_record);
    if (write(args->fd, malloc_string, strlen(malloc_string)) < 0) {
        perror("Unable to write malloc pair record to outfile");
        return -1;
    }
    if (write(args->fd, free_string, strlen(free_string)) < 0) {
        perror("Unable to write free pair record to outfile");
        return -1;
    }
    args->record_count += 2;
    return FALSE;
}

// This function can be called in any of the error trees with key void** and val tree_record*
gboolean lkreport_free_error_tree_write(gpointer key, gpointer value, gpointer data) {
    struct tree_record* free_error_record = (struct tree_record*) value;
    struct lkreport_args* args = (struct lkreport_args*) data;

    char* free_error_string = tree_record_to_string(free_error_record);
    if (write(args->fd, free_error_string, strlen(free_error_string)) < 0) {
        perror("Unable to free error record to outfile");
        return -1;
    }
    args->record_count += 1;
    return FALSE;
}


gboolean lkfree_match_malloc(gpointer key, gpointer value, gpointer data) {
    struct lkfree_match_malloc_args* args = (struct lkfree_match_malloc_args*) data;
    struct tree_record* val_record = (struct tree_record*) value;

    if (key == args->free_ptr_passed) {
        args->malloc_match = val_record;
        // printf("EXACT FREE AND MALLOC MATCH FOUND\n");
        // printf("free ptr passed is %p\n", args->free_ptr_passed);
        // printf("malloc record ptr is %p\n", key);
        return TRUE;
    } else if (key <= args->free_ptr_passed && args->free_ptr_passed < (key + val_record->size_or_flags)) {
        args->malloc_match = val_record;
        // printf("APPROXIMATE FREE AND MALLOC MATCH FOUND\n");
        // printf("free ptr passed is %p\n", args->free_ptr_passed);
        // printf("malloc record ptr is %p\n", key);
        return TRUE;
    }
    return FALSE;
}

gboolean free_pair_trees(gpointer key, gpointer value, gpointer data) {
    struct tree_record* malloc_record = (struct tree_record*) key;
    struct tree_record* val_record = (struct tree_record*) value;
    
    free(malloc_record->filename);
    free(malloc_record->func_name);
    free(malloc_record);
    
    free(val_record->filename);
    free(val_record->func_name);
    free(val_record);
    return FALSE;
}

gboolean free_single_trees(gpointer key, gpointer value, gpointer data) {
    struct tree_record* val_record = (struct tree_record*) value;
    free(val_record->filename);
    free(val_record->func_name);
    free(val_record);
    return FALSE;
}

int are_trees_initialized = 0;

int lkmalloc_helper(u_int size, void** ptr, u_int flags, char* filename, int line_num, const char* func_name) {
    if (flags < 0) return -EINVAL;
    if (!are_trees_initialized) {
        init_trees();
        are_trees_initialized = 1;
    }

    // We don't need lkm_reg because if none of these flags are true, then that will be the same as lkm_reg
    int lkm_init, lkm_over, lkm_under, lkm_exist, lkm_realloc;
    lkm_init = 0, lkm_over = 0, lkm_under = 0, lkm_exist = 0, lkm_realloc = 0;
    
    void* alloc_addr_returned = NULL;
    int total_size = size;

    if (LKM_INIT & flags) lkm_init = 1;
    if (LKM_OVER & flags) {
        lkm_over = 1;
        total_size += 8;
    }
    if (LKM_UNDER & flags) {
        lkm_under = 1;
        total_size += 8;
    }
    if (LKM_EXIST & flags) lkm_exist = 1;
    if (LKM_REALLOC & flags) lkm_realloc = 1;

    if (*ptr != NULL) {
        if (lkm_exist) {
            fprintf(stderr, "Refusing to allocate memory because *ptr passed is not NULL");
            return -EADDRINUSE;
        }
        if (lkm_realloc) {
            struct tree_record* record = g_tree_lookup(malloc_tree, *ptr);
            if (!(record)) {
                fprintf(stderr, "Ptr passed in does not match any malloc record and therefore cannot realloc");
                return -EINVAL;
            }

            alloc_addr_returned = realloc(*ptr, total_size * sizeof(char));
            if (!(alloc_addr_returned)) return -ENOMEM;
            // We don't need to return here. This code will either realloc or malloc depending
            // on the flags passed in and do the necessary allocating for redzones if needed.
        } else {
            alloc_addr_returned = malloc(total_size * sizeof(char));
            if (!(alloc_addr_returned)) return -ENOMEM;
        }
    } else {
        alloc_addr_returned = malloc(total_size * sizeof(char));
        if (!(alloc_addr_returned)) return -ENOMEM;
    }

    if (lkm_under) {
        memset(alloc_addr_returned, 0x6b, 8);
        alloc_addr_returned += 8;
    }
    if (lkm_init) memset(alloc_addr_returned, 0, size);
    if (lkm_over) memset(alloc_addr_returned+size, 0x5a, 8);

    int redzones = 0;
    if (lkm_under && lkm_over) { redzones = 3; }
    else if (lkm_under && !(lkm_over)) { redzones = 1; }
    else if (!(lkm_under) && lkm_over) { redzones = 2; }

    gettimeofday(&cur_time, NULL);
    *ptr = alloc_addr_returned;
    struct tree_record* record = new_record(0, filename, func_name, line_num, cur_time.tv_sec, cur_time.tv_usec, ptr, 0, size, alloc_addr_returned, 0, 0, redzones);
    
    // printf("About to insert following record\n");
    // print_tree_record(record);
    g_tree_insert(malloc_tree, alloc_addr_returned, record);
    return 0;
}


int lkfree_helper(void** ptr, u_int flags, char* filename, int line_num, const char* func_name) {
    if (flags < 0) return -EINVAL;
    if (!are_trees_initialized) {
        init_trees();
        are_trees_initialized = 1;
    }

    int lkf_approx, lkf_warn, lkf_unknown, lkf_error;
    lkf_approx = 0, lkf_warn = 0, lkf_unknown = 0, lkf_error = 0;
    int is_approx = 0;

    if (LKF_APPROX & flags) lkf_approx = 1;
    if (LKF_WARN & flags) lkf_warn = 1;
    if (LKF_UNKNOWN & flags) lkf_unknown = 1;
    if (LKF_ERROR & flags) lkf_error = 1;

    struct lkfree_match_malloc_args data = {
        .free_ptr_passed = *ptr,
        .malloc_match = NULL,
    };

    g_tree_foreach(malloc_tree, lkfree_match_malloc, &data);
    struct tree_record* malloc_match_record = data.malloc_match;

    // print_tree_record(malloc_match_record);
    
    // We have an orphan free if
    // 1.) ptr passed in is NULL
    // 2.) We cannot find the appropriate malloc record from the tree for the free record
    // 3.) We find a malloc record but it is already paired and it's not an exact match with ptr
    if (!(*ptr) || !(malloc_match_record) || (malloc_match_record && malloc_match_record->is_paired && *ptr != malloc_match_record->alloc_addr_returned)) {
        if (lkf_unknown) fprintf(stderr, "Warning! Attempting free a ptr that was never allocated\n");

        gettimeofday(&cur_time, NULL);
        struct tree_record* orphan_record = new_record(1, filename, func_name, line_num, cur_time.tv_sec, cur_time.tv_usec, ptr, -EINVAL, flags, NULL, 0, 0, -1);
        
        g_tree_insert(orphan_free_tree, ptr, orphan_record);
        if (lkf_error)  exit(-EINVAL);
        return -EINVAL;
    }

    is_approx = (*ptr == malloc_match_record->alloc_addr_returned) ? 0 : 1;

    // We found a double free, or a malloc record that already is paired with the exact free pointer
    if (malloc_match_record->is_paired && !(is_approx)) {
        fprintf(stderr, "Warning! Attempting to double free a ptr\n");
        
        gettimeofday(&cur_time, NULL);
        struct tree_record* double_free_record = new_record(1, filename, func_name, line_num, cur_time.tv_sec, cur_time.tv_usec, ptr, -EINVAL, flags, NULL, -1, 0, lkf_approx);
        g_tree_insert(double_free_tree, ptr, double_free_record);
        return -EINVAL;
    }

    // We have an approximate free but approx free flag is not set
    if (is_approx && !(lkf_approx)) {
        if (lkf_warn) fprintf(stderr, "Warning! Attempting to perform an approximate free when lkf_approx is not set\n");

        gettimeofday(&cur_time, NULL);
        struct tree_record* bad_free_record = new_record(1, filename, func_name, line_num, cur_time.tv_sec, cur_time.tv_usec, ptr, -EINVAL, flags, NULL, -1, 0, -1);
        g_tree_insert(bad_free_tree, ptr, bad_free_record);
        
        if (lkf_error)  exit(-EINVAL);
        return -EINVAL;
    }

    if (is_approx && lkf_approx) {
        if (malloc_match_record->redzones == 1 || malloc_match_record->redzones == 3) {
            void* addr_to_free = malloc_match_record->alloc_addr_returned - 8;
            free(addr_to_free);
        } else {
            free(malloc_match_record->alloc_addr_returned);
        }
        malloc_match_record->is_paired = 1;

        gettimeofday(&cur_time, NULL);
        struct tree_record* approx_free_record = new_record(1, filename, func_name, line_num, cur_time.tv_sec, cur_time.tv_usec, ptr, 0, flags, NULL, -1, 1, -1);
        g_tree_insert(approx_match_tree, malloc_match_record, approx_free_record);

        g_tree_insert(bad_free_tree, ptr, approx_free_record);  // I don't necessarily agree with this but I'll go along with it

        if (lkf_warn)  fprintf(stderr, "Warning! Performing approximate free\n");
        if (lkf_error)  return -EINVAL;
        return 0;
    }

    if (!(flags)) {
        // print_tree_record(malloc_match_record);
        if (malloc_match_record->redzones == 1 || malloc_match_record->redzones == 3) {
            void* addr_to_free = malloc_match_record->alloc_addr_returned - 8;
            free(addr_to_free);
        } else {
            free(malloc_match_record->alloc_addr_returned);
        }
        malloc_match_record->is_paired = 1;

        gettimeofday(&cur_time, NULL);
        struct tree_record* perfect_free_record = new_record(1, filename, func_name, line_num, cur_time.tv_sec, cur_time.tv_usec, ptr, 0, flags, NULL, -1, 0, -1);
        g_tree_insert(perfect_match_tree, malloc_match_record, perfect_free_record);
    }

    return 0;
}

int lkreport(int fd, u_int flags) {
    if (!(flags) || flags < 0) return 0;

    int lkr_serious, lkr_match, lkr_bad_free, lkr_orphan_free, lkr_double_free, lkr_approx;
    lkr_serious = 0, lkr_match = 0, lkr_bad_free = 0, lkr_orphan_free = 0, lkr_double_free = 0, lkr_approx = 0;

    if (LKR_SERIOUS & flags) lkr_serious = 1;
    if (LKR_MATCH & flags) lkr_match = 1;
    if (LKR_BAD_FREE & flags) lkr_bad_free = 1;
    if (LKR_ORPHAN_FREE & flags) lkr_orphan_free = 1;
    if (LKR_DOUBLE_FREE & flags) lkr_double_free = 1;
    if (LKR_APPROX & flags) lkr_approx = 1;

    struct lkreport_args data = {
        .fd = fd,
        .record_count = 0,
    };

    char* csv_format = "record_type,filename,fxname,line_num,timestamp,ptr_passed,retval,size_or_flags,alloc_addr_returned\n";
    if (write(fd, csv_format, strlen(csv_format)) < 0) {
        perror("Unable to write CSV format to top of outfile");
        return -1;
    }

    if (lkr_serious) {
        g_tree_foreach(malloc_tree, lkreport_serious_write, &data);
    }
    if (lkr_match) {
        g_tree_foreach(perfect_match_tree, lkreport_match_write, &data);
    }
    if (lkr_bad_free) {
        g_tree_foreach(bad_free_tree, lkreport_free_error_tree_write, &data);
    }
    if (lkr_orphan_free) {
        g_tree_foreach(orphan_free_tree, lkreport_free_error_tree_write, &data);
    }
    if (lkr_double_free) {
        g_tree_foreach(double_free_tree, lkreport_free_error_tree_write, &data);
    }
    if (lkr_approx) {
        g_tree_foreach(approx_match_tree, lkreport_match_write, &data);
    }

    return data.record_count;
}

void exit_gracefully() {
    lkreport(STDOUT_FILENO, 63);    // 63 will print out all records

    g_tree_foreach(malloc_tree, free_single_trees, NULL);
    g_tree_foreach(bad_free_tree, free_single_trees, NULL);
    g_tree_foreach(orphan_free_tree, free_single_trees, NULL);
    g_tree_foreach(double_free_tree, free_single_trees, NULL);
    // g_tree_foreach(perfect_match_tree, free_pair_trees, NULL);
    g_tree_foreach(approx_match_tree, free_pair_trees, NULL);

    g_tree_destroy(malloc_tree);
    g_tree_destroy(bad_free_tree);
    g_tree_destroy(orphan_free_tree);
    g_tree_destroy(double_free_tree);
    g_tree_destroy(perfect_match_tree);
    g_tree_destroy(approx_match_tree);
}