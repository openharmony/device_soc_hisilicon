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

#ifndef __KERNEL__
#include "securec.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(__GNUC__) || defined(__clang__)
#define MM_EXT_STATIC static __attribute__((unused))
#else
#define MM_EXT_STATIC static
#endif

OSAL_PHYS_ADDR_TYPE cmpi_mmz_malloc(const char *mmz_name, const char *buf_name, unsigned int size,
    td_bool kernel_only);
OSAL_PHYS_ADDR_TYPE cmpi_mmz_malloc_fix_addr(const char *buf_name, OSAL_PHYS_ADDR_TYPE start, unsigned int size,
    td_bool kernel_only);
td_void cmpi_mmz_free(OSAL_PHYS_ADDR_TYPE phy_addr, td_void *vir_addr);
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
td_void *cmpi_remap_cached(OSAL_PHYS_ADDR_TYPE phy_addr, unsigned int size);
td_void *cmpi_remap_nocache(OSAL_PHYS_ADDR_TYPE phy_addr, unsigned int size);
td_void cmpi_unmap(td_void *virt_addr);
td_void *cmpi_mmz_phys_to_handle(td_phys_addr_t phys_addr);
td_s32 cmpi_check_mmz_phy_addr(OSAL_PHYS_ADDR_TYPE phy_addr, unsigned int len);
td_void cmpi_dcache_region_wb(td_void *virt_addr, OSAL_PHYS_ADDR_TYPE phys_addr, unsigned int len);
td_s32 cmpi_invalid_cache_byaddr(td_void *virt_addr, OSAL_PHYS_ADDR_TYPE phys_addr, unsigned int len);
int cmpi_get_fd(void);
void cmpi_set_fd(int fd);


int ot_mmb_check_mem_share(const ot_mmb_t *mmb);
int ot_mmb_check_mem_share_with_pid(const ot_mmb_t *mmb, int pid);
int ot_mmz_check_phys_addr(unsigned long phys_addr, unsigned long size);
int ot_mmz_get_mem_process_isolation(void);

MM_EXT_STATIC td_bool read_user_linear_space_valid(unsigned char *addr_start, unsigned int len)
{
    unsigned char check;
    unsigned char *addr_end = TD_NULL;
#ifndef __KERNEL__
    const size_t cp_len = 1;
#endif
    if (len == 0) {
        return TD_FALSE;
    }

    if (!osal_access_ok(OSAL_VERIFY_READ, addr_start, len)) {
        return TD_FALSE;
    }

    addr_end = addr_start + len - 1;
#ifdef __KERNEL__
    if (osal_copy_from_user(&check, addr_end, 1)) {
#else
    if (memcpy_s(&check, cp_len, addr_end, cp_len) != EOK) {
#endif
        return TD_FALSE;
    }

    return TD_TRUE;
}

MM_EXT_STATIC td_bool write_user_linear_space_valid(unsigned char *addr_start, unsigned int len)
{
    unsigned char check = 0;
    unsigned char *addr_end = TD_NULL;
#ifndef __KERNEL__
    const size_t cp_len = 1;
#endif
    if (len == 0) {
        return TD_FALSE;
    }

    if (!osal_access_ok(OSAL_VERIFY_WRITE, addr_start, len)) {
        return TD_FALSE;
    }

    addr_end = addr_start + len - 1;
#ifdef __KERNEL__
    if (osal_copy_to_user(addr_end, &check, 1)) {
#else
    if (memcpy_s(addr_end, cp_len, &check, cp_len) != EOK) {
#endif
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
