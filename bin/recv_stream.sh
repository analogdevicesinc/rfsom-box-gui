killall -9 netcat 2>/dev/null;
ip=$(cat /usr/local/etc/rfsom-box-gui/modem-ip);
port=$(cat /usr/local/etc/rfsom-box-gui/modem-port);
udp=$(cat /usr/local/etc/rfsom-box-gui/netcat-param);
if [ "$udp" = "" ]
then
(>&2 echo Video connection is TCP)
else
(>&2 echo Video connection is UDP)
fi

scanport=$(($port+1))
(>&2 echo awaiting handshake port $scanport)
netcat -l $ip -p $scanport &
netcat -l $udp $ip -p $port| mplayer -fps 20 -vf scale -zoom -x 160 -y 128 -nosound -vo fbdev -cache 32 -cache-min 1 - 2>/dev/null
