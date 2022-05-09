#!/bin/sh
set -x
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
	/usr/local/bin/batt_man.sh &
	gpsd -n /dev/ttyPS1;
	echo 972 > /sys/class/gpio/export;
	echo 973 > /sys/class/gpio/export;
	echo 974 > /sys/class/gpio/export;

	echo 0x800000bc 0xffffffff > /sys/kernel/debug/iio/iio\:device3/direct_reg_access
	cfg_path=/usr/local/etc/rfsom-box-gui
	if [ ! -d $cfg_path ]; then
		mkdir $cfg_path 
	fi	
	# Radio config
	if [ -e $cfg_path/radio-ensm_mode ]; then
		iio_attr  -d ad9361-phy ensm_mode $(cat $cfg_path/radio-ensm_mode)
	fi
	if [ -e $cfg_path/radio-rx_lo_freq ]; then
		iio_attr  -c ad9361-phy altvoltage0 frequency $(cat $cfg_path/radio-rx_lo_freq)
	fi
	if [ -e $cfg_path/radio-tx_lo_freq ]; then
		iio_attr  -c ad9361-phy altvoltage1 frequency $(cat $cfg_path/radio-tx_lo_freq)
	fi
	if [ -e $cfg_path/radio-sampling_freq ]; then
		iio_attr  -i -c ad9361-phy voltage0 sampling_frequency $(cat $cfg_path/radio-sampling_freq)
	fi
	if [ -e $cfg_path/radio-rx_rf_bandwidth ]; then
		iio_attr  -i -c ad9361-phy voltage0 rf_bandwidth $(cat $cfg_path/radio-rx_rf_bandwidth)
	fi
	if [ -e $cfg_path/radio-tx_rf_bandwidth ]; then
		iio_attr  -o -c ad9361-phy voltage0 rf_bandwidth $(cat $cfg_path/radio-tx_rf_bandwidth)
	fi
	if [ -e $cfg_path/radio-tx_atten ]; then
		iio_attr  -o -c ad9361-phy voltage0 hardwaregain $(echo "$(cat $cfg_path/radio-tx_atten) * -1" |  bc)
	fi
	if [ -e $cfg_path/radio-rx_gain_ctrl_mode ]; then
		iio_attr  -i -c ad9361-phy voltage0 gain_control_mode $(cat $cfg_path/radio-rx_gain_ctrl_mode)
	fi
	if [ -e $cfg_path/radio-rx_gain ]; then
		iio_attr  -i -c ad9361-phy voltage0 hardwaregain $(cat $cfg_path/radio-rx_gain)
	fi
	if [ ! -e $cfg_path/netcat-param ]; then
		touch $cfg_path/netcat-param
	fi

	if [ -e $cfg_path/radio-xo_correction ]; then
		iio_attr  -d ad9361-phy xo_correction $(cat $cfg_path/radio-xo_correction)
	fi


	#Modem config
	if [ ! -e $cfg_path/modem-ip ]; then
		echo 192.168.23.1 > $cfg_path/modem-ip
	fi	
	if [ ! -e $cfg_path/modem-subnet ]; then
		echo 255.255.255.0 > $cfg_path/modem-subnet
	fi
	if [ ! -e $cfg_path/modem-delay ]; then
		echo 10000 > $cfg_path/modem-delay
	fi	
	if [ ! -e $cfg_path/modem-port ]; then
		echo 2000 > $cfg_path/modem-port
	fi	
	if [ ! -e $cfg_path/stream-ip ]; then
		echo 192.168.23.2 >  $cfg_path/stream-ip
	fi
	if [ ! -e $cfg_path/stream-audio ]; then
		echo 0 >  $cfg_path/stream-audio
	fi
	
	echo "No file selected" > /tmp/buffer_file.txt
	git --git-dir=/usr/local/src/rfsom-box-gui/.git describe --always --tags > /tmp/git_tag
	git --git-dir=/usr/local/src/rfsom-box-gui/.git log -1 --format=%cd > /tmp/git_date
	uname -r > /tmp/unamer
	uname -v > /tmp/unamev
	
	/usr/local/bin/ip_reg_default.sh
	QT_QPA_EVDEV_KEYBOARD_PARAMETERS=/dev/input/by-path/platform-gpio-keys-nav-switch-event:grab=1 \
	QT_QPA_EVDEV_MOUSE_PARAMETERS=/dev/input/by-path/platform-rotary-event:grab=1 \
	QT_QPA_FONTDIR=/home/analog/Qt/fonts \
	QT_QPA_GENERIC_PLUGINS=evdevmouse,evdevkeyboard \
        /usr/local/bin/rfsom-box-gui > /var/log/rfsom-box-gui 2>&1 &
else
set -x
	# could it still be the BOX, with a wrong model?
	iio_attr -a -d | grep -q ad7291-ccbox
	if [ $? -eq 0 ]; then
		command fw_setenv >/dev/null 2>&1
		if [ $? -eq 1 ]; then
			fw_setenv model "Analog Devices ADRV9361-Z7035 RFSOM-BOX (Z7035/AD9361)"
		fi
	fi
fi

