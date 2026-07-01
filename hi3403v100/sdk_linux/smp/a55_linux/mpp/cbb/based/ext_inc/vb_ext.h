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

#ifndef VB_EXT_H
#define VB_EXT_H

#include "ot_common_vb.h"
#include "ot_common.h"
#include "ot_common_video.h"
#include "mod_ext.h"

typedef td_u32 vb_blk_handle;

typedef struct {
    ot_mpp_chn mod;
    td_s32 (*call_back)(ot_mpp_chn mod, td_phys_addr_t phy_addr);
} vb_create_ex_info;

typedef struct {
    ot_vb_pool pool_id;
    td_bool is_user_vb;
    ot_vb_pool_info pool_info;
} vb_pool_info;

typedef struct {
    td_u32 blk_cnt;
    td_u64 blk_size;
    ot_vb_uid uid;
    td_char buf_name[OT_MAX_MMZ_NAME_LEN];
    ot_vb_remap_mode vb_remap_mode;
} vb_info;

/* create a private VB poo */
extern td_s32 vb_create_pool(td_u32 *pool_id, const td_char *pc_mmz_name, const vb_info *info);

/* add a buffer block to a specified pool */
extern td_s32 vb_get_pool_info(vb_pool_info *info);

extern td_s32 vb_destroy_pool(td_u32 pool_id);

#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
extern td_s32 vb_async_destroy_pool(td_u32 pool_id);
#endif

/* get the first one in module common pools in which block size is greater than  'blk_size' */
extern td_s32 vb_get_pool_id_by_size_and_module(td_u64 blk_size, td_u32 *pool_id,
    const td_char *mmz_name, ot_vb_uid uid);

/* get the first one in common pools in which block size is greater than  'blk_size' */
extern td_s32 vb_get_pool_id(td_u64 blk_size, td_u32 *pool_id, const td_char *mmz_name, ot_vb_uid uid);

/* get a buffer block from a specified pool */
extern vb_blk_handle vb_get_blk_by_pool_id(td_u32 pool_id, td_u32 uid);

/* get a block in the first fit common pool */
extern vb_blk_handle vb_get_blk_by_size(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name);

/* get a block in the module common pool */
extern vb_blk_handle vb_get_blk_by_size_and_module(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name);

/* get a block in the pool with id */
extern vb_blk_handle vb_get_blk_by_size_and_pool_id(td_u32 pool_id, td_u64 blk_size, td_u32 uid);

/*
 * forcebly free the buffer even if it is used by more than one user.
 * put a block back to free list, reset all counter for all users.
 */
extern td_s32 vb_put_blk(td_u32 pool_id, td_phys_addr_t phy_addr);

/* convert physical address to handle */
extern vb_blk_handle vb_phy_to_handle(td_phys_addr_t phy_addr);

/* convert handle to physical address of a buffer */
extern td_phys_addr_t vb_handle_to_phys(vb_blk_handle handle);

/* convert handle to kernel virtual address of a buffer */
extern td_phys_addr_t vb_handle_to_kern(vb_blk_handle handle);

/* convert handle to the size a buffer */
extern td_u64 vb_handle_to_blk_size(vb_blk_handle handle);

/* convert handle to pool ID */
extern td_u32 vb_handle_to_pool_id(vb_blk_handle handle);

/* convert handle to pool ID */
extern td_u32 vb_handle_to_blk_id(vb_blk_handle handle);

/* increase one to reference counter of  a buffer */
extern td_s32 vb_user_add(td_u32 pool_id, td_phys_addr_t phy_addr, td_u32 uid);

/* decrease one to reference counter of a buffer */
extern td_s32 vb_user_sub(td_u32 pool_id, td_phys_addr_t phy_addr, td_u32 uid);

/* inquire how many users are using the buffer */
extern td_u32 vb_inquire_user_cnt(vb_blk_handle handle);

extern td_u32 vb_inquire_one_user_cnt(vb_blk_handle handle, td_u32 uid);

extern td_u32 vb_inquire_total_user_cnt(vb_blk_handle handle);

/* inquire how many blocks the user is using */
extern td_u32 vb_inquire_blk_cnt(td_u32 uid, td_bool is_comm_pool);

/* inquire the statistic of a pool */
extern td_s32 vb_inquire_pool(td_u32 pool_id, ot_vb_pool_status *pool_status);

/* inquire how many blocks the user used in this pool */
extern td_s32 vb_inquire_pool_user_cnt(td_u32 pool_id, td_u32 uid, td_u32 *cnt);

/* check if the address is valid */
extern td_bool vb_is_blk_valid(td_u32 pool_id, td_phys_addr_t phys_addr);

/* convert handle to video_supplement */
extern ot_video_supplement *vb_handle_to_supplement(vb_blk_handle handle);

/* check whether supplement is supported */
extern td_bool vb_is_supplement_support(td_u32 mask);

/* initialize video buffer modules */
extern td_s32 vb_init(td_void);

/* cleanup video buffer module */
extern td_s32 vb_exit(td_void);

/* copy the supplement info form source to destination */
extern td_void vb_copy_supplement(ot_video_frame *dst_v_frame, const ot_video_frame *src_v_frame);

extern td_s32 vb_get_mod_pool_vb_config(ot_vb_uid uid, ot_vb_cfg *vb_cfg);

#ifdef CONFIG_OT_VB_LOG_SUPPORT
/* get a block in the first fit common pool */
vb_blk_handle vb_get_blk_by_size_ex(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name, td_u32 chn);

/* get a block in the pool with id */
vb_blk_handle vb_get_blk_by_size_and_pool_id_ex(td_u32 pool_id, td_u64 blk_size, td_u32 uid, td_u32 chn);

/* get a block in the module common pool */
vb_blk_handle vb_get_blk_by_size_and_module_ex(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name, td_u32 chn);
#endif

typedef struct {
    td_s32 (*pfn_vb_create_pool)(td_u32 *pool_id, const td_char *pc_mmz_name, const vb_info *info);
    td_s32 (*pfn_vb_destroy_pool)(td_u32 pool_id);
    vb_blk_handle (*pfn_vb_get_blk_by_pool_id)(td_u32 pool_id, td_u32 uid);
    vb_blk_handle (*pfn_vb_get_blk_by_size)(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name);
    vb_blk_handle (*pfn_vb_get_blk_by_size_and_module)(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name);
    vb_blk_handle (*pfn_vb_get_blk_by_size_and_pool_id)(td_u32 pool_id, td_u64 blk_size, td_u32 uid);
    td_s32 (*pfn_vb_get_pool_id_by_size_and_module)(td_u64 blk_size, td_u32 *pool_id, \
            const td_char *pc_mmz_name, ot_vb_uid uid);
    td_s32 (*pfn_vb_get_pool_id)(td_u64 blk_size, td_u32 *pool_id, const td_char *pc_mmz_name, ot_vb_uid uid);
    td_s32 (*pfn_vb_get_pool_info)(vb_pool_info *info);
    td_s32 (*pfn_vb_put_blk)(td_u32 pool_id, td_phys_addr_t phy_addr);
    vb_blk_handle (*pfn_vb_phy_to_handle)(td_phys_addr_t phy_addr);
    td_u32 (*pfn_vb_handle_to_pool_id)(vb_blk_handle handle);
    td_u32 (*pfn_vb_handle_to_blk_id)(vb_blk_handle handle);
    td_phys_addr_t (*pfn_vb_handle_to_kern)(vb_blk_handle handle);
    td_phys_addr_t (*pfn_vb_handle_to_phys)(vb_blk_handle handle);
    td_u64 (*pfn_vb_handle_to_blk_size)(vb_blk_handle handle);
    td_s32 (*pfn_vb_user_add)(td_u32 pool_id, td_phys_addr_t phy_addr, td_u32 uid);
    td_s32 (*pfn_vb_user_sub)(td_u32 pool_id, td_phys_addr_t phy_addr, td_u32 uid);
    td_u32 (*pfn_vb_inquire_user_cnt)(vb_blk_handle handle);
    td_u32 (*pfn_vb_inquire_one_user_cnt)(vb_blk_handle handle, td_u32 uid);
    td_u32 (*pfn_vb_inquire_total_user_cnt)(vb_blk_handle handle);
    td_u32 (*pfn_vb_inquire_blk_cnt)(td_u32 uid, td_bool is_comm_pool);
    td_bool (*pfn_vb_is_blk_valid)(td_u32 pool_id, td_phys_addr_t phy_addr);
    td_bool (*pfn_vb_is_pool_id_valid)(td_u32 pool_id);
    td_s32 (*pfn_vb_inquire_pool)(td_u32 pool_id, ot_vb_pool_status *pool_status);
    td_s32 (*pfn_vb_inquire_pool_user_cnt)(td_u32 pool_id, td_u32 uid, td_u32 *cnt);
    ot_video_supplement *(*pfn_vb_handle_to_supplement)(vb_blk_handle handle);
    td_bool (*pfn_vb_is_supplement_support)(td_u32 mask);
    td_void (*pfn_vb_copy_supplement)(ot_video_frame *dst_v_frame, const ot_video_frame *src_v_frame);
    td_s32 (*pfn_vb_get_mod_pool_vb_config)(ot_vb_uid uid, ot_vb_cfg *vb_cfg);
#ifdef CONFIG_OT_VB_LOG_SUPPORT
    vb_blk_handle (*pfn_vb_get_blk_by_size_ex)(td_u64 blk_size, ot_vb_uid uid, const td_char *pc_mmz_name, td_u32 chn);
    vb_blk_handle (*pfn_vb_get_blk_by_size_and_pool_id_ex)(td_u32 pool_id, td_u64 blk_size, td_u32 uid, td_u32 chn);
    vb_blk_handle (*pfn_vb_get_blk_by_size_and_module_ex)(td_u64 blk_size, ot_vb_uid uid,
        const td_char *pc_mmz_name, td_u32 chn);
#endif
#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
    td_s32 (*pfn_vb_async_destroy_pool)(td_u32 pool_id);
#endif
} vb_export_func;

#define ckfn_vb_create_pool() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_create_pool != NULL)
#define call_vb_create_pool(pool_id, pc_mmz_name, info) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_create_pool(pool_id, pc_mmz_name, info)

#define ckfn_vb_destroy_pool() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_destroy_pool != NULL)
#define call_vb_destroy_pool(pool_id) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_destroy_pool(pool_id)

#ifdef CONFIG_OT_VB_ASYNC_SUPPORT
#define ckfn_vb_async_destroy_pool() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_async_destroy_pool != NULL)
#define call_vb_async_destroy_pool(pool_id) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_async_destroy_pool(pool_id)
#endif

#define ckfn_vb_get_blk_by_pool_id() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_pool_id != NULL)
#define call_vb_get_blk_by_pool_id(pool_id, uid) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_pool_id(pool_id, uid)

#define ckfn_vb_get_blk_by_size() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size != NULL)
#define call_vb_get_blk_by_size(blk_size, uid, pc_mmz_name) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size(blk_size, uid, pc_mmz_name)

#define ckfn_vb_get_blk_by_size_and_module() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_and_module != NULL)
#define call_vb_get_blk_by_size_and_module(blk_size, uid, pc_mmz_name) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_and_module(blk_size, uid, pc_mmz_name)

#define ckfn_vb_get_blk_by_size_and_pool_id() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_and_pool_id != NULL)
#define call_vb_get_blk_by_size_and_pool_id(pool_id, blk_size, uid) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_and_pool_id(pool_id, blk_size, uid)

#define ckfn_vb_get_pool_id_by_size_and_module() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_pool_id_by_size_and_module != NULL)
#define call_vb_get_pool_id_by_size_and_module(blk_size, pool_id, mmz_name, uid) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_pool_id_by_size_and_module(blk_size, pool_id, mmz_name, uid)
#define ckfn_vb_get_pool_id() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_pool_id != NULL)
#define call_vb_get_pool_id(blk_size, pool_id, mmz_name, uid) \
        func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_pool_id(blk_size, pool_id, mmz_name, uid)

#define ckfn_vb_get_pool_info() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_pool_info != NULL)
#define call_vb_get_pool_info(info) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_pool_info(info)

#define ckfn_vb_put_blk() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_put_blk != NULL)
#define call_vb_put_blk(pool_id, phy_addr) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_put_blk(pool_id, phy_addr)

#define ckfn_vb_phy_to_handle() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_phy_to_handle != NULL)
#define call_vb_phy_to_handle(phy_addr) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_phy_to_handle(phy_addr)

#define ckfn_vb_handle_to_pool_id() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_pool_id != NULL)
#define call_vb_handle_to_pool_id(handle) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_pool_id(handle)

#define ckfn_vb_handle_to_blk_id() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_blk_id != NULL)
#define call_vb_handle_to_blk_id(handle) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_blk_id(handle)

#define ckfn_vb_handle_to_kern() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_kern != NULL)
#define call_vb_handle_to_kern(handle) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_kern(handle)

#define ckfn_vb_handle_to_phys() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_phys != NULL)
#define call_vb_handle_to_phys(handle) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_phys(handle)

#define ckfn_vb_handle_to_blk_size() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_blk_size != NULL)
#define call_vb_handle_to_blk_size(handle) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_blk_size(handle)

#define ckfn_vb_user_add() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_user_add != NULL)
#define call_vb_user_add(pool_id, phy_addr, uid) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_user_add(pool_id, phy_addr, uid)

#define ckfn_vb_user_sub() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_user_sub != NULL)
#define call_vb_user_sub(pool_id, phy_addr, uid) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_user_sub(pool_id, phy_addr, uid)

#define ckfn_vb_inquire_user_cnt() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_user_cnt != NULL)
#define call_vb_inquire_user_cnt(handle) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_user_cnt(handle)

#define ckfn_vb_inquire_one_user_cnt() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_one_user_cnt != NULL)
#define call_vb_inquire_one_user_cnt(handle, uid) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_one_user_cnt(handle, uid)

#define ckfn_vb_inquire_total_user_cnt() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_total_user_cnt != NULL)
#define call_vb_inquire_total_user_cnt(handle) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_total_user_cnt(handle)

#define ckfn_vb_inquire_blk_cnt() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_blk_cnt != NULL)
#define call_vb_inquire_blk_cnt(uid, is_comm_pool) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_blk_cnt(uid, is_comm_pool)

#define ckfn_vb_is_blk_valid() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_is_blk_valid != NULL)
#define call_vb_is_blk_valid(pool_id, phy_addr) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_is_blk_valid(pool_id, phy_addr)

#define ckfn_vb_is_pool_id_valid() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_is_pool_id_valid != NULL)
#define call_vb_is_pool_id_valid(pool_id) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_is_pool_id_valid(pool_id)

#define ckfn_vb_inquire_pool() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_pool != NULL)
#define call_vb_inquire_pool(pool_id, pool_status) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_pool(pool_id, pool_status)

#define ckfn_vb_inquire_pool_user_cnt() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_pool_user_cnt != NULL)
#define call_vb_inquire_pool_user_cnt(pool_id, uid, cnt) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_inquire_pool_user_cnt(pool_id, uid, cnt)

#define ckfn_vb_handle_to_supplement() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_supplement != NULL)
#define call_vb_handle_to_supplement(handle) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_handle_to_supplement(handle)

#define ckfn_vb_is_supplement_support() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_is_supplement_support != NULL)
#define call_vb_is_supplement_support(mask) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_is_supplement_support(mask)

#define ckfn_vb_copy_supplement() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_copy_supplement != NULL)
#define call_vb_copy_supplement(dst_v_frame, src_v_frame) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_copy_supplement(dst_v_frame, src_v_frame)

#define ckfn_vb_get_mod_pool_vb_config() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_mod_pool_vb_config != NULL)
#define call_vb_get_mod_pool_vb_config(uid, vb_config) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_mod_pool_vb_config(uid, vb_config)

#ifdef CONFIG_OT_VB_LOG_SUPPORT
typedef struct {
    td_void (*pfn_vb_analysisy_create_pool_log)(const vb_pool_info *pool_info);
    td_void (*pfn_vb_analysisy_destroy_pool_log)(td_u32 pool_id);
    td_void (*pfn_vb_analysisy_vb_log_record)(td_u32 pool_id, td_u32 blk_id, td_u32 uid, td_bool opt, td_u32 chn);
    td_void (*pfn_vb_analysisy_vb_save_seg)(td_u32 pool_id, td_u32 blk_id);
    td_void (*pfn_vb_analysisy_log_record)(ot_mod_id mod_id, td_u32 dev_id, td_u32 opt);
} vb_analysisy_export_func;

#define ckfn_vb_get_blk_by_size_ex() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_ex != NULL)
#define call_vb_get_blk_by_size_ex(blk_size, uid, pc_mmz_name, chn) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_ex(blk_size, uid, pc_mmz_name, chn)

#define ckfn_vb_get_blk_by_size_and_pool_id_ex() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_and_pool_id_ex != NULL)
#define call_vb_get_blk_by_size_and_pool_id_ex(pool_id, blk_size, uid, chn) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_and_pool_id_ex(pool_id, blk_size, uid, chn)

#define ckfn_vb_get_blk_by_size_and_module_ex() \
    (func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_and_module_ex != NULL)
#define call_vb_get_blk_by_size_and_module_ex(blk_size, uid, pc_mmz_name, chn) \
    func_entry(vb_export_func, OT_ID_VB)->pfn_vb_get_blk_by_size_and_module_ex(blk_size, uid, pc_mmz_name, chn)

#define ckfn_vb_analysisy_create_pool_log() \
    (func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_create_pool_log != NULL)
#define call_vb_analysisy_create_pool_log(pool_info) \
    func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_create_pool_log(pool_info)

#define ckfn_vb_analysisy_destroy_pool_log() \
    (func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_destroy_pool_log != NULL)
#define call_vb_analysisy_destroy_pool_log(pool_id) \
    func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_destroy_pool_log(pool_id)

#define ckfn_vb_analysisy_vb_log_record() \
    (func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_vb_log_record != NULL)
#define call_vb_analysisy_vb_log_record(pool_id, blk_id, uid, opt, chn) \
    func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_vb_log_record(pool_id, blk_id, uid, opt, chn)

#define ckfn_vb_analysisy_vb_log_record() \
    (func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_vb_log_record != NULL)
#define call_vb_analysisy_vb_log_record(pool_id, blk_id, uid, opt, chn) \
    func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_vb_log_record(pool_id, blk_id, uid, opt, chn)

#define ckfn_vb_analysisy_vb_save_seg() \
    (func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_vb_save_seg != NULL)
#define call_vb_analysisy_vb_save_seg(pool_id, blk_id) \
    func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_vb_save_seg(pool_id, blk_id)

#define ckfn_vb_analysisy_log_record() \
    (func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_log_record != NULL)
#define call_vb_analysisy_log_record(mod_id, dev_id, opt) \
    func_entry(vb_analysisy_export_func, OT_ID_VB_LOG)->pfn_vb_analysisy_log_record(mod_id, dev_id, opt)
#endif

#endif /* VB_EXT_H */

