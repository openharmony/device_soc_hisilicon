include ../Makefile.param

UAC_COMPILE := n
#UAC_COMPILE := y

ifeq ($(UAC_COMPILE), y)
CFLAGS += -DOT_UAC_COMPILE
endif

SRC_PATH := $(shell pwd)
# INC_FLAGS += -I$(SRC_PATH)/include
# CFLAGS += -Wall -g $(INC_FLAGS)

SMP_SRCS := $(SRC_PATH)/application.c
SMP_SRCS += $(SRC_PATH)/camera.c
SMP_SRCS += $(SRC_PATH)/frame_cache.c
SMP_SRCS += $(SRC_PATH)/ot_stream.c
SMP_SRCS += $(SRC_PATH)/ot_uvc.c
SMP_SRCS += $(SRC_PATH)/log.c
SMP_SRCS += $(SRC_PATH)/sample_yuv.c
SMP_SRCS += $(SRC_PATH)/uvc_venc_glue.c
SMP_SRCS += $(SRC_PATH)/ot_camera_gadget.c

SMP_SRCS += $(SRC_PATH)/sample_comm_venc.c
SMP_SRCS += $(SRC_PATH)/sample_venc.c

SMP_SRCS += $(SRC_PATH)/ot_audio.c
SMP_SRCS += $(SRC_PATH)/ot_uac.c
SMP_SRCS += $(SRC_PATH)/uvc_media.c

ifeq ($(UAC_COMPILE), y)
SMP_SRCS += $(SRC_PATH)/sample_audio.c
endif

# SMP_SRCS := $(wildcard *.c)

# ALSA
ifeq ($(UAC_COMPILE), y)
AUDIO_ALSA_DIR ?= $(shell pwd)/alsa
COMM_INC += -I$(AUDIO_ALSA_DIR)/include
MPI_LIBS += -L$(AUDIO_ALSA_DIR)/lib/ -lasound
endif

TARGET := sample_uvc

TARGET_PATH := $(PWD)

# compile linux or LITEOS
include $(PWD)/../$(ARM_ARCH)_$(OSTYPE).mak
