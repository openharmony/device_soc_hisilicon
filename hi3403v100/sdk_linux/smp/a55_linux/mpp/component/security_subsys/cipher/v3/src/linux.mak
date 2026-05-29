OBJS := $(CIPHER_API_OBJS:%.o=$(BUILD_DIR)/%.o)

obj-m       := $(TARGET).o
$(TARGET)-y := $(CIPHER_DRV_OBJS) $(MBEDTLS_OBJS)

.PHONY: all clean mbedtls_patch mbedtls_clean

all: $(OBJS)
	@echo -e "\e[0;32;1m... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- TARGET=$(TARGET)
	@echo ---- BUILD_DIR=$(BUILD_DIR)
	@echo -e "\e[0m"
	@mkdir -p $(BUILD_DIR)
	@touch $(BUILD_DIR_MAKEFILE)
	@make -C $(KERNEL_ROOT) $(LLVM_KERNEL_TOOLCHAIN_ARGS) M=$(BUILD_DIR) src=$(CURDIR) O=$(KERNEL_ROOT) V=10 modules
	@mkdir -p $(REL_KO) && cp -rf $(BUILD_DIR)/$(TARGET).ko $(REL_KO)
	@mkdir -p $(REL_LIB) && $(CC) $(LIBS_LD_CFLAGS) -shared -fPIC -o $(REL_LIB)/libot_cipher.so $(OBJS)
	@mkdir -p $(REL_LIB) && $(AR) -rv $(REL_LIB)/libot_cipher.a $(OBJS)
	@mkdir -p $(REL_INC) && cp -rf $(CIPHER_DIR_INC) $(REL_INC)
#	@$(CROSS_COMPILE)objdump -D $(TARGET).ko > cipher.asm

clean:
	@rm -f *.asm
	@rm -f $(OBJS)
	@rm -f $(CIPHER_REL_INC)
	@rm -f $(REL_KO)/$(TARGET).ko
	@rm -f $(REL_LIB)/lib$(TARGET).*
	@mkdir -p $(BUILD_DIR)
	@rm -f $(BUILD_DIR)/$(MBEDTLS_REL)/$(MBEDTLS_VER)/library/*
	@make -C $(KERNEL_ROOT) M=$(BUILD_DIR) src=$(CURDIR) O=$(KERNEL_ROOT) clean

$(OBJS): $(BUILD_DIR)/%.o : %.c
	@echo $(CC) $< ...
	@mkdir -p $(BUILD_DIR)/api/
	@mkdir -p $(BUILD_DIR)/common/
	@$(CC) $(MPP_CFLAGS) $(LIBS_CFLAGS) -c $< -o $@

