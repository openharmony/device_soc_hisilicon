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

#include "cryp_symc.h"
#include "drv_osal_lib.h"
#include "drv_symc.h"

/* ************************** Internal Structure Definition ****************** */
/* AES set IV for first package */
#define SYMC_IV_USAGE_ONE_PKG           1

/* AES set IV for first package */
#define SYMC_IV_USAGE_ALL_PKG           2

/* SYMC interrupt level */
#define SYMC_INT_LEVEL (SYMC_MAX_LIST_NUM - 16)  // (100UL)

/* Length of SYMC ccm q  */
#define SYMC_CCM_Q_LEN_2B               2
#define SYMC_CCM_Q_LEN_3B               3
#define SYMC_CCM_Q_LEN_4B               4

/* Length of SYMC ccm P  */
#define SYMC_CCM_P_LEN_2B               0xFFFF
#define SYMC_CCM_P_LEN_3B               0xFFFFFF

/* length range of aead */
#define AES_CCM_MIN_IV_LEN              7
#define AES_CCM_MAX_IV_LEN              13
#define AES_CCM_NQ_LEN                  14
#define AES_CCM_MIN_TAG_LEN             4
#define AES_CCM_MAX_TAG_LEN             16
#define AES_GCM_MIN_IV_LEN              1
#define AES_GCM_MAX_IV_LEN              16
#define AES_GCM_MIN_TAG_LEN             1
#define AES_GCM_MAX_TAG_LEN             16
#define AAD_EXIST                       1
#define AAD_NOT_EXIST                   0
#define N_AND_Q_VAL                     15

/* Multi nodes added status, finished or finished */
#define SYMC_NODES_ADD_FINISHED         0x0a0a0a0a
#define SYMC_NODES_ADD_NOTFINISHED      0x05050505

/* The max tab size of symc function */
#define SYMC_FUNC_TAB_SIZE              (SYMC_ALG_COUNT * SYMC_MODE_COUNT)

/* symc function list */
static symc_func *g_symc_descriptor = TD_NULL;

/* symc context */
static cryp_symc_context *g_symc_context = TD_NULL;

/* symc function register */
static td_void cryp_register_all_symc(td_void);

#ifdef CHIP_AES_CCM_GCM_SUPPORT
static td_u32 cryp_aead_gcm_clen(td_u8 *buf, td_u32 buf_len, td_u32 alen, td_u32 enclen);
#endif

td_s32 cryp_symc_init(td_void)
{
    td_s32 ret;

    func_enter();

    g_symc_descriptor = crypto_calloc(sizeof(symc_func), SYMC_FUNC_TAB_SIZE);
    if (g_symc_descriptor == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    g_symc_context = crypto_calloc(sizeof(cryp_symc_context), CRYPTO_HARD_CHANNEL_MAX);
    if (g_symc_context == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        crypto_free(g_symc_descriptor);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    ret = drv_symc_init();
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_init, ret);
        crypto_free(g_symc_descriptor);
        crypto_free(g_symc_context);
        return ret;
    }

    cryp_register_all_symc();

    func_exit();
    return TD_SUCCESS;
}

td_void cryp_symc_deinit(td_void)
{
    func_enter();

    drv_symc_deinit();

    crypto_free(g_symc_descriptor);

    crypto_free(g_symc_context);

    func_exit();
    return;
}

td_s32 cryp_symc_alloc_chn(td_u32 *hard_chn, ot_cipher_type type)
{
    td_s32 ret;
    td_u32 key[SYMC_KEY_MAX_SIZE_IN_WORD] = {0, 1, 2, 3, 4, 5, 6, 7};

    func_enter();

    /* allocate a aes hard key channel */
    ret = drv_symc_alloc_chn(hard_chn, type);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_alloc_chn, ret);
        return ret;
    }

    /* Set a fake key to clear the true key. */
    drv_symc_set_key(*hard_chn, key, sizeof(key), TD_TRUE);
    drv_symc_set_key(*hard_chn, key, sizeof(key), TD_FALSE);

    func_exit();
    return TD_SUCCESS;
}

td_void cryp_symc_free_chn(td_u32 hard_chn)
{
    td_u32 key[SYMC_KEY_MAX_SIZE_IN_WORD] = {0, 0, 0, 0, 0, 0, 0, 0};

    func_enter();

    drv_symc_set_key(hard_chn, key, sizeof(key), TD_TRUE);
    drv_symc_set_key(hard_chn, key, sizeof(key), TD_FALSE);
    drv_symc_free_chn(hard_chn);

    func_exit();
}

static td_void *cryp_symc_create(td_u32 hard_chn)
{
    cryp_symc_context *ot_ctx = TD_NULL;

    func_enter();

    if (hard_chn >= CRYPTO_HARD_CHANNEL_MAX) {
        ot_cipher_error("invalid hard channel %u\n", hard_chn);
        return TD_NULL;
    }

    ot_ctx = &g_symc_context[hard_chn];
    (td_void)memset_s(ot_ctx, sizeof(cryp_symc_context), 0, sizeof(cryp_symc_context));
    ot_ctx->hard_key = TD_FALSE;
    ot_ctx->hard_chn = hard_chn;

    func_exit();
    return ot_ctx;
}

void cryp_symc_set_key_source(const td_void *ctx, td_u32 keyslot)
{
    const cryp_symc_context *ot_ctx = TD_NULL;
    int i;

    func_enter();

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        /* only valid for hardware alg */
        if (ctx == &g_symc_context[i]) {
            ot_ctx = ctx;
            drv_symc_set_key_source(ot_ctx->hard_chn, keyslot);
            break;
        }
    }

    func_exit();
    return;
}

static td_void cryp_symc_setkey(td_void *ctx, const td_u8 *key, td_u32 keylen, td_u32 odd)
{
    cryp_symc_context *ot_ctx = ctx;

    crypto_unused(odd);
    crypto_unused(key);

    func_enter();

    if (ot_ctx == TD_NULL) {
        ot_cipher_error("Invalid pointer.\n");
        return;
    }

    ot_ctx->klen = keylen;

    func_exit();
    return;
}

static td_void cryp_symc_setiv(td_void *ctx, const td_u8 *iv, td_u32 ivlen, td_u32 usage)
{
    cryp_symc_context *ot_ctx = ctx;

    func_enter();

    if (ot_ctx == TD_NULL) {
        ot_cipher_error("Invalid pointer.\n");
        return;
    }

    if (iv == TD_NULL) {
        if (ivlen != 0) {
            ot_cipher_error("Invalid iv len(%u), iv is null.\n", ivlen);
        }
        return;
    }

    (td_void)memset_s(ot_ctx->iv, sizeof(ot_ctx->iv), 0, sizeof(ot_ctx->iv));
    if (memcpy_s(ot_ctx->iv, AES_IV_SIZE, iv, ivlen) != EOK) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return;
    }
    ot_ctx->iv_usage = usage;
    ot_ctx->ivlen = ivlen;

    func_exit();
    return;
}

static td_s32 cryp_symc_getiv(const td_void *ctx, td_u8 *iv, td_u32 iv_buf_len, td_u32 *ivlen)
{
    const cryp_symc_context *ot_ctx = ctx;

    func_enter();

    if (ot_ctx == TD_NULL) {
        ot_cipher_error("Invalid point.\n");
        return OT_ERR_CIPHER_INVALID_POINT;
    }

    if (iv_buf_len < AES_IV_SIZE) {
        ot_cipher_error("Invalid parameter.\n");
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    if (memcpy_s(iv, iv_buf_len, ot_ctx->iv, ot_ctx->ivlen) != EOK) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }
    *ivlen = ot_ctx->ivlen;

    func_exit();
    return TD_SUCCESS;
}

static td_void cryp_symc_setmode(td_void *ctx, symc_alg alg, symc_mode mode, symc_width width)
{
    cryp_symc_context *ot_ctx = ctx;

    func_enter();

    ot_ctx->mode = mode;
    ot_ctx->alg = alg;
    ot_ctx->width = width;

    func_exit();
    return;
}

static td_s32 symc_add_buf(const cryp_symc_context *ctx, symc_node_usage out_uasge)
{
    td_s32 ret;
    td_u32 cur = ctx->cur_nodes;

    func_enter();

    /* Add P in */
    ret = drv_symc_add_inbuf(ctx->hard_chn, ctx->input_list[cur], ctx->length_list[cur], ctx->usage_list[cur]);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_add_inbuf, ret);
        return ret;
    }

    /* Add P out, only need the last flag */
    ret = drv_symc_add_outbuf(ctx->hard_chn, ctx->output_list[cur], ctx->length_list[cur], out_uasge);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_add_outbuf, ret);
        return ret;
    }

    func_exit();

    return ret;
}

static symc_node_usage symc_get_out_usage(symc_mode mode, td_u32 cur_node, td_u32 total_node)
{
    symc_node_usage usage = SYMC_NODE_USAGE_NORMAL;

    if ((mode != SYMC_MODE_GCM) && ((cur_node + 1) == total_node)) {
        usage = SYMC_NODE_USAGE_LAST;
    }

    return usage;
}

static td_s32 symc_add_next_node(cryp_symc_context *ctx, td_u32 *total, symc_node_usage usage)
{
    td_s32 ret;
    td_u32 cur;
    td_u32 total_len;

    total_len = *total;
    total_len %= ctx->block_size;
    /* Compute the tail length. */
    if (total_len > 0) {
        total_len = ctx->block_size - total_len;
    }

    /* if the total length don't aligned with block size, split joint the follow nodes. */
    while ((total_len > 0) && (ctx->cur_nodes < ctx->total_nodes)) {
        cur = ctx->cur_nodes;

        /* The next node large than tail size, just split it to 2 nodes. */
        if (ctx->length_list[cur] > total_len) {
            /* Add P in. */
            ret = drv_symc_add_inbuf(ctx->hard_chn, ctx->input_list[cur], total_len, ctx->usage_list[cur]);
            if (ret != TD_SUCCESS) {
                print_func_errno(drv_symc_add_inbuf, ret);
                return ret;
            }

            /* Add P out. */
            usage = symc_get_out_usage(ctx->mode, cur, ctx->total_nodes);
            ret = drv_symc_add_outbuf(ctx->hard_chn, ctx->output_list[cur], total_len, usage);
            if (ret != TD_SUCCESS) {
                print_func_errno(drv_symc_add_outbuf, ret);
                return ret;
            }

            /* Let next node skip the tail size. */
            addr_u64(ctx->input_list[cur]) += total_len;
            addr_u64(ctx->output_list[cur]) += total_len;
            ctx->length_list[cur] -= total_len;
            total_len = 0;
        } else {
            /* The next node less than tail size, add it to nodes list. */
            usage = symc_get_out_usage(ctx->mode, cur, ctx->total_nodes);
            ret = symc_add_buf(ctx, usage);    /* Add one node. */
            if (ret != TD_SUCCESS) {
                print_func_errno(symc_add_buf, ret);
                return ret;
            }

            /* re-compute the tail size. */
            total_len -= ctx->length_list[cur];

            /* Process next node. */
            ctx->cur_nodes++;
        }
    }

    *total = total_len;
    return TD_SUCCESS;
}

static td_s32 symc_add_buf_list(cryp_symc_context *ot_ctx)
{
    td_s32 ret;
    td_u32 i;
    td_u32 nodes;
    td_u32 cur;
    td_u32 total_len;
    symc_node_usage usage;

    func_enter();

    /* compute finished. */
    if (ot_ctx->cur_nodes == ot_ctx->total_nodes) {
        func_exit();
        return SYMC_NODES_ADD_FINISHED;
    }

    /* compute not finished.
     * select the minimum numbers of nodes to calculate.
     */
    nodes = crypto_min(SYMC_INT_LEVEL, ot_ctx->total_nodes - ot_ctx->cur_nodes);
    total_len = 0;

    for (i = 0; i < nodes; i++) {
        cur = ot_ctx->cur_nodes;
        usage = symc_get_out_usage(ot_ctx->mode, cur, ot_ctx->total_nodes);

        /* Add one node. */
        ret = symc_add_buf(ot_ctx, usage);
        if (ret != TD_SUCCESS) {
            print_func_errno(symc_add_buf, ret);
            return ret;
        }

        total_len += ot_ctx->length_list[cur];
        ot_ctx->cur_nodes++;
    }

    /* For each compute, the total length of valid nodes list
     * must aligned with block size, otherwise can't recv interrupt,
     * which limit to hardware devising.
     */
    ret = symc_add_next_node(ot_ctx, &total_len, usage);
    if (ret != TD_SUCCESS) {
        print_func_errno(symc_add_next_node, ret);
        return ret;
    }

#ifdef CHIP_AES_CCM_GCM_SUPPORT
    /* gcm add nodes finished ? */
    if ((ot_ctx->mode == SYMC_MODE_GCM) && (ot_ctx->cur_nodes == ot_ctx->total_nodes)) {
        td_u8 clen[AES_BLOCK_SIZE] = {0};

        /* Format the length fields of C and add to nodes list. */
        cryp_aead_gcm_clen(clen, sizeof(clen), ot_ctx->alen, ot_ctx->enclen);
        ret = drv_aead_gcm_add_clen(ot_ctx->hard_chn, clen, sizeof(clen));
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_aead_gcm_add_clen, ret);
            return ret;
        }
    }
#endif

    func_exit();
    return SYMC_NODES_ADD_NOTFINISHED;
}

static symc_klen cryp_symc_key_type(symc_alg alg, td_u32 klen)
{
    symc_klen type;

    if ((alg == SYMC_ALG_AES) && (klen == AES_KEY_192BIT)) {
        type = SYMC_KEY_AES_192BIT;
    } else if ((alg == SYMC_ALG_AES) && (klen == AES_KEY_256BIT)) {
        type = SYMC_KEY_AES_256BIT;
    } else if ((alg == SYMC_ALG_TDES) && (klen == TDES_KEY_128BIT)) {
        type = SYMC_KEY_TDES_2KEY;
    } else {
        type = SYMC_KEY_DEFAULT;
    }

    return type;
}

static td_s32 cryp_symc_config(td_void *ctx, td_u32 decrypt)
{
    cryp_symc_context *ot_ctx = ctx;
    symc_klen type;
    td_s32 ret;

    func_enter();

    ot_cipher_dbg("symc configure, chn %u, alg %d, mode %d, dec %u, klen %u, hard %u, iv len %u, iv usage %u\n",
        ot_ctx->hard_chn, ot_ctx->alg, ot_ctx->mode, decrypt,
        ot_ctx->klen, ot_ctx->hard_key, ot_ctx->ivlen, ot_ctx->iv_usage);

    type = cryp_symc_key_type(ot_ctx->alg, ot_ctx->klen);

    /* configure */
    ret = drv_symc_cfg(ot_ctx, decrypt, type);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_cfg, ret);
        return ret;
    }

    /* set odd key */
    drv_symc_set_key(ot_ctx->hard_chn, ot_ctx->odd_key, sizeof(ot_ctx->odd_key), TD_TRUE);

    /* set even key */
    drv_symc_set_key(ot_ctx->hard_chn, ot_ctx->even_key, sizeof(ot_ctx->even_key), TD_FALSE);

    /* set iv */
    if (ot_ctx->mode == SYMC_MODE_CCM) {
        drv_symc_set_iv(ot_ctx->hard_chn, ot_ctx->iv, AES_IV_SIZE, ot_ctx->iv_usage);
    } else {
        drv_symc_set_iv(ot_ctx->hard_chn, ot_ctx->iv, ot_ctx->ivlen, ot_ctx->iv_usage);
    }

    /* first node must set iv except ecb mode. */
    if (ot_ctx->iv_usage == OT_CIPHER_IV_CHG_ONE_PACK) {
        /* don't set iv any more. */
        ot_ctx->iv_usage = OT_CIPHER_IV_CHG_NONE;
    }

    /* aes/sm4 16bytes block size */
    ot_ctx->block_size = AES_BLOCK_SIZE;

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_symc_crypto_init(cryp_symc_context *ot_ctx,
    td_u32 operation, symc_multi_pack *pack, symc_node_usage usage)
{
    td_u32 i = 0;
    td_s32 ret;

    func_enter();

    /* length of package can't be zero */
    ot_ctx->enclen = 0;
    for (i = 0; i < pack->num; i++) {
        if (pack->len[i] == 0 && ot_ctx->mode != SYMC_MODE_CCM && ot_ctx->mode != SYMC_MODE_GCM) {
            ot_cipher_error("the length of mode %d can't be zero!\n", ot_ctx->mode);
            return OT_ERR_CIPHER_INVALID_PARAM;
        }
        chk_param_err_return(ot_ctx->enclen + pack->len[i] < ot_ctx->enclen);

        ot_ctx->enclen += pack->len[i];
        pack->usage[i] = (td_u32)pack->usage[i] | (td_u32)usage;
    }

    /* configuration parameter */
    ret = cryp_symc_config(ot_ctx, operation);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_symc_config, ret);
        return ret;
    }

    ot_ctx->input_list = pack->in;
    ot_ctx->output_list = pack->out;
    ot_ctx->length_list = pack->len;
    ot_ctx->usage_list = pack->usage;
    ot_ctx->total_nodes = pack->num;
    ot_ctx->cur_nodes = 0;

    /* set isr callback function */
    ret = drv_symc_set_isr_callback(ot_ctx->hard_chn, TD_NULL, TD_NULL);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_set_isr_callback, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_symc_crypto_process(cryp_symc_context *ot_ctx, td_u32 wait)
{
    td_s32 ret;

    crypto_unused(wait);

    func_enter();

    while (symc_add_buf_list(ot_ctx) == SYMC_NODES_ADD_NOTFINISHED) {
        /* start running */
        ret = drv_symc_start(ot_ctx->hard_chn);
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_symc_start, ret);
            return ret;
        }

        /* wait done */
        ret = drv_symc_wait_done(ot_ctx->hard_chn, CRYPTO_TIME_OUT);
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_symc_wait_done, ret);
            return ret;
        }
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_symc_crypto(td_void *ctx, td_u32 operation, symc_multi_pack *pack, td_u32 wait)
{
    td_s32 ret;
    cryp_symc_context *ot_ctx = ctx;

    func_enter();
    chk_ptr_err_return(ot_ctx);
    chk_ptr_err_return(pack);
    chk_ptr_err_return(pack->in);
    chk_ptr_err_return(pack->out);
    chk_ptr_err_return(pack->len);
    chk_ptr_err_return(pack->usage);

    if (ot_ctx->alg == SYMC_ALG_NULL_CIPHER) {
        /* set last flag for each node when DMA copy */
        ot_ctx->iv_usage = OT_CIPHER_IV_CHG_ALL_PACK;
    }

    ret = cryp_symc_crypto_init(ot_ctx, operation, pack, SYMC_NODE_USAGE_NORMAL);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_symc_crypto_init, ret);
        return ret;
    }
    pack->usage[pack->num - 1] = (td_u32)pack->usage[pack->num - 1] | SYMC_NODE_USAGE_LAST;

    /* check the length of nodes list */
    ret = drv_symc_node_check(ot_ctx->alg, ot_ctx->mode, ot_ctx->klen, ot_ctx->block_size, pack);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_node_check, ret);
        return ret;
    }

    ret = cryp_symc_crypto_process(ot_ctx, wait);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_symc_crypto_process, ret);
        return ret;
    }

    drv_symc_get_iv(ot_ctx->hard_chn, ot_ctx->iv, sizeof(ot_ctx->iv));

    func_exit();
    return TD_SUCCESS;
}

#ifdef CHIP_AES_CCM_GCM_SUPPORT
static td_s32 cryp_aead_ccm_setiv(td_void *ctx, const td_u8 *iv, td_u32 ivlen, td_u32 usage)
{
    td_u8 ccm_iv[AES_IV_SIZE] = {0};

    /* The octet lengths of N are denoted  n,
     * The octet length of the binary represen tation of the
     * octet length of the payload denoted q,
     * n is an element of {7, 8, 9, 10, 11, 12, 13}
     * descript: n + q = 15
     * here the string of N  is pConfig->iv, and n is pConfig->ivLen.
     */
    if ((ivlen < AES_CCM_MIN_IV_LEN) || (ivlen > AES_CCM_MAX_IV_LEN)) {
        ot_cipher_error("Invalid ccm iv len, ivlen = 0x%x.\n", ivlen);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    /* Formatting of the Counter Blocks(IV for CTR)
     *
     * According to the CCM spec, the counter is equivalent to
     * a formatting of the counter index i into a complete data block.
     * The counter blocks Ctri are formatted as shown below:
     * | Octet number:  0    1 ... 15-q    16-q ... 15
     * | Contents:     Flags     N             [i]
     * Within each block Ctri, the N is get from pConfig->iv, n + q = 15,
     * so the q equal to 15 - pConfig->ivLen.
     * the [i] is the block conut start with 0,
     * In the Flags field, Bits 0, 1, and 2 contain the encoding of q - 1,
     * others bits shall be set to 0.
     * so the first byte of IV shall be q -1, that is 15 - pConfig->ivLen - 1
     */
    ccm_iv[0] = AES_CCM_NQ_LEN - ivlen; /* descript: IV[0] = q - 1 = 15 - n -1. */
    if (memcpy_s(&ccm_iv[1], sizeof(ccm_iv) - 1, iv, ivlen) != EOK) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }
    ivlen += 1;

    cryp_symc_setiv(ctx, ccm_iv, ivlen, usage);

    return TD_SUCCESS;
}

static td_s32 cryp_aead_gcm_setiv(td_void *ctx, const td_u8 *iv, td_u32 ivlen, td_u32 usage)
{
    if ((ivlen < AES_GCM_MIN_IV_LEN) || (ivlen > AES_GCM_MAX_IV_LEN)) {
        ot_cipher_error("Invalid gcm iv len, ivlen = 0x%x.\n", ivlen);
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    cryp_symc_setiv(ctx, iv, ivlen, usage);

    return TD_SUCCESS;
}

static td_s32 cyp_aead_ccm_n(const cryp_symc_context *ctx, td_u8 *buf, td_u32 buf_len)
{
    td_u32 idx = 0;
    td_u32 q;
    td_u8 *iv = (td_u8 *)ctx->iv;

    func_enter();

    /* Format B0
     * The leading octet of the first block of the formatting, B0,
     * contains four flags for control information: two single bits,
     * called  Reserved  and  Adata, and two strings of three bits,
     * to encode the values t and q.  The encoding of t is [(t -2)/2],
     * and the encoding of q is [ q-1].
     * The ordering of the flags with in the octet is given:
     *  _____________________________________________________
     * |Bit number  7     |   6   | 5  4  3     |  2  1  0   |
     * |Contents  Reserved  Adata   [(t -2)/2] |  [q-1]     |
     *  -----------------------------------------------------
     * The remaining 15 octets of the first block of  the formatting are
     * devoted to the nonce and the binary representation of
     * the message length in  q octets, as given:
     *  _____________________________________________
     * |Octet number  0   | 1 ... 15-q | 16-q ... 15 |
     * |Contents    Flags |      N     |      Q      |
     *  ---------------------------------------------
     */
    (td_void)memset_s(buf, buf_len, 0, buf_len);
    buf[idx]  = ((ctx->alen > 0) ? AAD_EXIST : AAD_NOT_EXIST) << SHIFT_6BITS;  /* descript: Adata exists or not. */
    buf[idx] |= ((ctx->tlen - CRYPTO_NUM_2) / MUL_VAL_2) << SHIFT_3BITS;     /* descript: (t -2) / 2 */
    buf[idx] |= (N_AND_Q_VAL - ctx->ivlen);                                  /* descript: q-1, n+q=15 */
    idx++;

    /* copy N, skip Flags in byte0. */
    if (memcpy_s(&buf[idx], AES_BLOCK_SIZE - idx, &iv[1], ctx->ivlen - 1) != EOK) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }
    idx += ctx->ivlen - 1;

    q = AES_BLOCK_SIZE - idx;

    if (q >= SYMC_CCM_Q_LEN_4B) {
        /* max payload len of 2^32, jump to the location of last word */
        idx = AES_BLOCK_SIZE - SYMC_CCM_Q_LEN_4B;

        buf[idx++] = (td_u8)(ctx->enclen >> SHIFT_24BITS);
        buf[idx++] = (td_u8)(ctx->enclen >> SHIFT_16BITS);
        buf[idx++] = (td_u8)(ctx->enclen >> SHIFT_8BITS);
        buf[idx++] = (td_u8)(ctx->enclen);
    } else if ((q == SYMC_CCM_Q_LEN_3B) && (ctx->enclen <= SYMC_CCM_P_LEN_3B)) {
        /* max payload len of 2^24. */
        buf[idx++] = (td_u8)(ctx->enclen >> SHIFT_16BITS);
        buf[idx++] = (td_u8)(ctx->enclen >> SHIFT_8BITS);
        buf[idx++] = (td_u8)(ctx->enclen);
    } else if ((q == SYMC_CCM_Q_LEN_2B) && (ctx->enclen <= SYMC_CCM_P_LEN_2B)) {
        /* max payload len of 2^16. */
        buf[idx++] = (td_u8)(ctx->enclen >> SHIFT_8BITS);
        buf[idx++] = (td_u8)(ctx->enclen);
    } else {
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_u32 cryp_aead_gcm_clen(td_u8 *buf, td_u32 buf_len, td_u32 alen, td_u32 enclen)
{
    td_u32 idx = 0;

    crypto_unused(buf_len);

    func_enter();

    /* Format len(C), 16 byets, coding in bits.
       Byet0~7:  bits number of Add
       Byet8~15: bits number of P */

    buf[idx++] = 0x00;
    buf[idx++] = 0x00;
    buf[idx++] = 0x00;
    buf[idx++] = (td_u8)((alen >> SHIFT_29BITS) & MAX_LOW_3BITS);
    buf[idx++] = (td_u8)((alen >> SHIFT_21BITS) & MAX_LOW_8BITS);
    buf[idx++] = (td_u8)((alen >> SHIFT_13BITS) & MAX_LOW_8BITS);
    buf[idx++] = (td_u8)((alen >> SHIFT_5BITS) & MAX_LOW_8BITS);
    buf[idx++] = (td_u8)((alen << SHIFT_3BITS) & MAX_LOW_8BITS);

    buf[idx++] = 0x00;
    buf[idx++] = 0x00;
    buf[idx++] = 0x00;
    buf[idx++] = (td_u8)((enclen >> SHIFT_29BITS) & MAX_LOW_3BITS);
    buf[idx++] = (td_u8)((enclen >> SHIFT_21BITS) & MAX_LOW_8BITS);
    buf[idx++] = (td_u8)((enclen >> SHIFT_13BITS) & MAX_LOW_8BITS);
    buf[idx++] = (td_u8)((enclen >> SHIFT_5BITS) & MAX_LOW_8BITS);
    buf[idx++] = (td_u8)((enclen << SHIFT_3BITS) & MAX_LOW_8BITS);

    return idx;
}

static td_s32 cryp_aead_ccm_set_aad(td_void *ctx, compat_addr aad, td_u32 alen, td_u32 tlen)
{
    cryp_symc_context *ot_ctx = ctx;

    func_enter();

    /* the parameter t denotes the octet length of T(tag)
     * t is an element of  { 4, 6, 8, 10, 12, 14, 16}
     * here t is pConfig->u32TagLen
     */
    if ((tlen & 0x01) || (tlen < AES_CCM_MIN_TAG_LEN) || (tlen > AES_CCM_MAX_TAG_LEN)) {
        ot_cipher_error("Invalid tag len, tlen = 0x%x.\n", tlen);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    ot_ctx->aad = aad;
    ot_ctx->alen = alen;
    ot_ctx->tlen = tlen;

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_aead_gcm_set_aad(td_void *ctx, compat_addr aad, td_u32 alen, td_u32 tlen)
{
    cryp_symc_context *ot_ctx = ctx;

    func_enter();

    if ((tlen < AES_GCM_MIN_TAG_LEN) || (tlen > AES_GCM_MAX_TAG_LEN)) {
        ot_cipher_error("Invalid tag len, tlen = 0x%x.\n", tlen);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    ot_ctx->aad = aad;
    ot_ctx->alen = alen;
    ot_ctx->tlen = tlen;

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_aead_crypto_zero(const cryp_symc_context *ot_ctx, td_u32 wait)
{
    td_s32 ret;

    func_enter();

    /* start running */
    ret = drv_symc_start(ot_ctx->hard_chn);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_symc_start, ret);
        return ret;
    }

    /* wait done */
    if (wait == TD_TRUE) {
        ret = drv_symc_wait_done(ot_ctx->hard_chn, CRYPTO_TIME_OUT);
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_symc_wait_done, ret);
            return ret;
        }
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_ccm_add_p(cryp_symc_context *ot_ctx, symc_multi_pack *pack, td_u32 wait)
{
    td_s32 ret;

    if (ot_ctx->enclen == 0) {
        /* Set CCM last flag. */
        drv_symc_add_buf_usage(ot_ctx->hard_chn, TD_TRUE, SYMC_NODE_USAGE_CCM_LAST);
        ret = cryp_aead_crypto_zero(ot_ctx, wait);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_aead_crypto_zero, ret);
            return ret;
        }
    } else {
        /* for CCM, must set P last and ccm last flag */
        pack->usage[pack->num - 1] = (td_u32)pack->usage[pack->num - 1] |
            (SYMC_NODE_USAGE_CCM_LAST | SYMC_NODE_USAGE_LAST);
        ret = cryp_symc_crypto_process(ot_ctx, wait);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_symc_crypto_process, ret);
            return ret;
        }
    }

    return TD_SUCCESS;
}

static td_s32 cryp_aead_ccm_crypto(td_void *ctx, td_u32 operation, symc_multi_pack *pack, td_u32 wait)
{
    td_s32 ret;
    td_u8 n[AES_BLOCK_SIZE] = {0};
    cryp_symc_context *ot_ctx = ctx;

    func_enter();

    ret = cryp_symc_crypto_init(ot_ctx, operation, pack, SYMC_NODE_USAGE_IN_CCM_P | SYMC_NODE_USAGE_FIRST);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_symc_crypto_init, ret);
        return ret;
    }

    /*
     * NIST Special Publication 800-38C
     * The data that CCM protects consists of a message, i.e., a bit string,
     * called the payload, denoted P, of bit length denoted Plen,
     * and a bit string, called the associated data, denoted A.
     * The associated data is optional, i.e., A may be the empty string.
     * CCM provides assurance of the confidentiality of P and assurance of
     * the authenticity of the origin of both A and P;
     * confidentiality is not provided for A.
     * Compute N. */
    ret  = cyp_aead_ccm_n(ot_ctx, n, sizeof(n));
    if (ret != TD_SUCCESS) {
        print_func_errno(cyp_aead_ccm_n, ret);
        return ret;
    }

    /* Compute N */
    ret = drv_aead_ccm_add_n(ot_ctx->hard_chn, n, sizeof(n));
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_aead_ccm_add_n, ret);
        return ret;
    }

    /* Compute A */
    ret = drv_aead_ccm_add_a(ot_ctx->hard_chn, ot_ctx->aad, ot_ctx->alen);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_aead_ccm_add_a, ret);
        return ret;
    }

    /* Compute A */
    ret = cryp_ccm_add_p(ot_ctx, pack, wait);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_ccm_add_p, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_aead_gcm_crypto(td_void *ctx, td_u32 operation, symc_multi_pack *pack, td_u32 wait)
{
    td_s32 ret;
    cryp_symc_context *ot_ctx = ctx;
    td_u8 clen[AES_BLOCK_SIZE] = {0};

    func_enter();

    ret = cryp_symc_crypto_init(ot_ctx, operation, pack, SYMC_NODE_USAGE_IN_GCM_P);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_symc_crypto_init, ret);
        return ret;
    }

    /* NIST Special Publication 800-38D
     * A || P || Clen.
     * Compute A. */
    ret = drv_aead_gcm_add_a(ot_ctx->hard_chn, ot_ctx->aad, ot_ctx->alen);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_aead_gcm_add_a, ret);
        return ret;
    }

    if (ot_ctx->enclen == 0) {
        /* Format the length fields of C and add to nodes list. */
        cryp_aead_gcm_clen(clen, sizeof(clen), ot_ctx->alen, 0x00);
        ret = drv_aead_gcm_add_clen(ot_ctx->hard_chn, clen, sizeof(clen));
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_aead_gcm_add_clen, ret);
            return ret;
        }
        if (ot_ctx->alen == 0) {
            drv_symc_add_buf_usage(ot_ctx->hard_chn, TD_TRUE, SYMC_NODE_USAGE_GCM_FIRST);
        }
        ret = cryp_aead_crypto_zero(ot_ctx, wait);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_aead_crypto_zero, ret);
            return ret;
        }
    } else {
        /* for GCM, must set P last and gcm last flag. */
        if (ot_ctx->alen == 0) {
            pack->usage[0] = (td_u32)pack->usage[0] | SYMC_NODE_USAGE_GCM_FIRST;
        }
        pack->usage[0] = (td_u32)pack->usage[0] | SYMC_NODE_USAGE_FIRST;
        pack->usage[pack->num - BOUND_VAL_1] = (td_u32)pack->usage[pack->num - BOUND_VAL_1] | SYMC_NODE_USAGE_LAST;
        ret = cryp_symc_crypto_process(ot_ctx, wait);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_symc_crypto_process, ret);
            return ret;
        }
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_aead_get_tag(const td_void *ctx, td_u32 tag[AEAD_TAG_SIZE_IN_WORD], td_u32 *taglen)
{
    td_s32 ret;
    const cryp_symc_context *ot_ctx = ctx;

    func_enter();

    chk_param_err_return(*taglen < ot_ctx->tlen);

    ot_cipher_dbg("tag buffer len %u, tag len %u\n", *taglen,  ot_ctx->tlen);

    *taglen = ot_ctx->tlen;

    ret = drv_aead_get_tag(ot_ctx->hard_chn, tag, ot_ctx->tlen);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_aead_get_tag, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}
#endif

static td_s32 cryp_register_symc(const symc_func *func)
{
    td_u32 i;

    func_enter();

    /* check availability */
    if ((func->create == TD_NULL) || (func->crypto == TD_NULL)) {
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    /* is it already registered? */
    for (i = 0; i < SYMC_FUNC_TAB_SIZE; i++) {
        if ((g_symc_descriptor[i].valid) && (g_symc_descriptor[i].alg == func->alg) &&
            (g_symc_descriptor[i].mode == func->mode)) {
            func_exit();
            return TD_SUCCESS;
        }
    }

    /* find a blank spot */
    for (i = 0; i < SYMC_FUNC_TAB_SIZE; i++) {
        if (!g_symc_descriptor[i].valid) {
            (td_void)memcpy_s(&g_symc_descriptor[i], sizeof(symc_func), func, sizeof(symc_func));
            g_symc_descriptor[i].valid = TD_TRUE;
            ot_cipher_dbg("g_symc_descriptor[%u], alg %d, mode %d\n", i,
                g_symc_descriptor[i].alg, g_symc_descriptor[i].mode);

            func_exit();
            return TD_SUCCESS;
        }
    }

    /* Can't find a blank spot */
    print_errno(OT_ERR_CIPHER_OVERFLOW);
    return OT_ERR_CIPHER_OVERFLOW;
}

static symc_func *cryp_get_symc(td_u32 alg, td_u32 mode)
{
    td_u32 i;

    func_enter();

    /* find the valid function */
    for (i = 0; i < SYMC_FUNC_TAB_SIZE; i++) {
        ot_cipher_dbg("g_symc_descriptor[%u] valid %u, alg %d, mode %d \n",
            i, g_symc_descriptor[i].valid, g_symc_descriptor[i].alg, g_symc_descriptor[i].mode);

        if (g_symc_descriptor[i].valid) {
            if (g_symc_descriptor[i].alg == alg && g_symc_descriptor[i].mode == mode) {
                return &g_symc_descriptor[i];
            }
        }
    }

    func_exit();
    return TD_NULL;
}

/*
 * Defined the default template of Symmetric cipher function,
 * the function can be replaced by other engine
 */
static td_s32 cryp_aes_setkey(td_void *ctx, td_u32 *ot_klen)
{
    td_u32 klen;
    cryp_symc_context *ot_ctx = ctx;

    func_enter();
    chk_ptr_err_return(ctx);

    /* set the key length depend on alg
     * des/3des support 2key and 3key
     * aes support 128, 192, and 256
     * sm1 support ak/ek/sk
     * sm4 support 128
     */
    switch (*ot_klen) {
        case OT_CIPHER_KEY_AES_128BIT: {
            klen = AES_KEY_128BIT;
            break;
        }
        case OT_CIPHER_KEY_AES_192BIT: {
            klen = AES_KEY_192BIT;
            break;
        }
        case OT_CIPHER_KEY_AES_256BIT: {
            klen = AES_KEY_256BIT;
            break;
        }
        default: {
            ot_cipher_error("Invalid aes key len: %u, key len should be in [%d, %d]!\n", *ot_klen,
                OT_CIPHER_KEY_AES_128BIT, OT_CIPHER_KEY_AES_256BIT);
            return OT_ERR_CIPHER_INVALID_PARAM;
        }
    }
    ot_cipher_dbg("key len %u, type %u\n", klen, *ot_klen);

    *ot_klen = klen;
    cryp_symc_setkey(ctx, TD_NULL, klen, TD_FALSE);

    ot_ctx->hard_key = TD_TRUE;

    func_exit();
    return TD_SUCCESS;
}

#ifdef CHIP_SM4_SUPPORT
static td_s32 cryp_sm4_setkey(td_void *ctx, td_u32 *ot_klen)
{
    cryp_symc_context *ot_ctx = ctx;

    func_enter();
    chk_ptr_err_return(ot_klen);
    chk_param_err_return(*ot_klen != OT_CIPHER_KEY_DEFAULT);
    chk_ptr_err_return(ot_ctx);

    /* sm4 support 128 */
    cryp_symc_setkey(ctx, TD_NULL, SM4_KEY_SIZE, TD_FALSE);

    ot_ctx->hard_key = TD_TRUE;
    *ot_klen = SM4_KEY_SIZE;

    func_exit();
    return TD_SUCCESS;
}
#endif

static td_s32 cryp_symc_setiv_default(td_void *ctx, const td_u8 *iv, td_u32 ivlen, td_u32 usage)
{
    func_enter();

    if (iv == TD_NULL) {
        return TD_SUCCESS;
    }

    if (ivlen > AES_IV_SIZE) {
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    cryp_symc_setiv(ctx, iv, ivlen, usage);

    func_exit();
    return TD_SUCCESS;
}

/* Default As AES */
static td_void cryp_register_symc_default(symc_func *func, symc_alg alg, symc_mode mode)
{
    (td_void)memset_s(func, sizeof(symc_func), 0, sizeof(symc_func));

    func->mode = mode;
    func->alg = alg;
    func->create = cryp_symc_create;
    func->setiv = cryp_symc_setiv_default;
    func->getiv = cryp_symc_getiv;
    func->crypto = cryp_symc_crypto;
    func->setmode = cryp_symc_setmode;
    func->setkey = cryp_aes_setkey;
    func->waitdone = TD_NULL;
    return;
}

static td_void cryp_register_symc_aes(td_u32 capacity, symc_mode mode)
{
    symc_func func;
    td_s32 ret;
    (td_void)memset_s(&func, sizeof(symc_func), 0, sizeof(symc_func));

    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
        cryp_register_symc_default(&func, SYMC_ALG_AES, mode);
        ret = cryp_register_symc(&func);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_register_symc, ret);
            return;
        }
    }
    return;
}

static td_void cryp_register_symc_dma(td_u32 dma_capacity, td_u32 tdes_capacity)
{
    symc_func func;
    td_s32 ret;

    crypto_unused(tdes_capacity);

    (td_void)memset_s(&func, sizeof(func), 0, sizeof(func));

    if (dma_capacity == CRYPTO_CAPACITY_SUPPORT) {
        func.mode = SYMC_MODE_ECB;
        func.alg = SYMC_ALG_NULL_CIPHER;
        func.create = cryp_symc_create;
        func.setmode = cryp_symc_setmode;
        func.crypto = cryp_symc_crypto;
        ret = cryp_register_symc(&func);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_register_symc, ret);
            return;
        }
    }
    return;
}

#ifdef CHIP_AES_CCM_GCM_SUPPORT
static td_void cryp_register_aead_ccm(td_u32 capacity, symc_mode mode)
{
    symc_func func;
    crypto_unused(mode);
    (td_void)memset_s(&func, sizeof(func), 0, sizeof(func));

    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
#ifdef CHIP_AES_CCM_GCM_SUPPORT
        td_s32 ret;

        cryp_register_symc_default(&func, SYMC_ALG_AES, mode);
        func.setadd = cryp_aead_ccm_set_aad;
        func.gettag = cryp_aead_get_tag;
        func.crypto = cryp_aead_ccm_crypto;
        func.setiv = cryp_aead_ccm_setiv;
        ret = cryp_register_symc(&func);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_register_symc, ret);
            return;
        }
#endif
    }

    return;
}

static td_void cryp_register_aead_gcm(td_u32 capacity, symc_mode mode)
{
    symc_func func;
    crypto_unused(mode);
    (td_void)memset_s(&func, sizeof(symc_func), 0, sizeof(symc_func));

    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
#ifdef CHIP_AES_CCM_GCM_SUPPORT
        td_s32 ret;

        cryp_register_symc_default(&func, SYMC_ALG_AES, mode);
        func.setadd = cryp_aead_gcm_set_aad;
        func.gettag = cryp_aead_get_tag;
        func.crypto = cryp_aead_gcm_crypto;
        func.setiv = cryp_aead_gcm_setiv;
        ret = cryp_register_symc(&func);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_register_symc, ret);
            return;
        }
#endif
    }

    return;
}
#endif

#ifdef CHIP_SM4_SUPPORT
static td_void cryp_register_symc_sm4(td_u32 capacity, symc_mode mode)
{
    symc_func func;
    (td_void)memset_s(&func, sizeof(symc_func), 0, sizeof(symc_func));

    if (capacity == CRYPTO_CAPACITY_SUPPORT) {
        td_s32 ret;
        cryp_register_symc_default(&func, SYMC_ALG_SM4, mode);
        func.setkey = cryp_sm4_setkey;
        ret = cryp_register_symc(&func);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_register_symc, ret);
            return;
        }
    }
    return;
}
#endif

/* symc function register */
static td_void cryp_register_all_symc(td_void)
{
    symc_capacity capacity;

    func_enter();

    (td_void)memset_s(&capacity, sizeof(capacity), 0, sizeof(capacity));

    /* register the symc function if supported */
    drv_symc_get_capacity(&capacity);

    /* AES */
    cryp_register_symc_aes(capacity.aes_ecb, SYMC_MODE_ECB);
    cryp_register_symc_aes(capacity.aes_cbc, SYMC_MODE_CBC);
    cryp_register_symc_aes(capacity.aes_cfb, SYMC_MODE_CFB);
    cryp_register_symc_aes(capacity.aes_ofb, SYMC_MODE_OFB);
    cryp_register_symc_aes(capacity.aes_ctr, SYMC_MODE_CTR);
    cryp_register_symc_dma(capacity.dma, capacity.tdes_ecb);

    /* AEAD */
#ifdef CHIP_AES_CCM_GCM_SUPPORT
    cryp_register_aead_ccm(capacity.aes_ccm, SYMC_MODE_CCM);
    cryp_register_aead_gcm(capacity.aes_gcm, SYMC_MODE_GCM);
#endif

    /* SM4 */
#ifdef CHIP_SM4_SUPPORT
    cryp_register_symc_sm4(capacity.sm4_ecb, SYMC_MODE_ECB);
    cryp_register_symc_sm4(capacity.sm4_cbc, SYMC_MODE_CBC);
    cryp_register_symc_sm4(capacity.sm4_cfb, SYMC_MODE_CFB);
    cryp_register_symc_sm4(capacity.sm4_ofb, SYMC_MODE_OFB);
    cryp_register_symc_sm4(capacity.sm4_ctr, SYMC_MODE_CTR);
#endif

    func_exit();

    return;
}

static td_s32 cryp_get_symc_mode(ot_cipher_work_mode mode, symc_mode *cryp_mode)
{
    switch (mode) {
        case OT_CIPHER_WORK_MODE_ECB:
            *cryp_mode = SYMC_MODE_ECB;
            break;
        case OT_CIPHER_WORK_MODE_CBC:
            *cryp_mode = SYMC_MODE_CBC;
            break;
        case OT_CIPHER_WORK_MODE_CFB:
            *cryp_mode = SYMC_MODE_CFB;
            break;
        case OT_CIPHER_WORK_MODE_OFB:
            *cryp_mode = SYMC_MODE_OFB;
            break;
        case OT_CIPHER_WORK_MODE_CTR:
            *cryp_mode = SYMC_MODE_CTR;
            break;
        case OT_CIPHER_WORK_MODE_CCM:
            *cryp_mode = SYMC_MODE_CCM;
            break;
        case OT_CIPHER_WORK_MODE_GCM:
            *cryp_mode = SYMC_MODE_GCM;
            break;
        default:
            ot_cipher_error("Invalid mode, mode = 0x%x. mode should be in [%d, %d]!\n",
                mode, OT_CIPHER_WORK_MODE_ECB, OT_CIPHER_WORK_MODE_GCM);
            return OT_ERR_CIPHER_INVALID_PARAM;
    }
    return TD_SUCCESS;
}

symc_func *cryp_get_symc_op(ot_cipher_alg alg, ot_cipher_work_mode mode)
{
    td_s32 ret;
    symc_alg cryp_alg = SYMC_ALG_COUNT;
    symc_mode cryp_mode = SYMC_MODE_COUNT;

    switch (alg) {
        case OT_CIPHER_ALG_AES:
            cryp_alg = SYMC_ALG_AES;
            break;
#ifdef CHIP_SM4_SUPPORT
        case OT_CIPHER_ALG_SM4:
            cryp_alg = SYMC_ALG_SM4;
            break;
#endif
        case OT_CIPHER_ALG_DMA:
            cryp_alg = SYMC_ALG_NULL_CIPHER;
            mode = OT_CIPHER_WORK_MODE_ECB;
            break;
        default:
            ot_cipher_error("Unsupported alg, alg = 0x%x. should be in [%d, %d]!\n",
                alg, OT_CIPHER_ALG_AES, OT_CIPHER_ALG_DMA);
            return TD_NULL;
    }

    ret = cryp_get_symc_mode(mode, &cryp_mode);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_get_symc_mode, ret);
        return TD_NULL;
    }

    return cryp_get_symc(cryp_alg, cryp_mode);
}
