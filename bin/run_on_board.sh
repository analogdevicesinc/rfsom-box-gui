#!/bin/sh

service lightdm stop

for i in $(ls /sys/class/input/event*/device/name)
do
	if [ "$(cat ${i})" = "gpio-keys-nav-switch" ] ; then
		nav=$(echo /dev${i} | sed -e 's:/device/name::' -e 's:/sys/class::')
	fi
	if [ "$(cat ${i})" = "rotary" ] ; then
		rotary=$(echo /dev${i} | sed -e 's:/device/name::' -e 's:/sys/class::')
	fi
	
done
echo NAV at $nav
echo Rotary at $rotary

export QT_QPA_EVDEV_KEYBOARD_PARAMETERS=${nav}:grab=1
export QT_QPA_EVDEV_MOUSE_PARAMETERS=${rotary}:grab=1
export QT_QPA_FB_DRM=1
export QT_QPA_FB_HIDECURSOR=0
export QT_QPA_FB_TSLIB=1
#export QT_QPA_FONTDIR=/home/analog/Qt/fonts
export QT_QPA_FONTDIR=/usr/share/fonts
export QT_QPA_PLATFORM=linuxfb
export QT_QPA_GENERIC_PLUGINS=evdevmouse,evdevkeyboard

./blueScorpionApp -platform linuxfb
