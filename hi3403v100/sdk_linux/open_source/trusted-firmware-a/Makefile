BUILD_DIR := $(shell pwd)

MAKE=make

CHIP ?= ss928v100

KERNEL_VER ?=linux-6.6.y

ifneq ($(filter $(CHIP), ss928v100), )
BUILD_CHIP = ss928v100
else ifneq ($(filter $(CHIP), ss927v100), )
BUILD_CHIP = ss927v100
else
$(warning "warning: "$(CHIP)" is invalid")
endif

ATF_VER:=arm-trusted-firmware-2.2
ATF_TAR:=arm-trusted-firmware-2.2.tar.gz

OSDRV_CROSS ?= aarch64-v01c01-linux-gnu
export LDFLAGS = --no-warn-rwx-segments

GREEN = "\e[32;1m"
DONE    = "\033[0m"

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

ARCH_TYPE ?= arm64

UIMAGE ?=uImage_$(CHIP)

all:
ifneq ($(BUILD_DIR)/$(ATF_VER), $(wildcard $(BUILD_DIR)/$(ATF_VER)))
	pushd $(BUILD_DIR);tar xzf $(ATF_TAR) -C .;popd
	pushd $(BUILD_DIR)/$(ATF_VER);patch -p1 < ../trusted-firmware-a-2.2.patch;popd
endif
	find $(BUILD_DIR)/$(ATF_VER) | xargs touch
	$(MAKE) -C $(BUILD_DIR)/$(ATF_VER) PLAT=$(BUILD_CHIP) SPD=none BL33=$(BUILD_DIR)/../linux/$(KERNEL_VER)/arch/arm64/boot/uImage \
		CCI_UP=0 DEBUG=0 BL33_SEC=0 fip CROSS_COMPILE=$(OSDRV_CROSS)-

clean:
ifeq ($(BUILD_DIR)/$(ATF_VER), $(wildcard $(BUILD_DIR)/$(ATF_VER)))
	$(MAKE) -C $(BUILD_DIR)/$(ATF_VER) PLAT=$(BUILD_CHIP) clean
endif

distclean:
	rm $(BUILD_DIR)/$(ATF_VER) -rf;
