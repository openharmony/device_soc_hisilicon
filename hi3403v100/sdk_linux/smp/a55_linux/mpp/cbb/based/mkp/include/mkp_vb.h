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

#ifndef MKP_VB_H
#define MKP_VB_H

#include "osal_ioctl.h"
#include "ot_common.h"
#include "ot_common_vb.h"
#include "mkp_ioctl.h"
#include "vb_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct {
    td_u32 pool_id;
    td_u64 blk_size;
    td_u32 blk_cnt;
    td_u32 blk_handle;
    td_bool is_comm_pool;
    ot_vb_remap_mode remap_mode;
    td_char mmz_name[OT_MAX_MMZ_NAME_LEN];
} vb_ioc_arg;

typedef struct {
    ot_vb_uid vb_uid;
    ot_vb_cfg vb_conf;
} vb_ioc_cfg_arg;

typedef struct {
    ot_vb_blk block;
    ot_video_supplement *ATTRIBUTE supplement;
} video_supplement_info;

typedef struct {
    ot_vb_blk vb_blk;
    td_s32 user_cnt;
} vb_ioc_user_cnt;

typedef enum {
    IOC_NR_VB_CREATE = 1,
    IOC_NR_VB_DESTROY,
    IOC_NR_VB_GETBLK,
    IOC_NR_VB_RELBLK,
    IOC_NR_VB_PHYS2H,
    IOC_NR_VB_H2PHYS,
    IOC_NR_VB_H2POOL,
    IOC_NR_VB_INITCPOOL,
    IOC_NR_VB_EXITCPOOL,
    IOC_NR_VB_GETCPOOL,
    IOC_NR_VB_SETCPOOL,
    IOC_NR_VB_GETINFO,
    IOC_NR_VB_QRYUSER,
    IOC_NR_VB_INITMODCPOOL,
    IOC_NR_VB_EXITMODCPOOL,
    IOC_NR_VB_GETMODCPOOL,
    IOC_NR_VB_SETMODCPOOL,
    IOC_NR_VB_SETSUPPLEMENT,
    IOC_NR_VB_GETSUPPLEMENT,
    IOC_NR_VB_GETSUPPLEMENTADDR,
    IOC_NR_VB_H2USERCNT,
} ioc_nr_vb;

#define VB_CREATE_CTRL              OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_CREATE, vb_ioc_arg)
#define VB_DESTROY_CTRL             OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_DESTROY, td_u32)
#define VB_GETBLK_CTRL              OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_GETBLK, vb_ioc_arg)
#define VB_RELBLK_CTRL              OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_RELBLK, td_u32)

#define VB_PHYS2H_CTRL              OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_PHYS2H, td_phys_addr_t)
#define VB_H2PHYS_CTRL              OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_H2PHYS, td_phys_addr_t)
#define VB_H2POOL_CTRL              OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_H2POOL, td_u32)
#define VB_H2USERCNT_CTRL           OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_H2USERCNT, vb_ioc_user_cnt)

#define VB_GETSUPPLEMENTADDR_CTRL   OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_GETSUPPLEMENTADDR, video_supplement_info)

#define VB_INIT_POOL_CTRL           OSAL_IO(IOC_TYPE_VB, IOC_NR_VB_INITCPOOL)
#define VB_EXIT_POOL_CTRL           OSAL_IO(IOC_TYPE_VB, IOC_NR_VB_EXITCPOOL)
#define VB_GET_CONF_CTRL            OSAL_IOR(IOC_TYPE_VB, IOC_NR_VB_GETCPOOL, ot_vb_cfg)
#define VB_SET_CONF_CTRL            OSAL_IOW(IOC_TYPE_VB, IOC_NR_VB_SETCPOOL, ot_vb_cfg)

#define VB_GET_INFO_CTRL            OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_GETINFO, vb_pool_info)
#define VB_QRY_USER_CTRL            OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_QRYUSER, td_u32)

#define VB_INIT_MOD_POOL_CTRL       OSAL_IOW(IOC_TYPE_VB, IOC_NR_VB_INITMODCPOOL, ot_vb_uid)
#define VB_EXIT_MOD_POOL_CTRL       OSAL_IOW(IOC_TYPE_VB, IOC_NR_VB_EXITMODCPOOL, ot_vb_uid)
#define VB_GET_MOD_CONF_CTRL        OSAL_IOWR(IOC_TYPE_VB, IOC_NR_VB_GETMODCPOOL, vb_ioc_cfg_arg)
#define VB_SET_MOD_CONF_CTRL        OSAL_IOW(IOC_TYPE_VB, IOC_NR_VB_SETMODCPOOL, vb_ioc_cfg_arg)

#define VB_SET_SUPPLEMENT_CONF_CTRL OSAL_IOW(IOC_TYPE_VB, IOC_NR_VB_SETSUPPLEMENT, ot_vb_supplement_cfg)
#define VB_GET_SUPPLEMENT_CONF_CTRL OSAL_IOR(IOC_TYPE_VB, IOC_NR_VB_GETSUPPLEMENT, ot_vb_supplement_cfg)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* MKP_VB_H */
