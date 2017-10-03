#!/bin/bash
#https://serverfault.com/questions/126291/ubuntu-keyboard-detection-from-bash-script
started=0
for dev in /sys/bus/usb/devices/*-*:*
do
  if [ -f $dev/bInterfaceClass ]
  then
    if [[ "$(cat $dev/bInterfaceClass)" == "03" && "$(cat $dev/bInterfaceProtocol)" == "01" ]]
    then
	bterm -f /usr/share/fonts/truetype/droid/DroidSansMono.bgf /bin/bash < /dev/tty0;started=1
    fi
  fi
done

if [ $started -eq 0 ]
then
(>&2 echo "Keyboard not found")
exit 1
fi
