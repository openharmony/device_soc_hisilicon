CIPHER_DIR := $(CIPHER_BASE_DIR)

CIPHER_DRV_CFLAGS += -I$(CIPHER_DIR)/drv/arch/$(INTER_DRV)

include $(CIPHER_DIR)/drv/drivers/build.mak
include $(CIPHER_DIR)/drv/osal/build.mak
include $(CIPHER_DIR)/drv/compat/build.mak

CIPHER_DRV_CFLAGS += -I$(CIPHER_DIR)/drv/include
