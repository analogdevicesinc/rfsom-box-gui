#!/bin/sh
/usr/local/bin/gps_get_coords.sh
lat=$(cat /tmp/gpslat)
lon=$(cat /tmp/gpslon)
key=$(cat /usr/local/share/rfsom-box-gui/gmaps.key)
echo "getting gmap"
wget -O /tmp/gmap.png "https://maps.googleapis.com/maps/api/staticmap?center=$lat,$lon&zoom=13&size=160x128&scale=2&format=png&markers=color:blue|$lat,$lon&key=$key"
echo "calling fim"
/usr/local/bin/fim --device /dev/fb0 -a -q /tmp/gmap.png

