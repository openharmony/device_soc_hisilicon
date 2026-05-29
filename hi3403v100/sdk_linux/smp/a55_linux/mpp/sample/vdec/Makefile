
include ../Makefile.param

SMP_SRCS := $(wildcard *.c)
TARGET := $(SMP_SRCS:%.c=%)

TARGET_PATH := $(PWD)

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
