# Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

ifeq ($(CFG_PRODUCT_TYPE),liteos)
#===============================================================================
#===============================================================================
#===============================================================================
# export variables
#===============================================================================
ifndef LITEOSTOPDIR
.EXPORT_ALL_VARIABLES:
CUR_DIR=${PWD}
SUB_DIR=/source/msp/drv/gpio
TOP_DIR=$(subst $(SUB_DIR),,$(CUR_DIR))
SOURCE_DIR=$(TOP_DIR)/source
include $(TOP_DIR)/base.mak
endif

LIBOUT = $(MSPOUT_DRV_DIR)/gpio

ARFLAGS = cr
RM = -rm -rf

MODULE_NAME := gpio_drv
LIBM = $(MSP_LIB_DIR)/libhi_$(MODULE_NAME).a

TARGET := gpio

DRV_FILES = $(wildcard *.c)

DRV_OBJS = $(patsubst %.c,$(LIBOUT)/%.o, $(DRV_FILES))

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/api/gpio			        \
                -I$(MSP_DIR)/drv/gpio			        \

EXTRA_CFLAGS += -DLOG_D_MODULE_ID=HI_ID_GPIO

SDK_CFLAGS  =  $(CFG_HI_CFLAGS)
SDK_CFLAGS  +=  $(EXTRA_CFLAGS)

-include $(TOP_DIR)/rule.mak
#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean
all: $(LIBM)
clean:
	$(RM) $(LIBM) $(LIBOUT)/* *.bak *~

$(LIBM): $(DRV_OBJS)
	$(call make-ar,$@,$^)

$(LIBOUT)/%.o: %.c
	$(call make-depend-compile,$<,$@,$(subst .o,.d,$@),$(SDK_CFLAGS))

-include $(DRV_OBJS:.o=.d)

else
#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(KERNELRELEASE),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(KERNEL_DIR)/../../..
    else
    SDK_DIR := $(CURDIR)/../../../..
    endif

    ifneq ($(SDK_SOURCE_DIR),)
    SDK_DIR := $(SDK_SOURCE_DIR)/..
    endif

include $(SDK_DIR)/base.mak
endif

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(MSP_DIR)/drv/pq/Module.symvers
export KBUILD_EXTRA_SYMBOLS

#===============================================================================
# local variables
#===============================================================================
EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/api/gpio			        \
                -I$(MSP_DIR)/drv/gpio			        \

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -DLOG_D_MODULE_ID=HI_ID_GPIO

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

MOD_NAME := hi_gpio

$(MOD_NAME)-y := drv_gpio_intf.o	\
		 drv_gpio.o	

obj-$(CFG_MSP_BUILDTYPE) += $(MOD_NAME).o


#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C $(LINUX_DIR) ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(CFG_HI_TOOLCHAINS_NAME)- M=$(CURDIR) modules

clean:
	$(AT)make -C $(LINUX_DIR) M=$(CURDIR) clean
	$(AT)rm -rf  *.o
	$(AT)rm -rf  .*.o.cmd
	$(AT)rm -rf .tmp_versions
	$(AT)rm -rf  Module.symvers
	$(AT)rm -rf  modules.order

install: all
ifneq ($(CFG_MSP_BUILDTYPE),y)
	$(AT)cp -f $(CURDIR)/$(MOD_NAME).ko $(MODULE_DIR)/
endif

uninstall:
	$(AT)rm -rf $(MODULE_DIR)/$(MOD_NAME).ko
endif

