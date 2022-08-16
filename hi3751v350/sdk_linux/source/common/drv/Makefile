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

###############################################################################
# export variables
###############################################################################
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

###############################################################################
# local variables
###############################################################################
KERNEL := $(KERNEL_DIR)/$(CFG_HI_KERNEL_VERSION)

## file(s) to be removed
REMOVED_FILES := "*.o" "*.ko" "*.order" "*.symvers" "*.mod" "*.mod.*" "*.cmd" ".tmp_versions" "*.d" "modules.builtin"

## header search path

EXTRA_CFLAGS += -I$(COMMON_DIR)/include \
                -I$(COMMON_DIR)/api/include \
                -I$(COMMON_DIR)/drv/include \
                -I$(COMMON_DIR)/api/log \
                -I$(COMMON_DIR)/api/stat \
                -I$(COMMON_DIR)/api/sys \
                -I$(COMMON_DIR)/api/mmz \
                -I$(COMMON_DIR)/drv/module \
                -I$(COMMON_DIR)/drv/mem \
                -I$(COMMON_DIR)/drv/log \
                -I$(COMMON_DIR)/drv/mmz \
                -I$(COMMON_DIR)/api/mem \
                -I$(COMMON_DIR)/drv/sys \
                -I$(MSP_DIR)/drv/include \
                -I$(MSP_DIR)/api/include \
                -I$(MSP_DIR)/api/tde/include \
                -I$(MSP_DIR)/include \
                -I$(KERNEL) \
                -I$(srctree)/bounds_checking_function/include \

EXTRA_CFLAGS +=-Wno-error=date-time
KBUILD_EXTRA_SYMBOLS += $(MSP_DIR)/drv/securec/Module.symvers

ifeq ($(CFG_HI_TVP_SUPPORT),y)
EXTRA_CFLAGS += -I$(LINUX_DIR)/drivers/hisilicon/tee/tee_hisi/tzdriver/
endif

ifdef CFG_HI_LOG_LEVEL
EXTRA_CFLAGS += -DCFG_HI_LOG_LEVEL=$(CFG_HI_LOG_LEVEL)
else
EXTRA_CFLAGS += -DCFG_HI_LOG_LEVEL=1
endif

ifeq (y,$(CFG_HI_LOG_NETWORK_SUPPORT))
EXTRA_CFLAGS += -DLOG_NETWORK_SUPPORT
endif

ifeq (y,$(CFG_HI_UNIFIED_IMAGE_SUPPORT))
EXTRA_CFLAGS += -DHI_UNIFIED_IMAGE_SUPPORT
endif

ifeq (y,$(CFG_HI_LOG_UDISK_SUPPORT))
EXTRA_CFLAGS += -DLOG_UDISK_SUPPORT
endif

ifeq (y,$(CFG_HI_MEMMGR_SUPPORT))
EXTRA_CFLAGS += -DCMN_MMGR_SUPPORT
endif

ifeq (y,$(CFG_HI_CLOSE_BOOT_PRINT_AND_UART0))
EXTRA_CFLAGS += -DCONFIG_CLOSE_UART0
endif

ifeq (y,$(CFG_HI_CIPLUS_SUPPORT))
EXTRA_CFLAGS += -DCONFIG_CIPLUS_SUPPORT
endif

ifeq ($(TARGET_BUILD_VARIANT),user)
EXTRA_CFLAGS += -DANDROID_BUILD_USER
endif

ifeq (y,$(CFG_HI_ITRUSTEE_SUPPORT))
EXTRA_CFLAGS += -DCFG_HI_TEE_ITRUSTEE_SUPPORT
endif

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

MOD_NAME_CMN   := hi_common
MOD_NAME_MMZ   := hi_mmz
MOD_NAME_MEDIA := hi_media

$(MOD_NAME_CMN)-y := sys/drv_base_ext_k.o  \
               sys/drv_sys_ext_k.o         \
               sys/drv_sys_ext_spread.o    \
               stat/drv_stat_ext_k.o       \
               log/drv_log_ext_k.o

$(MOD_NAME_CMN)-y +=    sys/drv_base_ext.o          \
                        sys/drv_sys_ext.o           \
                        sys/drv_reg_common.o        \
                        sys/drv_sys_policy.o      \
                        sys/drv_sys_capability.o      \
                        stat/drv_stat_ext.o         \
                        mem/drv_mutils.o            \
                        module/drv_module_ext.o     \
                        log/drv_log_ext.o           \
                        userproc/drv_userproc.o     \
                        osal/osal_debug.o            \
                        osal/osal_atomic.o           \
                        osal/osal_semaphore.o        \
                        osal/osal_mutex.o            \
                        osal/osal_spinlock.o         \
                        osal/osal_wait.o         \
                        osal/osal_workqueue.o         \
                        osal/osal_fence.o         \
                        osal/osal_interrupt.o         \
                        osal/osal_task.o         \
                        osal/osal_addr.o         \
                        osal/osal_cache.o         \
                        osal/osal_barrier.o         \
                        osal/osal_string.o         \
                        osal/osal_timer.o         \
                        osal/osal_proc.o         \
                        osal/osal_fileops.o         \
                        osal/osal_device.o         \
                        osal/osal_math.o         \
                        osal/osal_gpio.o         \
                        osal/osal_exportfunc.o

$(MOD_NAME_MEDIA)-y := himedia/himedia_base.o  \
                       himedia/himedia.o

ifeq ($(CFG_HI_MEMMGR_SUPPORT),y)
$(MOD_NAME_CMN)-y += mem/drv_mmgr_ext.o mem/drv_mem_ext.o
endif

ifeq (y,$(CFG_HI_TEST_SUPPORT))
EXTRA_CFLAGS += -DCMN_TEST_SUPPORTED
$(MOD_NAME_CMN)-y += test/drv_test_ext.o
endif


ifeq ($(KERNEL_VERSION), linux-5.10)
$(MOD_NAME_MMZ)-y := mmz/drv_mmz_ext_k.o  \
                    mmz/drv_mmz_intf.o	\
					mmz/drv_mmz_userdev.o \
					mmz/drv_media_mem.o
else
$(MOD_NAME_MMZ)-y := mmz/drv_mmz_ext_k.o  \
                    mmz/drv_mmz_intf.o	\
					mmz/drv_tzsmmu.o \
					mmz/drv_mmz_userdev.o \
					mmz/drv_media_mem.o
endif

ifdef CONFIG_COMPAT
$(MOD_NAME_MMZ)-y +=mmz/drv_mmz_compat.o
endif

DEPENDS := $(patsubst %.o, %.d, $($(MOD_NAME_MEDIA)-y))
DEPENDS += $(patsubst %.o, %.d, $($(MOD_NAME_MMZ)-y))
DEPENDS += $(patsubst %.o, %.d, $($(MOD_NAME_CMN)-y))

ifeq ($(CFG_HI_TVP_SUPPORT),y)
$(MOD_NAME_MMZ)-y +=	mmz/drv_tee_smmu_agent.o
endif
###############################################################################
obj-$(CFG_MSP_BUILDTYPE) += $(MOD_NAME_MEDIA).o $(MOD_NAME_MMZ).o $(MOD_NAME_CMN).o

ifeq ($(CFG_HI_USER_DRV),y)
$(MOD_NAME_CMN)-y :=    osal/osal_debug.o            \
                        osal/osal_atomic.o           \
                        osal/osal_semaphore.o        \
                        osal/osal_mutex.o            \
                        osal/osal_spinlock.o         \
                        osal/osal_wait.o         \
                        osal/osal_workqueue.o         \
                        osal/osal_fence.o         \
                        osal/osal_interrupt.o         \
                        osal/osal_task.o         \
                        osal/osal_addr.o         \
                        osal/osal_cache.o         \
                        osal/osal_barrier.o         \
                        osal/osal_string.o         \
                        osal/osal_timer.o         \
                        osal/osal_proc.o         \
                        osal/osal_fileops.o         \
                        osal/osal_device.o         \
                        osal/osal_math.o         \
                        osal/osal_gpio.o         \
                        osal/osal_exportfunc.o

DEPENDS := $(patsubst %.o, %.d, $($(MOD_NAME_MEDIA)-y))
DEPENDS += $(patsubst %.o, %.d, $($(MOD_NAME_CMN)-y))
endif

###############################################################################

###############################################################################
#   rules
###############################################################################
.PHONY: all clean install uninstall

all:
	@make ARCH=$(CFG_HI_CPU_ARCH) CROSS_COMPILE=$(CFG_HI_TOOLCHAINS_NAME)- -C $(KERNEL)  M=`pwd` modules
	@$(CFG_HI_TOOLCHAINS_NAME)-strip --strip-unneeded $(MOD_NAME_CMN).ko
	@echo "make $@  $(obj) over !"

clean:
	@$(foreach file,$(REMOVED_FILES),\
               `find ./ -name $(file) | xargs rm -rf`)
	@echo "make $@  $(obj) over !"

install: all
	@cp -f $(MOD_NAME_CMN).ko $(MODULE_DIR)/
	@cp -f $(MOD_NAME_MMZ).ko $(MODULE_DIR)/
	@cp -f $(MOD_NAME_MEDIA).ko $(MODULE_DIR)/

uninstall:
	@rm -f $(MODULE_DIR)/$(MOD_NAME_CMN).ko
	@rm -f $(MODULE_DIR)/$(MOD_NAME_MMZ).ko
	@rm -f $(MODULE_DIR)/$(MOD_NAME_MEDIA).ko

$(DEPENDS):%.d:%.c
	@set -e; rm -f $(COMMON_DIR)/drv/$@; \
	$(CFG_HI_TOOLCHAINS_NAME)-gcc $(CFG_HI_SEC_COMPILE_CFLAGS) -MM $(EXTRA_CFLAGS) $< > $(COMMON_DIR)/drv/$@; \
	sed -i 's,\($(notdir $*)\)\.o[ :]*,$(dir $@)\1.o $@ : ,g' $(COMMON_DIR)/drv/$@

-include $(DEPENDS)
