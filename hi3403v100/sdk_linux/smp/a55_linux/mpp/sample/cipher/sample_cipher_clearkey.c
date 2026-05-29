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

#include "securec.h"
#include "ot_type.h"
#include "ss_mpi_cipher.h"
#include "ot_common_cipher.h"
#include "ss_mpi_klad.h"
#include "ot_common_klad.h"
#include "ss_mpi_sys.h"

#include "sample_utils.h"

#define TEST_MULTI_PKG_NUM  16
#define MAX_KEY_LEN         32
#define IV_LEN              16
#define MAX_DATA_LEN        128
#define MAX_AAD_LEN         32

#define AES128_KEY_LEN      16
#define AES192_KEY_LEN      24
#define AES256_KEY_LEN      32

#define TEST_DATA_LEN       32

#define CCM_IV_LEN          11  /* for ccm, iv len is 7 ~ 13 */
#define GCM_IV_LEN          16  /* for gcm, iv len is 1 ~ 16 */
#define CCM_TAG_LEN         10  /* for ccm, tag len is {4, 6, 8, 10, 12, 14, 16} */
#define GCM_TAG_LEN         16  /* for gcm, tag len is 16 */

#define TEST_AAD_LEN        16

#define MAX_TAG_LEN         (16)

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
    td_u8 key[MAX_KEY_LEN];
    td_u32 key_len;
    td_u8 iv[IV_LEN];
    td_u32 iv_len;
    td_u8 src_data[MAX_DATA_LEN];
    td_u32 data_len;
    ot_cipher_alg alg;
    ot_cipher_work_mode mode;
    ot_cipher_attr cipher_attr;
    ot_keyslot_attr keyslot_attr;
} cipher_data_t;

typedef struct {
    cipher_data_t data;
    td_u8 aad[MAX_AAD_LEN];
    td_u32 aad_len;
    td_u32 tag_len;
} cipher_data_ex_t;

/* aes ecb/cbc/cfb/ofb/ctr */
static cipher_data_t g_aes_data[] = {
    {
        .name = "AES-CBC-128BITS",
        .key_len  = AES128_KEY_LEN, .iv_len = IV_LEN, .data_len = TEST_DATA_LEN,
        .alg = OT_CIPHER_ALG_AES, .mode = OT_CIPHER_WORK_MODE_CBC,
        .cipher_attr = {
            .cipher_type = OT_CIPHER_TYPE_NORMAL
        },
        .keyslot_attr = {
            .secure_mode = OT_KEYSLOT_SECURE_MODE_NONE, .type = OT_KEYSLOT_TYPE_MCIPHER
        }
    },
    {
        .name = "AES-CBC-192BITS",
        .key_len = AES192_KEY_LEN, .iv_len = IV_LEN, .data_len = TEST_DATA_LEN,
        .alg = OT_CIPHER_ALG_AES, .mode = OT_CIPHER_WORK_MODE_CBC,
        .cipher_attr = {
            .cipher_type = OT_CIPHER_TYPE_NORMAL
        },
        .keyslot_attr = {
            .secure_mode = OT_KEYSLOT_SECURE_MODE_NONE, .type = OT_KEYSLOT_TYPE_MCIPHER
        }
    },
    {
        .name = "AES-CBC-256BITS",
        .key_len = AES256_KEY_LEN, .iv_len = IV_LEN, .data_len = TEST_DATA_LEN,
        .alg = OT_CIPHER_ALG_AES, .mode = OT_CIPHER_WORK_MODE_CBC,
        .cipher_attr = {
            .cipher_type = OT_CIPHER_TYPE_NORMAL
        },
        .keyslot_attr = {
            .secure_mode = OT_KEYSLOT_SECURE_MODE_NONE, .type = OT_KEYSLOT_TYPE_MCIPHER
        }
    },
    {
        .name = "AES-CFB-128BITS",
        .key_len = AES128_KEY_LEN, .iv_len = IV_LEN, .data_len = TEST_DATA_LEN,
        .alg = OT_CIPHER_ALG_AES, .mode = OT_CIPHER_WORK_MODE_CFB,
        .cipher_attr = {
            .cipher_type = OT_CIPHER_TYPE_NORMAL
        },
        .keyslot_attr = {
            .secure_mode = OT_KEYSLOT_SECURE_MODE_NONE, .type = OT_KEYSLOT_TYPE_MCIPHER
        }
    },
    {
        .name = "AES-CTR-128BITS",
        .key_len = AES128_KEY_LEN, .iv_len = IV_LEN, .data_len = TEST_DATA_LEN,
        .alg = OT_CIPHER_ALG_AES, .mode = OT_CIPHER_WORK_MODE_CTR,
        .cipher_attr = {
            .cipher_type = OT_CIPHER_TYPE_NORMAL
        },
        .keyslot_attr = {
            .secure_mode = OT_KEYSLOT_SECURE_MODE_NONE, .type = OT_KEYSLOT_TYPE_MCIPHER
        }
    },
};

/* aes ccm/gcm data */
static cipher_data_ex_t g_aes_data_ex[] = {
    {
        .data = {
            .name = "AES-CCM-128BITS",
            .key_len = AES128_KEY_LEN, .iv_len = CCM_IV_LEN, .data_len = TEST_DATA_LEN,
            .alg = OT_CIPHER_ALG_AES, .mode = OT_CIPHER_WORK_MODE_CCM,
            .cipher_attr = {
                .cipher_type = OT_CIPHER_TYPE_NORMAL
            },
            .keyslot_attr = {
                .secure_mode = OT_KEYSLOT_SECURE_MODE_NONE, .type = OT_KEYSLOT_TYPE_MCIPHER
            }
        },
        .aad_len = TEST_AAD_LEN, .tag_len = CCM_TAG_LEN,
    },
    {
        .data = {
            .name = "AES-GCM-128BITS",
            .key_len = AES128_KEY_LEN, .iv_len = GCM_IV_LEN, .data_len = TEST_DATA_LEN,
            .alg = OT_CIPHER_ALG_AES,
            .mode = OT_CIPHER_WORK_MODE_GCM,
            .cipher_attr = {
                .cipher_type = OT_CIPHER_TYPE_NORMAL
            },
            .keyslot_attr = {
                .secure_mode = OT_KEYSLOT_SECURE_MODE_NONE, .type = OT_KEYSLOT_TYPE_MCIPHER
            }
        },
        .aad_len = TEST_AAD_LEN, .tag_len = GCM_TAG_LEN,
    },
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

td_s32 cipher_set_clear_key(td_handle cipher,
    td_handle handle_ks, ot_cipher_alg alg, const td_u8 *key, td_u32 keylen)
{
    td_s32 ret = TD_SUCCESS;
    td_handle handle_klad = 0;
    ot_klad_attr attr_klad = {0};
    ot_klad_clear_key key_clear = {0};

    /* 1. klad init */
    sample_chk_expr_return(ss_mpi_klad_init(), TD_SUCCESS);

    /* 2. klad create handle */
    sample_chk_expr_goto(ss_mpi_klad_create(&handle_klad), TD_SUCCESS, __KLAD_DEINIT__);

    /**
    * 3. klad set attr for clear key.
    * For clear key, the rootkey_attr in attr_klad needn't be set.
    */
    attr_klad.klad_cfg.klad_type = OT_KLAD_TYPE_CLEARCW;
    sample_chk_expr_goto(ss_mpi_klad_set_attr(handle_klad, &attr_klad), TD_SUCCESS, __KLAD_DESTORY__);

    /* 4. attach klad handle & kslot handle */
    sample_chk_expr_goto(ss_mpi_klad_attach(handle_klad, handle_ks), TD_SUCCESS, __KLAD_DESTORY__);

    /* 5. set clear key */
    if (alg == OT_CIPHER_ALG_AES) {
        key_clear.crypto_alg = OT_KLAD_CRYPTO_ALG_AES;
    } else if (alg == OT_CIPHER_ALG_SM4) {
        key_clear.crypto_alg = OT_KLAD_CRYPTO_ALG_SM4;
    }
    key_clear.key_size = keylen;
    sample_chk_expr_goto_with_ret(memcpy_s(key_clear.key, key_clear.key_size, key, keylen),
        EOK, ret, TD_FAILURE, __KLAD_DESTORY__);

    sample_chk_expr_goto(ss_mpi_klad_set_clear_key(handle_klad, &key_clear), TD_SUCCESS, __KLAD_DETACH__);
__KLAD_DETACH__:
    ss_mpi_klad_detach(handle_klad, handle_ks);
__KLAD_DESTORY__:
    ss_mpi_klad_destroy(handle_klad);
__KLAD_DEINIT__:
    ss_mpi_klad_deinit();
    return ret;
}

static td_s32 cipher_set_cfg(td_handle cipher, td_handle handle_ks, cipher_data_t *data)
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
        if (data->key_len == AES128_KEY_LEN) {
            ctrl_aes->key_len = OT_CIPHER_KEY_AES_128BIT;
        } else if (data->key_len == AES192_KEY_LEN) {
            ctrl_aes->key_len = OT_CIPHER_KEY_AES_192BIT;
        } else if (data->key_len == AES256_KEY_LEN) {
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
    sample_chk_expr_return(cipher_set_clear_key(cipher, handle_ks, data->alg, data->key, data->key_len), TD_SUCCESS);

    return TD_SUCCESS;
}

static td_s32 cipher_set_cfg_ex(td_handle cipher, td_handle handle_ks,
    td_phys_addr_t aad_phys, cipher_data_ex_t *data_ex)
{
    ot_cipher_ctrl cipher_ctrl = {0};
    ot_cipher_ctrl_aes_ccm_gcm *ctrl_aes_ex = TD_NULL;
    cipher_data_t *data = &data_ex->data;

    cipher_ctrl.alg = data->alg;
    cipher_ctrl.work_mode = data->mode;
    if ((data->alg == OT_CIPHER_ALG_AES) &&
        (data->mode == OT_CIPHER_WORK_MODE_CCM || data->mode == OT_CIPHER_WORK_MODE_GCM)) {
        ctrl_aes_ex = &cipher_ctrl.aes_ccm_gcm_ctrl;
        ctrl_aes_ex->aad_phys_addr = aad_phys;
        ctrl_aes_ex->aad_len = data_ex->aad_len;
        ctrl_aes_ex->tag_len = data_ex->tag_len;
        if (data->key_len == AES128_KEY_LEN) {
            ctrl_aes_ex->key_len = OT_CIPHER_KEY_AES_128BIT;
        } else if (data->key_len == AES192_KEY_LEN) {
            ctrl_aes_ex->key_len = OT_CIPHER_KEY_AES_192BIT;
        } else if (data->key_len == AES256_KEY_LEN) {
            ctrl_aes_ex->key_len = OT_CIPHER_KEY_AES_256BIT;
        }
        ctrl_aes_ex->iv_len = data->iv_len;
        sample_chk_expr_with_return(memcpy_s(ctrl_aes_ex->iv, sizeof(ctrl_aes_ex->iv), data->iv, data->iv_len),
            0, TD_FAILURE);
    } else {
        sample_err("Invalid alg.\n");
        return TD_FAILURE;
    }

    /* 1. cipher set cfg */
    sample_chk_expr_return(ss_mpi_cipher_set_cfg(cipher, &cipher_ctrl), TD_SUCCESS);

    /* 2. set clear key */
    sample_chk_expr_return(cipher_set_clear_key(cipher, handle_ks, data->alg, data->key, data->key_len), TD_SUCCESS);

    return TD_SUCCESS;
}

/* phy address crypto data using special chn */
static td_s32 sample_phy_crypto(cipher_data_t *data)
{
    td_s32 ret = TD_SUCCESS;
    td_handle handle = 0;
    td_handle handle_ks = 0;
    cipher_addr_t cipher_addr = {0};
    cipher_addr.byte_len = data->data_len;

    sample_chk_expr_return(cipher_alloc(&cipher_addr), TD_SUCCESS);

    /* 1. cipher init */
    sample_chk_expr_goto(ss_mpi_cipher_init(), TD_SUCCESS, CIPHER_FREE);

    /* 2. cipher create handle */
    sample_chk_expr_goto(ss_mpi_cipher_create(&handle, &data->cipher_attr), TD_SUCCESS, CIPHER_DEINIT);

    /* 3. create keyslot handle */
    sample_chk_expr_goto(ss_mpi_keyslot_create(&data->keyslot_attr, &handle_ks), TD_SUCCESS, CIPHER_DESTROY);

    /* 4. attach cipher handle & kslot handle */
    sample_chk_expr_goto(ss_mpi_cipher_attach(handle, handle_ks), TD_SUCCESS, KEYSLOT_DESTROY);
    /* 5. for encrypt */
    /* 5.1 set cfg for encrypt */
    sample_chk_expr_goto(cipher_set_cfg(handle, handle_ks, data), TD_SUCCESS, CIPHER_DETACH);

    /* 5.2 encrypt */
    sample_chk_expr_goto_with_ret(memcpy_s(cipher_addr.in_virt, cipher_addr.byte_len, data->src_data, data->data_len),
        TD_SUCCESS, ret, TD_FAILURE, CIPHER_DETACH);
    (td_void)memset_s(cipher_addr.out_virt, cipher_addr.byte_len, 0, cipher_addr.byte_len);
    sample_chk_expr_goto(ss_mpi_cipher_encrypt(handle, cipher_addr.in_phys, cipher_addr.out_phys, cipher_addr.byte_len),
        TD_SUCCESS, CIPHER_DETACH);

    /* 6. for decrypt */
    /* 6.1 set cfg for decrypt */
    sample_chk_expr_goto(cipher_set_cfg(handle, handle_ks, data), TD_SUCCESS, CIPHER_DETACH);

    /* 6.2 decrypt */
    sample_chk_expr_goto_with_ret(
        memcpy_s(cipher_addr.in_virt, cipher_addr.byte_len, cipher_addr.out_virt, cipher_addr.byte_len),
        EOK, ret, TD_FAILURE, CIPHER_DETACH);

    (td_void)memset_s(cipher_addr.out_virt, cipher_addr.byte_len, 0, cipher_addr.byte_len);
    sample_chk_expr_goto(
        ss_mpi_cipher_decrypt(handle, cipher_addr.in_phys, cipher_addr.out_phys, cipher_addr.byte_len),
        TD_SUCCESS, CIPHER_DETACH);

    /* 6.3 compare */
    sample_chk_expr_goto_with_ret(memcmp(cipher_addr.out_virt, data->src_data, data->data_len),
        0, ret, TD_FAILURE, CIPHER_DETACH);

CIPHER_DETACH:
    ss_mpi_cipher_detach(handle, handle_ks);
KEYSLOT_DESTROY:
    ss_mpi_keyslot_destroy(handle_ks);
CIPHER_DESTROY:
    ss_mpi_cipher_destroy(handle);
CIPHER_DEINIT:
    ss_mpi_cipher_deinit();

CIPHER_FREE:
    cipher_free(&cipher_addr);
    return ret;
}

/* virtual address crypto data using special chn */
static td_s32 sample_virt_crypto(cipher_data_t *data)
{
    td_s32 ret = TD_SUCCESS;
    td_handle handle = 0;
    td_handle handle_ks = 0;
    cipher_addr_t cipher_addr = {0};
    cipher_addr.byte_len = data->data_len;

    sample_chk_expr_return(cipher_alloc(&cipher_addr), TD_SUCCESS);

    /* 1. cipher init */
    sample_chk_expr_goto(ss_mpi_cipher_init(), TD_SUCCESS, CIPHER_FREE);

    /* 2. cipher create handle */
    sample_chk_expr_goto(ss_mpi_cipher_create(&handle, &data->cipher_attr), TD_SUCCESS, CIPHER_DEINIT);

    /* 3. create keyslot handle */
    sample_chk_expr_goto(ss_mpi_keyslot_create(&data->keyslot_attr, &handle_ks), TD_SUCCESS, CIPHER_DESTROY);

    /* 4. attach cipher handle & kslot handle */
    sample_chk_expr_goto(ss_mpi_cipher_attach(handle, handle_ks), TD_SUCCESS, KEYSLOT_DESTROY);
    /* 5. for encrypt */
    /* 5.1 set cfg for encrypt */
    sample_chk_expr_goto(cipher_set_cfg(handle, handle_ks, data), TD_SUCCESS, CIPHER_DETACH);

    /* 5.2 encrypt */
    sample_chk_expr_goto_with_ret(memcpy_s(cipher_addr.in_virt, cipher_addr.byte_len, data->src_data, data->data_len),
        TD_SUCCESS, ret, TD_FAILURE, CIPHER_DETACH);
    (td_void)memset_s(cipher_addr.out_virt, cipher_addr.byte_len, 0, cipher_addr.byte_len);
    sample_chk_expr_goto(
        ss_mpi_cipher_encrypt_virt(handle, cipher_addr.in_virt, cipher_addr.out_virt, cipher_addr.byte_len),
        TD_SUCCESS, CIPHER_DETACH);

    /* 6. for decrypt */
    /* 6.1 set cfg for decrypt */
    sample_chk_expr_goto(cipher_set_cfg(handle, handle_ks, data), TD_SUCCESS, CIPHER_DETACH);

    /* 6.2 decrypt */
    sample_chk_expr_goto_with_ret(
        memcpy_s(cipher_addr.in_virt, cipher_addr.byte_len, cipher_addr.out_virt, cipher_addr.byte_len),
        EOK, ret, TD_FAILURE, CIPHER_DETACH);

    (td_void)memset_s(cipher_addr.out_virt, cipher_addr.byte_len, 0, cipher_addr.byte_len);
    sample_chk_expr_goto(
        ss_mpi_cipher_decrypt_virt(handle, cipher_addr.in_virt, cipher_addr.out_virt, cipher_addr.byte_len),
        TD_SUCCESS, CIPHER_DETACH);

    /* 6.3 compare */
    sample_chk_expr_goto_with_ret(memcmp(cipher_addr.out_virt, data->src_data, data->data_len),
        0, ret, TD_FAILURE, CIPHER_DETACH);

CIPHER_DETACH:
    ss_mpi_cipher_detach(handle, handle_ks);
KEYSLOT_DESTROY:
    ss_mpi_keyslot_destroy(handle_ks);
CIPHER_DESTROY:
    ss_mpi_cipher_destroy(handle);
CIPHER_DEINIT:
    ss_mpi_cipher_deinit();
CIPHER_FREE:
    cipher_free(&cipher_addr);
    return ret;
}

static td_void copy_cipher_addr_to_pkg_data(ot_cipher_data *pkg_data, const cipher_addr_t *cipher_addr)
{
    pkg_data->src_phys_addr = cipher_addr->in_phys;
    pkg_data->dst_phys_addr = cipher_addr->out_phys;
    pkg_data->byte_len = cipher_addr->byte_len;
}

static td_s32 sample_multi_pkg_crypto(cipher_data_t *data)
{
    td_s32 ret = TD_SUCCESS;
    td_u32 i;
    td_handle handle = 0;
    td_handle handle_ks;
    cipher_addr_t cipher_addr[TEST_MULTI_PKG_NUM] = {0};
    ot_cipher_data pkg_data[TEST_MULTI_PKG_NUM] = {0};

    for (i = 0; i < TEST_MULTI_PKG_NUM; i++) {
        cipher_addr[i].byte_len = data->data_len;
        sample_chk_expr_goto(cipher_alloc(&cipher_addr[i]), TD_SUCCESS, CIPHER_FREE);
        copy_cipher_addr_to_pkg_data(&pkg_data[i], &cipher_addr[i]);
    }

    /* 1. cipher init */
    sample_chk_expr_goto(ss_mpi_cipher_init(), TD_SUCCESS, CIPHER_FREE);

    /* 2. cipher create handle */
    sample_chk_expr_goto(ss_mpi_cipher_create(&handle, &data->cipher_attr), TD_SUCCESS, CIPHER_DEINIT);

    /* 3. create keyslot handle */
    sample_chk_expr_goto(ss_mpi_keyslot_create(&data->keyslot_attr, &handle_ks), TD_SUCCESS, CIPHER_DESTROY);

    /* 4. attach cipher handle & keyslot handle */
    sample_chk_expr_goto(ss_mpi_cipher_attach(handle, handle_ks), TD_SUCCESS, KEYSLOT_DESTROY);

    /* 5. for encrypt */
    /* 5.1 set cfg for encrypt */
    sample_chk_expr_goto(cipher_set_cfg(handle, handle_ks, data), TD_SUCCESS, CIPHER_DETACH);

    /* 5.2 encrypt multiple packages */
    for (i = 0; i < TEST_MULTI_PKG_NUM; i++) {
        sample_chk_expr_goto_with_ret(
            memcpy_s(cipher_addr[i].in_virt, cipher_addr[i].byte_len, data->src_data, data->data_len),
            TD_SUCCESS, ret, TD_FAILURE, CIPHER_DETACH);
    }

    sample_chk_expr_goto(ss_mpi_cipher_encrypt_multi_pack(handle, pkg_data, TEST_MULTI_PKG_NUM),
        TD_SUCCESS, CIPHER_DETACH);

    /* 6. for decrypt */
    /* 6.1 set cfg for decrypt */
    sample_chk_expr_goto(cipher_set_cfg(handle, handle_ks, data), TD_SUCCESS, CIPHER_DETACH);

    /* 6.2 decrypt multiple packages */
    for (i = 0; i < TEST_MULTI_PKG_NUM; i++) {
        sample_chk_expr_goto_with_ret(
            memcpy_s(cipher_addr[i].in_virt, cipher_addr[i].byte_len, cipher_addr[i].out_virt, cipher_addr[i].byte_len),
            TD_SUCCESS, ret, TD_FAILURE, CIPHER_DETACH);
    }

    sample_chk_expr_goto(ss_mpi_cipher_decrypt_multi_pack(handle, pkg_data, TEST_MULTI_PKG_NUM),
        TD_SUCCESS, CIPHER_DETACH);

    /* 6.3 compare */
    for (i = 0; i < TEST_MULTI_PKG_NUM; i++) {
        sample_chk_expr_goto_with_ret(
            memcmp(cipher_addr[i].out_virt, data->src_data, data->data_len), 0, ret, TD_FAILURE, CIPHER_DETACH);
    }

CIPHER_DETACH:
    ss_mpi_cipher_detach(handle, handle_ks);
KEYSLOT_DESTROY:
    ss_mpi_keyslot_destroy(handle_ks);
CIPHER_DESTROY:
    ss_mpi_cipher_destroy(handle);
CIPHER_DEINIT:
    ss_mpi_cipher_deinit();
CIPHER_FREE:
    for (i = 0; i < TEST_MULTI_PKG_NUM; i++) {
        cipher_free(&cipher_addr[i]);
    }
    return ret;
}

/* sample aes ccm/gcm */
static td_s32 sample_phy_crypto_ex(cipher_data_ex_t *data_ex)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 enc_out_tag[MAX_TAG_LEN] = {0};
    td_u8 dec_out_tag[MAX_TAG_LEN] = {0};
    td_handle handle;
    td_handle handle_ks;
    cipher_data_t *data = &data_ex->data;
    cipher_addr_t cipher_addr = { .byte_len = data->data_len, .aad_len = data_ex->aad_len };

    sample_chk_expr_goto(cipher_alloc(&cipher_addr), TD_SUCCESS, CIPHER_FREE);

    /* 1. cipher init */
    sample_chk_expr_goto(ss_mpi_cipher_init(), TD_SUCCESS, CIPHER_FREE);

    /* 2. cipher create handle */
    sample_chk_expr_goto(ss_mpi_cipher_create(&handle, &data->cipher_attr), TD_SUCCESS, CIPHER_DEINIT);

    /* 3. create keyslot handle */
    sample_chk_expr_goto(ss_mpi_keyslot_create(&data->keyslot_attr, &handle_ks), TD_SUCCESS, CIPHER_DESTROY);

    /* 4. attach cipher handle & keyslot handle */
    sample_chk_expr_goto(ss_mpi_cipher_attach(handle, handle_ks), TD_SUCCESS, KEYSLOT_DESTROY);

    /* 5. for encrypt */
    /* 5.1 set cfg for encrypt(ccm, gcm) */
    sample_chk_expr_goto_with_ret(
        memcpy_s(cipher_addr.aad_virt, cipher_addr.byte_len, data_ex->aad, data_ex->aad_len),
        TD_SUCCESS, ret, TD_FAILURE, CIPHER_DETACH);

    sample_chk_expr_goto(cipher_set_cfg_ex(handle, handle_ks, cipher_addr.aad_phys, data_ex),
        TD_SUCCESS, CIPHER_DETACH);

    /* 5.2 encrypt(ccm, gcm) */
    sample_chk_expr_goto_with_ret(memcpy_s(cipher_addr.in_virt, cipher_addr.byte_len, data->src_data, data->data_len),
        TD_SUCCESS, ret, TD_FAILURE, CIPHER_DEINIT);
    sample_chk_expr_goto(ss_mpi_cipher_encrypt(handle, cipher_addr.in_phys, cipher_addr.out_phys, cipher_addr.byte_len),
        TD_SUCCESS, CIPHER_DEINIT);

    /* 5.3 get tag after encrypt */
    sample_chk_expr_goto(ss_mpi_cipher_get_tag(handle, enc_out_tag, sizeof(enc_out_tag)),
        TD_SUCCESS, CIPHER_DETACH);

    /* 6. for decrypt */
    /* 6.1 set cfg for decrypt(ccm, gcm) */
    sample_chk_expr_goto(cipher_set_cfg_ex(handle, handle_ks, cipher_addr.aad_phys, data_ex),
        TD_SUCCESS, CIPHER_DETACH);

    /* 6.2 decrypt(ccm, gcm) */
    sample_chk_expr_goto_with_ret(
        memcpy_s(cipher_addr.in_virt, cipher_addr.byte_len, cipher_addr.out_virt, cipher_addr.byte_len),
        EOK, ret, TD_FAILURE, CIPHER_DETACH);
    sample_chk_expr_goto(
        ss_mpi_cipher_decrypt(handle, cipher_addr.in_phys, cipher_addr.out_phys, cipher_addr.byte_len),
        TD_SUCCESS, CIPHER_DETACH);

    /* 6.3 compare data */
    sample_chk_expr_goto_with_ret(memcmp(cipher_addr.out_virt, data->src_data, data->data_len),
        0, ret, TD_FAILURE, CIPHER_DETACH);

    /* 6.4 compare tag */
    sample_chk_expr_goto(ss_mpi_cipher_get_tag(handle, dec_out_tag, sizeof(dec_out_tag)),
        TD_SUCCESS, CIPHER_DETACH);

    sample_chk_expr_goto_with_ret(memcmp(enc_out_tag, dec_out_tag, data_ex->tag_len),
        0, ret, TD_FAILURE, CIPHER_DETACH);

CIPHER_DETACH:
    ss_mpi_cipher_detach(handle, handle_ks);
KEYSLOT_DESTROY:
    ss_mpi_keyslot_destroy(handle_ks);
CIPHER_DESTROY:
    ss_mpi_cipher_destroy(handle);
CIPHER_DEINIT:
    ss_mpi_cipher_deinit();
CIPHER_FREE:
    cipher_free(&cipher_addr);

    return ret;
}

static td_s32 sample_aes(td_void)
{
    td_u32 i;
    td_s32 ret;
    td_u32 num = array_size(g_aes_data);

    for (i = 0; i < num; i++) {
        ret = sample_phy_crypto(&g_aes_data[i]);
        if (ret != TD_SUCCESS) {
            sample_err("************ test phy %s failed ************\n", g_aes_data[i].name);
            return ret;
        }
        sample_log("************ test phy %s success ************\n", g_aes_data[i].name);
    }

    for (i = 0; i < num; i++) {
        ret = sample_virt_crypto(&g_aes_data[i]);
        if (ret != TD_SUCCESS) {
            sample_err("************ test virt %s failed ************\n", g_aes_data[i].name);
            return ret;
        }
        sample_log("************ test virt %s success ************\n", g_aes_data[i].name);
    }

    for (i = 0; i < num; i++) {
        ret = sample_multi_pkg_crypto(&g_aes_data[i]);
        if (ret != TD_SUCCESS) {
            sample_err("************ test multi pkg %s failed ************\n", g_aes_data[i].name);
            return ret;
        }
        sample_log("************ test multi pkg %s success ************\n", g_aes_data[i].name);
    }
    num = array_size(g_aes_data_ex);
    for (i = 0; i < num; i++) {
        ret = sample_phy_crypto_ex(&g_aes_data_ex[i]);
        if (ret != TD_SUCCESS) {
            sample_err("************ test phy ex %s failed ************\n", g_aes_data_ex[i].data.name);
            return ret;
        }
        sample_log("************ test phy ex %s success ************\n", g_aes_data_ex[i].data.name);
    }

    return TD_SUCCESS;
}

static td_s32 data_init(td_void)
{
    td_u32 i;
    td_u32 num = 0;
    /* 1. init g_aes_data */
    num = array_size(g_aes_data);
    for (i = 0; i < num; i++) {
        sample_chk_expr_return(get_random_data(g_aes_data[i].key, sizeof(g_aes_data[i].key)), TD_SUCCESS);
        sample_chk_expr_return(get_random_data(g_aes_data[i].iv, sizeof(g_aes_data[i].iv)), TD_SUCCESS);
        sample_chk_expr_return(get_random_data(g_aes_data[i].src_data, sizeof(g_aes_data[i].src_data)), TD_SUCCESS);
    }

    /* 2. init g_aes_data_ex */
    num = array_size(g_aes_data_ex);
    for (i = 0; i < num; i++) {
        sample_chk_expr_return(get_random_data(g_aes_data_ex[i].data.key, sizeof(g_aes_data_ex[i].data.key)),
            TD_SUCCESS);
        sample_chk_expr_return(get_random_data(g_aes_data_ex[i].data.src_data, sizeof(g_aes_data_ex[i].data.src_data)),
            TD_SUCCESS);
        sample_chk_expr_return(get_random_data(g_aes_data_ex[i].aad, sizeof(g_aes_data_ex[i].aad)), TD_SUCCESS);
    }
    return TD_SUCCESS;
}

td_s32 sample_cipher_clearkey(td_bool sm4_support)
{
    td_s32 ret;
    sample_chk_expr_return(data_init(), TD_SUCCESS);
    sample_log("************ test cipher clearkey ************\n");
    ret = sample_aes();
    if (ret != TD_SUCCESS) {
        return ret;
    }
    sample_log("************ test cipher clearkey success ************\n");
    return TD_SUCCESS;
}
