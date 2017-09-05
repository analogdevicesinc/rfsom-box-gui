#!/bin/sh

ip=$(cat /tmp/modem-ip)
subnet=$(cat /tmp/modem-subnet)
delay=$(cat /tmp/modem-delay)
echo $ip
echo $subnet
echo $delay


PID=$(pidof modemd)
kill -9 $PID 

# en_radio.sh
echo 20000000 > /sys/bus/iio/devices/iio:device1/in_voltage_sampling_frequency
cat /sys/bus/iio/devices/iio:device1/in_voltage_sampling_frequency

echo 20000000 > /sys/bus/iio/devices/iio:device1/out_voltage_sampling_frequency
cat /sys/bus/iio/devices/iio:device1/out_voltage_sampling_frequency

echo 20000000 > /sys/bus/iio/devices/iio:device1/in_voltage_rf_bandwidth
echo 20000000 > /sys/bus/iio/devices/iio:device1/out_voltage_rf_bandwidth
cat /sys/bus/iio/devices/iio:device1/in_voltage_rf_bandwidth
cat /sys/bus/iio/devices/iio:device1/out_voltage_rf_bandwidth

cat /sys/bus/iio/devices/iio:device1/in_voltage0_rssi
# en_dds.sh
echo 0x40 0x0 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access
echo 0x40 0x2 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access
echo 0x40 0x3 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access

echo 0x4C 0x3 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access

echo 0x44 0x0 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access

echo 0x48 0x0 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access
for i in $(seq 0 3)
do
	echo $((0x418 + $i * 0x40)) 0 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
	cat /sys/kernel/debug/iio/iio:device3/direct_reg_access
done

echo 0x44 0x1 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access
# en_dma.sh
echo 0x40 0x0 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access
echo 0x40 0x2 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access
echo 0x40 0x3 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access

echo 0x4C 0x3 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access

echo 0x44 0x0 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access

echo 0x48 0x0 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access
for i in $(seq 0 3)
do
	echo $((0x418 + $i * 0x40)) 2 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
	cat /sys/kernel/debug/iio/iio:device3/direct_reg_access
done

echo 0x44 0x1 > /sys/kernel/debug/iio/iio:device3/direct_reg_access
cat /sys/kernel/debug/iio/iio:device3/direct_reg_access

/usr/local/bin/modemd -a $ip -m $subnet -d $delay &
echo Done
