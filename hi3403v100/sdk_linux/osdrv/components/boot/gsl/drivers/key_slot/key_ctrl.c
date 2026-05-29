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
#include <log.h>
#include "key_ctrl_reg.h"
#include "key_ctrl_com.h"
#include "key_ctrl.h"
#include "../klad/include/com.h"

static ALWAYS_INLINE uint32_t  _kc_read_reg(const uint32_t addr)
{
    uint32_t addr_phy = KC_BASE + addr;
    return reg_get(addr_phy);
}

static ALWAYS_INLINE void _kc_write_reg(const uint32_t addr, const uint32_t value)
{
    uint32_t addr_phy = KC_BASE + addr;
    reg_set(addr_phy, value);
    return;
}

static ALWAYS_INLINE  uint32_t _kc_get_flush_status(void)
{
    return _kc_read_reg(KC_TEE_FLUSH_BUSY);
}

static ALWAYS_INLINE  int32_t _kc_flush_wait(void)
{
    uint32_t time_out = 1000;

    while (--time_out) {
        if ((_kc_get_flush_status() & 0x1) == 0x0) {
            break;
        }
        udelay(10);
        random_delay();
    }
    if (time_out == 0) {
        log_printf("kc_flush_wait timeout.\n");
        return TD_FAILURE;
    }
    if (_kc_get_flush_status() & 0x02) {
        log_printf("flush failed.\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static ALWAYS_INLINE kc_slot_stat_e _kc_slot_status(const kc_slot_ind_e slot_ind,
        const uint32_t slot_num)
{
    kc_rd_lock_status stat;
    kc_rd_slot_num slot;
    kc_rd_slot_num tmp;

    slot.u32 = _kc_read_reg(KC_RD_SLOT_NUM);
    slot.bits.slot_num_cfg = slot_num;
    _kc_write_reg(KC_RD_SLOT_NUM, slot.u32);

    tmp.u32 = _kc_read_reg(KC_RD_SLOT_NUM);
    neq_ret(tmp.bits.slot_num_cfg, slot_num);

    stat.u32 = _kc_read_reg(KC_RD_LOCK_STATUS);
    return (kc_slot_stat_e)stat.bits.rd_lock_status;
}

static ALWAYS_INLINE int32_t _kc_slot_lock(const kc_slot_ind_e slot_ind, const uint32_t slot_num)
{
    kc_tee_lock_cmd tee_reg;
    kc_tee_lock_cmd tmp;

    tee_reg.u32 = _kc_read_reg(KC_TEE_LOCK_CMD);
    tee_reg.bits.tee_lock_cmd = 1;
    tee_reg.bits.tee_key_slot_num = slot_num;
    _kc_write_reg(KC_TEE_LOCK_CMD, tee_reg.u32);

    tmp.u32 = _kc_read_reg(KC_TEE_LOCK_CMD);
    neq_ret(tmp.bits.tee_key_slot_num, slot_num);
    neq_ret(tmp.bits.tee_lock_cmd, 1);
    if (_kc_slot_status(slot_ind, slot_num) != KC_SLOT_STAT_TEE_LOCK) {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _kc_slot_unlock(const kc_slot_ind_e slot_ind,
        const uint32_t slot_num)
{
    kc_tee_lock_cmd tee_reg;
    kc_tee_lock_cmd tmp;

    tee_reg.u32 = _kc_read_reg(KC_TEE_LOCK_CMD);
    tee_reg.bits.tee_lock_cmd = 0;
    tee_reg.bits.tee_key_slot_num = slot_num;
    _kc_write_reg(KC_TEE_LOCK_CMD, tee_reg.u32);

    tmp.u32 = _kc_read_reg(KC_TEE_LOCK_CMD);
    neq_ret(tmp.bits.tee_key_slot_num, slot_num);
    neq_ret(tmp.bits.tee_lock_cmd, 0);
    chk_ret(_kc_flush_wait());
    return TD_SUCCESS;
}

int32_t kc_slot_unlock(const kc_slot_ind_e slot_ind, const uint32_t slot_num)
{
    chk_ret(_kc_slot_unlock(slot_ind, slot_num));
    return TD_SUCCESS;
}

int32_t kc_slot_auto_lock(const kc_slot_ind_e slot_ind, uint32_t *slot_num)
{
    uint32_t slot;
    uint32_t i;

    if (slot_num == NULL)
        return TD_FAILURE;
    if (slot_ind == KC_SLOT_TS)
        slot = KC_TS_SLOT_NUM;
    else if (slot_ind == KC_SLOT_MC)
        slot = KC_MC_SLOT_NUM;
    else
        return TD_FAILURE;

    for (i = 0; i < slot; i++) {
        if (_kc_slot_status(slot_ind, i) != KC_SLOT_STAT_UN_LOCK)
            continue;
        if (_kc_slot_lock(slot_ind, i) != TD_SUCCESS)
            continue;
        *slot_num = i;
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

int32_t kc_slot_is_lock(const kc_slot_ind_e slot_ind, const uint32_t slot_num)
{
    return (_kc_slot_status(slot_ind, slot_num) != KC_SLOT_STAT_UN_LOCK);
}

