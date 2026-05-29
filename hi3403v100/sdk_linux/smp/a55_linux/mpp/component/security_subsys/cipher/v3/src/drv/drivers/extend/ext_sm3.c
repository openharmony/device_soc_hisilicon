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

#ifdef SOFT_SM3_SUPPORT

/*************************** Internal Structure Definition ****************************/
/* \addtogroup      sm3 */
/* @{ */  /* <!-- [sm3] */

#define ROTATE_DIVIDER 32

static td_u32 rotateleft(td_u32 x, td_u32 n)
{
    n %= ROTATE_DIVIDER;

    if (n == 0) {
        return x;
    }

    return (x << n) | (x >> (ROTATE_DIVIDER - n));
}

#define p0(x) ((x) ^  rotateleft((x), 9)  ^ rotateleft((x), 17))
#define p1(x) ((x) ^  rotateleft((x), 15) ^ rotateleft((x), 23))

#define ff0(x, y, z) ((x) ^ (y) ^ (z))
#define ff1(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))

#define gg0(x, y, z) ((x) ^ (y) ^ (z))
#define gg1(x, y, z) (((x) & (y)) | ((~(x)) & (z)))

#define SM3_BLOCK_SIZE            64
#define SM3_W_SIZE                (SM3_BLOCK_SIZE + WORD_WIDTH)
#define SM3_RESULT_SIZE_IN_WORD   (SM3_RESULT_SIZE / WORD_WIDTH)
#define SM3_PAD_MIN_SIZE          9
#define SM3_PAD_LEN_SIZE          8
#define SM3_BYTE_MSB              0x80

/* SM3, the initial hash value, H(0) */
#define SM3_H0    0x7380166F
#define SM3_H1    0x4914B2B9
#define SM3_H2    0x172442D7
#define SM3_H3    0xDA8A0600
#define SM3_H4    0xA96F30BC
#define SM3_H5    0x163138AA
#define SM3_H6    0xE38DEE4D
#define SM3_H7    0xB0FB0E4E

/**
 * \brief          aes ccm context structure
 */
typedef struct {
    td_u32 state[SM3_RESULT_SIZE_IN_WORD];
    td_u8  tail[SM3_BLOCK_SIZE];
    td_u32 tail_len;
    td_u32 total;
} ext_sm3_context;

typedef struct {
    td_u32 a;
    td_u32 b;
    td_u32 c;
    td_u32 d;
    td_u32 e;
    td_u32 f;
    td_u32 g;
    td_u32 h;
} sm3_round_state;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API Code *****************************/
/** \addtogroup      sm3 */
/** @{ */  /** <!-- [sm3] */

static void sm3_load_round_state(sm3_round_state *round, const ext_sm3_context *ctx)
{
    td_s32 k = 0;

    round->a = ctx->state[k++];
    round->b = ctx->state[k++];
    round->c = ctx->state[k++];
    round->d = ctx->state[k++];
    round->e = ctx->state[k++];
    round->f = ctx->state[k++];
    round->g = ctx->state[k++];
    round->h = ctx->state[k++];
}

static void sm3_update_state(ext_sm3_context *ctx, const sm3_round_state *round)
{
    td_s32 k = 0;

    ctx->state[k++] ^= round->a;
    ctx->state[k++] ^= round->b;
    ctx->state[k++] ^= round->c;
    ctx->state[k++] ^= round->d;
    ctx->state[k++] ^= round->e;
    ctx->state[k++] ^= round->f;
    ctx->state[k++] ^= round->g;
    ctx->state[k++] ^= round->h;
}

static void sm3_prepare_message(td_u32 *w, td_u32 *w1, const td_u32 *pblock)
{
    td_s32 j;

    for (j = 0; j < 16; j++) { /* 16 */
        w[j] = crypto_cpu_to_be32(pblock[j]);
    }
    for (j = 16; j < SM3_W_SIZE; j++) { /* 16 */
        w[j] = p1(w[j - 16] ^ w[j - 9] ^ rotateleft(w[j - 3], 15)) /* 16 9 3 15 */
            ^ rotateleft(w[j - 13], 7) ^ w[j - 6]; /* 13 7 6 */
    }
    for (j = 0; j < SM3_BLOCK_SIZE; j++) {
        w1[j] = w[j] ^ w[j + 4]; /* 4 */
    }
}

static void sm3_round_0_15(sm3_round_state *round, td_u32 *w, td_u32 *w1, td_u32 *t)
{
    td_s32 j;
    td_u32 ss1;
    td_u32 ss2;
    td_u32 tt1;
    td_u32 tt2;

    for (j = 0; j < 16; j++) { /* 16 */
        t[j] = 0x79CC4519;
        ss1 = rotateleft((rotateleft(round->a, 12) + round->e + rotateleft(t[j], j)), 7); /* 12 7 */
        ss2 = ss1 ^ rotateleft(round->a, 12); /* 12 */
        tt1 = ff0(round->a, round->b, round->c) + round->d + ss2 + w1[j];
        tt2 = gg0(round->e, round->f, round->g) + round->h + ss1 + w[j];
        round->d = round->c;
        round->c = rotateleft(round->b, 9); /* 9 */
        round->b = round->a;
        round->a = tt1;
        round->h = round->g;
        round->g = rotateleft(round->f, 19); /* 19 */
        round->f = round->e;
        round->e = p0(tt2);
    }
}

static void sm3_round_16_63(sm3_round_state *round, td_u32 *w, td_u32 *w1, td_u32 *t)
{
    td_s32 j;
    td_u32 ss1;
    td_u32 ss2;
    td_u32 tt1;
    td_u32 tt2;

    for (j = 16; j < SM3_BLOCK_SIZE; j++) { /* 16 */
        t[j] = 0x7A879D8A;
        ss1 = rotateleft((rotateleft(round->a, 12) + round->e + rotateleft(t[j], j)), 7); /* 12 7 */
        ss2 = ss1 ^ rotateleft(round->a, 12); /* 12 */
        tt1 = ff1(round->a, round->b, round->c) + round->d + ss2 + w1[j];
        tt2 = gg1(round->e, round->f, round->g) + round->h + ss1 + w[j];
        round->d = round->c;
        round->c = rotateleft(round->b, 9); /* 9 */
        round->b = round->a;
        round->a = tt1;
        round->h = round->g;
        round->g = rotateleft(round->f, 19); /* 19 */
        round->f = round->e;
        round->e = p0(tt2);
    }
}

static td_s32 sm3_compress(ext_sm3_context *ctx, const td_u8 *block, td_u32 block_len)
{
    td_u32 *w = TD_NULL;
    td_u32 *w1 = TD_NULL;
    td_u32 *t = TD_NULL;
    const td_u32 *pblock = (const td_u32 *)block;
    td_u32 *buffer = TD_NULL;
    td_u32 buffer_size = sizeof(td_u32) * (SM3_W_SIZE + SM3_BLOCK_SIZE + SM3_BLOCK_SIZE);
    sm3_round_state round = {0};

    chk_param_err_return(block_len != SM3_BLOCK_SIZE);

    buffer = (td_u32 *)crypto_malloc(buffer_size);
    if (buffer == TD_NULL) {
        print_func_errno(crypto_malloc, OT_ERR_CIPHER_FAILED_MEM);
        return OT_ERR_CIPHER_FAILED_MEM;
    }
    (td_void)memset_s(buffer, buffer_size, 0, buffer_size);

    w = buffer;
    w1 = buffer + SM3_W_SIZE;
    t = buffer + SM3_W_SIZE + SM3_BLOCK_SIZE;
    sm3_load_round_state(&round, ctx);
    sm3_prepare_message(w, w1, pblock);
    sm3_round_0_15(&round, w, w1, t);
    sm3_round_16_63(&round, w, w1, t);
    sm3_update_state(ctx, &round);

    crypto_free(buffer);

    return TD_SUCCESS;
}

static void sm3_init(ext_sm3_context *ctx)
{
    int i = 0;
    func_enter();

    ctx->state[i++] = SM3_H0;
    ctx->state[i++] = SM3_H1;
    ctx->state[i++] = SM3_H2;
    ctx->state[i++] = SM3_H3;
    ctx->state[i++] = SM3_H4;
    ctx->state[i++] = SM3_H5;
    ctx->state[i++] = SM3_H6;
    ctx->state[i++] = SM3_H7;

    func_exit();

    return;
}

static td_s32 sm3_update(ext_sm3_context *ctx, const td_u8 *data, td_u32 data_len)
{
    td_u32 left;
    td_s32 ret;

    func_enter();

    ctx->total += data_len;

    if (ctx->tail_len) {
        left = SM3_BLOCK_SIZE - ctx->tail_len;
        if (data_len < left) {
            ret = memcpy_s(ctx->tail + ctx->tail_len, left, data, data_len);
            if (ret != TD_SUCCESS) {
                print_func_errno(memcpy_s, ret);
                return ret;
            }
            ctx->tail_len += data_len;
            return TD_SUCCESS;
        } else {
            ret = memcpy_s(ctx->tail + ctx->tail_len, left, data, left);
            if (ret != TD_SUCCESS) {
                print_func_errno(memcpy_s, ret);
                return ret;
            }
            ret = sm3_compress(ctx, ctx->tail, SM3_BLOCK_SIZE);
            if (ret != TD_SUCCESS) {
                print_func_errno(sm3_compress, ret);
                return ret;
            }

            data += left;
            data_len -= left;
        }
    }

    while (data_len >= SM3_BLOCK_SIZE) {
        ret = sm3_compress(ctx, data, SM3_BLOCK_SIZE);
        if (ret != TD_SUCCESS) {
            print_func_errno(sm3_compress, ret);
            return ret;
        }

        data += SM3_BLOCK_SIZE;
        data_len -= SM3_BLOCK_SIZE;
    }

    ctx->tail_len = data_len;
    if (data_len) {
        ret = memcpy_s(ctx->tail, SM3_BLOCK_SIZE, data, data_len);
        if (ret != TD_SUCCESS) {
            print_func_errno(memcpy_s, ret);
            return ret;
        }
    }

    func_exit();

    return TD_SUCCESS;
}

static td_s32 sm3_final(ext_sm3_context *ctx, td_u8 *digest, td_u32 digest_buf_len)
{
    td_s32 i;
    td_s32 ret;
    td_u32 hash[SM3_RESULT_SIZE_IN_WORD] = {0};

    ctx->tail[ctx->tail_len] = SM3_BYTE_MSB;

    /* a block is enough */
    if (ctx->tail_len + SM3_PAD_MIN_SIZE <= SM3_BLOCK_SIZE) {
        ret = memset_s(ctx->tail + ctx->tail_len + 1, SM3_BLOCK_SIZE - ctx->tail_len - 1, 0,
            SM3_BLOCK_SIZE - ctx->tail_len - 1);
        if (ret != TD_SUCCESS) {
            print_func_errno(memset_s, ret);
            return ret;
        }
    } else {
        /* 2 block is request */
        ret = memset_s(ctx->tail + ctx->tail_len + 1, SM3_BLOCK_SIZE - ctx->tail_len - 1,
            0, SM3_BLOCK_SIZE - ctx->tail_len - 1);
        if (ret != TD_SUCCESS) {
            print_func_errno(memset_s, ret);
            return ret;
        }

        ret = sm3_compress(ctx, ctx->tail, SM3_BLOCK_SIZE);
        if (ret != TD_SUCCESS) {
            print_func_errno(sm3_compress, ret);
            return ret;
        }

        ret = memset_s(ctx->tail, SM3_BLOCK_SIZE, 0, SM3_BLOCK_SIZE);
        if (ret != TD_SUCCESS) {
            print_func_errno(memset_s, ret);
            return ret;
        }
    }

    /* write 8 bytes fix data length * 8 */
    ctx->tail[SM3_BLOCK_SIZE - 5] = (td_u8)((ctx->total >> 29) & 0x07); /* 5 29 0x07 */
    ctx->tail[SM3_BLOCK_SIZE - 4] = (td_u8)((ctx->total >> 21) & 0xff); /* 4 21 0xff */
    ctx->tail[SM3_BLOCK_SIZE - 3] = (td_u8)((ctx->total >> 13) & 0xff); /* 3 13 0xff */
    ctx->tail[SM3_BLOCK_SIZE - 2] = (td_u8)((ctx->total >> 5)  & 0xff); /* 2  5 0xff */
    ctx->tail[SM3_BLOCK_SIZE - 1] = (td_u8)((ctx->total << 3)  & 0xff); /* 1  3 0xff */

    ret = sm3_compress(ctx, ctx->tail, SM3_BLOCK_SIZE);
    if (ret != TD_SUCCESS) {
        print_func_errno(sm3_compress, ret);
        return ret;
    }

    for (i = 0; i < SM3_RESULT_SIZE_IN_WORD; i++) {
        hash[i] = crypto_cpu_to_be32(ctx->state[i]);
    }

    ret = memcpy_s(digest, digest_buf_len, hash, SM3_RESULT_SIZE);
    if (ret != TD_SUCCESS) {
        print_func_errno(memcpy_s, ret);
    }

    return ret;
}

td_void *ext_sm3_create(hash_mode mode)
{
    ext_sm3_context *ctx = TD_NULL;

    crypto_unused(mode);

    func_enter();

    ctx = crypto_malloc(sizeof(ext_sm3_context));
    if (ctx == TD_NULL) {
        print_func_errno(crypto_malloc, OT_ERR_CIPHER_FAILED_MEM);
        return TD_NULL;
    }
    (td_void)memset_s(ctx, sizeof(ext_sm3_context), 0, sizeof(ext_sm3_context));

    sm3_init(ctx);

    func_exit();

    return ctx;
}

td_s32 ext_sm3_update(td_void *ctx, const td_u8 *chunk, td_u32 chunk_len, hash_chunk_src src)
{
    td_u8 *ptr = TD_NULL;
    td_s32 ret = TD_FAILURE;
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

            ret = crypto_copy_from_user(ptr, HASH_MAX_BUFFER_SIZE, chunk + offset, length);
            if (ret != TD_SUCCESS) {
                print_func_errno(crypto_copy_from_user, ret);
                goto exit;
            }

            ret = sm3_update(ctx, ptr, length);
            if (ret != TD_SUCCESS) {
                print_func_errno(sm3_update, ret);
                goto exit;
            }

            crypto_msleep(1);
            offset += length;
        }
    } else {
        chk_ptr_err_return(chunk);
        ret = sm3_update(ctx, chunk, chunk_len);
        if (ret != TD_SUCCESS) {
            print_func_errno(sm3_update, ret);
        }
        return ret;
    }

exit:
    crypto_free(ptr);

    func_exit();

    return ret;
}

td_s32 ext_sm3_finish(td_void *ctx, td_void *hash, td_u32 hash_buf_len, td_u32 *hashlen)
{
    td_s32 ret;
    func_enter();

    chk_ptr_err_return(ctx);

    ret = sm3_final(ctx, hash, hash_buf_len);
    if (ret != TD_SUCCESS) {
        print_func_errno(sm3_final, ret);
        return ret;
    }
    *hashlen = SM3_RESULT_SIZE;

    func_exit();
    return TD_SUCCESS;
}

td_s32 ext_sm3_destroy(td_void *ctx)
{
    func_enter();

    chk_ptr_err_return(ctx);

    crypto_free(ctx);

    func_exit();
    return TD_SUCCESS;
}

#endif
/** @} */  /** <!-- ==== API Code end ==== */
