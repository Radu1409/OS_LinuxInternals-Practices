#!/bin/bash

# get the testing utilities
. $(dirname "$0")/testutil.sh dirs

CMD="cp"

# some temporary files used for testing
SOURCE=`mktemp`
DST1=`mktemp`
DST2=`mktemp`

# have some content in the source file
cat <<EOF > $SOURCE
Test file

contains just a few lines of text
EOF

# test for insuficient parameters
run_test "cp without files"
run_test "cp with 1 file only" $SOURCE
run_test "cp with more than 2 files" $SOURCE $DST1 $DST2

# copy the test file into working directory
run_test "simple copy" $SOURCE ./file1

# check if file permissions are copied as well
umask 0000 # disable umask so it won't interfere with permissions
chmod 0600 $SOURCE
run_test "check permissions 0600" $SOURCE ./file2
chmod 0714 $SOURCE
run_test "check permissions 0714" $SOURCE ./file3
chmod 0644 $SOURCE
run_test "check permissions 0644" $SOURCE ./file4

# check permission related error messages
chmod -r $SOURCE
run_test "permission denied on source file" $SOURCE ./file5
chmod +r $SOURCE

chmod -w $DST1
run_test "permission denied on destination file" $SOURCE $DST1
chmod +w $DST1

#test "cp file dir1 dir2"
DST_DIR1=`mktemp -d`
DST_DIR2=`mktemp -d`
run_test "cp file dir1 dir2" $SOURCE $DST_DIR1 $DST_DIR2
rm -r $DST_DIR1
rm -r $DST_DIR2

rm $SOURCE
# test for err message inexistent file
run_test "inexistent source file" $SOURCE ./file6

rm $DST1
rm $DST2

