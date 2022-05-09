#!/bin/bash 

source /usr/local/share/rfsom-box-gui/tests/test_util.sh

TEST_NAME="TEST_IMU"

TEST_ID="01"
SHORT_DESC="adis16460 IIO device exists" 
CMD="pid=\$(iio_attr  -D adis16460 product_id);"
CMD+="test \$pid == \"16460\""
run_test $TEST_ID "$SHORT_DESC" "$CMD"


TEST_ID="02"
SHORT_DESC="place device on the table with the display UP - timeout 10 seconds" 
CMD="timed_log '/usr/local/share/rfsom-box-gui/test_imu.py accel_z \<-9.6';"
CMD+="/usr/local/share/rfsom-box-gui/tests/test_imu.py accel_z \<-9.6"
run_test $TEST_ID "$SHORT_DESC" "$CMD"


TEST_ID="03"
SHORT_DESC="place device on the table with the display to the LEFT - timeout 10 seconds" 
CMD="timed_log '/usr/local/share/rfsom-box-gui/test_imu.py accel_x \>9.6';"
CMD+="/usr/local/share/rfsom-box-gui/tests/test_imu.py accel_x \>9.6"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="04"
SHORT_DESC="rotate the device so antennas are towards the ground - timeout 10 seconds" 
CMD="timed_log '/usr/local/share/rfsom-box-gui/test_imu.py accel_y \>9.6';"
CMD+="/usr/local/share/rfsom-box-gui/tests/test_imu.py accel_y \>9.6"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

: #if reached this point, ensure exit code 0

