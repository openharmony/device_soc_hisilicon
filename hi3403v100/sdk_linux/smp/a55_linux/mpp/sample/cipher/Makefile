include ../Makefile.param


########### SRCS & INC ############
SMP_SRCS := $(wildcard $(PWD)/*.c)

CFLAGS += -I$(REL_INC)
CFLAGS += -L$(REL_LIB) -lsecurec -lss_cipher -lss_klad

TARGET := sample_cipher

TARGET_PATH := $(PWD)

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
