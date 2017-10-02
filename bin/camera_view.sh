camnr=$(ls /dev -1 | grep video | wc -l)

if [ $camnr -gt 0 ]; then
mplayer -vf scale -zoom -xy 160 -nosound -vo fbdev -tv driver=v4l2:device=$(ls /dev/video* | head -n1 | cut -d " " -f1) tv://;
else
(>&2 echo "No webcam found");
exit 1;
fi
