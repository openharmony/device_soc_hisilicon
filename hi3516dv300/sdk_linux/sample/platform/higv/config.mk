#/*
# * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *     http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# */
LOCAL_DIR := $(shell pwd)

include ./cfg.mak

COMPILER_PATH := $(CFG_COMPILE_DIR)
CFG_SDK_TOOLCHAIN=$(COMPILER_PATH)/arm-linux-ohoseabi-

CC := $(CFG_SDK_TOOLCHAIN)gcc
CXX := $(CFG_SDK_TOOLCHAIN)g++
AR := $(CFG_SDK_TOOLCHAIN)ar
LD := $(CFG_SDK_TOOLCHAIN)ld
STRIP := $(CFG_SDK_TOOLCHAIN)strip
OBJDUMP := $(CFG_SDK_TOOLCHAIN)objdump

SEC_CFLAGS := -O2 -fPIC
SEC_CFLAGS += -D_FORTIFY_SOURCE=2
SEC_LFLAGS += -Wl,-z,relro,-z,now,-z,noexecstack
SEC_CFLAGS += -s
CFLAGS += -Wall -Werror
CFLAGS += -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE -D_LARGEFILE64_SOURCE

ifeq (hi3518ev300, $(CFG_CHIP_TYPE))
CFLAGS += -D__HI3518EV300__
endif

ifeq (hi3516dv300, $(CFG_CHIP_TYPE))
CFLAGS += -D__SUPPORT_STERO__ -D__HI3516DV300__
endif

ifeq ($(CFG_OS_TYPE),OHOS)
CFLAGS += -D__OHOS__
endif

CFLAGS += $(SEC_CFLAGS)
CFLAGS += -ffunction-sections

ifeq ($(CFG_TURNON_PROC),y)
CFLAGS += -DENABLE_PROC
endif

ifeq ($(CFG_TURNON_LOG),y)
CFLAGS += -DENABLE_LOG
endif

ifeq ($(CFG_CFG_64BIT),y)
CFLAGS +=-Wextra -Wno-missing-field-initializers -Wformat=2 -Wpointer-arith
endif

ifeq ($(CFG_ENABLE_AUDIO),y)
CFLAGS += -DENABLE_AUDIO
endif

ifeq ($(CFG_ENABLE_MP3),y)
CFLAGS += -DENABLE_MP3
endif

ifeq ($(CFG_ENABLE_FALLOCATE),y)
CFLAGS += -DFILE_FALLOCATE_ENABLE
endif

ifeq ($(CFG_ENABLE_DEBUG),y)
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -ftrapv
endif

ifeq ($(CFG_ENABLE_SANITIZERS),y)
CFLAGS += -fsanitize=address -fsanitize=leak
endif

MW_CFLAGS  := -fno-common
MW_CFLAGS  += -fno-strict-aliasing
MW_CFLAGS  += -Wextra
MW_CFLAGS  += -pipe
CFLAGS     += $(MW_CFLAGS)
CXXFLAGS   := $(CFLAGS)
$(CFLAGS)   += -fsigned-char
$(CXXFLAGS) += -Weffc++
LFLAGS += $(SEC_LFLAGS)

SDK_ROOT := ../../../../../../../device/hisilicon/$(CFG_CHIP_TYPE)/sdk_liteos
MPP_ROOT := $(SDK_ROOT)/mpp
MPP_INC_PATH   := $(MPP_ROOT)/include
MPP_LIB_PATH   := $(MPP_ROOT)/lib

HIGV_SAMPLE_DEP_INC_PATH += -I$(LOCAL_DIR)/include
HIGV_SAMPLE_DEP_INC_PATH += -I$(MPP_INC_PATH)
HIGV_SAMPLE_DEP_INC_PATH += $(CFG_INC)

HIGV_SAMPLE_DEP_LIB_PATH += -L$(LOCAL_DIR)/lib
HIGV_SAMPLE_DEP_LIB_PATH += -L$(MPP_LIB_PATH)

#specify your custmized compile flags here
CXXFLAGS += $(HIGV_SAMPLE_DEP_INC_PATH)
CFLAGS   += $(HIGV_SAMPLE_DEP_INC_PATH)


LIBS += -Wl,-Bstatic -Wl,-Bdynamic
LIBS += -Wl,-Bstatic -lhigvsample -ltde -lsecurec -lstdc++ -lhdmi -lisp -l_hicalcflicker -lmpi -l_hiae -l_hiawb -l_hiawb_natura -lhi_cipher -l_hidehaze -lhifisheyecalibrate -l_hiir_auto -l_hiacs -l_hidrc -l_hildci -lmd  -lupvqe -ldnvqe -lVoiceEngine -Wl,-Bdynamic -ldl -lpthread -lm
LDFLAGS += $(HIGV_SAMPLE_DEP_LIB_PATH) -Wl,--start-group $(LIBS) -Wl,--end-group
LDXXFLAGS += $(HIGV_SAMPLE_DEP_LIB_PATH) -Wl,--start-group $(LIBS) -Wl,--end-group