#!/bin/bash


# get the testing utilities
. "$(dirname "$0")/testutil.sh"

CMD="head"

run_test "on this script" $TESTDIR/head.sh

run_test "fisier1.txt" $TESTDIR/fisier1.txt

#test for more than 4096 octets 
TEMPFILE=`mktemp`
python -c "print('a'*4090 + ' ' + 'b'*20)" > $TEMPFILE
run_test "4096+ bytes" $TEMPFILE

# permission denied
chmod -r $TEMPFILE
run_test "permission denied on file" $TEMPFILE

TESTDIR=`mktemp -d`
chmod -r $TESTDIR
run_test "permission denied on dir" $TESTDIR
rmdir $TESTDIR


rm $TEMPFILE
# test for err message inexistent file
run_test "inexistent file" $TEMPFILE

#test for directory 
run_test "directory" ..

#test for multiple files 
run_test "multiple files" $TEMEDIR/tail.c $TEMEDIR/head.c $TEMEDIR/grep.c
run_test "multiple inexistent files" foo bar baz
run_test "multiple existent-inexistent files" mama.c $TEMEDIR/hole.c


# test for -n option to alter number of lines displayed
run_test "n=3 lines" -n 3 $TEMEDIR/head.c
run_test "n=2.5 lines" -n 2.5 $TEMEDIR/head.c
run_test "n=0 lines" -n 0 $TEMEDIR/head.c
run_test "00 lines" -n 00 $TEMEDIR/head.c
run_test "0 lines no files" -n 0
run_test "n=-0 lines" -n -0 $TEMEDIR/head.c
run_test "n=-4 lines" -n -4 $TEMEDIR/head.c
run_test "n=-1000 lines" -n -1000 $TEMEDIR/head.c
run_test "n=-4x lines" -n -4x $TEMEDIR/head.c

# test for -c option 
run_test "c=3 characters" -c 3 $TEMEDIR/head.c
run_test "c=500 characters" -c 500 $TEMEDIR/head.c
run_test "c=2.5 characters" -c 2.5 $TEMEDIR/head.c
run_test "c=0 characters" -c 0 $TEMEDIR/head.c
run_test "c=00 characters" -c 00 $TEMEDIR/head.c
run_test "c=0 characters no files" -c 0
run_test "c=-0 characters" -c -0 $TEMEDIR/head.c
run_test "c=-4 characters" -c -4 $TEMEDIR/head.c
run_test "c=-10000 characters" -c -10000 $TEMEDIR/head.c
run_test "c=-4x characters" -c -4x $TEMEDIR/head.c

# test for both -c and -n option
run_test "c after n" -n 2 -c 2 $TEMEDIR/head.c
run_test "n after c" -c 2 -n 2 $TEMEDIR/head.c
run_test "c after n" -n 0 -c 2 $TEMEDIR/head.c
run_test "n after c" -c 0 -n 2 $TEMEDIR/head.c
run_test "c after n" -n 2 -c 0 $TEMEDIR/head.c
run_test "n after c" -c 2 -n 0 $TEMEDIR/head.c
run_test "wrong n good c" -n 2.5 -c 2 $TEMEDIR/head.c

#test for stdin
run_test "c=7 chars from stdin" -c 7 < $TEMEDIR/head.c
run_test "c=-0 chars from stdin" -c -0 < $TEMEDIR/head.c
run_test "c=-7 chars from stdin" -c -7 < $TEMEDIR/head.c
run_test "n=7 lines from stdin" -n 7 < $TEMEDIR/head.c
run_test "n=-0 lines from stdin" -n -0 < $TEMEDIR/head.c
run_test "n=-7 lines from stdin" -n -7 < $TEMEDIR/head.c
