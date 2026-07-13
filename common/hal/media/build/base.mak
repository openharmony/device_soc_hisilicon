# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
include $(MEDIA_BASE_DIR)/build/cfg.mak

#====================================================================================
#                          export variable
#====================================================================================
PROJECT_ROOT := $(MEDIA_BASE_DIR)/../../../../../..

OUT_DIR := $(PROJECT_ROOT)/out
MEDIA_HAL_COMMN_INC_PATH := $(MEDIA_BASE_DIR)/common/source/include

ifeq ($(CFG_OS_TYPE),Linux)
	KERNAL_TYPE=linux
else
	KERNAL_TYPE=liteos_a
endif

MIDDLEWARE_DIR := $(MEDIA_BASE_DIR)/../middleware/source
MIDDLEWARE_COMPONENT := $(MIDDLEWARE_DIR)/component
MIDDLEWARE_COMMON_DIR := $(MIDDLEWARE_DIR)/common
MIDDLEWARE_COMMON_INC_DIR := $(MIDDLEWARE_COMMON_DIR)/include
MIDDLEWARE_COMPONENT_DIR := $(MIDDLEWARE_DIR)/component
THIRDPARTY_DIR := $(PROJECT_ROOT)/third_party
CODEC_INTERFACE_PATH := $(PROJECT_ROOT)/drivers/peripheral/codec/interfaces/include
DISPLAY_INTERFACE_PATH := $(PROJECT_ROOT)/drivers/peripheral/display/interfaces/include
BASE_INTERFACE_PATH := $(PROJECT_ROOT)/drivers/peripheral/base

AT :=
#====================================================================================
#                          compile toolchain
#====================================================================================
ifeq ($(CFG_OS_TYPE),OHOS)
CFG_OS_TYPE_DIR=ohos
COMPILER_BASE_PATH=$(PROJECT_ROOT)/prebuilts/gcc/linux-x86/arm/arm-linux-ohoseabi-gcc
COMPILER_PATH := $(COMPILER_BASE_PATH)/bin
CFG_SDK_TOOLCHAIN=$(COMPILER_PATH)/arm-linux-ohoseabi-
else
COMPILER_BASE_PATH := /opt/linux/x86-arm/aarch64-v01c01-linux-musl-gcc
COMPILER_PATH := $(COMPILER_BASE_PATH)/bin
CFG_SDK_TOOLCHAIN=$(COMPILER_PATH)/aarch64-v01c01-linux-musl-
CFG_OS_TYPE_DIR=linux
endif

CFG_TOOLCHAIN_TYPE_DIR=gcc
# ifeq ($(CFG_COMPILE_TYPE), clang)
ifeq ($(findstring clang,$(CFG_COMPILE_TYPE)),clang)
COMPILER_BASE_PATH=$(PROJECT_ROOT)/prebuilts/clang/ohos/linux-x86_64/llvm
COMPILER_PATH := $(COMPILER_BASE_PATH)/bin
CFG_SDK_TOOLCHAIN=$(COMPILER_PATH)/llvm-
CFG_TOOLCHAIN_TYPE_DIR=llvm
endif

TARGET_DIR=../$(CFG_CHIP_TYPE)/libs
MEDIA_HAL_COMMN_LIB_PATH := $(MEDIA_BASE_DIR)/common/$(CFG_CHIP_TYPE)/libs
CC := $(CFG_SDK_TOOLCHAIN)gcc
CXX := $(CFG_SDK_TOOLCHAIN)g++
AR := $(CFG_SDK_TOOLCHAIN)ar
LD := $(CFG_SDK_TOOLCHAIN)ld
STRIP := $(CFG_SDK_TOOLCHAIN)strip
OBJDUMP := $(CFG_SDK_TOOLCHAIN)objdump

# ifeq ($(CFG_COMPILE_TYPE), clang)
ifeq ($(findstring clang,$(CFG_COMPILE_TYPE)),clang)
CC := $(COMPILER_PATH)/clang
CXX := $(COMPILER_PATH)/clang++
LD := $(COMPILER_PATH)/clang
endif

SEC_CFLAGS := -O2 -fstack-protector-all -fPIC
SEC_CFLAGS += -ftrapv -D_FORTIFY_SOURCE=2

# ifneq ($(CFG_COMPILE_TYPE), clang)
ifneq ($(findstring clang,$(CFG_COMPILE_TYPE)),clang)
SEC_LFLAGS += -Wl,-z,relro,-z,now,-z,noexecstack
SEC_CFLAGS += -s
CFLAGS += -Wall -Werror
endif
# CFLAGS += -mcpu=cortex-a7 -mfloat-abi=softfp -mfpu=neon-vfpv4
CFLAGS += -mcpu=cortex-a55
CFLAGS += -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -D_LARGEFILE64_SOURCE

# ifeq ($(CFG_COMPILE_TYPE), clang)
ifeq ($(findstring clang,$(CFG_COMPILE_TYPE)),clang)
CFLAGS += -Wall -Wno-error
CFLAGS += -march=armv8-a
CFLAGS += --target=aarch64-linux-ohos
CFLAGS += --sysroot=$(OHOS_OUT_DIR)/sysroot

LFLAGS += -march=armv8-a
LFLAGS += --target=aarch64-linux-ohos
LFLAGS += --sysroot=$(OHOS_OUT_DIR)/sysroot
endif

ifeq (hi3518ev300, $(CFG_CHIP_TYPE))
CFLAGS += -D__HI3518EV300__
endif

ifeq (hi3516dv300, $(CFG_CHIP_TYPE))
CFLAGS += -D__SUPPORT_STERO__ -D__HI3516DV300__
endif

ifeq (hi3403v100, $(CFG_CHIP_TYPE))
CFLAGS += -DMEDIA_INTERFACE_V1_0
CFLAGS += -DENABLE_H8
endif

ifeq ($(CFG_OS_TYPE),OHOS)
CFLAGS += -D__OHOS__
endif

CFLAGS += -DMW_VERSION=\"$(CFG_MW_VERSION)\"
CFLAGS += $(SEC_CFLAGS)
CFLAGS += -ffunction-sections
ifeq ($(CFG_TURNON_LOG),y)
CFLAGS += -DENABLE_LOG
endif
CFLAGS += -DNEED_SYS_INIT

ifeq ($(CFG_ENABLE_FALLOCATE),y)
CFLAGS += -DFILE_FALLOCATE_ENABLE
endif

ifeq ($(CFG_CFG_64BIT),y)
CFLAGS +=-Wextra -Wno-missing-field-initializers -Wformat=2 -Wpointer-arith
endif

ifeq ($(CFG_SUPPORT_DYNAMIC_LOADING),y)
CFLAGS += -DSUPPORT_DL
endif

ifeq ($(CFG_ENABLE_SANITIZERS),y)
CFLAGS += -fsanitize=address -fsanitize=leak
endif

ifeq ($(CFG_GRAPHIC_UTILS_LITE_ENABLE_DRM_DISPLAY_HAL),true)
CFLAGS += -DDRM_OPTION_ENABLE
endif

MEDIA_CFLAGS  := -fno-common
MEDIA_CFLAGS  += -fno-strict-aliasing
MEDIA_CFLAGS  += -Wextra
MEDIA_CFLAGS  += -pipe

CFLAGS     += $(MEDIA_CFLAGS)
CXXFLAGS   := $(CFLAGS)

$(CFLAGS)   += -fsigned-char
$(CXXFLAGS) += -Weffc++

LFLAGS += $(SEC_LFLAGS)
#====================================================================================
#                          common funciton
#====================================================================================
DEP_DIR := sdk
ifeq ($(CFG_OS_TYPE),OHOS)

    ifeq ($(CFG_SUPPORT_OPENSOURCE),y)
        SDK_ROOT := $(PROJECT_ROOT)/device/hisilicon/$(CFG_CHIP_TYPE)/sdk_liteos
        MPP_ROOT := $(SDK_ROOT)/soc
    else
        SDK_ROOT := $(PROJECT_ROOT)/device/hisilicon/$(CFG_CHIP_TYPE)/sdk_liteos
        MPP_ROOT := $(SDK_ROOT)/mpp
    endif
else
    ifeq ($(filter $(CFG_CHIP_TYPE),hi3403v100),$(CFG_CHIP_TYPE))
        SDK_ROOT := $(OHOS_OUT_DIR)/sdk_linux/src_tmp/smp/a55_linux/mpp
        MPP_ROOT := $(SDK_ROOT)/out
        SOURCE_CHIP_TYPE := ss928v100
    else
        SDK_ROOT := $(PROJECT_ROOT)/device/soc/hisilicon/hi3516dv500/sdk_linux/smp/a55_linux/source
        SOURCE_CHIP_TYPE := hi3516dv500
    endif
    MPP_ROOT := $(SDK_ROOT)/out
endif

MPP_INC_PATH   := $(MPP_ROOT)/include
MPP_LIB_PATH   := $(MPP_ROOT)/lib

SECUREC_INC_PATH := $(MPP_INC_PATH)
SECUREC_LIB_PATH := $(MPP_LIB_PATH)

CreateDir = $(shell [ -d $1 ] || mkdir -p $1 || echo ":mkdir '$1' fail")
RemoveDir = $(shell [ -d $1 ] && rm -rf $1 && echo -e "rmdir '$1'\t [ OK ]" || echo ":rm dir '$1' fail")
