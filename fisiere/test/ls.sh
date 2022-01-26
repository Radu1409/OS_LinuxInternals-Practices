#!/bin/bash

export TABSIZE=0

# get the testing utilities
. $(dirname "$0")/testutil.sh

CMD="ls -xw0"

run_test "on this script" $TESTDIR/ls.sh


# test for ls on current director
run_test "ls on current dir"

# permission denied
TEMPFILE=`mktemp`
chmod -r $TEMPFILE
run_test "permission denied" $TEMPFILE

rm $TEMPFILE
# test for err message inexistent file
run_test "inexistent file" $TEMPFILE

#test for directory 
run_test "directory" ..

#test for multiple directory
run_test "multiple directory" .. ../..

#test for 1,l,a,A options
run_test "ls with -l" -l
run_test "ls with -1" -1
run_test "ls with -a" -a
run_test "ls with -A" -A

#test for combined options (1,l,a,A)
run_test "ls with -a -A" -a -A
run_test "ls with -A -1 -a" -A -1 -a

#test for regular file
run_test "regular file" $TEMEDIR/cat.c

#test for l option & combinations
run_test "-l only" -l
run_test "-l -a only" -l -a
run_test "-l -A only" -l -A
run_test "-l -r only" -l -r
run_test "-l -r -a" -l -r -a
run_test "-l -r -A" -l -r -A
run_test "-alr .." -alr ..
run_test "-alr ../.." -alr ../..
run_test "-al .." -al ..
run_test "-al ../.." -al ../..

#some tests with TIME_STYLE with different values
export TIME_STYLE=locale
run_test "ls with TIME_STYLE=locale" -l

export TIME_STYLE=iso
run_test "ls with TIME_STYLE=iso" -l

export TIME_STYLE=long-iso
run_test "ls with TIME_STYLE=long-iso" -l

export TIME_STYLE=full-iso
run_test "ls with TIME_STYLE=full-iso" -l

#test for director without reading permission
TEMPDIR=`mktemp -d`
chmod -r $TEMPDIR

run_test "on dir without reading permission" $TEMPDIR

#some tests with combined options and multiple arguments
run_test "-l .. ../.." .. ../.. -l
run_test "-al ../.. .." -al ../.. ..
run_test "-1 ../.. .." -1 .. ../..
run_test ".. ../.. file1 file2" .. ../.. file1 file2
run_test ".. ../.. dir without permission" .. ../.. $TEMPDIR
run_test "-l .. ../.. file1 file2 + reg files" -l .. ../.. file1 file2 $TEMEDIR/head.c $TEMEDIR/ls.c
rmdir $TEMPDIR
run_test "reg file + dir + inexistent file" $TEMEDIR/head.c $TEMEDIR/ls.c .. ../.. file1 file2

#test for -v option
DIR=`mktemp -d`
chmod 777 $DIR
touch $DIR/{a1,a2,a120,a13,a11b,a10b,a00b,a01b}
mkdir $DIR/{dir1,dir2,dir120,dir13,dir11b,dir10b,dir00b,dir01b}
run_test "-v option" -v
run_test "-vl" -vl
run_test "-val" -val
run_test "-v .. ../.. + reg file" -v .. ../.. $TEMEDIR/head.c $TEMEDIR/ls.c $TEMEDIR/tail.c
run_test "-vl .. ../.. + reg file" -vl .. ../.. $TEMEDIR/head.c $TEMEDIR/ls.c $TEMEDIR/tail.c
run_test "-val .. ../.. + reg file" -val .. ../.. $TEMEDIR/head.c $TEMEDIR/ls.c $TEMEDIR/tail.c
run_test "dir with number in name" -v $DIR/dir*
run_test "files with number in name" -v $DIR/a*
run_test "dir and file with number in name" -vl $DIR/a* $DIR/dir* 

rm -rf $DIR
