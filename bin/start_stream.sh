#!/bin/sh


ip=$(cat /usr/local/etc/rfsom-box-gui/stream-ip)
port=$(cat /usr/local/etc/rfsom-box-gui/modem-port)
udp=$(cat /usr/local/etc/rfsom-box-gui/netcat-param)
audio=$(cat /usr/local/etc/rfsom-box-gui/stream-audio)

(>&2 echo searching for video)
video=$(ls /dev/video* | head -n1 | cut -d " " -f1) 2>/dev/null 

if [ "$video" = "" ]
then
  (>&2 echo /dev/video* not found)
  exit 1
else
  (>&2 echo found $video)
fi

if [ "$udp" = "" ]
then
(>&2 echo Video connection is TCP)
else
(>&2 echo Video connection is UDP)
fi

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

netcat -z $ip $scanport
conn=$(echo $?)
while [ $conn -ne 0 ];do
  sleep 2
  (>&2 echo -n ".")
  netcat -z $ip $scanport
  conn=$(echo $?)
done

(>&2 echo "Started the stream")

if [ $audio -eq 1 ]
then
(>&2 echo "also streaming audio")

/root/ffmpeg_sources/ffmpeg/ffmpeg -f alsa -ac 1 -ar 8000 -sample_fmt s32_le -i default -f v4l2 -framerate 20 -video_size 320x240 -i $video -r 20 -s 160x128 -movflags faststart -c:a aac -vcodec libx264 -pix_fmt yuv420p -preset superfast -tune zerolatency -f avi pipe:1 | netcat $udp $ip $port  
 
else
/usr/local/bin/ffmpeg -f v4l2 -framerate 20 -video_size 320x240 -loglevel panic -i $video -r 20 -s 160x128 -movflags faststart -an -vcodec libx264 -pix_fmt yuv420p -preset superfast -tune zerolatency -f avi pipe:1 | netcat $udp $ip $port  
fi
echo "end"
