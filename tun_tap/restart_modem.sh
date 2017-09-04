#!/bin/bash

PID=$(pidof modemd)
kill -9 $PID 
sh cfg_modem.sh

./modemd -a $1 -m $2 -d $3 &
