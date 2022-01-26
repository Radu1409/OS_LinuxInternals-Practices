#!/bin/bash

# testing utilites
. $(dirname "$0")/testutil.sh
CMD="grep -F"

# simple search
run_test "simple" include $TEMEDIR/grep.c

# multiple files
run_test "multiple files" include $TEMEDIR/grep.c $TEMEDIR/ls.c

# test pentru parametru optional -i
run_test "case insensitive" Include -i $TEMEDIR/grep.c
run_test "case insensitive multiple files" Include -i $TEMEDIR/grep.c $TEMEDIR/head.c
run_test "-i -w options" Include -i -w $TEMEDIR/grep.c $TEMEDIR/head.c

# test pentru parametru optional -w
run_test "-w option" stat -w $TEMEDIR/grep.c
run_test "-w option multiple files" stat -w $TEMEDIR/grep.c $TEMEDIR/head.c

# test pentru grep cu nume de director
# în mod normal trebuie să afișeze mesaj de eroare
run_test "directory" grep ..

# test pentru grep cu fisier inaccesibil
# în mod normal trebuie să afișeze mesaj de eroare
TESTFILE=$(mktemp)
chmod -r $TESTFILE
run_test "inaccessible file" grep $TESTFILE
rm $TESTFILE

#test pentru fisier inexistent
# use same temporary file name which was deleted above
run_test "inexistent file" include $TESTFILE

#test for grep with no parameters
run_test "no params"

