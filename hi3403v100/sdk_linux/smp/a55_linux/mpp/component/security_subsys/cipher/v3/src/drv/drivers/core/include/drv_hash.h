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

#ifndef DRV_HASH_H
#define DRV_HASH_H

/*************************** Structure Definition ****************************/
/** \addtogroup     hash */
/** @{ */ /** <!-- [hash] */

/* ! \hash block size */
#define HASH_BLOCK_SIZE_64   64   /* SHA1, SHA224, SHA256 */
#define HASH_BLOCK_SIZE_128  128  /* SHA384, SHA512 */

/* ! \hash capacity, 0-nonsupport, 1-support */
typedef struct {
    td_u32 sha1 : 1;   /* !<  Support SHA1 */
    td_u32 sha224 : 1; /* !<  Support SHA224 */
    td_u32 sha256 : 1; /* !<  Support SHA256 */
    td_u32 sha384 : 1; /* !<  Support SHA384 */
    td_u32 sha512 : 1; /* !<  Support SHA512 */
    td_u32 sm3 : 1;    /* !<  Support SM3 */
} hash_capacity;

/* ! \hash mode */
typedef enum {
    HASH_MODE_SHA1,   /* !<  SHA1 */
    HASH_MODE_SHA224, /* !<  SHA2 224 */
    HASH_MODE_SHA256, /* !<  SHA2 256 */
    HASH_MODE_SHA384, /* !<  SHA2 384 */
    HASH_MODE_SHA512, /* !<  SHA2 512 */
    HASH_MODE_SM3,    /* !<  SM3 */
    HASH_MODE_COUNT,
} hash_mode;

/** @} */ /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      hash */
/** @{ */ /** <!--[hash] */

/**
\brief  Initialize the hash module.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_hash_init(void);

/**
\brief  Deinitialize the hash module.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_hash_deinit(void);

/**
\brief  resume the hash module.
\retval     NA.
 */
void drv_hash_resume(void);

/**
\brief  set work params.
\param[in]  chn_num The channel number.
\param[in]  mode The hash mode.
\param[in] state The hash initial result.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_hash_cfg(td_u32 chn_num, hash_mode mode, const td_u32 state[HASH_RESULT_MAX_SIZE_IN_WORD]);

/**
\brief  start hash calculation.
\param[in]  chn_num The channel number.
\param[in]  buf_phy The MMZ/SMMU address of in buffer.
\param[in]  buf_size The MMZ/SMMU siae of in buffer.
\retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 drv_hash_start(td_u32 chn_num, const crypto_mem *mem, td_u32 buf_size);

/**
\brief  wait running finished.
\param[in]  chn_num The channel number.
\param[out] state The hash result.
\param[in]  hashLen The length of hash result.
\retval     On received interception, TD_TRUE is returned  otherwise TD_FALSE is returned.
 */
td_s32 drv_hash_wait_done(td_u32 chn_num, td_u32 *state, td_u32 len_in_word);

/**
\brief  reset hash after hash finished.
\param[in]  chn_num The channel number.
\retval     NA.
 */
void drv_hash_reset(td_u32 chn_num);

/**
\brief  compute a block hmac.
\param[in]  din block data.
\param[out] hamc The output.
\retval     On received interception, TD_TRUE is returned  otherwise TD_FALSE is returned.
 */
td_s32 drv_hmac256_block(td_u32 *din, td_u32 *hamc, td_u32 len_in_word);

/**
\brief  get the hash capacity.
\param[out] capacity The hash capacity.
\retval     NA.
 */
void drv_hash_get_capacity(hash_capacity *capacity);

/** @} */ /** <!-- ==== API declaration end ==== */

#endif /* DRV_HASH_H */
