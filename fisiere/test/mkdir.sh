#!/bin/bash

# get the testing utilities
. $(dirname "$0")/testutil.sh dirs

CMD="mkdir"

# some temporary files used for testing
SOURCE=`mktemp`

# test without param
run_test "without param" 

# test with director 
DIR=`mktemp -d`
run_test "with dir" $DIR

# test with file
run_test "with file" $SOURCE

# test
run_test "create one dir" DIR1
run_test "create multiple dirs" DIR2 DIR3 DIR4

# test with -v param
run_test "directory with param -v" -v DIR5

# test with -p param
run_test "directory with param -p" -p DIR6/DIR7/DIR8

# test with -p param --permision denied--
mkdir DIR9
chmod -x DIR9
run_test "-p DIR6 permission denied" -p DIR9/DIR10

mkdir DIR12
chmod -x DIR12
run_test "-p DIR12 permission denied" -p DIR11/DIR12/DIR13
rm -r DIR12

rm -f $SOURCE
rm -r DIR9 
rmdir $DIR
