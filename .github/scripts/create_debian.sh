#!/bin/bash

version=$1
architecture=$2

source_code=$(basename "$PWD")

# Install build dependencies
sudo apt update
sudo apt install -y build-essential make devscripts debhelper qtbase5-dev qt5-qmake cmake libfftw3-dev gpsd gpsd-clients bc

# Replace placeholders inside the debian template files
sed -i "s/@VERSION@/$version-1/" packaging/debian/changelog
sed -i "s/@DATE@/$(date -R)/" packaging/debian/changelog
sed -i "s/@ARCHITECTURE@/$architecture/" packaging/debian/control

# Copy debian files to root
cp -r packaging/debian .

# Create the orig tarball
pushd ..
tar czf $source_code\_$version.orig.tar.gz $source_code
$source_code/.github/scripts/install_libiio_deb.sh
popd
# Build the package
debuild
