#!/bin/bash

PID=$(pidof modemd)
kill -9 $PID
sh cfg_modem.sh
./modemd &
./en_macsec.sh
