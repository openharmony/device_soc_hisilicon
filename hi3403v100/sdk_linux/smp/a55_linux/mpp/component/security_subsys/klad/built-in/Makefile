KLAD_BASE_DIR := drivers/vendor/mpp/klad/src
OSTYPE=linux

ifneq (, $(filter $(CONFIG_ARCH_SS528V100) $(CONFIG_ARCH_SS625V100), y))
INTER_DRV := ss528v100
else ifneq (, $(filter $(CONFIG_ARCH_SS524V100) $(CONFIG_ARCH_SS522V100) \
	$(CONFIG_ARCH_SS624V100), y))
INTER_DRV := ss524v100
else ifneq (, $(filter $(CONFIG_ARCH_SS928V100) $(CONFIG_ARCH_SS000V100), y))
INTER_DRV := ss928v100
else
$(error unsupported otp module)
endif

# Add objs
include $(KLAD_BASE_DIR)/mkp/build.mak
include $(KLAD_BASE_DIR)/osal/build.mak
include $(KLAD_BASE_DIR)/arch/build.mak

EXTRA_CFLAGS += -Idrivers/vendor/mpp/include
EXTRA_CFLAGS += -I$(KLAD_BASE_DIR)/include
EXTRA_CFLAGS += -I$(KLAD_BASE_DIR)/../include
EXTRA_CFLAGS += -I$(KLAD_BASE_DIR)/../ext_inc
EXTRA_CFLAGS += $(KLAD_DRV_CFLAGS)

EXTRA_CFLAGS += -DOT_KLAD_DEBUG=0

obj-y += $(KLAD_DRV_OBJS)