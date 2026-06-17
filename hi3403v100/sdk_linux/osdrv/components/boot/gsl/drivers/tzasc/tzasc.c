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
#include <platform.h>
#include "tzasc.h"

#define TZASC_BYPASS_DIS    0x00
#define TZASC_RNG_EN        1

typedef union {
    struct {
        unsigned rgn_base_addr : 22;    // [21:0]
        unsigned reserved : 9;            // [30:22]
        unsigned rgn_en : 1;            // [31]
    } bits;
    uint32_t u32;
} sec_rgn_map;

typedef union {
    struct {
        unsigned rgn_size : 24;            // [23:0]
        unsigned reserved : 8;            // [31:24]
    } bits;
    uint32_t u32;
} sec_rgn_map_ext;

typedef union {
    struct {
        unsigned sp : 4;                // [3:0]
        unsigned security_inv : 1;        // [4]
        unsigned reserved_1 : 3;        // [7:5]
        unsigned mid_en : 1;            // [8]
        unsigned mid_inv : 1;            // [9]
        unsigned reserved_0 : 6;        // [15:10]
        unsigned tee_sp_nosec_w : 4;    // [19:16]
        unsigned tee_sp_nosec_r : 4;    // [23:20]
        unsigned tee_sp_sec_w : 4;        // [27:24]
        unsigned tee_sp_sec_r : 4;        // [31:28]
    } bits;
    uint32_t u32;
} sec_rgn_attr;

static volatile sec_rgn_map *get_sec_rgn_map_reg(uint8_t rgn)
{
    uintptr_t addr;
    if (rgn < TZASC_MAX_RGN_NUMS) {
        addr = SEC_MODULE_BASE + tzasc_rgn_offset(rgn) + TZASC_RGN_MAP_REG_OFFSET;
        return (sec_rgn_map *)addr;
    }
    return NULL;
}

static volatile sec_rgn_map_ext *get_sec_rgn_map_ext_reg(uint8_t rgn)
{
    uintptr_t addr;
    if (rgn < TZASC_MAX_RGN_NUMS) {
        addr = SEC_MODULE_BASE + tzasc_rgn_offset(rgn) + TZASC_RGN_MAP_EXT_REG_OFFSET;
        return (sec_rgn_map_ext *)addr;
    }
    return NULL;
}

static volatile sec_rgn_attr *get_sec_rgn_attr_reg(uint8_t rgn)
{
    uintptr_t addr;
    if (rgn < TZASC_MAX_RGN_NUMS) {
        addr = SEC_MODULE_BASE + tzasc_rgn_offset(rgn) + TZASC_RGN_ATTR_REG;
        return (sec_rgn_attr *)addr;
    }
    return NULL;
}

void tzasc_bypass_disable()
{
    volatile uintptr_t addr = SEC_MODULE_BASE + TZASC_BYPASS_REG_OFFSET;
    *(uint32_t *)addr = TZASC_BYPASS_DIS;
}

void tzasc_rgn_enable(uint8_t rgn)
{
	volatile sec_rgn_map *map_reg = get_sec_rgn_map_reg(rgn);
    if (map_reg == NULL) {
		return;
    }

    map_reg->bits.rgn_en = TZASC_RNG_EN;
}

void tzasc_set_rgn_map(uint8_t rgn, uint64_t addr, size_t size)
{
	volatile sec_rgn_map *map_reg = get_sec_rgn_map_reg(rgn);
	volatile sec_rgn_map_ext *map_ext_reg = get_sec_rgn_map_ext_reg(rgn);
    if ((map_reg == NULL) || (map_ext_reg == NULL)) {
		return;
    }

    map_reg->bits.rgn_base_addr = addr >> TZASC_ALGIN_BITS;
    map_ext_reg->bits.rgn_size = size >> TZASC_ALGIN_BITS;
}

void tzasc_set_rgn_attr(uint8_t rgn, uint32_t attr)
{
    volatile sec_rgn_attr *attr_reg = get_sec_rgn_attr_reg(rgn);
    if (attr_reg != NULL)
        attr_reg->u32 = attr;
}
