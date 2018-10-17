#!/bin/bash

TIMED_LOG_SUFFIX=""
function date_time() {
echo $(date --iso-8601=seconds)
}

function timed_log() {

timestamp=$(date_time)
echo [ $timestamp ] -- $TIMED_LOG_SUFFIX - $1 

}

function timed_log_no_newline() {

timestamp=$(date_time)
echo -n [ $timestamp ] -- $TIMED_LOG_SUFFIX - $1
}

function YES_no() {
if [ -z "$1" ] 
then 
	str="Are you sure ?"
else 
	str="$1"
fi
timed_log_no_newline "$str"
read -r -p "[Y/n]" response
case "$response" in
    [nN][oO]|[nN]) 
	return 1
	;;
    *)	
	return 0
	;;
esac
}

function proceed_if_ok() {
if [ -z "$1" ] 
then
	echo "NO PARAM"
	return
fi

if [ $1 -eq 1 ]
then
	if [ -z "$2" ]  
	then
		str="An error occurred"
	else
		str=$2
	fi
	timed_log "$str"
	exit 1
fi
if [ -n "$3" ]
then
	timed_log "$3"
fi
}

function run_test() { # params test_number short_desc test_cmd
TIMED_LOG_SUFFIX=$TEST_NAME"_"$1
timed_log "$2"
eval "$3"
answer=$?
proceed_if_ok $answer "FAIL" "OK"

}
