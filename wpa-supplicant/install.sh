#!/bin/sh
killall -9 wpa_supplicant
#install the tools needed by macsec
cd /usr/local/src
rm -rf crypto
mkdir crypto
cd crypto
rm -rf /wpa_supplicant
git clone https://github.com/emaclean-adi/wpa_supplicant.git
cd wpa_supplicant
cp if_macsec.h /usr/include/linux/
chmod +x ./install_wpa_supplicant.sh
#	./install_wpa_supplicant.sh

#install iproute2
cd /usr/local/src
git clone --depth 1 git://git.kernel.org/pub/scm/linux/kernel/git/shemminger/iproute2.git
cd iproute2/
./configure
make
make install

#install libnl
cd /usr/local/src
wget https://github.com/thom311/libnl/releases/download/libnl3_3_0/libnl-3.3.0.tar.gz
tar -xvf libnl-3.3.0.tar.gz
cd libnl-3.3.0
./configure
make
make install
cd ..
rm libnl-3.3.0.tar.gz

#install wpa_supplicant
cd /usr/local/src/crypto
#NEED TO REPLACE THIS WITH A DIFFERENT REPO NOT IN MY PRIVATE REPO
#	rm -rf ./wpa_supplicant
#git clone https://github.com/emaclean-adi/wpa_supplicant.git
cd wpa_supplicant
cp ./wpa_supplicant.conf /etc/wpa_supplicant.conf
cd ./hostap/wpa_supplicant/
make
cp wpa_cli wpa_supplicant /usr/local/bin
chmod 777 /usr/local/bin/wpa_cli
chmod 777 /usr/local/bin/wpa_supplicant

#need to call wpa_supplicant once before relinking libnl libraries below
wpa_supplicant -h

cd /lib/arm-linux-gnueabihf/
rm libnl-3.so.200
ln -s /usr/local/lib/libnl-3.so.200 libnl-3.so.200
rm libnl-genl-3.so.200
ln -s /usr/local/lib/libnl-genl-3.so.200 libnl-genl-3.so.200
cd /usr/lib/arm-linux-gnueabihf/
rm libnl-route-3.so.200
ln -s /usr/local/lib/libnl-route-3.so.200 libnl-route-3.so.200
cd ../..
#	rm -rf ./crypto	

