echo 20000000 > /sys/bus/iio/devices/iio:device1/in_voltage_sampling_frequency
cat /sys/bus/iio/devices/iio:device1/in_voltage_sampling_frequency

echo 20000000 > /sys/bus/iio/devices/iio:device1/out_voltage_sampling_frequency
cat /sys/bus/iio/devices/iio:device1/out_voltage_sampling_frequency

echo 950000000 > /sys/bus/iio/devices/iio:device1/out_altvoltage0_RX_LO_frequency
echo 900000000 > /sys/bus/iio/devices/iio:device1/out_altvoltage1_TX_LO_frequency
cat /sys/bus/iio/devices/iio:device1/out_altvoltage0_RX_LO_frequency
cat /sys/bus/iio/devices/iio:device1/out_altvoltage1_TX_LO_frequency

echo 20000000 > /sys/bus/iio/devices/iio:device1/in_voltage_rf_bandwidth
echo 20000000 > /sys/bus/iio/devices/iio:device1/out_voltage_rf_bandwidth
cat /sys/bus/iio/devices/iio:device1/in_voltage_rf_bandwidth
cat /sys/bus/iio/devices/iio:device1/out_voltage_rf_bandwidth

cat /sys/bus/iio/devices/iio:device1/in_voltage0_rssi