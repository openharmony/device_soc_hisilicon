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

MBEDTLS_DIR=$1
MBEDTLS_VER=$2
PATCH=$4
cmd=$3
if [[ $cmd == PATCH ]]; then
	if [ ! -d $MBEDTLS_DIR/$MBEDTLS_VER ]; then
		tar -zxf $MBEDTLS_DIR/$MBEDTLS_VER.tar.gz -C $MBEDTLS_DIR;
		if [ -f $PATCH ]; then
		        patch -p1 -d $MBEDTLS_DIR/$MBEDTLS_VER < $PATCH;
        	fi
	fi
else
	if [ -e $MBEDTLS_DIR/$MBEDTLS_VER.tar.gz ]; then
        	rm -rf $MBEDTLS_DIR/$MBEDTLS_VER;
	fi
fi


