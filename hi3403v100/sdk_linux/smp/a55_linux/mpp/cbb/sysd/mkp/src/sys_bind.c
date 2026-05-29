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

#include "sys_bind.h"

#include "securec.h"

#include "ot_osal.h"
#include "mm_ext.h"
#include "mod_ext.h"
#include "proc_ext.h"
#include "ot_common_sys.h"
#include "sys_ext.h"
#include "mkp_sys.h"
#include "sys_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define BIND_NAME_MAX_LEN     16

#if (OT_CHIP == OT_CHIP_SS626V100)
#define BIND_MAX_TABLE_SIZE   8192
#else
#define BIND_MAX_TABLE_SIZE   4096
#endif

static osal_spinlock_t g_sys_bind_lock;

static osal_semaphore_t g_sys_dev_sem;

#define bind_spin_lock_init()   osal_spin_lock_init(&g_sys_bind_lock)
#define bind_spin_lock_uninit() osal_spin_lock_destroy(&g_sys_bind_lock)
#define bind_spin_lock(flags) osal_spin_lock_irqsave(&g_sys_bind_lock, &(flags))
#define bind_spin_unlock(flags) osal_spin_unlock_irqrestore(&g_sys_bind_lock, &(flags))
#define bind_kmalloc(size) osal_kmalloc((size), osal_gfp_kernel)
#define bind_kfree             osal_kfree

static char *bind_get_mod_name(ot_mod_id mod)
{
    char *mod_name = cmpi_get_module_name(mod);
    if (mod_name == TD_NULL) {
        return "unknown mod";
    }

    return mod_name;
}

#define bind_sem_init()         osal_sema_init(&g_sys_dev_sem, 1)
#define bind_sem_uninit()       osal_sema_destroy(&g_sys_dev_sem)

#define bind_sem_down_return()                         \
    do {                                               \
        if (osal_down(&g_sys_dev_sem)) { \
            return (-ERESTARTSYS);                     \
        }                                              \
    } while (0)

#define bind_sem_up()   osal_up(&g_sys_dev_sem)

typedef struct {
    struct osal_list_head list;
    ot_mpp_chn mpp_chn;
    td_u32 send_cnt;
    td_u32 reset_cnt;
} sys_bind_node;

typedef struct {
    struct osal_list_head list_head;
    td_u32 dest_num;
} sys_bind_src;

typedef struct {
    ot_mod_id mod_id;
    td_char name[BIND_NAME_MAX_LEN];
    td_u32 max_dev_cnt;
    td_u32 max_chn_cnt;
    td_s32(*call_back)
    (td_s32 dev_id, td_s32 chn_id, td_bool block,
     mpp_data_type data_type, td_void *pv_data);

    td_s32 (*reset_call_back)(td_s32 dev_id, td_s32 chn_id, td_void *v_data);

    td_s32 (*give_bind_call_back)(td_s32 dev_id, td_s32 chn_id, ot_mpp_bind_dst *bind_send);

    sys_binder_type type;
    td_u32 tbl_size;
    sys_bind_src *src_tbl;
    sys_bind_dest *dest_tbl;

    td_bool support_delay_data;
} sys_binder_ctx;

typedef struct {
    td_u32 max_index;
    ot_mpp_bind_dst **ppst_send_bind_src;
} sys_binder_send;

static sys_binder_ctx *g_sender_tbl[OT_ID_BUTT];

static sys_binder_ctx *g_receiver_tbl[OT_ID_BUTT];

static sys_binder_send g_bind_send[OT_ID_BUTT];

static td_u32 sys_get_idx_by_dev_chn(const sys_binder_ctx *binder_ctx, td_s32 dev_id, td_s32 chn_id)
{
    return (binder_ctx->max_chn_cnt * dev_id) + chn_id;
}

static td_void sys_get_dev_chn_by_idx(const sys_binder_ctx *binder_ctx, td_s32 index, td_s32 *dev_id, td_s32 *chn_id)
{
    if (binder_ctx->max_chn_cnt != 0) {
        *dev_id = index / binder_ctx->max_chn_cnt;
        *chn_id = index % binder_ctx->max_chn_cnt;
    }
}

td_s32 sys_check_bind_chn(sys_binder_type type, const ot_mpp_chn *bind_chn)
{
    sys_binder_ctx *binder_ctx = TD_NULL;

    if (bind_chn->mod_id >= OT_ID_BUTT) {
        sys_err_trace("mod_id:%d is invalid !\n", bind_chn->mod_id);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    binder_ctx = (type == SYS_BINDER_SENDER) ? g_sender_tbl[bind_chn->mod_id] :
                    g_receiver_tbl[bind_chn->mod_id];
    if (binder_ctx == TD_NULL) {
        sys_err_trace("mod_id:%d(%s) is not supported !\n",
            bind_chn->mod_id, bind_get_mod_name(bind_chn->mod_id));
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    if ((bind_chn->dev_id >= (td_s32)binder_ctx->max_dev_cnt) ||
        (bind_chn->chn_id >= (td_s32)binder_ctx->max_chn_cnt) ||
        (bind_chn->dev_id <= -1) ||
        (bind_chn->chn_id <= -1)) {
        sys_err_trace("bind dev or chn is invalid, dev:%d, chn:%d !\n",
            bind_chn->dev_id, bind_chn->chn_id);
            return OT_ERR_SYS_ILLEGAL_PARAM;
        }

    return TD_SUCCESS;
}

td_s32 sys_get_binder_by_src(const ot_mpp_chn *src_chn, ot_mpp_bind_dst *bind_dest)
{
    td_s32 ret;
    td_u32 src_tbl_idx;
    sys_bind_node *node = TD_NULL;
    sys_bind_src *src_bind = TD_NULL;
    sys_binder_ctx *sender_ctx = TD_NULL;
    struct osal_list_head *list_tmp = TD_NULL;
    struct osal_list_head *list_node = TD_NULL;

    sys_check_null_ptr_return(src_chn);
    sys_check_null_ptr_return(bind_dest);

    ret = sys_check_bind_chn(SYS_BINDER_SENDER, src_chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    sender_ctx = g_sender_tbl[src_chn->mod_id];

    src_tbl_idx = sys_get_idx_by_dev_chn(sender_ctx, src_chn->dev_id, src_chn->chn_id);
    src_bind = &sender_ctx->src_tbl[src_tbl_idx];

    bind_dest->num = 0;
    osal_list_for_each_safe(list_node, list_tmp, &src_bind->list_head) {
        bind_dest->num++;

        if (bind_dest->num > OT_MAX_BIND_DST_NUM) {
            sys_err_trace("bind src(mod:%d,dev:%d,chn:%d) too many!\n",
                          src_chn->mod_id, src_chn->dev_id, src_chn->chn_id);
            return OT_ERR_SYS_NOT_PERM;
        }

        node = osal_list_entry(list_node, sys_bind_node, list);
        (td_void)memcpy_s(&bind_dest->mpp_chn[bind_dest->num - 1], sizeof(ot_mpp_chn),
                          &node->mpp_chn, sizeof(ot_mpp_chn));
    }

    return TD_SUCCESS;
}

td_s32 sys_get_bind_num_by_src(const ot_mpp_chn *src_chn, td_u32 *bind_num)
{
    td_s32 ret;
    unsigned long flags;
    td_u32 src_tbl_idx;
    sys_bind_src *src_bind = TD_NULL;
    sys_binder_ctx *sender_ctx = TD_NULL;

    sys_check_null_ptr_return(src_chn);

    bind_spin_lock(flags);

    ret = sys_check_bind_chn(SYS_BINDER_SENDER, src_chn);
    if (ret != TD_SUCCESS) {
        bind_spin_unlock(flags);
        return ret;
    }

    sender_ctx = g_sender_tbl[src_chn->mod_id];

    src_tbl_idx = sys_get_idx_by_dev_chn(sender_ctx, src_chn->dev_id, src_chn->chn_id);
    src_bind = &sender_ctx->src_tbl[src_tbl_idx];
    *bind_num = src_bind->dest_num;

    bind_spin_unlock(flags);

    return TD_SUCCESS;
}

td_s32 sys_register_bind_sender(sys_binder_ctx *binder_ctx, ot_mod_id mod_id,
    td_u32 bind_tbl_size, const bind_sender_info *sender)
{
    td_u32 i;
    errno_t err;

    binder_ctx->src_tbl = (sys_bind_src *)bind_kmalloc(bind_tbl_size * sizeof(sys_bind_src));
    if (binder_ctx->src_tbl == TD_NULL) {
        sys_err_trace("no memory for bind table!\n");
        return OT_ERR_SYS_NO_MEM;
    }

    if (g_bind_send[mod_id].ppst_send_bind_src == TD_NULL) {
        g_bind_send[mod_id].max_index = sender->max_dev_cnt * sender->max_chn_cnt;
        g_bind_send[mod_id].ppst_send_bind_src =
            (ot_mpp_bind_dst **)bind_kmalloc(sizeof(ot_mpp_bind_dst *) * g_bind_send[mod_id].max_index);
        if (g_bind_send[mod_id].ppst_send_bind_src == TD_NULL) {
            sys_err_trace("no memory for bind SRC!\n");
            bind_kfree(binder_ctx->src_tbl);
            binder_ctx->src_tbl = TD_NULL;
            return OT_ERR_SYS_NO_MEM;
        }

        err = memset_s(g_bind_send[mod_id].ppst_send_bind_src, (sizeof(ot_mpp_bind_dst *) *
            g_bind_send[mod_id].max_index), 0, (sizeof(ot_mpp_bind_dst *) * g_bind_send[mod_id].max_index));
        if (err != EOK) {
            sys_err_trace("memset_s err!\n");
            bind_kfree(binder_ctx->src_tbl);
            binder_ctx->src_tbl = TD_NULL;
            return OT_ERR_SYS_ILLEGAL_PARAM;
        }
    }

    for (i = 0; i < bind_tbl_size; i++) {
        OSAL_INIT_LIST_HEAD(&binder_ctx->src_tbl[i].list_head);
        binder_ctx->src_tbl[i].dest_num = 0;
    }

    binder_ctx->mod_id = sender->mod_id;
    binder_ctx->max_dev_cnt = sender->max_dev_cnt;
    binder_ctx->max_chn_cnt = sender->max_chn_cnt;
    binder_ctx->give_bind_call_back = sender->give_bind_call_back;

    g_sender_tbl[mod_id] = binder_ctx;
    return TD_SUCCESS;
}

td_s32 sys_register_bind_receiver(sys_binder_ctx *binder_ctx, ot_mod_id mod_id,
    td_u32 bind_tbl_size, const bind_receiver_info *receiver)
{
    td_u32 i;

    binder_ctx->dest_tbl = (sys_bind_dest *)bind_kmalloc(bind_tbl_size * sizeof(sys_bind_dest));
    if (binder_ctx->dest_tbl == TD_NULL) {
        sys_err_trace("no memory for bind table!\n");
        return OT_ERR_SYS_NO_MEM;
    }

    for (i = 0; i < bind_tbl_size; i++) {
        binder_ctx->dest_tbl[i].is_bind_src = TD_FALSE;
    }

    binder_ctx->mod_id = receiver->mod_id;
    binder_ctx->max_dev_cnt = receiver->max_dev_cnt;
    binder_ctx->max_chn_cnt = receiver->max_chn_cnt;
    binder_ctx->call_back = receiver->call_back;
    binder_ctx->reset_call_back = receiver->reset_call_back;
    binder_ctx->support_delay_data = receiver->support_delay_data;

    g_receiver_tbl[mod_id] = binder_ctx;

    return TD_SUCCESS;
}

static td_s32 sys_binder_comm_check(sys_binder_type type, ot_mod_id mod_id, td_u32 bind_tbl_size)
{
    sys_binder_ctx *binder_ctx = TD_NULL;

    if (mod_id >= OT_ID_BUTT) {
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    binder_ctx = (type == SYS_BINDER_SENDER) ? g_sender_tbl[mod_id] : g_receiver_tbl[mod_id];
    if (binder_ctx != TD_NULL) {
        sys_err_trace("mod %d have register!\n", mod_id);
        return OT_ERR_SYS_NOT_PERM;
    }

    if ((bind_tbl_size == 0) || (bind_tbl_size > BIND_MAX_TABLE_SIZE)) {
        sys_err_trace("max_dev_cnt or max_chn_cnt invalid!\n");
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 sys_register_binder(sys_binder_type type, const bind_sender_info *sender,
    const bind_receiver_info *receiver)
{
    ot_mod_id mod_id = OT_ID_BUTT;
    td_s32 ret;
    td_u32 bind_tbl_size;
    sys_binder_ctx *binder_ctx = TD_NULL;

    if ((type != SYS_BINDER_SENDER) && (type != SYS_BINDER_RECEIVER)) {
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    if (type == SYS_BINDER_SENDER) {
        sys_check_null_ptr_return(sender);
        mod_id = sender->mod_id;
        bind_tbl_size = sender->max_dev_cnt * sender->max_chn_cnt;
    } else {
        sys_check_null_ptr_return(receiver);
        sys_check_null_ptr_return(receiver->call_back);
        mod_id = receiver->mod_id;
        bind_tbl_size = receiver->max_dev_cnt * receiver->max_chn_cnt;
    }

    ret = sys_binder_comm_check(type, mod_id, bind_tbl_size);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    binder_ctx = (sys_binder_ctx *)bind_kmalloc(sizeof(sys_binder_ctx));
    if (binder_ctx == TD_NULL) {
        return OT_ERR_SYS_NO_MEM;
    }

    ret = snprintf_s(binder_ctx->name, BIND_NAME_MAX_LEN, BIND_NAME_MAX_LEN - 1, bind_get_mod_name(mod_id));
    if (ret < 0) {
        bind_kfree(binder_ctx);
        binder_ctx = TD_NULL;
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }
    binder_ctx->tbl_size = bind_tbl_size;

    if (type == SYS_BINDER_SENDER) {
        ret = sys_register_bind_sender(binder_ctx, mod_id, bind_tbl_size, sender);
    } else {
        ret = sys_register_bind_receiver(binder_ctx, mod_id, bind_tbl_size, receiver);
    }
    if (ret != TD_SUCCESS) {
        bind_kfree(binder_ctx);
        binder_ctx = TD_NULL;
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sys_unregister_binder(sys_binder_type type, ot_mod_id mod_id)
{
    td_u32 i;
    td_u32 bind_tbl_size;
    sys_bind_node *node = TD_NULL;
    struct osal_list_head *list_tmp = TD_NULL;
    struct osal_list_head *list_node = TD_NULL;
    sys_bind_src *src_bind = TD_NULL;
    sys_binder_ctx *binder_ctx = TD_NULL;
    unsigned long flags;

    if (mod_id >= OT_ID_BUTT) {
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    bind_spin_lock(flags);

    binder_ctx = (type == SYS_BINDER_SENDER) ? g_sender_tbl[mod_id] : g_receiver_tbl[mod_id];
    if (binder_ctx == TD_NULL) {
        sys_err_trace("mod:%d have not register ! \n", mod_id);
        bind_spin_unlock(flags);
        return OT_ERR_SYS_NOT_PERM;
    }

    bind_tbl_size = binder_ctx->tbl_size;

    if (type == SYS_BINDER_SENDER) {
        for (i = 0; i < bind_tbl_size; i++) {
            src_bind = &binder_ctx->src_tbl[i];

            osal_list_for_each_safe(list_node, list_tmp, &src_bind->list_head) {
                node = osal_list_entry(list_node, sys_bind_node, list);
                osal_list_del(list_node);
                bind_kfree(node);
                node = TD_NULL;
            }
        }
        bind_kfree(binder_ctx->src_tbl);
        binder_ctx->src_tbl = TD_NULL;
    } else if (type == SYS_BINDER_RECEIVER) {
        bind_kfree(binder_ctx->dest_tbl);
        binder_ctx->dest_tbl = TD_NULL;
    }

    bind_kfree(binder_ctx);
    binder_ctx = TD_NULL;
    if (type == SYS_BINDER_SENDER) {
        g_sender_tbl[mod_id] = TD_NULL;
    } else {
        g_receiver_tbl[mod_id] = TD_NULL;
    }

    bind_spin_unlock(flags);

    return TD_SUCCESS;
}

static td_s32 sys_bind_check_chn_validity(const ot_mpp_chn *src_chn, const ot_mpp_chn *dest_chn)
{
    td_s32 ret;

    ret = sys_check_bind_chn(SYS_BINDER_SENDER, src_chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return sys_check_bind_chn(SYS_BINDER_RECEIVER, dest_chn);
}

static td_s32 sys_bind_get_valid_dest_tbl(const ot_mpp_chn *dest_chn, sys_bind_dest **dest_bind)
{
    td_u32 dest_tbl_idx;
    sys_binder_ctx *receiver_ctx = g_receiver_tbl[dest_chn->mod_id];

    dest_tbl_idx = sys_get_idx_by_dev_chn(receiver_ctx, dest_chn->dev_id, dest_chn->chn_id);
    *dest_bind = &receiver_ctx->dest_tbl[dest_tbl_idx];

    if ((*dest_bind)->is_bind_src == TD_TRUE) {
        sys_err_trace("dest have bind src:(%s,%d,%d)!\n", bind_get_mod_name((*dest_bind)->bind_src.mod_id),
                      (*dest_bind)->bind_src.dev_id, (*dest_bind)->bind_src.chn_id);
        return OT_ERR_SYS_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 sys_bind_get_valid_src_tbl(const ot_mpp_chn *src_chn, sys_bind_src **src_bind)
{
    td_u32 src_tbl_idx;
    sys_binder_ctx *sender_ctx = g_sender_tbl[src_chn->mod_id];

    src_tbl_idx = sys_get_idx_by_dev_chn(sender_ctx, src_chn->dev_id, src_chn->chn_id);
    *src_bind = &sender_ctx->src_tbl[src_tbl_idx];

    if ((*src_bind)->dest_num >= OT_MAX_BIND_DST_NUM) {
        sys_err_trace("src bind max(%d)!\n", (*src_bind)->dest_num);
        return OT_ERR_SYS_NOT_PERM;
    }

    return TD_SUCCESS;
}

static td_s32 sys_init_send_bind_src_mem(const ot_mpp_chn *src_chn)
{
    td_u32 src_tbl_idx;
    ot_mpp_bind_dst *send_bind_src = TD_NULL;
    sys_binder_ctx *sender_ctx = g_sender_tbl[src_chn->mod_id];

    src_tbl_idx = sys_get_idx_by_dev_chn(sender_ctx, src_chn->dev_id, src_chn->chn_id);

    send_bind_src = g_bind_send[src_chn->mod_id].ppst_send_bind_src[src_tbl_idx];
    if (send_bind_src == TD_NULL) {
        send_bind_src = (ot_mpp_bind_dst *)bind_kmalloc(sizeof(ot_mpp_bind_dst));
        if (send_bind_src == TD_NULL) {
            sys_err_trace("no memory for bind SRC!\n");
            return OT_ERR_SYS_NO_MEM;
        }

        (td_void)memset_s(send_bind_src, sizeof(ot_mpp_bind_dst), 0, sizeof(ot_mpp_bind_dst));
        g_bind_send[src_chn->mod_id].ppst_send_bind_src[src_tbl_idx] = send_bind_src;
    }
    return TD_SUCCESS;
}

static inline td_void sys_deinit_send_bind_src_mem(const ot_mpp_chn *src_chn)
{
    td_u32 src_tbl_idx;
    sys_binder_ctx *sender_ctx = g_sender_tbl[src_chn->mod_id];

    src_tbl_idx = sys_get_idx_by_dev_chn(sender_ctx, src_chn->dev_id, src_chn->chn_id);

    bind_kfree(g_bind_send[src_chn->mod_id].ppst_send_bind_src[src_tbl_idx]);
    g_bind_send[src_chn->mod_id].ppst_send_bind_src[src_tbl_idx] = TD_NULL;
}

static td_s32 sys_really_bind(const ot_mpp_chn *src_chn, const ot_mpp_chn *dest_chn,
                              sys_bind_src *src_bind, sys_bind_dest *dest_bind)
{
    td_s32 ret;
    sys_bind_node *node = TD_NULL;
    unsigned long flags;

    ret = sys_init_send_bind_src_mem(src_chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    node = (sys_bind_node *)bind_kmalloc(sizeof(sys_bind_node));
    if (node == TD_NULL) {
        sys_err_trace("bind_kmalloc err!\n");
        sys_deinit_send_bind_src_mem(src_chn);
        return OT_ERR_SYS_NO_MEM;
    }

    if (memcpy_s(&node->mpp_chn, sizeof(ot_mpp_chn), dest_chn, sizeof(ot_mpp_chn)) != EOK) {
        bind_kfree(node);
        node = TD_NULL;
        sys_deinit_send_bind_src_mem(src_chn);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }
    node->send_cnt = 0;
    node->reset_cnt = 0;

    bind_spin_lock(flags);

    if (memcpy_s(&dest_bind->bind_src, sizeof(ot_mpp_chn), src_chn, sizeof(ot_mpp_chn)) != EOK) {
        bind_spin_unlock(flags);
        bind_kfree(node);
        node = TD_NULL;
        sys_deinit_send_bind_src_mem(src_chn);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    dest_bind->is_bind_src = TD_TRUE;
    dest_bind->send_cnt = 0;
    dest_bind->reset_cnt = 0;

    osal_list_add_tail(&node->list, &src_bind->list_head);
    src_bind->dest_num++;

    bind_spin_unlock(flags);

    return TD_SUCCESS;
}

static td_void sys_give_bind_call_back(const ot_mpp_chn *src_chn)
{
    static ot_mpp_bind_dst bind_src;
    sys_binder_ctx *sender_ctx = g_sender_tbl[src_chn->mod_id];

    if ((src_chn->mod_id == OT_ID_VPSS) || (src_chn->mod_id == OT_ID_VI)) {
        (td_void)memset_s(&bind_src, sizeof(ot_mpp_bind_dst), 0, sizeof(ot_mpp_bind_dst));
        if (sys_get_binder_by_src(src_chn, &bind_src) == TD_SUCCESS) {
            if (sender_ctx->give_bind_call_back != TD_NULL) {
                sender_ctx->give_bind_call_back(src_chn->dev_id, src_chn->chn_id, &bind_src);
            }
        }
    }
}

td_s32 sys_bind(const ot_mpp_chn *src_chn, const ot_mpp_chn *dest_chn)
{
    td_s32 ret;
    sys_bind_src *src_bind = TD_NULL;
    sys_bind_dest *dest_bind = TD_NULL;

    sys_check_null_ptr_return(src_chn);
    sys_check_null_ptr_return(dest_chn);

    ret = sys_bind_check_chn_validity(src_chn, dest_chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    bind_sem_down_return();

    ret = sys_bind_get_valid_dest_tbl(dest_chn, &dest_bind);
    if (ret != TD_SUCCESS) {
        goto fail1;
    }

    ret = sys_bind_get_valid_src_tbl(src_chn, &src_bind);
    if (ret != TD_SUCCESS) {
        goto fail1;
    }

    ret = sys_really_bind(src_chn, dest_chn, src_bind, dest_bind);
    if (ret != TD_SUCCESS) {
        goto fail1;
    }

    sys_give_bind_call_back(src_chn);

    bind_sem_up();
    return TD_SUCCESS;

fail1:
    bind_sem_up();
    return ret;
}

static td_s32 sys_really_unbind(const ot_mpp_chn *src_chn, const ot_mpp_chn *dest_chn, sys_bind_dest *dest_bind)
{
    td_u32 src_tbl_idx;
    ot_mod_id src_mod_id;
    td_s32 src_dev_id;
    td_s32 src_chn_id;
    unsigned long flags;
    sys_binder_ctx *sender_ctx = TD_NULL;
    sys_bind_node *node = TD_NULL;
    sys_bind_src *src_bind = TD_NULL;
    struct osal_list_head *list_tmp = TD_NULL;
    struct osal_list_head *list_node = TD_NULL;

    src_mod_id = dest_bind->bind_src.mod_id;
    src_dev_id = dest_bind->bind_src.dev_id;
    src_chn_id = dest_bind->bind_src.chn_id;

    if ((src_mod_id != src_chn->mod_id) ||
        (src_dev_id != src_chn->dev_id) ||
        (src_chn_id != src_chn->chn_id)) {
        sys_err_trace("src and dest is not binded!\n");
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    sender_ctx = g_sender_tbl[src_mod_id];
    src_tbl_idx = sys_get_idx_by_dev_chn(sender_ctx, src_dev_id, src_chn_id);
    src_bind = &sender_ctx->src_tbl[src_tbl_idx];

    bind_spin_lock(flags);

    osal_list_for_each_safe(list_node, list_tmp, &src_bind->list_head) {
        node = osal_list_entry(list_node, sys_bind_node, list);
        if ((dest_chn->mod_id == node->mpp_chn.mod_id) &&
            (dest_chn->dev_id == node->mpp_chn.dev_id) &&
            (dest_chn->chn_id == node->mpp_chn.chn_id)) {
            osal_list_del(list_node);
            bind_kfree(node);
            node = TD_NULL;
            ot_assert(src_bind->dest_num > 0);
            src_bind->dest_num--;
            break;
        }
    }

    dest_bind->is_bind_src = TD_FALSE;

    bind_spin_unlock(flags);
    return TD_SUCCESS;
}

static inline td_void sys_bind_reset_callback(const ot_mpp_chn *src_chn, const ot_mpp_chn *dest_chn)
{
    sys_binder_ctx *receiver_ctx = g_receiver_tbl[dest_chn->mod_id];

    if (src_chn->mod_id == OT_ID_VDEC) {
        if (receiver_ctx->reset_call_back != TD_NULL) {
            receiver_ctx->reset_call_back(dest_chn->dev_id, dest_chn->chn_id, TD_NULL);
        }
    }
}

td_s32 sys_unbind(const ot_mpp_chn *src_chn, const ot_mpp_chn *dest_chn)
{
    td_s32 ret;
    td_u32 dest_tbl_idx;
    sys_bind_dest *dest_bind = TD_NULL;
    sys_binder_ctx *receiver_ctx = TD_NULL;

    sys_check_null_ptr_return(src_chn);
    sys_check_null_ptr_return(dest_chn);

    ret = sys_check_bind_chn(SYS_BINDER_RECEIVER, dest_chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = sys_check_bind_chn(SYS_BINDER_SENDER, src_chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    bind_sem_down_return();

    receiver_ctx = g_receiver_tbl[dest_chn->mod_id];

    if (receiver_ctx == TD_NULL) {
        bind_sem_up();
        return TD_SUCCESS;
    }

    dest_tbl_idx = sys_get_idx_by_dev_chn(receiver_ctx, dest_chn->dev_id, dest_chn->chn_id);
    dest_bind = &receiver_ctx->dest_tbl[dest_tbl_idx];

    if (dest_bind->is_bind_src == TD_FALSE) {
        sys_warn_trace("src and dest is not binded!\n");
        bind_sem_up();
        return TD_SUCCESS;
    }

    ret = sys_really_unbind(src_chn, dest_chn, dest_bind);
    if (ret != TD_SUCCESS) {
        bind_sem_up();
        return ret;
    }

    sys_give_bind_call_back(&dest_bind->bind_src);
    sys_bind_reset_callback(src_chn, dest_chn);

    bind_sem_up();
    return TD_SUCCESS;
}

td_s32 sys_get_bind_by_dest_inner(const ot_mpp_chn *dest_chn, ot_mpp_chn *src_chn)
{
    return sys_get_bind_by_dest(dest_chn, src_chn, TD_TRUE);
}

td_s32 sys_get_bind_by_dest(const ot_mpp_chn *dest_chn, ot_mpp_chn *src_chn, td_bool inner_call)
{
    td_s32 ret;
    td_u32 dest_tbl_idx;
    sys_bind_dest *dest_bind = TD_NULL;
    sys_binder_ctx *receiver_ctx = TD_NULL;
    unsigned long flags;

    sys_check_null_ptr_return(src_chn);
    sys_check_null_ptr_return(dest_chn);

    bind_spin_lock(flags);

    ret = sys_check_bind_chn(SYS_BINDER_RECEIVER, dest_chn);
    if (ret != TD_SUCCESS) {
        bind_spin_unlock(flags);
        return ret;
    }

    receiver_ctx = g_receiver_tbl[dest_chn->mod_id];
    dest_tbl_idx = sys_get_idx_by_dev_chn(receiver_ctx, dest_chn->dev_id, dest_chn->chn_id);
    dest_bind = &receiver_ctx->dest_tbl[dest_tbl_idx];

    if (dest_bind->is_bind_src == TD_FALSE) {
        if (inner_call) {
            sys_warn_trace("dest have not bind any src \n");
        } else {
            sys_err_trace("dest have not bind any src \n");
        }

        bind_spin_unlock(flags);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    if (memcpy_s(src_chn, sizeof(ot_mpp_chn), &dest_bind->bind_src, sizeof(ot_mpp_chn)) != EOK) {
        bind_spin_unlock(flags);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    bind_spin_unlock(flags);

    return TD_SUCCESS;
}

td_s32 sys_get_bind_by_src(const ot_mpp_chn *src_chn, ot_mpp_bind_dst *bind_dest)
{
    td_s32 ret;

    bind_sem_down_return();
    ret = sys_get_binder_by_src(src_chn, bind_dest);

    bind_sem_up();

    return ret;
}

td_s32 sys_get_bind_by_src_with_lock(const ot_mpp_chn *src_chn, ot_mpp_bind_dst *bind_dest)
{
    td_s32 ret;
    unsigned long flags;

    bind_spin_lock(flags);
    ret = sys_get_binder_by_src(src_chn, bind_dest);

    bind_spin_unlock(flags);

    return ret;
}

td_s32 sys_bind_register_sender(const bind_sender_info *info)
{
    return sys_register_binder(SYS_BINDER_SENDER, info, TD_NULL);
}

td_s32 sys_bind_register_receiver(const bind_receiver_info *info)
{
    return sys_register_binder(SYS_BINDER_RECEIVER, TD_NULL, info);
}

td_s32 sys_bind_unregister_sender(ot_mod_id mod_id)
{
    return sys_unregister_binder(SYS_BINDER_SENDER, mod_id);
}

td_s32 sys_bind_unregister_receiver(ot_mod_id mod_id)
{
    return sys_unregister_binder(SYS_BINDER_RECEIVER, mod_id);
}

static td_s32 sys_bind_really_send_data(const ot_mpp_bind_dst *send_bind_src, td_u32 flag,
                                        mpp_data_type data_type, td_void *v_data)
{
    td_u32 i;
    td_s32 ret = TD_SUCCESS;
    td_s32 tmp_ret;
    td_u32 dest_tbl_idx;
    sys_binder_ctx *receiver = TD_NULL;
    sys_bind_dest *dest_bind = TD_NULL;
    unsigned long flags;

    if ((flag & SYS_SEND_DATA_BLOCK_MASK) == SYS_SEND_DATA_NOBLOCK) {
        ret = TD_FAILURE;
    }

    for (i = 0; i < send_bind_src->num; i++) {
        bind_spin_lock(flags);

        if (send_bind_src->mpp_chn[i].mod_id >= OT_ID_BUTT) {
            bind_spin_unlock(flags);
            continue;
        }

        receiver = g_receiver_tbl[send_bind_src->mpp_chn[i].mod_id];
        if (receiver == TD_NULL) {
            sys_warn_trace("mod %d have not register!\n", send_bind_src->mpp_chn[i].mod_id);
            bind_spin_unlock(flags);
            continue;
        }

        if (receiver->call_back == TD_NULL) {
            bind_spin_unlock(flags);
            continue;
        }

        if ((((flag & SYS_SEND_DATA_DELAY_MASK) == SYS_SEND_DATA_LOWDELAY) && (!receiver->support_delay_data)) ||
            (((flag & SYS_SEND_DATA_DELAY_MASK) == SYS_SEND_DATA_LOWDELAY_FINISH) && (receiver->support_delay_data))) {
            bind_spin_unlock(flags);
            continue;
        }

        dest_tbl_idx = sys_get_idx_by_dev_chn(receiver, send_bind_src->mpp_chn[i].dev_id,
                                              send_bind_src->mpp_chn[i].chn_id);
        dest_bind = &receiver->dest_tbl[dest_tbl_idx];
        dest_bind->send_cnt++;

        bind_spin_unlock(flags);
        tmp_ret = receiver->call_back(send_bind_src->mpp_chn[i].dev_id, send_bind_src->mpp_chn[i].chn_id,
                                      (flag & SYS_SEND_DATA_BLOCK_MASK), data_type, v_data);
        if ((flag & SYS_SEND_DATA_BLOCK_MASK) == SYS_SEND_DATA_NOBLOCK) {
            ret = (td_u32)ret & (td_u32)tmp_ret;
        } else {
            ret = (td_u32)ret | (td_u32)tmp_ret;
        }
    }

    return ret;
}

td_s32 sys_bind_send_data(ot_mpp_chn *bind_chn, td_u32 flag, mpp_data_type data_type, td_void *v_data)
{
    td_s32 ret;
    td_u32 src_tbl_idx;
    sys_binder_ctx *sender = TD_NULL;
    ot_mpp_bind_dst *send_bind_src = TD_NULL;
    unsigned long flags;

    sys_check_null_ptr_return(bind_chn);
    sys_check_null_ptr_return(v_data);
    if ((bind_chn->mod_id >= OT_ID_BUTT) || (data_type >= MPP_DATA_BUTT)) {
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    bind_adjust_src_dev_id(bind_chn->mod_id, bind_chn->dev_id);
    bind_adjust_src_chn_id(bind_chn->mod_id, bind_chn->chn_id);

    ret = sys_check_bind_chn(SYS_BINDER_SENDER, bind_chn);
    if (ret != TD_SUCCESS) {
        return OT_ERR_SYS_NOT_PERM;
    }

    bind_spin_lock(flags);

    sender = g_sender_tbl[bind_chn->mod_id];
    if (sender == TD_NULL) {
        sys_err_trace("mod %d have not register!\n", bind_chn->mod_id);
        bind_spin_unlock(flags);
        return OT_ERR_SYS_NOT_PERM;
    }

    src_tbl_idx = sys_get_idx_by_dev_chn(g_sender_tbl[bind_chn->mod_id], bind_chn->dev_id, bind_chn->chn_id);

    send_bind_src = g_bind_send[bind_chn->mod_id].ppst_send_bind_src[src_tbl_idx];
    if (send_bind_src == TD_NULL) {
        sys_info_trace("mod %d, dev %d, chn %d, have not bind!\n",
            bind_chn->mod_id, bind_chn->dev_id, bind_chn->chn_id);
        bind_spin_unlock(flags);
        return OT_ERR_SYS_NOT_PERM;
    }

    send_bind_src->num = 0;
    ret = sys_get_binder_by_src(bind_chn, send_bind_src);
    ot_assert(ret == TD_SUCCESS);

    bind_spin_unlock(flags);

    if (send_bind_src->num == 0) {
        sys_warn_trace("have not binder!\n");
        return OT_ERR_SYS_NOT_PERM;
    }

    return sys_bind_really_send_data(send_bind_src, flag, data_type, v_data);
}

#define sys_bind_reset_data_lock(flags) \
    do {                                \
        if (osal_in_interrupt()) {      \
            bind_spin_lock(flags);      \
        } else {                        \
            bind_sem_down_return();     \
        }                               \
    } while (0)

#define sys_bind_reset_data_unlock(flags) \
    do {                                  \
        if (osal_in_interrupt()) {        \
            bind_spin_unlock(flags);      \
        } else {                          \
            bind_sem_up();                \
        }                                 \
    } while (0)

static td_s32 sys_bind_really_reset_data(const ot_mpp_bind_dst *send_bind_src, td_void *private)
{
    td_u32 i;
    td_u32 dest_tbl_idx;
    sys_binder_ctx *receiver = TD_NULL;
    sys_bind_dest *dest_bind = TD_NULL;
    unsigned long flags = 0;

    for (i = 0; i < send_bind_src->num; i++) {
        sys_bind_reset_data_lock(flags);

        if (send_bind_src->mpp_chn[i].mod_id >= OT_ID_BUTT) {
            sys_bind_reset_data_unlock(flags);
            continue;
        }

        receiver = g_receiver_tbl[send_bind_src->mpp_chn[i].mod_id];
        if (receiver == TD_NULL) {
            sys_warn_trace("mod %d have not register!\n", send_bind_src->mpp_chn[i].mod_id);
            sys_bind_reset_data_unlock(flags);
            continue;
        }

        if (receiver->reset_call_back == TD_NULL) {
            sys_bind_reset_data_unlock(flags);
            continue;
        }

        dest_tbl_idx = sys_get_idx_by_dev_chn(receiver, send_bind_src->mpp_chn[i].dev_id,
                                              send_bind_src->mpp_chn[i].chn_id);
        dest_bind = &receiver->dest_tbl[dest_tbl_idx];
        dest_bind->reset_cnt++;

        sys_bind_reset_data_unlock(flags);

        receiver->reset_call_back(send_bind_src->mpp_chn[i].dev_id, send_bind_src->mpp_chn[i].chn_id, private);
    }

    return TD_SUCCESS;
}

td_s32 sys_bind_reset_data(ot_mod_id mod_id, td_s32 dev_id, td_s32 chn_id, td_void *private)
{
    td_s32 ret;
    td_u32 src_tbl_idx;
    ot_mpp_chn bind_chn;
    sys_binder_ctx *sender = TD_NULL;
    ot_mpp_bind_dst *send_bind_src = TD_NULL;
    unsigned long flags = 0;

    ot_assert(mod_id < OT_ID_BUTT);
    sys_check_null_ptr_return(private);

    bind_adjust_src_dev_id(mod_id, dev_id);
    bind_adjust_src_chn_id(mod_id, chn_id);

    bind_chn.mod_id = mod_id;
    bind_chn.dev_id = dev_id;
    bind_chn.chn_id = chn_id;
    ret = sys_check_bind_chn(SYS_BINDER_SENDER, &bind_chn);
    if (ret != TD_SUCCESS) {
        return OT_ERR_SYS_NOT_PERM;
    }

    sys_bind_reset_data_lock(flags);

    sender = g_sender_tbl[mod_id];
    if (sender == TD_NULL) {
        sys_err_trace("mod %d have not register!\n", mod_id);
        sys_bind_reset_data_unlock(flags);
        return OT_ERR_SYS_NOT_PERM;
    }

    src_tbl_idx = sys_get_idx_by_dev_chn(g_sender_tbl[mod_id], dev_id, chn_id);

    send_bind_src = g_bind_send[mod_id].ppst_send_bind_src[src_tbl_idx];
    if (send_bind_src == TD_NULL) {
        sys_info_trace("mod %d, dev %d, chn %d, have not bind!\n", mod_id, dev_id, chn_id);
        sys_bind_reset_data_unlock(flags);
        return TD_SUCCESS;
    }

    send_bind_src->num = 0;
    ret = sys_get_binder_by_src(&bind_chn, send_bind_src);
    ot_assert(ret == TD_SUCCESS);

    sys_bind_reset_data_unlock(flags);

    if (send_bind_src->num == 0) {
        sys_warn_trace("have not binder!\n");
        return TD_SUCCESS;
    }

    return sys_bind_really_reset_data(send_bind_src, private);
}

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT

static td_void sys_proc_show_sec_bindship(osal_proc_entry_t *s, const sys_bind_node *node,
    const sys_binder_ctx *sec_sender, const sys_bind_dest *dest_bind, const ot_mpp_chn *fir_mpp_chn)
{
    td_u32 k;
    td_s32 sec_index;
    td_s32 sec_src_chn;
    struct osal_list_head *list_tmp2 = TD_NULL;
    struct osal_list_head *list_node2 = TD_NULL;
    sys_bind_src *sec_bind_src = TD_NULL;
    sys_bind_node *node2 = TD_NULL;
    td_bool print_sec = TD_FALSE;

    for (k = 0; k < sec_sender->max_chn_cnt; k++) {
        sec_src_chn = k;
        sec_index = k + sec_sender->max_chn_cnt * node->mpp_chn.dev_id;
        sec_bind_src = &sec_sender->src_tbl[sec_index];

        bind_adjust_dest_chn_id(node->mpp_chn.mod_id, sec_src_chn);
        if ((node->mpp_chn.chn_id == sec_src_chn) &&
            (!osal_list_empty(&sec_bind_src->list_head))) {
            osal_list_for_each_safe(list_node2, list_tmp2, &sec_bind_src->list_head) {
                node2 = osal_list_entry(list_node2, sys_bind_node, list);
                osal_seq_printf(s,
                    "%9s" "%9d" "%9d" "%9s" "%9d" "%9u" "%9s" "%9d" "%9d" "%12d" "%12u" "\n",
                    bind_get_mod_name(fir_mpp_chn->mod_id), fir_mpp_chn->dev_id, fir_mpp_chn->chn_id,
                    bind_get_mod_name(node->mpp_chn.mod_id), node->mpp_chn.dev_id, k,
                    bind_get_mod_name(node2->mpp_chn.mod_id), node2->mpp_chn.dev_id,
                    node2->mpp_chn.chn_id, dest_bind->send_cnt, dest_bind->reset_cnt);
            }
            print_sec = TD_TRUE;
        }
    }

    if (print_sec == TD_FALSE) {
        osal_seq_printf(s, "%9s" "%9d" "%9d" "%9s" "%9d" "%9d" "%9s" "%9d" "%9d" "%12d" "%12u" "\n",
            bind_get_mod_name(fir_mpp_chn->mod_id), fir_mpp_chn->dev_id, fir_mpp_chn->chn_id,
            bind_get_mod_name(node->mpp_chn.mod_id),
            node->mpp_chn.dev_id, node->mpp_chn.chn_id, "null", 0, 0,
            dest_bind->send_cnt, dest_bind->reset_cnt);
    }
}

static td_void sys_bindship_proc_show_node(osal_proc_entry_t *s, const ot_mpp_chn *fir_mpp_chn,
    const sys_bind_node *node)
{
    td_u32 dest_tbl_idx;
    sys_binder_ctx *sec_sender = TD_NULL;
    sys_binder_ctx *receiver = TD_NULL;
    sys_bind_dest *dest_bind = TD_NULL;

    if (node->mpp_chn.mod_id >= OT_ID_BUTT) {
        return;
    }
    receiver = g_receiver_tbl[node->mpp_chn.mod_id];
    if (receiver == TD_NULL) {
        return;
    }
    dest_tbl_idx = sys_get_idx_by_dev_chn(receiver, node->mpp_chn.dev_id, node->mpp_chn.chn_id);
    dest_bind = &receiver->dest_tbl[dest_tbl_idx];

    sec_sender = g_sender_tbl[node->mpp_chn.mod_id];
    if ((sec_sender == TD_NULL) || (node->mpp_chn.dev_id >= sec_sender->max_dev_cnt) ||
        (node->mpp_chn.chn_id >= sec_sender->max_chn_cnt)) {
        osal_seq_printf(s,
            "%9s" "%9d" "%9d" "%9s" "%9d" "%9d" "%9s" "%9d" "%9d" "%12d" "%12u" "\n",
            bind_get_mod_name(fir_mpp_chn->mod_id), fir_mpp_chn->dev_id, fir_mpp_chn->chn_id,
            bind_get_mod_name(node->mpp_chn.mod_id), node->mpp_chn.dev_id, node->mpp_chn.chn_id,
            "null", 0, 0, dest_bind->send_cnt, dest_bind->reset_cnt);
        return;
    }
    sys_proc_show_sec_bindship(s, node, sec_sender, dest_bind, fir_mpp_chn);
}

td_void sys_bindship_proc_show(osal_proc_entry_t *s)
{
    td_u32 i;
    td_u32 j;
    ot_mpp_chn fir_mpp_chn;
    sys_binder_ctx *fir_sender = TD_NULL;
    sys_bind_src *fir_bind_src = TD_NULL;
    struct osal_list_head *list_tmp = TD_NULL;
    struct osal_list_head *list_node = TD_NULL;
    sys_bind_node *node = TD_NULL;

    for (i = 0; i < OT_ID_BUTT; i++) {
        fir_sender = g_sender_tbl[i];
        if (fir_sender == TD_NULL) {
            continue;
        }
        fir_mpp_chn.mod_id = i;
        for (j = 0; j < fir_sender->tbl_size; j++) {
            fir_bind_src = &fir_sender->src_tbl[j];

            sys_get_dev_chn_by_idx(fir_sender, j, &fir_mpp_chn.dev_id, &fir_mpp_chn.chn_id);

            osal_list_for_each_safe(list_node, list_tmp, &fir_bind_src->list_head) {
                node = osal_list_entry(list_node, sys_bind_node, list);
                sys_bindship_proc_show_node(s, &fir_mpp_chn, node);
            }
        }
    }
}

td_void sys_bind_proc_show(osal_proc_entry_t *s)
{
    unsigned long flags;

    call_sys_print_proc_title(s, "bind relation table");
    osal_seq_printf(s, "%9s" "%9s" "%9s" "%9s" "%9s" "%9s" "%9s" "%9s" "%9s" "%12s" "%12s" "\n",
        "fir_mod", "fir_dev", "fir_chn", "sec_mod", "sec_dev", "sec_chn", "tir_mod", "tir_dev",
        "tir_chn", "send_cnt", "rst_cnt");

    bind_spin_lock(flags);
    sys_bindship_proc_show(s);
    bind_spin_unlock(flags);
}
#endif

td_s32 sys_bind_mod_init(td_void)
{
    if (bind_spin_lock_init() < 0) {
        return TD_FAILURE;
    }

    if (bind_sem_init() < 0) {
        bind_spin_lock_uninit();
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sys_bind_init(td_void)
{
    (td_void)memset_s(g_sender_tbl, sizeof(g_sender_tbl), 0, sizeof(g_sender_tbl));
    (td_void)memset_s(g_receiver_tbl, sizeof(g_receiver_tbl), 0, sizeof(g_receiver_tbl));
    (td_void)memset_s(g_bind_send, sizeof(g_bind_send), 0, sizeof(g_bind_send));
    return TD_SUCCESS;
}

td_void sys_bind_mod_exit(td_void)
{
    bind_spin_lock_uninit();
    bind_sem_uninit();
}

td_void sys_bind_exit(td_void)
{
    td_u32 i;
    td_u32 j;

    for (i = 0; i < OT_ID_BUTT; i++) {
        if (g_bind_send[i].ppst_send_bind_src == TD_NULL) {
            continue;
        }
        for (j = 0; j < g_bind_send[i].max_index; j++) {
            if (g_bind_send[i].ppst_send_bind_src[j] != TD_NULL) {
                bind_kfree(g_bind_send[i].ppst_send_bind_src[j]);
                g_bind_send[i].ppst_send_bind_src[j] = TD_NULL;
            }
        }

        bind_kfree(g_bind_send[i].ppst_send_bind_src);
        g_bind_send[i].ppst_send_bind_src = TD_NULL;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
