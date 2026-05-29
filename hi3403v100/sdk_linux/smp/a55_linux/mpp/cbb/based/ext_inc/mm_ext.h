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

#ifndef MM_EXT_H
#define MM_EXT_H

#include "ot_osal.h"
#include "ot_math.h"
#include "ot_common.h"
#include "osal_mmz.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#if defined(__GNUC__) || defined(__clang__)
#define CBB_MM_EXT_STATIC static __attribute__((unused))
#else
#define CBB_MM_EXT_STATIC static
#endif
td_phys_addr_t cmpi_mmz_malloc(const td_char *mmz_name, const td_char *buf_name, td_ulong size,
    td_bool kernel_only);
td_phys_addr_t cmpi_mmz_malloc_fix_addr(const td_char *buf_name, td_phys_addr_t start, td_ulong size,
    td_bool kernel_only);
td_void cmpi_mmz_free(td_phys_addr_t phys_addr, td_void *virt_addr);
typedef struct {
    const td_char *mmz_name;
    const td_char *buf_name;
    td_phys_addr_t *phys_addr;
    td_void **virt_addr;
    td_ulong len;
    td_bool kernel_only;
} cmpi_mmz_malloc_param;
td_s32 cmpi_mmz_malloc_nocache(const cmpi_mmz_malloc_param *param);
td_s32 cmpi_mmz_malloc_cached(const cmpi_mmz_malloc_param *param);
td_void *cmpi_remap_cached(td_phys_addr_t phy_addr, td_ulong ul_size);
td_void *cmpi_remap_nocache(td_phys_addr_t phy_addr, td_ulong ul_size);
td_void cmpi_unmap(td_void *virt_addr);

CBB_MM_EXT_STATIC td_bool read_user_linear_space_valid(td_u8 *addr_start, td_u32 len)
{
    td_u8 check;
    td_u8 *addr_end = TD_NULL;

    if (len == 0) {
        return TD_FALSE;
    }

    if (!osal_access_ok(OSAL_VERIFY_READ, addr_start, len)) {
        return TD_FALSE;
    }

    addr_end = addr_start + len - 1;
    if (osal_copy_from_user(&check, addr_end, 1)) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

CBB_MM_EXT_STATIC td_bool write_user_linear_space_valid(td_u8 *addr_start, td_u32 len)
{
    td_u8 check = 0;
    td_u8 *addr_end = TD_NULL;

    if (len == 0) {
        return TD_FALSE;
    }

    if (!osal_access_ok(OSAL_VERIFY_WRITE, addr_start, len)) {
        return TD_FALSE;
    }

    addr_end = addr_start + len - 1;
    if (osal_copy_to_user(addr_end, &check, 1)) {
        return TD_FALSE;
    }

    return TD_TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
