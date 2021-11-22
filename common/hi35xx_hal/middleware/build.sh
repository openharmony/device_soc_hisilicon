#!/bin/bash
# Copyright (c) 2021 HiSilicon (Shanghai) Technologies CO., LIMITED.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Compile middleware project, this is the entrance script

# error out on errors
set -e
OUT_DIR="$1"
BOARD_NAME="$2"
KERNEL_TYPE="$3"
CC_PATH="$4"

function main(){
    CUR_DIR=$(cd $(dirname "$0");pwd)

	OS_TYPE=$KERNEL_TYPE
    if [ "$KERNEL_TYPE" = "linux" ];then
        OS_TYPE="linux_small"
    fi

	echo "BOARD_NAME=${BOARD_NAME} OS_TYPE=${OS_TYPE} CC_PATH=${CC_PATH}"
    #######################################
    # build ffmpeg library
    #######################################
    ./ffmpeg_adapt/build_ffmpeg.sh $OUT_DIR $BOARD_NAME $OS_TYPE $CC_PATH

    cp -rf $CUR_DIR/component/fileformat/mp4/lib/$OS_TYPE/libmp4.so $OUT_DIR/
    cp -rf $CUR_DIR/component/fileformat/ts/lib/$OS_TYPE/libts.so $OUT_DIR/
    cp -rf $CUR_DIR/component/fileformat/exif/lib/$OS_TYPE/libexif.so $OUT_DIR/
    cp -rf $CUR_DIR/component/fileformat/common/lib/$OS_TYPE/libfileformat.so $OUT_DIR/
    cp -rf $CUR_DIR/component/recorder_pro/lib/$OS_TYPE/librecorder_pro.so $OUT_DIR/
    cp -rf $CUR_DIR/component/dtcf/lib/$OS_TYPE/libdtcf.so $OUT_DIR/
    cp -rf $CUR_DIR/component/fstool/lib/$OS_TYPE/libfstool.so $OUT_DIR/
    cp -rf $CUR_DIR/common/hitimer/lib/$OS_TYPE/libhitimer.so $OUT_DIR/
    cp -rf $CUR_DIR/common/mbuffer/lib/$OS_TYPE/libmbuf.so $OUT_DIR/
    cp -rf $CUR_DIR/common/log/lib/$OS_TYPE/libmwlog.so $OUT_DIR/
}

if [ "x" != "x$5" ]; then
export SYSROOT_PATH=$5
fi
if [ "x" != "x$6" ]; then
export ARCH_CFLAGS="$6"
fi
main "$@"
