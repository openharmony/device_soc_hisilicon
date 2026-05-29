include $(PWD)/../Makefile.param

CFLAGS += -I$(PWD)/include
CFLAGS += -O3

SMP_SRCS := $(wildcard ./*.c)
SMP_SRCS += $(wildcard ./sample_svp_mau/*.c)
SMP_SRCS += $(wildcard $(PWD)/../common/*.c)
TARGET := sample_mau_main
TARGET_PATH := $(PWD)

# compile linux or LITEOS
include $(PWD)/../../$(ARM_ARCH)_$(OSTYPE).mak