#*************************************************************************
ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../../Makefile.param
    include $(CBB_PARAM_FILE)
endif

ifeq ($(CONFIG_OT_VO_SUPPORT), y)
#*************************************************************************
# local header
MKP_CFLAGS+=-I $(PWD)/include
MKP_CFLAGS+=-I $(PWD)/ext_inc
MKP_CFLAGS+=-I $(PWD)/mkp/include
MKP_CFLAGS+=-I $(PWD)/mkp/src/common
MKP_CFLAGS+=-I $(PWD)/mkp/src/dev
MKP_CFLAGS+=-I $(PWD)/mkp/src/intf
MKP_CFLAGS+=-I $(PWD)/mkp/src/layer
MKP_CFLAGS+=-I $(PWD)/mkp/src/chn
MKP_CFLAGS+=-I $(PWD)/mkp/src/wbc
MKP_CFLAGS+=-I $(PWD)/arch/include
MKP_CFLAGS+=-I $(PWD)/arch/$(OT_ARCH)/include
MKP_CFLAGS+=-I $(PWD)/arch/comm/src
MKP_CFLAGS+=-I $(PWD)/arch/comm/include
ifeq ($(CONFIG_DRIVER_SPACE_TYPE),user_space)
MKP_CFLAGS :=$(subst base/,based/,$(MKP_CFLAGS))
MKP_CFLAGS :=$(subst sys/,sysd/,$(MKP_CFLAGS))
endif
#*************************************************************************
# all source file in this module
SRCS :=

# src relative path
CBB_VO_ROOT=.
CBB_VO_ARCH=arch/$(OT_ARCH)
CBB_VO_ARCH_COMM_SRC=arch/comm/src

# Makefile absolute path
include $(CBB_ROOT)/vo/vo_dev/mkp/src/Makefile
include $(CBB_ROOT)/vo/vo_dev/arch/$(OT_ARCH)/hal/Makefile
include $(CBB_ROOT)/vo/vo_dev/$(CBB_VO_ARCH_COMM_SRC)/Makefile

INIT_FILE := ./init/$(OSTYPE)/src/vo_init.c
INIT_INC := ./init/$(OSTYPE)/include/ot_vo_mod_init.h
MKP_CFLAGS += -I$(PWD)/./init/$(OSTYPE)/include
SRCS_O := $(SRCS)
SRCS += $(INIT_FILE)

#*************************************************************************
# release header
# INC_FILE := $(wildcard include/*.h)

MOD_NAME := mod_vo

TARGET   := $(KO_PREFIX)_vo
#*************************************************************************
# compile linux or liteos
include $(PWD)/../../Makefile.$(OSTYPE)

else
all:
	@echo "vo dev module not compile"
clean:
	@echo "vo dev module don't need clean"
endif
