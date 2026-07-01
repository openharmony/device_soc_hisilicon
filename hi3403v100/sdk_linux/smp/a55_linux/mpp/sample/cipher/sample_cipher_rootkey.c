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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

#include <securec.h>

#include "ot_type.h"
#include "ss_mpi_cipher.h"
#include "ot_common_cipher.h"
#include "ss_mpi_klad.h"
#include "ot_common_klad.h"
#include "ss_mpi_sys.h"
#include "sample_utils.h"

#define TEST_MULTI_PKG_NUM  16
#define SESSION_KEY_LEN     16
#define CONTENT_KEY_LEN     16
#define IV_LEN              16
#define DATA_LEN            16
#define MAX_AAD_LEN         32
#define AAD_LEN             16
#define TAG_LEN             16

#define MAX_SESSION_KEY     32
#define MAX_CONTENT_KEY     32
#define MAX_DATA_LEN        128
#define MAX_AAD_LEN         32
#define MAX_TAG_LEN         16

#define AES128_KEY_LEN      16
#define AES192_KEY_LEN      24
#define AES256_KEY_LEN      32

typedef struct {
    td_phys_addr_t in_phys;
    td_phys_addr_t out_phys;
    td_phys_addr_t aad_phys;
    td_u8 *in_virt;
    td_u8 *out_virt;
    td_u8 *aad_virt;
    td_u32 byte_len;
    td_u32 aad_len;
} cipher_addr_t;

typedef struct {
    char *name;
    ot_klad_alg_type session_alg;
    td_u8 session_key[MAX_SESSION_KEY];
    td_u32 session_len;
    ot_klad_alg_type content_alg;
    td_u8 content_key[MAX_CONTENT_KEY];
    td_u32 content_len;
    td_u8 iv[IV_LEN];
    td_u32 iv_len;
    td_u8 src_data[MAX_DATA_LEN];
    td_u32 data_len;
    ot_klad_rootkey_sel key_sel;
    ot_cipher_alg alg;
    ot_cipher_work_mode mode;
    td_u32 owner_id;
} common_cipher_data_t;

typedef struct {
    common_cipher_data_t data;
    td_u8 aad[MAX_AAD_LEN];
    td_u32 aad_len;
    td_u8 tag[MAX_TAG_LEN];
    td_u32 tag_len;
} cipher_data_ex_t;

/* aes ecb/cbc/cfb/ofb/ctr */
static common_cipher_data_t g_aes_data[] = {
    {
        .name = "AES-CBC-1-AES128-2-AES128",
        .session_alg = OT_KLAD_ALG_TYPE_AES, .session_len = SESSION_KEY_LEN,
        .content_alg = OT_KLAD_ALG_TYPE_AES, .content_len = CONTENT_KEY_LEN,
        .iv_len = IV_LEN, .data_len = DATA_LEN,
        .key_sel = OT_KLAD_ROOTKEY_SEL_OEM1,
        .alg = OT_CIPHER_ALG_AES,
        .mode = OT_CIPHER_WORK_MODE_CBC,
    }
};

static td_void cipher_free(cipher_addr_t *addr)
{
    if (addr->in_virt != TD_NULL) {
        ss_mpi_sys_mmz_free(addr->in_phys, addr->in_virt);
    }

    if (addr->out_virt != TD_NULL) {
        ss_mpi_sys_mmz_free(addr->out_phys, addr->out_virt);
    }

    if (addr->aad_virt != TD_NULL) {
        ss_mpi_sys_mmz_free(addr->aad_phys, addr->aad_virt);
    }
}

static td_s32 cipher_alloc(cipher_addr_t *addr)
{
    td_s32 ret;

    if (addr->byte_len != 0) {
        ret = ss_mpi_sys_mmz_alloc(&addr->in_phys, (td_void **)&addr->in_virt, NULL, NULL, addr->byte_len);
        if (ret != TD_SUCCESS) {
            sample_err("call ss_mpi_sys_mmz_alloc failed\n");
            goto exit__;
        }

        ret = ss_mpi_sys_mmz_alloc(&addr->out_phys, (td_void **)&addr->out_virt, NULL, NULL, addr->byte_len);
        if (ret != TD_SUCCESS) {
            sample_err("call ss_mpi_sys_mmz_alloc failed\n");
            goto exit__;
        }
    }

    if (addr->aad_len != 0) {
        ret = ss_mpi_sys_mmz_alloc(&addr->aad_phys, (td_void **)&addr->aad_virt, NULL, NULL, addr->aad_len);
        if (ret != TD_SUCCESS) {
            sample_err("call ss_mpi_sys_mmz_alloc failed\n");
            goto exit__;
        }
    }

    return TD_SUCCESS;

exit__:
    cipher_free(addr);
    return TD_FAILURE;
}


td_s32 cipher_set_root_key(td_handle cipher, td_handle handle_ks, common_cipher_data_t *data)
{
    td_s32 ret = TD_SUCCESS;
    td_handle handle_klad = 0;
    ot_klad_attr attr_klad = {0};
    ot_klad_session_key key0 = {0};
    ot_klad_content_key key1 = {0};

    sample_chk_expr_return(ss_mpi_klad_init(), TD_SUCCESS);

    sample_chk_expr_goto(ss_mpi_klad_create(&handle_klad), TD_SUCCESS, KLAD_DEINIT);

    attr_klad.klad_cfg.klad_type = OT_KLAD_TYPE_COMMON;
    attr_klad.klad_cfg.rootkey_attr.key_secure = OT_KLAD_ROOTKEY_SEC_REE;
    attr_klad.klad_cfg.rootkey_attr.key_sel = data->key_sel;
    attr_klad.klad_cfg.rootkey_attr.owner_id = data->owner_id;

    sample_chk_expr_goto(ss_mpi_klad_create(&handle_klad), TD_SUCCESS, KLAD_DESTROY);

    sample_chk_expr_goto(ss_mpi_klad_set_attr(handle_klad, &attr_klad), TD_SUCCESS, KLAD_DESTROY);

    sample_chk_expr_goto(ss_mpi_klad_attach(handle_klad, handle_ks), TD_SUCCESS, KLAD_DESTROY);

    key0.alg = data->session_alg;
    key0.level = OT_KLAD_LEVEL1;
    key0.key_size = data->session_len;
    sample_chk_expr_goto(memcpy_s(key0.key, sizeof(key0.key), data->session_key, data->session_len),
        0, KLAD_DETACH);

    sample_chk_expr_goto(ss_mpi_klad_set_session_key(handle_klad, &key0), TD_SUCCESS, KLAD_DETACH);
    key1.crypto_alg = OT_KLAD_CRYPTO_ALG_AES;
    key1.alg = data->content_alg;
    key1.key_size = data->content_len;

    sample_chk_expr_goto(memcpy_s(key1.key, sizeof(key1.key), data->session_key, data->session_len),
        0, KLAD_DETACH);

    sample_chk_expr_goto(ss_mpi_klad_set_content_key(handle_klad, &key1), TD_SUCCESS, KLAD_DETACH);

KLAD_DETACH:
    ss_mpi_klad_detach(handle_klad, handle_ks);
KLAD_DESTROY:
    ss_mpi_klad_destroy(handle_klad);
KLAD_DEINIT:
    ss_mpi_klad_deinit();
    return ret;
}

static td_s32 cipher_set_cfg(td_handle cipher, td_handle handle_ks, common_cipher_data_t *data)
{
    ot_cipher_ctrl cipher_ctrl = {0};
    ot_cipher_ctrl_aes *ctrl_aes = TD_NULL;

    cipher_ctrl.alg = data->alg;
    cipher_ctrl.work_mode = data->mode;
    if ((data->alg == OT_CIPHER_ALG_AES) &&
        (data->mode == OT_CIPHER_WORK_MODE_ECB || data->mode == OT_CIPHER_WORK_MODE_CBC ||
         data->mode == OT_CIPHER_WORK_MODE_CFB || data->mode == OT_CIPHER_WORK_MODE_OFB ||
         data->mode == OT_CIPHER_WORK_MODE_CTR)) {
        ctrl_aes = &cipher_ctrl.aes_ctrl;
        ctrl_aes->bit_width = OT_CIPHER_BIT_WIDTH_128BIT;
        if (data->content_len == AES128_KEY_LEN) {
            ctrl_aes->key_len = OT_CIPHER_KEY_AES_128BIT;
        } else if (data->content_len == AES192_KEY_LEN) {
            ctrl_aes->key_len = OT_CIPHER_KEY_AES_192BIT;
        } else if (data->content_len == AES256_KEY_LEN) {
            ctrl_aes->key_len = OT_CIPHER_KEY_AES_256BIT;
        }

        if (cipher_ctrl.work_mode != OT_CIPHER_WORK_MODE_ECB) {
            ctrl_aes->chg_flags = OT_CIPHER_IV_CHG_ALL_PACK;
            sample_chk_expr_with_return(memcpy_s(ctrl_aes->iv, sizeof(ctrl_aes->iv), data->iv, data->iv_len),
                0, TD_FAILURE);
        }
    } else {
        sample_err("Invalid alg.\n");
        return TD_FAILURE;
    }

    /* 1. cipher set cfg */
    sample_chk_expr_return(ss_mpi_cipher_set_cfg(cipher, &cipher_ctrl), TD_SUCCESS);

    /* 2. set clear key */
    sample_chk_expr_return(cipher_set_root_key(cipher, handle_ks, data), TD_SUCCESS);

    return TD_SUCCESS;
}

/* phy address crypto data using special chn */
static td_s32 sample_phy_crypto(common_cipher_data_t *data)
{
    td_s32 ret = TD_SUCCESS;
    td_handle handle = 0;
    ot_cipher_attr cipher_attr;
    ot_keyslot_attr keyslot_attr = {0};
    td_handle handle_ks = 0;
    cipher_addr_t cipher_addr = {0};
    cipher_addr.byte_len = data->data_len;

    sample_chk_expr_return(cipher_alloc(&cipher_addr), TD_SUCCESS);

    /* 1. cipher init */
    sample_chk_expr_goto(ss_mpi_cipher_init(), TD_SUCCESS, __CIPHER_FREE__);

    /* 2. cipher create handle */
    cipher_attr.cipher_type = OT_CIPHER_TYPE_NORMAL;
    sample_chk_expr_goto(ss_mpi_cipher_create(&handle, &cipher_attr), TD_SUCCESS, __CIPHER_DEINIT__);

    /* 3. create keyslot handle */
    keyslot_attr.secure_mode = OT_KEYSLOT_SECURE_MODE_NONE;
    keyslot_attr.type = OT_KEYSLOT_TYPE_MCIPHER;
    sample_chk_expr_goto(ss_mpi_keyslot_create(&keyslot_attr, &handle_ks), TD_SUCCESS, __CIPHER_DESTROY__);

    /* 4. attach cipher handle & kslot handle */
    sample_chk_expr_goto(ss_mpi_cipher_attach(handle, handle_ks), TD_SUCCESS, __KEYSLOT_DESTROY__);
    /* 5. for encrypt */
    /* 5.1 set cfg for encrypt */
    sample_chk_expr_goto(cipher_set_cfg(handle, handle_ks, data), TD_SUCCESS, __CIPHER_DETACH__);

    /* 5.2 encrypt */
    sample_chk_expr_goto_with_ret(memcpy_s(cipher_addr.in_virt, cipher_addr.byte_len, data->src_data, data->data_len),
        TD_SUCCESS, ret, TD_FAILURE, __CIPHER_DETACH__);
    (td_void)memset_s(cipher_addr.out_virt, cipher_addr.byte_len, 0, cipher_addr.byte_len);
    sample_chk_expr_goto(ss_mpi_cipher_encrypt(handle, cipher_addr.in_phys, cipher_addr.out_phys, cipher_addr.byte_len),
        TD_SUCCESS, __CIPHER_DETACH__);

    /* 6. for decrypt */
    /* 6.1 set cfg for decrypt */
    sample_chk_expr_goto(cipher_set_cfg(handle, handle_ks, data), TD_SUCCESS, __CIPHER_DETACH__);

    /* 6.2 decrypt */
    sample_chk_expr_goto_with_ret(
        memcpy_s(cipher_addr.in_virt, cipher_addr.byte_len, cipher_addr.out_virt, cipher_addr.byte_len),
        EOK, ret, TD_FAILURE, __CIPHER_DETACH__);

    (td_void)memset_s(cipher_addr.out_virt, cipher_addr.byte_len, 0, cipher_addr.byte_len);
    sample_chk_expr_goto(
        ss_mpi_cipher_decrypt(handle, cipher_addr.in_phys, cipher_addr.out_phys, cipher_addr.byte_len),
        TD_SUCCESS, __CIPHER_DETACH__);

    /* 6.3 compare */
    sample_chk_expr_goto_with_ret(memcmp(cipher_addr.out_virt, data->src_data, data->data_len),
        0, ret, TD_FAILURE, __CIPHER_DETACH__);

__CIPHER_DETACH__:
    ss_mpi_cipher_detach(handle, handle_ks);
__KEYSLOT_DESTROY__:
    ss_mpi_keyslot_destroy(handle_ks);
__CIPHER_DESTROY__:
    ss_mpi_cipher_destroy(handle);
__CIPHER_DEINIT__:
    ss_mpi_cipher_deinit();

__CIPHER_FREE__:
    cipher_free(&cipher_addr);
    return ret;
}

static td_s32 data_init(td_void)
{
    td_u32 i;
    td_u32 num = 0;
    /* 1. init g_aes_data */
    num = array_size(g_aes_data);
    for (i = 0; i < num; i++) {
        sample_chk_expr_return(get_random_data(g_aes_data[i].session_key, sizeof(g_aes_data[i].session_key)),
            TD_SUCCESS);
        sample_chk_expr_return(get_random_data(g_aes_data[i].content_key, sizeof(g_aes_data[i].content_key)),
            TD_SUCCESS);
        sample_chk_expr_return(get_random_data(g_aes_data[i].iv, sizeof(g_aes_data[i].iv)), TD_SUCCESS);
        sample_chk_expr_return(get_random_data(g_aes_data[i].src_data, sizeof(g_aes_data[i].src_data)), TD_SUCCESS);
        sample_chk_expr_return(get_random_data((td_u8 *)(&g_aes_data[i].owner_id), sizeof(td_u32)), TD_SUCCESS);
    }
    return TD_SUCCESS;
}

static td_s32 sample_aes(td_void)
{
    td_u32 i;
    td_s32 ret;
    sample_chk_expr_return(data_init(), TD_SUCCESS);
    td_u32 num = array_size(g_aes_data);
    for (i = 0; i < num; i++) {
        ret = sample_phy_crypto(&g_aes_data[i]);
        if (ret != TD_SUCCESS) {
            sample_err("************ test phy %s failed ************\n", g_aes_data[i].name);
            return ret;
        }
        sample_log("************ test phy %s success ************\n", g_aes_data[i].name);
    }

    return TD_SUCCESS;
}

td_s32 sample_cipher_rootkey(td_void)
{
    td_s32 ret;

    sample_log("************ test cipher rootkey ************\n");
    ret = sample_aes();
    if (ret != TD_SUCCESS) {
        return ret;
    }
    sample_log("************ test cipher rootkey success ************\n");
    return TD_SUCCESS;
}

