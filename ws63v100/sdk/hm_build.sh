#!/bin/bash
set -e
product_out_dir="$1"
build_ws63_sdk_open="$2"
# prebuild #
CROOT=$(pwd)

OUTPUT_SRC_DIR=$CROOT/output/ws63/fwpkg
OUTPUT_DST_DIR=$1

OHOS_LIBS_DIR=$CROOT/interim_binary/ws63/libs/ohos/ws63-liteos-app
OHOS_LIBS_XTS_DIR=$CROOT/interim_binary/ws63/libs/ohos/ws63-liteos-xts

rm $OHOS_LIBS_DIR -rf
rm $OHOS_LIBS_XTS_DIR -rf

mkdir -p $OHOS_LIBS_DIR
mkdir -p $OHOS_LIBS_XTS_DIR

find $OUTPUT_DST_DIR/libs/ -name '*.a' -exec cp "{}" $OHOS_LIBS_XTS_DIR  \;
find $OUTPUT_DST_DIR/libs/ -name '*.a' -exec cp "{}" $OHOS_LIBS_DIR  \;


# build #
echo "build ws63 sdk = $build_ws63_sdk_open"
if [ "$build_ws63_sdk_open" = "true" ]; then
    export build_ws63_sdk_open=true
    echo "export build_ws63_sdk_open=true "
fi

if [[ "$product_out_dir" == *xts* ]]; then
    python3 build.py -c ws63-liteos-xts
else
    python3 build.py -c ws63-liteos-app
fi

if [ "$build_ws63_sdk_open" = "true" ]; then
echo "build ws63 sdk"
target_file="sdk.tar.gz"
cd ../../../../../
rm -rf sdk_temp
mkdir -p sdk_temp

echo "copy drivers to $sdk_temp"
cp -rf drivers applications  base  vendor .gn build.py  commonlibrary  device  ide  kernel   test   third_party arkcompiler  build  build.sh  developtools   domains  foundation  interface  napi_generator  productdefine  sdk_temp

find sdk_temp -name ".git" -type d -exec rm -rf {} \; || true
find sdk_temp/device/soc/hisilicon/ws63v100/src/open_source/mbedtls -type f -name "*.c" -delete
find sdk_temp/device/soc/hisilicon/ws63v100/src/open_source/lwip -type f -name "*.c" -delete
find sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS -type f -name "*.c" -delete
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/arch/riscv/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/compat/cmsis/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/compat/linux/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/drivers/base/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/drivers/interrupt/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/fs/compat/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/fs/vfs/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/kernel/base/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/kernel/extended/console/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/kernel/extended/cpup/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/kernel/init/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/lib/libc/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/lib/liteos_libc/arch/riscv32/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/lib/liteos_libc/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/targets/CMakeLists.txt
echo "" > sdk_temp/device/soc/hisilicon/ws63v100/src/kernel/liteos/liteos_v208.5.0/Huawei_LiteOS/lib/libm/CMakeLists.txt

tar --warning=no-file-changed -zcvf $target_file sdk_temp
fi

#
# after build #
if [ ! -d $OUTPUT_DST_DIR ]; then
   mkdir $OUTPUT_DST_DIR
fi
cp $OUTPUT_SRC_DIR/* $OUTPUT_DST_DIR/ -rf

