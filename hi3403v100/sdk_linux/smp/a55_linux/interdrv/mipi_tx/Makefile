ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

TARGET := ot_mipi_tx
#*************************************************************************
# all source file in this module
SRCS := mipi_tx.c \
        mipi_tx_hal.c  \
        ../init/$(OSTYPE)/mipi_tx_init.c
#*************************************************************************
EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += $(MKP_CFLAGS)
EXTRA_CFLAGS += -I./
EXTRA_CFLAGS += -I$(REL_INC)
EXTRA_CFLAGS += -I$(OSAL_ROOT)/include
ifeq ($(MPP_BUILD), y)
EXTRA_CFLAGS += -I$(SDK_PATH)/interdrv/$(OT_ARCH)/init/$(OSTYPE)
else
EXTRA_CFLAGS += -I$(SDK_PATH)/interdrv/init/$(OSTYPE)
endif
EXTRA_CFLAGS += -D$(OT_FPGA)

ifeq ($(OSTYPE),liteos)
EXTRA_CFLAGS += $(CFLAGS)
endif

.PHONY: all clean dependence_get dependence_remove
all: $(OSTYPE)_build
clean: $(OSTYPE)_clean

#############################################
$(TARGET)-objs = $(SRCS:%.c=%.o)
ifneq ($(KERNELRELEASE),)
    obj-m += $(TARGET).o
endif

ifeq ($(MPP_BUILD), y)
    DEV_EXT_HEADER := dev_ext.h
    INC_DIR=$(MPP_PATH)/../interdrv/$(OT_ARCH)/mipi_tx
endif

dependence_get:
ifeq ($(MPP_BUILD), y)
	@rm -f $(INC_DIR)/$(DEV_EXT_HEADER)
	@cp $(MPP_PATH)/cbb/base/ext_inc/$(DEV_EXT_HEADER) $(INC_DIR)
endif

dependence_remove:
ifeq ($(MPP_BUILD), y)
	@rm -f $(INC_DIR)/$(DEV_EXT_HEADER)
endif

linux_build: dependence_get
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- OT_ARCH=$(OT_ARCH), OT_CHIP=$(OT_CHIP), CVER=$(CVER), DBG=$(OT_DBG), OT_FPGA=$(OT_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE)
	@echo ---- MPP_CFLAGS=$(MPP_CFLAGS)
	@echo "EXTRA_CFLAGS=$(EXTRA_CFLAGS)"
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo ---- KERNEL_ROOT=$(KERNEL_ROOT)
	@echo ---- ARCH_ROOT=$(ARCH_ROOT), ARCH_HAL=$(ARCH_HAL)
	@@echo -e "\e[0m"
	@mkdir -p $(REL_INC) && cp -f ot_mipi_tx.h $(REL_INC)/
	@$(MAKE) -C $(KERNEL_ROOT) $(LLVM_KERNEL_TOOLCHAIN_ARGS) M=$(PWD) modules
	@mkdir -p $(REL_KO) && cp $(TARGET).ko $(REL_KO)

linux_clean: dependence_remove
	@rm -f *.o *.ko *.mod.c
	@rm -f *.symvers *.order
	@rm -rf .*.ko.cmd .*.o.cmd .tmp_versions
	@rm -f $(REL_KO)/$(TARGET).ko
	@rm -f $(REL_INC)/ot_mipi_tx.h
	@rm -f ../init/linux/.mipi_tx_init.o.cmd
	@rm -f ../init/linux/mipi_tx_init.o

#############################################
OBJS = $(SRCS:%.c=%.o)
LIB := lib$(TARGET).a

liteos_build: $(OBJS) $(LIB) dependence_get
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'...\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- OT_ARCH=$(OT_ARCH), OT_CHIP=$(OT_CHIP), CVER=$(CVER), DBG=$(OT_DBG), OT_FPGA=$(OT_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE)
	@echo ---- MPP_CFLAGS=$(MPP_CFLAGS)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@@echo -e "\e[0m"
	@mkdir -p $(REL_KO) && cp -rf $(LIB) $(REL_KO)/
	@mkdir -p $(REL_INC) && cp -f ot_mipi_tx.h $(REL_INC)/

liteos_clean: dependence_remove
	@rm -f $(OBJS) $(LIB)
	@rm -f $(REL_KO)/$(LIB)

$(OBJS): %.o : %.c
	@$(CC) $(EXTRA_CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	@$(AR) $(ARFLAGS) $@ $(OBJS)
#============================================
