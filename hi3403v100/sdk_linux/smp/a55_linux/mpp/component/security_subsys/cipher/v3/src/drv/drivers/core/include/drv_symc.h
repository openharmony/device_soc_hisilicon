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

#ifndef DRV_SYMC_H
#define DRV_SYMC_H

/* symmetric cipher max key size in words */
#define SYMC_KEY_MAX_SIZE_IN_WORD      8

/* symmetric cipher max iv size in word */
#define SYMC_IV_MAX_SIZE_IN_WORD       4

/* symmetric sm1 sk size in words */
#define SYMC_SM1_SK_SIZE_IN_WORD       4

/* DES BLOCK size */
#define DES_BLOCK_SIZE                 8

/* Numbers of nodes list */
#define SYMC_MAX_LIST_NUM              255

/* Length of CCM N */
#define SYMC_CCM_N_LEN                 16

/* Length of CCM A head */
#define SYMC_CCM_A_HEAD_LEN            16

/* Length of GCM CLEN */
#define SYMC_GCM_CLEN_LEN              16

/* Small length of CCM A  */
#define SYMC_CCM_A_SMALL_LEN           (0x10000 - 0x100)

/* AES KEY size 128bit */
#define AES_KEY_128BIT                 16

/* AES KEY size 192bit */
#define AES_KEY_192BIT                 24

/* AES KEY size 256bit */
#define AES_KEY_256BIT                 32

/* DES KEY size 128bit */
#define DES_KEY_SIZE                   8

/* TDES KEY size 128bit */
#define TDES_KEY_128BIT                16

/* TDES KEY size 192bit */
#define TDES_KEY_192BIT                24

/* DES block size */
#define DES_BLOCK_SIZE                 8

/* SM1 KEY size */
#define SM1_AK_EK_SIZE                 32
#define SM1_SK_SIZE                    16

/* SM4 KEY size */
#define SM4_KEY_SIZE                   16

/* symmetric cipher width */
typedef enum {
    SYMC_DAT_WIDTH_128 = 0,
    SYMC_DAT_WIDTH_8,
    SYMC_DAT_WIDTH_1,
    SYMC_DAT_WIDTH_64 = 3,
    SYMC_DAT_WIDTH_COUNT,
} symc_width;

/* symmetric cipher algorithm */
typedef enum {
    SYMC_ALG_DES,         /* Data Encryption Standard  */
    SYMC_ALG_TDES,        /* Triple Data Encryption Standard  */
    SYMC_ALG_AES,         /* Advanced Encryption Standard  */
    SYMC_ALG_SM4,         /* SM4 Algorithm  */
    SYMC_ALG_SM1,         /* SM1 Algorithm  */
    SYMC_ALG_NULL_CIPHER, /* null cipher, dma copy  */
    SYMC_ALG_COUNT,
} symc_alg;

/* symmetric cipher key length */
typedef enum {
    SYMC_KEY_DEFAULT = 0,       /* Default, aes-128, 3des-192, sm1-256, sm4-128 */
    SYMC_KEY_AES_192BIT,        /* AES 192 bit key */
    SYMC_KEY_AES_256BIT,        /* AES 256 bit key */
    SYMC_KEY_TDES_2KEY,         /* 3DES 128 bit key */
    SYMC_KEY_LEN_COUNT,
} symc_klen;

/* symmetric cipher mode */
typedef enum {
    SYMC_MODE_ECB = 0, /* Electronic Codebook Mode */
    SYMC_MODE_CBC,     /* Cipher Block Chaining */
    SYMC_MODE_CFB,     /* Cipher Feedback Mode */
    SYMC_MODE_OFB,     /* Output Feedback Mode */
    SYMC_MODE_CTR,     /* Counter Mode */
    SYMC_MODE_CCM,     /* Counter with Cipher Block Chaining-Message Authentication Code */
    SYMC_MODE_GCM,     /* Galois/Counter Mode */
    SYMC_MODE_CTS,     /* CTS Mode */
    SYMC_MODE_COUNT,
} symc_mode;

/* locational of buffer under symmetric cipher */
typedef enum {
    SYMC_NODE_USAGE_NORMAL      = 0x00, /* The normal buffer, don't update the iv */
    SYMC_NODE_USAGE_FIRST       = 0x01, /* The first buffer, the usage of iv is expired */
    SYMC_NODE_USAGE_LAST        = 0x02, /* The last buffer, must update the iv */
    SYMC_NODE_USAGE_ODD_KEY     = 0x40, /* Use the odd key to encrypt or decrypt this buffer */
    SYMC_NODE_USAGE_EVEN_KEY    = 0x00, /* Use the even key to encrypt or decrypt this buffer */
    SYMC_NODE_USAGE_IN_GCM_A    = 0x00, /* The buffer of GCM A */
    SYMC_NODE_USAGE_IN_GCM_P    = 0x08, /* The buffer of GCM P */
    SYMC_NODE_USAGE_IN_GCM_LEN  = 0x10, /* The buffer of GCM LEN */
    SYMC_NODE_USAGE_IN_CCM_N    = 0x00, /* The buffer of CCM N */
    SYMC_NODE_USAGE_IN_CCM_A    = 0x08, /* The buffer of CCM A */
    SYMC_NODE_USAGE_IN_CCM_P    = 0x10, /* The buffer of CCM P */
    SYMC_NODE_USAGE_CCM_LAST    = 0x20, /* The buffer of CCM LAST */
    SYMC_NODE_USAGE_GCM_FIRST   = 0x10000, /* The buffer of GCM FIRST */
} symc_node_usage;

typedef struct {
    td_u32 id;                     /* channel number */
    td_char *open;                 /* Open or closed */
    td_char *alg;                  /* algorithm */
    td_char *mode;                 /* work mode */
    td_u8 klen;                    /* Length of key */
    td_u8 ksrc;                    /* Hard or soft key */
    td_u8 decrypt;                 /* Decrypt or Encrypt */
    td_u32 inaddr;                 /* In DDR address */
    td_u32 outaddr;                /* Out DDR address */
    td_u8 int_status;              /* Status interrupt */
    td_u8 int_en;                  /* Enable interrupt */
    td_u8 int_raw;                 /* Raw interrupt */
    td_u8 secure;                  /* Secure channel or not */
    td_u8 smmu_enable;             /* SMMU Enable */
    td_u32 in_node_head;           /* In node list head */
    td_u8 in_node_depth;           /* In node depth */
    td_u8 in_node_rptr;            /* In node list read index */
    td_u8 in_node_wptr;            /* In node list write index */
    td_u32 out_node_head;          /* Out node list head */
    td_u8 out_node_depth;          /* Out node depth */
    td_u8 out_node_rptr;           /* Out node list read index */
    td_u8 out_node_wptr;           /* Out node list write index */
    td_u8 in_last_block;           /* first word of last input block */
    CRYPTO_OWNER owner;            /* Process PID of owner */
    td_u8 iv[AES_IV_SIZE * 2 + 1]; /* Out IV */
} symc_chn_status;

typedef struct {
    td_u8 sec_cpu;     /* secure CPU or not */
    td_u8 allow_reset; /* allow reset CPU or not */
    td_u32 int_num;    /* interrupt number */
    const char *name;  /* interrupt name */
    td_u8 int_en;      /* interrupt enable */
    td_u32 smmu_base;  /* smmu base address */
    td_u32 err_code;   /* error code */
} symc_module_status;

/* symc capacity, 0-nonsupport, 1-support */
typedef struct {
    td_u32 aes_ecb     : 1;    /* Support AES ECB  */
    td_u32 aes_cbc     : 1;    /* Support AES CBC  */
    td_u32 aes_cfb     : 1;    /* Support AES CFB  */
    td_u32 aes_ofb     : 1;    /* Support AES OFB  */
    td_u32 aes_ctr     : 1;    /* Support AES CTR  */
    td_u32 aes_ccm     : 1;    /* Support AES CCM  */
    td_u32 aes_gcm     : 1;    /* Support AES GCM  */
    td_u32 aes_cts     : 1;    /* Support AES CTS  */
    td_u32 tdes_ecb    : 1;    /* Support TDES ECB */
    td_u32 tdes_cbc    : 1;    /* Support TDES CBC */
    td_u32 tdes_cfb    : 1;    /* Support TDES CFB */
    td_u32 tdes_ofb    : 1;    /* Support TDES OFB */
    td_u32 tdes_ctr    : 1;    /* Support TDES CTR */
    td_u32 des_ecb     : 1;    /* Support DES ECB */
    td_u32 des_cbc     : 1;    /* Support DES CBC */
    td_u32 des_cfb     : 1;    /* Support DES CFB */
    td_u32 des_ofb     : 1;    /* Support DES OFB */
    td_u32 des_ctr     : 1;    /* Support DES CTR */
    td_u32 sm1_ecb     : 1;    /* Support SM1 ECB  */
    td_u32 sm1_cbc     : 1;    /* Support SM1 CBC  */
    td_u32 sm1_cfb     : 1;    /* Support SM1 CFB  */
    td_u32 sm1_ofb     : 1;    /* Support SM1 OFB  */
    td_u32 sm1_ctr     : 1;    /* Support SM1 CTR  */
    td_u32 sm4_ecb     : 1;    /* Support SM4 ECB  */
    td_u32 sm4_cbc     : 1;    /* Support SM4 CBC  */
    td_u32 sm4_cfb     : 1;    /* Support SM4 CFB  */
    td_u32 sm4_ofb     : 1;    /* Support SM4 OFB  */
    td_u32 sm4_ctr     : 1;    /* Support SM4 CTR  */
    td_u32 dma         : 1;    /* Support DMA  */
} symc_capacity;

typedef struct {
    compat_addr *in;
    compat_addr *out;
    td_u32 *len;
    symc_node_usage *usage;
    td_u32 num;
} symc_multi_pack;

/*
 * \brief          symc context structure
 *
 * \note           if the aes key derived from klad, the context msut
 *                 attached with a independent hard key channel,
 *                 otherwise the context can attached with a fixed common channel.
 */
typedef struct {
    td_u32 even_key[SYMC_KEY_SIZE / WORD_WIDTH];    /* SYMC even round keys, default */
    td_u32 odd_key[SYMC_KEY_SIZE / WORD_WIDTH];     /* SYMC odd round keys, default */
    td_u32 sk[SYMC_SM1_SK_SIZE / WORD_WIDTH];       /* sm1 sk */
    td_u32 iv[AES_IV_SIZE / WORD_WIDTH];            /* symc IV */
    td_u32 ivlen;                                   /* symc IV length */
    td_u32 iv_usage;                                /* symc IV usage */

    td_u32 hard_chn;             /* hard channel number */
    td_u32 hard_key;             /* Key derived from klad or CPU */

    symc_alg alg;                /* Symmetric cipher algorithm */
    symc_width width;            /* Symmetric cipher width */
    td_u32 klen;                 /* Symmetric cipher key length */

    compat_addr aad;             /* Associated Data */
    td_u32 alen;                 /* Associated Data length */
    td_u32 tlen;                 /* Tag length */

    symc_mode mode;              /* Symmetric cipher mode */

    td_u32 sm1_round;            /* SM1 round number */
    td_u32 enclen;               /* encrypt length */

    td_u32 block_size;           /* Block size */

    td_u32 cur_nodes;            /* current nodes id  */
    td_u32 total_nodes;          /* total number of nodes */

    compat_addr *input_list;     /* input node list */
    compat_addr *output_list;    /* output node list */
    td_u32 *length_list;         /* length of node list */
    symc_node_usage *usage_list; /* usage of node list */
} cryp_symc_context;

typedef td_s32 (*callback_symc_isr)(td_void *ctx);
typedef td_void (*callback_symc_destroy)(td_void);

/******************************* API Declaration *****************************/
/*
 * brief  Initialize the symc module.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_symc_init(td_void);

/*
 * brief  Deinitialize the symc module.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_symc_deinit(td_void);

/*
 * brief  resume the symc module.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_symc_resume(td_void);

/*
 * brief  allocate a hard symc channel.
 * param[out]  chn_num The channel number.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_symc_alloc_chn(td_u32 *chn_num, ot_cipher_type type);

/*
 * brief  free a hard symc channel.
 * param[in]  chn_num The channel number.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_void drv_symc_free_chn(td_u32 chn_num);

/*
 * brief  set the iv to the symc module.
 * param[in]  chn_num The channel number.
 * retval     NA.
 */
td_s32 drv_symc_reset(td_u32 chn_num);

/*
 * brief  check the length of nodes list.
 * param[in]  alg The symmetric cipher algorithm.
 * param[in]  mode The symmetric cipher mode.
 * param[in]  block_size The block size.
 * param[in]  pack pack data info.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_symc_node_check(symc_alg alg, symc_mode mode,
    symc_klen klen, td_u32 block_size, const symc_multi_pack *pack);

/*
 * brief  set work params.
 * param[in]  ctx cryp symc context info.
 * param[in]  decrypt Decrypt or encrypt.
 * param[in]  klen The key length.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_symc_cfg(const cryp_symc_context *symc_ctx, td_u32 decrypt, symc_klen klen);

/*
 * brief  set the iv to the symc module.
 * param[in]  chn_num The channel number.
 * param[in]  iv The IV data, hardware use the valid bytes according to the alg.
 * param[in]  flag The IV flag, should be OT_CIPHER_IV_CHG_ONE_PACK or OT_CIPHER_IV_CHG_ALL_PACK.
 * retval     NA.
 */
td_void drv_symc_set_iv(td_u32 chn_num, const td_u32 *iv, td_u32 ivlen, td_u32 flag);

/*
 * brief  get the iv to the symc module.
 * param[in]  chn_num The channel number.
 * param[out] iv The IV data, the length is 16.
 * retval     NA.
 */
void drv_symc_get_iv(td_u32 chn_num, td_u32 iv[SYMC_IV_MAX_SIZE_IN_WORD], td_u32 iv_len);

/*
 * brief  set the key to the symc module.
 * param[in]  chn_num The channel number.
 * param[in]  key The key data, hardware use the valid bytes according to the alg.
 * param[in]  odd This id odd key or not .
 * retval     NA.
 */
td_void drv_symc_set_key(td_u32 chn_num, const td_u32 *key, td_u32 key_len, td_u32 odd);

/*
\brief  set the key source to the symc module.
\param[in]  chn_num The channel number.
\param[in]  src The key source.
\retval     NA.
 */
void drv_symc_set_key_source(td_u32 chn_num, td_u32 keyslot);

/*
 * brief  add a in buffer to the nodes list.
 * param[in]  chn_num The channel number.
 * param[in]  buf_phy The MMZ/SMMU address of in buffer.
 * param[in]  buf_size The MMZ/SMMU siae of in buffer.
 * param[in]  local The locational of in buffer under a symmetric cipher.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_symc_add_inbuf(td_u32 chn_num, compat_addr buf_phy, td_u32 buf_size, symc_node_usage usage);

/*
 * brief  add a out buffer to the nodes list.
 * param[in]  chn_num The channel number.
 * param[in]  buf_phy The MMZ/SMMU address of out buffer.
 * param[in]  buf_size The MMZ/SMMU siae of out buffer.
 * param[in]  local The locational of in buffer under a symmetric cipher.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_symc_add_outbuf(td_u32 chn_num, compat_addr buf_phy, td_u32 buf_size, symc_node_usage usage);

/*
 * brief  add a buffer usage to the nodes list.
 * param[in]  chn_num The channel number.
 * param[in]  in in or out.
 * param[in]  usage uasge.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_void drv_symc_add_buf_usage(td_u32 chn_num, td_u32 in, symc_node_usage usage);

/*
 * brief  add N of CCM to the nodes list.
 * param[in]  chn_num The channel number.
 * param[in]  nonce The buffer of n, the size is 16.
 * param[in]  nonce_len The buffer of n, the size is 16.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_aead_ccm_add_n(td_u32 chn_num, const td_u8 *nonce, td_u32 nonce_len);

/*
 * brief  add A of CCM to the nodes list.
 * param[in]  chn_num The channel number.
 * param[in]  buf_phy The MMZ/SMMU address of A.
 * param[in]  buf_size The MMZ/SMMU size of A.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_aead_ccm_add_a(td_u32 chn_num, compat_addr buf_phy, td_u32 buf_size);

/*
 * brief  add A of GCM to the nodes list.
 * param[in]  chn_num The channel number.
 * param[in]  buf_phy The MMZ/SMMU address of A.
 * param[in]  buf_size The MMZ/SMMU size of A.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_aead_gcm_add_a(td_u32 chn_num, compat_addr buf_phy, td_u32 buf_size);

/*
 * brief  add length field of GCM to the nodes list.
 * param[in]  chn_num The channel number.
 * param[in]  buf_phy The MMZ/SMMU address of length field, the size is 16.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_aead_gcm_add_clen(td_u32 chn_num, const td_u8 *clen, td_u32 clen_len);

/*
 * brief  get ccm/gcm tag.
 * param[in]   chn_num The channel number.
 * param[out]  tag The tag value.
 * param[in]   tag_buf_len length in byte.
 * retval     On received interception, TD_TRUE is returned  otherwise TD_FALSE is returned.
 */
td_s32 drv_aead_get_tag(td_u32 chn_num, td_u32 *tag, td_u32 tag_buf_len);

/*
 * brief  start symmetric cipher calculation.
 * param[in]  chn_num The channel number.
 * retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_symc_start(td_u32 chn_num);

/*
 * brief  wait running finished.
 * param[in]  chn_num The channel number.
 * retval     On received interception, TD_TRUE is returned  otherwise TD_FALSE is returned.
 */
td_s32 drv_symc_wait_done(td_u32 chn_num, td_u32 timeout);

/*
 * brief  set isr callback function.
 * param[in]  chn_num The channel number.
 * retval     On finished, TD_TRUE is returned otherwise TD_FALSE is returned.
 */
td_s32 drv_symc_set_isr_callback(td_u32 chn_num, callback_symc_isr callback, td_void *tx);

/*
 * brief  proc status.
 * param[in]  status The  proc status.
 * retval     On received interception, TD_TRUE is returned  otherwise TD_FALSE is returned.
 */
void drv_symc_proc_status(symc_chn_status *status, td_u32 status_buf_len, symc_module_status *module);

/*
 * brief  get the symc capacity.
 * param[out] capacity The symc capacity.
 * retval     NA.
 */
td_void drv_symc_get_capacity(symc_capacity *capacity);

#endif /* DRV_SYMC_H */
