ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../../../../mpp/Makefile.param
        include $(PARAM_FILE)
endif

KBUILD_EXTRA_SYMBOLS += $(OT_KBUILD_EXTRA_SYMBOLS)

KLAD_BASE_DIR := $(PWD)

include $(KLAD_BASE_DIR)/mpi/build.mak
include $(KLAD_BASE_DIR)/mkp/build.mak
include $(KLAD_BASE_DIR)/osal/build.mak
include $(KLAD_BASE_DIR)/arch/build.mak

MPP_CFLAGS += -I$(KLAD_BASE_DIR)/include
MPP_CFLAGS += -I$(KLAD_BASE_DIR)/../include
MPP_CFLAGS += -I$(KLAD_BASE_DIR)/../../copy_inc
MPP_CFLAGS += -DOT_KLAD_DEBUG=0
ifneq ($(MPP_BUILD), y)
MPP_CFLAGS += -I$(REL_INC)
endif

MPP_CFLAGS :=$(subst /base/,/based/,$(MPP_CFLAGS))
MPP_CFLAGS :=$(subst /sys/,/sysd/,$(MPP_CFLAGS))

#LIB_CFLAGS += $(KLAD_MPI_CFLAGS)

EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += $(DRV_CFLAGS)
EXTRA_CFLAGS += $(KLAD_DRV_CFLAGS)

BUILD_DIR = $(TEMP_PATH)/$(subst $(SDK_PATH)/,,$(CURDIR))
BUILD_DIR_MAKEFILE := $(BUILD_DIR)/Makefile

KLAD_INC     := ot_common_klad.h ot_mpi_klad.h
KLAD_DIR_INC := $(patsubst %.h,$(KLAD_BASE_DIR)/../include/%.h,$(KLAD_INC))
KLAD_REL_INC := $(patsubst %.h,$(REL_INC)/%.h,$(KLAD_INC))

TARGET := ot_klad

include $(KLAD_BASE_DIR)/$(OSTYPE).mak
