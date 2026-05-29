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

KERN_DIR=$1
MBEDTLS_DIR=$2
KERN_MPP_DIR=$KERN_DIR/drivers/vendor/mpp
KERN_CIPHER_DIR=$KERN_MPP_DIR/cipher
KERN_MBEDTLS_DIR=$KERN_CIPHER_DIR/mbedtls
SDK_CIPHER_DIR=$(pwd)/..
if [ ! -e $KERN_MPP_DIR ]; then
	echo "Kernel mpp directory $KERN_MPP_DIR isn't exist!"
	exit -1
fi

if [ ! -e $MBEDTLS_DIR ]; then
    echo "Mbedtls directory $MBEDTLS_DIR ins't exist!"
    exit -1
fi

mkdir -p $KERN_CIPHER_DIR
mkdir -p $KERN_CIPHER_DIR/ext_inc
mkdir -p $KERN_MBEDTLS_DIR
mkdir -p $KERN_MBEDTLS_DIR/src
mkdir -p $KERN_MBEDTLS_DIR/include

cp -r $SDK_CIPHER_DIR/ext_inc/* $KERN_CIPHER_DIR/ext_inc
cp -r $SDK_CIPHER_DIR/v3 $KERN_CIPHER_DIR/
cp -r $SDK_CIPHER_DIR/../copy_inc/* $KERN_CIPHER_DIR/ext_inc/

cp $MBEDTLS_DIR/library/*.c $KERN_MBEDTLS_DIR/src
cp -r $MBEDTLS_DIR/include/* $KERN_MBEDTLS_DIR/include

cp build.mak.mbedtls $KERN_MBEDTLS_DIR/src/build.mak
cp Makefile $KERN_CIPHER_DIR/
