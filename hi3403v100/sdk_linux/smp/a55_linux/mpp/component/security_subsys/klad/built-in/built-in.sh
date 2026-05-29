
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
KERN_MPP_DIR=$KERN_DIR/drivers/vendor/mpp
KERN_KLAD_DIR=$KERN_MPP_DIR/klad
SDK_KLAD_DIR=$(pwd)/..
if [ ! -e $KERN_MPP_DIR ]; then
	echo "Kernel mpp directory $KERN_MPP_DIR isn't exist!"
	exit -1
fi
mkdir -p $KERN_KLAD_DIR
cp -a $SDK_KLAD_DIR/include $KERN_KLAD_DIR/
cp -a $SDK_KLAD_DIR/src $KERN_KLAD_DIR/
rm -rf $KERN_KLAD_DIR/src/mpi
rm -rf $KERN_KLAD_DIR/src/linux.mak
cp Makefile $KERN_KLAD_DIR/src/
cp -r $SDK_KLAD_DIR/../copy_inc $KERN_KLAD_DIR/ext_inc
