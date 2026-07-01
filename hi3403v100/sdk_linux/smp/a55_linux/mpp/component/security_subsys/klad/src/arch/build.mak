
include $(KLAD_BASE_DIR)/arch/$(INTER_DRV)/build.mak

KLAD_DRV_CFLAGS += -I$(KLAD_BASE_DIR)/arch/$(INTER_DRV)/
KLAD_DRV_CFLAGS += -I$(KLAD_BASE_DIR)/arch/hal/$(OT_KLAD_VERSION)
KLAD_DRV_CFLAGS += -I$(KLAD_BASE_DIR)/arch/hal/$(OT_KLAD_VERSION)/rkp/

KLAD_DRV_OBJS    += arch/hal/$(OT_KLAD_VERSION)/rkp/hal_rkp.o
KLAD_DRV_OBJS    += arch/hal/$(OT_KLAD_VERSION)/hal_klad.o

# lower to upper
KLAD_DRV_CFLAGS += -DOT_KLAD_$(shell echo $(OT_KLAD_VERSION) | tr a-z A-Z)
