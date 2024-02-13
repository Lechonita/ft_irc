#!/bin/bash

CUNIT_FOLDER=./tests/CUNIT/
CUNIT="$CUNIT_FOLDER"cunit

# Access the command line argument
valgrind="$1"
VALGRIND_FOLDER="${CUNIT_FOLDER}"/valgrind/
LOG_FILE="${VALGRIND_FOLDER}"valgrind_min.out

# Use the variable in the script
if [[ "$valgrind" == "valgrind" ]]; then
    VALGRIND="valgrind -s --leak-check=full --log-file=$LOG_FILE --show-leak-kinds=all"
fi

function print_debug() {
	mkdir -p $VALGRIND_FOLDER
	if [[ (-n "$VALGRIND" && $(grep -c "ERROR SUMMARY: 0 errors" $LOG_FILE) -eq 0) || $1 -ne 0 ]]; then
		cat "$LOG_FILE"
		ret_val=1
	else
		ret_val=0
	fi
}

$VALGRIND $CUNIT
res=$?
print_debug "$res"
exit "$ret_val"