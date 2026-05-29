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
#include "cryp_hash.h"
#include "cipher_ext.h"

#define HASH_SOFT_CHANNEL_MAX   0x0F
#define HASH_SOFT_CHANNEL_MASK  0x7FFF

/* hmac ipad byte */
#define HMAC_IPAD_BYTE                    0x36

/* hmac opad byte */
#define HMAC_OPAD_BYTE                    0x5C

#define HMAC_HASH                         0x01
#define HMAC_AESCBC                       0x02

typedef struct {
    hash_func *func;                      /* HASH function */
    td_void *cryp_ctx;                    /* Context of cryp instance */
    td_u32 hmac;                          /* HMAC or not */
    td_u32 mac_id;                        /* CMAC handle */
    td_u8 hmac_ipad[HASH_BLOCK_SIZE_128]; /* hmac ipad */
    td_u8 hmac_opad[HASH_BLOCK_SIZE_128]; /* hmac opad */
    CRYPTO_OWNER owner;                   /* user ID */
} kapi_hash_ctx;

/* Context of cipher */
static channel_context g_hash_ctx[HASH_SOFT_CHANNEL_MAX];

/* hash mutex */
static CRYPTO_MUTEX g_hash_mutex;

#define kapi_hash_lock_err_return()                     \
    do {                                                \
        ret = crypto_mutex_lock(&g_hash_mutex);         \
        if (ret != TD_SUCCESS) {                        \
            ot_cipher_error("error, hash lock failed\n");  \
            print_errno(OT_ERR_CIPHER_BUSY);  \
            return OT_ERR_CIPHER_BUSY;                  \
        }                                               \
    } while (0)

#define kapi_hash_unlock()   crypto_mutex_unlock(&g_hash_mutex)

static td_s32 kapi_hash_chk_handle(td_handle handle)
{
    if ((td_handle_get_modid(handle) != OT_ID_CIPHER) ||
        (td_handle_get_private_data(handle) != 0)) {
        ot_cipher_error("invalid handle 0x%x!\n", handle);
        print_errno(OT_ERR_CIPHER_INVALID_HANDLE);
        return OT_ERR_CIPHER_INVALID_HANDLE;
    }

    if (td_handle_get_chnid(handle) >= HASH_SOFT_CHANNEL_MAX) {
        ot_cipher_error("chan %u is too large, max: %d\n", td_handle_get_chnid(handle), HASH_SOFT_CHANNEL_MAX);
        print_errno(OT_ERR_CIPHER_INVALID_HANDLE);
        return OT_ERR_CIPHER_INVALID_HANDLE;
    }

    if (g_hash_ctx[td_handle_get_chnid(handle)].open == TD_FALSE) {
        ot_cipher_error("chan %u is not open\n", td_handle_get_chnid(handle));
        print_errno(OT_ERR_CIPHER_INVALID_HANDLE);
        return OT_ERR_CIPHER_INVALID_HANDLE;
    }

    return TD_SUCCESS;
}

/* API Code for kapi hash. */
td_s32 kapi_hash_init(td_void)
{
    td_s32 ret;

    func_enter();

    ot_cipher_dbg("HASH init\n");

    crypto_mutex_init(&g_hash_mutex);

    ret = cryp_hash_init();
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_hash_init, ret);
        crypto_mutex_destroy(&g_hash_mutex);
        return ret;
    }

    /* Initialize soft channel list */
    ret = crypto_channel_init(g_hash_ctx, HASH_SOFT_CHANNEL_MAX, sizeof(kapi_hash_ctx));
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_channel_init, ret);
        cryp_hash_deinit();
        crypto_mutex_destroy(&g_hash_mutex);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_hash_deinit(td_void)
{
    td_s32 ret;

    func_enter();

    ret = crypto_channel_deinit(g_hash_ctx, HASH_SOFT_CHANNEL_MAX);
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_channel_deinit, ret);
    }

    cryp_hash_deinit();

    crypto_mutex_destroy(&g_hash_mutex);

    func_exit();
    return TD_SUCCESS;
}

static kapi_hash_ctx *kapi_hash_get_ctx(td_u32 id)
{
    return crypto_channel_get_context(g_hash_ctx, HASH_SOFT_CHANNEL_MAX, id);
}

static td_s32 kapi_hash_create(td_u32 *id)
{
    td_s32 ret;
    td_u32 chn = 0;

    func_enter();

    /* allocate a hash channel */
    ret = crypto_channel_alloc(g_hash_ctx, HASH_SOFT_CHANNEL_MAX, HASH_SOFT_CHANNEL_MASK, &chn);
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_channel_alloc, ret);
        return ret;
    }

    *id = chn;

    ot_cipher_dbg("kapi create soft chn %u\n", chn);

    func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_hash_destroy(td_u32 id)
{
    func_enter();

    chk_param_err_return(id >= HASH_SOFT_CHANNEL_MAX);

    /* Free soft channel */
    crypto_channel_free(g_hash_ctx, HASH_SOFT_CHANNEL_MAX, id);

    func_exit();
    return TD_SUCCESS;
}

static td_s32 kapi_hash_mode_transform(ot_cipher_hash_type type, hash_mode *mode, td_u32 *hmac)
{
    *hmac = TD_FALSE;

    /* transform hash mode */
    switch (type) {
        case OT_CIPHER_HASH_TYPE_HMAC_SHA256:
            *hmac = HMAC_HASH;
            *mode = HASH_MODE_SHA256;
            break;
        case OT_CIPHER_HASH_TYPE_SHA256:
            *mode = HASH_MODE_SHA256;
            break;
        case OT_CIPHER_HASH_TYPE_HMAC_SHA384:
            *hmac = HMAC_HASH;
            *mode = HASH_MODE_SHA384;
            break;
        case OT_CIPHER_HASH_TYPE_SHA384:
            *mode = HASH_MODE_SHA384;
            break;
        case OT_CIPHER_HASH_TYPE_HMAC_SHA512:
            *hmac = HMAC_HASH;
            *mode = HASH_MODE_SHA512;
            break;
        case OT_CIPHER_HASH_TYPE_SHA512:
            *mode = HASH_MODE_SHA512;
            break;
#ifdef CHIP_SM3_SUPPORT
        case OT_CIPHER_HASH_TYPE_HMAC_SM3:
            *hmac = HMAC_HASH;
            *mode = HASH_MODE_SM3;
            break;
        case OT_CIPHER_HASH_TYPE_SM3:
            *mode = HASH_MODE_SM3;
            break;
#endif
        default:
            ot_cipher_error("error, Unsupported hash type %d\n", type);
            print_errno(OT_ERR_CIPHER_INVALID_PARAM);
            return OT_ERR_CIPHER_INVALID_PARAM;
    }

    return TD_SUCCESS;
}

static td_void kapi_hmac_cal_ipad_opad(kapi_hash_ctx *ctx)
{
    td_u32 i;

    for (i = 0; i < ctx->func->block_size; i++) {
        ctx->hmac_ipad[i] ^= HMAC_IPAD_BYTE;
        ctx->hmac_opad[i] ^= HMAC_OPAD_BYTE;
    }
}

static td_s32 kapi_hmac_start(kapi_hash_ctx *ctx, const td_u8 *key, td_u32 keylen)
{
    td_s32 ret;
    td_u8 sum[HASH_RESULT_MAX_SIZE] = {0};
    td_u32 len = 0;

    func_enter();

    chk_ptr_err_return(key);

    /* clean ipad and opad */
    if ((memset_s(ctx->hmac_ipad, HASH_BLOCK_SIZE_128, 0x00, ctx->func->block_size) != EOK) ||
        (memset_s(ctx->hmac_opad, HASH_BLOCK_SIZE_128, 0x00, ctx->func->block_size) != EOK)) {
        print_func_errno(memset_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }

    /* compute K0 */
    if (keylen <= ctx->func->block_size) {
        /* If the length of K = B: set K0 = K.
         *
         * If the length of K > B: hash K to obtain an L byte string,
         * then append (B-L) zeros to create a B-byte
         * string K0 (i.e., K0 = H(K) || 00...00).
         */
        if ((memcpy_s(ctx->hmac_ipad, HASH_BLOCK_SIZE_128, key, keylen) != EOK) ||
            (memcpy_s(ctx->hmac_opad, HASH_BLOCK_SIZE_128, key, keylen) != EOK)) {
            return OT_ERR_CIPHER_FAILED_SEC_FUNC;
        }
    } else {
        /* If the length of K > B: hash K to obtain an L byte string,
         * then append (B-L) zeros to create a B-byte
         * string K0 (i.e., K0 = H(K) || 00...00).
         */
        ctx->cryp_ctx = ctx->func->create(ctx->func->mode); /* H(K) */
        if (ctx->cryp_ctx == TD_NULL) {
            print_func_errno(ctx->func->create, OT_ERR_CIPHER_BUSY);
            return OT_ERR_CIPHER_BUSY;
        }

        /* update key */
        chk_func_err_goto(ctx->func->update(ctx->cryp_ctx, key, keylen, HASH_CHUNCK_SRC_LOCAL));

        /* sum */
        chk_func_err_goto(ctx->func->finish(ctx->cryp_ctx, sum, sizeof(sum), &len));
        ctx->func->destroy(ctx->cryp_ctx);
        ctx->cryp_ctx = TD_NULL;

        /* descript: K0 = H(K) || 00...00. */
        if ((memcpy_s(ctx->hmac_ipad, sizeof(ctx->hmac_ipad), sum, len) != EOK) ||
            (memcpy_s(ctx->hmac_opad, sizeof(ctx->hmac_opad), sum, len) != EOK)) {
            ret = OT_ERR_CIPHER_FAILED_SEC_FUNC;
            goto exit__;
        }
    }

    /* Exclusive-Or K0 with ipad/opad byte to produce K0 ^ ipad and K0 ^ opad */
    kapi_hmac_cal_ipad_opad(ctx);

    /* H(K0 ^ ipad) */
    ctx->cryp_ctx = ctx->func->create(ctx->func->mode);
    if (ctx->cryp_ctx == TD_NULL) {
        print_func_errno(ctx->func->create, OT_ERR_CIPHER_BUSY);
        return OT_ERR_CIPHER_BUSY;
    }
    chk_func_err_goto(ctx->func->update(ctx->cryp_ctx,
        ctx->hmac_ipad, ctx->func->block_size, HASH_CHUNCK_SRC_LOCAL));

    func_exit();
    return TD_SUCCESS;

exit__:
    ctx->func->destroy(ctx->cryp_ctx);
    ctx->cryp_ctx = TD_NULL;
    (td_void)memset_s(ctx->hmac_ipad, sizeof(ctx->hmac_ipad), 0, sizeof(ctx->hmac_ipad));
    (td_void)memset_s(ctx->hmac_opad, sizeof(ctx->hmac_opad), 0, sizeof(ctx->hmac_opad));
    return ret;
}

static td_s32 kapi_hmac_finish(kapi_hash_ctx *ctx, td_u8 *hash, td_u32 hash_buf_len, td_u32 *hashlen)
{
    td_s32 ret;
    td_u8 *sum = TD_NULL;

    func_enter();
    sum = crypto_calloc(1, HASH_RESULT_MAX_SIZE);
    if (sum == TD_NULL) {
        return OT_ERR_CIPHER_FAILED_MEM;
    }

    /* descript: sum = H((K0 ^ ipad) || text). */
    ret = ctx->func->finish(ctx->cryp_ctx, sum, HASH_RESULT_MAX_SIZE, hashlen);
    if (ret != TD_SUCCESS) {
        print_func_errno(ctx->func->finish, ret);
        goto exit__;
    }

    ret = ctx->func->destroy(ctx->cryp_ctx);
    if (ret != TD_SUCCESS) {
        print_func_errno(ctx->func->destroy, ret);
        goto exit__;
    }

    ctx->cryp_ctx = TD_NULL;

    /* H((K0 ^ opad)|| sum). */
    ctx->cryp_ctx = ctx->func->create(ctx->func->mode);
    if (ctx->cryp_ctx == TD_NULL) {
        print_func_errno(ctx->func->create, 0);
        ret =  OT_ERR_CIPHER_BUSY;
        goto exit__;
    }

    /* update(K0 ^ opad) */
    ret = ctx->func->update(ctx->cryp_ctx, ctx->hmac_opad, ctx->func->block_size, HASH_CHUNCK_SRC_LOCAL);
    if (ret != TD_SUCCESS) {
        print_func_errno(ctx->func->update, ret);
        goto exit__;
    }

    /* update(sum) */
    ret = ctx->func->update(ctx->cryp_ctx, sum, ctx->func->size, HASH_CHUNCK_SRC_LOCAL);
    if (ret != TD_SUCCESS) {
        print_func_errno(ctx->func->update, ret);
        goto exit__;
    }

    /* H */
    ret = ctx->func->finish(ctx->cryp_ctx, hash, hash_buf_len, hashlen);
    if (ret != TD_SUCCESS) {
        print_func_errno(ctx->func->finish, ret);
        goto exit__;
    }

    func_exit();
exit__:
    if (sum != TD_NULL) {
        crypto_free(sum);
    }

    return ret;
}

static td_s32 kapi_hash_finsh_calc(kapi_hash_ctx *ctx, td_u8 *hash, td_u32 hash_buf_len, td_u32 *hashlen)
{
    td_s32 ret;

    if (ctx->hmac) {
        ret = kapi_hmac_finish(ctx, hash, hash_buf_len, hashlen);
        if (ret != TD_SUCCESS) {
            print_func_errno(kapi_hmac_finish, ret);
            return ret;
        }
    } else {
        ret = ctx->func->finish(ctx->cryp_ctx, hash, hash_buf_len, hashlen);
        if (ret != TD_SUCCESS) {
            print_func_errno(ctx->func->finish, ret);
            return ret;
        }
    }

    return TD_SUCCESS;
}

static td_s32 kapi_hash_chk_ctx(td_u32 id, td_u32 *soft_hash_id)
{
    td_s32 ret;
    kapi_hash_ctx *ctx = TD_NULL;

    ret = kapi_hash_chk_handle((td_handle)id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_chk_handle, ret);
        return ret;
    }

    *soft_hash_id = td_handle_get_chnid(id);
    ctx = kapi_hash_get_ctx(*soft_hash_id);
    if (ctx == TD_NULL) {
        ot_cipher_error("kapi hash get ctx is null.\n");
        print_func_errno(kapi_hash_get_ctx, OT_ERR_CIPHER_INVALID_POINT);
        return OT_ERR_CIPHER_INVALID_POINT;
    }

    crypto_chk_owner_err_return(&ctx->owner);
    return TD_SUCCESS;
}

static td_s32 kapi_hash_set_ctx(kapi_hash_ctx *ctx, hash_mode mode, td_u32 hmac)
{
    (td_void)memset_s(ctx, sizeof(kapi_hash_ctx), 0, sizeof(kapi_hash_ctx));
    /* record owner */
    crypto_get_owner(&ctx->owner);
    ctx->hmac = hmac;

    /* Clone the function from template of hash engine */
    ctx->cryp_ctx = TD_NULL;
    ctx->func = cryp_get_hash(mode);
    if (ctx->func == TD_NULL) {
        print_func_errno(cryp_get_hash, OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    if ((ctx->func->create == TD_NULL) || (ctx->func->update == TD_NULL) ||
        (ctx->func->finish == TD_NULL) || (ctx->func->destroy == TD_NULL)) {
        return OT_ERR_CIPHER_UNSUPPORTED;
    }

    return TD_SUCCESS;
}

static td_s32 kapi_hash_create_calc(kapi_hash_ctx *ctx,
    hash_mode mode, td_u32 hmac, const td_u8 *key, td_u32 keylen)
{
    td_s32 ret;

    ret = kapi_hash_set_ctx(ctx, mode, hmac);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_set_ctx, ret);
        return ret;
    }

    if (ctx->hmac) {
        ret = kapi_hmac_start(ctx, key, keylen);
        if (ret != TD_SUCCESS) {
            print_func_errno(kapi_hmac_start, ret);
            return ret;
        }
    } else {
        ctx->cryp_ctx = ctx->func->create(mode);
        if (ctx->cryp_ctx == TD_NULL) {
            print_func_errno(ctx->func->create, OT_ERR_CIPHER_FAILED_MEM);
            return OT_ERR_CIPHER_FAILED_MEM;
        }
    }
    return TD_SUCCESS;
}

td_s32 kapi_hash_start(td_u32 *id, ot_cipher_hash_type type, const td_u8 *key, td_u32 keylen)
{
    td_s32 ret;
    td_s32 ret_error;
    kapi_hash_ctx *ctx = TD_NULL;
    hash_mode mode = 0x00;
    td_u32 hmac = 0;
    td_u32 soft_hash_id = 0;

    func_enter();

    chk_ptr_err_return(id);

    /* transform hash mode */
    ret = kapi_hash_mode_transform(type, &mode, &hmac);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_mode_transform, ret);
        return ret;
    }

    kapi_hash_lock_err_return();

    /* Create hash channel */
    ret = kapi_hash_create(&soft_hash_id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_create, ret);
        kapi_hash_unlock();
        return ret;
    }

    ctx = kapi_hash_get_ctx(soft_hash_id);
    if (ctx == TD_NULL) {
        print_func_errno(kapi_hash_get_ctx, OT_ERR_CIPHER_FAILED_MEM);
        ret = OT_ERR_CIPHER_FAILED_MEM;
        goto error1;
    }

    ret = kapi_hash_create_calc(ctx, mode, hmac, key, keylen);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_create_calc, ret);
        goto error1;
    }

    *id = td_handle_init(OT_ID_CIPHER, 0, soft_hash_id);
    kapi_hash_unlock();
    func_exit();
    return TD_SUCCESS;

error1:
    ret_error = kapi_hash_destroy(soft_hash_id);
    if (ret_error != TD_SUCCESS) {
        print_func_errno(kapi_hash_destroy, ret_error);
    }

    kapi_hash_unlock();
    func_exit();
    return ret;
}

td_s32 kapi_hash_update(td_u32 id, const td_u8 *input, td_u32 length, hash_chunk_src src)
{
    td_s32 ret;
    kapi_hash_ctx *ctx = TD_NULL;
    td_u32 soft_hash_id = 0;

    func_enter();

    chk_param_err_return(input > input + length); /* check overflow */

    ret = kapi_hash_chk_ctx((td_handle)id, &soft_hash_id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_chk_ctx, ret);
        return ret;
    }

    ctx = kapi_hash_get_ctx(soft_hash_id);
    chk_ptr_err_return(ctx);
    chk_ptr_err_return(ctx->cryp_ctx);
    chk_ptr_err_return(ctx->func);
    chk_ptr_err_return(ctx->func->update);

    kapi_hash_lock_err_return();

    ret = ctx->func->update(ctx->cryp_ctx, input, length, src);
    /* release resource */
    if (ret != TD_SUCCESS) {
        ctx->func->destroy(ctx->cryp_ctx);
        ctx->cryp_ctx = TD_NULL;
        kapi_hash_destroy(soft_hash_id);
        print_func_errno(ctx->func->update, ret);
    }

    kapi_hash_unlock();

    func_exit();

    return ret;
}

td_s32 kapi_hash_get(td_handle handle, drv_hash_context *clone_ctx)
{
    td_s32 ret;
    kapi_hash_ctx *ctx = TD_NULL;
    td_u32 soft_hash_id = 0;

    func_enter();

    ret = kapi_hash_chk_ctx(handle, &soft_hash_id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_chk_ctx, ret);
        return ret;
    }

    ctx = kapi_hash_get_ctx(soft_hash_id);
    chk_ptr_err_return(ctx);
    chk_ptr_err_return(ctx->cryp_ctx);
    chk_ptr_err_return(ctx->func);
    chk_ptr_err_return(ctx->func->get);

    kapi_hash_lock_err_return();

    ret = ctx->func->get(ctx->cryp_ctx, clone_ctx);
    /* release resource */
    if (ret != TD_SUCCESS) {
        ctx->func->destroy(ctx->cryp_ctx);
        ctx->cryp_ctx = TD_NULL;
        kapi_hash_destroy(soft_hash_id);
        print_func_errno(ctx->func->get, ret);
    }

    kapi_hash_unlock();

    func_exit();

    return ret;
}

td_s32 kapi_hash_set(td_handle handle, const drv_hash_context *clone_ctx)
{
    td_s32 ret;
    kapi_hash_ctx *ctx = TD_NULL;
    td_u32 soft_hash_id = 0;

    func_enter();

    ret = kapi_hash_chk_ctx(handle, &soft_hash_id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_chk_ctx, ret);
        return ret;
    }

    ctx = kapi_hash_get_ctx(soft_hash_id);
    chk_ptr_err_return(ctx);
    chk_ptr_err_return(ctx->cryp_ctx);
    chk_ptr_err_return(ctx->func);
    chk_ptr_err_return(ctx->func->get);

    kapi_hash_lock_err_return();

    ret = ctx->func->set(ctx->cryp_ctx, clone_ctx);
    /* release resource */
    if (ret != TD_SUCCESS) {
        ctx->func->destroy(ctx->cryp_ctx);
        ctx->cryp_ctx = TD_NULL;
        kapi_hash_destroy(soft_hash_id);
        print_func_errno(ctx->func->set, ret);
    }

    kapi_hash_unlock();

    func_exit();

    return ret;
}

td_s32 kapi_hash_destroy_handle(td_handle handle)
{
    td_s32 ret;
    td_u32 soft_hash_id = 0;
    kapi_hash_ctx *ctx = TD_NULL;
    func_enter();

    ret = kapi_hash_chk_ctx(handle, &soft_hash_id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_chk_ctx, ret);
        return ret;
    }

    ctx = kapi_hash_get_ctx(soft_hash_id);
    chk_ptr_err_return(ctx);
    chk_ptr_err_return(ctx->cryp_ctx);
    chk_ptr_err_return(ctx->func);
    chk_ptr_err_return(ctx->func->destroy);

    kapi_hash_lock_err_return();

    /* Free soft channel */
    ctx->func->destroy(ctx->cryp_ctx);
    ctx->cryp_ctx = TD_NULL;
    crypto_channel_free(g_hash_ctx, HASH_SOFT_CHANNEL_MAX, soft_hash_id);

    kapi_hash_unlock();

    func_exit();
    return TD_SUCCESS;
}

td_s32 kapi_hash_finish(td_u32 id, td_u8 *hash, td_u32 hash_buf_len, td_u32 *hashlen)
{
    td_s32 ret;
    kapi_hash_ctx *ctx = TD_NULL;
    td_u32 soft_hash_id = 0;

    func_enter();

    chk_param_err_return(hash_buf_len == 0);
    chk_ptr_err_return(hashlen);

    ret = kapi_hash_chk_ctx((td_handle)id, &soft_hash_id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_chk_ctx, ret);
        return ret;
    }

    ctx = kapi_hash_get_ctx(soft_hash_id);
    chk_ptr_err_return(ctx);
    chk_ptr_err_return(ctx->cryp_ctx);
    chk_ptr_err_return(ctx->func);
    chk_ptr_err_return(ctx->func->destroy);

    kapi_hash_lock_err_return();

    ret = kapi_hash_finsh_calc(ctx, hash, hash_buf_len, hashlen);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_finsh_calc, ret);
        kapi_hash_unlock();
        return ret;
    }

    /* release resource */
    ctx->func->destroy(ctx->cryp_ctx);
    ctx->cryp_ctx = TD_NULL;
    ret = kapi_hash_destroy(soft_hash_id);
    if (ret != TD_SUCCESS) {
        print_func_errno(kapi_hash_destroy, ret);
    }

    kapi_hash_unlock();
    func_exit();
    return ret;
}

td_s32 kapi_hash_release(td_void)
{
    td_u32 i;
    td_s32 ret;
    kapi_hash_ctx *ctx = TD_NULL;
    CRYPTO_OWNER owner;

    func_enter();

    crypto_get_owner(&owner);

    /* destroy the channel which are created by current user */
    for (i = 0; i < HASH_SOFT_CHANNEL_MAX; i++) {
        if (g_hash_ctx[i].open == TD_FALSE) {
            continue;
        }

        ctx = kapi_hash_get_ctx(i);
        if (ctx == TD_NULL) {
            print_func_errno(kapi_hash_get_ctx, OT_ERR_CIPHER_INVALID_POINT);
            return OT_ERR_CIPHER_INVALID_POINT;
        }

        if (memcmp(&owner, &ctx->owner, sizeof(owner)) != 0) {
            continue;
        }

        ot_cipher_dbg("hash release chn %u\n", i);
        if ((ctx->func != TD_NULL) && (ctx->func->destroy != TD_NULL) && (ctx->cryp_ctx != TD_NULL)) {
            ctx->func->destroy(ctx->cryp_ctx);
        }
        ctx->cryp_ctx = TD_NULL;
        ret = kapi_hash_destroy(i);
        if (ret != TD_SUCCESS) {
            print_func_errno(kapi_hash_destroy, ret);
            return ret;
        }
    }

    func_exit();
    return TD_SUCCESS;
}
