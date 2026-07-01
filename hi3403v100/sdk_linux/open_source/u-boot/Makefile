BUILD_DIR := $(shell pwd)
OSDRV_DIR ?= ../../osdrv

MAKE=make
CHIP ?= ss928v100

UBOOT_VER ?=u-boot-2020.01
UBOOT_TAR:=$(UBOOT_VER).tar.bz2
ARCH_TYPE ?= arm64
ARM_ARCH_VERSION ?=armv8
UBOOT_REG_BIN :=$(CHIP)_reg_info.bin
UBOOT_PATCH_NAME:=u-boot-2020.01

BOOT_MEDIA ?= spi
ifeq ($(BOOT_MEDIA), spi)
MEDIUM_FLAG = _
endif
ifeq ($(BOOT_MEDIA), nand)
MEDIUM_FLAG = _nand_
endif
ifeq ($(BOOT_MEDIA), emmc)
MEDIUM_FLAG = _emmc_
endif

GZIP_TOOL ?=
OSDRV_CROSS ?= aarch64-v01c01-linux-gnu
UBOOT_CONFIG ?=$(CHIP)$(MEDIUM_FLAG)defconfig

GREEN = "\e[32;1m"
DONE    = "\033[0m"

all:
	@echo -e $(GREEN)"-------- uboot : $(UBOOT_VER) "  $(DONE)

ifneq ($(BUILD_DIR)/$(UBOOT_VER), $(wildcard $(BUILD_DIR)/$(UBOOT_VER)))
	pushd $(BUILD_DIR);tar xjf $(UBOOT_TAR) -C .;popd
	pushd $(BUILD_DIR)/$(UBOOT_VER);patch -p1 < ../$(UBOOT_PATCH_NAME).patch;popd
endif

ifeq ($(shell find $(OSDRV_DIR)/tools/pc/uboot_tools -name "$(UBOOT_REG_BIN)"), )
	$(warning "-------------- ------------------------------------------------------")
	$(warning "          Cannot found the $(UBOOT_REG_BIN) file                     ")
	$(warning "          Please to generate $(UBOOT_REG_BIN) file                   ")
	$(warning "---------------------------------------------------------------------")
	$(error )
endif
	cp $(OSDRV_DIR)/tools/pc/uboot_tools/$(UBOOT_REG_BIN) $(BUILD_DIR)/$(UBOOT_VER)/.reg
	$(MAKE) -C $(BUILD_DIR)/$(UBOOT_VER) CROSS_COMPILE=$(OSDRV_CROSS)- $(UBOOT_CONFIG)
	$(MAKE) -C $(BUILD_DIR)/$(UBOOT_VER) CROSS_COMPILE=$(OSDRV_CROSS)-
	$(MAKE) -C $(BUILD_DIR)/$(UBOOT_VER) CROSS_COMPILE=$(OSDRV_CROSS)- u-boot-z.bin

clean:
ifeq ($(BUILD_DIR)/$(UBOOT_VER), $(wildcard $(BUILD_DIR)/$(UBOOT_VER)))
ifeq ($(BUILD_DIR)/$(UBOOT_VER)/.config, $(wildcard $(BUILD_DIR)/$(UBOOT_VER).config))
	$(MAKE) -C $(BUILD_DIR)/$(UBOOT_VER) u-boot-z.clean
endif
	$(MAKE) -C $(BUILD_DIR)/$(UBOOT_VER) clean
	rm -f $(BUILD_DIR)/$(UBOOT_VER)/.reg
endif

distclean:
	rm $(BUILD_DIR)/$(UBOOT_VER) -rf

