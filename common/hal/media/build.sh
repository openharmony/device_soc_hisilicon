#!/bin/bash
# Copyright 2020-2020, Huawei Technologies Co. Ltd.
#
# ALL RIGHTS RESERVED
#
# Compile media/hal project, this is the entrance script

# error out on errors
set -e
OUT_DIR="$1"
BOARD_NAME="$2"
KERNEL_TYPE="$3"

function main(){
    CUR_DIR=$(cd $(dirname "$0");pwd)
	OS_TYPE=$KERNEL_TYPE
    if [ "$KERNEL_TYPE" = "linux" ];then
        OS_TYPE="linux_small"
    fi
    cp -rf $CUR_DIR/audio/$BOARD_NAME/$OS_TYPE/libs/libaudio_hw.so $OUT_DIR/
    #cp -rf $CUR_DIR/audio/$BOARD_NAME/$OS_TYPE/libs/libaudio_input_port.so $OUT_DIR/
    #cp -rf $CUR_DIR/audio/$BOARD_NAME/$OS_TYPE/libs/libaudio_output_port.so $OUT_DIR/
    cp -rf $CUR_DIR/audio/$BOARD_NAME/$OS_TYPE/libs/libaudio_primary_port.so $OUT_DIR/

    cp -rf $CUR_DIR/codec/$BOARD_NAME/$OS_TYPE/libs/libcodec.so $OUT_DIR/
    cp -rf $CUR_DIR/codec/$BOARD_NAME/$OS_TYPE/libs/libhiaacdec.so $OUT_DIR/
    #cp -rf $CUR_DIR/codec/$BOARD_NAME/$OS_TYPE/libs/libhimp3dec.so $OUT_DIR/
    cp -rf $CUR_DIR/codec/$BOARD_NAME/$OS_TYPE/libs/libplugin_adec_35xx.so $OUT_DIR/
    cp -rf $CUR_DIR/codec/$BOARD_NAME/$OS_TYPE/libs/libplugin_aenc_35xx.so $OUT_DIR/
    cp -rf $CUR_DIR/codec/$BOARD_NAME/$OS_TYPE/libs/libplugin_vdec_35xx.so $OUT_DIR/
    cp -rf $CUR_DIR/codec/$BOARD_NAME/$OS_TYPE/libs/libplugin_venc_35xx.so $OUT_DIR/

    cp -rf $CUR_DIR/format/$BOARD_NAME/$OS_TYPE/libs/libformat_hw.so $OUT_DIR/
    cp -rf $CUR_DIR/format/$BOARD_NAME/$OS_TYPE/libs/libplugin_demuxer_ffmpeg.so $OUT_DIR/
    cp -rf $CUR_DIR/format/$BOARD_NAME/$OS_TYPE/libs/libplugin_demuxer_mpf.so $OUT_DIR/
    cp -rf $CUR_DIR/format/$BOARD_NAME/$OS_TYPE/libs/libplugin_demuxer_raw.so $OUT_DIR/
    cp -rf $CUR_DIR/format/$BOARD_NAME/$OS_TYPE/libs/libplugin_muxer_recorder.so $OUT_DIR/

    cp -rf $CUR_DIR/common/$BOARD_NAME/$OS_TYPE/libs/libmedia_hal_common.so $OUT_DIR/

    cp -rf $CUR_DIR/camera/$BOARD_NAME/$OS_TYPE/libs/libhdi_camera.so $OUT_DIR/
    cp -rf $CUR_DIR/videodisplay/$BOARD_NAME/$OS_TYPE/libs/libhdi_videodisplayer.so $OUT_DIR/
}

main "$@"
