# $(ROOT)/mpp/code/component/gfbg/Makefile

ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CONFIG_OT_GFBG_SUPPORT), y)
GFBG_HEADER     := gfbg.h
BASE_MKP_HEADER := mod_ext.h proc_ext.h vb_ext.h
SYS_EXT_HEADER  := sys_ext.h
TDE_EXT_HEADER  := ot_drv_tde.h
TDE_MKP_HEADER  := ot_common_tde.h
INIT_HEADER     := ot_module_param.h
OSAL_HEADER     := ot_osal.h osal_list.h osal_ioctl.h osal_mmz.h
VO_HEADER       := vou_ext.h
VGS_HEADER      := vgs_ext.h
VGS_COMM_HEADER := ot_common_vgs.h
COMMON_HEADER   := ot_type.h ot_common.h autoconf.h ot_debug.h ot_math.h ot_errno.h ot_common_video.h

INC_DIR          =$(MPP_PATH)/component/gfbg/include

#*******************************************************************************
#********************* Set the source code and include path ********************
MPP_ROOT        := $(SDK_PATH)/mpp
CBB_ROOT        := $(SDK_PATH)/mpp/cbb
COMPONENT_ROOT  := $(SDK_PATH)/mpp/component

# Head file path variable's
CFG_INC += -I$(PWD)/include
CFG_INC += -I$(PWD)/src
CFG_INC += -I$(PWD)/src/init
CFG_INC += -I$(PWD)/drv/adp
CFG_INC += -I$(PWD)/drv/include
ifeq ($(MPP_BUILD), n)
CFG_INC += -I$(REL_INC)
endif
ifeq ($(CONFIG_SYNC_FILE),y)
CFG_INC += -I$(KERNEL_ROOT)/drivers/ot_fence
EXTRA_CFLAGS += -DCONFIG_GFBG_FENCE_SUPPORT
endif

ifneq ($(findstring $(OT_ARCH), ss524v100 ss626v100),)
EXTRA_CFLAGS += -DCONFIG_GFBG_LOW_DELAY_SUPPORT
endif

ifneq ($(findstring $(OT_ARCH), ss524v100 ss928v100 ss626v100),)
EXTRA_CFLAGS += -DCONFIG_TDE_GFBG_COMPRESS_V2
endif
ifeq ($(OT_ARCH),ss528v100)
EXTRA_CFLAGS += -DCONFIG_TDE_GFBG_COMPRESS_V1
endif

ifeq ($(OT_ARCH),ss626v100)
EXTRA_CFLAGS += -DCONFIG_GFBG_SMART_RECT_V2
EXTRA_CFLAGS += -DCONFIG_GFBG_G1_SUPPORT_ZME
EXTRA_CFLAGS += -DCONFIG_GFBG_G0_SUPPORT_ZME_CHN0
EXTRA_CFLAGS += -DCONFIG_TDE_CLUT_RECT_V2
EXTRA_CFLAGS += -DCONFIG_GFBG_SET_CSC_CTRL
EXTRA_CFLAGS += -DCONFIG_GFBG_G1_4K_SUPPORT
EXTRA_CFLAGS += -DCONFIG_GFBG_G0_4096_SUPPORT
EXTRA_CFLAGS += -DCONFIG_GFBG_G3_4096_SUPPORT
endif

ifeq ($(OT_ARCH),ss928v100)
EXTRA_CFLAGS += -DCONFIG_GFBG_G0_SUPPORT_ZME_CHN0
EXTRA_CFLAGS += -DCONFIG_TDE_CLUT_RECT_V2
EXTRA_CFLAGS += -DCONFIG_GFBG_G3_960_SUPPORT
endif

ifneq ($(findstring $(OT_SUBARCH), ss528v100 ss626v100),)
EXTRA_CFLAGS += -DCONFIG_GFBG_G1_4K_SUPPORT
EXTRA_CFLAGS += -DCONFIG_GFBG_G0_4096_SUPPORT
EXTRA_CFLAGS += -DCONFIG_GFBG_G3_4096_SUPPORT
endif

ifeq ($(OT_ARCH), ss625v100)
EXTRA_CFLAGS += -DCONFIG_GFBG_G1_4M_SUPPORT
EXTRA_CFLAGS += -DCONFIG_GFBG_G0_4096_SUPPORT
endif

EXTRA_CFLAGS += -DCONFIG_COMPRESS_ECONOMIZE_MEMERY
#***************************************************************************#


#********************* List all the release file *******************************

ifeq ($(OT_GDB),y)
EXTRA_CFLAGS += -g
endif

EXTRA_CFLAGS += $(CFG_INC)
EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += $(DRV_CFLAGS)

GFBG_MOD_NAME:=gfbg
MOD_NAME := mod_fb

EXTRA_CFLAGS += -Wall -D$(OT_ARCH) -D$(OT_FPGA)
ifeq ($(CONFIG_DRIVER_SPACE_TYPE),user_space)
EXTRA_CFLAGS :=$(subst base/,based/,$(EXTRA_CFLAGS))
EXTRA_CFLAGS :=$(subst sys/,sysd/,$(EXTRA_CFLAGS))
endif
-include $(PWD)/src/Makefile
-include $(PWD)/drv/adp/Makefile

$(GFBG_MOD_NAME)-objs = $(GFBG_SRC:%.c=%.o)

ifneq ($(KERNELRELEASE),)
    obj-m += gfbg.o
endif

# compile linux or liteos
include $(PWD)/Make.$(OSTYPE)

#CONFIG_OT_GFBG_SUPPORT endif
else
all:
	@echo "gfbg module not compile"
clean:
	@echo "gfbg module don't need clean"
endif
