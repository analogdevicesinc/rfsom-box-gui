#/bin/bash

if [ "$(id -u)" != "0" ] ; then
	echo "This script must be run as root"
	exit 1
fi

PID=$(pidof wpa_supplicant)
if [ ! -z "$PID" ] ; then
	kill -9 $PID
fi

if [ -f /dev/net/tap ] ; then
	/bin/mknod /dev/net/tap c 10 200
fi

sleep 2
wpa_supplicant -i adi_radio -Dmacsec_linux -P/var/run/wpa_supplicant.pid -c /etc/wpa_supplicant.conf &
sleep 2
if [ -f /var/run/wpa_supplicant.pid ] ; then
	ip link set macsec0 up
	ip addr add 192.168.22.1/24 dev macsec0
fi
