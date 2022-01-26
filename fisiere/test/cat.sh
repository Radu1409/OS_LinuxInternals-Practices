#!/bin/bash

# get the testing utilities

. $(dirname "$0")/testutil.sh

CMD="cat"

run_test "on this script" $TESTDIR/cat.sh

run_test "fisier1.txt" $TESTDIR/fisier1.txt

# test for more than 4096 octets
TEMPFILE=`mktemp`
python -c "print('a'*4090 + ' ' + 'b'*20)" > $TEMPFILE
run_test "4096+ bytes" $TEMPFILE

# permission denied
chmod -r $TEMPFILE
run_test "permission denied" $TEMPFILE

rm $TEMPFILE
# test for err message inexistent file
run_test "inexistent file" $TEMPFILE

#test for directory 
run_test "directory" ..

#test for multiple files 
run_test "multiple files" $TEMEDIR/head.c $TEMEDIR/wc.c $TEMEDIR/grep.c
run_test "multiple existent-inexistent files" mama.c $TEMEDIR/hole.c

#test with -E
run_test "with -E option" -E $TEMEDIR/head.c $TEMEDIR/wc.c

#test with -n option
run_test "with -n option" -n $TEMEDIR/head.c $TEMEDIR/wc.c
run_test "-n -E" -n -E $TEMEDIR/head.c $TEMEDIR/wc.c
