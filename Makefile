CC := gcc
SRCD := src
TSTD := tests
BLDD := build
BIND := bin
INCD := include
LIBD := lib

ALL_SRCF := $(shell find $(SRCD) -type f -name *.c)
#ALL_LIBF := $(shell find $(LIBD) -type f -name *.o)
ALL_OBJF := $(BLDD)/binary_search_tree.o $(BLDD)/lkmalloc.o
ALL_TSTF := $(shell find $(TSTD) -type f -name *.c)

CFLAGS := -Wall -Werror -Wno-unused-function -g -O2
LDFLAGS := lkmalloc.a -lglib-2.0 

INC := -I $(INCD) -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include

.PHONY: clean depend tests

depend:	setup $(ALL_OBJF) $(BLDD)
	ar -crv lkmalloc.a $(ALL_OBJF)
	$(CC) $(CFLAGS) -o lkmalloc_driver lkmalloc_driver.c $(INC) $(LDFLAGS)

setup: $(BLDD)
$(BLDD):
	mkdir -p $(BLDD)

tests: create_tests
	clear
	$(TSTD)/test01.sh
	$(TSTD)/test02.sh
	mkdir -p $(TSTD)/lkreports
	$(TSTD)/test03.sh
	rm $(TSTD)/*.out

# I tried making this generic but could not get it to work D:
create_tests:
	$(CC) $(TSTD)/malloc_tests/double_malloc.c $(INC) $(LDFLAGS) -o $(TSTD)/double_malloc.out
	$(CC) $(TSTD)/malloc_tests/exist_malloc.c $(INC) $(LDFLAGS) -o $(TSTD)/exist_malloc.out
	$(CC) $(TSTD)/malloc_tests/init_malloc.c $(INC) $(LDFLAGS) -o $(TSTD)/init_malloc.out
	$(CC) $(TSTD)/malloc_tests/init_over_malloc.c $(INC) $(LDFLAGS) -o $(TSTD)/init_over_malloc.out
	$(CC) $(TSTD)/malloc_tests/init_over_under_malloc.c $(INC) $(LDFLAGS) -o $(TSTD)/init_over_under_malloc.out
	$(CC) $(TSTD)/malloc_tests/init_realloc.c $(INC) $(LDFLAGS) -o $(TSTD)/init_realloc.out
	$(CC) $(TSTD)/malloc_tests/init_under_malloc.c $(INC) $(LDFLAGS) -o $(TSTD)/init_under_malloc.out
	$(CC) $(TSTD)/malloc_tests/over_malloc.c $(INC) $(LDFLAGS) -o $(TSTD)/over_malloc.out
	$(CC) $(TSTD)/malloc_tests/reg_malloc.c $(INC) $(LDFLAGS) -o $(TSTD)/reg_malloc.out
	$(CC) $(TSTD)/malloc_tests/under_malloc.c $(INC) $(LDFLAGS) -o $(TSTD)/under_malloc.out

	$(CC) $(TSTD)/free_tests/approx_free.c $(INC) $(LDFLAGS) -o $(TSTD)/approx_free.out
	$(CC) $(TSTD)/free_tests/bad_free.c $(INC) $(LDFLAGS) -o $(TSTD)/bad_free.out
	$(CC) $(TSTD)/free_tests/double_free.c $(INC) $(LDFLAGS) -o $(TSTD)/double_free.out
	$(CC) $(TSTD)/free_tests/error_unknown_free.c $(INC) $(LDFLAGS) -o $(TSTD)/error_unknown_free.out
	$(CC) $(TSTD)/free_tests/error_warn_approx_free.c $(INC) $(LDFLAGS) -o $(TSTD)/error_warn_approx_free.out
	$(CC) $(TSTD)/free_tests/reg_free.c $(INC) $(LDFLAGS) -o $(TSTD)/reg_free.out
	$(CC) $(TSTD)/free_tests/unknown_free.c $(INC) $(LDFLAGS) -o $(TSTD)/unknown_free.out
	$(CC) $(TSTD)/free_tests/warn_approx_free.c $(INC) $(LDFLAGS) -o $(TSTD)/warn_approx_free.out

	$(CC) $(TSTD)/report_tests/approx_free_report.c $(TSTD)/report_tests/write_report.c $(INC) $(LDFLAGS) -o $(TSTD)/approx_free_report.out
	$(CC) $(TSTD)/report_tests/bad_free_report.c $(TSTD)/report_tests/write_report.c $(INC) $(LDFLAGS) -o $(TSTD)/bad_free_report.out
	$(CC) $(TSTD)/report_tests/double_free_report.c $(TSTD)/report_tests/write_report.c $(INC) $(LDFLAGS) -o $(TSTD)/double_free_report.out
	$(CC) $(TSTD)/report_tests/match_report.c $(TSTD)/report_tests/write_report.c $(INC) $(LDFLAGS) -o $(TSTD)/match_report.out
	$(CC) $(TSTD)/report_tests/orphan_free_report.c $(TSTD)/report_tests/write_report.c $(INC) $(LDFLAGS) -o $(TSTD)/orphan_free_report.out
	$(CC) $(TSTD)/report_tests/serious_report.c $(TSTD)/report_tests/write_report.c $(INC) $(LDFLAGS) -o $(TSTD)/serious_report.out

$(BLDD)/lkmalloc.o: $(SRCD)/lkmalloc.c $(INCD)/lkmalloc.h
	$(CC) $(CFLAGS) $(INC) -c $(SRCD)/lkmalloc.c -o $@

$(BLDD)/binary_search_tree.o: $(SRCD)/binary_search_tree.c $(INCD)/lkmalloc.h
	$(CC) $(CFLAGS) $(INC) -c $(SRCD)/binary_search_tree.c -o $@

clean:
	rm -rf lkmalloc_driver $(BLDD) lkmalloc.a
