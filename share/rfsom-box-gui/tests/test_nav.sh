#!/bin/bash 

source /usr/local/share/rfsom-box-gui/tests/test_util.sh

TEST_NAME="TEST_NAV"

TEST_ID="01"
SHORT_DESC="rotary input exists" 
CMD="timeout 1 evtest /dev/input/by-path/platform-rotary-event | grep 'Input device name: \"rotary\"' > /dev/null"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="02"
SHORT_DESC="gpio-keys-nav-switch input exists" 
CMD="timeout 1 evtest /dev/input/by-path/platform-gpio-keys-nav-switch-event | grep 'Input device name: \"gpio-keys-nav-switch\"' > /dev/null"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="03"
SHORT_DESC="rotary clockwise - timeout 10 seconds" 
CMD="timeout 10 evtest /dev/input/by-path/platform-rotary-event | grep -m 2 \"type 2 (EV_REL), code 8 (REL_WHEEL), value -1\" > /dev/null"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="04"
SHORT_DESC="rotary counter-clockwise - timeout 10 seconds" 
CMD="timeout 10 evtest /dev/input/by-path/platform-rotary-event | grep -m 2 \"type 2 (EV_REL), code 8 (REL_WHEEL), value 1\" > /dev/null"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="05"
SHORT_DESC="double press gpio-keys-nav-switch center button - timeout 10 seconds" 
CMD="timeout 10 evtest /dev/input/by-path/platform-gpio-keys-nav-switch-event | grep -m 2 \"type 1 (EV_KEY), code 28 (KEY_ENTER), value 1\" > /dev/null"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="06"
SHORT_DESC="double press gpio-keys-nav-switch up button - timeout 10 seconds" 
CMD="timeout 10 evtest /dev/input/by-path/platform-gpio-keys-nav-switch-event | grep -m 2 \"type 1 (EV_KEY), code 103 (KEY_UP), value 1\" > /dev/null"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="07"
SHORT_DESC="double press gpio-keys-nav-switch down button - timeout 10 seconds" 
CMD="timeout 10 evtest /dev/input/by-path/platform-gpio-keys-nav-switch-event | grep -m 2 \"type 1 (EV_KEY), code 108 (KEY_DOWN), value 1\" > /dev/null"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="08"
SHORT_DESC="double press gpio-keys-nav-switch left button - timeout 10 seconds" 
CMD="timeout 10 evtest /dev/input/by-path/platform-gpio-keys-nav-switch-event | grep -m 2 \"type 1 (EV_KEY), code 105 (KEY_LEFT), value 1\" > /dev/null"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="09"
SHORT_DESC="double press gpio-keys-nav-switch right button - timeout 10 seconds" 
CMD="timeout 10 evtest /dev/input/by-path/platform-gpio-keys-nav-switch-event | grep -m 2 \"type 1 (EV_KEY), code 106 (KEY_RIGHT), value 1\" > /dev/null"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

: #if reached this point, ensure exit code 0
