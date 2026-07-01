/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef DRV_CIPHER_IOCTL_H
#define DRV_CIPHER_IOCTL_H

#include <asm-generic/ioctl.h>
#include "ot_type.h"
#include "ot_common.h"
#include "ot_common_cipher.h"
#include "mkp_ioctl.h"
#include "drv_cipher_define.h"
#include "osal_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifndef SM2_LEN_IN_WROD
#define SM2_LEN_IN_WROD                 8
#define SM2_LEN_IN_BYTE                 32
#endif

#define crypto_unused(x)    ((x) = (x))

#define crypto_max(a, b) ((a) > (b) ? (a) : (b))

#define crypto_min(a, b) ((a) < (b) ? (a) : (b))

#define CRYPTO_MAGIC_NUM          0xc0704d19

/* AES KEY size */
#define SYMC_KEY_SIZE       32

/* SM1 SK size */
#define SYMC_SM1_SK_SIZE    16

/* AES IV size */
#define AES_IV_SIZE         16

/* AES BLOCK size */
#define AES_BLOCK_SIZE      16

/* DES IV size */
#define DES_IV_SIZE         8

/* aead tag length */
#define AEAD_TAG_SIZE                  16
#define AEAD_TAG_SIZE_IN_WORD          4

/* bits in a byte */
#define BITS_IN_BYTE                   8

/* hash result max size */
#define HASH_RESULT_MAX_SIZE           64

/* hash result max size in word, used for hash state value address. */
#define HASH_RESULT_MAX_SIZE_IN_WORD   16

/*! capacity upport */
#define CRYPTO_CAPACITY_SUPPORT        1
#define CRYPTO_CAPACITY_NONSUPPORT     0

/* max length of SM2 ID */
#define SM2_ID_MAX_LEN                 0x1FFF

#define SM2_ENCRYPT_PAD_LEN  (SM2_LEN_IN_BYTE * 3)

/* Define the time out */
#define CRYPTO_TIME_OUT                6000
#define MS_TO_US                       1000

/* result size */
#define SHA1_RESULT_SIZE           20
#define SHA224_RESULT_SIZE         28
#define SHA256_RESULT_SIZE         32
#define SHA384_RESULT_SIZE         48
#define SHA512_RESULT_SIZE         64
#define SM3_RESULT_SIZE            32
/* rsa key length */
#define RSA_MIN_KEY_LEN           256
#define RSA_MAX_KEY_LEN           512
#define RSA_KEY_BITWIDTH_1024     128
#define RSA_KEY_BITWIDTH_2048     256
#define RSA_KEY_BITWIDTH_3072     384
#define RSA_KEY_BITWIDTH_4096     512
#define RSA_KEY_EXPONENT_VALUE1   0X3
#define RSA_KEY_EXPONENT_VALUE2   0X10001

/* the source of hash message */
typedef enum {
    HASH_CHUNCK_SRC_LOCAL,     /* Local buffer, e.g. Kernel  */
    HASH_CHUNCK_SRC_USER,      /* User buffer, use copy_from_user to read data */
} hash_chunk_src;

/* union of compat addr */
typedef union {
    td_void *p;                   /* virtual address */
    const td_void *cp;            /* const virtual address */
    td_phys_addr_t phy;           /* physical address */
    unsigned int word[MUL_VAL_2]; /* double word of address */
} compat_addr;

#define addr_h32(addr)          (addr).word[1]  /* High 32 bit of td_u64 */
#define addr_l32(addr)          (addr).word[0]  /* Low 32 bit of td_u64 */
#define addr_u64(addr)          (addr).phy      /* 64 bit of td_u64 */
#define addr_via(addr)          (addr).p        /* buffer point */
#define addr_via_const(addr)    (addr).cp       /* const buffer point */

/* struct of Symmetric cipher create */
typedef struct {
    td_u32 id;              /* to store the id of soft channel */
    ot_cipher_type type;    /* symc channel type */
} symc_create_t;

/* struct of Symmetric cipher destroy */
typedef struct {
    td_u32 id;              /* id of soft channel */
    td_u32 reserve;         /* reserve to make align at 64bit */
} symc_destroy_t;

/* struct of Symmetric cipher configure information */
typedef struct {
    td_u32 id;                           /* Id of soft channel */
    ot_cipher_alg alg;                   /* Symmetric cipher algorithm */
    ot_cipher_work_mode mode;            /* Symmetric cipher algorithm */
    ot_cipher_bit_width width;           /* Symmetric cipher bit width */
    ot_cipher_key_len klen;              /* Symmetric cipher key length */
    td_u8 iv[AES_IV_SIZE];               /* IV buffer */
    td_u32 ivlen;                        /* IV length */
    td_u32 iv_usage;                     /* Usage of IV */
    td_u32 reserve;                      /* reserve to make align at 64bit */
    compat_addr aad;                     /* Associated Data */
    td_u32 alen;                         /* Associated Data Length */
    td_u32 tlen;                         /* Tag length */
} symc_cfg_t;

typedef enum {
    SYMC_OPERATION_ENCRYPT = 0,
    SYMC_OPERATION_DECRYPT = 1,
    SYMC_OPERATION_ENCRYPT_VIA = 0x10,
    SYMC_OPERATION_DECRYPT_VIA = 0x11,
} symc_operation_type;

/* struct of Symmetric cipher encrypt/decrypt */
typedef struct {
    td_u32 id;                 /* Id of soft channel */
    td_u32 length;             /* Length of the encrypted data */
    td_u32 operation;          /* operation type */
    td_u32 last;               /* last or not */
    compat_addr input;         /* Physical address of the input data */
    compat_addr output;        /* Physical address of the output data */
} symc_encrypt_t;

/* struct of Symmetric cipher get tag */
typedef struct {
    td_u32 id;                            /* Id of soft channel */
    td_u32 tag[AEAD_TAG_SIZE_IN_WORD];    /* Buffer of tag */
    td_u32 taglen;                        /* Length of tag */
} aead_tag_t;

/* struct of Symmetric cipher get ctrl */
typedef struct {
    td_u32 id;                            /* Id of soft channel */
    ot_cipher_ctrl ctrl;                  /* control information */
} symc_get_cfg_t;

/* struct of Hash start */
typedef struct {
    td_u32 id;                            /* Id of soft channel */
    ot_cipher_hash_type type;             /* HASH type */
    compat_addr key;                      /* HMAC key */
    td_u32 keylen;                        /* HMAC key */
    td_u32 reserve;                       /* reserve for align at 64bit */
} hash_start_t;

/* struct of Hash update */
typedef struct {
    td_u32 id;             /* Id of soft channel */
    td_u32 length;         /* Length of the message */
    compat_addr input;     /* Message data buffer */
    hash_chunk_src src;    /* source of hash message */
    td_u32 reserve;        /* reserve for align at 64bit */
} hash_update_t;

/* struct of Hash update */
typedef struct {
    td_u32 id;             /* Id of soft channel */
    td_u32 hash[HASH_RESULT_MAX_SIZE_IN_WORD]; /* buffer holding the hash data */
    td_u32 hashlen;        /* length of the hash data */
    td_u32 reserve;        /* reserve for align at 64bit */
} hash_finish_t;

/* struct of rsa encrypt/decrypt */
typedef struct {
    td_u32 scheme;             /* RSA scheme */
    td_u16 public;             /* Type of key, true-public or false-private */
    td_u16 reserve;
    td_u32 klen;               /* length of rsa key */
    td_u32 e;                  /* The public exponent */
    compat_addr d;             /* The private exponent */
    compat_addr n;             /* The modulus */
    compat_addr p;             /* The p factor of N */
    compat_addr q;             /* The q factor of N */
    compat_addr qp;            /* The 1/q mod p CRT param */
    compat_addr dp;            /* The d mod (p - 1) CRT param */
    compat_addr dq;            /* The d mod (q - 1) CRT param */
    compat_addr in;            /* input data to be encryption */
    compat_addr out;           /* output data of encryption */
    td_u32 inlen;              /* length of input data to be encryption */
    td_u32 outlen;             /* length of output data */
} rsa_info_t;

/* RSA PKCS style key */
typedef struct {
    td_u8 public;              /* Type of key, true-public or false-private */
    td_u8 reserve;
    td_u16 klen;               /* The key length */
    td_u32 e;                  /* The public exponent */
    td_u8 *d;                  /* The private exponent */
    td_u8 *n;                  /* The modulus */
    td_u8 *p;                  /* The p factor of n */
    td_u8 *q;                  /* The q factor of n */
    td_u8 *qp;                 /* The 1/q mod p CRT param */
    td_u8 *dp;                 /* The d mod (p - 1) CRT param */
    td_u8 *dq;                 /* The d mod (q - 1) CRT param */
    td_u32 bufsize;            /* The buffer size alloc for n */
} cryp_rsa_key;

/* struct of rsa crypt input and output data. */
typedef struct {
    td_u8 *in;
    td_u32 in_len;
    td_u8 *out;
    td_u32 out_len;
} cryp_rsa_crypto_data;

/* Rsa encrypt and decrypt scheme. */
typedef enum {
    OT_CIPHER_RSA_ENCRYPT_SCHEME_NO_PADDING  = 0x00, /* without padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_0,       /* PKCS#1 block type 0 padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_1,       /* PKCS#1 block type 1padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_BLOCK_TYPE_2,       /* PKCS#1 block type 2 padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA1,    /* PKCS#1 RSAES-OAEP-SHA1 padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA224,  /* PKCS#1 RSAES-OAEP-SHA224 padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA256,  /* PKCS#1 RSAES-OAEP-SHA256   padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA384,  /* PKCS#1 RSAES-OAEP-SHA384   padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_OAEP_SHA512,  /* PKCS#1 RSAES-OAEP-SHA512   padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_RSAES_PKCS1_V1_5,   /* PKCS#1 RSAES-PKCS1_V1_5    padding. */
    OT_CIPHER_RSA_ENCRYPT_SCHEME_BUTT,
} ot_cipher_rsa_encrypt_scheme;

typedef enum {
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1 = 0x100, /* PKCS#1 RSASSA_PKCS1_V15_SHA1 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA224,       /* PKCS#1 RSASSA_PKCS1_V15_SHA224 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256,       /* PKCS#1 RSASSA_PKCS1_V15_SHA256 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA384,       /* PKCS#1 RSASSA_PKCS1_V15_SHA384 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA512,       /* PKCS#1 RSASSA_PKCS1_V15_SHA512 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA1,         /* PKCS#1 RSASSA_PKCS1_PSS_SHA1 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA224,       /* PKCS#1 RSASSA_PKCS1_PSS_SHA224 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA256,       /* PKCS#1 RSASSA_PKCS1_PSS_SHA256 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA384,       /* PKCS#1 RSASSA_PKCS1_PSS_SHA1 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_PSS_SHA512,       /* PKCS#1 RSASSA_PKCS1_PSS_SHA256 signature. */
    OT_CIPHER_RSA_SIGN_SCHEME_BUTT,
} ot_cipher_rsa_sign_scheme;

/* struct of rsa sign or verify data. */
typedef struct {
    td_u8 *in;
    td_u32 in_len;
    td_u8 *out;
    td_u32 out_len;
} cryp_rsa_sign_data;

/* struct of trng */
typedef struct {
    td_u32 randnum;            /* randnum rand number */
    td_u32 timeout;            /* time out  */
} trng_t;

/*! \struct of Symmetric cipher get key slot handle */
typedef struct {
    td_u32 cipher;             /*!< cipher channel */
    td_u32 keyslot;            /*!< keyslot channel */
} symc_keyslot_t;

typedef struct {
    td_u8 *x;
    td_u8 *y;
    td_u32 len;
} ecc_point_t;

/*! \struct of ecc  */
typedef struct {
    /*!<  Finite field: equal to p in case of prime field curves or equal to 2^n in case of binary field curves. */
    td_u8 *p;
    td_u8 *a;       /*!<  Curve parameter a (q-3 in Suite B). */
    td_u8 *b;       /*!<  Curve parameter b */
    td_u8 *n;       /*!<  Prime which is the order of G point. */
    /*!<  Cofactor, which is the order of the elliptic curve divided by the order of the point G.
        For the Suite B curves, h = 1.
    */
    ecc_point_t g;      /*!<  X coordinates of G which is a base point on the curve. */
    td_u32 h;
    td_u32 ksize;   /*!<  Key size in bytes. It corresponds to the size in bytes of the prime ptd_u8n */
} ecc_param_t;

/*! \struct of sm2 sign */
typedef struct {
    td_u32 d[SM2_LEN_IN_WROD];    /* sm2 private key */
    td_u32 px[SM2_LEN_IN_WROD];   /* sm2 x public key */
    td_u32 py[SM2_LEN_IN_WROD];   /* sm2 y public key */
    const td_u8 *id;              /* sm2 user id */
    td_u32 id_len;                /* length of sm2 user id */
    const td_u8 *msg;             /* message to be sign */
    td_u32 msglen;                /* length of message to be sign */
    td_u32 r[SM2_LEN_IN_WROD];    /* sm2 sign result of r */
    td_u32 s[SM2_LEN_IN_WROD];    /* sm2 sign result of s */
} sm2_sign_t;

/*! \struct of sm2 verify */
typedef struct {
    td_u32 px[SM2_LEN_IN_WROD];   /* sm2 x public key */
    td_u32 py[SM2_LEN_IN_WROD];   /* sm2 y public key */
    const td_u8 *id;              /* sm2 user id */
    td_u32 id_len;                /* length of sm2 user id */
    const td_u8 *msg;             /* message to be sign */
    td_u32 msglen;                /* length of message to be sign */
    td_u32 r[SM2_LEN_IN_WROD];    /* sm2 sign result of r */
    td_u32 s[SM2_LEN_IN_WROD];    /* sm2 sign result of s */
} sm2_verify_t;

/*! \struct of sm2 encrypt */
typedef struct {
    ot_cipher_sm2_public_key pub_key;
    const td_u8 *plain_data;
    td_u32 plain_len;
    td_u8 *cipher_data;
    td_u32 cipher_len;
} sm2_encrypt_t;

/*! \struct of sm2 decrypt */
typedef struct {
    ot_cipher_sm2_private_key pri_key;
    const td_u8 *cipher_data;
    td_u32 cipher_len;
    td_u8 *plain_data;
    td_u32 plain_len;
} sm2_decrypt_t;

typedef struct {
    ot_keyslot_attr attr;
    td_handle keyslot;
} keyslot_create_t;

typedef struct {
    td_handle keyslot;
} keyslot_destroy_t;

typedef enum {
    CIPHER_IOC_NR_SYMC_CREATE,
    CIPHER_IOC_NR_SYMC_DESTROY,
    CIPHER_IOC_NR_SYMC_SET_CFG,
    CIPHER_IOC_NR_SYMC_GET_CFG,
    CIPHER_IOC_NR_SYMC_CRYPTO,
    CIPHER_IOC_NR_SYMC_GET_TAG,
    CIPHER_IOC_NR_HASH_START,
    CIPHER_IOC_NR_HASH_UPDATE,
    CIPHER_IOC_NR_HASH_FINISH,
    CIPHER_IOC_NR_TRNG,
    CIPHER_IOC_NR_RSA_ENCRYPT,
    CIPHER_IOC_NR_RSA_DECRYPT,
    CIPHER_IOC_NR_RSA_SIGN,
    CIPHER_IOC_NR_RSA_VERIFY,
    CIPHER_IOC_NR_SM2_ENCRYPT,
    CIPHER_IOC_NR_SM2_DECRYPT,
    CIPHER_IOC_NR_SM2_SIGN,
    CIPHER_IOC_NR_SM2_VERIFY,
    CIPHER_IOC_NR_KEYSLOT_CREATE,
    CIPHER_IOC_NR_KEYSLOT_DESTROY,
    CIPHER_IOC_NR_SYMC_ATTACH,
    CIPHER_IOC_NR_SYMC_DETACH,
    CIPHER_IOC_NR_BUTT,
} cipher_ioc_nr;

/* Ioctl definitions */
#define CRYPTO_CMD_SYMC_CREATE        OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_SYMC_CREATE, symc_create_t)
#define CRYPTO_CMD_SYMC_DESTROY       OSAL_IOW (IOC_TYPE_CIPHER, CIPHER_IOC_NR_SYMC_DESTROY, symc_destroy_t)
#define CRYPTO_CMD_SYMC_SET_CFG       OSAL_IOW (IOC_TYPE_CIPHER, CIPHER_IOC_NR_SYMC_SET_CFG, symc_cfg_t)
#define CRYPTO_CMD_SYMC_GET_CFG       OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_SYMC_GET_CFG, symc_get_cfg_t)
#define CRYPTO_CMD_SYMC_ENCRYPT       OSAL_IOW (IOC_TYPE_CIPHER, CIPHER_IOC_NR_SYMC_CRYPTO, symc_encrypt_t)
#define CRYPTO_CMD_SYMC_GET_TAG       OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_SYMC_GET_TAG, aead_tag_t)
#define CRYPTO_CMD_HASH_START         OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_HASH_START, hash_start_t)
#define CRYPTO_CMD_HASH_UPDATE        OSAL_IOW (IOC_TYPE_CIPHER, CIPHER_IOC_NR_HASH_UPDATE, hash_update_t)
#define CRYPTO_CMD_HASH_FINISH        OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_HASH_FINISH, hash_finish_t)
#define CRYPTO_CMD_TRNG               OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_TRNG, trng_t)
#define CRYPTO_CMD_RSA_ENCRYPT        OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_RSA_ENCRYPT, rsa_info_t)
#define CRYPTO_CMD_RSA_DECRYPT        OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_RSA_DECRYPT, rsa_info_t)
#define CRYPTO_CMD_RSA_SIGN           OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_RSA_SIGN, rsa_info_t)
#define CRYPTO_CMD_RSA_VERIFY         OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_RSA_VERIFY, rsa_info_t)
#define CRYPTO_CMD_SM2_ENCRYPT        OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_SM2_ENCRYPT, sm2_encrypt_t)
#define CRYPTO_CMD_SM2_DECRYPT        OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_SM2_DECRYPT, sm2_decrypt_t)
#define CRYPTO_CMD_SM2_SIGN           OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_SM2_SIGN, sm2_sign_t)
#define CRYPTO_CMD_SM2_VERIFY         OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_SM2_VERIFY, sm2_verify_t)
#define CRYPTO_CMD_KEYSLOT_CREATE     OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_KEYSLOT_CREATE, keyslot_create_t)
#define CRYPTO_CMD_KEYSLOT_DESTROY    OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_KEYSLOT_DESTROY, keyslot_destroy_t)
#define CRYPTO_CMD_SYMC_ATTACH        OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_SYMC_ATTACH, symc_keyslot_t)
#define CRYPTO_CMD_SYMC_DETACH        OSAL_IOWR(IOC_TYPE_CIPHER, CIPHER_IOC_NR_SYMC_DETACH, symc_keyslot_t)
#define CRYPTO_CMD_COUNT              CIPHER_IOC_NR_BUTT

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DRV_CIPHER_IOCTL_H */
