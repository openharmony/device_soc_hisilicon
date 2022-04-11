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
ifndef LITEOSTOPDIR
.EXPORT_ALL_VARIABLES:
CUR_DIR=${PWD}
SUB_DIR=/source/msp/drv/i2c
TOP_DIR=$(subst $(SUB_DIR),,$(CUR_DIR))
SOURCE_DIR=$(TOP_DIR)/source
include $(TOP_DIR)/base.mak
endif

depends := gpio_i2c
depends += std_i2c

###############################################################################
#         rules
###############################################################################
.PHONY: all clean $(depends)

all: $(depends)

$(depends):
	make -C $@

clean:
	for dir in $(depends); \
		do make -C $$dir clean || exit 1; \
	done
else

#=======================================
#	export variable
#=======================================
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

#=======================================
#	local variable
#=======================================
#objects:=${subst /,,${shell ls -d */}}
objects :=

ifeq ($(CFG_HI_GPIOI2C_SUPPORT),y)	  
CFG_HI_KMOD_CFLAGS += -DHI_GPIOI2C_SUPPORT
objects += gpio_i2c
endif

objects += std_i2c

objects_clean:=${addsuffix _clean,${objects}}
objects_install:=${addsuffix _install,${objects}}
objects_uninstall:=${addsuffix _uninstall,${objects}}

ifeq ($(CFG_MSP_BUILDTYPE),y)
obj-$(CFG_MSP_BUILDTYPE) += ${addsuffix /,${objects}}
endif
#=================================================================
#	  rules
#=================================================================

.PHONY: all clean install uninstall ${objects} ${objects_clean} ${objects_install} ${objects_uninstall}

all: ${objects}

clean: ${objects_clean}
	$(AT)rm -rf  *.o
	$(AT)rm -rf  .*.o.cmd
	$(AT)rm -rf  modules.order modules.builtin

install: ${objects_install}

uninstall: ${objects_uninstall}

${objects}:
	make -C $@

${objects_clean}:
	make -C ${patsubst %_clean,%,$@} clean

${objects_install}:
	make -C ${patsubst %_install,%,$@} install

${objects_uninstall}:
	make -C ${patsubst %_uninstall,%,$@} uninstall

endif
