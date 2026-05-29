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
#ifndef MKP_VO_INIT_H
#define MKP_VO_INIT_H

#include "ot_osal.h"
#include "ot_vo_export.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VO_SEMAPHORE_VAL   1

/* vou 的模块状态 */
#define VOU_STATE_STARTED                   0
#define VOU_STATE_STOPPING                  1
#define VOU_STATE_STOPPED                   2

osal_spinlock_t *vo_get_dev_lock(td_void);
osal_semaphore_t *vo_get_dev_sema(td_void);

#ifdef CONFIG_OT_VO_EXPORT_FUNCTION
ot_vo_export_callback *vo_get_export_callback(td_void);
#endif
td_bool vo_get_vo_init_flag(td_void);
td_bool vo_is_vo_started(td_void);
td_bool vo_is_vo_stopped(td_void);

#define vo_down_sem_return()                                   \
    do {                                                       \
        if (osal_down_interruptible(vo_get_dev_sema()) != 0) { \
            return (-ERESTARTSYS);                             \
        }                                                      \
    } while (0)

#define vo_up_sem()         osal_up(vo_get_dev_sema())

#define vo_dev_spin_lock(flags)     osal_spin_lock_irqsave(vo_get_dev_lock(), flags)
#define vo_dev_spin_unlock(flags)   osal_spin_unlock_irqrestore(vo_get_dev_lock(), flags)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef MKP_VO_INIT_H */
