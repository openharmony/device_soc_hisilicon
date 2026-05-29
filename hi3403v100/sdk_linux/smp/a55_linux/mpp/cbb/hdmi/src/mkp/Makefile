# $(ROOT)/mpp/code/component/hdmi/mkp/Makefile
ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../../../Makefile.param
    include $(CBB_PARAM_FILE)
endif

ifeq ($(OT_ARCH), ss528v100)
ifeq ($(OT_SUBARCH), ss625v100)
    MKP_CFLAGS += -DHDMI_PRODUCT_SS625V100
    MKP_CFLAGS += -DCHIP_TYPE_ss625v100
    export SCDC_SUPPORT=n
else
    MKP_CFLAGS += -DHDMI_PRODUCT_SS528V100
    MKP_CFLAGS += -DCHIP_TYPE_ss528v100
    export SCDC_SUPPORT=y
endif
    HDMI_CHIP := ss528v100
    CTRL_DIR := hal/ctrl/v100
    PHY_DIR  := hal/phy/v100
    export CEC_SUPPORT=n
    MKP_CFLAGS += -DPHY_CRAFT_S28
else ifeq ($(OT_ARCH), ss524v100)
    HDMI_CHIP := ss524v100
    MKP_CFLAGS += -DHDMI_PRODUCT_SS524V100
    MKP_CFLAGS += -DCHIP_TYPE_ss524v100
    MKP_CFLAGS += -DPHY_CRAFT_S28
    CTRL_DIR := hal/ctrl/v100
    PHY_DIR  := hal/phy/v100
    export CEC_SUPPORT=n
    export SCDC_SUPPORT=n
else ifeq ($(OT_ARCH), ss928v100)
    HDMI_CHIP := ss928v100
    MKP_CFLAGS += -DHDMI_PRODUCT_SS928V100
    MKP_CFLAGS += -DCHIP_TYPE_ss928v100
    CTRL_DIR := hal/ctrl/v100
    PHY_DIR  := hal/phy/v200
    export CEC_SUPPORT=n
    export FRL_SUPPORT=n
    export SCDC_SUPPORT=y
else ifeq ($(OT_ARCH), ss626v100)
    HDMI_CHIP := ss626v100
    MKP_CFLAGS += -DHDMI_PRODUCT_SS626V100
    MKP_CFLAGS += -DCHIP_TYPE_ss626v100
    MKP_CFLAGS += -DHDMI_SUPPORT_DUAL_CHANNEL
    CTRL_DIR := hal/ctrl/v200
    PHY_DIR  := hal/phy/v200
    export CEC_SUPPORT=n
    export FRL_SUPPORT=n
    export SCDC_SUPPORT=y
endif
####################################################################
# path
####################################################################
HDMI_VERSION := hdmi

MPP_PATH := $(SDK_PATH)/mpp
CODE_ROOT := $(MPP_PATH)/cbb

INC_PATH := -I$(PWD)/../include

INC_PATH += -I$(MPP_PATH)/out/$(OT_ARCH)/$(OSTYPE)/$(CPU_TYPE)/include

ifeq ($(OT_ARCH), $(filter $(OT_ARCH), ss528v100))
INC_PATH += -I$(CODE_ROOT)/base/arch/$(OT_ARCH)/include/$(OT_SUBARCH)
else
INC_PATH += -I$(CODE_ROOT)/base/arch/$(OT_ARCH)/include
endif

CUR_DIR := $(PWD)

####################################################################
# hal
####################################################################
MKP_CFLAGS += -DHDMI_SUPPORT_LOGIC_V100
HAL := v100
HAL_DIR := hal/ctrl/$(HAL)

PRODUCT_DIR := product/$(HDMI_CHIP)
LINUX_VERSION := osal/v600

INC_PATH += -I$(CUR_DIR)
INC_PATH += -I$(CUR_DIR)/hal
INC_PATH += -I$(CUR_DIR)/proc
INC_PATH += -I$(CUR_DIR)/$(LINUX_VERSION)
INC_PATH += -I$(CUR_DIR)/$(PRODUCT_DIR)
INC_PATH += -I$(CUR_DIR)/$(PRODUCT_DIR)/regs
INC_PATH += -I$(CUR_DIR)/$(CTRL_DIR)
INC_PATH += -I$(CUR_DIR)/$(PHY_DIR)
INC_PATH += -I$(CUR_DIR)/$(PHY_DIR)/regs
INC_PATH += -I$(CUR_DIR)/$(CTRL_DIR)/regs

SRCS += $(CTRL_DIR)/regs/hdmi_reg_aon.c\
        $(CTRL_DIR)/regs/hdmi_reg_audio_path.c\
        $(CTRL_DIR)/regs/hdmi_reg_ctrl.c\
        $(CTRL_DIR)/regs/hdmi_reg_tx.c\
        $(CTRL_DIR)/regs/hdmi_reg_video_path.c

SRCS += $(CTRL_DIR)/hdmi_hal_intf.c\
        $(CTRL_DIR)/hdmi_hal_machine.c\
        $(CTRL_DIR)/hdmi_hal_ddc.c\
        $(CTRL_DIR)/hdmi_hal_ncts.c\
        $(CTRL_DIR)/hdmi_hal_ctrl.c

SRCS += $(PRODUCT_DIR)/regs/hdmi_reg_crg.c\
        $(PRODUCT_DIR)/hdmi_product_define.c\
        $(PHY_DIR)/regs/hdmi_reg_dphy.c\
        $(PHY_DIR)/hdmi_hal_phy.c

SRCS += drv_hdmi_infoframe.c\
        drv_hdmi_event.c\
        drv_hdmi_edid.c\
        drv_hdmi_intf.c\
        drv_hdmi_intf_k.c\
        drv_hdmi_common.c\
        drv_hdmi_compatibility.c\
        $(LINUX_VERSION)/hdmi_osal.c

ifeq ($(CONFIG_OT_PROC_SHOW_SUPPORT), y)
SRCS += drv_hdmi_proc.c
endif

ifeq ($(FRL_SUPPORT), y)
MKP_CFLAGS += -DHDMI_FRL_SUPPORT
SRCS += $(CTRL_DIR)/regs/hdmi_reg_frl.c\
        $(CTRL_DIR)/hdmi_hal_frl.c\
        drv_hdmi_frl.c\
        drv_hdmi_dfm.c
endif

ifeq ($(SCDC_SUPPORT), y)
MKP_CFLAGS += -DHDMI_SCDC_SUPPORT
SRCS += $(CTRL_DIR)/hdmi_hal_scdc.c
endif

ifeq ($(HDR_SUPPORT), y)
MKP_CFLAGS += -DHDMI_HDR_SUPPORT
endif

ifeq ($(OSTYPE),liteos)
MKP_CFLAGS += -DHDMI_LITEOS_SUPPORT
else
# if debug support, add drv_hdmi_debug.c
MKP_CFLAGS += -DHDMI_DEBUG_SUPPORT
SRCS += drv_hdmi_debug.c
endif

INIT_FILE := ../init/$(OT_ARCH)/$(OSTYPE)/src/hdmi_init.c
INIT_INC := $(PWD)/../init/$(OT_ARCH)/$(OSTYPE)/include/ot_hdmi_mod_init.h
INC_PATH += -I$(PWD)/../init/$(OT_ARCH)/$(OSTYPE)/include
INC_PATH += -I$(CBB_ROOT)/based/ext_inc

MOD_NAME := mod_hdmi

SRCS_O := $(SRCS)
SRCS += $(INIT_FILE)

MKP_CFLAGS += -DHDMI_DEFUALT_CALLBACK_SUPPORT

ifeq ($(FPGA),y)
MKP_CFLAGS += -DHDMI_FPGA_SUPPORT
endif

MKP_CFLAGS += -Wall -c -O2
MKP_CFLAGS += -I. $(INC_PATH)
MKP_CFLAGS += -D$(OT_FPGA) -D$(OT_ARCH)

#export hdmi functional support switch
MKP_CFLAGS += -DDVI_SUPPORT
MKP_CFLAGS += -DHDMI_LOG_SUPPORT

ifeq ($(CONFIG_DRIVER_SPACE_TYPE),user_space)
MKP_CFLAGS :=$(subst base/,based/,$(MKP_CFLAGS))
MKP_CFLAGS :=$(subst sys/,sysd/,$(MKP_CFLAGS))
endif

# release header
INC_FILE := ../include/ot_common_hdmi.h

define maco_cp_file
    if [ ! -z "$(1)" ];then cp -rf $(1) "$(2)"; fi
endef

MKP_CFLAGS += -I$(PWD)

TARGET := $(KO_PREFIX)_hdmi

# compile linux or liteos
include $(PWD)/../../../Makefile.$(OSTYPE)
