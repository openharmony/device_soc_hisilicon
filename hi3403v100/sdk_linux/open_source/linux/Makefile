BUILD_DIR := $(shell pwd)

MAKE=make
CHIP ?= ss928v100

KERNEL_ORIGIN_VER:=linux-6.6.86
KERNEL_VER:=linux-6.6.y

LLVM ?= 0
ARCH_TYPE ?= arm64
KERNEL_CFG ?=$(CHIP)_$(EMMC_FLAG)defconfig

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

ifeq ($(LLVM), 1)
COMPILE_OPTIONS := LLVM=1 CC="clang --target=aarch64-linux-musl" HOSTCC=gcc
else
OSDRV_CROSS ?= aarch64-v01c01-linux-gnu
COMPILE_OPTIONS := CROSS_COMPILE=$(OSDRV_CROSS)- LLVM=''
endif

GREEN = "\e[32;1m"
DONE    = "\033[0m"

all:
	@echo -e $(GREEN)"-------- kernel : $(KERNEL_ORIGIN_VER) "  $(DONE)

ifneq ($(BUILD_DIR)/$(KERNEL_VER), $(wildcard $(BUILD_DIR)/$(KERNEL_VER)))
	pushd $(BUILD_DIR);tar xf $(KERNEL_ORIGIN_VER).tar.gz;popd
	@echo -e $(GREEN)"patching kernel..."  $(DONE)
	pushd $(BUILD_DIR);mv $(KERNEL_ORIGIN_VER) $(KERNEL_VER);popd
	pushd $(BUILD_DIR)/$(KERNEL_VER); patch -p1< ../$(KERNEL_ORIGIN_VER).patch;popd
endif
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) $(COMPILE_OPTIONS) $(KERNEL_CFG)
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) $(COMPILE_OPTIONS) uImage
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) $(COMPILE_OPTIONS) scripts/module.lds

clean:
ifeq ($(BUILD_DIR)/$(KERNEL_VER), $(wildcard $(BUILD_DIR)/$(KERNEL_VER)))
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) $(COMPILE_OPTIONS) clean
endif

distclean:
	rm $(BUILD_DIR)/$(KERNEL_ORIGIN_VER) -rf
	rm $(BUILD_DIR)/$(KERNEL_VER) -rf

