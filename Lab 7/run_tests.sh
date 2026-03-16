#!/bin/bash

cd /mnt/c/Users/User/CompilerLab/Lab\ 7

PASSED=0
FAILED=0

echo "Running test suite..."
echo ""

for i in {01..55}; do
  testfile="test_suite/test${i}.lang"
  correctfile="test_suite/test${i}.correct"
  
  if [ ! -f "$testfile" ]; then
    continue
  fi
  
  # Compile the test
  ./lang "$testfile" "langout" 2>/dev/null
  if [ $? -ne 0 ]; then
    echo "test$i COMPILE FAILED"
    ((FAILED++))
    continue
  fi
  
  # Assemble and run
  slasm langout.sl io320.sl >/dev/null 2>&1
  if [ $? -ne 0 ]; then
    echo "test$i ASSEMBLY FAILED"
    ((FAILED++))
    continue
  fi
  
  timeout 5 stackl langout.slb > out 2>&1
  if [ $? -eq 124 ]; then
    echo "test$i TIMEOUT (infinite loop?)"
    ((FAILED++))
    continue
  fi
  
  # Compare output
  if diff -qbwB out "$correctfile" >/dev/null 2>&1; then
    echo "test$i PASSED"
    ((PASSED++))
  else
    echo "test$i FAILED"
    ((FAILED++))
  fi
done

echo ""
echo "========== SUMMARY =========="
echo "Passed: $PASSED"
echo "Failed: $FAILED"
echo "Total:  $((PASSED + FAILED))"

# Cleanup temporary files
rm -f out langout.sl langout.slb
