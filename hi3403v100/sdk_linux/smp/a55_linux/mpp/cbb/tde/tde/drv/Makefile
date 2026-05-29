ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../Makefile.param
    include $(PARAM_FILE)
endif
ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../../../Makefile.param
    include $(CBB_PARAM_FILE)
endif

DRIVER_DIR := $(PWD)

ifneq ($(OT_MAKE_DEBUG),)
EXTRA_CFLAGS +=-D$(OT_MAKE_DEBUG)
endif

ifeq ($(SIXTYFOUR_SYS_STATUS), y)
EXTRA_CFLAGS += -DSIXFOUR_SYS_STATUS=1
else
EXTRA_CFLAGS += -DSIXFOUR_SYS_STATUS=0
endif

EXTRA_CFLAGS += $(MKP_CFLAGS) $(DRV_CFLAGS)
EXTRA_CFLAGS += -D$(OT_ARCH)  -D$(OT_FPGA)
ifeq ($(CONFIG_OT_PROC_SHOW_SUPPORT),y)
EXTRA_CFLAGS += -DCONFIG_OT_PROC_SHOW_SUPPORT
endif
CHIP_TYPE=CHIP_TYPE_$(OT_ARCH)
KBUILD_EXTRA_SYMBOLS += $(OT_KBUILD_EXTRA_SYMBOLS)

EXTRA_CFLAGS += -D$(CHIP_TYPE)

EXTRA_CFLAGS += -DOT_TDE_MEMCOUNT_SUPPORT=0
EXTRA_CFLAGS += -DOT_TDE_BUFFER=0x20000
EXTRA_CFLAGS += -DTDE_LOWPOWER
ifeq ($(OT_ARCH), $(filter $(OT_ARCH), ss528v100))
EXTRA_CFLAGS += -I$(SDK_PATH)/mpp/cbb/base/arch/$(OT_ARCH)/include/$(OT_SUBARCH)
else
ifeq ($(CONFIG_DRIVER_SPACE_TYPE),user_space)
EXTRA_CFLAGS += -I$(SDK_PATH)/mpp/cbb/based/arch/$(OT_ARCH)/include
else
EXTRA_CFLAGS += -I$(SDK_PATH)/mpp/cbb/base/arch/$(OT_ARCH)/include
endif
endif

ifneq ($(findstring $(OT_ARCH), ss524v100 ss626v100),)
EXTRA_CFLAGS += -DCONFIG_GFBG_LOW_DELAY_SUPPORT
endif

ifneq ($(findstring $(OT_ARCH), ss524v100 ss928v100 ss626v100),)
EXTRA_CFLAGS += -DCONFIG_TDE_GFBG_COMPRESS_V2
endif

ifeq ($(OT_ARCH), $(filter $(OT_ARCH), ss528v100))
EXTRA_CFLAGS += -DCONFIG_TDE_GFBG_COMPRESS_V1
endif

ifneq ($(findstring $(OT_ARCH), ss928v100 ss626v100),)
EXTRA_CFLAGS += -DCONFIG_TDE_ROTATE_SUPPORT
endif

ifeq ($(OT_ARCH), $(filter $(OT_ARCH), ss626v100))
EXTRA_CFLAGS += -DCONFIG_TDE_DMA_CORNER_V2
EXTRA_CFLAGS += -DCONFIG_TDE_DRD_LINE_SUPPORT
EXTRA_CFLAGS += -DCONFIG_TDE_MISCELLANEOUS_OUTSTANDING
else
EXTRA_CFLAGS += -DCONFIG_TDE_DMA_CORNER_V1
endif

ifeq ($(OT_ARCH), $(filter $(OT_ARCH), ss928v100))
EXTRA_CFLAGS += -DCONFIG_TDE_ZME_LINE_BUFFER2048
endif

EXTRA_CFLAGS += -DCONFIG_COMPRESS_ECONOMIZE_MEMERY
EXTRA_CFLAGS += -DCONFIG_USE_SYS_CONFIG
#EXTRA_CFLAGS += -DTDE_TIME_COUNT

ifeq ($(FPGA),y)
EXTRA_CFLAGS += -I$(SDK_PATH)/osdrv/$(ARCH_DIR)_fpga/linux/tools/board/reg-tools-1.0.0/include/common
endif

EXTRA_CFLAGS += -I$(DRIVER_DIR)/include
EXTRA_CFLAGS += -I$(DRIVER_DIR)/../api/tde/include
EXTRA_CFLAGS += -I$(DRIVER_DIR)/tde/include
EXTRA_CFLAGS += -I$(DRIVER_DIR)/tde/adp/tde_v2_0
EXTRA_CFLAGS += -I$(DRIVER_DIR)/tde/src

SRCS += tde/src/tde_osr.c\
       tde/adp/tde_v2_0/tde_hal_k.c \
       tde/adp/tde_v2_0/tde_osictl_k.c \
       tde/src/tde_osilist.c \
       tde/src/wmalloc.c \
       tde/src/tde_ioctl.c \
       tde/src/tde_handle.c \
       tde/adp/tde_v2_0/tde_adp.c

ifdef CONFIG_OT_PROC_SHOW_SUPPORT
SRCS += tde/src/tde_proc.c
endif

ifeq ($(CONFIG_DRIVER_SPACE_TYPE),user_space)
MKP_CFLAGS :=$(subst base/,based/,$(MKP_CFLAGS))
MKP_CFLAGS :=$(subst sys/,sysd/,$(MKP_CFLAGS))
endif

INIT_FILE := ./init//$(OSTYPE)/src/tde_init.c
INIT_INC := ./init//$(OSTYPE)/include/ot_tde_mod_init.h
MKP_CFLAGS += -I$(PWD)/./init//$(OSTYPE)/include
MOD_NAME := mod_tde
SRCS_O := $(SRCS)
SRCS += $(INIT_FILE)
TARGET := $(KO_PREFIX)_tde

# compile linux or liteos
include $(PWD)/../../../Makefile.$(OSTYPE)
