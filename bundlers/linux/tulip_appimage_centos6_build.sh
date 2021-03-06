#!/bin/bash

cd

# clean packages cache
yum -y clean all
yum -y clean expire-cache
yum -y update

# install base build system
yum -y install epel-release
yum -y install xz cmake tar gzip make wget ccache

# install GCC 4.8.2 as Tulip requires a C++11 compiler
wget https://people.centos.org/tru/devtools-2/devtools-2.repo -O /etc/yum.repos.d/devtools-2.repo
yum -y upgrade
yum -y install devtoolset-2-gcc devtoolset-2-binutils devtoolset-2-gcc-c++

# install tulip deps
yum -y install zlib-devel qhull-devel cppunit-devel
yum -y install freetype-devel libpng-devel libjpeg-devel glew-devel

# needed for generating the AppImage
yum -y install fuse fuse-libs

# set USR_LIB (needed to set CMAKE_PREFIX_PATH)
if [ "$(uname -p)" = "x86_64" ]; then
  USR_LIB=/usr/lib64
else
  USR_LIB=/usr/lib
fi

if [ "$3" == "" ]; then
  # install qt5
  yum install -y qt5-qtbase-devel qt5-qtwebkit-devel
  QT_PATH=$USR_LIB/qt5
else
  # we can use our own build of qt5
  QT_PATH=$3
fi

# install Python 3.6 from the IUS Community Project
wget https://centos6.iuscommunity.org/ius-release.rpm
rpm -Uvh ius-release*rpm
yum -y install python36u-devel python36u-pip
pip3.6 install sphinx

# build and install tulip
if [ -d /tulip/build ]; then
  rm -rf /tulip/build
fi
mkdir /tulip/build
cd /tulip/build
if [ "$1" == "NO_CCACHE" ]; then
  CCACHE=OFF
else
  CCACHE=ON
fi
if [ "$2" == "RUN_TESTS" ]; then
  RUN_TESTS=ON
else
  RUN_TESTS=OFF
fi

cmake -DCMAKE_C_COMPILER=/opt/rh/devtoolset-2/root/usr/bin/gcc -DCMAKE_CXX_COMPILER=/opt/rh/devtoolset-2/root/usr/bin/g++ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/install -DCMAKE_PREFIX_PATH=$QT_PATH -DPYTHON_EXECUTABLE=/usr/bin/python3.6 -DTULIP_USE_CCACHE=$CCACHE -DTULIP_BUILD_FOR_APPIMAGE=ON -DTULIP_BUILD_TESTS=$RUN_TESTS ..
make -j4 install

# run unit tests
if [ "$2" == "RUN_TESTS" ]; then
  make runTests
fi

# build a bundle dir suitable for AppImageKit
sh bundlers/linux/make_appimage_bundle.sh --appdir $PWD

# get appimagetool
wget "https://github.com/probonopd/AppImageKit/releases/download/continuous/appimagetool-$(uname -p).AppImage"
chmod a+x appimagetool-$(uname -p).AppImage

# finally build the portable app
./appimagetool-$(uname -p).AppImage Tulip.AppDir Tulip-$(sh tulip-config --version)-$(uname -p).AppImage
