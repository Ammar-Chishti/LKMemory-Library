#!/bin/bash

# Test/Success/Failure colors
TESTCOLOR="\033[0;36m"
SUCCESS="\033[0;32m"
FAILURE="\033[0;31m"
NC="\033[0m"

cd /home/achishti/hw2-achishti/tests

echo -e "${TESTCOLOR} Test 1: Regular Free${NC}"
./reg_free.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 2: Approx Free${NC}"
./approx_free.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 3: Warn Approx Free${NC}"
./warn_approx_free.out
if [ $? -eq 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 4: Unknown Free${NC}"
./unknown_free.out
if [ $? -ne 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 5: Error Unknown Free${NC}"
./error_unknown_free.out
if [ $? -ne 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 6: Error Warn Approx Free${NC}"
./error_warn_approx_free.out
if [ $? -ne 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 7: Bad Free${NC}"
./bad_free.out
if [ $? -ne 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 8: Double Free${NC}"
./double_free.out
if [ $? -ne 0 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi