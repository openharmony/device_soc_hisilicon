/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <lib.h>
#include <common.h>
#include <securecutil.h>
#include <platform.h>
#include <sys_reg.h>
#include "../pke/ot_drv_pke.h"
#include "cipher_reg.h"
#include "cipher_union_define.h"
#include "cipher.h"

#define CIPHER_LOGIC_MAX_CHN                16
#define CIPHER_MAX_DEPTH                    2
#define CIPHER_IN_NODE_SIZE                 64
#define CIPHER_HASH_IN_NODE_SIZE            16
#define CIPHER_OUT_NODE_SIZE                16
#define CIPHER_PAGE_SIZE                    ((CIPHER_IN_NODE_SIZE + CIPHER_OUT_NODE_SIZE + \
            CIPHER_HASH_IN_NODE_SIZE)* CIPHER_MAX_DEPTH)
#define CIPHER_TRASHBOX_SIZE                128
#define SPACC_SYMC_IN_ENTRY_TOTAL_SIZE      (CIPHER_IN_NODE_SIZE * CIPHER_MAX_DEPTH)
#define SPACC_SYMC_OUT_ENTRY_TOTAL_SIZE     (CIPHER_OUT_NODE_SIZE * CIPHER_MAX_DEPTH)
#define SPACC_DIGEST_IN_ENTRY_TOTAL_SIZE    (CIPHER_HASH_IN_NODE_SIZE * CIPHER_MAX_DEPTH)
#define SPACC_LOCAL_BUF_SIZE                (CIPHER_PAGE_SIZE + CIPHER_TRASHBOX_SIZE + \
        (sizeof(struct cipher_symc_context) * CIPHER_MAX_DEPTH) + \
        (sizeof(struct cipher_digest_context) * CIPHER_MAX_DEPTH))

#define CIPHER_TIME_OUT                     5000
#define CIPHER_TIME_OUT_LONG                50000

#define SYMC_KEY_SIZE128                    0x01
#define SYMC_KEY_SIZE256                    0x03
#define SYMC_DAT_WIDTH128                   0x00

#define DIGEST_ALG_SM3                      0x0c
#define DIGEST_ALG_SHA2                     0x0b
#define DIGEST_ALG_SHA256                   0x01
#define CIPHER_CTRL_NONE                    0x00
#define CIPHER_HASH_VAL_SIZA                4UL

#define CIPHER_CTRL_HASH_IN_FIRST           0x01
#define CIPHER_CTRL_HASH_IN_LAST            0x02

#define CHN_WHO_USED_IDLE                   0x00
#define CHN_WHO_USED_TEE                    0x02

#define chn_who_used_clr(used, chn)         (used) &= ~(0x03U << ((chn) * 2U))
#define chn_who_used_set(used, chn, who)    (used) |= (who) << ((chn) * 2U)
#define chn_who_used_get(used, chn)         (((used) >> ((chn) * 2U)) & 0x03U)

/* SHA256, the initial hash value */
#define SHA256_H0    0x6a09e667UL
#define SHA256_H1    0xbb67ae85UL
#define SHA256_H2    0x3c6ef372UL
#define SHA256_H3    0xa54ff53aUL
#define SHA256_H4    0x510e527fUL
#define SHA256_H5    0x9b05688cUL
#define SHA256_H6    0x1f83d9abUL
#define SHA256_H7    0x5be0cd19UL

/* SM3, the initial hash value */
#define SM3_H0    0x7380166FUL
#define SM3_H1    0x4914B2B9UL
#define SM3_H2    0x172442D7UL
#define SM3_H3    0xDA8A0600UL
#define SM3_H4    0xA96F30BCUL
#define SM3_H5    0x163138AAUL
#define SM3_H6    0xE38DEE4DUL
#define SM3_H7    0xB0FB0E4EUL

#define SHIFT_29 29
#define SHIFT_21 21
#define SHIFT_13 13
#define SHIFT_5 5
#define SHIFT_3 3

#define PADDING_LEN_THRESHOLD 56
#define PADDING_LEN_MAX 120
#define PAD_LEN_FIELD_SIZE 8
#define SHA256_BUFF_LEN 16
#define SM3_BUFF_LEN 16
#define INFO_DIGEST_LEN 32
#define IV_WORD_SIZE 4     /* iv 4 word size */

#define cpu_to_be32(v) (((unsigned int)(v) >> 24) | \
        (((unsigned int)(v) >> 8) & 0xff00) | \
        (((unsigned int)(v) << 8) & 0xff0000) | \
        ((unsigned int)(v) << 24))

struct hash_info {
    uint32_t     is_init;
    uint32_t     mode;
    uint32_t     alg;
};

static struct hash_info  g_hash_info;

struct hash_chann_param_t {
    in_hash_chn_node_wr_point in_node_wr_ptr;
    in_hash_chn_node_rd_point in_node_rd_ptr;
    hash_chann_raw_int    int_raw;
};

struct cipher_symc_in_entry_t {
    uint32_t     sym_first_node : 1;
    uint32_t     sym_last_node : 1;
    uint32_t     rev1 : 7;
    uint32_t     odd_even : 1;
    uint32_t     rev2 : 6;
    uint32_t     sym_gcm_first : 1;
    uint32_t     sym_ccm_last : 1;
    uint32_t     aes_ccm_gcm_in_flag : 2;
    uint32_t     rev3 : 4;
    uint32_t     sym_gcm_and_ctr_iv_len : 5;
    uint32_t     rev4 : 3;
    uint32_t     sym_alg_length;
    uint32_t     sym_start_addr;
    uint32_t     sym_start_high;
    uint32_t     iv[IV_WORD_SIZE];
    uint32_t     cenc_enable : 1;
    uint32_t     rev5 : 7;
    uint32_t     cenc_head : 1;
    uint32_t     rev6 : 23;
    uint32_t     e_num : 5;
    uint32_t     rev7 : 27;
    uint32_t     c_num : 5;
    uint32_t     rev8 : 27;
    uint32_t     cenc_offset : 8;
    uint32_t     rev9 : 24;
    uint32_t     rev10;
    uint32_t     rev11;
    uint32_t     rev12;
    uint32_t     rev13;
};

struct cipher_digest_in_entry_t {
    uint32_t    spacc_cmd : 2;
    uint32_t    rev1 : 14;
    uint32_t    hmac_last_block : 1;
    uint32_t    rev2 : 15;
    uint32_t    hash_alg_length;
    uint32_t    hash_start_addr_l;
    uint32_t    hash_start_addr_h;
};

struct cipher_symc_out_entry_t {
    uint32_t    rev1;
    uint32_t    sym_alg_length;
    uint32_t    sym_start_addr;
    uint32_t    sym_start_high;
};

struct cipher_symc_context {
    struct cipher_symc_in_entry_t   *entry_symc_in;
    struct cipher_symc_out_entry_t  *entry_symc_out;

    uint32_t entry_symc_in_depth;
    uint32_t entry_symc_out_depth;
    uint32_t symc_cur_in_nodes;
    uint32_t symc_cur_out_nodes;
};

struct cipher_digest_context {
    unsigned int     digest_len;

    struct cipher_digest_in_entry_t *entry_digest_in;
    unsigned int entry_digest_in_depth;
    unsigned int digest_cur_in_nodes;
};

static struct cipher_symc_context  *g_symc;
static struct cipher_digest_context *g_digest;
static uint8_t *g_cipher_nodes_phy;
static uint8_t *g_trash_box_phy;
static uint8_t g_is_spacc_init = 0;
static uint32_t g_channel_number = 0;

/*********************************************************************************************
 *
 *  internal functions
 *
 *********************************************************************************************/
/**
 * \brief Begin DMA operation
 * \param chn Channel number of DMA channel to start
 * \return TD_SUCCESS if started OK, other wise CIPHER_ERR_BUSY
 */
static int symc_start(const uint32_t chn, struct cipher_symc_context *info)
{
    in_sym_chn_node_wr_point cipher_in_wr_ptr;
    out_sym_chn_node_wr_point cipher_out_wr_ptr;
    out_sym_chn_node_rd_point out_node_rd_ptr;
    uint32_t ptr;

    cipher_out_wr_ptr.u32 = reg_get(out_sym_chn_node_wr_point(chn));
    out_node_rd_ptr.u32 = reg_get(out_sym_chn_node_rd_point(chn));
    if (cipher_out_wr_ptr.bits.out_sym_chn_node_wr_point !=
        out_node_rd_ptr.bits.out_sym_chn_node_rd_point)
        return CIPHER_ERR_BUSY;

    ptr = cipher_out_wr_ptr.bits.out_sym_chn_node_wr_point + info->entry_symc_out_depth;
    cipher_out_wr_ptr.bits.out_sym_chn_node_wr_point = ptr % CIPHER_MAX_DEPTH;
    reg_set(out_sym_chn_node_wr_point(chn), cipher_out_wr_ptr.u32);
    if (cipher_out_wr_ptr.u32 != reg_get(out_sym_chn_node_wr_point(chn)))
        return TD_FAILURE;

    cipher_in_wr_ptr.u32 = reg_get(in_sym_chn_node_wr_point_fun(chn));
    ptr = cipher_in_wr_ptr.bits.sym_chn_node_wr_point + info->entry_symc_in_depth;
    cipher_in_wr_ptr.bits.sym_chn_node_wr_point = ptr % CIPHER_MAX_DEPTH;
    reg_set(in_sym_chn_node_wr_point_fun(chn), cipher_in_wr_ptr.u32);
    if (cipher_in_wr_ptr.u32 != reg_get(in_sym_chn_node_wr_point_fun(chn)))
        return TD_FAILURE;

    info->entry_symc_in_depth = 0;
    info->entry_symc_out_depth = 0;

    return TD_SUCCESS;
}

/**
 * \brief Check for DMA channel completion
 * \param chn Channel to check
 * \return TD_FAILURE if not complete, otherwise TD_SUCCESS
 */
static int symc_done_notify(const uint32_t chn)
{
    out_sym_chan_int    int_raw;
    uint32_t            chn_mask = 1 << chn;

    int_raw.u32 = reg_get(OUT_SYM_CHAN_RAW_LAST_NODE_INT);
    int_raw.bits.out_sym_chan_int &= chn_mask;
    chn_mask = int_raw.bits.out_sym_chan_int;
    reg_set(OUT_SYM_CHAN_RAW_LAST_NODE_INT, int_raw.u32);

    return (chn_mask) ? TD_SUCCESS : TD_FAILURE;
}

/*! all config of hash chn will be clear up, this chn need init before used */
static int clear_hash_entry(uint32_t chn)
{
    uint32_t i = 0;
    spacc_hash_chn_clear_req   hash_chn_clear_req;
    spacc_int_raw_hash_clear_finish  hash_clear_finish;

    hash_chn_clear_req.u32 = reg_get(SPACC_HASH_CHN_CLEAR_REQ);
    hash_chn_clear_req.bits.hash_chn_clear_req |= (0x1 << chn);
    reg_set(SPACC_HASH_CHN_CLEAR_REQ, hash_chn_clear_req.u32);
    while (i++ < CIPHER_TIME_OUT_LONG) {
        udelay(10); /* delay 10 us */
        hash_clear_finish.u32 = reg_get(SPACC_INT_RAW_HASH_CLEAR_FINISH);
        if (hash_clear_finish.bits.int_raw_hash_clear_finish & (0x1 << chn))  {
            hash_clear_finish.bits.int_raw_hash_clear_finish &= (0x1 << chn);
            reg_set(SPACC_INT_RAW_HASH_CLEAR_FINISH, hash_clear_finish.u32);
            break;
        }
    }
    if (i >= CIPHER_TIME_OUT)
        return TD_FAILURE;

    return TD_SUCCESS;
}

/*! all config of hash chn will be clear up, this chn need init before used */
static int clear_symc_entry(uint32_t chn)
{
    uint32_t i = 0;
    spacc_sym_chn_clear_req   sym_chn_clear_req;
    spacc_int_raw_sym_clear_finish  sym_clear_finish;

    sym_chn_clear_req.u32 = reg_get(SPACC_SYM_CHN_CLEAR_REQ);
    sym_chn_clear_req.bits.sym_chn_clear_req |= (0x1 << chn);
    reg_set(SPACC_SYM_CHN_CLEAR_REQ, sym_chn_clear_req.u32);
    while (i++ < CIPHER_TIME_OUT_LONG) {
        udelay(10); /* delay 10 us */
        sym_clear_finish.u32 = reg_get(SPACC_INT_RAW_SYM_CLEAR_FINISH);
        if (sym_clear_finish.bits.int_raw_sym_clear_finish & (0x1 << chn))  {
            sym_clear_finish.bits.int_raw_sym_clear_finish &= (0x1 << chn);
            reg_set(SPACC_INT_RAW_SYM_CLEAR_FINISH, sym_clear_finish.u32);
            break;
        }
    }
    if (i >= CIPHER_TIME_OUT)
        return TD_FAILURE;

    return TD_SUCCESS;
}

/**
 * \brief clear prev channel config and init channel configuration
 * \param chn channel id which alloc success
 * \return channel id (1~15) for successful completion or TD_FAILURE on failure
 */
static int cipher_init_chn(uint32_t chn, uint32_t key_addr)
{
    void *page_phy = NULL;
    in_sym_chn_ctrl in_sym_chn_ctrl;
    in_sym_chn_node_wr_point cipher_in_cfg;
    out_sym_chn_node_wr_point cipher_out_cfg;
    in_sym_chn_node_length cipher_in_depth;
    out_sym_chn_node_length cipher_out_depth;
    in_sym_chn_key_ctrl in_sym_key_ctrl;

    if (TD_FAILURE == clear_symc_entry(chn))
        return TD_FAILURE;

    page_phy = g_cipher_nodes_phy;

    /* set total num and start addr for cipher in node */
    cipher_in_depth.u32  = reg_get(in_sym_chn_node_length_fun(chn));
    cipher_in_depth.bits.sym_chn_node_length = CIPHER_MAX_DEPTH;
    reg_set(in_sym_chn_node_length_fun(chn), cipher_in_depth.u32);
    reg_set(in_sym_chn_node_start_addr_l_fun(chn), (uintptr_t)page_phy);
    cipher_in_cfg.u32  = reg_get(in_sym_chn_node_wr_point_fun(chn));
    g_symc->entry_symc_in = page_phy;
    g_symc->symc_cur_in_nodes = cipher_in_cfg.bits.sym_chn_node_wr_point;
    g_symc->entry_symc_in_depth = 0;
    page_phy += SPACC_SYMC_IN_ENTRY_TOTAL_SIZE;

    /* set total num and start addr for cipher out node */
    cipher_out_depth.u32 = reg_get(out_sym_chn_node_length(chn));
    cipher_out_depth.bits.out_sym_chn_node_length = CIPHER_MAX_DEPTH;
    reg_set(out_sym_chn_node_length(chn), cipher_out_depth.u32);
    reg_set(out_sym_chn_node_start_addr_l(chn), (uintptr_t)page_phy);
    cipher_out_cfg.u32  = reg_get(out_sym_chn_node_wr_point(chn));
    g_symc->entry_symc_out = page_phy;
    g_symc->symc_cur_out_nodes = cipher_out_cfg.bits.out_sym_chn_node_wr_point;
    g_symc->entry_symc_out_depth = 0;

    in_sym_chn_ctrl.u32 = reg_get(in_sym_chn_ctrl_fun(chn));
    in_sym_chn_ctrl.bits.sym_chn_en = 1;
    in_sym_chn_ctrl.bits.sym_chn_ss = 0x5;
    in_sym_chn_ctrl.bits.sym_chn_ds = 0x5;
    reg_set(in_sym_chn_ctrl_fun(chn), in_sym_chn_ctrl.u32);

    in_sym_key_ctrl.u32 = reg_get(in_sym_chn_key_ctrl_fun(chn));
    in_sym_key_ctrl.bits.sym_key_chn_id = key_addr;
    reg_set(in_sym_chn_key_ctrl_fun(chn), in_sym_key_ctrl.u32);

    return TD_SUCCESS;
}

/**
 * \brief clear prev channel config and init channel configuration
 * \param chn channel id which alloc success
 * \return channel id (1~15) for successful completion or TD_FAILURE on failure
 */
static int hash_init_chn(uint32_t chn)
{
    void *page_phy = NULL;
    in_hash_chn_ctrl hash_chn_ctrl;
    in_hash_chn_node_length hash_node_len;
    in_hash_chn_node_wr_point hash_wr_ptr;

    if (TD_FAILURE == clear_hash_entry(chn))
        return TD_FAILURE;

    page_phy = g_cipher_nodes_phy;
    page_phy += (SPACC_SYMC_IN_ENTRY_TOTAL_SIZE + SPACC_SYMC_OUT_ENTRY_TOTAL_SIZE);

    /* set total num and start addr for cipher in node */
    hash_node_len.u32  = reg_get(in_hash_chn_node_length_fun(chn));
    hash_node_len.bits.hash_chn_node_length = CIPHER_MAX_DEPTH;
    reg_set(in_hash_chn_node_length_fun(chn), hash_node_len.u32);
    reg_set(in_hash_chn_node_start_fun(chn), (uintptr_t)page_phy);
    hash_wr_ptr.u32  = reg_get(in_hash_chn_node_wr_point_fun(chn));
    g_digest->entry_digest_in = page_phy;
    g_digest->digest_cur_in_nodes = hash_wr_ptr.bits.hash_chn_node_wr_point;
    g_digest->entry_digest_in_depth = 0;

    hash_chn_ctrl.u32 = reg_get(in_hash_chn_ctrl_fun(chn));
    hash_chn_ctrl.bits.hash_chn_en = 1;
    hash_chn_ctrl.bits.hash_chn_ss = 1;
    reg_set(in_hash_chn_ctrl_fun(chn), hash_chn_ctrl.u32);

    return TD_SUCCESS;
}
/*********************************************************************************************
 *
 *  Initialisation and de-initialisation functions
 *
 *********************************************************************************************/
/**
 * \brief Reset the spacc and configure it
 */
static int drv_spacc_init(void)
{
    tee_crg_ctrl un_peri_crg_scipher;
    errno_t err;

    if (g_is_spacc_init == 1)
        return TD_SUCCESS;

    /* Enable spacc clock */
    un_peri_crg_scipher.u32 = reg_get(TEE_CRG_CTRL);

    un_peri_crg_scipher.bits.spacc_cken = 0x1;   /* clock opened */
    reg_set(TEE_CRG_CTRL, un_peri_crg_scipher.u32);
    udelay(10); /* delay 10 us */

    un_peri_crg_scipher.bits.spacc_srst_req = 0x1; /* reset */
    reg_set(TEE_CRG_CTRL, un_peri_crg_scipher.u32);
    udelay(10); /* delay 10 us */

    un_peri_crg_scipher.bits.spacc_srst_req = 0x0; /* cancel reset */
    reg_set(TEE_CRG_CTRL, un_peri_crg_scipher.u32);
    udelay(10); /* delay 10 us */

    /* Allocate the node buffer */
    g_cipher_nodes_phy = gsl_malloc(SPACC_LOCAL_BUF_SIZE);
    if (g_cipher_nodes_phy == NULL)
        return TD_FAILURE;

    err = memset_s(g_cipher_nodes_phy, SPACC_LOCAL_BUF_SIZE, 0, SPACC_LOCAL_BUF_SIZE);
    if (err != EOK) {
        return TD_FAILURE;
    }

    g_trash_box_phy = g_cipher_nodes_phy + CIPHER_PAGE_SIZE;
    g_symc = (struct cipher_symc_context *)((uint8_t *)g_trash_box_phy + CIPHER_TRASHBOX_SIZE);
    g_digest = (struct cipher_digest_context *)((uint8_t *)g_symc +
            sizeof(struct cipher_symc_context) * CIPHER_MAX_DEPTH);

    reg_set(SPACC_MMU_IN_SYM_SEC_EADDR_L, (uintptr_t)g_trash_box_phy);
    reg_set(SPACC_MMU_OUT_SYM_SEC_EADDR_L, (uintptr_t)g_trash_box_phy);

    reg_set(SPACC_MMU_IN_SYM_NOSEC_EADDR_L, (uintptr_t)g_trash_box_phy);
    reg_set(SPACC_MMU_OUT_SYM_NOSEC_EADDR_L, (uintptr_t)g_trash_box_phy);

    g_is_spacc_init = 1;

    return TD_SUCCESS;
}

void drv_spacc_deinit(void)
{
    if (g_is_spacc_init == 0) {
        return;
    }

    gsl_free(g_cipher_nodes_phy);

    g_cipher_nodes_phy = NULL;
    g_trash_box_phy = NULL;
    g_symc = NULL;
    g_digest = NULL;

    g_is_spacc_init = 0;

    return;
}

/*********************************************************************************************
 *
 *  Plain and encrypting DMA functions
 *
 *********************************************************************************************/
/**
 * \brief alloc cipher channel and init channel configuration
 * \param chn channel id which alloc success
 * \return channel id (1~15) for successful completion or TD_FAILURE on failure
 */
static int drv_spacc_alloc_cipher(uint32_t key_addr, uint32_t *chn)
{
    uint32_t i;
    uint32_t spacc_sym_chn_lock;

    for (i = 1; i < CIPHER_LOGIC_MAX_CHN; i++) {
        spacc_sym_chn_lock = reg_get(SPACC_SYM_CHN_LOCK);
        if (chn_who_used_get(spacc_sym_chn_lock, i) == CHN_WHO_USED_IDLE) {
            chn_who_used_set(spacc_sym_chn_lock, i, CHN_WHO_USED_TEE);
            reg_set(SPACC_SYM_CHN_LOCK, spacc_sym_chn_lock);
        } else {
            continue;
        }
        spacc_sym_chn_lock = reg_get(SPACC_SYM_CHN_LOCK);
        if (chn_who_used_get(spacc_sym_chn_lock, i) == CHN_WHO_USED_TEE) {
            if (TD_FAILURE == cipher_init_chn(i, key_addr))
                return TD_FAILURE;

            *chn = i;
            return TD_SUCCESS;
        }
    }
    return TD_FAILURE;
}

/**
 * \brief release cipher channel and clear channel config
 * \param chn channel id which need to release
 * \return TD_SUCCESS for successful completion or TD_FAILURE on failure
 */
uint32_t drv_spacc_release_cipher(uint32_t chn)
{
    uint8_t release_times = 10;
    uint32_t spacc_sym_chn_lock;
    uint32_t reg_val_readback;

    spacc_sym_chn_lock = reg_get(SPACC_SYM_CHN_LOCK);
    if (chn_who_used_get(spacc_sym_chn_lock, chn) != CHN_WHO_USED_TEE) {
        return TD_FAILURE;
    }

    if (TD_FAILURE == clear_symc_entry(chn)) {
        return TD_FAILURE;
    }

    chn_who_used_clr(spacc_sym_chn_lock, chn);
    while (release_times--) {
        reg_set(SPACC_SYM_CHN_LOCK, spacc_sym_chn_lock);
        reg_val_readback = reg_get(SPACC_SYM_CHN_LOCK);
        if (chn_who_used_get(reg_val_readback, chn) == CHN_WHO_USED_IDLE) {
            break;
        }
    }
    if (release_times == 0) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/**
 * \brief alloc hash channel and init channel configuration
 * \param chn channel id which alloc success
 * \return channel id (0~16) for successful completion or TD_FAILURE on failure
 */
static int drv_spacc_alloc_hash(uint32_t *chn)
{
    uint32_t i;
    uint32_t spacc_sym_chn_lock;

    for (i = 1; i < CIPHER_LOGIC_MAX_CHN; i++) {
        spacc_sym_chn_lock = reg_get(SPACC_HASH_CHN_LOCK);
        if (chn_who_used_get(spacc_sym_chn_lock, i) == CHN_WHO_USED_IDLE)  {
            chn_who_used_set(spacc_sym_chn_lock, i, CHN_WHO_USED_TEE);
            reg_set(SPACC_HASH_CHN_LOCK, spacc_sym_chn_lock);
        } else {
            continue;
        }
        spacc_sym_chn_lock = reg_get(SPACC_HASH_CHN_LOCK);
        if (chn_who_used_get(spacc_sym_chn_lock, i) == CHN_WHO_USED_TEE)  {
            if (TD_FAILURE == hash_init_chn(i))
                return TD_FAILURE;

            *chn = i;
            return TD_SUCCESS;
        }
    }
    return TD_FAILURE;
}
/**
 * \brief release hash channel and clear channel config
 * \param chn channel id which need to release
 * \return TD_SUCCESS for successful completion or TD_FAILURE on failure
 */
static int drv_spacc_release_hash(uint32_t chn)
{
    uint8_t release_times = 10;
    uint32_t spacc_hash_chn_lock;
    uint32_t reg_val_readback;

    spacc_hash_chn_lock = reg_get(SPACC_HASH_CHN_LOCK);
    if (chn_who_used_get(spacc_hash_chn_lock, chn) != CHN_WHO_USED_TEE)
        return TD_FAILURE;

    if (TD_FAILURE == clear_hash_entry(chn))
        return TD_FAILURE;

    chn_who_used_clr(spacc_hash_chn_lock, chn);
    while (release_times--) {
        reg_set(SPACC_HASH_CHN_LOCK, spacc_hash_chn_lock);
        reg_val_readback = reg_get(SPACC_HASH_CHN_LOCK);
        if (chn_who_used_get(reg_val_readback, chn) == CHN_WHO_USED_IDLE) {
            break;
        }
    }
    if (release_times == 0)
        return TD_FAILURE;

    return TD_SUCCESS;
}

int do_structure(spacc_decrypt_params decrypt_params)
{
    uint32_t id_in;
    uint32_t id_out;
    uint32_t size;
    void *addr = NULL;
    struct cipher_symc_context *info = g_symc;

    info->entry_symc_in_depth = 0;
    info->entry_symc_out_depth = 0;

    /* Input structure */
    id_in = info->symc_cur_in_nodes++;
    addr = &info->entry_symc_in[id_in];
    size = sizeof(struct cipher_symc_in_entry_t);
    if (memset_s(addr, sizeof(struct cipher_symc_in_entry_t), 0, size) != EOK)
        return TD_FAILURE;

    info->entry_symc_in[id_in].sym_first_node = 1;
    info->entry_symc_in[id_in].sym_last_node = 1;
    info->entry_symc_in[id_in].sym_start_addr = decrypt_params.src_addr;
    info->entry_symc_in[id_in].sym_alg_length = decrypt_params.length;
    if (decrypt_params.alg != SYMC_ALG_DMA) {
        if (memset_s(info->entry_symc_in[id_in].iv, IV_WORD_SIZE * sizeof(uint32_t),
      0, decrypt_params.iv_length) != EOK)
            return TD_FAILURE;

        if (memcpy_s(info->entry_symc_in[id_in].iv, decrypt_params.iv_length,
      (const void *)(uintptr_t)decrypt_params.iv_addr,
      decrypt_params.iv_length) != EOK)
            return TD_FAILURE;
    }
    info->entry_symc_in_depth++;
    info->symc_cur_in_nodes %= CIPHER_MAX_DEPTH;

    /* Output structure */
    id_out = info->symc_cur_out_nodes++;
    addr = &info->entry_symc_out[id_out];
    size = sizeof(struct cipher_symc_out_entry_t);
    if (memset_s(addr, sizeof(struct cipher_symc_out_entry_t), 0, size) != EOK)
        return TD_FAILURE;
    info->entry_symc_out[id_out].sym_start_addr = decrypt_params.dst_addr; /* In-place copy */
    info->entry_symc_out[id_out].sym_alg_length = decrypt_params.length;
    info->entry_symc_out_depth++;
    info->symc_cur_out_nodes %= CIPHER_MAX_DEPTH;

    return TD_SUCCESS;
}

int do_symc_dma_configuration(spacc_decrypt_params decrypt_params)
{
    in_sym_chn_key_ctrl cipher_ctrl;
    in_sym_out_ctrl cipher_out_ctrl;
    struct cipher_symc_context *info = g_symc;
    unsigned time_out = 0;

    /* spacc symc configuration */
    cipher_ctrl.u32 = reg_get(in_sym_chn_key_ctrl_fun(decrypt_params.chn));
    cipher_ctrl.bits.sym_alg_decrypt = 0x01;
    cipher_ctrl.bits.sym_alg_data_width = SYMC_DAT_WIDTH128;
    cipher_ctrl.bits.sym_alg_key_len = SYMC_KEY_SIZE256;
    cipher_ctrl.bits.sym_alg_mode = decrypt_params.mode;
    cipher_ctrl.bits.sym_alg_sel = decrypt_params.alg;
    reg_set(in_sym_chn_key_ctrl_fun(decrypt_params.chn), cipher_ctrl.u32);

    /* spacc DMA configuration */
    cipher_out_ctrl.u32 = reg_get(in_sym_out_ctrl_fun(decrypt_params.chn));
    if (decrypt_params.alg == SYMC_ALG_DMA) {
        cipher_out_ctrl.bits.sym_dma_copy = 1;
    } else {
        cipher_out_ctrl.bits.sym_dma_copy = 0;
    }

    reg_set(in_sym_out_ctrl_fun(decrypt_params.chn), cipher_out_ctrl.u32);

    /* check spacc symc configuration */
    cipher_ctrl.u32 = reg_get(in_sym_chn_key_ctrl_fun(decrypt_params.chn));

    /* check spacc DMA configuration */
    cipher_out_ctrl.u32 = reg_get(in_sym_out_ctrl_fun(decrypt_params.chn));

    if (symc_start(decrypt_params.chn, info) !=  TD_SUCCESS)
        return TD_FAILURE;

    /* Wait for completion */
    while (time_out++ < CIPHER_TIME_OUT_LONG) {
        udelay(10); /* delay 10 us */
        if (TD_SUCCESS == symc_done_notify(decrypt_params.chn))
            return TD_SUCCESS;
    }

    return CIPHER_ERR_TIMEOUT;
}

/**
 * \brief Perform an in-place decryption of the code
 * \return TD_SUCCESS for successful completion or CIPHER_ERR_TIMEOUT on failure
 */
int drv_spacc_decrypt(spacc_decrypt_params decrypt_params)
{
    if (do_structure(decrypt_params) != TD_SUCCESS)
        return TD_FAILURE;

    return do_symc_dma_configuration(decrypt_params);
}

/*********************************************************************************************
 *
 *  Hash functions
 *
 *********************************************************************************************/

static int hash_process(uint32_t chn, uint32_t data_addr, uint32_t data_len, uint32_t ctrl)
{
    uint32_t time_out;
    uint32_t mask;
    uint32_t id;
    uint32_t size;
    struct cipher_digest_context *info = g_digest;
    void *addr = NULL;
    uint32_t ptr;
    struct hash_chann_param_t hash_chann_param = {0};

    time_out = 0;
    /* Configure the node */
    id = info->digest_cur_in_nodes++;
    addr = &info->entry_digest_in[id];
    size = sizeof(struct cipher_digest_in_entry_t);
    if (memset_s(addr, sizeof(struct cipher_digest_in_entry_t), 0, size) !=  EOK)
        return TD_FAILURE;

    info->entry_digest_in[id].spacc_cmd = ctrl;
    info->entry_digest_in[id].hmac_last_block = 0;
    info->entry_digest_in[id].hash_start_addr_l = data_addr;
    info->entry_digest_in[id].hash_start_addr_h = 0;
    info->entry_digest_in[id].hash_alg_length = data_len;
    info->entry_digest_in_depth++;
    info->digest_cur_in_nodes %= CIPHER_MAX_DEPTH;

    hash_chann_param.in_node_wr_ptr.u32 = reg_get(in_hash_chn_node_wr_point_fun(chn));
    hash_chann_param.in_node_rd_ptr.u32 = reg_get(in_hash_chn_node_rd_point_fun(chn));
    if (hash_chann_param.in_node_wr_ptr.bits.hash_chn_node_wr_point !=
        hash_chann_param.in_node_rd_ptr.bits.hash_chn_node_rd_point)
        return CIPHER_ERR_BUSY;

    ptr = hash_chann_param.in_node_wr_ptr.bits.hash_chn_node_wr_point + info->entry_digest_in_depth;
    hash_chann_param.in_node_wr_ptr.bits.hash_chn_node_wr_point = ptr % CIPHER_MAX_DEPTH;

    reg_set(in_hash_chn_node_wr_point_fun(chn), hash_chann_param.in_node_wr_ptr.u32);

    info->entry_digest_in_depth = 0;

    while (time_out++ < CIPHER_TIME_OUT_LONG) {
        hash_chann_param.int_raw.u32 = reg_get(HASH_CHANN_RAW_INT);
        hash_chann_param.int_raw.bits.hash_chann_raw_int &= (1 << chn);
        mask = hash_chann_param.int_raw.bits.hash_chann_raw_int;
        reg_set(HASH_CHANN_RAW_INT, hash_chann_param.int_raw.u32);
        if (mask)
            return TD_SUCCESS;

        udelay(10); /* delay 10 us */
    }

    return TD_FAILURE;
}

static uint32_t drv_spacc_hash_padding(uint8_t *msg, uint32_t msg_size, uint32_t cur_len,
    uint32_t total_len)
{
    uint32_t tmp;
    uint32_t padding_len = 0;
    errno_t err;

    if (g_hash_info.alg == HASH_ALG_SHA256 || g_hash_info.alg == HASH_ALG_SM3) {
        tmp = total_len % HASH_BLOCK_SIZE;
        padding_len = (tmp < PADDING_LEN_THRESHOLD) ?
                  (PADDING_LEN_THRESHOLD - tmp) : (PADDING_LEN_MAX - tmp);
        padding_len += PAD_LEN_FIELD_SIZE;

        err = memset_s(&msg[cur_len], msg_size, 0, padding_len - PAD_LEN_FIELD_SIZE);
        if (err != EOK) {
            return 0;
        }

        msg[cur_len++] = 0x80;
        cur_len += padding_len - 1 - PAD_LEN_FIELD_SIZE;

        /* write 8 bytes fix data length */
        msg[cur_len++] = 0x00;
        msg[cur_len++] = 0x00;
        msg[cur_len++] = 0x00;
        msg[cur_len++] = (unsigned char)((total_len >> SHIFT_29) & 0x07);
        msg[cur_len++] = (unsigned char)((total_len >> SHIFT_21) & 0xff);
        msg[cur_len++] = (unsigned char)((total_len >> SHIFT_13) & 0xff);
        msg[cur_len++] = (unsigned char)((total_len >> SHIFT_5) & 0xff);
        msg[cur_len++] = (unsigned char)((total_len << SHIFT_3) & 0xff);
    } else {
        call_reset();
    }

    return padding_len;
}

static int drv_spacc_hash_init(uint32_t chn, hash_mode mode, hash_alg alg)
{
    uint32_t i;
    in_hash_chn_key_ctrl hash_ctrl;
    struct cipher_digest_context *info = g_digest;
    static const uint32_t sha256_state[SHA256_BUFF_LEN] = {
    cpu_to_be32(SHA256_H0), cpu_to_be32(SHA256_H1), cpu_to_be32(SHA256_H2),
    cpu_to_be32(SHA256_H3), cpu_to_be32(SHA256_H4), cpu_to_be32(SHA256_H5),
    cpu_to_be32(SHA256_H6), cpu_to_be32(SHA256_H7)
    };

    g_hash_info.is_init = 0;
    g_hash_info.mode = mode;
    g_hash_info.alg = alg;

    hash_ctrl.u32 = reg_get(in_hash_chn_key_ctrl_fun(chn));
    hash_ctrl.bits.hash_chn_alg_mode = DIGEST_ALG_SHA256;
    hash_ctrl.bits.hash_chn_alg_sel = DIGEST_ALG_SHA2;
    reg_set(in_hash_chn_key_ctrl_fun(chn), hash_ctrl.u32);

    info->entry_digest_in_depth = 0;
    info->digest_len = INFO_DIGEST_LEN;
    for (i = 0; i < SHA256_BUFF_LEN; i++)  {
        reg_set(chn_hash_state_val_addr(chn), i);
        reg_set(chn_hash_state_val(chn), sha256_state[i]);
    }

    return TD_SUCCESS;
}

static int drv_spacc_hash_update(uint32_t chn, const uint32_t data_addr,
    const uint32_t data_len)
{
    volatile int ret;
    uint32_t ctrl = CIPHER_CTRL_NONE;

    if (data_len == 0)
        return TD_SUCCESS;

    if (!g_hash_info.is_init) {
        ctrl |= CIPHER_CTRL_HASH_IN_FIRST;
        g_hash_info.is_init = 1;
    }
    ctrl |= CIPHER_CTRL_HASH_IN_LAST;
    ret = hash_process(chn, data_addr, data_len, ctrl);
    if (ret != TD_SUCCESS) {
        return CIPHER_ERR_BUSY;
    }

    return TD_SUCCESS;
}

static int drv_spacc_hash_final(uint32_t chn, const uint32_t data_addr,
    const uint32_t data_len, uint8_t *output_hash)
{
    uint32_t i;
    uint32_t tmp;
    volatile int ret;
    struct cipher_digest_context *info = g_digest;
    errno_t err;

    if ((g_hash_info.alg == HASH_ALG_SHA256) || (g_hash_info.alg == HASH_ALG_SM3)) {
        ret = drv_spacc_hash_update(chn, data_addr, data_len);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        for (i = 0; i < (info->digest_len / WORD_WIDTH); i++) {
            reg_set(chn_hash_state_val_addr(chn), i);
            tmp = reg_get(chn_hash_state_val(chn));
            if (memcpy_s(output_hash + (i * WORD_WIDTH), CIPHER_HASH_VAL_SIZA,
       (uint8_t *)&tmp, 4UL) != EOK)
                return TD_FAILURE;
        }
        err = memset_s((uint8_t *)&g_hash_info, sizeof(g_hash_info), 0, sizeof(g_hash_info));
        if (err != EOK) {
            return TD_FAILURE;
        }
    } else {
        call_reset();
    }

    return TD_SUCCESS;
}

int spacc_init(uint32_t key_addr)
{
    volatile int ret;

    ret = drv_spacc_init();
    if (ret != TD_SUCCESS) {
        debug_info("Mcipher init error.\n", 0);
        return ret;
    }

    ret = drv_spacc_alloc_cipher(key_addr, &g_channel_number);
    if (ret != TD_SUCCESS) {
        debug_info("Mcipher alloc channel error.\n", 0);
        return ret;
    }

    return ret;
}

int spacc_deinit(void)
{
    volatile uint32_t ret;
    ret = drv_spacc_release_cipher(g_channel_number);
    drv_spacc_deinit();
    return ret;
}

int calc_sha(uint32_t src_addr, uint32_t src_len, uint8_t *data_sha, uint32_t data_sha_len)
{
    volatile int ret;
    uint32_t channel = 0;
    uint32_t pad_size;
    uint8_t *padding = NULL;
    uint32_t malloc_size;

    ret = drv_spacc_alloc_hash(&channel);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = drv_spacc_hash_init(channel, MODE_HASH, HASH_ALG_SHA256);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    pad_size = src_len % HASH_BLOCK_SIZE;

    ret = drv_spacc_hash_update(channel, src_addr, src_len - pad_size);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    if (pad_size < 56) { /* the minimum of padding size is (56 + 8) */
        malloc_size = HASH_BLOCK_SIZE;
    } else {
        malloc_size = HASH_BLOCK_SIZE * 2; /* padding size should be 2 hash block size */
    }

    padding = gsl_malloc(malloc_size);
    if (padding == NULL) {
        return TD_FAILURE;
    }

    if (memcpy_s(padding, malloc_size, (void *)(uintptr_t)(src_addr + src_len - pad_size),
        pad_size) != EOK) {
        gsl_free(padding);
        padding = NULL;
        return TD_FAILURE;
    }

    pad_size += drv_spacc_hash_padding(padding, malloc_size, pad_size, src_len);

    ret = drv_spacc_hash_final(channel, (uintptr_t)padding, pad_size, data_sha);
    if (ret != TD_SUCCESS) {
        gsl_free(padding);
        padding = NULL;
        return TD_FAILURE;
    }

    gsl_free(padding);
    padding = NULL;

    ret = drv_spacc_release_hash(channel);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/***********************************************************************************
 * Function:      ca_auth
 * Description:   verify sha-256 hash/rsa digitial signature.
 * Input:         rsa_key  rsa key
 *                mem      data to be verified
 *                length   data length
 *                sign     signature of data
 ***********************************************************************************/
int secure_authenticate(const uint8_t * const key, const uint32_t data, const uint32_t len,
    const uint8_t * const signature)
{
    volatile int ret;
    uint8_t data_hash[SHA256_LEN];
    errno_t err;

    err = memset_s(data_hash, sizeof(data_hash), 0x5A, sizeof(data_hash));
    if (err != EOK) {
        return TD_FAILURE;
    }

    ret = calc_sha(data, len, data_hash, sizeof(data_hash));
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = drv_rsa_verify_hash(key, key + RSA_4096_LEN,
                     data_hash, signature);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}
