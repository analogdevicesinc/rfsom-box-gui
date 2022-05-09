#!/bin/sh

gain=$(/usr/bin/iio_attr  -c -o ad9361-phy voltage0 hardwaregain | awk '{print $1}')
tx_lo=$(/usr/bin/iio_attr  -c ad9361-phy TX_LO frequency)
tx_sps=$(/usr/bin/iio_attr  -c -o ad9361-phy voltage0 sampling_frequency)

echo $tx_lo
echo $tx_sps
tone=$(expr $tx_sps / 8 + $tx_lo)

# Turn the attenuation down
/usr/bin/iio_attr  -c -o ad9361-phy voltage0 hardwaregain -5

# https://wiki.analog.com/resources/tools-software/linux-drivers/iio-transceiver/ad9361#bist_tone
# <Injection Point> <Tone Frequency> <Tone Level> <Channel Mask>
# Inject 0dBFS tone at Fsample/32 into TX (all channels enabled)
/usr/bin/iio_attr  -D ad9361-phy bist_tone "1 3 6 0"
# 3	FSample / 8

/usr/local/bin/cal_ad9361 -u ip:$(cat /usr/local/etc/rfsom-box-gui/peer-cal-ip) -e ${tone}

/usr/bin/iio_attr  -D ad9361-phy bist_tone "0 0 0 0"
/usr/bin/iio_attr  -c -o ad9361-phy voltage0 hardwaregain ${gain}

