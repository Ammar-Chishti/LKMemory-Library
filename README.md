# LKmemory-Library

An advanced C library that wraps around stdlib functions malloc and free to track information such as

    * Matching allocated/free pairs
    * Memory leaks
    * Buffer overflows
    * Double frees (valid free followed by an additional free)
    * Bad frees (where the passed addr is in the middle of the alloc addr)
    * Orphan frees (where the passed addr had never been allocated)
    * Regression tests for all of the functionality mentioned above


# LKmemory functions
* `int lkmalloc(u_int size, void** ptr, u_int flags)` - This will ask to allocate "size" bytes, and if successful, assign the newly allocated address to *ptr
* `int lkfree(void** ptr, u_int flags)` - This will take the addr of the ptr that was presumably allocated by lkmalloc (but maybe not), and attempt to free it
* `int lkreport(int fd, u_int flags)` - This will dump a report of memory issues to the file whose file descriptor is 'fd'

# How to compile and link with your C program
* NOTE - You must have glib installed on the machine in order to run this program! [Installing gtk and glib](https://stackoverflow.com/questions/5275196/)
* Run `make depend` to create the lkmalloc.a library file, the lkmalloc_driver executable file, and other necessary files. You can also run `make clean` if you want to remove the build and bin directories
* You can experiment with the functions defined in lkmalloc.a by calling them in lkmalloc_driver.c which has already been appropriately linked with lkmalloc.a and all of its dependencies. Note that there is an on_exit(...) function call at the beginning of lkmalloc_driver.c whose purpose is to ensure that a full report malloc/free pairs, memory leaks, and other errors are printed to stdout when the lkmalloc_driver executable is finished. You can comment this out if you wish
* To link lkmalloc.a with your own C program, insert the lkmalloc.a file into your own codebase as well as the file name in LDFLAGS of your own makefile

# Regression Tests
    * test01.sh tests the functionality of lkmalloc(...) 
    * test02.sh tests the functionality of lkfree(...)
    * test03.sh tests the functionality of lkreport(...)
Run `make tests` to compile all of the test programs and run all of the shell scripts. You will see if either test passed or fail on the screen. Note that test03.sh will check if the number of records successfully written to a file is correct but it will not check if contents of the records written to the file are correct. That is something the user will need to verify themselves in tests/lkreports which contain the reports created from test03.sh

# My Approach
* I used a set of binary search trees to hold records because of the fast log(n) insertion and retrieval. I also used different trees to hold different records such as malloc'd buffers, perfect malloc/free pairs, bad frees, orphan frees, etc. The reason I made this design decision is because if we wanted to simply print out only bad frees in lkreport(...), we would just have to traverse the bad_free tree isntead of master tree of records (if we decided to design the program that way) 
* The general flow of how the program works is that everytime a malloc is called, a record of the malloc is stored in the malloc_tree. Everytime the user attempts to free, we will try and find the appropriate malloc record from the malloc_tree and pair the two. If the free is unsuccessful (we can't find a pair, etc), we will store a record of that free in the appropriate tree (ex. orphan_free_tree, etc), however if the free is successful we will either store the malloc/free pair in the perfect_match_tree or the approx_match_tree and mark the malloc record in the malloc_tree as paired. Finally, to find memory leaks, we go through the malloc tree and find any malloc records that are not paired. This should handle most scenarios
