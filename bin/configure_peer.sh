#!/bin/bash

SSH_IP=$(cat /usr/local/etc/rfsom-box-gui/peer-cal-ip)
SSH_HOST="root@"$SSH_IP
SSH_PASS="analog"
SSH_ARGS="-o StrictHostKeyChecking=no"
log_in_remote() {
   sshpass -p $SSH_PASS "$@"
}

a=(modem-ip radio-rx_lo_freq radio-sampling_freq radio-xo_correction modem-ip)
b=(stream-ip radio-tx_lo_freq radio-sampling_freq radio-xo_correction stream-ip)

# Set default values
echo 900000000 > /usr/local/etc/rfsom-box-gui/radio-rx_lo_freq
echo 950000000 > /usr/local/etc/rfsom-box-gui/radio-tx_lo_freq
echo 20000000 > /usr/local/etc/rfsom-box-gui/radio-sampling_freq
echo 40000000 > /usr/local/etc/rfsom-box-gui/radio-xo_correction
echo "192.168.23.1" > /usr/local/etc/rfsom-box-gui/modem-ip
echo "192.168.23.2" > /usr/local/etc/rfsom-box-gui/stream-ip

# Update IPs
for i in "${!a[@]}"; do
  printf "%s\t%s\n" "$i" "${foo[$i]}"
  echo "scp /usr/local/etc/rfsom-box-gui/${b[$i]} $SSH_IP:/usr/local/etc/rfsom-box-gui/${a[$i]}"
  log_in_remote scp $SSH_ARGS /usr/local/etc/rfsom-box-gui/${b[$i]} $SSH_IP:/usr/local/etc/rfsom-box-gui/${a[$i]}
  echo "scp /usr/local/etc/rfsom-box-gui/${a[$i]} $SSH_IP:/usr/local/etc/rfsom-box-gui/${b[$i]}"
  log_in_remote scp $SSH_ARGS /usr/local/etc/rfsom-box-gui/${a[$i]} $SSH_IP:/usr/local/etc/rfsom-box-gui/${b[$i]}
done

iio_attr -u ip:$SSH_IP -c ad9361-phy altvoltage1 frequency $(cat /usr/local/etc/rfsom-box-gui/radio-tx_lo_freq)
iio_attr -u ip:$SSH_IP -c ad9361-phy altvoltage0 frequency $(cat /usr/local/etc/rfsom-box-gui/radio-rx_lo_freq)
iio_attr -u ip:$SSH_IP -d ad9361-phy xo_correction $(cat /usr/local/etc/rfsom-box-gui/radio-xo_correction)
iio_attr -u ip:localhost -c ad9361-phy altvoltage1 frequency $(cat /usr/local/etc/rfsom-box-gui/radio-rx_lo_freq)
iio_attr -u ip:localhost -c ad9361-phy altvoltage0 frequency $(cat /usr/local/etc/rfsom-box-gui/radio-tx_lo_freq)
iio_attr -u ip:localhost -d ad9361-phy xo_correction $(cat /usr/local/etc/rfsom-box-gui/radio-xo_correction)

sleep 3

echo "Restarting GUI"
log_in_remote ssh $SSH_ARGS $SSH_HOST '/usr/local/bin/rfsom-box-gui-start.sh'

echo "Calibrating"
log_in_remote ssh $SSH_ARGS $SSH_HOST '/usr/local/bin/send_tone.sh'
/usr/local/bin/send_tone.sh

echo "Restarting MODEM Remotely"
log_in_remote ssh $SSH_ARGS $SSH_HOST '/usr/local/bin/restart_modem_gui.sh >/tmp/modem.log 2>&1' &

echo "Restarting MODEM Locally"
/usr/local/bin/restart_modem_gui.sh >/tmp/modem.log 2>&1
