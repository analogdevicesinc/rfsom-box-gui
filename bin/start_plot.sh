devnr=$(cat /sys/bus/iio/devices/iio\:device*/name | grep cf-ad9361-lpc | wc -l)
if [ $devnr -eq 1 ]
then
	/usr/local/bin/fft_plot
else
	(>&2 echo cf-ad9361-lpc device not found)
	exit 1
fi
