#!/bin/bash

PID=$(pidof modemd)
kill -9 $PID

PID=$(pidof wpa_supplicant)
kill -9 $PID