
include $(PWD)/../Makefile.param

SMP_SRCS := $(wildcard *.c)
SMP_SRCS += $(wildcard ./sample_svp_dpu/*.c)
SMP_SRCS += $(wildcard ./common/*.c)

CFLAGS += -I$(PWD)/common/
CFLAGS += -I$(PWD)/sample_svp_dpu/
TARGET := sample_dpu_main
TARGET_PATH := $(PWD)
# compile linux or LITEOS
include $(PWD)/../../$(ARM_ARCH)_$(OSTYPE).mak
