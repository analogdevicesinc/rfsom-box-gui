peer_ip = $("root@"$(cat /usr/local/etc/rfsom-box-gui/peer-cal-ip))
echo $peer_ip
#ping $peer_ip -c 1 -W 1 > /dev/null;
#conn=$(echo $?);echo $conn > /tmp/peer-connectivity;
#if [ $conn -eq 0 ]; 
#then 
#echo Peer ON > /tmp/peer-connectivity-str; 
#else 
#echo Peer OFF > /tmp/peer-connectivity-str; 
#fi;
ssh $peer_ip 'echo "analog"'| 'sudo -Sv && bash -s' < /usr/local/share/configure_peer_remote.sh
