#!/bin/sh
grep -q "RFSOM-BOX" /sys/firmware/devicetree/base/model
if [ $? -eq 0 ]; then
	sudo service lightdm stop;
	QT_QPA_EVDEV_KEYBOARD_PARAMETERS=/dev/input/by-path/platform-gpio-keys-nav-switch-event:grab=1 \
	QT_QPA_EVDEV_MOUSE_PARAMETERS=/dev/input/by-path/platform-rotary-event:grab=1 \
	QT_QPA_FB_DRM=1 \
	QT_QPA_FB_HIDECURSOR=0 \
	QT_QPA_FB_TSLIB=1 \
	QT_QPA_FONTDIR=/home/analog/Qt/fonts \
	QT_QPA_PLATFORM=linuxfb \
	QT_QPA_GENERIC_PLUGINS=evdevmouse,evdevkeyboard \
	/usr/local/bin/rfsom-box-gui;
fi
