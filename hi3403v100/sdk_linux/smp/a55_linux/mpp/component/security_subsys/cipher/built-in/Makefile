OSTYPE=linux

ifneq (, $(filter $(CONFIG_ARCH_SS528V100) $(CONFIG_ARCH_SS524V100), y))
CIPHER_BASE_DIR := drivers/vendor/mpp/cipher/v2/src
PREFIX_STR := v2/src
else
CIPHER_BASE_DIR := drivers/vendor/mpp/cipher/v3/src
PREFIX_STR := v3/src
endif

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
include drivers/vendor/mpp/cipher/mbedtls/src/build.mak
include $(CIPHER_BASE_DIR)/api/build.mak
include $(CIPHER_BASE_DIR)/drv/build.mak

EXTRA_CFLAGS += $(CIPHER_API_CFLAGS)
EXTRA_CFLAGS += $(CIPHER_DRV_CFLAGS)
EXTRA_CFLAGS += -Idrivers/vendor/mpp/include
EXTRA_CFLAGS += -Idrivers/vendor/mpp
EXTRA_CFLAGS += -Idrivers/vendor/mpp/ext_inc
EXTRA_CFLAGS += -Idrivers/vendor/mpp/cipher/ext_inc
EXTRA_CFLAGS += $(MBEDTLS_CFLAGS)

obj-y += $(patsubst %.o, $(PREFIX_STR)/%.o, $(CIPHER_API_OBJS))
obj-y += $(patsubst %.o, $(PREFIX_STR)/%.o, $(CIPHER_DRV_OBJS))
obj-y += $(patsubst %.o, mbedtls/src/%.o, $(MBEDTLS_CRYPTO_OBJS))