#!/bin/bash

# Script to build and install libiio from source
# Based on the specified branch

libiio_branch="libiio-v0"

echo "Installing libiio dependencies..."
sudo apt-get -y update
sudo apt-get -y install git build-essential libxml2-dev bison flex libcdk5-dev cmake libaio-dev libusb-1.0-0-dev libserialport-dev libavahi-client-dev doxygen graphviz python3 python3-pip python3-setuptools

echo "Cloning libiio from branch: $libiio_branch"
git clone -b "$libiio_branch" https://github.com/analogdevicesinc/libiio.git
cd libiio

echo "Building libiio..."
mkdir build && cd build
cmake ../ -DCPP_BINDINGS=ON -DPYTHON_BINDINGS=ON -DENABLE_PACKAGING=ON
make -j$(nproc)

echo "Creating libiio packages..."
make package

echo "Installing libiio packages..."
sudo dpkg -i libiio-*.deb

echo "libiio installation completed!"

# Clean up
cd ../..
rm -rf libiio

