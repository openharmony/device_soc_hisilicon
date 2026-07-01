include ../Makefile.param

########### SRCS & INC ############
SMP_SRCS := $(wildcard $(PWD)/*.c)

MPI_LIBS += -lss_snap

TARGET := sample_snap

TARGET_PATH := $(PWD)

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
