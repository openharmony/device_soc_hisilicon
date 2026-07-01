
include ../Makefile.param

ifeq ($(ARM_ARCH),amp)
AMP_SRCS := $(wildcard *.c)
TARGET := $(AMP_SRCS:%.c=%)
else
SMP_SRCS := $(wildcard *.c)
TARGET := $(SMP_SRCS:%.c=%)
endif

TARGET_PATH := $(PWD)

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak