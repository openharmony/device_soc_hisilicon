##########################################################################################
#	prepare param
##########################################################################################
export OSDRV_DIR=$(shell pwd)
SHELL := /bin/bash
ifeq ($(OSDRV_DIR)/../open_source, $(wildcard $(OSDRV_DIR)/../open_source))
OPEN_SOURCE=$(OSDRV_DIR)/../open_source
else ifeq ($(OSDRV_DIR)/../../open_source, $(wildcard $(OSDRV_DIR)/../../open_source))
OPEN_SOURCE=$(OSDRV_DIR)/../../open_source
else ifeq ($(OSDRV_DIR)/open_source, $(wildcard $(OSDRV_DIR)/open_source))
OPEN_SOURCE=$(OSDRV_DIR)/open_source
else
$(warning "---------------------------------------------------------------------")
$(warning "   Cannot found the open_source Contents!!       ")
$(warning "---------------------------------------------------------------------")
$(error )
endif

MAKE=make
LLVM ?= 1
export CHIP?=ss928v100
export OSDRV_CROSS ?= aarch64-openeuler-linux-gnu
export CROSS_COMPILE=$(OSDRV_CROSS)-
export BUILD :=all
ARCH_TYPE ?= arm64
BOOT_MEDIA ?= emmc

ifneq ($(CHIP),ss928v100)
ifneq ($(CHIP),ss927v100)
$(error you must set valid CHIP:ss928v100 or ss927v100!)
endif
endif

ifeq ($(BOOT_MEDIA), spi)
MEDIUM_FLAG = _
endif

ifeq ($(BOOT_MEDIA), nand)
MEDIUM_FLAG = _nand_
endif
ifeq ($(BOOT_MEDIA), emmc)
MEDIUM_FLAG = _emmc_
ifeq ($(CHIP),ss928v100)
REGBIN_XLSM ?= SS928V100_demo_8L_LPDDR4x_3733M-8GB_32bitx2-A55_1400M-emmc.xlsm
endif
ifeq ($(CHIP),ss927v100)
REGBIN_XLSM ?= SS927V100_demo_8L_LPDDR4x_2666M-8GB_32bitx2-A55_1400M-eMMC.xlsm
endif
endif

ifeq ($(CHIP),ss928v100)
REGBIN_XLSM ?= SS928V100_demo_8L_LPDDR4x_3733M-8GB_32bitx2-A55_1400M.xlsm
endif
ifeq ($(CHIP),ss927v100)
REGBIN_XLSM ?= SS927V100_demo_8L_LPDDR4x_2666M-8GB_32bitx2-A55_1400M.xlsm
endif

export KERNEL_CFG:=$(CHIP)$(MEDIUM_FLAG)defconfig
export UBOOT_CONFIG:=$(CHIP)$(MEDIUM_FLAG)defconfig

ATF_VER:=arm-trusted-firmware-2.2
ATF_TAR:=$(ATF_VER).tar.gz
ATF_DIR:=$(OPEN_SOURCE)/trusted-firmware-a

KERNEL_VER:=linux-6.6.y
KERNEL_DIR:=$(OPEN_SOURCE)/linux

UBOOT_REG_BIN:=$(CHIP)_reg_info.bin
UBOOT_VER:=u-boot-2020.01
UBOOT_DIR:=$(OPEN_SOURCE)/u-boot
UBOOT_TAR:=$(UBOOT_VER).tar.bz2

REGBING_PACKAGE_VER:=regbin-v1.0.2
BOOT_COMPILE_DIR=$(OSDRV_DIR)/components/boot/image_map/
##########################################################################################
#	prepare regbin tools
##########################################################################################
REGBING_PACKAGE = $(shell find $(OSDRV_DIR)/tools/pc/uboot_tools/ \
					-name "$(REGBING_PACKAGE_VER).tgz")
ifeq ($(REGBING_PACKAGE), )
$(warning "---------------------------------------------------------------------")
$(warning "   Cannot found the $(REGBING_PACKAGE_VER).tgz package !!       ")
$(warning "---------------------------------------------------------------------")
$(error )
endif
TARGET_XLSM := $(REGBIN_XLSM)

ifeq ($(LLVM), 1)
export PUB_IMAGE:=$(CHIP)$(MEDIUM_FLAG)image_musl
else
export PUB_IMAGE:=$(CHIP)$(MEDIUM_FLAG)image_glibc
endif

BLACK = "\e[30;1m"
RED  =  "\e[31;1m"
GREEN = "\e[32;1m"
YELLOW = "\e[33;1m"
BLUE  = "\e[34;1m"
PURPLE = "\e[35;1m"
CYAN  = "\e[36;1m"
WHITE = "\e[37;1m"
DONE    = "\033[0m"
##########################################################################################
#	set task
##########################################################################################
all: prepare boot atf gslboot_build

clean: boot_clean kernel_clean atf_clean gslboot_clean regbin_clean

distclean: boot_distclean kernel_distclean atf_distclean gslboot_distclean regbin_distclean
	rm $(OSDRV_DIR)/pub -rf

##########################################################################################
#task [1]	prepare
##########################################################################################
prepare:
	mkdir $(OSDRV_DIR)/pub/$(PUB_IMAGE) -p

##########################################################################################
#	regbin_prepare
##########################################################################################
regbin_prepare:
	@echo "---------task [0]    regbin_prepare"
	@echo "---------TARGET_XLSM: $(TARGET_XLSM)"
	rm $(OSDRV_DIR)/tools/pc/uboot_tools/$(REGBING_PACKAGE_VER) -rf
	tar xzf $(REGBING_PACKAGE) -C $(OSDRV_DIR)/tools/pc/uboot_tools
	chmod 777 $(OSDRV_DIR)/tools/pc/uboot_tools/$(REGBING_PACKAGE_VER)/regbin
	cp $(OSDRV_DIR)/tools/pc/uboot_tools/$(TARGET_XLSM) $(OSDRV_DIR)/tools/pc/uboot_tools/$(REGBING_PACKAGE_VER)
	cd $(OSDRV_DIR)/tools/pc/uboot_tools/$(REGBING_PACKAGE_VER) && ./regbin $(TARGET_XLSM) $(UBOOT_REG_BIN);cd -
	mv $(OSDRV_DIR)/tools/pc/uboot_tools/$(REGBING_PACKAGE_VER)/$(UBOOT_REG_BIN) $(OSDRV_DIR)/tools/pc/uboot_tools
	rm $(OSDRV_DIR)/tools/pc/uboot_tools/$(REGBING_PACKAGE_VER) -rf

regbin_clean:
	rm -f $(OSDRV_DIR)/tools/pc/uboot_tools/$(UBOOT_REG_BIN)

regbin_distclean: regbin_clean

##########################################################################################
#task [1]	build uboot64
##########################################################################################
boot: prepare regbin_prepare
	@echo -e $(GREEN)"---------task [1]	build boot64  $(UBOOT_DIR)"   $(DONE)
	$(MAKE) -C $(UBOOT_DIR) CHIP=$(CHIP) BOOT_MEDIA=$(BOOT_MEDIA) \
			ARCH_TYPE=$(ARCH_TYPE) all
	cp $(UBOOT_DIR)/$(UBOOT_VER)/u-boot-$(CHIP).bin $(OSDRV_DIR)/pub/$(PUB_IMAGE)/

boot_clean:
	$(MAKE) -C $(UBOOT_DIR) clean

boot_distclean:
	$(MAKE) -C $(UBOOT_DIR) distclean

##########################################################################################
#task [2]	build atf
##########################################################################################
atf: kernel
	@echo -e $(GREEN)"---------task [2] build atf"  $(DONE)
	$(MAKE) -C $(ATF_DIR) CHIP=$(CHIP) BOOT_MEDIA=$(BOOT_MEDIA) \
				ARCH_TYPE=$(ARCH_TYPE) all
	cp $(ATF_DIR)/$(ATF_VER)/build/$(CHIP)/release/fip.bin $(OSDRV_DIR)/pub/$(PUB_IMAGE)/uImage_$(CHIP)

atf_clean:
	$(MAKE) -C $(ATF_DIR) clean

atf_distclean:
	$(MAKE) -C $(ATF_DIR) distclean

##########################################################################################
#task [2.1]	build kernel
##########################################################################################
kernel: prepare
	@echo -e $(GREEN)"---------task [2.1] build kernel"  $(DONE)
	$(MAKE) -C $(KERNEL_DIR) CHIP=$(CHIP) BOOT_MEDIA=$(BOOT_MEDIA) LLVM=$(LLVM) \
				ARCH_TYPE=$(ARCH_TYPE) all
kernel_clean:
	$(MAKE) -C $(KERNEL_DIR) clean

kernel_distclean:
	$(MAKE) -C $(KERNEL_DIR) distclean

##########################################################################################
#task [3]	build boot_image.bin
##########################################################################################
gslboot_build:boot
	@echo -e $(GREEN)"---------task [4] build boot_image.bin"  $(DONE)
	make -C $(OSDRV_DIR)/components/boot/gsl CROSS_COMPILE=$(CROSS_COMPILE) CHIP=$(CHIP)
	cp $(OSDRV_DIR)/tools/pc/uboot_tools/$(UBOOT_REG_BIN) $(BOOT_COMPILE_DIR)/.reg
	cp $(UBOOT_DIR)/$(UBOOT_VER)/u-boot-$(CHIP).bin $(BOOT_COMPILE_DIR)/u-boot-original.bin
	cp $(OSDRV_DIR)/components/boot/gsl/pub/gsl.bin $(BOOT_COMPILE_DIR)/gsl.bin

	python $(OSDRV_DIR)/components/boot/image_map/oem/oem_quick_build.py;
	cp $(BOOT_COMPILE_DIR)/image/oem/boot_image.bin $(OSDRV_DIR)/pub/$(PUB_IMAGE)/boot_image.bin
	@echo -e $(GREEN)"---------finish osdrv work"  $(DONE)

gslboot_clean:
	make -C $(OSDRV_DIR)/components/boot/gsl clean
	python $(OSDRV_DIR)/components/boot/image_map/oem/oem_main.py clean;
	python $(OSDRV_DIR)/components/boot/image_map/third_party/third_party_main.py clean;
	rm -f $(BOOT_COMPILE_DIR)/.reg
	rm -f $(BOOT_COMPILE_DIR)/u-boot-original.bin
	rm -f $(BOOT_COMPILE_DIR)/gsl.bin

gslboot_distclean: gslboot_clean

