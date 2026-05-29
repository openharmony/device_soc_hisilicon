#!/bin/bash
# Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see
# <https://www.gnu.org/licenses/>.

KERNEL_VERSION=linux-4.19.90
KERNEL_NAME=linux-4.19.y

MBEDTLS_VERSION=mbedtls-2.16.10

SECURITY_SUBSYS_DIR=`pwd`/..
OPEN_SOURCE_DIR=$SECURITY_SUBSYS_DIR/../../../../../open_source/
KERN_DIR=$OPEN_SOURCE_DIR/linux/$KERNEL_NAME
MBEDTLS_DIR=$OPEN_SOURCE_DIR/mbedtls/$MBEDTLS_VERSION
KERN_VENDOR_DIR=$KERN_DIR/drivers/vendor

# build kernel dir
if [ -d $KERN_DIR ]; then
    if [ -d $KERN_VENDOR_DIR ]; then
        echo "$KERN_VENDOR_DIR exist!!!"
    else    # need patch
        pushd $KERN_DIR;
        patch -p1 < ../$KERNEL_VERSION.patch
        popd;
    fi
else    # need decompress the tar and patch
    pushd $OPEN_SOURCE_DIR/linux;
        tar xf $KERNEL_VERSION.tar.gz; mv $KERNEL_VERSION $KERNEL_NAME; cd $KERNEL_NAME;
        patch -p1 < ../$KERNEL_VERSION.patch;
    popd;
fi

# build mbedtls dir
if [ -d $MBEDTLS_DIR ]; then
    echo "$MBEDTLS_DIR exist!!!"
else    # need decompress the tar and patch
    pushd $OPEN_SOURCE_DIR/mbedtls;
        tar xf $MBEDTLS_VERSION.tar.gz; cd $MBEDTLS_VERSION;
        patch -p1 < ../vendor_$MBEDTLS_VERSION.patch
    popd;
fi

pushd $KERN_DIR;
    # osal
    mkdir -p drivers/vendor/osal;
    cp -r ../../../smp/a55_linux/osal/include  drivers/vendor/osal/;
    cp -r ../../../smp/a55_linux/osal/linux/*  drivers/vendor/osal/;
    
    # sysconfig
    mkdir -p drivers/vendor/sysconfig;
    cp ../../../smp/a55_linux/interdrv/sysconfig/*  drivers/vendor/sysconfig/;

    # mpp
    mkdir -p drivers/vendor/mpp
    cp -r ../../../smp/a55_linux/mpp/out/include  drivers/vendor/mpp
    cp -r ../../../smp/a55_linux/mpp/out/init  drivers/vendor/mpp
    cp ../../../smp/a55_linux/mpp/out/obj/*  drivers/vendor/mpp
    cp -r ../../../smp/a55_linux/mpp/cbb/isp/ext_inc  drivers/vendor/mpp
popd;

# cipher built-in
pushd $SECURITY_SUBSYS_DIR/cipher/built-in;
    chmod +x built-in.sh;
    ./built-in.sh $KERN_DIR $MBEDTLS_DIR
popd;

# klad built-in
pushd $SECURITY_SUBSYS_DIR/klad/built-in;
    chmod +x built-in.sh;
    ./built-in.sh $KERN_DIR
popd;

# modify kernel
chmod +x modify_kernel.sh;
./modify_kernel.sh $KERN_DIR
