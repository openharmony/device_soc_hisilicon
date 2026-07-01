ifeq ($(PARAM_FILE), )
    PARAM_FILE:=../../mpp/Makefile.param
    include $(PARAM_FILE)
endif

TARGET := $(KO_PREFIX)_wdt
# /* ARCH_LIBNAME*/
#*************************************************************************
# all source file in this module
SRCS := ot_wtdg.c \
        ../init/$(OSTYPE)/wtdg_init.c

#*************************************************************************
EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += $(MKP_CFLAGS)
EXTRA_CFLAGS += -I$(PWD)
EXTRA_CFLAGS += -I$(OSAL_ROOT)/include
EXTRA_CFLAGS += -I$(PWD)/arch/$(OT_ARCH)

ifeq ($(MPP_BUILD), n)
EXTRA_CFLAGS += -I$(REL_INC)
endif

ifeq ($(OSTYPE),liteos)
EXTRA_CFLAGS += $(CFLAGS)
endif

.PHONY: all clean
all: $(OSTYPE)_build
clean: $(OSTYPE)_clean

#############################################
$(TARGET)-objs = $(SRCS:%.c=%.o)
ifneq ($(KERNELRELEASE),)
    obj-m += $(TARGET).o
endif

linux_build:
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- OT_ARCH=$(OT_ARCH), OT_CHIP=$(OT_CHIP), CVER=$(CVER), DBG=$(OT_DBG), OT_FPGA=$(OT_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE)
	@echo ---- MPP_CFLAGS=$(MPP_CFLAGS)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo ---- KERNEL_ROOT=$(KERNEL_ROOT)
	@echo ---- ARCH_ROOT=$(ARCH_ROOT), ARCH_HAL=$(ARCH_HAL)
	@@echo -e "\e[0m"
	@$(MAKE) -C  $(KERNEL_ROOT) $(LLVM_KERNEL_TOOLCHAIN_ARGS) M=$(PWD) modules
	@mkdir -p $(REL_INC) && cp -f ot_wtdg.h $(REL_INC)/
	@mkdir -p $(REL_KO) && cp $(TARGET).ko $(REL_KO)

linux_clean:
	@rm -f *.o *.ko *.mod.c
	@rm -f *.symvers *.order
	@rm -rf .*.ko.cmd .*.o.cmd .tmp_versions
	@rm -f $(REL_KO)/$(TARGET).ko
	@rm -f $(REL_INC)/ot_wtdg.h
	@for x in `find ../init/$(OSTYPE) -name "*wtdg_init.o.cmd"`;do rm -rf $$x;done
	@for x in `find ../init/$(OSTYPE) -name "*wtdg_init.o*"`;do rm -rf $$x;done
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
	@mkdir -p $(REL_KO) && cp -rf $(LIB) $(REL_KO)/
	@mkdir -p $(REL_INC) && cp -f ot_wtdg.h $(REL_INC)/

liteos_clean:
	@rm -f $(OBJS) $(LIB)
	@rm -f $(REL_KO)/$(LIB)
	@rm -f $(REL_INC)/ot_wtdg.h

$(OBJS): %.o : %.c
	@$(CC) $(EXTRA_CFLAGS) -c $< -o $@

$(LIB): $(OBJS) 
	@$(AR) $(ARFLAGS) $@ $(OBJS)
#============================================
