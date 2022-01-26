#!/bin/bash

# get the testing utilities
. "$(dirname "$0")"/testutil.sh

CMD="wc"

run_test "on this script" $TESTDIR/wc.sh

TESTFILE=../test/fisier1.txt
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
run_test "multiple inexistent files" foo bar baz
run_test "multiple existent-inexistent files" mama.c $TEMEDIR/hole.c

SMALLFILE=`mktemp`
echo Hello > $SMALLFILE
run_test "multiple alignment 1-4" $TEMPFILE $SMALLFILE $TEMEDIR/wc.c
run_test "multiple alignment 1-3" $TEMPFILE $SMALLFILE $TEMEDIR/Makefile
run_test "multiple alignment 3-1-4" $TEMEDIR/Makefile $SMALLFILE $TEMEDIR/wc.c
rm $SMALLFILE

# test for -w, -l, -c options
run_test "-w only" -w $TEMEDIR/wc.c
run_test "-l only" -l $TEMEDIR/wc.c
run_test "-c only" -c $TEMEDIR/wc.c
run_test "-w -l only" -w -l $TEMEDIR/wc.c
run_test "-w -l only (var2)" -l -w $TEMEDIR/wc.c
run_test "-w -c only" -w -c $TEMEDIR/wc.c
run_test "-c -l only" -c -l $TEMEDIR/wc.c
run_test "-w -c -l" -w -c -l $TEMEDIR/wc.c

# test for -L option
run_test "-L only" -L $TEMEDIR/wc.c
run_test "-L multiple files" -L $TEMEDIR/wc.c $TEMEDIR/head.c
run_test "-w -L" -w -L $TEMEDIR/wc.c
run_test "-c -L" -c -L $TEMEDIR/wc.c
run_test "-l -L" -l -L $TEMEDIR/wc.c
run_test "-w -c -L" -w -c -L $TEMEDIR/wc.c
run_test "-c -l -L" -c -l -L $TEMEDIR/wc.c
run_test "-w -l -L" -w -l -L $TEMEDIR/wc.c
run_test "-w -c -l -L" -w -c -l -L $TEMEDIR/wc.c
run_test "-w -c -l -L multiple files" -w -c -l -L $TEMEDIR/wc.c $TEMEDIR/head.c

# binary file
run_test "binary" $TEMEDIR/wc

