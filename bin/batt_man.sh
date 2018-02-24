#!/bin/bash

dev=/sys/class/power_supply/ltc2942

if [ ! -f $dev/voltage_now ] ; then
	echo ltc2942 not installed
	exit 1
fi

if [ "$(id -u)" != "0" ] ; then
        echo "This script must be run as root"
        exit 1
fi

if [ $(i2cdetect -y 0 0x14 0x14 | grep -e "^10:" | grep 14 | wc -l) -eq "0" ] ; then
	echo "Could not find ADP5061"
	exit 1
fi

me="$(basename "$(test -L "$0" && readlink "$0" || echo "$0")")"

for pid in $(pidof -x ${me}); do
	if [ ${pid} != $$ ]; then
		echo killing duplicate process ${pid}
		kill -9 ${pid}
	fi
done

previous_mode=foo
while true; do
	voltage=`echo "scale=5;$(cat $dev/voltage_now) / 1000000" | bc`
	# Get the ADP5061 Linear Battery Charger status
	# this is a super dirty hack, since a driver doesn't exist yet.
	status=`i2cget -y 0 0x14 0xB`

	case $(($status & 0x7)) in
		0)  mode="Off" ;;
		1)  mode="Trickle Charge" ;;
		2)  mode="Fast Charge Constant Current" ;;
		3)  mode="Fast Charge Constant Voltage" ;;
		4)  mode="Charge Complete" ;;
		5)  mode="LDO mode" ;;
		6)  mode="Charge Timer Expired" ;;
		7)  mode="Battery Detection" ;;
	esac

	case $(($status & 0x7)) in
		0)  soc=`echo "p=160.96*$voltage-549.82;scale=0; if (p > 100) {100} else if (p < 0) {0} else {p/1}" | bc` ;;
		*)  soc=`echo "if ($voltage < 3.72) {p=11.3472 * ($voltage - 3.27)} else {p=188.717*$voltage-697};scale=0;if (p > 100) {100} else if (p < 0) {0} else {p/1}" | bc` ;;
	esac

	#echo $voltage $mode $soc%

	echo "$voltage" > /tmp/rfsom_batt_voltage
	echo "$soc%" > /tmp/rfsom_batt_soc
	echo "$mode" > /tmp/rfsom_batt_mode

	if [ "$mode" = "Charge Complete" -a \
	     "$previous_mode" = "Fast Charge Constant Voltage" ]; then
		echo 3200000 > $dev/charge_now
	fi

	previous_mode=$mode

	if [ $(echo "$voltage < 3.3" | bc -l) -eq "1" -a \
	     "$mode" = "Off" ]; then
		echo 0 > $dev/charge_now
		poweroff
	fi

	sleep 10
done
