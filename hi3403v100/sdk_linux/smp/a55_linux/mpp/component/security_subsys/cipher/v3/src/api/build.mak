CIPHER_API_OBJS += $(patsubst %.c,%.o, $(wildcard api/*.c))
CIPHER_API_CFLAGS += -I$(CIPHER_BASE_DIR)/api/
