#!/bin/bash 

source /usr/local/share/rfsom-box-gui/tests/test_util.sh

TEST_NAME="TEST_IMU"


TEST_ID="1"
SHORT_DESC="adis16460 IIO device exists" 
CMD="pid=\$(iio_attr -q -D adis16460 product_id);"
CMD+="test \$pid == \"16460\""
echo $CMD
run_test $TEST_ID "$SHORT_DESC" "$CMD"


TEST_ID="2"
SHORT_DESC="place device on the table with the display UP [timeout in 10 seconds] " 
CMD="timed_log '/usr/local/share/rfsom-box-gui/test_imu.py accel_z \<-9.6';"
CMD+="/usr/local/share/rfsom-box-gui/tests/test_imu.py accel_z \<-9.6"
run_test $TEST_ID "$SHORT_DESC" "$CMD"


TEST_ID="3"
SHORT_DESC="place device on the table with the display to the LEFT [timeout in 10 seconds] " 
CMD="timed_log '/usr/local/share/rfsom-box-gui/test_imu.py accel_x \>9.6';"
CMD+="/usr/local/share/rfsom-box-gui/tests/test_imu.py accel_x \>9.6"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

TEST_ID="3"
SHORT_DESC="rotate the device so antennas are towards the ground" 
CMD="timed_log '/usr/local/share/rfsom-box-gui/test_imu.py accel_y \>9.6';"
CMD+="/usr/local/share/rfsom-box-gui/tests/test_imu.py accel_y \>9.6"
run_test $TEST_ID "$SHORT_DESC" "$CMD"

return 1
