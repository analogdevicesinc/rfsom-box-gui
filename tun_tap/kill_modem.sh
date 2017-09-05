#!/bin/bash

PID=$(pidof modemd)
kill -9 $PID 
