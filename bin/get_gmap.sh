#!/bin/sh
(>&2 echo getting GPS coords)
/usr/local/bin/gps_get_coords.sh
lat=$(cat /tmp/gpslat)
lon=$(cat /tmp/gpslon)
(>&2 echo $lat, $lon)
key=$(cat /usr/local/share/rfsom-box-gui/gmaps.key)
if [ "$key" = "paste-google-api-key-here" ];
then
(>&2 echo Put google api key in /usr/local/share/rfsom-box-gui/gmaps.key)
exit 1
fi;
(>&2 echo reaching google)
ping www.google.com -c 1 -W 1
con=$(echo $?)

if [ $con -eq 0 ];
then
(>&2 echo OK );
else
(>&2 echo Not OK );
fi

(>&2 echo "getting map")
wget -O /tmp/gmap.png "https://maps.googleapis.com/maps/api/staticmap?center=$lat,$lon&zoom=13&size=160x128&scale=2&format=png&markers=color:blue|$lat,$lon&key=$key" 2>/dev/null
(>&2 echo "showing image")
/usr/local/bin/fim --device /dev/fb0 -a -q /tmp/gmap.png 2>/dev/null

