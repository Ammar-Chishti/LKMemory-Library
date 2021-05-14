#!/bin/bash

# Test/Success/Failure colors
TESTCOLOR="\033[0;36m"
SUCCESS="\033[0;32m"
FAILURE="\033[0;31m"
NC="\033[0m"

cd /home/achishti/hw2-achishti/tests

echo -e "${TESTCOLOR} Test 1: Regular Malloc${NC}"
./reg_malloc.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 2: Init Malloc${NC}"
./init_malloc.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 3: Over Malloc${NC}"
./over_malloc.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 4: Under Malloc${NC}"
./under_malloc.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 5: Exist Malloc${NC}"
./exist_malloc.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 6: Init Realloc${NC}"
./init_realloc.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 7: Init Under Malloc${NC}"
./init_under_malloc.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 8 Init Over Malloc${NC}"
./init_over_malloc.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 9 Init Over Under Malloc${NC}"
./init_over_under_malloc.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

# echo -e "${TESTCOLOR} Test 10 Double Malloc${NC}"
# ./double_malloc.out
# if [ $? -eq 1 ]; then
#     echo -e "${SUCCESS} PASSED!${NC}"
# else
#     echo -e "${FAILURE} FAILED! ${NC}"
# fi