PATCH := $(wildcard $(MBEDTLS_DIR)/*.patch)
TAR_FILE := $(MBEDTLS_DIR)/$(MBEDTLS_VER).tar.gz
mbedtls_patch:
	@sh ./mbedtls.sh $(MBEDTLS_DIR) $(MBEDTLS_VER) PATCH $(PATCH)

mbedtls_clean:
	@sh ./mbedtls.sh $(MBEDTLS_DIR) $(MBEDTLS_VER) CLEAN
