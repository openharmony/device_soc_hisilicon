/*
 * Copyright (c) 2026 HiSilicon (Shanghai) Technologies Co., Ltd.
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

#include <unistd.h>
#include <stdio.h>
#include "securec.h"

#include "huks_hdi_cipher.h"
#include "huks_hdi_cipher_adapter.h"

#include "ss_mpi_cipher.h"
#include "ss_mpi_klad.h"
#include "ss_mpi_otp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define IV_LEN 16
#define CONTENT_KEY_LEN 32
#define SESSION_KEY_LEN 16
#define SALT_LEN 28
#define TEE_ENABLE_OFFSET 0x12

#define USE_ROOT_KEY

#ifndef USE_ROOT_KEY
td_s32 cipher_set_clear_key(td_handle keyslot_handle, td_u8 *key)
{
    HDF_LOGI("[huks_hdi] %s:called.\n", __func__);
    td_handle klad_handle = 0;
    td_s32 ret = TD_FAILURE;
    ot_klad_attr klad_attr = {
        .klad_cfg = {
            .klad_type = OT_KLAD_TYPE_CLEARCW,
        },
    };
    ot_klad_clear_key klad_clear_key = {
        .crypto_alg = OT_KLAD_CRYPTO_ALG_AES,
        .key_size = CONTENT_KEY_LEN
    };

    ret = memcpy_s(klad_clear_key.key, OT_KLAD_MAX_KEY_LEN, key, CONTENT_KEY_LEN);
    if (ret != EOK) {
        HDF_LOGE("[huks_hdi] %s:memcpy_s failed\n", __func__);
        goto KEY_CLEAN;
    }

    ret = ss_mpi_klad_create(&klad_handle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_klad_create failed\n", __func__);
        goto KEY_CLEAN;
    }

    ret = ss_mpi_klad_attach(klad_handle, keyslot_handle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_klad_attach failed\n", __func__);
        goto KLAD_DESTROY;
    }

    ret = ss_mpi_klad_set_attr(klad_handle, &klad_attr);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_klad_set_attr failed\n", __func__);
        goto KLAD_DETACH;
    }

    ret = ss_mpi_klad_set_clear_key(klad_handle, &klad_clear_key);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_klad_set_clear_key failed\n", __func__);
        goto KLAD_DETACH;
    }

KLAD_DETACH:
    (void)ss_mpi_klad_detach(klad_handle, keyslot_handle);
KLAD_DESTROY:
    (void)ss_mpi_klad_destroy(klad_handle);
KEY_CLEAN:
    (void)memset_s(klad_clear_key.key, OT_KLAD_MAX_KEY_LEN, 0, OT_KLAD_MAX_KEY_LEN);
    HDF_LOGI("[huks_hdi] %s:end.\n", __func__);
    return ret;
}
#else /* USE_ROOT_KEY */
typedef struct {
    td_handle kladHandle;
    ot_klad_attr kladAttr;
    ot_klad_session_key sessionKey;
    ot_klad_content_key contentKey;
} CipherRootKeyCtx;

static td_s32 CipherRootKeyInitCtx(CipherRootKeyCtx *ctx, const td_u8 *key)
{
    td_s32 ret;
    ctx->kladAttr.klad_cfg.klad_type = OT_KLAD_TYPE_COMMON;
    ctx->kladAttr.klad_cfg.rootkey_attr.key_secure = OT_KLAD_ROOTKEY_SEC_REE;
    ctx->kladAttr.klad_cfg.rootkey_attr.key_sel = OT_KLAD_ROOTKEY_SEL_OEM0;
    ctx->sessionKey.level = OT_KLAD_LEVEL1;
    ctx->sessionKey.alg = OT_KLAD_ALG_TYPE_AES;
    ctx->sessionKey.key_size = SESSION_KEY_LEN;
    ctx->contentKey.alg = OT_KLAD_ALG_TYPE_AES;
    ctx->contentKey.crypto_alg = OT_KLAD_CRYPTO_ALG_AES;
    ctx->contentKey.key_size = CONTENT_KEY_LEN;

    ret = memcpy_s(ctx->sessionKey.key, OT_KLAD_MAX_KEY_LEN,
        key + CONTENT_KEY_LEN, SESSION_KEY_LEN);
    if (ret != EOK) {
        HDF_LOGE("[huks_hdi] %s:memcpy_s failed\n", __func__);
        return ret;
    }
    ret = memcpy_s(ctx->contentKey.key, OT_KLAD_MAX_KEY_LEN,
        key, CONTENT_KEY_LEN);
    if (ret != EOK) {
        HDF_LOGE("[huks_hdi] %s:memcpy_s failed\n", __func__);
        return ret;
    }
    return TD_SUCCESS;
}

static td_s32 CipherRootKeyKladOps(CipherRootKeyCtx *ctx, td_handle keyslot_handle)
{
    td_s32 ret = ss_mpi_klad_create(&ctx->kladHandle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_klad_create failed\n", __func__);
        return ret;
    }
    ret = ss_mpi_klad_attach(ctx->kladHandle, keyslot_handle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_klad_attach failed\n", __func__);
        goto KLAD_DESTROY;
    }
    ret = ss_mpi_klad_set_attr(ctx->kladHandle, &ctx->kladAttr);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_klad_set_attr failed\n", __func__);
        goto KLAD_DETACH;
    }
    ret = ss_mpi_klad_set_session_key(ctx->kladHandle, &ctx->sessionKey);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_klad_set_session_key failed\n", __func__);
        goto KLAD_DETACH;
    }
    ret = ss_mpi_klad_set_content_key(ctx->kladHandle, &ctx->contentKey);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_klad_set_content_key failed\n", __func__);
    }
KLAD_DETACH:
    (void)ss_mpi_klad_detach(ctx->kladHandle, keyslot_handle);
KLAD_DESTROY:
    (void)ss_mpi_klad_destroy(ctx->kladHandle);
    return ret;
}

td_s32 cipher_set_root_key(td_handle keyslot_handle, td_u8 *key)
{
    HDF_LOGI("[huks_hdi] %s:called.\n", __func__);
    CipherRootKeyCtx ctx = {0};
    td_s32 ret;

    ret = CipherRootKeyInitCtx(&ctx, key);
    if (ret != TD_SUCCESS) {
        goto KEY_CLEAN;
    }
    ret = CipherRootKeyKladOps(&ctx, keyslot_handle);
KEY_CLEAN:
    (void)memset_s(ctx.sessionKey.key, OT_KLAD_MAX_KEY_LEN, 0, OT_KLAD_MAX_KEY_LEN);
    (void)memset_s(ctx.contentKey.key, OT_KLAD_MAX_KEY_LEN, 0, OT_KLAD_MAX_KEY_LEN);
    HDF_LOGI("[huks_hdi] %s:end.\n", __func__);
    return ret;
}
#endif /* end USE_ROOT_KEY */

typedef struct {
    td_handle symcHandle;
    td_handle keyslotHandle;
    ot_cipher_attr symcAttr;
    ot_cipher_ctrl symcCtrl;
    ot_keyslot_attr keyslotAttr;
} HksCipherCtx;

static int32_t HksCipherCtxInit(HksCipherCtx *ctx, const struct HksBlob *keyMaterial)
{
    (void)memset_s(ctx, sizeof(HksCipherCtx), 0, sizeof(HksCipherCtx));
    ctx->keyslotAttr.type = OT_KEYSLOT_TYPE_MCIPHER;
    ctx->keyslotAttr.secure_mode = OT_KEYSLOT_SECURE_MODE_NONE;
    ctx->symcAttr.cipher_type = OT_CIPHER_TYPE_NORMAL;
    ctx->symcCtrl.alg = OT_CIPHER_ALG_AES;
    ctx->symcCtrl.work_mode = OT_CIPHER_WORK_MODE_CBC;
    ctx->symcCtrl.aes_ctrl.bit_width = OT_CIPHER_BIT_WIDTH_128BIT;
    ctx->symcCtrl.aes_ctrl.key_len = OT_CIPHER_KEY_AES_256BIT;
    ctx->symcCtrl.aes_ctrl.chg_flags = OT_CIPHER_IV_CHG_ONE_PACK;
    if (memcpy_s(ctx->symcCtrl.aes_ctrl.iv, sizeof(ctx->symcCtrl.aes_ctrl.iv),
                 keyMaterial->data + CONTENT_KEY_LEN + SESSION_KEY_LEN, IV_LEN) != EOK) {
        HDF_LOGE("[huks_hdi] %s:memcpy  failed! \n", __func__);
        return HKS_ERROR_BAD_STATE;
    }
    return TD_SUCCESS;
}

static int32_t HksCipherOpen(HksCipherCtx *ctx)
{
    int32_t ret = ss_mpi_cipher_init();
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s: ss_mpi_cipher_init failed! \n", __func__);
        return ret;
    }

    ret = ss_mpi_cipher_create(&ctx->symcHandle, &ctx->symcAttr);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s: ss_mpi_cipher_create failed! \n", __func__);
        goto CIPHER_DEINIT;
    }

    ret = ss_mpi_klad_init();
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_km_init failed\n", __func__);
        goto CIPHER_DESTROY;
    }

    ret = ss_mpi_keyslot_create(&ctx->keyslotAttr, &ctx->keyslotHandle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_keyslot_create failed\n", __func__);
        goto KM_DEINIT;
    }

    ret = ss_mpi_cipher_attach(ctx->symcHandle, ctx->keyslotHandle);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_cipher_symc_attach failed\n", __func__);
        goto KEYSLOT_DESTROY;
    }
    return TD_SUCCESS;

KEYSLOT_DESTROY:
    (void)ss_mpi_keyslot_destroy(ctx->keyslotHandle);
KM_DEINIT:
    (void)ss_mpi_klad_deinit();
CIPHER_DESTROY:
    ss_mpi_cipher_destroy(ctx->symcHandle);
CIPHER_DEINIT:
    ss_mpi_cipher_deinit();
    return ret;
}

static int32_t HksCipherSetKeyAndCfg(HksCipherCtx *ctx, const struct HksBlob *keyMaterial)
{
    int32_t ret;

#ifdef USE_ROOT_KEY
    ret = cipher_set_root_key(ctx->keyslotHandle, keyMaterial->data);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:cipher_set_root_key failed! \n", __func__);
        return ret;
    }
#else
    ret = cipher_set_clear_key(ctx->keyslotHandle, keyMaterial->data);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:cipher_set_clear_key failed! \n", __func__);
        return ret;
    }
#endif

    ret = ss_mpi_cipher_set_cfg(ctx->symcHandle, &ctx->symcCtrl);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_cipher_symc_set_config failed! \n", __func__);
    }
    return ret;
}

static int32_t HksCipherProcess(HksCipherCtx *ctx, const struct HksBlob *mainKey,
    bool isEncrypt, struct HksBlob *derivedKey)
{
    int32_t ret;

    if (isEncrypt) {
        ret = ss_mpi_cipher_encrypt_virt(ctx->symcHandle, mainKey->data, derivedKey->data, derivedKey->size);
    } else {
        ret = ss_mpi_cipher_decrypt_virt(ctx->symcHandle, mainKey->data, derivedKey->data, derivedKey->size);
    }
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_cipher_symc_encrypt or decrypt failed! \n", __func__);
    }
    return ret;
}

static void HksCipherClose(HksCipherCtx *ctx)
{
    ss_mpi_cipher_detach(ctx->symcHandle, ctx->keyslotHandle);
    (void)ss_mpi_keyslot_destroy(ctx->keyslotHandle);
    (void)ss_mpi_klad_deinit();
    ss_mpi_cipher_destroy(ctx->symcHandle);
    ss_mpi_cipher_deinit();
    (void)memset_s(ctx->symcCtrl.aes_ctrl.iv, sizeof(ctx->symcCtrl.aes_ctrl.iv), 0,
        sizeof(ctx->symcCtrl.aes_ctrl.iv));
}

int32_t HksCipherEncryptAndDecrypt(const struct HksBlob *mainKey,
    const struct HksBlob *keyMaterial, bool isEncrypt, struct HksBlob *derivedKey)
{
    HDF_LOGI("[huks_hdi] %s:called.\n", __func__);
    int32_t ret = HKS_ERROR_BAD_STATE;
    HksCipherCtx ctx = {0};

    ret = HksCipherCtxInit(&ctx, keyMaterial);
    if (ret != TD_SUCCESS) {
        goto CIPHER_EXIT;
    }
    ret = HksCipherOpen(&ctx);
    if (ret != TD_SUCCESS) {
        goto CIPHER_EXIT;
    }
    ret = HksCipherSetKeyAndCfg(&ctx, keyMaterial);
    if (ret == TD_SUCCESS) {
        ret = HksCipherProcess(&ctx, mainKey, isEncrypt, derivedKey);
    }
    HksCipherClose(&ctx);
CIPHER_EXIT:
    (void)memset_s(ctx.symcCtrl.aes_ctrl.iv, sizeof(ctx.symcCtrl.aes_ctrl.iv), 0, sizeof(ctx.symcCtrl.aes_ctrl.iv));
    if (ret == TD_SUCCESS) {
        HDF_LOGI("[huks_hdi] %s:end.\n", __func__);
    }
    HDF_LOGI("[huks_hdi] %s:CIPHER_EXIT.\n", __func__);
    return ret == TD_SUCCESS ? HKS_SUCCESS : HKS_ERROR_BAD_STATE;
}

#define CIPHER_RANDOM_LEN 4
int32_t HksCipherGenerateRandom(struct HksBlob *randomData)
{
    HDF_LOGI("[huks_hdi] %s:called.\n", __func__);
    td_s32 ret = TD_SUCCESS;
    if (randomData == NULL) {
        return HKS_ERROR_BAD_STATE;
    }
    ret = cipher_get_random(randomData->data, randomData->size);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:cipher_get_random failed! ret = 0x%X", __func__, ret);
        return HKS_ERROR_BAD_STATE;
    }
    HDF_LOGI("[huks_hdi] %s:end.\n", __func__);
    return HKS_SUCCESS;
}

int cipher_get_random(uint8_t *data, uint32_t size)
{
    HDF_LOGI("[huks_hdi] %s:called.\n", __func__);
    uint32_t count = size / CIPHER_RANDOM_LEN;
    uint32_t tail_len = size % CIPHER_RANDOM_LEN;
    int outLen = 0;
    uint32_t random_num = 0;
    uint32_t cur_len = 0;
    td_s32 ret = TD_SUCCESS;

    ret = ss_mpi_cipher_init();
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:Cipher init failed! cipher ret = 0x%X \n", __func__, ret);
        return ret;
    }

    for (uint32_t i = 0; i <= count; ++i) {
        ret = ss_mpi_cipher_get_random_num(&random_num);
        if (ret != TD_SUCCESS) {
            HDF_LOGE("[huks_hdi] %s:Cipher random failed! cipher ret = 0x%X \n", __func__, ret);
            goto errExit;
        }

        if (i == count && tail_len == 0) {
            break;
        } else if (i != count) {
        cur_len = CIPHER_RANDOM_LEN;
    } else if (tail_len != 0) {
        cur_len = tail_len;
    }

    ret = memcpy_s(data + outLen, size - outLen, &random_num, cur_len);
        if (ret != EOK) {
            HDF_LOGE("[huks_hdi] %s:Memcpy random failed! \n", __func__);
            ret = TD_FAILURE;
            goto errExit;
        }

        outLen += cur_len;
    }

    HDF_LOGI("[huks_hdi] %s:end.\n", __func__);
    return ret;

errExit:
    (void)memset_s(data, size, 0, size);
    (void)ss_mpi_cipher_deinit();
    HDF_LOGI("[huks_hdi] %s:errExit.\n", __func__);
    return ret;
}

#define HKS_HARDWARE_UDID_OFFSET 0x0
int32_t GetDevDieid(uint8_t *dieid, uint32_t dieidLen)
{
    HDF_LOGI("[huks_hdi] %s:called.\n", __func__);
    td_s32 ret = TD_FAILURE;

    (void)memset_s(dieid, dieidLen, 0, dieidLen);
    ret = ss_mpi_otp_init();
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_otp_init failed!\n", __func__);
        return TD_FAILURE;
    }

    ret = ss_mpi_otp_get_user_data("ree_user_data", HKS_HARDWARE_UDID_OFFSET, dieid, dieidLen);
    if (ret != TD_SUCCESS) {
        HDF_LOGE("[huks_hdi] %s:ss_mpi_otp_get_user_data failed!\n", __func__);
        (void)memset_s(dieid, dieidLen, 0, dieidLen);
    }

    (void)ss_mpi_otp_deinit();
    HDF_LOGI("[huks_hdi] %s:end.\n", __func__);
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */