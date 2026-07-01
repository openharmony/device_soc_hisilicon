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
#include <com.h>
#include "rkp.h"
#include "rkp_reg.h"
#include "rkp_com_reg.h"

static ALWAYS_INLINE uint32_t _rkp_read_reg(const uint32_t addr)
{
    uint32_t addr_phy = RKP_BASE + addr;
    return reg_get(addr_phy);
}

static ALWAYS_INLINE void _rkp_write_reg(const uint32_t addr, const uint32_t value)
{
    uint32_t addr_phy = RKP_BASE + addr;
    reg_set(addr_phy, value);
}

static ALWAYS_INLINE uint32_t _rkp_read_calc_start(void)
{
    rkp_calc_start reg_v;
    reg_v.u32 = _rkp_read_reg(RKP_CALC_CTRL);
    return reg_v.bits.rkp_calc_start;
}

static ALWAYS_INLINE int32_t _rkp_wait_idle(void)
{
    uint32_t time_out = 1000;

    while (--time_out) {
        if (_rkp_read_calc_start() == 0x0)
            break;

        udelay(10);
    }
    if (time_out == 0) {
        log_printf("rkp_wait_idle timeout!");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _rkp_sw_reg(const uint32_t sw_reg)
{
    _rkp_write_reg(RKP_SW_REG, sw_reg);
    neq_ret(_rkp_read_reg(RKP_SW_REG), sw_reg);
    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _rkp_calc_start(const rkp_kdf *kdf)
{
    rkp_calc_start rkp_reg;

    rkp_reg.u32 = _rkp_read_reg(RKP_CALC_CTRL);
    rkp_reg.bits.rkp_calc_start = 1;
    rkp_reg.bits.rkp_rk_sel = kdf->rootkey_slot;
    rkp_reg.bits.rkp_remap = kdf->remap;
    _rkp_write_reg(RKP_CALC_CTRL, rkp_reg.u32);

    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _rkp_secure_config()
{
    uint32_t reg = 0;

    reg = _rkp_read_reg(RKP_SEC_CFG);
    reg = reg & (~0x11);
    _rkp_write_reg(RKP_SEC_CFG, reg);

    return TD_SUCCESS;
}

static ALWAYS_INLINE int32_t _rkp_check_error(void)
{
    uint32_t reg = 0;
    reg = _rkp_read_reg(RKP_ERROR);
    if (reg != 0)
        return TD_FAILURE;

    return TD_SUCCESS;
}

int32_t rkp_eff_rk_start(const rkp_deob_kdf *deob_kdf, const uint32_t check_wd)
{
    int32_t ret;

    if (deob_kdf == NULL)
        return TD_FAILURE;

    _rkp_wait_idle();
    _rkp_sw_reg(deob_kdf->kdf.sw_reg);
    ret = _rkp_secure_config();
    if (ret != TD_SUCCESS)
        return TD_FAILURE;

    ret = _rkp_calc_start(&deob_kdf->kdf);
    if (ret != TD_SUCCESS)
        return TD_FAILURE;

    ret = _rkp_wait_idle();
    if (ret != TD_SUCCESS)
        return TD_FAILURE;

    ret = _rkp_check_error();
    if (ret != TD_SUCCESS)
        return TD_FAILURE;

    return TD_SUCCESS;
}

