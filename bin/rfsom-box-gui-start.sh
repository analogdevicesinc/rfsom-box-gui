#!/bin/sh
grep -q "RFSOM-BOX" /sys/firmware/devicetree/base/model
if [ $? -eq 0 ]; then

	case "$(pidof rfsom-box-gui | wc -w)" in

	0)  echo "Restarting rfsom-box-gui:     $(date)" >> /var/log/rfsom-box-gui-start.txt
	    ;;
	1)  # all ok
	    exit ;;
	*)  echo "Removed double rfsom-box-gui: $(date)" >> /var/log/rfsom-box-gui-start.txt
	    kill $(pidof rfsom-box-gui | awk '{print $1}')
	    exit
	    ;;
	esac
	sudo service lightdm stop;
	gpsd -n /dev/ttyPS1;
	echo 972 > /sys/class/gpio/export;
	echo 973 > /sys/class/gpio/export;
	echo 974 > /sys/class/gpio/export;
	QT_QPA_EVDEV_KEYBOARD_PARAMETERS=/dev/input/by-path/platform-gpio-keys-nav-switch-event:grab=1 \
	QT_QPA_EVDEV_MOUSE_PARAMETERS=/dev/input/by-path/platform-rotary-event:grab=1 \
	QT_QPA_FB_DRM=1 \
	QT_QPA_FB_HIDECURSOR=0 \
	QT_QPA_FB_TSLIB=1 \
	QT_QPA_FONTDIR=/home/analog/Qt/fonts \
	QT_QPA_PLATFORM=linuxfb \
	QT_QPA_GENERIC_PLUGINS=evdevmouse,evdevkeyboard \
	FRAMEBUFFER=/dev/fb0 \
        /usr/local/bin/rfsom-box-gui > /var/log/rfsom-box-gui 2>&1 &
fi
