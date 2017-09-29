#!/bin/sh


/usr/local/bin/fim --device /dev/fb0 -q -o fb -a /usr/local/share/rfsom-box-gui/live_streaming.jpg > /var/log/fim.log & fimpid=$(echo $!)
ip=$(cat /usr/local/etc/rfsom-box-gui/stream-ip)
port=$(cat /usr/local/etc/rfsom-box-gui/modem-port)
udp=$(cat /usr/local/etc/rfsom-box-gui/netcat-param)
video=$(ls /dev/video* | head -n1 | cut -d " " -f1)
/usr/local/bin/ffmpeg -f v4l2 -framerate 20 -video_size 320x240 -i $video -r 20 -s 160x128 -movflags faststart -an -vcodec libx264 -preset superfast -tune zerolatency -f avi pipe:1 | netcat $udp $ip $port 
kill $fimpid
