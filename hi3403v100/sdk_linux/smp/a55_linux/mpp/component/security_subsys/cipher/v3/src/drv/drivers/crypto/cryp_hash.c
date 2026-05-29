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
#include "drv_hash.h"
#include "cryp_hash.h"
#include "ext_alg.h"

/* size of hash physical memory 64k */
#define HASH_PHY_MEM_SIZE               0x10000

/* try to create memory for HASH */
#define HASH_PHY_MEM_CREATE_TRY_TIME    10

/* block size */
#define SHA256_BLOCK_SIZE               64
#define SHA384_BLOCK_SIZE               128
#define SHA512_BLOCK_SIZE               128
#define SM3_BLOCK_SIZE                  64

/* first byte of hash padding */
#define HASH_PADDING_B0                 0x80

/* Hash padding 0x80 || Len(64)  */
#define HASH_BLOCK64_PAD_MIN            9
/* Hash padding 0x80 || Len(128) */
#define HASH_BLOCK128_PAD_MIN           17

/* The max tab size of symc function */
#define HASH_FUNC_TAB_SIZE              16

/* SHA-256, the initial hash value, H(0) */
#define SHA256_H0                       0x6a09e667
#define SHA256_H1                       0xbb67ae85
#define SHA256_H2                       0x3c6ef372
#define SHA256_H3                       0xa54ff53a
#define SHA256_H4                       0x510e527f
#define SHA256_H5                       0x9b05688c
#define SHA256_H6                       0x1f83d9ab
#define SHA256_H7                       0x5be0cd19

/* SHA-384, the initial hash value, H(0) */
#define SHA384_H0                       0xcbbb9d5dc1059ed8ULL
#define SHA384_H1                       0x629a292a367cd507ULL
#define SHA384_H2                       0x9159015a3070dd17ULL
#define SHA384_H3                       0x152fecd8f70e5939ULL
#define SHA384_H4                       0x67332667ffc00b31ULL
#define SHA384_H5                       0x8eb44a8768581511ULL
#define SHA384_H6                       0xdb0c2e0d64f98fa7ULL
#define SHA384_H7                       0x47b5481dbefa4fa4ULL

/* SHA-512, the initial hash value, H(0) */
#define SHA512_H0                       0x6a09e667f3bcc908ULL
#define SHA512_H1                       0xbb67ae8584caa73bULL
#define SHA512_H2                       0x3c6ef372fe94f82bULL
#define SHA512_H3                       0xa54ff53a5f1d36f1ULL
#define SHA512_H4                       0x510e527fade682d1ULL
#define SHA512_H5                       0x9b05688c2b3e6c1fULL
#define SHA512_H6                       0x1f83d9abfb41bd6bULL
#define SHA512_H7                       0x5be0cd19137e2179ULL

/* SM3, the initial hash value, H(0) */
#define SM3_H0                          0x7380166F
#define SM3_H1                          0x4914B2B9
#define SM3_H2                          0x172442D7
#define SM3_H3                          0xDA8A0600
#define SM3_H4                          0xA96F30BC
#define SM3_H5                          0x163138AA
#define SM3_H6                          0xE38DEE4D
#define SM3_H7                          0xB0FB0E4E

/* hash function list */
static hash_func *g_hash_descriptor = TD_NULL;

#ifdef CHIP_HASH_SUPPORT

/*
 * \brief          hash context structure
 */
typedef struct {
    hash_mode mode;                             /* HASH mode */
    td_u32 block_size;                          /* HASH block size */
    td_u32 hash_size;                           /* HASH result size */
    td_u32 hard_chn;                            /* HASH hardware channel number */
    td_u8 tail[HASH_BLOCK_SIZE_128 << 1];       /* buffer to store the tail and padding data */
    td_u32 tail_len;                            /* length of the tail message */
    td_u32 total;                               /* total length of the message */
    td_u32 hash[HASH_RESULT_MAX_SIZE_IN_WORD];  /* buffer to store the result */
    crypto_mem mem;                             /* DMA memory of hash message */
} cryp_hash_context;

/* hash dma memory */
static crypto_mem g_hash_mem;

/* ****************************** API Code **************************** */
static td_void cryp_sha2_256_init(cryp_hash_context *ot_ctx)
{
    ot_ctx->hash_size = SHA256_RESULT_SIZE;
    ot_ctx->block_size = SHA256_BLOCK_SIZE;
    ot_ctx->hash[WORD_IDX_0] = crypto_cpu_to_be32(SHA256_H0);
    ot_ctx->hash[WORD_IDX_1] = crypto_cpu_to_be32(SHA256_H1);
    ot_ctx->hash[WORD_IDX_2] = crypto_cpu_to_be32(SHA256_H2);
    ot_ctx->hash[WORD_IDX_3] = crypto_cpu_to_be32(SHA256_H3);
    ot_ctx->hash[WORD_IDX_4] = crypto_cpu_to_be32(SHA256_H4);
    ot_ctx->hash[WORD_IDX_5] = crypto_cpu_to_be32(SHA256_H5);
    ot_ctx->hash[WORD_IDX_6] = crypto_cpu_to_be32(SHA256_H6);
    ot_ctx->hash[WORD_IDX_7] = crypto_cpu_to_be32(SHA256_H7);
}

static td_void cryp_sha2_384_init(cryp_hash_context *ot_ctx)
{
    td_u32 idx;
    td_u32 i;
    td_u64 tmp_h;
    td_u64 arr_h[8] = {SHA384_H0, SHA384_H1, SHA384_H2, SHA384_H3, /* 8 - sha384 init array */
                       SHA384_H4, SHA384_H5, SHA384_H6, SHA384_H7};

    ot_ctx->hash_size = SHA384_RESULT_SIZE;
    ot_ctx->block_size = SHA384_BLOCK_SIZE;

    for (i = 0, idx = 0; i < (sizeof(arr_h) / sizeof(arr_h[0])); i++) {
        tmp_h = crypto_cpu_to_be64(arr_h[i]);
        if (memcpy_s(&ot_ctx->hash[idx], DOUBLE_WORD_WIDTH, &tmp_h, sizeof(td_u64)) != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            return;
        }
        idx += WORD_IDX_2;
    }
}

static td_void cryp_sha2_512_init(cryp_hash_context *ot_ctx)
{
    td_u32 idx;
    td_u32 i;
    td_u64 tmp_h;
    td_u64 arr_h[8] = {SHA512_H0, SHA512_H1, SHA512_H2, SHA512_H3, /* 8 - sha512 init array */
                       SHA512_H4, SHA512_H5, SHA512_H6, SHA512_H7};

    ot_ctx->hash_size = SHA512_RESULT_SIZE;
    ot_ctx->block_size = SHA512_BLOCK_SIZE;

    for (i = 0, idx = 0; i < (sizeof(arr_h) / sizeof(arr_h[0])); i++) {
        tmp_h = crypto_cpu_to_be64(arr_h[i]);
        if (memcpy_s(&ot_ctx->hash[idx], DOUBLE_WORD_WIDTH, &tmp_h, sizeof(td_u64)) != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            return;
        }
        idx += WORD_IDX_2;
    }
}

#ifdef CHIP_SM3_SUPPORT
static td_void cryp_sm3_init(cryp_hash_context *ot_ctx)
{
    ot_ctx->hash_size = SM3_RESULT_SIZE;
    ot_ctx->block_size = SM3_BLOCK_SIZE;
    ot_ctx->hash[WORD_IDX_0] = crypto_cpu_to_be32(SM3_H0);
    ot_ctx->hash[WORD_IDX_1] = crypto_cpu_to_be32(SM3_H1);
    ot_ctx->hash[WORD_IDX_2] = crypto_cpu_to_be32(SM3_H2);
    ot_ctx->hash[WORD_IDX_3] = crypto_cpu_to_be32(SM3_H3);
    ot_ctx->hash[WORD_IDX_4] = crypto_cpu_to_be32(SM3_H4);
    ot_ctx->hash[WORD_IDX_5] = crypto_cpu_to_be32(SM3_H5);
    ot_ctx->hash[WORD_IDX_6] = crypto_cpu_to_be32(SM3_H6);
    ot_ctx->hash[WORD_IDX_7] = crypto_cpu_to_be32(SM3_H7);
}
#endif

static td_s32 cryp_hash_initial(cryp_hash_context *ot_ctx, hash_mode mode)
{
    func_enter();

    switch (mode) {
        case HASH_MODE_SHA256:
            cryp_sha2_256_init(ot_ctx);
            break;
        case HASH_MODE_SHA384:
            cryp_sha2_384_init(ot_ctx);
            break;
        case HASH_MODE_SHA512:
            cryp_sha2_512_init(ot_ctx);
            break;
#ifdef CHIP_SM3_SUPPORT
        case HASH_MODE_SM3:
            cryp_sm3_init(ot_ctx);
            break;
#endif
        default:
            ot_cipher_error("Invalid hash mode, mode = 0x%x.\n", mode);
            print_errno(OT_ERR_CIPHER_INVALID_PARAM);
            return OT_ERR_CIPHER_INVALID_PARAM;
    }

    func_exit();
    return TD_SUCCESS;
}

/*
 * \brief          Create DMA memory of HASH message
 */
static td_s32 cryp_hash_create_mem(td_void)
{
    td_s32 ret;
    td_u32 i;
    td_u32 length = HASH_PHY_MEM_SIZE;

    func_enter();

    (td_void)memset_s(&g_hash_mem, sizeof(g_hash_mem), 0x00, sizeof(g_hash_mem));

    /* Try to alloc memory, halve the length if failed */
    for (i = 0; i < HASH_PHY_MEM_CREATE_TRY_TIME; i++) {
#ifdef OT_SMMU_SUPPORT
        ret = crypto_mem_create(&g_hash_mem, MEM_TYPE_SMMU, "hash_msg_dma", length);
#else
        ret = crypto_mem_create(&g_hash_mem, MEM_TYPE_MMZ, "hash_msg_dma", length);
#endif
        if (ret == TD_SUCCESS) {
            return TD_SUCCESS;
        } else {
            /* half the length */
            length /= MUL_VAL_2;
        }
    }

    func_exit();
    return OT_ERR_CIPHER_FAILED_MEM;
}

static td_s32 cryp_hash_destroy_mem(td_void)
{
    td_s32 ret;

    ret = crypto_mem_destroy(&g_hash_mem);
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_mem_destroy, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_hash_chunk_copy(td_void *dma, td_u32 dma_len,
    const td_void *chunk, td_u32 chunk_len, hash_chunk_src src)
{
    td_s32 ret;

    func_enter();

    /* Don't process the empty message */
    if (chunk_len == 0x00 || dma_len < chunk_len) {
        func_exit();
        return TD_SUCCESS;
    }

    chk_ptr_err_return(chunk);
    chk_ptr_err_return(dma);

    if (src == HASH_CHUNCK_SRC_LOCAL) {
        if (memcpy_s(dma, dma_len, chunk, chunk_len) != EOK) {
            print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
            return OT_ERR_CIPHER_FAILED_SEC_FUNC;
        }
    } else {
        ret = crypto_copy_from_user(dma, dma_len, chunk, chunk_len);
        if (ret != TD_SUCCESS) {
            print_func_errno(crypto_copy_from_user, ret);
            return ret;
        }
    }

    func_exit();
    return TD_SUCCESS;
}

/* hash hardware computation */
static td_s32 cryp_hash_process(cryp_hash_context *ot_ctx,
    const td_u8 *msg, td_u32 length, hash_chunk_src src)
{
    td_s32 ret;
    td_void *buf = TD_NULL;
    td_u32 left;
    td_u32 size;
    td_u32 max;

    func_enter();

    /* Don't process the empty message */
    if (length == 0x00) {
        func_exit();
        return TD_SUCCESS;
    }

    ot_cipher_dbg("length 0x%x, dma_size 0x%x\n", length, ot_ctx->mem.dma_size);

    /* get dma buffer */
    buf = crypto_mem_virt(&ot_ctx->mem);
    left = length;

    /* align at block size */
    max = ot_ctx->mem.dma_size - ot_ctx->mem.dma_size % ot_ctx->block_size;

    while (left > 0) {
        if (left <= max) {
            /* left size less than dma buffer,
             * can process all left message
             */
            size = left;
        } else {
            /* left size large than dma buffer,
             * just process message with dma size
             */
            size = max;
        }

        ot_cipher_dbg("size 0x%x, left 0x%x, max 0x%x\n", size, left, max);

        /* copy message to dma buffer */
        ret = cryp_hash_chunk_copy(buf, size, msg, size, src);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_hash_chunk_copy, ret);
            return ret;
        }

        /* configure mode */
        ret = drv_hash_cfg(ot_ctx->hard_chn, ot_ctx->mode, ot_ctx->hash);
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_hash_cfg, ret);
            return ret;
        }

        /* start */
        ret = drv_hash_start(ot_ctx->hard_chn, &ot_ctx->mem, size);
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_hash_start, ret);
            return ret;
        }

        /* wait done */
        ret = drv_hash_wait_done(ot_ctx->hard_chn, ot_ctx->hash, HASH_RESULT_MAX_SIZE_IN_WORD);
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_hash_wait_done, ret);
            break;
        }

        /* re-compute left length */
        left -= size;
        msg += size;
    }

    func_exit();
    return TD_SUCCESS;
}

/* hash message paading to align at block size */
static td_u32 cryp_hash_padding(td_u32 block_size, td_u8 *msg, td_u32 tail_size, td_u32 total)
{
    td_u32 pad_len;
    td_u32 min;

    func_enter();

    /* get min padding size */
    if (block_size == HASH_BLOCK_SIZE_64) {
        min = HASH_BLOCK64_PAD_MIN;
    } else {
        min = HASH_BLOCK128_PAD_MIN;
    }

    pad_len = block_size - (total % block_size);

    /* if pad len less than min value, add a block */
    if (pad_len < min) {
        pad_len += block_size;
    }

    /* Format(binary): data || 0x80 || 00 00 ... || Len(64) */
    (td_void)memset_s(&msg[tail_size], HASH_BLOCK_SIZE_128 * MUL_VAL_2 - tail_size, 0, pad_len);
    msg[tail_size] = HASH_PADDING_B0;
    tail_size += pad_len - WORD_WIDTH * MUL_VAL_2; /* Two word length left. */

    /* write 8 bytes fix data length * 8 */
    msg[tail_size++] = 0x00;
    msg[tail_size++] = 0x00;
    msg[tail_size++] = 0x00;
    msg[tail_size++] = (td_u8)((total >> SHIFT_29BITS) & MAX_LOW_3BITS);
    msg[tail_size++] = (td_u8)((total >> SHIFT_21BITS) & MAX_LOW_8BITS);
    msg[tail_size++] = (td_u8)((total >> SHIFT_13BITS) & MAX_LOW_8BITS);
    msg[tail_size++] = (td_u8)((total >> SHIFT_5BITS)  & MAX_LOW_8BITS);
    msg[tail_size++] = (td_u8)((total << SHIFT_3BITS)  & MAX_LOW_8BITS);

    func_exit();

    return tail_size;
}

static td_void *cryp_hash_create(hash_mode mode)
{
    td_s32 ret;
    cryp_hash_context *ot_ctx = TD_NULL;

    func_enter();

    ot_ctx = crypto_calloc(MUL_VAL_1, sizeof(cryp_hash_context));
    if (ot_ctx == TD_NULL) {
        print_errno(OT_ERR_CIPHER_FAILED_MEM);
        return TD_NULL;
    }

    ot_ctx->mode = mode;
    ot_ctx->hard_chn = HASH_HARD_CHANNEL;

    ret = cryp_hash_initial(ot_ctx, mode);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_hash_initial, ret);
        goto error1;
    }

    (td_void)memcpy_s(&ot_ctx->mem, sizeof(crypto_mem), &g_hash_mem, sizeof(crypto_mem));
    func_exit();

    return ot_ctx;

error1:
    crypto_free(ot_ctx);

    return TD_NULL;
}

static td_s32 cryp_hash_destroy(td_void *ctx)
{
    func_enter();
    if (ctx == TD_NULL) {
        func_exit();
        return TD_SUCCESS;
    }

    drv_hash_reset(((cryp_hash_context *)ctx)->hard_chn);
    crypto_free(ctx);

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_hash_update(td_void *ctx, const td_u8 *chunk, td_u32 chunk_len, hash_chunk_src src)
{
    td_s32 ret;
    cryp_hash_context *ot_ctx = ctx;
    td_u32 inverse_len;

    func_enter();
    chk_ptr_err_return(ot_ctx);

    ot_cipher_dbg("last: total 0x%x, block size %d, tail_len %d, chunkLen 0x%x, src %d\n",
        ot_ctx->total, ot_ctx->block_size, ot_ctx->tail_len, chunk_len, src);

    /* total len */
    ot_ctx->total += chunk_len;

    /* left tail len to make up a block */
    inverse_len = ot_ctx->block_size - ot_ctx->tail_len;

    if (chunk_len < inverse_len) {
        /* can't make up a block */
        inverse_len = chunk_len;
    }

    /* try to make up the tail data to be a block */
    ret = cryp_hash_chunk_copy(&ot_ctx->tail[ot_ctx->tail_len],
        sizeof(ot_ctx->tail) - ot_ctx->tail_len, chunk, inverse_len, src);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_hash_chunk_copy, ret);
        return ret;
    }
    ot_ctx->tail_len += inverse_len;
    chunk += inverse_len;
    chunk_len -= inverse_len;  /* the chunk_len may be zero */

    ot_cipher_dbg("new: total 0x%x, tail_len %u, chunk_len 0x%x\n", ot_ctx->total, ot_ctx->tail_len, chunk_len);

    /* process tail block */
    if (ot_ctx->tail_len == ot_ctx->block_size) {
        ret = cryp_hash_process(ot_ctx, ot_ctx->tail, ot_ctx->tail_len, HASH_CHUNCK_SRC_LOCAL);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_hash_process, ret);
            return ret;
        }
        /* new tail len */
        ot_ctx->tail_len = chunk_len % ot_ctx->block_size;

        /* new chunk len align at block size */
        chunk_len -= ot_ctx->tail_len;

        /* save new tail */
        ret = cryp_hash_chunk_copy(ot_ctx->tail, sizeof(ot_ctx->tail),
            chunk + chunk_len, ot_ctx->tail_len, src);
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_hash_chunk_copy, ret);
            return ret;
        }
    }

    ot_cipher_dbg("new: total 0x%x, tail_len %u, chunk_len 0x%x\n", ot_ctx->total, ot_ctx->tail_len, chunk_len);

    /* process left block, just return TD_SUCCESS if the chunk_len is zero */
    ret = cryp_hash_process(ot_ctx, chunk, chunk_len, src);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_hash_process, ret);
    }

    func_exit();
    return ret;
}

static td_s32 cryp_hash_finish(td_void *ctx,  td_void *hash, td_u32 hash_buf_len, td_u32 *hashlen)
{
    td_s32 ret;
    cryp_hash_context *ot_ctx = ctx;
    td_u32 left;

    func_enter();
    chk_ptr_err_return(ot_ctx);

    if (ot_ctx->hash_size > hash_buf_len) {
        ot_cipher_error("input hash buffer is samller than expected hash len\n");
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    /* padding message */
    left = cryp_hash_padding(ot_ctx->block_size, ot_ctx->tail, ot_ctx->tail_len, ot_ctx->total);
    ret = cryp_hash_process(ot_ctx, ot_ctx->tail, left, HASH_CHUNCK_SRC_LOCAL);
    if (ret != TD_SUCCESS) {
        print_func_errno(cryp_hash_process, ret);
        return ret;
    }

    if (memcpy_s(hash, hash_buf_len, ot_ctx->hash, ot_ctx->hash_size) != EOK) {
        print_func_errno(memcpy_s, OT_ERR_CIPHER_FAILED_SEC_FUNC);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }
    *hashlen = ot_ctx->hash_size;

    func_exit();
    return TD_SUCCESS;
}

static td_s32 cryp_hash_set(td_void *ctx, const drv_hash_context *clone_ctx)
{
    cryp_hash_context *ot_ctx = (cryp_hash_context *)ctx;

    ot_ctx->tail_len = clone_ctx->tail_len;
    ot_ctx->total = clone_ctx->total;
    (void)memcpy_s(ot_ctx->tail, sizeof(ot_ctx->tail), clone_ctx->tail, sizeof(clone_ctx->tail));
    (void)memcpy_s(ot_ctx->hash, sizeof(ot_ctx->hash), clone_ctx->hash, sizeof(clone_ctx->hash));

    return TD_SUCCESS;
}

static td_s32 cryp_hash_get(td_void *ctx, drv_hash_context *clone_ctx)
{
    cryp_hash_context *ot_ctx = (cryp_hash_context *)ctx;

    clone_ctx->tail_len = ot_ctx->tail_len;
    clone_ctx->total = ot_ctx->total;
    (void)memcpy_s(clone_ctx->tail, sizeof(clone_ctx->tail), ot_ctx->tail, sizeof(ot_ctx->tail));
    (void)memcpy_s(clone_ctx->hash, sizeof(clone_ctx->hash), ot_ctx->hash, sizeof(ot_ctx->hash));

    return TD_SUCCESS;
}
#endif

static td_s32 func_register_hash(const hash_func *func)
{
    td_u32 i;

    func_enter();

    /* check availability */
    if ((func->create == TD_NULL) || (func->destroy == TD_NULL) ||
        (func->update == TD_NULL) || (func->finish == TD_NULL)) {
        ot_cipher_error("hash function is null.\n");
        print_errno(OT_ERR_CIPHER_INVALID_PARAM);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    ot_cipher_dbg("register hash mode %u\n", func->mode);

    /* is it already registered? */
    for (i = 0; i < HASH_FUNC_TAB_SIZE; i++) {
        if (g_hash_descriptor[i].valid && g_hash_descriptor[i].mode == func->mode) {
            func_exit();
            return TD_SUCCESS;
        }
    }

    /* find a blank spot */
    for (i = 0; i < HASH_FUNC_TAB_SIZE; i++) {
        if (!g_hash_descriptor[i].valid) {
            g_hash_descriptor[i] = *func;
            g_hash_descriptor[i].valid = TD_TRUE;
            func_exit();
            return TD_SUCCESS;
        }
    }

    func_exit();
    return OT_ERR_CIPHER_INVALID_PARAM;
}

/* hash function register */
static td_void cryp_register_hash_default(td_u32 capacity, hash_mode mode, td_u32 blocksize, td_u32 hashlen)
{
    hash_func func;

    (td_void)memset_s(&func, sizeof(hash_func), 0, sizeof(hash_func));

    /* register the hash function if supported */
    if (capacity) {
#ifdef CHIP_HASH_SUPPORT
        func.mode = mode;
        func.block_size = blocksize;
        func.size = hashlen;
        func.create = cryp_hash_create;
        func.destroy = cryp_hash_destroy;
        func.update = cryp_hash_update;
        func.finish = cryp_hash_finish;
        func.get = cryp_hash_get;
        func.set = cryp_hash_set;
        func_register_hash(&func);
#endif
    } else if (mode == HASH_MODE_SM3) {
#if defined(SOFT_SM3_SUPPORT)
        func.mode = mode;
        func.block_size = blocksize;
        func.size = hashlen;
        func.create = ext_sm3_create;
        func.destroy = ext_sm3_destroy;
        func.update = ext_sm3_update;
        func.finish = ext_sm3_finish;
        func_register_hash(&func);
#endif
    } else {
#if defined(SOFT_SHA256_SUPPORT) || defined(SOFT_SHA512_SUPPORT)
        func.mode = mode;
        func.block_size = blocksize;
        func.size = hashlen;
        func.create = mbedtls_hash_create;
        func.destroy = mbedtls_hash_destroy;
        func.update = mbedtls_hash_update;
        func.finish = mbedtls_hash_finish;
        func_register_hash(&func);
#endif
    }
    return;
}

/* hash function register */
static td_void cryp_register_all_hash(td_void)
{
    hash_capacity capacity;

    (td_void)memset_s(&capacity, sizeof(capacity), 0, sizeof(capacity));

#ifdef CHIP_HASH_SUPPORT
    /* get hash capacity */
    drv_hash_get_capacity(&capacity);
#endif

    cryp_register_hash_default(capacity.sha256, HASH_MODE_SHA256, SHA256_BLOCK_SIZE, SHA256_RESULT_SIZE);
    cryp_register_hash_default(capacity.sha384, HASH_MODE_SHA384, SHA384_BLOCK_SIZE, SHA384_RESULT_SIZE);
    cryp_register_hash_default(capacity.sha512, HASH_MODE_SHA512, SHA512_BLOCK_SIZE, SHA512_RESULT_SIZE);
    cryp_register_hash_default(capacity.sm3, HASH_MODE_SM3, SM3_BLOCK_SIZE, SM3_RESULT_SIZE);

    return;
}

hash_func *cryp_get_hash(hash_mode mode)
{
    td_u32 i;
    hash_func *template = TD_NULL;

    func_enter();

    /* find the valid function */
    for (i = 0; i < HASH_FUNC_TAB_SIZE; i++) {
        if (g_hash_descriptor[i].valid) {
            if (g_hash_descriptor[i].mode == mode) {
                template = &g_hash_descriptor[i];
                break;
            }
        }
    }

    func_exit();
    return template;
}

td_s32 cryp_hash_init(td_void)
{
    func_enter();

    g_hash_descriptor = crypto_calloc(sizeof(hash_func), HASH_FUNC_TAB_SIZE);
    if (g_hash_descriptor == TD_NULL) {
        print_func_errno(crypto_calloc, OT_ERR_CIPHER_FAILED_MEM);
        return OT_ERR_CIPHER_FAILED_MEM;
    }

#ifdef CHIP_HASH_SUPPORT
    {
        td_s32 ret;

        ret = drv_hash_init();
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_hash_init, ret);
            crypto_free(g_hash_descriptor);
            return ret;
        }

        ret = cryp_hash_create_mem();
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_hash_create_mem, ret);
            crypto_free(g_hash_descriptor);
            drv_hash_deinit();
            return ret;
        }
    }
#endif

    /* hash function register */
    cryp_register_all_hash();

    func_exit();
    return TD_SUCCESS;
}

td_void cryp_hash_deinit(td_void)
{
    func_enter();

#ifdef CHIP_HASH_SUPPORT
    {
        td_s32 ret;
        ret = drv_hash_deinit();
        if (ret != TD_SUCCESS) {
            print_func_errno(drv_hash_deinit, ret);
        }

        ret = cryp_hash_destroy_mem();
        if (ret != TD_SUCCESS) {
            print_func_errno(cryp_hash_destroy_mem, ret);
        }
    }
#endif

    crypto_free(g_hash_descriptor);
    func_exit();
}
