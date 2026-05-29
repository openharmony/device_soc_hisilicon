OBJS      := $(KLAD_API_OBJS:%.o=$(BUILD_DIR)/%.o)

obj-m     := $(TARGET).o
$(TARGET)-y := $(KLAD_DRV_OBJS)

.PHONY: all clean build_env linux_build

all: build_env $(OSTYPE)_build

build_env:
	@echo -e "\e[0;32;1m... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- OT_ARCH=$(OT_ARCH), OT_CHIP=$(OT_CHIP), CVER=$(CVER), DBG=$(OT_DBG), OT_FPGA=$(OT_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE) OSTYPE=$(OSTYPE)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo ---- KERNEL_ROOT=$(KERNEL_ROOT)
	@echo ---- ARCH_ROOT=$(ARCH_ROOT), ARCH_HAL=$(ARCH_HAL)
	@echo -e "\e[0m"

linux_build: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	@touch $(BUILD_DIR_MAKEFILE)
	@make -C $(KERNEL_ROOT) $(LLVM_KERNEL_TOOLCHAIN_ARGS) M=$(BUILD_DIR) src=$(CURDIR) O=$(KERNEL_ROOT) V=10 modules
	@mkdir -p $(REL_LIB) && $(CC) $(LIBS_LD_CFLAGS) -shared -fPIC -o $(REL_LIB)/lib$(TARGET).so $(OBJS)
	@mkdir -p $(REL_LIB) && $(AR) -rv $(REL_LIB)/lib$(TARGET).a $(OBJS)
	@mkdir -p $(REL_KO)  && cp $(BUILD_DIR)/$(TARGET).ko $(REL_KO)
	@mkdir -p $(REL_INC) && cp $(KLAD_DIR_INC) $(REL_INC)

clean:
	@rm -f *.asm
	@rm -f $(OBJS)
	@rm -f $(KLAD_REL_INC)
	@rm -f $(REL_KO)/$(TARGET).ko
	@rm -f $(REL_LIB)/lib$(TARGET).*
	@mkdir -p $(BUILD_DIR)
	@make -C $(KERNEL_ROOT)  M=$(BUILD_DIR) src=$(CURDIR) O=$(KERNEL_ROOT) clean

$(OBJS): $(BUILD_DIR)/%.o : %.c
	@echo $(CC) $< ...
	@mkdir -p $(BUILD_DIR)/mpi
	@$(CC) $(MPP_CFLAGS) $(LIBS_CFLAGS) -c $< -o $@
