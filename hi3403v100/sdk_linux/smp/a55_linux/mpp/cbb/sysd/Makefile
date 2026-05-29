#*************************************************************************
ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../Makefile.param
    include $(PARAM_FILE)
endif

ifeq ($(CBB_PARAM_FILE), )
    CBB_PARAM_FILE:=../Makefile.param
    include $(CBB_PARAM_FILE)
endif
#*************************************************************************
# local header
MKP_CFLAGS+=-I $(PWD)/mkp/include
MKP_CFLAGS+=-I $(PWD)/arch/include
MKP_CFLAGS+=-I $(PWD)/arch/$(OT_ARCH)/include
ifeq ($(OT_ARCH), ss928v100)
MKP_CFLAGS+=-I $(SDK_PATH)/interdrv/$(OT_ARCH)/sysconfig/$(OT_SUBARCH)
else
MKP_CFLAGS+=-I $(SDK_PATH)/interdrv/$(OT_ARCH)/sysconfig
endif
MKP_CFLAGS :=$(subst base/,based/,$(MKP_CFLAGS))
MKP_CFLAGS :=$(subst sys/,sysd/,$(MKP_CFLAGS))
#*************************************************************************
# all source file in this module
#SRCS := mkp/src/sys_ext.c \
#        mkp/src/sys_bind.c
SRCS:=

# src relative path
CBB_SYS_ROOT=./
CBB_SYS_ARCH=arch/$(OT_ARCH)/

# Makefile absolute path
ifeq ($(CONFIG_DRIVER_SPACE_TYPE),user_space)
    include $(CBB_ROOT)/sysd/mkp/src/Makefile
    include $(CBB_ROOT)/sysd/arch/$(OT_ARCH)/hal/Makefile
else
    include $(CBB_ROOT)/sys/mkp/src/Makefile
    include $(CBB_ROOT)/sys/arch/$(OT_ARCH)/hal/Makefile
endif

INIT_FILE := ./init/$(OSTYPE)/src/sys_init.c
INIT_INC := ./init/$(OSTYPE)/include/ot_sys_mod_init.h
MKP_CFLAGS += -I$(PWD)/./init/$(OSTYPE)/include
SRCS_O := $(SRCS)
SRCS += $(INIT_FILE)

#*************************************************************************
# release header

MOD_NAME := mod_sys

TARGET   := $(KO_PREFIX)_sys
#*************************************************************************
# compile linux or liteos
include $(PWD)/../Makefile.$(OSTYPE)
