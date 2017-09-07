#!/bin/sh


/usr/local/bin/fim --device /dev/fb0 -q -o fb -a /usr/local/share/rfsom-box-gui/adi.png > /var/log/fim.log & fimpid=$(echo $!)
ip=$(cat /tmp/stream-ip)
port=$(cat /tmp/modem-port)
/usr/local/bin/ffmpeg -f v4l2 -framerate 20 -video_size 320x240 -i /dev/video0 -r 20 -s 160x128 -movflags faststart -an -vcodec libx264 -preset superfast -tune zerolatency -f avi pipe:1 | netcat $ip $port 
kill $fimpid
