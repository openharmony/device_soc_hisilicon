ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

TARGET := ot_user
#*************************************************************************
# all source file in this module
SRCS := ot_user.c

#*************************************************************************
DRV_CFLAGS += -fstack-protector-strong
EXTRA_CFLAGS += $(DRV_CFLAGS)
EXTRA_CFLAGS += $(MKP_CFLAGS)
EXTRA_CFLAGS += -I./
EXTRA_CFLAGS += -I$(REL_INC)

ifeq ($(OSTYPE),liteos)
EXTRA_CFLAGS += $(CFLAGS)
endif

.PHONY: all clean
all: $(OSTYPE)_build
clean: $(OSTYPE)_clean

#############################################
$(TARGET)-obj = $(SRCS:%.c=%.o)
ifneq ($(KERNELRELEASE),)
    obj-m += ot_user.o
endif

linux_build:
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- OT_ARCH=$(OT_ARCH), OT_CHIP=$(OT_CHIP), CVER=$(CVER), DBG=$(OT_DBG), OT_FPGA=$(OT_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE)
	@echo ---- DRV_CFLAGS=$(DRV_CFLAGS)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo ---- KERNEL_ROOT=$(KERNEL_ROOT)
	@@echo -e "\e[0m"
	@mkdir -p $(REL_INC)
	@$(MAKE) -C $(KERNEL_ROOT) $(LLVM_KERNEL_TOOLCHAIN_ARGS) M=$(PWD) modules
	@mkdir -p $(REL_KO) && cp $(TARGET).ko $(REL_KO)

linux_clean:
	@rm -f *.o *.ko *.mod.c
	@rm -f *.symvers *.order
	@rm -rf .*.ko.cmd .*.o.cmd .tmp_versions
	@rm -f $(REL_KO)/$(TARGET).ko

#============================================

#############################################
OBJS = $(SRCS:%.c=%.o)
LIB := lib$(TARGET).a

liteos_build: $(OBJS) $(LIB)
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'...\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- OT_ARCH=$(OT_ARCH), OT_CHIP=$(OT_CHIP), CVER=$(CVER), DBG=$(OT_DBG), OT_FPGA=$(OT_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE)
	@echo ---- MPP_CFLAGS=$(MPP_CFLAGS)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@@echo -e "\e[0m"
	@cp -rf $(LIB) $(REL_KO)/

liteos_clean:
	@rm -f $(OBJS) $(LIB)
	@rm -f $(REL_KO)/$(LIB)

$(OBJS): %.o : %.c
	@$(CC) $(EXTRA_CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	@$(AR) $(ARFLAGS) $@ $(OBJS)
#============================================
