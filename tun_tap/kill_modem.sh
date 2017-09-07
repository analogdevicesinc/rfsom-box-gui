#!/bin/bash

PID=$(pidof modemd)
kill -9 $PID
sh cfg_modem.sh
./modemd &
./en_macsec.sh
root@analog:/home/analog/tap1# cat kill_modem.sh
#!/bin/bash

PID=$(pidof modemd)
kill -9 $PID

PID=$(pidof wpa_supplicant)
kill -9 $PID