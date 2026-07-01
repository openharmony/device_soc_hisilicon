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

#include "drv_osal_lib.h"
#include "cryp_symc.h"
#include "drv_ks.h"
#include "cipher_ext.h"

/* max number of nodes */
#define MAX_PKG_NUMBER                  100000

/* max length of CCM/GCM AAD */
#define MAX_AEAD_A_LEN                  0x100000

typedef struct {
    td_u32 open : 1;                    /* open or close */
    td_u32 config : 1;                  /* aleardy config or not */
    td_u32 attach : 1;                  /* aleardy config or not */
    symc_func *func;
    td_void *cryp_ctx;                  /* Context of cryp instance */
    CRYPTO_OWNER owner;                 /* user ID */
    ot_cipher_ctrl ctrl;                /* control information */
    td_u32 key_slot;
} kapi_symc_ctx;

/*! Context of cipher */
static kapi_symc_ctx *g_symc_ctx = TD_NULL;

/* keyslot open or close */
static td_bool g_keyslot_open[KEYSLOT_CHANNEL_MAX];
static td_bool g_keyslot_owner[KEYSLOT_CHANNEL_MAX];

/* symc mutex */
static CRYPTO_MUTEX g_symc_mutex;

#define kapi_symc_lock_err_return()                         \
    do {                                                    \
        ret = crypto_mutex_lock(&g_symc_mutex);             \
        if (ret != TD_SUCCESS) {                            \
            ot_cipher_error("error, symc lock failed\n");      \
            print_func_errno(crypto_mutex_lock, ret);  \
            return ret;                                     \
        }                                                   \
    } while (0)

#define kapi_symc_unlock()          crypto_mutex_unlock(&g_symc_mutex)
#define AES_CCM_MIN_TAG_LEN         4
#define AES_CCM_MAX_TAG_LEN         16
#define AES_GCM_MIN_TAG_LEN         1
#define AES_GCM_MAX_TAG_LEN         16

static td_s32 _kapi_symc_keyslot_release(td_void)
{
    td_s32 ret;
    td_u32 i;
    CRYPTO_OWNER owner;
    func_enter();
    kapi_symc_lock_err_return();
    crypto_get_owner(&owner);
    for (i = 0; i < KEYSLOT_CHANNEL_MAX; i++) {
        if (g_keyslot_open[i] == TD_TRUE) {
            if (owner != g_keyslot_owner[i]) {
                continue;
            }
            ret = drv_kc_slot_unlock(i);
            if (ret != TD_SUCCESS) {
                print_func_errno(drv_kc_slot_unlock, ret);
                kapi_symc_unlock();
                return ret;
            }
            g_keyslot_open[i] = TD_FALSE;
        }
    }
    kapi_symc_unlock();
    func_exit();
    return ret;
}

static td_s32 _kapi_symc_keyslot_release_all(td_void)
{
    td_s32 ret;
    td_u32 i;
    func_enter();
    kapi_symc_lock_err_return();
    for (i = 0; i < KEYSLOT_CHANNEL_MAX; i++) {
        ret = drv_kc_slot_unlock(i);
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_kc_slot_unlock, ret);
            kapi_symc_unlock();
            return ret;
        }
        g_keyslot_open[i] = TD_FALSE;
    }
    kapi_symc_unlock();
    func_exit();
    return ret;
}

/* ****************************** API Code **************************** */
static td_s32 kapi_symc_chk_handle(td_handle handle)
{
    chk_handle_modid(handle, OT_ID_CIPHER);
    chk_handle_private_data(handle, OT_PRIVATE_ID_SYMC);
    chk_handle_chnid(handle, CRYPTO_HARD_CHANNEL_MAX);

    if (g_symc_ctx[td_handle_get_chnid(handle)].open == TD_FALSE) {
        ot_cipher_error("channel %u is not open\n", td_handle_get_chnid(handle));
        return OT_ERR_CIPHER_INVALID_HANDLE;
    }

    return TD_SUCCESS;
}

td_s32 kapi_symc_init(void)
{
    td_s32 ret;

    func_enter();

    crypto_mutex_init(&g_symc_mutex);

    g_symc_ctx = crypto_calloc(sizeof(kapi_symc_ctx), CRYPTO_HARD_CHANNEL_MAX);
    if (g_symc_ctx == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        crypto_mutex_destroy(&g_symc_mutex);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    ret = cryp_symc_init();
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_symc_init, ret);
        crypto_mutex_destroy(&g_symc_mutex);
        crypto_free(g_symc_ctx);
        g_symc_ctx = TD_NULL;
        return ret;
    }

    ret = drv_kc_slot_init();
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_init, ret);
        crypto_mutex_destroy(&g_symc_mutex);
        cryp_symc_deinit();
        crypto_free(g_symc_ctx);
        g_symc_ctx = TD_NULL;
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 kapi_symc_release_all(void)
{
    td_u32 i;
    td_u32 chn;
    td_s32 ret;
    kapi_symc_ctx *ctx = TD_NULL;
    CRYPTO_OWNER owner;
    td_u32 soft_id;

    func_enter();

    crypto_get_owner(&owner);

    /* destroy all channels */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        ctx = &g_symc_ctx[i];
        if (ctx->open == TD_FALSE) {
            continue;
        }
        chn = td_handle_init(OT_ID_CIPHER, OT_PRIVATE_ID_SYMC, i);
        soft_id = td_handle_get_chnid(chn);
        ot_cipher_dbg("symc release chn 0x%x\n", chn);
        cryp_symc_free_chn(soft_id);
        /* Destroy the attached instance of Symmetric cipher engine */
        if ((ctx->func != TD_NULL) && (ctx->func->destroy != TD_NULL)) {
            ret = ctx->func->destroy(ctx->cryp_ctx);
            if (ret != TD_SUCCESS) {
                print_func_errno(ctx->func->destroy, ret);
                kapi_symc_unlock();
                return ret;
            }
            ctx->cryp_ctx = TD_NULL;
        }

        ctx->open = TD_FALSE;
    }
    /* destroy all locked keyslot by current process. */
    ret = _kapi_symc_keyslot_release_all();
    if (ret != TD_SUCCESS) {
        print_func_errno(_kapi_symc_keyslot_release_all, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_symc_deinit(void)
{
    func_enter();

    kapi_symc_release_all();
    drv_kc_slot_deinit();

    cryp_symc_deinit();

    crypto_mutex_destroy(&g_symc_mutex);
    crypto_free(g_symc_ctx);
    g_symc_ctx = TD_NULL;

    func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_symc_release(void)
{
    td_u32 i;
    td_u32 chn;
    td_s32 ret;
    kapi_symc_ctx *ctx = TD_NULL;
    CRYPTO_OWNER owner;
    td_u32 soft_id;
    func_enter();

    crypto_get_owner(&owner);

    /* destroy the channel which are created by current user */
    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        ctx = &g_symc_ctx[i];
        if (ctx->open == TD_FALSE) {
            continue;
        }
        if (owner != ctx->owner) {
            continue;
        }
        chn = td_handle_init(OT_ID_CIPHER, OT_PRIVATE_ID_SYMC, i);
        soft_id = td_handle_get_chnid(chn);
        ot_cipher_dbg("symc release chn 0x%x\n", chn);
        cryp_symc_free_chn(soft_id);
        /* Destroy the attached instance of Symmetric cipher engine */
        if ((ctx->func != TD_NULL) && (ctx->func->destroy != TD_NULL)) {
            ret = ctx->func->destroy(ctx->cryp_ctx);
            if (ret != TD_SUCCESS) {
                kapi_symc_unlock();
                return ret;
            }
            ctx->cryp_ctx = TD_NULL;
        }

        ctx->open = TD_FALSE;
    }
    /* destroy all locked keyslot by current process. */
    ret = _kapi_symc_keyslot_release();
    if (ret != TD_SUCCESS) {
        print_func_errno(_kapi_symc_keyslot_release, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_symc_create(td_u32 *id, ot_cipher_type type)
{
    td_s32 ret;
    td_u32 chn = 0;
    kapi_symc_ctx *ctx = TD_NULL;

    func_enter();

    kapi_symc_lock_err_return();

    /* allocate a aes soft channel for hard channel allocated */
    ret = cryp_symc_alloc_chn(&chn, type);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_symc_alloc_chn, ret);
        kapi_symc_unlock();
        return ret;
    }
    ctx = &g_symc_ctx[chn];

    (td_void)memset_s(ctx, sizeof(kapi_symc_ctx), 0, sizeof(kapi_symc_ctx));
    crypto_get_owner(&ctx->owner);

    *id = td_handle_init(OT_ID_CIPHER, OT_PRIVATE_ID_SYMC, chn);
    ctx->open = TD_TRUE;
    ctx->config = TD_FALSE;
    ctx->attach = TD_FALSE;

    ot_cipher_dbg("[SYMC] create handle 0x%x, owner 0x%x\n", *id, ctx->owner);

    kapi_symc_unlock();

    func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_symc_destroy(td_u32 id)
{
    td_s32 ret;
    kapi_symc_ctx *ctx = TD_NULL;
    td_u32 soft_id;

    func_enter();

    ret = kapi_symc_chk_handle((td_handle)id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_chk_handle, ret);
        return ret;
    }

    soft_id = td_handle_get_chnid(id);
    ctx = &g_symc_ctx[soft_id];
    crypto_chk_owner_err_return(&ctx->owner);
    if (ctx->ctrl.alg != OT_CIPHER_ALG_DMA) {
        chk_param_err_return(ctx->attach == TD_TRUE);
    }

    kapi_symc_lock_err_return();

    cryp_symc_free_chn(soft_id);

    /* Destroy the attached instance of Symmetric cipher engine */
    if ((ctx->func != TD_NULL) && (ctx->func->destroy != TD_NULL)) {
        ret = ctx->func->destroy(ctx->cryp_ctx);
        if (ret != TD_SUCCESS) {
            print_func_errno(ctx->func->destroy, ret);
            kapi_symc_unlock();
            return ret;
        }
        ctx->cryp_ctx = TD_NULL;
    }

    ctx->open = TD_FALSE;

    ot_cipher_dbg("[SYMC] destroy handle 0x%x, owner 0x%x, keyslot 0x%x\n", id, ctx->owner, ctx->key_slot);

    kapi_symc_unlock();

    func_exit();
    return TD_SUCCESS;
}

static td_s32 kapi_symc_chk_aes_param(ot_cipher_alg alg, ot_cipher_work_mode mode, td_u32 width)
{
    if ((mode == OT_CIPHER_WORK_MODE_CFB) && (width != SYMC_DAT_WIDTH_1) &&
        (width != SYMC_DAT_WIDTH_8) && (width != SYMC_DAT_WIDTH_128)) {
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    if ((mode == OT_CIPHER_WORK_MODE_OFB) && (width != SYMC_DAT_WIDTH_128)) {
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

#ifdef CHIP_SM4_SUPPORT
static td_s32 kapi_symc_chk_sm4_param(ot_cipher_alg alg, ot_cipher_work_mode mode)
{
    if ((mode != OT_CIPHER_WORK_MODE_ECB) &&
        (mode != OT_CIPHER_WORK_MODE_CBC) && (mode != OT_CIPHER_WORK_MODE_CTR)) {
        ot_cipher_error("Invalid alg %d and mode %d\n", alg, mode);
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}
#endif

static td_s32 kapi_symc_width_check(const symc_cfg_t *cfg, td_u32 width)
{
    td_s32 ret;

    /* the bit width depend on alg and mode, which limit to hardware
     * aes with cfb/ofb only support bit128.
     * sm1/sm4 with ofb only support bit128, cfb support bit1, bit8, bit 128.
     */
    func_enter();

    chk_param_err_return(cfg->alg >= OT_CIPHER_ALG_BUTT);
    chk_param_err_return((cfg->alg != OT_CIPHER_ALG_DMA) && (cfg->mode >= OT_CIPHER_WORK_MODE_BUTT));
    chk_param_err_return((cfg->alg != OT_CIPHER_ALG_DMA) && (width >= SYMC_DAT_WIDTH_COUNT));

    if (cfg->alg == OT_CIPHER_ALG_AES) {
        ret = kapi_symc_chk_aes_param(cfg->alg, cfg->mode, width);
        if (ret != TD_SUCCESS) {
            print_func_errno(kapi_symc_chk_aes_param, ret);
            return ret;
        }
#ifdef CHIP_SM4_SUPPORT
    } else if (cfg->alg == OT_CIPHER_ALG_SM4) {
        ret = kapi_symc_chk_sm4_param(cfg->alg, cfg->mode);
        if (ret != TD_SUCCESS) {
            print_func_errno(kapi_symc_chk_sm4_param, ret);
            return ret;
        }
#endif
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 kapi_symc_match_width(ot_cipher_work_mode work_mode, ot_cipher_bit_width bit_width, symc_width *width)
{
    func_enter();

    /* set the bit width which depend on alg and mode */
    if (work_mode == OT_CIPHER_WORK_MODE_CFB) {
        switch (bit_width) {
            case OT_CIPHER_BIT_WIDTH_128BIT:
                *width = SYMC_DAT_WIDTH_128;
                break;
            case OT_CIPHER_BIT_WIDTH_8BIT:
                *width = SYMC_DAT_WIDTH_8;
                break;
            case OT_CIPHER_BIT_WIDTH_1BIT:
                *width = SYMC_DAT_WIDTH_1;
                break;
            default:
                ot_cipher_error("Invalid width: 0x%x, mode 0x%x, bit_width should be in [%d, %d]\n",
                    bit_width, work_mode, OT_CIPHER_BIT_WIDTH_128BIT, OT_CIPHER_BIT_WIDTH_1BIT);
                print_errno(OT_ERR_CIPHER_INVALID_PARAM);
                return OT_ERR_CIPHER_INVALID_PARAM;
        }
    } else {
        if (bit_width == OT_CIPHER_BIT_WIDTH_128BIT) {
            *width = SYMC_DAT_WIDTH_128;
        } else {
            ot_cipher_error("Invalid width: 0x%x, mode 0x%x, bit_width should be %d!\n",
                bit_width, work_mode, OT_CIPHER_BIT_WIDTH_128BIT);
            print_errno(OT_ERR_CIPHER_INVALID_PARAM);
            return OT_ERR_CIPHER_INVALID_PARAM;
        }
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 kapi_symc_chk_param(const symc_cfg_t *cfg, symc_width *width)
{
    td_s32 ret;

    func_enter();

    if (cfg->alg == OT_CIPHER_ALG_DMA) {
        ot_cipher_dbg("Alg is DMA.\n");
        return TD_SUCCESS;
    }

    if (cfg->klen >= OT_CIPHER_KEY_LEN_BUTT) {
        ot_cipher_error("Invalid key len: 0x%x\n", cfg->klen);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    /* set the bit width which depend on alg and mode */
    ret = kapi_symc_match_width(cfg->mode, cfg->width, width);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_match_width, ret);
        return ret;
    }

    ret = kapi_symc_width_check(cfg, *width);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_width_check, ret);
        return ret;
    }

    if (cfg->iv_usage > OT_CIPHER_IV_CHG_ALL_PACK) {
        ot_cipher_error("Invalid IV Change Flags: 0x%x\n", cfg->iv_usage);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    func_exit();
    return TD_SUCCESS;
}

#ifdef CHIP_AES_CCM_GCM_SUPPORT
static td_s32 kapi_symc_check_ccm_gcm_taglen(ot_cipher_alg alg, ot_cipher_work_mode work_mode, td_u32 tlen)
{
    if (work_mode == OT_CIPHER_WORK_MODE_CCM) {
        /* the parameter t denotes the octet length of T(tag)
         * t is an element of  { 4, 6, 8, 10, 12, 14, 16}
         * here t is pConfig->u32TagLen
         */
        if ((tlen & 0x01) || (tlen < AES_CCM_MIN_TAG_LEN) || (tlen > AES_CCM_MAX_TAG_LEN)) {
            ot_cipher_error("Invalid ccm tag len, tlen = 0x%x.\n", tlen);
            return OT_ERR_CIPHER_INVALID_PARAM;
        }
    } else if (work_mode == OT_CIPHER_WORK_MODE_GCM) {
        if ((tlen < AES_GCM_MIN_TAG_LEN) || (tlen > AES_GCM_MAX_TAG_LEN)) {
            ot_cipher_error("Invalid gcm tag len, tlen = 0x%x.\n", tlen);
            return OT_ERR_CIPHER_INVALID_PARAM;
        }
    } else {
        ot_cipher_error("Aes with invalid work mode 0x%x for check tag length.\n", work_mode);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}
#endif

static td_s32 kapi_symc_set_key_iv(const symc_cfg_t *cfg, const kapi_symc_ctx *ctx, td_u32 *klen)
{
    td_s32 ret;

    /* Set even key, may be also need set odd key */
    switch (cfg->iv_usage) {
        case OT_CIPHER_IV_CHG_ONE_PACK:
        case OT_CIPHER_IV_CHG_ALL_PACK:
            if (ctx->func->setiv != TD_NULL) {
                ret = ctx->func->setiv(ctx->cryp_ctx, cfg->iv, cfg->ivlen, cfg->iv_usage);
                if (ret != TD_SUCCESS) {
                    print_func_errno(ctx->func->setiv, ret);
                    return ret;
                }
            }
            fallthrough;
            /* fall through */
        case OT_CIPHER_IV_CHG_NONE:
            if (ctx->func->setkey != TD_NULL) {
                ret = ctx->func->setkey(ctx->cryp_ctx, klen);
                if (ret != TD_SUCCESS) {
                    print_func_errno(ctx->func->setkey, ret);
                    return ret;
                }
                cryp_symc_set_key_source(ctx->cryp_ctx, td_handle_get_chnid(ctx->key_slot));
            }
            break;
        default:
            print_errno(OT_ERR_CIPHER_INVALID_PARAM);
            return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 kapi_symc_save_ctrl(const symc_cfg_t *cfg, ot_cipher_ctrl *ctrl)
{
    td_u32 *iv = TD_NULL;

    (td_void)memset_s(ctrl, sizeof(ot_cipher_ctrl), 0, sizeof(ot_cipher_ctrl));
    ctrl->alg = cfg->alg;
    ctrl->work_mode = cfg->mode;

    if (ctrl->alg == OT_CIPHER_ALG_AES) {
        if (ctrl->work_mode == OT_CIPHER_WORK_MODE_CCM ||
            ctrl->work_mode == OT_CIPHER_WORK_MODE_GCM) {
            ot_cipher_ctrl_aes_ccm_gcm *aes_ccm_gcm = &ctrl->aes_ccm_gcm_ctrl;
            aes_ccm_gcm->aad_len = cfg->alen;
            aes_ccm_gcm->iv_len = cfg->ivlen;
            aes_ccm_gcm->key_len = cfg->klen;
            aes_ccm_gcm->tag_len = cfg->tlen;
            aes_ccm_gcm->aad_phys_addr = addr_u64(cfg->aad);
            iv = aes_ccm_gcm->iv;
        } else {
            ot_cipher_ctrl_aes *aes = &ctrl->aes_ctrl;
            aes->bit_width = cfg->width;
            aes->chg_flags = cfg->iv_usage;
            aes->key_len = cfg->klen;
            iv = aes->iv;
        }
    } else if (ctrl->alg == OT_CIPHER_ALG_SM4) {
#ifdef CHIP_SM4_SUPPORT
        ot_cipher_ctrl_sm4 *sm4 = &ctrl->sm4_ctrl;
        sm4->chg_flags = cfg->iv_usage;
        iv = sm4->iv;
#endif
    }
    if (iv != TD_NULL) {
        chk_param_err_return(cfg->ivlen > AES_IV_SIZE);
        if (memcpy_s(iv, AES_IV_SIZE, cfg->iv, cfg->ivlen) != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            return OT_ERR_CIPHER_FAILED_SEC_FUNC;
        }
    }

    return TD_SUCCESS;
}

static td_s32 kapi_symc_unlock_cfg(td_u32 soft_id,
    const symc_cfg_t *cfg, kapi_symc_ctx *ctx, symc_width width)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 klen = cfg->klen;

    /* Destroy the last attached instance of Symmetric cipher engine. */
    if ((ctx->func != TD_NULL) && (ctx->func->destroy != TD_NULL)) {
        (void)ctx->func->destroy(ctx->cryp_ctx);
        ctx->cryp_ctx = TD_NULL;
    }

    /* Clone the function from template of symc engine. */
    ctx->func = cryp_get_symc_op(cfg->alg, cfg->mode);
    if (ctx->func == TD_NULL) {
        print_func_errno(cryp_get_symc_op, OT_ERR_CIPHER_INVALID_POINT);
        return OT_ERR_CIPHER_UNSUPPORTED;
    }

    /* null means can ignore the function */
    if ((ctx->cryp_ctx == TD_NULL) && (ctx->func->create != TD_NULL)) {
        /* Create a instance from template of engine */
        ctx->cryp_ctx = ctx->func->create(soft_id);
        if (ctx->cryp_ctx == TD_NULL) {
            print_func_errno(ctx->func->create, OT_ERR_CIPHER_INVALID_POINT);
            return OT_ERR_CIPHER_INVALID_POINT;
        }
    }

    /* set mode and alg */
    if (ctx->func->setmode) {
        ctx->func->setmode(ctx->cryp_ctx, ctx->func->alg, ctx->func->mode, width);
    }

    chk_func_err_goto(kapi_symc_set_key_iv(cfg, ctx, &klen));

    /* Set AAD */
#ifdef CHIP_AES_CCM_GCM_SUPPORT
    if (ctx->func->setadd) {
        chk_func_err_goto(cipher_check_mmz_phy_addr((td_phys_addr_t)addr_u64(cfg->aad), cfg->alen));

        chk_func_err_goto(kapi_symc_check_ccm_gcm_taglen(cfg->alg, cfg->mode, cfg->tlen));

        chk_func_err_goto(ctx->func->setadd(ctx->cryp_ctx, cfg->aad, cfg->alen, cfg->tlen));
    }
#endif
    /* save ctrl */
    chk_func_err_goto(kapi_symc_save_ctrl(cfg, &ctx->ctrl));

    ctx->config = TD_TRUE;
exit__:
    return ret;
}

td_s32 kapi_symc_cfg(const symc_cfg_t *cfg)
{
    td_s32 ret;
    kapi_symc_ctx *ctx = TD_NULL;
    symc_width width = SYMC_DAT_WIDTH_COUNT;
    td_u32 soft_id;

    chk_ptr_err_return(cfg);
    chk_param_err_return(cfg->alen > MAX_AEAD_A_LEN);
    chk_param_err_return(addr_l32(cfg->aad) + cfg->alen < cfg->alen);

    ret = kapi_symc_chk_handle((td_handle)cfg->id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_chk_handle, ret);
        return ret;
    }

    soft_id = td_handle_get_chnid(cfg->id);
    ctx = &g_symc_ctx[soft_id];
    if (ctx == TD_NULL) {
        ot_cipher_error("kapi symc ctx is null.\n");
        return OT_ERR_CIPHER_INVALID_POINT;
    }
    crypto_chk_owner_err_return(&ctx->owner); /* called by user space, must check the pid */

    ret = kapi_symc_chk_param(cfg, &width);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_chk_param, ret);
        return ret;
    }

    kapi_symc_lock_err_return();

    ret = kapi_symc_unlock_cfg(soft_id, cfg, ctx, width);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_unlock_cfg, ret);
    }

    kapi_symc_unlock();
    return ret;
}

td_s32 kapi_symc_get_cfg(td_u32 id, ot_cipher_ctrl *ctrl)
{
    td_s32 ret;
    kapi_symc_ctx *ctx = TD_NULL;
    td_u32 *piv = TD_NULL;
    td_u32 *piv_len = TD_NULL;
    td_u32 soft_id;
    td_u32 iv_len;

    func_enter();

    ret = kapi_symc_chk_handle((td_handle)id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_chk_handle, ret);
        return ret;
    }

    soft_id = td_handle_get_chnid(id);
    ctx = &g_symc_ctx[soft_id];
    crypto_chk_owner_err_return(&ctx->owner); /* called by user space, must check the pid */
    chk_param_err_return(ctx->config != TD_TRUE);

    kapi_symc_lock_err_return();

    (td_void)memcpy_s(ctrl, sizeof(ot_cipher_ctrl), &ctx->ctrl, sizeof(ot_cipher_ctrl));
    if (ctrl->alg == OT_CIPHER_ALG_AES) {
        if ((ctrl->work_mode == OT_CIPHER_WORK_MODE_CCM) ||
            (ctrl->work_mode == OT_CIPHER_WORK_MODE_GCM)) {
            piv = ctrl->aes_ccm_gcm_ctrl.iv;
            piv_len = &ctrl->aes_ccm_gcm_ctrl.iv_len;
        } else {
            piv = ctrl->aes_ctrl.iv;
        }
    } else if (ctrl->alg == OT_CIPHER_ALG_SM4) {
#ifdef CHIP_SM4_SUPPORT
        piv = ctrl->sm4_ctrl.iv;
#endif
    }
    if (piv != TD_NULL && ctx->func != TD_NULL && ctx->func->getiv != TD_NULL) {
        (td_void)memset_s((td_u8 *)piv, AES_IV_SIZE, 0, AES_IV_SIZE);
        ret = ctx->func->getiv(ctx->cryp_ctx, (td_u8 *)piv, AES_IV_SIZE, &iv_len);
        if (ret != TD_SUCCESS) {
            kapi_symc_unlock();
            print_func_errno(ctx->func->getiv, ret);
            return ret;
        }
        if (piv_len != TD_NULL) {
            *piv_len = iv_len;
        }
    }

    kapi_symc_unlock();

    func_exit();

    return ret;
}

static td_s32 kapi_symc_check_crypto_addr(symc_encrypt_t *crypt, td_bool is_from_virt)
{
    td_s32 ret;

    ret = kapi_symc_chk_handle((td_handle)crypt->id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_chk_handle, ret);
        return ret;
    }
    if (!is_from_virt) {
        ret = cipher_check_mmz_phy_addr((td_phys_addr_t)addr_u64(crypt->input), crypt->length);
        if (ret != TD_SUCCESS) {
            print_func_errno(cipher_check_mmz_phy_addr, ret);
            return ret;
        }
        ret = cipher_check_mmz_phy_addr((td_phys_addr_t)addr_u64(crypt->output), crypt->length);
        if (ret != TD_SUCCESS) {
            print_func_errno(cipher_check_mmz_phy_addr, ret);
            return ret;
        }
    }
    return TD_SUCCESS;
}

static td_s32 kapi_symc_check_crypto_ctx(kapi_symc_ctx *ctx, const symc_encrypt_t *crypt)
{
    crypto_chk_owner_err_return(&ctx->owner); /* called by user space, must check the pid */
    chk_ptr_err_return(ctx->func);
    chk_ptr_err_return(ctx->func->crypto);
    chk_param_err_return(ctx->config != TD_TRUE);
    if (ctx->ctrl.alg != OT_CIPHER_ALG_DMA) {
        chk_param_err_return(ctx->attach != TD_TRUE);
    }
    chk_param_err_return((crypt->operation != SYMC_OPERATION_ENCRYPT) &&
        (crypt->operation != SYMC_OPERATION_DECRYPT));
    return TD_SUCCESS;
}

static void kapi_symc_build_pack(symc_encrypt_t *crypt, symc_node_usage *usage, symc_multi_pack *pack)
{
    *usage = SYMC_NODE_USAGE_NORMAL;
    (td_void)memset_s(pack, sizeof(*pack), 0, sizeof(*pack));
    pack->in  = &crypt->input;
    pack->out = &crypt->output;
    pack->len = &crypt->length;
    pack->usage = usage;
    pack->num = 1; /* 1 single package encrypt or decrypt. */
}

td_s32 kapi_symc_crypto(symc_encrypt_t *crypt, td_bool is_from_virt)
{
    td_s32 ret;
    symc_node_usage usage;
    kapi_symc_ctx *ctx = TD_NULL;
    td_u32 soft_id;
    symc_multi_pack pack;

    func_enter();

    ret = kapi_symc_check_crypto_addr(crypt, is_from_virt);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    soft_id = td_handle_get_chnid(crypt->id);
    ctx = &g_symc_ctx[soft_id];
    ret = kapi_symc_check_crypto_ctx(ctx, crypt);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    kapi_symc_build_pack(crypt, &usage, &pack);

    kapi_symc_lock_err_return();

    ret = ctx->func->crypto(ctx->cryp_ctx, crypt->operation, &pack, TD_TRUE);
    if (ret != TD_SUCCESS) {
        print_func_errno(ctx->func->crypto, ret);
        kapi_symc_unlock();
        return ret;
    }

    kapi_symc_unlock();
    func_exit();
    return TD_SUCCESS;
}

static void crypt_copy_to_local(symc_encrypt_t *crypt, symc_encrypt_t *local_crypt)
{
    local_crypt->id     = crypt->id;
    local_crypt->length = crypt->length;
    local_crypt->last   = crypt->last;
    local_crypt->operation = crypt->operation & SYMC_OPERATION_DECRYPT;
}

td_s32 kapi_symc_crypto_via(symc_encrypt_t *crypt, td_u32 is_from_user)
{
    td_s32 ret;
    td_s32 ret_exit;
    crypto_mem mem = {0};
    symc_encrypt_t local_crypt;

    func_enter();

    chk_ptr_err_return(addr_via_const(crypt->input));
    chk_ptr_err_return(addr_via(crypt->output));
    /* 4 * 1024 * 1024 is the max size of malloc, 4M */
    chk_param_err_return(crypt->length == 0x00 || crypt->length > 4 * 1024 * 1024);

    (td_void)memset_s(&local_crypt, sizeof(local_crypt), 0, sizeof(local_crypt));
    ret = crypto_mem_create(&mem, MEM_TYPE_MMZ, "aes_in", crypt->length);
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_mem_create, ret);
        return ret;
    }

    if (is_from_user == TD_TRUE) {
        chk_func_err_goto(crypto_copy_from_user(mem.dma_virt,
            crypt->length, addr_via_const(crypt->input), crypt->length));
    } else {
        if (memcpy_s(mem.dma_virt, mem.dma_size, addr_via_const(crypt->input), crypt->length) != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            ret = OT_ERR_CIPHER_FAILED_SEC_FUNC;
            goto exit__;
        }
    }

    local_crypt.input  = mem.dma_addr;
    local_crypt.output = mem.dma_addr;
    crypt_copy_to_local(crypt, &local_crypt);
    chk_func_err_goto(kapi_symc_crypto(&local_crypt, TD_TRUE));

    if (is_from_user == TD_TRUE) {
        chk_func_err_goto(crypto_copy_to_user(addr_via(crypt->output), crypt->length, mem.dma_virt, crypt->length));
    } else {
        if (memcpy_s(addr_via(crypt->output), crypt->length, mem.dma_virt, crypt->length) != EOK) {
            (td_void)memset_s(mem.dma_virt, crypt->length, 0, crypt->length);
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            ret = OT_ERR_CIPHER_FAILED_SEC_FUNC;
            goto exit__;
        }
    }

exit__:
    ret_exit = crypto_mem_destroy(&mem);
    if (ret_exit != TD_SUCCESS) {
        print_func_errno(crypto_mem_destroy, ret_exit);
        return ret_exit;
    }

    func_exit();
    return ret;
}

#ifdef CHIP_AES_CCM_GCM_SUPPORT
td_s32 kapi_aead_get_tag(td_u32 id, td_u32 tag[AEAD_TAG_SIZE_IN_WORD], td_u32 *taglen)
{
    td_s32 ret;
    kapi_symc_ctx *ctx = TD_NULL;
    td_u32 soft_id;

    func_enter();

    ret = kapi_symc_chk_handle((td_handle)id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_chk_handle, ret);
        return ret;
    }

    soft_id = td_handle_get_chnid(id);
    ctx = &g_symc_ctx[soft_id];
    crypto_chk_owner_err_return(&ctx->owner); /* called by user space, must check the pid */
    chk_ptr_err_return(ctx->func);
    chk_ptr_err_return(ctx->func->gettag);

    kapi_symc_lock_err_return();

    if (ctx->func->gettag) {
        ret = ctx->func->gettag(ctx->cryp_ctx, tag, taglen);
        if (ret != TD_SUCCESS) {
            print_func_errno(ctx->func->gettag, ret);
            kapi_symc_unlock();
            return ret;
        }
    }

    kapi_symc_unlock();

    func_exit();
    return TD_SUCCESS;
}
#endif

static td_s32 kapi_keyslot_chk_handle(td_handle handle)
{
    chk_handle_modid(handle, OT_ID_KEYSLOT);
    chk_handle_private_data(handle, OT_PRIVATE_ID_KEYSLOT);
    chk_handle_chnid(handle, KEYSLOT_CHANNEL_MAX);

    if (g_keyslot_open[td_handle_get_chnid(handle)] != TD_TRUE) {
        ot_cipher_error("channel %u is not open\n", td_handle_get_chnid(handle));
        return OT_ERR_CIPHER_INVALID_HANDLE;
    }

    return TD_SUCCESS;
}

td_s32 kapi_keyslot_create(const ot_keyslot_attr *attr, td_handle *keyslot)
{
    td_s32 ret;
    td_handle id;

    func_enter();
    kapi_symc_lock_err_return();
    ret = drv_kc_slot_lock(attr, &id);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_kc_slot_lock, ret);
        kapi_symc_unlock();
        return ret;
    }

    *keyslot = td_handle_init(OT_ID_KEYSLOT, OT_PRIVATE_ID_KEYSLOT, id);
    g_keyslot_open[id] = TD_TRUE;
    crypto_get_owner(&g_keyslot_owner[id]);
    kapi_symc_unlock();

    func_exit();

    return ret;
}

td_s32 kapi_keyslot_destroy(td_handle keyslot)
{
    td_s32 ret;
    td_u32 id;
    CRYPTO_OWNER owner;

    func_enter();

    ret = kapi_keyslot_chk_handle(keyslot);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_keyslot_chk_handle, ret);
        return ret;
    }

    kapi_symc_lock_err_return();

    id = td_handle_get_chnid(keyslot);
    crypto_get_owner(&owner);
    if (g_keyslot_owner[id] != owner) {
        ot_cipher_error("owner ID is error!!\n");
        kapi_symc_unlock();
        return TD_FAILURE;
    }

    ret = drv_kc_slot_unlock(id);
    if (ret != TD_SUCCESS) {
        print_func_errno(drv_kc_slot_unlock, ret);
        kapi_symc_unlock();
        return ret;
    }
    g_keyslot_open[id] = TD_FALSE;
    g_keyslot_owner[id] = 0;

    kapi_symc_unlock();
    func_exit();
    return ret;
}

td_s32 kapi_symc_attach(td_handle cipher, td_handle keyslot)
{
    td_s32 ret;
    td_u32 id;
    kapi_symc_ctx *symc_ctx = TD_NULL;

    func_enter();

    ret = kapi_symc_chk_handle(cipher);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_chk_handle, ret);
        return ret;
    }

    ret = kapi_keyslot_chk_handle(keyslot);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_keyslot_chk_handle, ret);
        return ret;
    }

    kapi_symc_lock_err_return();

    id = td_handle_get_chnid(cipher);
    symc_ctx = &g_symc_ctx[id];

    symc_ctx->attach = TD_TRUE;
    symc_ctx->key_slot = keyslot;

    kapi_symc_unlock();

    func_exit();

    return TD_SUCCESS;
}

td_s32 kapi_symc_detach(td_handle cipher, td_handle keyslot)
{
    td_s32 ret;
    td_u32 id;
    kapi_symc_ctx *symc_ctx = TD_NULL;

    func_enter();

    ret = kapi_symc_chk_handle(cipher);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_symc_chk_handle, ret);
        return ret;
    }

    ret = kapi_keyslot_chk_handle(keyslot);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_keyslot_chk_handle, ret);
        return ret;
    }

    kapi_symc_lock_err_return();

    id = td_handle_get_chnid(cipher);
    symc_ctx = &g_symc_ctx[id];

    if (symc_ctx->attach != TD_TRUE) {
        ot_cipher_error("cipher %u isn't attached\n", id);
        kapi_symc_unlock();
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    if (symc_ctx->key_slot != keyslot) {
        ot_cipher_error("handle don't match, attach 0x%x, input 0x%x\n", symc_ctx->key_slot, keyslot);
        kapi_symc_unlock();
        return OT_ERR_CIPHER_INVALID_PARAM;
    }
    symc_ctx->key_slot = OT_INVALID_HANDLE;
    symc_ctx->attach = TD_FALSE;

    kapi_symc_unlock();

    func_exit();

    return TD_SUCCESS;
}
