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

#include <types.h>
#include <common.h>
#include <platform.h>
#include <securecutil.h>
#include <otp.h>
#include <com.h>
#include <rkp.h>
#include "hkl.h"
#include "klad.h"
#include "ot_klad.h"

#define KLAD_SW_REG_NUM 4

#define OEM_ROOTKEY_SLOT    0
#define VENDOR_ROOTKEY_SLOT    4

static const uint32_t g_klad_sw_reg[KLAD_SW_REG_NUM] = {
    0x00000005,        // FlashProt_MasterKey_Boot
    0x00000006,        // ATF_Rootkey
    0x00000007,        // TEE_RootKey
};

static ALWAYS_INLINE int32_t _hpp_klad_com_klad_param(common_hkl *com_klad, const hpp_klad *klad)
{
    errno_t err;

    if (klad->alg == HPP_ALG_AES) {
        com_klad->cfg.tgt_cfg.dsc_mode = 0x20;
        com_klad->klad_alg = KLAD_ALG_AES;
    } else if (klad->alg == HPP_ALG_SM4) {
        com_klad->cfg.tgt_cfg.dsc_mode = 0x50;
        com_klad->klad_alg = KLAD_ALG_SM4;
    } else {
        return TD_FAILURE;
    }

    err = memcpy_s(com_klad->session_key[0], HKL_KEY_LEN, klad->session_key0, KLAD_KEY_LEN);
    if (err !=  EOK)
    {
        return TD_FAILURE;
    }
    err = memcpy_s(com_klad->session_key[1], HKL_KEY_LEN, klad->session_key1, KLAD_KEY_LEN);
    if (err !=  EOK)
    {
        return TD_FAILURE;
    }
    err = memcpy_s(com_klad->session_key[2], HKL_KEY_LEN, klad->session_key1 + KLAD_KEY_LEN,
               KLAD_KEY_LEN);
    if (err !=  EOK)
    {
        return TD_FAILURE;
    }
    com_klad->cfg.is_odd = klad->cfg.is_odd;
    com_klad->cfg.key_slot_num = klad->cfg.key_slot_num;
    com_klad->cfg.tgt_sec_cfg.dest_sec = TRUE;
    com_klad->cfg.tgt_sec_cfg.dest_nsec = TRUE;
    com_klad->cfg.tgt_sec_cfg.src_sec = TRUE;
    com_klad->cfg.tgt_sec_cfg.src_nsec = TRUE;
    com_klad->cfg.tgt_sec_cfg.key_sec = TRUE;

    return TD_SUCCESS;
}

int32_t hpp_klad_process(const hpp_keyladder_sel keyladder, const hpp_klad *klad)
{
    uint32_t tmp;
    rkp_deob_kdf deob_kdf;
    common_hkl com_klad;
    int32_t ret;

    if (klad == NULL)
    {
        return TD_FAILURE;

    }
    if (memset_s((uint8_t *)&com_klad, sizeof(common_hkl), 0, sizeof(common_hkl)) !=  EOK)
    {
        return TD_FAILURE;
    }
    if (memset_s((uint8_t *)&deob_kdf, sizeof(rkp_deob_kdf), 0, sizeof(rkp_deob_kdf)) != EOK) {
        return TD_FAILURE;
    }
    switch (keyladder) {
    case KLAD_BOOT:
        deob_kdf.kdf.rootkey_slot = OEM_ROOTKEY_SLOT;
        deob_kdf.kdf.remap = 0x1;
        break;
    case KLAD_ATF:
    case KLAD_TEE:
        if ((is_tee_enable() == AUTH_SUCCESS) &&
            (get_tee_owner() == TEE_OWNER_IS_VENDOR))
            deob_kdf.kdf.rootkey_slot = VENDOR_ROOTKEY_SLOT;
        else
            deob_kdf.kdf.rootkey_slot = 0;
        /* remap is invalid, no setting */
        break;
    default:
        break;
    }

    deob_kdf.kdf.sw_reg = g_klad_sw_reg[keyladder];

    ret = _hpp_klad_com_klad_param(&com_klad, klad);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    tmp = gen_wd_2(&deob_kdf, &com_klad);
    ret = klad_com_process(&deob_kdf, &com_klad, tmp);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
