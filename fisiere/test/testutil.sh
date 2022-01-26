#!/bin/bash
# collection of utilities for testing scripts
# individual test scripts should source this script first

# set up directory paths
TESTDIR=$(realpath "$(dirname "$BASH_SOURCE")")
TEMEDIR=$(realpath "$TESTDIR"/../teme)

TEST_WORKDIR=.
CHECK_WORKDIR=.

# check command line arguments for additional options:
# dirs - tells the script to run test and check commands in their own
#        working directory: this way commands that alter files or
#        directories can be checked without interfering with each other
while [[ $# -gt 0 ]] ; do
    case "$1" in
        "dirs")
            TEST_WORKDIR=$(mktemp -d)
            CHECK_WORKDIR=$(mktemp -d)
            ;;
        DEFAULT)
            echo "Unknown option $1" >&2
            exit 1
    esac
    shift
done


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


# helper function: create an escaped sed pattern from command name
# this will allow to diff the stderr output of two different commands
# without regarding the command name:
#   if the output starts with the command name followed by colon it will be replaced
#   with "<TEST >cmd:" where cmd is a shortened name of the command
#   and if output starts with <TEST > or <test >, it will have switched case
#   so the two outputs will only match if both start with their own program name
#   and all else is equal
sed_pattern () {
    PATTERN_FORMAT="s/^<\(\(TEST\)\|\(test\)\) >/<\\\\L\\\\2\\\\U\\\\3 >/g
                    s/^%s:/<TEST >%s:/g" 
    sed "$(printf "$PATTERN_FORMAT" "$(echo $1 | sed 's/[[\.*^$/]/\\&/g')" "$(basename $1)")"
}


# make a reusable testing function:
# run a diff on two commands: one is a program in directory $TEMEDIR with the same
# name as the calling test script's basename and the other is stored in the $CMD
# variable (this can be a single command or a bash function or any other bash
# construct which should yield the very same output as the tested program both on
# stdout and stderr, and the same side-effects in the working directory
# the first argument will contain a message to be printed together with success or
# failure of the test
# all the remaining arguments given to this function are passed to both commands
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

    # if the stdin comes from a file, probably the checking program must
    # also read from that file from the beginning as it did the tested program
    # so in this case try to redirect stdin from the same file to rewind it
    # back to the beginning
    if [ ! -t 0 ] ; then
        exec < /dev/stdin
    fi

    # run checking command
    pushd $CHECK_WORKDIR > /dev/null
    $CMD $* > $TEMP_OUT_2 2> $TEMP_ERR_2
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
    if ! diff <(sed_pattern $TEST_COMMAND < $TEMP_ERR_1) <(sed_pattern $CMD < $TEMP_ERR_2) ; then
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

