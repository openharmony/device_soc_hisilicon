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

#=======================================
#	export variable
#=======================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(KERNELRELEASE),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(KERNEL_DIR)/../../..
    else
    SDK_DIR := $(CURDIR)/../../..
    endif

    ifneq ($(SDK_SOURCE_DIR),)
    SDK_DIR := $(SDK_SOURCE_DIR)/..
    endif
include $(SDK_DIR)/base.mak
endif

#=======================================
#	local variable
#======================================
objs_depended := pm
objs_depended += gpio
ifneq ($(CFG_HI_USER_DRV),y)
objs_depended += pq
endif
ifneq ($(CFG_HI_CHIP_TYPE),hi3751v350)
objects := spi
endif
ifeq ($(CFG_HI_FPGA),y)
objects += ssp
endif
objects += hiirq
objects += hiproc
objects += i2c
objects += frontend
objects += vfe
objects += tvd
objects += hddec
objects += hdmirx
ifneq ($(CFG_HI_USER_DRV),y)
objects += panel
endif
ifeq ($(CFG_HI_MEMC_SUPPORT),y)
objects += memc
endif
ifneq ($(CFG_HI_USER_DRV),y)
objects += vo
endif
objects += pdm
ifneq ($(CFG_HI_USER_DRV),y)
objects += tde
objects += hifb
endif
objects += vi
objects += adsp
objects += aiao
objects += aflt
objects += adec
objects += demux
objects += jpeg
objects += vfmw
ifneq ($(CFG_HI_USER_DRV),y)
objects += vpss
endif
objects += vdec
objects += omxvdec
objects += vplugin
objects += avplay
objects += sync
objects += sif
objects += ir
objects += lsadc
objects += wdg
objects += gpu
objects += cipher
ifeq ($(CFG_HI_SCI_SUPPORT), y)
objects += sci
endif
objects += otp
ifneq ($(CFG_HI_USER_DRV),y)
objects += custom
endif
objects += pwm
ifneq ($(CFG_HI_USER_DRV),y)
objects += rm
endif
objects += dbe
objects += vbi
objects += mddrc

ifeq ($(CFG_HI_FDMNG_SUPPORT), y)
objects += fdmng
endif

ifeq ($(CFG_HI_PVR_SUPPORT),y)
objects += pvr
endif

ifeq ($(CFG_HI_ADVCA_SUPPORT), y)
objects += advca
endif

ifeq ($(CFG_HI_AENC_SUPPORT),y)
objects += aenc
endif

ifeq ($(CFG_HI_VENC_SUPPORT),y)
objects += venc
endif

ifeq ($(CFG_HI_CIPLUS_SUPPORT),y)
objects += ci
ifeq ($(CFG_HI_CI_DEV_CIMAXPLUS),y)
objects += cimaxplus
endif
endif
ifeq ($(CFG_HI_SND_AMP_SUPPORT),y)
objects += amp
endif
ifneq ($(CFG_HI_ADAC_SLIC_SUPPORT)_$(CFG_HI_ADAC_TVL320AIC31_SUPPORT),_)
objects += adac
endif

ifeq ($(CFG_MSP_BUILDTYPE),y)
objects += base

ifeq ($(KERNEL_VERSION), linux-5.10)
ifeq ($(CFG_HI_USER_DRV),y)
objects := hiirq
objects += hiproc
objects += hidev
objects += gpio
objects += i2c
objects += gt911
objects += fm11nt081d
objects += devfreq
objs_depended :=
else
objects += gt911
objects += fm11nt081d
objects += devfreq
objects += securec
endif

EXTRA_CFLAGS += -I$(srctree)/bounds_checking_function/include
endif

obj-$(CFG_MSP_BUILDTYPE) += ${addsuffix /,${objs_depended}}
obj-$(CFG_MSP_BUILDTYPE) += ${addsuffix /,${objects}}
endif

objects_clean:=${addsuffix _clean,${objects}}
objects_install:=${addsuffix _install,${objects}}
objects_uninstall:=${addsuffix _uninstall,${objects}}
objects_prepare:=${addsuffix _prepare,${objects}}

objs_depended_clean:=${addsuffix _clean,${objs_depended}}
objs_depended_install:=${addsuffix _install,${objs_depended}}
objs_depended_uninstall:=${addsuffix _uninstall,${objs_depended}}
objs_depended_prepare:=${addsuffix _prepare,${objs_depended}}


#=================================================================
#	  rules
#=================================================================

.PHONY: all clean install uninstall ${objects} ${objects_clean} ${objects_install} ${objects_uninstall} ${objects_prepare}

all: ${objects}

clean: ${objects_clean} ${objs_depended_clean}
	$(AT)rm -rf  *.o
	$(AT)rm -rf  .*.o.cmd
	$(AT)rm -rf  modules.order modules.builtin

install: ${objects_install} 
${objects_install}: ${objs_depended_install}

uninstall: ${objects_uninstall} ${objs_depended_uninstall}

prepare: ${objects_prepare} ${objs_depended_prepare}

${objects}:
	make -C $@
${objs_depended}:
	make -C $@

${objects_clean}:
	make -C ${patsubst %_clean,%,$@} clean
${objs_depended_clean}:
	make -C ${patsubst %_clean,%,$@} clean

${objects_install}:
	make -C ${patsubst %_install,%,$@} install
${objs_depended_install}:
	make -C ${patsubst %_install,%,$@} install

${objects_uninstall}:
	make -C ${patsubst %_uninstall,%,$@} uninstall
${objs_depended_uninstall}:
	make -C ${patsubst %_uninstall,%,$@} uninstall

${objects_prepare}:
	@echo "prepare "$@
#	make -C ${patsubst %_prepare,%,$@} prepare
