#!/bin/bash

source /usr/local/share/rfsom-box-gui/tests/test_oled.sh
answer=$?
proceed_if_ok $answer

echo 
source /usr/local/share/rfsom-box-gui/tests/test_imu.sh
answer=$?
proceed_if_ok $answer

echo 
source /usr/local/share/rfsom-box-gui/tests/test_nav.sh
answer=$?
proceed_if_ok $answer

echo Tests OK
