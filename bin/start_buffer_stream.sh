#!/bin/sh
devnr=$(/usr/bin/iio_attr -d | grep cf-ad9361-dds | wc -l)
filename=$(cat /tmp/buffer_file.txt)
filepath=$(echo /usr/local/share/rfsom-box-gui/buffer)
fullfilepath=$(echo $filepath/$filename)
nrofsamples=$(expr $(wc -c < $fullfilepath) / 4)

modemon=$(echo 0x800000bc > /sys/kernel/debug/iio/iio\:device3/direct_reg_access;tmp=$(($(cat /sys/kernel/debug/iio/iio\:device3/direct_reg_access)));if [ $tmp -eq 0 ]; then echo 0; else echo 1; fi)

if [ $devnr -eq 0 ]
then
        echo cf-ad9361-dds device not found
	exit
fi
if [ $modemon -eq 0 ];
then
	killall -9 iio_writedev
	if [ $filename != "stop" ];
	then
		cat $fullfilepath | /usr/bin/iio_writedev -a -c -b $nrofsamples cf-ad9361-dds-core-lpc voltage0 voltage1 &
	fi
fi
