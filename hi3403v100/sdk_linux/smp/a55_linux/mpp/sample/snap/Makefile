include ../Makefile.param

########### SRCS & INC ############
SMP_SRCS := $(wildcard $(PWD)/*.c)

MPI_LIBS += $(REL_LIB)/libss_snap.so

TARGET := sample_snap

TARGET_PATH := $(PWD)

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
