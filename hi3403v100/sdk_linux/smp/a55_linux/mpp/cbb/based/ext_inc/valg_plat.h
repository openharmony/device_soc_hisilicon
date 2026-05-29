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

#ifndef VALG_PLAT_H
#define VALG_PLAT_H

#include "ot_osal.h"

#include "ot_type.h"
#include "ot_debug.h"
#include "ot_common.h"
#include "vb_ext.h"
#include "proc_ext.h"

#ifdef __KERNEL__
#else
#include <sys/time.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define valg_trace(level, mod_id, fmt, ...)                                                                \
    do {                                                                                                    \
        OT_TRACE(level, mod_id, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define VALG_ERR_LEVEL_NOTE 0
typedef osal_spinlock_t valg_lock;

__inline static td_s32 valg_create_lock(valg_lock *lock, td_bool locked)
{
    ot_unused(locked);
    return osal_spin_lock_init(lock);
}

__inline static td_s32 valg_lock_irqsave(valg_lock *lock, td_ulong *flag)
{
    osal_spin_lock_irqsave(lock, flag);
    return TD_SUCCESS;
}

__inline static td_s32 valg_unlock(valg_lock *lock, td_ulong *flag)
{
    osal_spin_unlock_irqrestore(lock, flag);
    return TD_SUCCESS;
}

__inline static td_void valg_delete_lock(valg_lock *lock)
{
    osal_spin_lock_destroy(lock);
    return;
}
#define FORCEINLINE         __inline__ __attribute__((always_inline))

typedef osal_timer_t valg_timer;
typedef td_void (*ptr_timer_call_back)(unsigned long data);

__inline static td_s32 valg_timer_init(valg_timer *ph_timer, ptr_timer_call_back ptr_timer, td_ulong ul_data)
{
    td_s32 ret;

    ph_timer->function = ptr_timer;
    ph_timer->data = ul_data;
    ret = osal_timer_init(ph_timer);
    if (ret != 0) {
        return ret;
    }

    return TD_SUCCESS;
}

__inline static td_s32 valg_timer_start_once(valg_timer *ph_timer, td_ulong ul_interval)
{
    return osal_set_timer(ph_timer, ul_interval);
}

__inline static td_void valg_timer_delete(valg_timer *ph_timer)
{
    osal_del_timer(ph_timer);
    osal_timer_destroy(ph_timer);

    return;
}

__inline static td_s32 valg_thread_init(osal_task_t **thread, td_void *task_function, td_void *data, char *task_name)
{
    *thread = osal_kthread_create(task_function, data, task_name);
    if (*thread == NULL) {
        osal_printk("create thread fail!!!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

__inline static td_u32 valg_thread_delete(osal_task_t *task, td_bool stop_flag)
{
    osal_kthread_destroy(task, stop_flag);
    return TD_SUCCESS;
}

typedef td_u32 valg_pool_handle;
typedef vb_blk_handle valg_block_handle;

__inline static td_s32 valg_vb_init(valg_pool_handle *ph_pool, const td_char *mmz_name, vb_info *info)
{
    return call_vb_create_pool((td_u32 *)ph_pool, mmz_name, info);
}
__inline static td_s32 valg_vb_free(valg_pool_handle h_pool)
{
    return call_vb_destroy_pool((td_u32)h_pool);
}
__inline static valg_block_handle valg_vb_get_blk(valg_pool_handle h_pool, td_u32 uid)
{
    return call_vb_get_blk_by_pool_id((td_u32)h_pool, uid);
}
__inline static td_s32 valg_vb_user_add(valg_block_handle h_block, td_u32 uid)
{
    return call_vb_user_add(call_vb_handle_to_pool_id(h_block), call_vb_handle_to_phys(h_block), uid);
}
__inline static td_s32 valg_vb_user_sub(valg_block_handle h_block, td_u32 uid)
{
    return call_vb_user_sub(call_vb_handle_to_pool_id(h_block), call_vb_handle_to_phys(h_block), uid);
}
__inline static td_s32 valg_vb_user_add_by_phy(td_u32 pool_id, td_phys_addr_t phy_addr, td_u32 uid)
{
    return call_vb_user_add(pool_id, phy_addr, uid);
}

__inline static td_s32 valg_vb_user_sub_by_phy(td_u32 pool_id, td_phys_addr_t phy_addr, td_u32 uid)
{
    return call_vb_user_sub(pool_id, phy_addr, uid);
}

__inline static td_u32 valg_vb_get_user_cnt(valg_block_handle h_block)
{
    return call_vb_inquire_user_cnt(h_block);
}
__inline static td_phys_addr_t valg_vb_handle_to_phys(valg_block_handle h_block)
{
    return call_vb_handle_to_phys(h_block);
}
__inline static td_void *valg_vb_handle_to_kern(valg_block_handle h_block)
{
    return ((td_void *)(td_uintptr_t)call_vb_handle_to_kern(h_block));
}
__inline static td_u32 valg_vb_handle_to_pool_id(valg_block_handle h_block)
{
    return call_vb_handle_to_pool_id(h_block);
}


typedef osal_irq_handler_t ptr_int_callback;

__inline static td_s32 valg_irq_init(td_u32 irq, ptr_int_callback ptr_call_back, td_void *data)
{
    return osal_request_irq(irq, ptr_call_back, NULL, "vedu", data);
}

__inline static td_s32 valg_irq_delete(td_u32 irq, td_void *dev_id)
{
    osal_free_irq(irq, dev_id);
    return TD_SUCCESS;
}

__inline static td_u64 get_sys_time_by_sec(td_void)
{
#ifdef __KERNEL__
    osal_timeval_t time;
    osal_gettimeofday(&time);
#else
    struct timeval time;
    gettimeofday(&time, NULL);
#endif
    return (td_u64)time.tv_sec;
}

__inline static td_u64 get_sys_time_by_usec(td_void)
{
#ifdef __KERNEL__
    osal_timeval_t time;
    osal_gettimeofday(&time);
#else
    struct timeval time;
    gettimeofday(&time, NULL);
#endif
    return (td_u64)((time.tv_sec * 1000000LLU) + time.tv_usec);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
