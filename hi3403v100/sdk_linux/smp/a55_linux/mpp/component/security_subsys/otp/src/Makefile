ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../../../Makefile.param
        include $(PARAM_FILE)
endif

KBUILD_EXTRA_SYMBOLS += $(OT_KBUILD_EXTRA_SYMBOLS)

OTP_BASE_DIR := $(PWD)

# Add objs
include $(OTP_BASE_DIR)/mpi/build.mak
include $(OTP_BASE_DIR)/mkp/build.mak
include $(OTP_BASE_DIR)/arch/build.mak
include $(OTP_BASE_DIR)/osal/build.mak

MPP_CFLAGS += -I$(OTP_BASE_DIR)/include
MPP_CFLAGS += -I$(OTP_BASE_DIR)/../include
MPP_CFLAGS += -I$(OTP_BASE_DIR)/../../copy_inc
MPP_CFLAGS += -DOT_OTP_DEBUG=0
ifeq ($(MPP_BUILD), n)
MPP_CFLAGS += -I$(REL_INC)
endif

MPP_CFLAGS :=$(subst /base/,/based/,$(MPP_CFLAGS))
MPP_CFLAGS :=$(subst /sys/,/sysd/,$(MPP_CFLAGS))

LIB_CFLAGS += $(OTP_MPI_CFLAGS)

EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += $(DRV_CFLAGS)
EXTRA_CFLAGS += $(OTP_DRV_CFLAGS)

OTP_INC     := ot_common_otp.h ot_mpi_otp.h
OTP_DIR_INC := $(patsubst %.h,$(OTP_BASE_DIR)/../include/%.h,$(OTP_INC))
OTP_REL_INC := $(patsubst %.h,$(REL_INC)/%.h,$(OTP_INC))

BUILD_DIR = $(TEMP_PATH)/$(subst $(SDK_PATH)/,,$(CURDIR))
BUILD_DIR_MAKEFILE := $(BUILD_DIR)/Makefile

TARGET := ot_otp

include $(OTP_BASE_DIR)/$(OSTYPE).mak
