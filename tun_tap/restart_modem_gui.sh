#!/bin/sh

ip=$(cat /usr/local/etc/rfsom-box-gui/modem-ip)
subnet=$(cat /usr/local/etc/rfsom-box-gui/modem-subnet)
delay=$(cat /usr/local/etc/rfsom-box-gui/modem-delay)
echo $ip
echo $subnet
echo $delay

devnr=$(/usr/bin/iio_attr -d | grep cf-ad9361-lpc | wc -l)
if [ $devnr -eq 0 ]
then
        echo cf-ad9361-lpc device not found
	exit
fi

PID=$(pidof modemd)
if [ ! -z "$PID" ] ; then
	kill -9 $PID
fi

# en_radio.sh
/usr/bin/iio_attr -c -i ad9361-phy voltage0 sampling_frequency 20000000
/usr/bin/iio_attr -c -o ad9361-phy voltage0 sampling_frequency 20000000

/usr/bin/iio_attr -c -i ad9361-phy voltage0 rf_bandwidth 20000000
/usr/bin/iio_attr -c -o ad9361-phy voltage0 rf_bandwidth 20000000

/usr/bin/iio_attr -c -i ad9361-phy voltage0 rssi

# en_dds.sh
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40 0x0
echo -n "cf-ad9361-dds-core-lpc reg 0x040 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40 0x2
echo -n "cf-ad9361-dds-core-lpc reg 0x040 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40 0x3
echo -n "cf-ad9361-dds-core-lpc reg 0x040 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x4C 0x3
echo -n "cf-ad9361-dds-core-lpc reg 0x04c : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x4C

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x44 0x0
echo -n "cf-ad9361-dds-core-lpc reg 0x044 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x44

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x48 0x0
echo -n "cf-ad9361-dds-core-lpc reg 0x048 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x48

for i in $(seq 0 3)
do
	/usr/bin/iio_reg cf-ad9361-dds-core-lpc $((0x418 + $i * 0x40)) 0x0
	echo -n "cf-ad9361-dds-core-lpc reg" $(printf "0x%x" $((0x418 + 1 * 0x40))) " : "
	/usr/bin/iio_reg cf-ad9361-dds-core-lpc $((0x418 + $i * 0x40))
done

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x44 0x1
echo -n "cf-ad9361-dds-core-lpc reg 0x044 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x44

# en_dma.sh
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40 0x0
echo -n "cf-ad9361-dds-core-lpc reg 0x040 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40 0x2
echo -n "cf-ad9361-dds-core-lpc reg 0x040 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40 0x3
echo -n "cf-ad9361-dds-core-lpc reg 0x040 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x40

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x4c 0x3
echo -n "cf-ad9361-dds-core-lpc reg 0x04c : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x4c

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x44 0x0
echo -n "cf-ad9361-dds-core-lpc reg 0x044 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x44

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x48 0x0
echo -n "cf-ad9361-dds-core-lpc reg 0x048 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x48

for i in $(seq 0 3)
do
	/usr/bin/iio_reg cf-ad9361-dds-core-lpc $((0x418 + $i * 0x40)) 0x2
	echo -n "cf-ad9361-dds-core-lpc reg" $(printf "0x%x" $((0x418 + 1 * 0x40))) " : "
	/usr/bin/iio_reg cf-ad9361-dds-core-lpc $((0x418 + $i * 0x40))
done

/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x44 0x1
echo -n "cf-ad9361-dds-core-lpc reg 0x044 : "
/usr/bin/iio_reg cf-ad9361-dds-core-lpc 0x44

#load a custom FIR
if [ -f /usr/local/share/rfsom-box-gui/modem_filter.ftr ] ; then
	/usr/bin/iio_attr -c -i ad9361-phy voltage0 filter_fir_en 0
	echo "loading filter file"
	cat /usr/local/share/rfsom-box-gui/modem_filter.ftr > /sys/bus/iio/devices/iio:device1/filter_fir_config
	/usr/bin/iio_attr -c -i ad9361-phy voltage0 filter_fir_en 1
fi

#start modem daemon
/usr/local/bin/modemd -a $ip -m $subnet -d $delay -n tap & 
/usr/local/bin/en_macsec.sh
/usr/local/bin/ip_reg -x

