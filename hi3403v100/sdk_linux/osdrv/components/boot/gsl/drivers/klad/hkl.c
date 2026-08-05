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
#include "hkl_reg.h"
#include "com.h"
#include "hkl.h"
#include "hkl_com.h"

static ALWAYS_INLINE uint32_t  _hkl_read_reg(const uint32_t addr)
{
    uint32_t addr_phy = HKL_BASE + addr;
    return reg_get(addr_phy);
}

static ALWAYS_INLINE void _hkl_write_reg(const uint32_t addr, const uint32_t value)
{
    uint32_t addr_phy = HKL_BASE + addr;
    reg_set(addr_phy, value);
    return;
}

static ALWAYS_INLINE int32_t _hkl_lock_ctrl_proces()
{
    hkl_lock_ctrl lock_ctl;

    lock_ctl.u32 = _hkl_read_reg(HKL_LOCK_CTRL);
    lock_ctl.bits.kl_lock = 1;
    _hkl_write_reg(HKL_LOCK_CTRL, lock_ctl.u32);

    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _hkl_unlock_ctrl_proces()
{
    hkl_unlock_ctrl unlock_ctl;

    unlock_ctl.u32 = _hkl_read_reg(HKL_UNLOCK_CTRL);
    unlock_ctl.bits.kl_unlock = 1;

    _hkl_write_reg(HKL_UNLOCK_CTRL, unlock_ctl.u32);

    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _hkl_com_lock_wait_idle(hkl_com_lock_info *lock_info)
{
    uint32_t time_out = 1000;

    while (--time_out) {
        lock_info->u32 = _hkl_read_reg(HKL_COM_LOCK_INFO);
        if (lock_info->bits.kl_com_lock_busy == 0b10) {
            break;
        }
        udelay(10);
        random_delay();
    }
    if (time_out == 0) {
        log_printf("hkl lock timeout!");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _hkl_get_com_lock_num()
{
    hkl_com_lock_info lock_info;
    int32_t ret;

    lock_info.u32 = 0;
    ret = _hkl_com_lock_wait_idle(&lock_info);
    if (ret != TD_SUCCESS)
    {
        return TD_FAILURE;
    }
    if (lock_info.bits.kl_com_lock_fail == 0b10)
    {
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

static ALWAYS_INLINE int32_t _hkl_check_com_unlock_info(void)
{
    hkl_com_lock_info lock_info;

    lock_info.u32 = 0;
    chk_ret(_hkl_com_lock_wait_idle(&lock_info));
    if (lock_info.bits.kl_com_unlock_fail == 0b10) {
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

static ALWAYS_INLINE int32_t _hkl_check_com_lock_stat()
{
    hkl_com_lock_status lock_stat;

    lock_stat.u32 = _hkl_read_reg(HKL_COM_LOCK_STAT);
    if (lock_stat.bits.kl_com_lock_stat != HKL_LOCK_STATE_TEE) {
        log_printf("unlocked or locked by other CPU.\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static ALWAYS_INLINE uint32_t _hkl_get_busy(void)
{
    hkl_state reg_v;
    reg_v.u32 = _hkl_read_reg(HKL_STATE);
    return reg_v.bits.kl_busy;
}

static ALWAYS_INLINE int32_t _hkl_wait_idle(void)
{
    uint32_t time_out = 1000;

    while (--time_out) {
        if (_hkl_get_busy() == 0b10) {
            break;
        }
        udelay(10);
        random_delay();
    }
    if (time_out == 0) {
        log_printf("hkl_wait_idle timeout!");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _hkl_set_addr(const uint16_t addr, const uint32_t is_odd)
{
    hkl_key_addr key_addr;

    key_addr.u32 = _hkl_read_reg(HKL_KEY_ADDR);
    key_addr.bits.key_addr = (addr << 1) | is_odd;
    _hkl_write_reg(HKL_KEY_ADDR, key_addr.u32);

    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _hkl_set_cfg(const hkl_target_cfg *cfg)
{
    hkl_key_cfg reg;

    reg.u32 = _hkl_read_reg(HKL_KEY_CFG);
    reg.bits.dsc_mode = cfg->dsc_mode;
    _hkl_write_reg(HKL_KEY_CFG, reg.u32);

    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _hkl_set_sec_cfg(const hkl_target_sec_cfg *sec_cfg)
{
    hkl_key_sec_cfg reg;

    reg.u32 = _hkl_read_reg(HKL_KEY_SEC_CFG);
    reg.bits.dest_sec = sec_cfg->dest_sec;
    reg.bits.dest_nsec = sec_cfg->dest_nsec;
    reg.bits.src_sec = sec_cfg->src_sec;
    reg.bits.src_nsec = sec_cfg->src_nsec;
    reg.bits.key_sec = sec_cfg->key_sec;
    _hkl_write_reg(HKL_KEY_SEC_CFG, reg.u32);

    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _hkl_set_data_in(const uint8_t *data, const uint32_t len)
{
    uint32_t i = 0;
    uint32_t din = 0;

    if (data == NULL)
    {
        return TD_FAILURE;
    }
    if (len != HKL_KEY_LEN)
    {
        return TD_FAILURE;
    }
    for (i = 0; i < HKL_KEY_LEN; i += 4) {
        din = ((uint32_t)data[i + 3] << 24) | ((uint32_t)data[i + 2] << 16) |
            ((uint32_t)data[i + 1] << 8) | data[i];
        _hkl_write_reg(HKL_DATA_IN_0 + i, din);
    }

    if (i != HKL_KEY_LEN)
    {
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _hkl_set_com_ctrl(uint8_t level, const uint8_t alg,
        const uint32_t to_ta_kl, const uint32_t key_size)
{
    hkl_com_ctrl reg;

    reg.u32 = _hkl_read_reg(HKL_COM_CTRL);
    reg.bits.kl_com_start = 1;
    if (level == 2) {
        level = 1;
    }
    reg.bits.kl_com_level_sel = level;
    reg.bits.kl_com_alg_sel = alg;
    reg.bits.kl_com_key_size = key_size;
    _hkl_write_reg(HKL_COM_CTRL, reg.u32);

    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _hkl_check_err(void)
{
    hkl_error hkl_err;
    hkl_kc_error kc_err;

    hkl_err.u32 = _hkl_read_reg(HKL_ERROR);
    kc_err.u32 = _hkl_read_reg(HKL_KC_ERROR);
    if ((kc_err.u32 == 0) && (hkl_err.u32 == 0)) {
        return TD_SUCCESS;
    }
    return TD_FAILURE;
}

int32_t hkl_content_start(const common_hkl *com_klad, const uint32_t level,
    const uint32_t check_wd)
{
    if (level >= HKL_LEVEL || com_klad == NULL) {
        return TD_FAILURE;
    }
    chk_wd_2(com_klad, level, check_wd);
    chk_ret(_hkl_wait_idle());
    chk_ret(_hkl_set_addr(com_klad->cfg.key_slot_num, com_klad->cfg.is_odd));
    chk_ret(_hkl_set_cfg(&com_klad->cfg.tgt_cfg));
    chk_ret(_hkl_set_sec_cfg(&com_klad->cfg.tgt_sec_cfg));
    chk_ret(_hkl_set_data_in(com_klad->session_key[level], HKL_KEY_LEN));
    chk_ret(_hkl_set_com_ctrl(level, com_klad->klad_alg, FALSE, com_klad->key_size));
    chk_ret(_hkl_wait_idle());
    chk_ret(_hkl_check_err());

    return TD_SUCCESS;
}

int32_t hkl_com_lock()
{
    chk_ret(_hkl_lock_ctrl_proces());
    chk_ret(_hkl_get_com_lock_num());
    chk_ret(_hkl_check_com_lock_stat());
    return TD_SUCCESS;
}

int32_t hkl_com_unlock()
{
    chk_ret(_hkl_unlock_ctrl_proces());
    chk_ret(_hkl_check_com_unlock_info());
    return TD_SUCCESS;
}

