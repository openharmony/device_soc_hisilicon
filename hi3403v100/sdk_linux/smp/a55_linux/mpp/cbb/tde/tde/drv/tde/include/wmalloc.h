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

#ifndef WMALLOC_H
#define WMALLOC_H

#include "ot_type.h"
#include "ot_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* CNcomment:
 * Function: wmeminit
 * Description: init memory pool. CNcomment: TDE memory pool initialization
 * Return: success/error code. CNcomment: Success/error code
 */
td_s32 wmeminit(void);

/*
 * Function: wmemterm
 * Description: Deinit memory pool. CNcomment:TDE memory pool initialization
 */
td_void wmemterm(void);

/*
 * Function: wgetphy
 * Description: get the physics address of memory. CNcomment: To obtain the physical address of memory
 * Input: ptr  the pointer of memory. CNcomment: Pointer to the memory
 * Output:  physics address. CNcomment: Physical address
 */
td_phys_addr_t wgetphy(const td_void *ptr);

/*
 * Function: wgetphy
 * Description: get the virtual address of memory. CNcomment: Access to the virtual address of memory
 * Input: phyaddr  physics address. CNcomment: Physical address
 * Output: virtual address. CNcomment: A virtual address
 */
td_void *wgetvrt(td_phys_addr_t phyaddr);

/*
 * Function: wgetfreenum
 * Description:get the number of the free unit. CNcomment: Memory for the rest of the Unit cell number
 * Return: the number of the free unit. CNcomment: The rest of the unit number
 */
td_u32 wgetfreenum(td_void);
#ifndef TDE_BOOT
osal_proc_entry_t *wprintinfo(osal_proc_entry_t *page);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* WMALLOC_H */
