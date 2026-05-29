
include ../Makefile.param

SMP_SRCS := $(wildcard *.c)

TARGET := $(SMP_SRCS:%.c=%)

CFLAGS += -L$(REL_LIB) -lsecurec

SMP_SRCS += $(wildcard $(PWD)/adp/*.c)

TARGET_PATH := $(PWD)

# compile linux or liteos
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
