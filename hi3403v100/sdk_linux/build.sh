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
 
set -e
OHOS_ROOT_PATH=$1
OHOS_OUTDIR=$2
if [ -n "${OHOS_OUTDIR}" ] && [[ "${OHOS_OUTDIR}" != /* ]]; then
    OHOS_OUTDIR="$(pwd)/${OHOS_OUTDIR}"
fi
OHOS_LITE=$3
COMPILER_DIR=$4
CHIP=$6
 
export KERNEL_VERSION="$5"
 
if [ -z "${OHOS_ROOT_PATH}" ];then
    OHOS_ROOT_PATH=$(pwd)/../../../..
else
    echo "OHOS_ROOT_PATH=${OHOS_ROOT_PATH}"
    if [[ "${OHOS_ROOT_PATH}" != /* ]]; then
        OHOS_ROOT_PATH="$(pwd)/${OHOS_ROOT_PATH}"
    fi
fi
 
export OHOS_ROOT_PATH
export OHOS_OUTDIR
if [ ${COMPILER_DIR} != "" ];then
    export COMPILER_PATH=${COMPILER_DIR}/bin
    export PATH=${COMPILER_PATH}:${PATH}
fi
 
SDK_LINUX_SRC_PATH=${OHOS_ROOT_PATH}/device/soc/hisilicon/hi3403v100/sdk_linux
BATCH_SIGN_KO_SCRIPT=${OHOS_ROOT_PATH}/device/board/hisilicon/hispark_aifly/kernel/batch_sign_ko.sh
SDK_LINUX_TMP_PATH=${OHOS_OUTDIR}/sdk_linux/src_tmp
SDK_LINUX_SMP_PATH=${SDK_LINUX_TMP_PATH}/smp
SDK_LINUX_OPEN_PATH=${SDK_LINUX_TMP_PATH}/open_source
SDK_LINUX_ATF_PATH=${SDK_LINUX_TMP_PATH}/open_source/trusted-firmware-a
SYSROOT_PATH=${OHOS_OUTDIR}/sysroot
export SYSROOT_PATH
OSDRV_CROSS_PATH=${OHOS_ROOT_PATH}/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu
 
rm -rdf ${SDK_LINUX_TMP_PATH}; mkdir -p ${SDK_LINUX_TMP_PATH}
mkdir -p ${SDK_LINUX_SMP_PATH}
cp -rf ${SDK_LINUX_SRC_PATH}/smp/* ${SDK_LINUX_SMP_PATH}
cp -rf ${SDK_LINUX_SRC_PATH}/*.patch ${SDK_LINUX_SMP_PATH}
 
mkdir -p ${SDK_LINUX_OPEN_PATH}
mkdir -p ${SDK_LINUX_ATF_PATH}
cp -rf ${SDK_LINUX_SRC_PATH}/open_source/trusted-firmware-a/* ${SDK_LINUX_ATF_PATH}
cp -rf ${SDK_LINUX_SRC_PATH}/002_trusted_firmware.patch ${SDK_LINUX_ATF_PATH}
cp -rf ${SDK_LINUX_SRC_PATH}/open_source/mbedtls ${SDK_LINUX_OPEN_PATH}/
 
echo "Add patchs to sdk..."
pushd ${SDK_LINUX_SMP_PATH}
patch -p1 < ./001_mpp.patch
popd
 

echo "compile ko..."
pushd "${SDK_LINUX_SMP_PATH}/a55_linux/mpp/out/obj" && \
    make clean OHOS_LITE=y CHIP="${CHIP}" SYSROOT_PATH="${SYSROOT_PATH}" && \
    make -j OHOS_LITE=y CHIP="${CHIP}" SYSROOT_PATH="${SYSROOT_PATH}" && popd

mkdir -p ${SDK_LINUX_TMP_PATH}/out
cp -rf ${SDK_LINUX_SMP_PATH}/a55_linux/mpp/out/ko ${SDK_LINUX_TMP_PATH}/out

# batch sign ko file: sign in-place on copied output
chmod +x ${BATCH_SIGN_KO_SCRIPT}
${BATCH_SIGN_KO_SCRIPT} -o ${SDK_LINUX_TMP_PATH}/out ${SDK_LINUX_TMP_PATH}/out

# verify signed ko files
KO_COUNT=$(find ${SDK_LINUX_TMP_PATH}/out -name "*.ko" | wc -l)
if [ ${KO_COUNT} -gt 0 ]; then
    echo "SDK KO: ${SDK_LINUX_TMP_PATH}/out verification success, ${KO_COUNT} ko file(s) found"
else
    echo "SDK KO: ${SDK_LINUX_TMP_PATH}/out verification failed!!!"
    exit 1
fi

# copy lib to out/lib for fs.yml packaging
SDK_LIB_PATH=${SDK_LINUX_SMP_PATH}/a55_linux/mpp/out/lib
OUT_LIB_PATH=${SDK_LINUX_TMP_PATH}/out/lib
mkdir -p ${OUT_LIB_PATH}
find ${SDK_LIB_PATH} -name "*.so" -exec cp -f {} ${OUT_LIB_PATH}/ \;

# copy uboot file
cp -rf ${SDK_LINUX_SRC_PATH}/../uboot/* ${OHOS_OUTDIR}
