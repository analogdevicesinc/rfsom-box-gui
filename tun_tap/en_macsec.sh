#/bin/bash

PID=$(pidof wpa_supplicant)
kill -9 $PID
mknod /dev/net/tap c 10 200
sleep 2
sudo wpa_supplicant -i adi_radio -Dmacsec_linux -c /etc/wpa_supplicant.conf &
sleep 2
sudo ip link set macsec0 up
sudo ip addr add 192.168.22.1/24 dev macsec0
