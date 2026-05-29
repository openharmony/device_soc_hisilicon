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

#ifndef TDE_DEFINE_H
#define TDE_DEFINE_H

#include "ot_type.h"
#include "ot_debug.h"
#include "wmalloc.h"
#include "tde_config.h"
#include "ot_osal.h"
#include "mm_ext.h"
#include "proc_ext.h"
#include "mod_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Pixel format classify */
typedef enum {
    TDE_COLORFMT_CATEGORY_ARGB,
    TDE_COLORFMT_CATEGORY_CLUT,
    TDE_COLORFMT_CATEGORY_AN,
    TDE_COLORFMT_CATEGORY_YCBCR,
    TDE_COLORFMT_CATEGORY_BYTE,
    TDE_COLORFMT_CATEGORY_HALFWORD,
    TDE_COLORFMT_CATEGORY_MB,
    TDE_COLORFMT_CATEGORY_BUTT
} tde_colorfmt_category;

/* Node submit type, Independent operation node, Operated head node, Operated middle node */
typedef enum {
    TDE_NODE_SUBM_ALONE = 0,  /* current node submit as independent operated node */
    TDE_NODE_SUBM_PARENT = 1, /* current node submit as operated parent node */
    TDE_NODE_SUBM_CHILD       /* current node submit as operated child node */
} tde_node_subm_type;

/* Operation setting information node */
typedef struct {
    td_void *buf;    /* setting information node buffer */
    td_u32 node_sz; /* current node occupied size, united in byte */
    td_u64 update; /* current node update flag */
    td_phys_addr_t phy_addr;  /* current node addr in hardware list */
} tde_node_buf;

/* Notified mode  after current node's job end */
typedef enum {
    TDE_JOB_NONE_NOTIFY = 0, /* none notify after job end */
    TDE_JOB_COMPL_NOTIFY,    /* notify after job end */
    TDE_JOB_WAKE_NOTIFY,     /* wake noytify after job end */
    TDE_JOB_NOTIFY_BUTT
} tde_notify_mode;

#define TDE_MAX_WAIT_TIMEOUT 2000

#define MMB_ADDR_INVALID 1

#define tde_fatal(fmt, ...)   \
    OT_EMERG_TRACE(OT_ID_TDE, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define tde_error(fmt, ...)   \
    OT_ERR_TRACE(OT_ID_TDE, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define tde_warning(fmt, ...) \
    OT_WARN_TRACE(OT_ID_TDE, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

#define tde_info(fmt, ...)    \
    OT_INFO_TRACE(OT_ID_TDE, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__) \

td_void *tde_malloc(td_u32 size);
td_void tde_free(td_void *ptr);
#define tde_spin_lock(lock, flag)   osal_spin_lock_irqsave((lock), &(flag))
#define tde_spin_unlock(lock, flag) osal_spin_unlock_irqrestore((lock), &(flag))

#define tde_free_mmb(phyaddr)              \
    do {                                   \
        if ((phyaddr) != MMB_ADDR_INVALID) { \
            cmpi_mmz_free((phyaddr), TD_NULL); \
        }                                  \
    } while (0)

#define tde_get_phyaddr_mmb(bufname, size, phyaddr)    \
    do {                                               \
        (phyaddr) = cmpi_mmz_malloc(TD_NULL, (bufname), (size), TD_TRUE); \
        if ((phyaddr) == 0) {                           \
            tde_error("new_mmb failed!");              \
            (phyaddr) = 0;                              \
        }                                              \
    } while (0)

#define tde_unmap_mmb(virtaddr) cmpi_unmap((td_void *)(virtaddr))

#define tde_flush_cached_mmb(virtaddr, phyaddr, len)    \
    do {                                                \
        osal_flush_dcache_area((virtaddr), (phyaddr), (len)); \
    } while (0)

#define tde_unequal_eok_return(ret)                       \
    do {                                                  \
        if ((ret) != EOK) {                               \
            tde_error("secure function failure\n");       \
            return DRV_ERR_TDE_INVALID_PARA;              \
        }                                                 \
    } while (0)

#define tde_reg_map(base, size) osal_ioremap_nocache((base), (size))

#define tde_reg_unmap(base, size) osal_iounmap((td_void *)(base), (size))

#define tde_init_waitqueue_head(pqueue) osal_wait_init(pqueue)

#define tde_wait_event_interruptible_timeout(queue, condition, timeout) osal_wait_timeout(queue, condition, timeout)

#define tde_wakeup_interruptible(pqueue) osal_wakeup(pqueue)

typedef osal_timeval_t tde_timeval;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* TDE_DEFINE_H */
