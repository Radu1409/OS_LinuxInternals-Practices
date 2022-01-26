#!/bin/bash

# set up directory paths
TESTDIR=$(realpath "$(dirname "$BASH_SOURCE")")
TEMEDIR=$(realpath "$TESTDIR"/../teme)

TEST_WORKDIR=.
CHECK_WORKDIR=.

# tested program is infered from the script name
TEST_COMMAND_NAME=$(basename "$0" .sh)

# create temporary files for storing stdout and stderr for later diffing
TEMP_OUT_1=$(mktemp)
TEMP_OUT_2=$(mktemp)
TEMP_ERR_1=$(mktemp)
TEMP_ERR_2=$(mktemp)

# cleanup function called at the end of testing script to clean up
# temporary files
cleanup () {
    rm $TEMP_OUT_1 $TEMP_OUT_2 $TEMP_ERR_1 $TEMP_ERR_2
    if [[ "$TEST_WORKDIR" != "$CHECK_WORKDIR" ]] ; then
        rm -rf "$TEST_WORKDIR"
        rm -rf "$CHECK_WORKDIR"
    fi
}

trap cleanup EXIT

sed_pattern () {
    PATTERN_FORMAT="s/^<\(\(TEST\)\|\(test\)\) >/<\\\\L\\\\2\\\\U\\\\3 >/g
                    s/^%s:/<TEST >%s:/g" 
    sed "$(printf "$PATTERN_FORMAT" "$(echo $1 | sed 's/[[\.*^$/]/\\&/g')" "$(basename $1)")"
}

run_test () {
    # fist argument is the message
    MESSAGE="$1"
    shift

    # run the tested program
    pushd $TEST_WORKDIR > /dev/null
    local TEST_COMMAND=$(realpath --relative-to . $TEMEDIR)/$TEST_COMMAND_NAME
    $TEST_COMMAND $* > $TEMP_OUT_1 2> $TEMP_ERR_1
    TEST_COMMAND_STATUS=$?
    popd > /dev/null


    # run checking command
    pushd $CHECK_WORKDIR > /dev/null
    $* > $TEMP_OUT_2 2> $TEMP_ERR_2
    CMD_STATUS=$?
    popd > /dev/null

    # diff the outputs
    RESULT=()
    if (( $TEST_COMMAND_STATUS != $CMD_STATUS )) ; then
        RESULT+=( "exit code" )
    fi
    if ! diff $TEMP_OUT_1 $TEMP_OUT_2 ; then
        RESULT+=( "stdout" )
    fi
    if ! diff <(sed_pattern $TEST_COMMAND < $TEMP_ERR_1) <(sed_pattern $* < $TEMP_ERR_2) ; then
        RESULT+=( "stderr" )
    fi
    if [[ "$TEST_WORKDIR" != "$CHECK_WORKDIR" ]] ; then
        if ! diff "$TEST_WORKDIR" "$CHECK_WORKDIR" ; then
            RESULT+=( "content" )
        fi
    fi

    echo -n "Test $MESSAGE: "
    if (( ${#RESULT[@]} == 0 ))
    then
        echo -e "\033[0;32mSuccess\033[0m"
    else
        echo -e "\033[0;31mFailed on $(printf "%s" "${RESULT[@]/#/ and }" | cut -c 6-)\033[0m"
    fi
}

run_test "execvp ls" ls
run_test "execvp ls -l" ls -l
run_test "execvp ls -l -a" ls -l -a
run_test "execvp wc (on this script)" wc $TESTDIR/execvp.sh
run_test "execvp wc -w (on this script)" wc -w $TESTDIR/execvp.sh
run_test "execvp wc -w -l (on this script)" wc -w -l $TESTDIR/execvp.sh
run_test "execvp ./execvp" $TEMEDIR/execvp
