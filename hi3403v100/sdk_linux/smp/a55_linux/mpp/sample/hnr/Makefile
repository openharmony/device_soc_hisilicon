include ../Makefile.param

TARGET := sample_hnr

TARGET_PATH := $(PWD)

SMP_SRCS := $(wildcard $(PWD)/sample_hnr.c)

ifeq ($(CONFIG_OT_ISP_HNR_SUPPORT), y)
CFLAGS += -DSAMPLE_HNR_SUPPORT
MPI_LIBS += $(REL_LIB)/libss_pqp.so
MPI_LIBS += $(REL_LIB)/libss_hnr.so
endif

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak

