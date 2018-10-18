#!/bin/bash 

source /usr/local/share/rfsom-box-gui/tests/test_util.sh

TEST_NAME="TEST_OLED"

TEST_ID="01"
SHORT_DESC="random data to fb0 " 
CMD="cat /dev/urandom > /dev/fb0 2>/dev/null;"  #make sure you add ; if multiple commands are issued
CMD+="YES_no 'Is random data displayed on the OLED ? '"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="02"
SHORT_DESC="clear screen"
CMD="cat /dev/zero > /dev/fb0 2>/dev/null;"
CMD+="YES_no 'Is the display cleared ? '"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="03"
SHORT_DESC="red screen"
CMD="python /usr/local/share/rfsom-box-gui/tests/create_fb_data.py 31 0 0 > /dev/fb0;"
CMD+="YES_no 'Is the display filled with red ? '"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="04"
SHORT_DESC="green screen"
CMD="python /usr/local/share/rfsom-box-gui/tests/create_fb_data.py 0 63 0 > /dev/fb0;"
CMD+="YES_no 'Is the display filled with green ? '"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="05"
SHORT_DESC="blue screen"
CMD="python /usr/local/share/rfsom-box-gui/tests/create_fb_data.py 0 0 31 > /dev/fb0;"
CMD+="YES_no 'Is the display filled with blue ? '"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="06"
SHORT_DESC="use fim to output test pattern "
CMD="((/usr/local/bin/fim --device /dev/fb0 -q -p -o fb -a /usr/local/share/rfsom-box-gui/tests/testpattern-160x128.jpg 2>&1) >/dev/null)  & "
CMD+="YES_no 'Can you see a testpattern on the OLED display ?'"
run_test $TEST_ID "$SHORT_DESC" "$CMD"
pid=$(pidof fim)
(kill $pid 2>&1) > /dev/null 
#(wait $pid)
(cat /dev/zero > /dev/fb0) 2>/dev/null

: #if reached this point, ensure exit code 0
