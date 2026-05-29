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

#ifndef CRYP_SYMC_H
#define CRYP_SYMC_H

#include "drv_osal_lib.h"
#include "drv_symc.h"

/******************************* API Code *****************************/
/*
 * brief          Initialize crypto of symc
 */
td_s32 cryp_symc_init(td_void);

/*
 * brief          Deinitialize crypto of symc
 */
td_void cryp_symc_deinit(td_void);

/*
 * brief          Create symc handle
 *
 * param handle   symc handle to be initialized
 * param chn      symc channel
 */
typedef td_void *(*func_symc_create)(td_u32 hard_chn);

/**
 * brief          Clear symc context
 *
 * param handle      symc handle to be destroy
 */
typedef td_s32 (*func_symc_destroy)(td_void *ctx);

/**
 * brief          symc key schedule
 *
 * param handle   symc handle
 * param[in]      fkey first  key buffer, default
 * param[in]      skey second key buffer, expand
 * param ot_klen input key type, output key length in bytes
 *
 * return         0 if successful, or OT_SYMC_ERR_KEY_LEN_INVALID
 */
typedef td_s32 (*func_symc_setkey)(td_void *ctx, td_u32 *ot_klen);

/**
 * brief          Symc iv schedule
 *
 * param handle   symc handle
 * param IV       Symc IV
 * param ivlen    length of iv
 *
 * return         0 if successful.
 */
typedef td_s32 (*func_symc_setiv)(td_void *ctx, const td_u8 *iv, td_u32 ivlen, td_u32 usage);

/**
 * brief          Symc iv schedule
 *
 * param handle   symc handle
 * param IV       Symc IV
 * param ivlen    must be 128, 192 or 256
 *
 * return         0 if successful.
 */
typedef td_s32 (*func_symc_getiv)(const td_void *ctx, td_u8 *iv, td_u32 iv_buf_len, td_u32 *ivlen);

/**
 * brief          SYMC alg and mode
 *
 * param handle   SYMC handle
 * param alg      Symmetric cipher alg
 * param mode     Symmetric cipher mode
 * param keybits  must be 128, 192 or 256
 *
 * return         0 if successful.
 */
typedef td_void (*func_symc_setmode)(td_void *ctx, symc_alg alg, symc_mode mode, symc_width width);

/**
 * brief          SYMC wait done
 *
 * param ctx      SYMC handle
 * return         0 if successful.
 */
typedef td_s32 (*func_symc_wait_done)(td_void *ctx, td_u32 timeout);

/**
 * brief          SYMC alg and mode
 *
 * param handle   SYMC handle
 * param round    SM1 round number
 *
 * return         0 if successful.
 */
typedef td_s32 (*func_symc_sm1_setround)(td_void *ctx, td_u32 round);

/**
 * brief          symc  buffer encryption/decryption.
 *
 * Note: Due to the nature of aes you should use the same key schedule for
 * both encryption and decryption.
 *
 * param ctx       symc ctx
 * param operation decrypt or encrypt
 * param pack     package for encrypt or decrypt.
 * param wait     last or not
 *
 * return         0 if successful
 */
typedef td_s32 (*func_symc_crypto)(td_void *ctx, td_u32 operation, symc_multi_pack *pack, td_u32 wait);

/**
 * brief          CCM/GCM set Associated Data
 *
 * param ctx      SYMC handle
 * param aad      Associated Data
 * param alen     Associated Data Length
 * param tlen     Tag length
 *
 * return         0 if successful.
 */
typedef td_s32 (*func_aead_set_aad)(td_void *ctx, compat_addr aad, td_u32 alen, td_u32 tlen);

/**
 * brief          SYMC multiple buffer encryption/decryption.
 * param[in]  id The channel number.
 * param[in]  tag tag data of CCM/GCM
 * param uuid uuid The user identification.
 *
 * return         0 if successful
 */
typedef td_s32 (*func_aead_get_tag)(const td_void *ctx, td_u32 tag[AEAD_TAG_SIZE_IN_WORD], td_u32 *taglen);

/* struct of Symmetric cipher function template */
typedef struct {
    td_u32 valid;               /* vliad or not */
    symc_alg alg;               /* Alg of Symmetric cipher */
    symc_mode mode;             /* Mode of Symmetric cipher */
    func_symc_setmode setmode;  /* Set mode function */
    func_symc_sm1_setround setround; /* SM1 set round function */
    func_symc_create  create;   /* Create function */
    func_symc_destroy destroy;  /* destroy function */
    func_symc_setkey  setkey;   /* setkey function */
    func_symc_setiv   setiv;    /* setiv function */
    func_symc_getiv   getiv;    /* getiv function */
    func_aead_set_aad setadd;   /* setadd function */
    func_aead_get_tag gettag;   /* get tag function */
    func_symc_crypto  crypto;   /* crypto function */
    func_symc_wait_done waitdone; /* wait done */
} symc_func;

/*
 * brief  symc alloc channel.
 * param[out]  hard_chn symc channel.
 * retval     On success, func is returned.  On error, TD_NULL is returned.
 */
td_s32 cryp_symc_alloc_chn(td_u32 *hard_chn, ot_cipher_type type);

/*
 * brief  symc free channel.
 * param[in]  hard_chn symc channel.
 * retval     On success, func is returned.  On error, TD_NULL is returned.
 */
td_void cryp_symc_free_chn(td_u32 hard_chn);

/**
\brief  set the key source to the symc module.
\param[in]  ctx SYMC handle.
\param[in]  src The key source.
\retval     NA.
 */
void cryp_symc_set_key_source(const td_void *ctx, td_u32 keyslot);

/*
 * brief  Clone the function from template of aes engine.
 * param[in]  alg The alg of Symmetric cipher.
 * param[in]  mode The work mode.
 * retval     On success, func is returned.  On error, TD_NULL is returned.
 */
symc_func *cryp_get_symc_op(ot_cipher_alg alg, ot_cipher_work_mode mode);

#endif /* CRYP_SYMC_H */
