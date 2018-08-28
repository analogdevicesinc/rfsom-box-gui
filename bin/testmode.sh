ip=$(cat /usr/local/etc/rfsom-box-gui/stream-ip)
ownip=$(cat /usr/local/etc/rfsom-box-gui/modem-ip)
port=$(cat /usr/local/etc/rfsom-box-gui/modem-port)

(>&2 echo waiting for peer@$ip)
ping $ip -c 1 -W 1
conn=$(echo $?)
while [ $conn -ne 0 ];do
 sleep 1;
  (>&2 echo -n ".")
  ping $ip -c 1 -W 1
  conn=$(echo $?)
done
(>&2 echo peer online)
scanport=$(($port+1))
(>&2 echo handshaking port $scanport)
netcat -v -z $ip $scanport
conn=$(echo $?)
if [ $conn -ne 0 ]; then
 netcat -v -l $ownip -p $scanport 
 (>&2 iperf -s -t 5 )
else
 (>&2 iperf -c $ip -t 5)
fi
killall -9 netcat
killall -9 iperf
