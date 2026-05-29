#!/bin/bash
# Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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
# Compile mpp/sample project, this is the entrance script

# error out on errors
set -e
OHOS_OUT_DIR="$1"
export OHOS_OUT_DIR
CHIP="$2"
HOS_KERNEL_TYPE="$3"
HOS_BUILD_COMPILER="$4"
HOS_BUILD_COMPILER_DIR="$5"
GRAPHIC_UTILS_LITE_ENABLE_DRM_DISPLAY_HAL="$6"
function main(){
    echo "GN::hardware/media--------------------$OHOS_OUT_DIR:$CHIP:$HOS_KERNEL_TYPE:$HOS_BUILD_COMPILER"
    #衔接具体模块编译的Makefile
    if [[ "${CHIP}" =~ "_openharmony" ]]; then
        sed -i "/^CFG_CHIP_TYPE=/cCFG_CHIP_TYPE=${CHIP%%_*}" $(pwd)/build/cfg.mak
        sed -i "s/CFG_SUPPORT_OPENSOURCE=y/CFG_SUPPORT_OPENSOURCE=y/g" $(pwd)/build/cfg.mak
    else
        sed -i "/^CFG_CHIP_TYPE=/cCFG_CHIP_TYPE=$CHIP" $(pwd)/build/cfg.mak
        sed -i "s/CFG_SUPPORT_OPENSOURCE=y/CFG_SUPPORT_OPENSOURCE=y/g" $(pwd)/build/cfg.mak
    fi
    sed -i "/^CFG_COMPILE_DIR=/cCFG_COMPILE_DIR=$HOS_BUILD_COMPILER_DIR" $(pwd)/build/cfg.mak
    if [ "$HOS_KERNEL_TYPE" = "liteos_a" ];then
        OS_TYPE="OHOS"
        KERNEL_OS_TYPE=ohos
    elif [ "$HOS_KERNEL_TYPE" = "linux" ];then
        OS_TYPE="Linux"
        KERNEL_OS_TYPE=linux
    fi
    if [ "$HOS_BUILD_COMPILER" = "gcc" ];then
        TOOLCHAIN_BUILD_COMPILER=gcc
        COMPILE_TYPE=gcc_musl
        sed -i "/^CFG_COMPILE_TYPE=/cCFG_COMPILE_TYPE=$COMPILE_TYPE" $(pwd)/build/cfg.mak
    else
        TOOLCHAIN_BUILD_COMPILER=llvm
        COMPILE_TYPE=clang_musl
        sed -i "/^CFG_COMPILE_TYPE=/cCFG_COMPILE_TYPE=$COMPILE_TYPE" $(pwd)/build/cfg.mak
    fi
    if [ "$GRAPHIC_UTILS_LITE_ENABLE_DRM_DISPLAY_HAL" = "true" ];then
        sed -i "/^CFG_GRAPHIC_UTILS_LITE_ENABLE_DRM_DISPLAY_HAL=/cCFG_GRAPHIC_UTILS_LITE_ENABLE_DRM_DISPLAY_HAL=true" $(pwd)/build/cfg.mak
    else
        sed -i "/^CFG_GRAPHIC_UTILS_LITE_ENABLE_DRM_DISPLAY_HAL=/cCFG_GRAPHIC_UTILS_LITE_ENABLE_DRM_DISPLAY_HAL=false" $(pwd)/build/cfg.mak
    fi
    # Todo: middleware lib chip type
if [ "$CHIP" = "hi3403v100" ];then
    CHIP_TYPE=ss928v100
else
    CHIP_TYPE=hi3516dv500
fi
    sed -i "/^CFG_OS_TYPE=/cCFG_OS_TYPE=$OS_TYPE" $(pwd)/build/cfg.mak

    pushd .
    make clean
    make -j1 VERBOSE=1
    popd
    # Rootfs依赖本模块的库
    ROOT_DIR=$(cd $(dirname "$0")/../;pwd)
    if [ ! -d "$OHOS_OUT_DIR/libs" ]; then
        mkdir -p $OHOS_OUT_DIR/libs
    fi

    cp $ROOT_DIR/../middleware/source/component/fileformat/heif/lib/$CHIP_TYPE/$COMPILE_TYPE/libheif.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/fileformat/mp4/lib/$CHIP_TYPE/$COMPILE_TYPE/libmp4.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/fileformat/ts/lib/$CHIP_TYPE/$COMPILE_TYPE/libts.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/fileformat/common/lib/$CHIP_TYPE/$COMPILE_TYPE/libfileformat.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/fileformat/exif/lib/$CHIP_TYPE/$COMPILE_TYPE/libexif.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/recorder_pro/lib/$CHIP_TYPE/$COMPILE_TYPE/librecorder_pro.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/liteplayer/lib/$CHIP_TYPE/$COMPILE_TYPE/libplugin_demuxer_hmf.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/liteplayer/lib/$CHIP_TYPE/$COMPILE_TYPE/libmpf_demuxer.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/liteplayer/lib/$CHIP_TYPE/$COMPILE_TYPE/libplugin_vdec_35xx.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/liteplayer/lib/$CHIP_TYPE/$COMPILE_TYPE/libplugin_adec_35xx.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/liteplayer/lib/$CHIP_TYPE/$COMPILE_TYPE/libavplay_common.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/otconfaccess/lib/$CHIP_TYPE/$COMPILE_TYPE/libotconfaccess.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/component/dtcf/lib/$CHIP_TYPE/$COMPILE_TYPE/libdtcf.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/common/ottimer/lib/$CHIP_TYPE/$COMPILE_TYPE/libottimer.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/common/uproc/lib/$CHIP_TYPE/$COMPILE_TYPE/libuproc.so $OHOS_OUT_DIR/
    cp $ROOT_DIR/../middleware/source/common/log/lib/$CHIP_TYPE/$COMPILE_TYPE/libmwlog.so $OHOS_OUT_DIR/
    # cp $ROOT_DIR/../middleware/source/common/securec/lib/$CHIP_TYPE/$COMPILE_TYPE/libsecurec.so $OHOS_OUT_DIR/
    
    cp $ROOT_DIR/audio/$CHIP/libs/libaudio_hw.so $OHOS_OUT_DIR/
    if [ -f "$ROOT_DIR/audio/$CHIP/libs/libaudio_primary_port.so" ]; then
        cp $ROOT_DIR/audio/$CHIP/libs/libaudio_primary_port.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/audio/$CHIP/libs/libaudio_usb_port.so" ]; then
        cp $ROOT_DIR/audio/$CHIP/libs/libaudio_usb_port.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/audio/source/plugins/hi35xx/audio_effect_manager/audio_vqe.ini" ]; then
        cp $ROOT_DIR/audio/source/plugins/hi35xx/audio_effect_manager/audio_vqe.ini $OHOS_OUT_DIR/data/
    fi
    cp $ROOT_DIR/codec/$CHIP/libs/libcodec.so $OHOS_OUT_DIR/
    if [ -f "$ROOT_DIR/codec/$CHIP/libs/libplugin_vdec_35xx.so" ]; then
        cp $ROOT_DIR/codec/$CHIP/libs/libplugin_vdec_35xx.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/codec/$CHIP/libs/libplugin_venc_35xx.so" ]; then
        cp $ROOT_DIR/codec/$CHIP/libs/libplugin_venc_35xx.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/codec/$CHIP/libs/libplugin_adec_35xx.so" ]; then
        cp $ROOT_DIR/codec/$CHIP/libs/libplugin_adec_35xx.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/codec/$CHIP/libs/libplugin_aenc_35xx.so" ]; then
        cp $ROOT_DIR/codec/$CHIP/libs/libplugin_aenc_35xx.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/codec/$CHIP/libs/libotaacdec.so" ]; then
        cp $ROOT_DIR/codec/$CHIP/libs/libotaacdec.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/format/$CHIP/libs/libformat_hw.so" ]; then
        cp $ROOT_DIR/format/$CHIP/libs/libformat_hw.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/format/$CHIP/libs/libplugin_demuxer_ffmpeg.so" ]; then
        cp $ROOT_DIR/format/$CHIP/libs/libplugin_demuxer_ffmpeg.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/format/$CHIP/libs/libplugin_demuxer_hmf.so" ]; then
        cp $ROOT_DIR/format/$CHIP/libs/libplugin_demuxer_hmf.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/format/$CHIP/libs/libplugin_demuxer_raw.so" ]; then
        cp $ROOT_DIR/format/$CHIP/libs/libplugin_demuxer_raw.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/format/$CHIP/libs/libplugin_demuxer_mpf.so" ]; then
        cp $ROOT_DIR/format/$CHIP/libs/libplugin_demuxer_mpf.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/format/$CHIP/libs/libplugin_muxer_recorder.so" ]; then
        cp $ROOT_DIR/format/$CHIP/libs/libplugin_muxer_recorder.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/common/$CHIP/libs/libmedia_hal_common.so" ]; then
        cp $ROOT_DIR/common/$CHIP/libs/libmedia_hal_common.so $OHOS_OUT_DIR/
    fi
    if [ -f "$ROOT_DIR/format/$CHIP/libs/libplugin_muxer_m4a.so" ]; then
        cp $ROOT_DIR/format/$CHIP/libs/libplugin_muxer_m4a.so $OHOS_OUT_DIR/
    fi
}

main "$@"
