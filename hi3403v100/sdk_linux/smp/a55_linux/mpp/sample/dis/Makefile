
include ../Makefile.param

SMP_SRCS := sample_dis.c
ifeq ($(GYRO_DIS), y)
SMP_SRCS += sample_gyro_dis.c
SMP_SRCS += sample_fov2ldc.c
SMP_SRCS += sample_dis_two_pipe.c
endif
TARGET := sample_dis

TARGET_PATH := $(PWD)

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
