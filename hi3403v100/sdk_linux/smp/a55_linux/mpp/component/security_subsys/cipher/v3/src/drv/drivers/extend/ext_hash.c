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

#if defined(SOFT_SHA1_SUPPORT) || defined(SOFT_SHA256_SUPPORT) || defined(SOFT_SHA512_SUPPORT)
#include "mbedtls/md.h"

td_void *mbedtls_hash_create(hash_mode mode)
{
    td_s32 ret;
    mbedtls_md_type_t md_type;
    const mbedtls_md_info_t *info = TD_NULL;
    mbedtls_md_context_t *ctx = TD_NULL;

    func_enter();

    /* convert to mebdtls type */
    md_type = MBEDTLS_MD_SHA1 + (mode - HASH_MODE_SHA1);

    info = mbedtls_md_info_from_type(md_type);
    if (info == TD_NULL) {
        ot_cipher_error("error, invalid hash mode %d\n", mode);
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return TD_NULL;
    }

    ctx = crypto_malloc(sizeof(mbedtls_md_context_t));
    if (ctx == TD_NULL) {
        ot_cipher_error("malloc hash context buffer failed!");
        print_errno(OT_ERR_CIPHER_FAILED_MEM);
        return TD_NULL;
    }
    (td_void)memset_s(ctx, sizeof(mbedtls_md_context_t), 0, sizeof(mbedtls_md_context_t));

    mbedtls_md_init(ctx);
    ret = mbedtls_md_setup(ctx, info, TD_FALSE);
    if (ret != TD_SUCCESS) {
        crypto_free(ctx);
        print_func_errno(mbedtls_md_setup, ret);
        return TD_NULL;
    }

    ret = mbedtls_md_starts(ctx);
    if (ret != TD_SUCCESS) {
        mbedtls_md_free(ctx);
        crypto_free(ctx);
        print_func_errno(mbedtls_md_starts, ret);
        return TD_NULL;
    }

    func_exit();

    return ctx;
}

td_s32 mbedtls_hash_update(td_void *ctx, const td_u8 *chunk, td_u32 chunk_len, hash_chunk_src src)
{
    td_u8 *ptr = TD_NULL;
    td_s32 ret;
    mbedtls_md_context_t *md = ctx;
    td_u32 offset = 0;

    func_enter();

    chk_ptr_err_return(ctx);

    if (chunk_len == 0x00) {
        return TD_SUCCESS;
    }

    if (src == HASH_CHUNCK_SRC_USER) {
        ptr = crypto_malloc(HASH_MAX_BUFFER_SIZE);
        if (ptr == TD_NULL) {
            print_func_errno(crypto_malloc, OT_ERR_CIPHER_FAILED_MEM);
            return OT_ERR_CIPHER_FAILED_MEM;
        }

        while (offset < chunk_len) {
            td_u32 length = chunk_len - offset;

            if (length > HASH_MAX_BUFFER_SIZE) {
                length = HASH_MAX_BUFFER_SIZE;
            }

            chk_func_err_goto(crypto_copy_from_user(ptr, HASH_MAX_BUFFER_SIZE, chunk + offset, length));
            chk_func_err_goto(mbedtls_md_update(md, ptr, length));

            crypto_msleep(1);
            offset += length;
        }
    } else {
        ret = mbedtls_md_update(md, chunk, chunk_len);
        if (ret != TD_SUCCESS) {
            print_func_errno(mbedtls_md_update, ret);
        }
    }

exit__:
    crypto_free(ptr);

    func_exit();

    return ret;
}

td_s32 mbedtls_hash_finish(td_void *ctx,  td_void *hash, td_u32 hash_buf_len, td_u32 *hashlen)
{
    td_s32 ret;
    mbedtls_md_context_t *md = ctx;

    func_enter();

    chk_ptr_err_return(ctx);

    ret = mbedtls_md_finish(md, hash);
    if (ret != TD_SUCCESS) {
        print_func_errno(mbedtls_md_finish, ret);
        return TD_NULL;
    }

    *hashlen = mbedtls_md_get_size(md->md_info);
    chk_param_err_return(*hashlen > hash_buf_len);

    func_exit();
    return TD_SUCCESS;
}

td_s32 mbedtls_hash_destroy(td_void *ctx)
{
    mbedtls_md_context_t *md = (mbedtls_md_context_t *)ctx;

    func_enter();

    chk_ptr_err_return(ctx);

    mbedtls_md_free(md);
    crypto_free(md);

    func_exit();
    return TD_SUCCESS;
}

#endif // End of SOFT_AES_CCM_GCM_SUPPORT
