# $(ROOT)/mpp/code/component/hdmi/mpi/Makefile

ifeq ($(PARAM_FILE), ) 
    PARAM_FILE:=../../../../Makefile.param
    include $(PARAM_FILE)
endif

include ../../../../Makefile.$(OSTYPE).param

HDMI_VERSION := hdmi

MPP_PATH := $(SDK_PATH)/mpp
CODE_ROOT := $(MPP_PATH)/cbb

#################################################
MKP_PATH := $(PWD)/../mkp
INC_PATH += -I$(MKP_PATH)\
            -Icommon
#################################################
#FLAGS += -Wall -fPIC -c -O2
#FLAGS += -I. $(INC_PATH) -lpthread
#FLAGS += -D$(OT_FPGA) -D$(OT_ARCH) -DDVI_SUPPORT

# Security compiler options
FLAGS += $(LIBS_CFLAGS)
FLAGS += $(MPP_CFLAGS)

ifeq ($(OT_ARCH), ss528v100)
ifeq ($(OT_SUBARCH), ss625v100)
    FLAGS += -DHDMI_PRODUCT_SS625V100
    FLAGS += -DOT_HDMI_SUPPORT_1_4
else
    FLAGS += -DHDMI_PRODUCT_SS528V100
endif
endif

ifeq ($(OT_ARCH), ss524v100)
    FLAGS += -DHDMI_PRODUCT_SS524V100
    FLAGS += -DOT_HDMI_SUPPORT_1_4
endif

ifeq ($(OT_ARCH), ss928v100)
    FLAGS += -DHDMI_PRODUCT_SS928V100
endif

ifeq ($(OT_ARCH), ss626v100)
    FLAGS += -DHDMI_PRODUCT_SS626V100
    FLAGS += -DHDMI_SUPPORT_DUAL_CHANNEL
endif
INC_PATH += -I$(PWD)/../include\
            -I$(PWD)/../mkp\
            -I$(CODE_ROOT)/include\
            -I$(CODE_ROOT)/base/ext_inc\
            -I$(CODE_ROOT)/base/arch/$(OT_ARCH)/include\
            -I$(MPP_PATH)/component/securec/include

FLAGS += -Wall -fPIC -c -O2
FLAGS += -I. $(INC_PATH) -lpthread
FLAGS += -D$(OT_FPGA) -D$(OT_ARCH) -DDVI_SUPPORT
FLAGS += -I$(REL_DIR)/include
FLAGS += -I$(CBB_ROOT)/based/ext_inc

ifeq ($(OT_ARCH), $(filter $(OT_ARCH), ss528v100))
INC_PATH += -I$(CODE_ROOT)/base/arch/$(OT_ARCH)/include/$(OT_SUBARCH)
else
INC_PATH += -I$(CODE_ROOT)/base/arch/$(OT_ARCH)/include
endif

ifeq ($(HDR_SUPPORT), y)
FLAGS += -DHDMI_HDR_SUPPORT
endif

ifeq ($(HDMI21_SUPPORT), y)
FLAGS += -DHDMI_SUPPORT_2_1
endif

ifeq ($(CEC_SUPPORT), y)
FLAGS += -DHDMI_CEC_SUPPORT
endif

HDMI_SRC := $(PWD)/mpi_hdmi_com.c
HDMI_SRC +=  $(PWD)/ot_mpi_hdmi.c

$(OT_ARCH)_hdmi-objs := $(HDMI_SRC:%.c=%.o)
LIB := libot_hdmi.a
LIB_SO := libot_hdmi.so

INC_FILE := ../include/ot_common_hdmi.h
INC_FILE += ../include/ot_mpi_hdmi.h

define maco_cp_file
    if [ ! -z "$(1)" ];then cp -rf $(1) "$(2)"; fi
endef

include $(PWD)/Makefile.$(OSTYPE)

