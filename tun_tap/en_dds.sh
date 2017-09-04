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