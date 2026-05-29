ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../../../../Makefile.param
        include $(PARAM_FILE)
endif

ifeq ($(KERNELRELEASE),)
export CIPHER_BASE_DIR=$(PWD)
endif

KBUILD_EXTRA_SYMBOLS += $(OT_KBUILD_EXTRA_SYMBOLS)

ifeq ($(filter ss626v100, $(OT_ARCH)),)
out_dir=smp
else
out_dir=amp
endif

# mbedtls config
MBEDTLS_DIR := $(OPEN_SOURCE_PATH)/mbedtls
include $(MBEDTLS_DIR)/build.mak
ifeq ($(MPP_BUILD), n)
MBEDTLS_REL := ../../../../../../../../open_source/mbedtls/
BUILD_DIR = $(TEMP_PATH)/$(out_dir)/a55_linux/$(subst $(SDK_PATH)/,,$(CURDIR))
else
MBEDTLS_REL := ../../../../../../open_source/mbedtls/
BUILD_DIR = $(TEMP_PATH)/$(subst $(SDK_PATH)/,,$(CURDIR))
endif

include $(CIPHER_BASE_DIR)/api/build.mak
include $(CIPHER_BASE_DIR)/drv/build.mak
include $(MBEDTLS_DIR)/Makefile.param
MBEDTLS_OBJS := $(OBJS_CRYPTO:%.o=$(MBEDTLS_REL)/$(MBEDTLS_VER)/library/%.o)

MPP_CFLAGS += $(CIPHER_API_CFLAGS)
MPP_CFLAGS += $(CIPHER_DRV_CFLAGS)
MPP_CFLAGS += -I$(CIPHER_BASE_DIR)/../include
MPP_CFLAGS += -I$(CIPHER_BASE_DIR)/../../ext_inc
MPP_CFLAGS += -I$(CIPHER_BASE_DIR)/../../../copy_inc
ifeq ($(MPP_BUILD), n)
MPP_CFLAGS += -I$(REL_INC)
endif

MPP_CFLAGS :=$(subst /base/,/based/,$(MPP_CFLAGS))
MPP_CFLAGS :=$(subst /sys/,/sysd/,$(MPP_CFLAGS))

EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += $(DRV_CFLAGS)
EXTRA_CFLAGS += -I$(MBEDTLS_DIR)/$(MBEDTLS_VER)/include/
EXTRA_CFLAGS += -Wno-unused-but-set-variable

BUILD_DIR_MAKEFILE := $(BUILD_DIR)/Makefile

CIPHER_INC     := ot_common_cipher.h ot_mpi_cipher.h
CIPHER_DIR_INC := $(patsubst %.h,$(CIPHER_BASE_DIR)/../include/%.h,$(CIPHER_INC))
CIPHER_REL_INC := $(patsubst %.h,$(REL_INC)/%.h,$(CIPHER_INC))

TARGET := ot_cipher
include $(CIPHER_BASE_DIR)/$(OSTYPE).mak
include $(CIPHER_BASE_DIR)/mbedtls.mak