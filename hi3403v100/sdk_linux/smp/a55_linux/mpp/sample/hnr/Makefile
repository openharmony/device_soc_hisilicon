include ../Makefile.param

TARGET := sample_hnr

TARGET_PATH := $(PWD)

SMP_SRCS := $(wildcard $(PWD)/sample_hnr.c)

ifeq ($(CONFIG_OT_ISP_HNR_SUPPORT), y)
CFLAGS += -DSAMPLE_HNR_SUPPORT
MPI_LIBS += -lss_pqp
MPI_LIBS += -lss_hnr
endif

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak

