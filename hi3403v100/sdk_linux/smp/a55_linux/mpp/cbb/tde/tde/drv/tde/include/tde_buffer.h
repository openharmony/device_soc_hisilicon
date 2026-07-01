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
#ifndef TDE_BUFFER_H
#define TDE_BUFFER_H

#include "tde_define.h"
#include "ot_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static td_phys_addr_t g_tde_phy_buffer = 0;
static td_u32 g_tde_ref_buffer = 0;
static td_u32 g_tde_tmp_buf = 0;

td_void tde_set_tde_tmp_buffer(td_u32 tde_tmp_buf)
{
    g_tde_tmp_buf = tde_tmp_buf;
}

static osal_spinlock_t g_tde_buf_lock;

static td_phys_addr_t tde_alloc_physic_buffer(td_u32 cbcr_offset)
{
    td_phys_addr_t phys_addr = 0;

    td_ulong lock_flags = 0;

#ifdef CFG_OT_TDE_CSCTMPBUFFER_SIZE
    td_u32 csc_buffer_size = CFG_OT_TDE_CSCTMPBUFFER_SIZE;
#else
    td_u32 csc_buffer_size = g_tde_tmp_buf;
#endif

    tde_spin_lock(&g_tde_buf_lock, lock_flags);

    if ((cbcr_offset * 3) > csc_buffer_size) { /* 3 for component */
            tde_spin_unlock(&g_tde_buf_lock, lock_flags);
            return 0;
    }

    if (g_tde_phy_buffer == 0) {
        g_tde_ref_buffer = 0;
        tde_spin_unlock(&g_tde_buf_lock, lock_flags);

        tde_get_phyaddr_mmb("TDE_TEMP_BUFFER", csc_buffer_size, phys_addr);
        osal_printk("\n csc_buffer_size is %x 0x%lx\n", csc_buffer_size, (td_ulong)phys_addr);
        if (phys_addr == 0) {
            return 0;
        }

        tde_spin_lock(&g_tde_buf_lock, lock_flags);
        g_tde_phy_buffer = phys_addr;
    }

    g_tde_ref_buffer++;
    tde_spin_unlock(&g_tde_buf_lock, lock_flags);

    return g_tde_phy_buffer + cbcr_offset;
}

static td_void tde_free_physic_buffer(td_void)
{
    td_ulong lock_flags = 0;
    td_phys_addr_t tmp_phy_buffer;

    tde_spin_lock(&g_tde_buf_lock, lock_flags);
    if (g_tde_ref_buffer == 0) {
        tde_spin_unlock(&g_tde_buf_lock, lock_flags);
        return;
    }

    g_tde_ref_buffer--;
    if (g_tde_ref_buffer == 0) {
        tmp_phy_buffer = g_tde_phy_buffer;
        g_tde_phy_buffer = 0;
        tde_spin_unlock(&g_tde_buf_lock, lock_flags);
        osal_printk("FREE MMB 0x%lx\n", (td_ulong)tmp_phy_buffer);
        tde_free_mmb(tmp_phy_buffer);
        return;
    }
    tde_spin_unlock(&g_tde_buf_lock, lock_flags);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TDE_BUFFER_H */
