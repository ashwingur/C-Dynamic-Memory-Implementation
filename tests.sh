#!/bin/bash

count=0 # number of test cases run so far

# Note that the longtimetable test may take a few seconds to complete.
echo "---Compiling test cases---"
for test in testcases/*.c; do
    name=$(basename $test .c)
    # Compile the file
    gcc -fsanitize=address -std=gnu11 -g -o testcases/$name $test virtual_sbrk.c virtual_alloc.c -lm
    echo "$name.c compiled."
done
echo ""
echo "---Running tests---"
for test in testcases/*.c; do
    name=$(basename $test .c)
    expected=testcases/$name.out
    echo "Running test $name"
    ./testcases/$name | diff - $expected || echo "Test $name: failed!"
    count=$((count+1))
done

echo "Finished running $count tests!"