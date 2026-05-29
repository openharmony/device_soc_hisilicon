
CIPHER_DRV_CFLAGS += -I$(CIPHER_DIR)/drv/drivers/core/include
CIPHER_DRV_CFLAGS += -I$(CIPHER_DIR)/drv/drivers/crypto/include
CIPHER_DRV_CFLAGS += -I$(CIPHER_DIR)/drv/drivers/extend/include
CIPHER_DRV_CFLAGS += -I$(CIPHER_DIR)/../include

CIPHER_DRV_OBJS +=  drv/drivers/core/drv_symc_v300.o          \
                    drv/drivers/core/drv_hash_v300.o          \
                    drv/drivers/core/drv_pke_v200.o           \
                    drv/drivers/core/drv_trng_v200.o          \
                    drv/drivers/core/drv_lib.o

CIPHER_DRV_OBJS += drv/drivers/crypto/cryp_symc.o             \
                   drv/drivers/crypto/cryp_hash.o             \
                   drv/drivers/crypto/cryp_trng.o             \
                   drv/drivers/crypto/cryp_rsa.o              \
                   drv/drivers/crypto/cryp_sm2.o

CIPHER_DRV_OBJS += drv/drivers/kapi_symc.o                    \
                   drv/drivers/kapi_hash.o                    \
                   drv/drivers/kapi_rsa.o                     \
                   drv/drivers/kapi_trng.o                    \
                   drv/drivers/kapi_sm2.o                     \
                   drv/drivers/ot_drv_cipher.o                \
                   drv/drivers/kapi_dispatch.o

CIPHER_DRV_OBJS += drv/drivers/extend/ext_hash.o              \
                   drv/drivers/extend/ext_sm2.o               \
                   drv/drivers/extend/ext_sm3.o

CIPHER_DRV_OBJS += drv/drivers/common_check_param.o