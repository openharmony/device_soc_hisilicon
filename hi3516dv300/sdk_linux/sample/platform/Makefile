ifeq ($(PARAM_FILE), )
	PARAM_FILE:=../../Makefile.param
	include $(PARAM_FILE)
endif

ifeq ($(ARM_ARCH), amp)

ifeq ($(OSTYPE), liteos)

COMMON_DIR ?= $(shell pwd)/common
AUDIO_ADP_DIR ?= $(shell pwd)/audio/adp

include $(shell pwd)/./Makefile.param

ifeq ($(LITEOS_BIN_SUPPORT_VI), y)
AMP_SERVER_SRCS += $(wildcard $(shell pwd)/vio/$(ARM_ARCH)/server/*.c)
AMP_INC += -I$(shell pwd)/vio/
AMP_INC += -I$(shell pwd)/vio/$(ARM_ARCH)/include
endif

TARGET := sample
TARGET_PATH := $(shell pwd)

include $(shell pwd)/./$(ARM_ARCH)_$(OSTYPE).mak

else

all:
	@cd vio;     make
ifeq ($(ARCH_LIBNAME),$(filter $(ARCH_LIBNAME), hi3559v200))
	@cd svp_amp; make
endif
clean:
	@cd vio;     make clean
ifeq ($(ARCH_LIBNAME),$(filter $(ARCH_LIBNAME), hi3559v200))
	@cd svp_amp; make clean
endif
endif

else

all:
	@cd iniparser; make
	@cd awb_online_calibration; make
	@cd dis;     make
	@cd fisheye; make
	@cd hifb;    make
	@cd lsc_online_cali; make
	@cd region;    make
	@cd snap;    make
	@cd tde;     make
	@cd vdec;    make
	@cd venc;    make
	@cd vgs;     make
	@cd vio;     make
	@cd vo;      make
	@cd svp;     make
	@cd scene_auto;make
	@cd calcflicker; make
ifeq ($(CONFIG_HI_VENC_SVC_SUPPORT), y)
	@cd $(SDK_PATH)/open_source/iniparser;make
	@cd bitrate_auto;make
endif
ifneq ($(CONFIG_HI_AUDIO_ONLY_DRIVER_SUPPORT), y)
	@cd audio;   make
endif
ifeq ($(OSTYPE), linux)
	@cd traffic_capture; make
	@cd uvc_app; make
endif
ifeq ($(OSTYPE)_$(CONFIG_ARM_ARCH_TYPE), liteos_smp)
	@cd quickstart; make
	@cd uvc_app; make
endif

clean:
	@cd iniparser; make clean
	@cd awb_online_calibration; make clean
	@cd dis;     make clean
	@cd fisheye; make clean
	@cd hifb;    make clean
	@cd lsc_online_cali; make clean
	@cd region;  make clean
	@cd snap;    make clean
	@cd tde;     make clean
	@cd vdec;    make clean
	@cd venc;    make clean
	@cd vgs;     make clean
	@cd vio;     make clean
	@cd vo;      make clean
	@cd svp;     make clean
	@cd scene_auto;make clean
	@cd calcflicker; make clean
ifeq ($(CONFIG_HI_VENC_SVC_SUPPORT), y)
	@cd $(SDK_PATH)/open_source/iniparser;make clean
	@cd bitrate_auto;make clean
endif
ifneq ($(CONFIG_HI_AUDIO_ONLY_DRIVER_SUPPORT), y)
	@cd audio;   make clean
endif
ifeq ($(OSTYPE), linux)
	@cd traffic_capture; make clean
	@cd uvc_app; make clean
endif
ifeq ($(OSTYPE)_$(CONFIG_ARM_ARCH_TYPE), liteos_smp)
	@cd quickstart; make clean
	@cd uvc_app; make clean
endif
endif
