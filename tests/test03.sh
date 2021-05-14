#!/bin/bash

# Test/Success/Failure colors
TESTCOLOR="\033[0;36m"
SUCCESS="\033[0;32m"
FAILURE="\033[0;31m"
NC="\033[0m"

cd /home/achishti/hw2-achishti/tests

echo -e "${TESTCOLOR} Test 1: Serious Report${NC}"
./serious_report.out
if [ $? -eq 2 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 2: Match Report${NC}"
./match_report.out
if [ $? -eq 6 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 3: Bad Free Report${NC}"
./bad_free_report.out
if [ $? -eq 2 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 4: Orphan Free Report${NC}"
./orphan_free_report.out
if [ $? -eq 1 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 5: Double Free Report${NC}"
./double_free_report.out
if [ $? -eq 1 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi

echo -e "${TESTCOLOR} Test 6: Approx Free Report${NC}"
./approx_free_report.out
if [ $? -eq 4 ]; then
    echo -e "${SUCCESS} PASSED!${NC}"
else
    echo -e "${FAILURE} FAILED! ${NC}"
fi