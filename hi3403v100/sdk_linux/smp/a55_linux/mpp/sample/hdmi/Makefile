
include ../Makefile.param

SMP_SRCS := $(wildcard *.c)
TARGET := sample_hdmi

TARGET_PATH := $(PWD)

CFLAGS += -I./include/ -L$(REL_LIB) -lsecurec

ifeq ($(OT_ARCH), ss524v100)
    CFLAGS += -DHDMI_SUPPORT_1_4
endif

ifeq ($(OT_SUBARCH), ss625v100)
    CFLAGS += -DHDMI_SUPPORT_1_4
endif

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
