#!/bin/bash
set -e
product_out_dir="$1"
win_compile="$2"
# 3rd arg: "xts_overlay" to enable the XTS bss overlay, anything else disables
# it. Driven by the xts_overlay GN arg in sdk_liteos/BUILD.gn.
xts_overlay_arg="$3"
# 4th arg: "hctest_rodata_opt" to move suite metadata to const .rodata.
# Driven by the hctest_rodata_opt GN arg. Together with xts_overlay_arg it
# drives CONFIG_HCTEST_NEW_RUNNER (the .xts_init KEEP block gate in link.ld.S).
hctest_rodata_arg="$4"
# prebuild #
CROOT=$(pwd)

OUTPUT_SRC_DIR=$CROOT/output/bin
OUTPUT_DST_DIR=$1

OHOS_LIBS_DIR=$CROOT/ohos/libs

rm $OHOS_LIBS_DIR -rf

mkdir -p $OHOS_LIBS_DIR
find $OUTPUT_DST_DIR/libs/ -name '*.a' -exec cp "{}" $OHOS_LIBS_DIR  \;
find $CROOT/3rd_sdk/ -name '*.a' -exec cp "{}" $OHOS_LIBS_DIR  \;

# patch win compile#
if [ "$win_compile" = "win" ]; then
    cp -rvf $CROOT/build/win_scripts/* $CROOT/
fi

# XTS overlay isolation: normalize CONFIG_XTS_OVERLAY in usr_config.mk so the
# scons link-script preprocessing (link.ld.S) receives -DXTS_OVERLAY_ENABLE iff
# the GN arg xts_overlay is true. Runs every build to clear any stale value
# left by a previous overlay-enabled build (keeps GN side and scons side in sync).
usr_cfg="$CROOT/build/config/usr_config.mk"
if [ -f "$usr_cfg" ]; then
    sed -i '/^CONFIG_XTS_OVERLAY=/d; /^# CONFIG_XTS_OVERLAY is not set$/d' "$usr_cfg"
    if [ "$xts_overlay_arg" = "xts_overlay" ]; then
        echo "CONFIG_XTS_OVERLAY=y" >> "$usr_cfg"
    else
        echo "# CONFIG_XTS_OVERLAY is not set" >> "$usr_cfg"
    fi

    # HCTEST_NEW_RUNNER gates the .xts_init KEEP blocks in link.ld.S. It is on
    # when either feature that needs the new runner is on (xts_overlay OR
    # hctest_rodata_opt); OFF only when both are off (then the .xts_init
    # sections are empty anyway, so the KEEP blocks are omitted to keep
    # link.ld.S preprocessing identical to pristine).
    sed -i '/^CONFIG_HCTEST_NEW_RUNNER=/d; /^# CONFIG_HCTEST_NEW_RUNNER is not set$/d' "$usr_cfg"
    if [ "$xts_overlay_arg" = "xts_overlay" ] || [ "$hctest_rodata_arg" = "hctest_rodata_opt" ]; then
        echo "CONFIG_HCTEST_NEW_RUNNER=y" >> "$usr_cfg"
    else
        echo "# CONFIG_HCTEST_NEW_RUNNER is not set" >> "$usr_cfg"
    fi
fi

# build #
./build.sh wifiiot_app

# after build #
if [ ! -d $OUTPUT_DST_DIR ]; then
   mkdir $OUTPUT_DST_DIR
fi
cp $OUTPUT_SRC_DIR/* $OUTPUT_DST_DIR/ -rf

