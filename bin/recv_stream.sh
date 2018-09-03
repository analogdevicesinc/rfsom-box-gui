killall -9 netcat 2>/dev/null;
ip=$(cat /usr/local/etc/rfsom-box-gui/modem-ip);
port=$(cat /usr/local/etc/rfsom-box-gui/modem-port);
udp=$(cat /usr/local/etc/rfsom-box-gui/netcat-param);
audio=$(cat /usr/local/etc/rfsom-box-gui/stream-audio);
if [ "$udp" = "" ]
then
(>&2 echo Video connection is TCP)
else
(>&2 echo Video connection is UDP)
fi

scanport=$(($port+1))
(>&2 echo awaiting handshake port $scanport)
netcat -l $ip -p $scanport &
if [ $audio -eq 1 ]
then
netcat -l $udp $ip -p $port| mplayer -fps 15 -vf scale -zoom -x 160 -y 120 -vo fbdev -cache 640 -cache-min 1 - #2>/dev/null
else
netcat -l $udp $ip -p $port| mplayer -fps 15 -vf scale -zoom -x 160 -y 120 -nosound -vo fbdev -framedrop -cache 640 -cache-min 1 - #2>/dev/null
fi
