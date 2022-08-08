# Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
#!/bin/sh

# error out on errors
set -e
OUT_DIR="$1"
BOARD_NAME="$2"
OS_TYPE="$3"
CC_PATH="$4"

function main(){
    CUR_DIR=$(cd $(dirname "$0");pwd)
    ROOT_DIR=$CUR_DIR/../../../../../../..
    COMPILER_TYPE="clang"
    export LLVM_COMPILER=y
    CONFIGURE_FILE=configure_llvm
    echo "###### $BOARD_NAME:$OS_TYPE ######"

    FF_CONFIG_SH=disable_decoder_config.sh
    FFMPEG_ORIGN_DIR=$ROOT_DIR/third_party/ffmpeg
    FFMPEG_DIR=$CUR_DIR/ffmpeg

    ############################################
    # copy ffmpeg file to current path
    ############################################
    cp -r $FFMPEG_ORIGN_DIR $CUR_DIR

    ############################################
    # copy sh to ffmpeg file patch
    ############################################
    cp $CUR_DIR/$CONFIGURE_FILE $FFMPEG_DIR/configure_temp
    cp $CUR_DIR/adapt_config.sh $FFMPEG_DIR/adapt_config.sh
    cp $CUR_DIR/disable_decoder_config.sh $FFMPEG_DIR/disable_decoder_config.sh
    cp $CUR_DIR/library.mak $FFMPEG_DIR/ffbuild/library.mak
    cp $CUR_DIR/Makefile_llvm $FFMPEG_DIR/Makefile
    chmod -R 777 $FFMPEG_DIR
    cd $FFMPEG_DIR

    ############################################
    # run ./disable_decoder_config.sh
    ############################################
    echo "######################"
    echo "###### run ./disable_decoder_config.sh ######"
    echo "###### BOARD_NAME=$BOARD_NAME OS_TYPE=$OS_TYPE COMPILER_TYPE=$COMPILER_TYPE CC_PATH=$CC_PATH  ######"
    echo "######################"
    ./disable_decoder_config.sh $CC_PATH;

    ############################################
    # run ./adapt_config.sh
    ############################################
    echo "######################"
    echo "###### run ./adapt_config.sh ######"
    echo "######################"
    ./adapt_config.sh

    ############################################
    # make and make install
    ############################################
    echo "######################"
    echo "###### run make -j16 ######"
    echo "######################"
    make -j16

    echo "######################"
    echo "###### run make install ######"
    echo "######################"
    make install

    #############################################
    # copy library
    #############################################
    cp -rf $FFMPEG_DIR/install/lib/libavcodec.so $OUT_DIR/
    cp -rf $FFMPEG_DIR/install/lib/libavformat.so $OUT_DIR/
    cp -rf $FFMPEG_DIR/install/lib/libavutil.so $OUT_DIR/
    cp -rf $FFMPEG_DIR/install/lib/libswresample.so $OUT_DIR/

    cd -;
    rm -rf $FFMPEG_DIR
}

main "$@"