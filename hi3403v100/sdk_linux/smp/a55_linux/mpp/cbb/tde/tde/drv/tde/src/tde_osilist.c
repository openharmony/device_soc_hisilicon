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

#include "tde_handle.h"
#include "tde_buffer.h"
#include "tde_osilist.h"
#include "tde_hal.h"
#include "wmalloc.h"
#include "tde_check_para.h"

/* JOB LIST head node definition */
typedef struct {
    struct osal_list_head list_head;
    td_s32 handle_last;     /* job handle wait for last submit */
    td_s32 handle_finished; /* job handle last completed */
    td_u32 job_num;         /* job number in queue */
    osal_spinlock_t lock;
    tde_sw_job *pst_job_committed; /* last submitted job node pointer, which is the first job */
    tde_sw_job *pst_job_to_commit;  /* job node pointer wait for submit,which is the first job */
    td_phys_addr_t job_finished;
    tde_sw_job *pst_job_last; /* last job in the list */
} tde_swjoblist;

typedef td_void (*TDE_WQ_CB)(td_u32);
typedef struct {
    td_u32 count;
    TDE_WQ_CB p_wqcb;
    struct osal_work_struct work;
} tdefreewq;

static osal_wait_t g_tde_block_job_wq; /* wait queue used to block */
static tde_swjoblist *g_tde_osi_job_list; /* global job list queue */
static td_void tde_osi_list_do_free_phy_buff(td_u32 buf_num);

static inline td_void tde_osi_list_safe_destroy_job(tde_sw_job *pst_job);
static td_void tde_osi_list_release_handle(tde_handle_mgr *pst_job_header);
static inline td_void tde_osi_list_add_job(tde_sw_job *pst_job);
static td_void tde_osi_list_destroy_job(tde_sw_job *pst_job);

static td_void tde_osi_list_safe_destroy_job(tde_sw_job *pst_job)
{
    tde_check_nullpointer_return_novalue(pst_job);

    /* if user query this job, release job in query function */
    if (pst_job->wait_for_done_count != 0) {
        tde_info("query handle %d complete!\n", pst_job->handle);
        pst_job->notify_type = TDE_JOB_NOTIFY_BUTT;
        osal_wakeup(&pst_job->query);
    } else {
        tde_osi_list_destroy_job(pst_job);
    }

    return;
}

static td_void tde_osi_list_release_handle(tde_handle_mgr *job_header)
{
    if (job_header != TD_NULL) {
        if (release_handle(job_header->handle)) {
            tde_free(job_header);
        }
    }

    return;
}

static td_void tde_osi_list_do_free_phy_buff(td_u32 buf_num)
{
    td_u32 i;

    for (i = 0; i < buf_num; i++) {
        tde_free_physic_buffer();
    }
    return;
}

td_s32 tde_osi_list_init(td_void)
{
    if (osal_wait_init(&g_tde_block_job_wq) != 0) {
        return TD_FAILURE;
    }

    if (!initial_handle()) {
        osal_wait_destroy(&g_tde_block_job_wq);
        return TD_FAILURE;
    }

    g_tde_osi_job_list = (tde_swjoblist *)tde_malloc(sizeof(tde_swjoblist));
    if (g_tde_osi_job_list == TD_NULL) {
        osal_wait_destroy(&g_tde_block_job_wq);
        destroy_handle();
        return TD_FAILURE;
    }

    OSAL_INIT_LIST_HEAD(&g_tde_osi_job_list->list_head);
    osal_spin_lock_init(&g_tde_osi_job_list->lock);
    osal_spin_lock_init(&g_tde_buf_lock);

    return TD_SUCCESS;
}

static td_void tde_osi_list_free_node(tde_sw_node *node)
{
    if (node == TD_NULL) {
        tde_error("Node is null !\n");
        return;
    }
    osal_list_del_init(&node->list_head);
    if (node->node_buf.buf != TD_NULL) {
        tde_free(node->node_buf.buf);
    }

    tde_osi_list_put_phy_buff(node->phy_buf_num);
    tde_free(node);
}

/*
 * Function:      tde_osi_list_free_serial_cmd
 * Description:   release from FstCmd to LastCmd
 * Input:         fst_cmd: first node
 *                last_cmd:last node
 */
static td_void tde_osi_list_free_serial_cmd(tde_sw_node *fst_cmd, tde_sw_node *last_cmd)
{
    tde_sw_node *next_cmd = TD_NULL;
    tde_sw_node *cur_cmd = TD_NULL;

    if ((fst_cmd == TD_NULL) || (last_cmd == TD_NULL)) {
        return;
    }

    cur_cmd = next_cmd = fst_cmd;
    while (next_cmd != last_cmd) {
        next_cmd = osal_list_entry(cur_cmd->list_head.next, tde_sw_node, list_head);
        if (next_cmd == TD_NULL) {
            return;
        }
        tde_osi_list_free_node(cur_cmd);
        cur_cmd = next_cmd;
    }

    tde_osi_list_free_node(last_cmd);
    return;
}

td_void tde_osi_list_term(td_void)
{
    tde_sw_job *pst_job = TD_NULL;

    osal_wait_destroy(&g_tde_block_job_wq);
    osal_spin_lock_destroy(&g_tde_buf_lock);
    if (g_tde_osi_job_list == TD_NULL) {
        return;
    }

    while (!osal_list_empty(&g_tde_osi_job_list->list_head)) {
        pst_job = osal_list_entry(g_tde_osi_job_list->list_head.next, tde_sw_job, list_head);
        osal_list_del_init(&pst_job->list_head);
        tde_osi_list_destroy_job(pst_job);
    }
    osal_spin_lock_destroy(&g_tde_osi_job_list->lock);

    tde_free(g_tde_osi_job_list);
    g_tde_osi_job_list = TD_NULL;
    destroy_handle();
    return;
}

#ifdef TDE_CACH_STRATEGY
static td_void tde_osi_list_flush_node(const tde_sw_node *hw_node)
{
    tde_node_buf *p_node_buf_info = TD_NULL;
    td_u32 cur_node_phy;

    if (hw_node == TD_NULL) {
        tde_error("Node is null !\n");
        return;
    }

    p_node_buf_info = &hw_node->node_buf;

    if (p_node_buf_info->vir_buf != TD_NULL) {
        cur_node_phy = wgetphy(p_node_buf_info->vir_buf);
        tde_flush_cached_mmb((td_u8 *)p_node_buf_info->vir_buf, cur_node_phy,
                             p_node_buf_info->node_size + TDE_NODE_HEAD_BYTE + TDE_NODE_TAIL_BYTE);
    }

    return;
}

static td_void tde_osi_list_flush_job(tde_sw_job *pst_job)
{
    tde_sw_node *pst_next_cmd = TD_NULL;
    tde_sw_node *pst_cur_cmd = TD_NULL;
    tde_sw_node *first_cmd = TD_NULL;
    tde_sw_node *last_cmd = TD_NULL;

    if (pst_job == TD_NULL) {
        tde_error("pstJob is null !\n");
        return;
    }

    first_cmd = pst_job->first_cmd;
    last_cmd = pst_job->tail_node;

    if ((first_cmd == TD_NULL) || (last_cmd == TD_NULL)) {
        return;
    }

    pst_cur_cmd = pst_next_cmd = first_cmd;
    while (pst_next_cmd != last_cmd) {
        pst_next_cmd = osal_list_entry(pst_cur_cmd->list_head.next, tde_sw_node, list_head);
        tde_osi_list_flush_node(pst_cur_cmd);
        pst_cur_cmd = pst_next_cmd;
    }

    tde_osi_list_flush_node(last_cmd);
    return;
}
#endif

static inline td_void tde_osi_list_add_job(tde_sw_job *job)
{
    osal_list_add_tail(&job->list_head, &g_tde_osi_job_list->list_head);
    g_tde_osi_job_list->job_num++;
    g_tde_osi_job_list->handle_last = job->handle;
    g_tde_osi_job_list->pst_job_last = job;
}

td_s32 tde_osi_list_begin_job(td_s32 *handle, td_void *private_data)
{
    tde_handle_mgr *handle_mgr = TD_NULL;
    tde_sw_job *job = TD_NULL;
    td_s32 ret;

    if (handle == TD_NULL) {
        return TD_FAILURE;
    }
    handle_mgr = (tde_handle_mgr *)tde_malloc(sizeof(tde_handle_mgr) + sizeof(tde_sw_job));
    if (handle_mgr == TD_NULL) {
        tde_error("tde begin job malloc fail!\n");
        return DRV_ERR_TDE_NO_MEM;
    }
    job = (tde_sw_job *)((td_u8 *)handle_mgr + sizeof(tde_handle_mgr));
    handle_mgr->res = (td_void *)job;
    get_handle(handle_mgr, handle);
    if (*handle < 0) {
        tde_free(handle_mgr);
        return DRV_ERR_TDE_NO_MEM;
    }

    OSAL_INIT_LIST_HEAD(&job->list_head);

    ret = osal_wait_init(&job->query);
    if (ret != 0) {
        tde_error("osal_wait_init Fail!\n");
        tde_osi_list_release_handle(handle_mgr);
        return DRV_ERR_TDE_NO_MEM;
    }

    job->handle = *handle;
    if (private_data != TD_NULL) {
        job->private_data = private_data;
    }
    return TD_SUCCESS;
}

static td_void tde_osi_list_destroy_job(tde_sw_job *job)
{
    tde_handle_mgr *handle_mgr = TD_NULL;
#ifdef CONFIG_GFX_MMU_SUPPORT
    tde_hw_node *hw_node = TD_NULL;
#endif

    if (!tde_query_handle(job->handle, &handle_mgr)) {
        tde_info("handle %d does not exist!\n", job->handle);
        return;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    hw_node = (tde_hw_node *)(job->first_cmd->node_buf.vir_buf + TDE_NODE_HEAD_BYTE);
    tde_hal_free_tmp_buf(hw_node);
#endif

    tde_osi_list_free_serial_cmd(job->first_cmd, job->tail_node);

    osal_wait_destroy(&job->query);
    tde_osi_list_release_handle(handle_mgr);
    return;
}

td_s32 tde_osi_list_cancel_job(td_s32 handle)
{
    tde_handle_mgr *handle_mgr = TD_NULL;
    tde_sw_job *job = TD_NULL;
    unsigned long lockflags;

    tde_spin_lock(&g_tde_osi_job_list->lock, lockflags);
    if (!tde_query_handle(handle, &handle_mgr)) {
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        return TD_SUCCESS;
    }
    job = (tde_sw_job *)handle_mgr->res;

    if (job->has_submitted) {
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        tde_error("Handle %d has been submitted!\n", handle);
        return TD_FAILURE;
    }

    tde_osi_list_free_serial_cmd(job->first_cmd, job->tail_node);
    osal_wait_destroy(&job->query);
    tde_osi_list_release_handle(handle_mgr);
    tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);

    return TD_SUCCESS;
}

static td_s32 tde_osi_wait_call_back(const td_void *param)
{
    tde_sw_job *job = TD_NULL;

    if (param == TD_NULL) {
        return TD_FAILURE;
    }
    job = (tde_sw_job *)param;
    return (job->notify_type == TDE_JOB_NOTIFY_BUTT);
}

static td_void tde_job_to_commit(tde_sw_job *job, drv_tde_func_callback func_compl_cb, td_void *func_para,
                                 tde_notify_mode noti_type)
{
    td_u8 *next_node_buf = TD_NULL;

    job->has_submitted = TD_TRUE;
    job->notify_type = noti_type;
    job->func_complete_callback = func_compl_cb;
    job->func_para = func_para;
    /* If the job to commit is not null,join the current job to the tail node of the last job. */
    if (g_tde_osi_job_list->pst_job_to_commit != TD_NULL) {
        tde_sw_node *tail_node_in_job_list = g_tde_osi_job_list->pst_job_last->tail_node;
        next_node_buf = (td_u8 *)tail_node_in_job_list->node_buf.buf + TDE_NODE_HEAD_BYTE;
        tde_hal_next_node_addr((td_void *)next_node_buf, job->first_cmd->node_buf.phy_addr);
        if (job->aq_use_buf) {
            g_tde_osi_job_list->pst_job_to_commit->aq_use_buf = TD_TRUE;
        }
#ifdef TDE_CACH_STRATEGY
        /* Flush all hw will be the job of the corresponding node into memory, ensure correct hardware can access */
        tde_osi_list_flush_job(g_tde_osi_job_list->pst_job_last);
#endif
    } else {
        g_tde_osi_job_list->pst_job_to_commit = job;
    }

#ifdef TDE_CACH_STRATEGY
    /* Flush all hw will be the job of the corresponding node into memory, ensure correct hardware can access */
    tde_osi_list_flush_job(job);
#endif
    tde_osi_list_add_job(job);
    return;
}
static td_s32 tde_list_set_job(tde_sw_job **job, td_s32 handle, td_bool is_sync)
{
    td_bool valid;
    tde_handle_mgr *handle_mgr = TD_NULL;
    tde_sw_node *tail_node = TD_NULL;
    td_u8 *buf = TD_NULL;

    valid = tde_query_handle(handle, &handle_mgr);
    if (!valid) {
        tde_error("invalid handle %d!\n", handle);
        return DRV_ERR_TDE_INVALID_HANDLE;
    }

    *job = (tde_sw_job *)handle_mgr->res;
    if ((*job)->has_submitted) {
        tde_error("job %d already submitted!\n", handle);
        return DRV_ERR_TDE_INVALID_HANDLE;
    }
    if ((*job)->first_cmd == TD_NULL) {
        tde_info("no cmd !\n");
        osal_wait_destroy(&(*job)->query);
        tde_osi_list_release_handle(handle_mgr);
        return -1;
    }
    tail_node = (*job)->tail_node;

    buf = (td_u8 *)tail_node->node_buf.buf + TDE_NODE_HEAD_BYTE;
    tde_hal_node_enable_complete_int((td_void *)buf);

#if (TDE_CAPABILITY & SYNC)
    if (is_sync) {
        tde_hal_node_enable_sync((td_void *)buf);
    }
#else
    ot_unused(is_sync);
#endif
    return TD_SUCCESS;
}

static td_void tde_list_submit_first_cmd(td_void)
{
    td_s32 ret;

    ret = tde_hal_node_execute(g_tde_osi_job_list->pst_job_to_commit->first_cmd->node_buf.phy_addr,
                               g_tde_osi_job_list->pst_job_to_commit->first_cmd->node_buf.update,
                               g_tde_osi_job_list->pst_job_to_commit->aq_use_buf);
    if (ret == TD_SUCCESS) {
        g_tde_osi_job_list->pst_job_committed = g_tde_osi_job_list->pst_job_to_commit;
        g_tde_osi_job_list->pst_job_to_commit = TD_NULL;
        g_tde_osi_job_list->job_finished = 0x0;
    }
    return;
}

static td_void tde_set_job_wake_notify(tde_sw_job *job, tde_notify_mode *noti_type, td_u32 *time_out, td_bool *asynflag)
{
    if (*noti_type != TDE_JOB_WAKE_NOTIFY) {
        if ((!osal_in_interrupt()) && (wgetfreenum() < 5)) { /* 5 num */
            job->notify_type = TDE_JOB_WAKE_NOTIFY;
            *noti_type = TDE_JOB_WAKE_NOTIFY;
            *time_out = 1000; /* timeout 1000ms */
            *asynflag = 1; /* By sign non-blocking way to block the way */
        }
    }

    return;
}

static td_s32 tde_list_wake_up_job(tde_sw_job *job, td_bool asynflag, const td_s32 *ret)
{
    if (job->notify_type == TDE_JOB_NOTIFY_BUTT) {
        tde_info("handle:%d complete!\n", job->handle);
        if (job->wait_for_done_count == 0) {
            tde_osi_list_destroy_job(job);
        }
        return TD_SUCCESS;
    } else {
        job->notify_type = TDE_JOB_COMPL_NOTIFY;
        if (*ret == (-ERESTARTSYS)) {
            return DRV_ERR_TDE_INTERRUPT;
        }
        if (asynflag == 1) {
            /* If from a non-blocking way to block the way, no overtime information */
            return TD_SUCCESS;
        }
        return DRV_ERR_TDE_JOB_TIMEOUT;
    }
}

td_s32 tde_osi_list_submit_job(drv_tde_end_job_cmd *end_job, drv_tde_func_callback func_compl_cb, td_void *func_para,
                               tde_notify_mode noti_type, td_bool is_sync)
{
    tde_sw_job *job = TD_NULL;
    td_s32 wait_ret;
    td_s32 ret;
    td_s32 handle;
    td_u32 time_out;
    unsigned long lockflags;
    td_bool asynflag = 0;

    handle = end_job->handle;
    time_out = end_job->time_out;

    tde_spin_lock(&g_tde_osi_job_list->lock, lockflags);
    ret = tde_list_set_job(&job, handle, is_sync);
    if (ret != TD_SUCCESS) {
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        if (ret == -1) {
            return TD_SUCCESS;
        }
        return ret;
    }
    tde_job_to_commit(job, func_compl_cb, func_para, noti_type);
    tde_set_job_wake_notify(job, &noti_type, &time_out, &asynflag);
    tde_list_submit_first_cmd();

    if (noti_type == TDE_JOB_WAKE_NOTIFY) {
        job->wait_for_done_count++;
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        wait_ret = osal_wait_event_timeout_uninterruptible(&g_tde_block_job_wq, tde_osi_wait_call_back,
            (td_void *)job, time_out);
        tde_spin_lock(&g_tde_osi_job_list->lock, lockflags);
        job->wait_for_done_count--;
        ret = tde_list_wake_up_job(job, asynflag, &wait_ret);
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        if (ret != TD_SUCCESS) {
            return ret;
        }
        return TD_SUCCESS;
    }
    tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
    return TD_SUCCESS;
}

td_s32 tde_osi_list_wait_all_done(td_void)
{
    td_s32 ret;
    td_s32 wait_handle;
    td_ulong lockflags = 0;

    tde_spin_lock(&g_tde_osi_job_list->lock, lockflags);
    wait_handle = g_tde_osi_job_list->handle_last;
    tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
    if (-1 == wait_handle) {
        return TD_SUCCESS;
    }

    ret = tde_osi_list_wait_for_done(wait_handle, 0);

    return ret;
}

static td_s32 tde_osi_list_timeout(tde_sw_job *job, td_s32 wait_ret)
{
    job->wait_for_done_count--;

    if (job->notify_type != TDE_JOB_NOTIFY_BUTT) {
        if (wait_ret == (-ERESTARTSYS)) {
            tde_error("query handle (%d) interrupt!\n", job->handle);
            return DRV_ERR_TDE_INTERRUPT;
        } else {
            tde_error("query handle (%d) time out!\n", job->handle);
        }
        return DRV_ERR_TDE_QUERY_TIMEOUT;
    }

    /* complete */
    if (job->wait_for_done_count == 0) {
        tde_osi_list_destroy_job(job);
    }
    return TD_SUCCESS;
}

static td_s32 tde_osi_list_not_timeout(tde_sw_job *job, td_s32 wait_ret)
{
    job->wait_for_done_count--;
    if (wait_ret == (-ERESTARTSYS)) {
        if ((job->notify_type == TDE_JOB_NOTIFY_BUTT) && (job->wait_for_done_count == 0)) {
            tde_osi_list_destroy_job(job);
        }
        tde_error("query handle (%d) interrupt!\n", job->handle);
        return DRV_ERR_TDE_INTERRUPT;
    }

    if (job->wait_for_done_count == 0) {
        tde_osi_list_destroy_job(job);
    }
    return TD_SUCCESS;
}


/*
 * Function:      tde_osi_list_wait_for_done
 * Description:   block to wait for job done
 * Input:         handle: job handle
 *                time_out: timeout value
 * Return:        =0,successfully completed <0,error
 */
td_s32 tde_osi_list_wait_for_done(td_s32 handle, td_u32 time_out)
{
    tde_sw_job *job = TD_NULL;
    tde_handle_mgr *handle_mgr = TD_NULL;
    td_s32 ret;
    td_s32 wait_ret;
    td_bool valid;
    unsigned long lockflags;

    tde_spin_lock(&g_tde_osi_job_list->lock, lockflags);
    valid = tde_query_handle(handle, &handle_mgr);
    if (!valid) {
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        return TD_SUCCESS;
    }

    job = (tde_sw_job *)handle_mgr->res;
    if (!job->has_submitted) {
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        tde_error("job %d has no submitted!\n", handle);
        return DRV_ERR_TDE_INVALID_HANDLE;
    }
    job->wait_for_done_count++;
    tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
    if (time_out) {
        wait_ret = osal_wait_event_timeout_uninterruptible(&(job->query), tde_osi_wait_call_back, (td_void *)job,
            time_out);
        tde_spin_lock(&g_tde_osi_job_list->lock, lockflags);
        ret = tde_osi_list_timeout(job, wait_ret);
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        return ret;
    }
    wait_ret = osal_wait_event_interruptible(&(job->query), tde_osi_wait_call_back, (td_void *)job);
    tde_spin_lock(&g_tde_osi_job_list->lock, lockflags);
    ret = tde_osi_list_not_timeout(job, wait_ret);
    tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
    return ret;
}

/*
 * Prototype    : tde_osi_list_comp_proc
 * Description  : list complete interrupt servic, mainly complete switch on hardware lists
 * Modification : Created function
 */
static td_void tde_osi_list_comp_proc(td_void)
{
    td_s32 ret;

    if ((g_tde_osi_job_list != TD_NULL)  && (g_tde_osi_job_list->pst_job_to_commit != TD_NULL) &&
        (g_tde_osi_job_list->pst_job_to_commit->first_cmd != TD_NULL)) {
        ret = tde_hal_node_execute(g_tde_osi_job_list->pst_job_to_commit->first_cmd->node_buf.phy_addr,
                                   g_tde_osi_job_list->pst_job_to_commit->first_cmd->node_buf.update,
                                   g_tde_osi_job_list->pst_job_to_commit->aq_use_buf);
        if (ret == TD_SUCCESS) {
            g_tde_osi_job_list->pst_job_committed = g_tde_osi_job_list->pst_job_to_commit;
            g_tde_osi_job_list->pst_job_to_commit = TD_NULL;
            g_tde_osi_job_list->job_finished = 0x0;
        }
    }

    return;
}

static td_s32 tde_list_comp_get_finished_handle(td_s32 *finished_handle, tde_handle_mgr *handle_mgr)
{
    tde_sw_job *job = TD_NULL;
    tde_sw_job *del_job = TD_NULL;

    job = (tde_sw_job *)handle_mgr->res;
    if (job->list_head.prev == &g_tde_osi_job_list->list_head) {
        tde_info("No pre Job left, finishedhandle:%d\n", *finished_handle);
        return TD_FAILURE;
    }

    del_job = osal_list_entry(job->list_head.prev, tde_sw_job, list_head);
    *finished_handle = del_job->handle;
    g_tde_osi_job_list->job_finished = 0x0;
    return TD_SUCCESS;
}

static td_void tde_list_comp_wakeup(tde_sw_job *del_job, td_ulong *lockflags)
{
    tde_info("handle:%d!\n", del_job->handle);
    del_job->notify_type = TDE_JOB_NOTIFY_BUTT;
    if (del_job->func_complete_callback != TD_NULL) {
        tde_info("handle:%d has callback func!\n", del_job->handle);
        osal_spin_unlock_irqrestore(&g_tde_osi_job_list->lock, lockflags);
        del_job->func_complete_callback(del_job->func_para, &(del_job->handle));
        osal_spin_lock_irqsave(&g_tde_osi_job_list->lock, lockflags);
    }

    if (del_job->wait_for_done_count != 0) {
        tde_info("query handle %d complete!\n", del_job->handle);
        osal_wakeup(&del_job->query);
    }

    osal_wakeup(&g_tde_block_job_wq);
}

static td_void tde_list_comp_proc_job(td_void)
{
    if (tde_hal_ctl_is_idle_safely()) {
        tde_hal_set_clock(TD_FALSE);
        tde_osi_list_comp_proc();
    }
}

static td_s32 tde_list_comp_delete_job(td_bool work, td_s32 finished_handle, td_ulong *lockflags)
{
    tde_sw_job *del_job = TD_NULL;
    td_s32 delhandle;

    while (!osal_list_empty(&g_tde_osi_job_list->list_head)) {
        del_job = osal_list_entry(g_tde_osi_job_list->list_head.next, tde_sw_job, list_head);
        delhandle = del_job->handle;
        g_tde_osi_job_list->job_num--;
        if (delhandle == g_tde_osi_job_list->handle_last) {
            g_tde_osi_job_list->handle_last = -1;
        }
        osal_list_del_init(&del_job->list_head);
        if (del_job->notify_type == TDE_JOB_WAKE_NOTIFY) {
            tde_list_comp_wakeup(del_job, lockflags);
        } else if (del_job->notify_type == TDE_JOB_COMPL_NOTIFY) {
            tde_info("handle:%d!\n", del_job->handle);
            if (del_job->func_complete_callback != TD_NULL) {
                tde_info("handle:%d has callback func!\n", del_job->handle);
                osal_spin_unlock_irqrestore(&g_tde_osi_job_list->lock, lockflags);
                del_job->func_complete_callback(del_job->func_para, &(del_job->handle));
                osal_spin_lock_irqsave(&g_tde_osi_job_list->lock, lockflags);
            }
            tde_osi_list_safe_destroy_job(del_job);
        } else {
            tde_error("Error Status!!\n");
        }
        if (delhandle == finished_handle) {
            if (!work) {
                tde_list_comp_proc_job();
            }
            osal_spin_unlock_irqrestore(&g_tde_osi_job_list->lock, lockflags);
            return TD_SUCCESS;
        }
    }
    return TD_FAILURE;
}

/*
 * Function:      tde_osi_list_node_comp
 * Description:   node complete interrupt service, maily complete deleting node and resume suspending,free node
 * Return:        create job handle
 * Modification : Created function
 */
td_void tde_osi_list_node_comp(void)
{
    tde_handle_mgr *handle_mgr = TD_NULL;
    td_s32 finished_handle;
    td_phys_addr_t running_sw_node_addr;
    td_bool work = TD_TRUE;
    td_u32 *finish_handle = TD_NULL;
    unsigned long lockflags;

    tde_spin_lock(&g_tde_osi_job_list->lock, lockflags);
    if (tde_hal_ctl_is_idle_safely()) {
        work = TD_FALSE;
    }
    running_sw_node_addr = tde_hal_cur_node();
    if ((running_sw_node_addr == 0) || (running_sw_node_addr == g_tde_osi_job_list->job_finished)) {
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        return;
    }

    finish_handle = (td_u32 *)wgetvrt(running_sw_node_addr - TDE_NODE_HEAD_BYTE + 8); /* 8 alg data */
    if (finish_handle == TD_NULL) {
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        return;
    }

    finished_handle = *finish_handle;
    if (!tde_query_handle(finished_handle, &handle_mgr)) {
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        return;
    }
    if (!work) {
        g_tde_osi_job_list->job_finished = running_sw_node_addr;
    } else {
        if (tde_list_comp_get_finished_handle(&finished_handle, handle_mgr) < 0) {
            tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
            return;
        }
    }

    if (!tde_query_handle(finished_handle, &handle_mgr)) {
        tde_info("handle %d already delete!\n", finished_handle);
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        return;
    }

    g_tde_osi_job_list->handle_finished = finished_handle;
    if (tde_list_comp_delete_job(work, finished_handle, &lockflags) == TD_SUCCESS) {
        return;
    }
    if (tde_hal_ctl_is_idle_safely()) {
        tde_hal_set_clock(TD_FALSE);
    }
    tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
}

td_phys_addr_t tde_osi_list_get_phy_buff(td_u32 cbcr_offset)
{
    td_phys_addr_t ret;
    if (osal_in_interrupt()) {
        return 0;
    }
    ret = tde_alloc_physic_buffer(cbcr_offset);

    return ret;
}

static td_void tde_osi_list_freevmem(struct osal_work_struct *work)
{
    tdefreewq *p_w_queue_info = TD_NULL;

    p_w_queue_info = osal_container_of(work, tdefreewq, work);
    if (p_w_queue_info != TD_NULL) {
    p_w_queue_info->p_wqcb(p_w_queue_info->count);
    tde_free(p_w_queue_info);
    }

    return;
}

static td_void tde_osi_list_hsr(td_void *pst_func, td_u32 data)
{
    tdefreewq *pst_wq = TD_NULL;

    pst_wq = tde_malloc(sizeof(tdefreewq));
    if (pst_wq == TD_NULL) {
        tde_error("Malloc tdefreewq failed!\n");
        return;
    }

    pst_wq->count = data;
    pst_wq->p_wqcb = (TDE_WQ_CB)pst_func;

    OSAL_INIT_WORK(&pst_wq->work, tde_osi_list_freevmem);
    osal_schedule_work(&pst_wq->work);
    return;
}

td_void tde_osi_list_put_phy_buff(td_u32 buf_num)
{
    if (buf_num != 0) {
        tde_osi_list_hsr((td_void *)tde_osi_list_do_free_phy_buff, buf_num);
    }
    return;
}

td_void tde_list_free_pending_job(td_void *private_data)
{
    tde_handle_mgr *handle = TD_NULL;
    tde_handle_mgr *save = TD_NULL;
    tde_sw_job *job = TD_NULL;
    unsigned long lockflags;
    td_ulong list_lock_flags = 0;
    tde_handle_mgr *tde_handle_list = tde_get_handle_list();

    if (tde_handle_list == TD_NULL) {
        return;
    }
    tde_spin_lock(&g_tde_osi_job_list->lock, lockflags);
    tde_lock_handle_list(&list_lock_flags);
    handle = osal_list_entry(tde_handle_list->list_head.next, tde_handle_mgr, list_head);
    if (handle == TD_NULL) {
        tde_unlock_handle_list(&list_lock_flags);
        tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
        tde_info("No pending job!!\n");
        return;
    }
    while (handle != tde_handle_list) {
        save = handle;
        job = (tde_sw_job *)save->res;
        /* debug */
        if (job == TD_NULL) {
            tde_unlock_handle_list(&list_lock_flags);
            tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
            tde_error("ERR:pstJob Null Pointer!!!\n");
            return;
        }

        /* END */
        handle = osal_list_entry(handle->list_head.next, tde_handle_mgr, list_head);
        /* free when it is not submitted */
#ifndef __RTOS__
        if ((!job->has_submitted) && (private_data == job->private_data)) {
#else
        if (!job->has_submitted) {
#endif
            /* free handle resource */
            tde_osi_list_free_serial_cmd(job->first_cmd, job->tail_node);
            osal_wait_destroy(&job->query);
            osal_list_del_init(&save->list_head);
            tde_free(save);
        }

        if (handle == TD_NULL) {
            tde_unlock_handle_list(&list_lock_flags);
            tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
            tde_error("ERR: pstHandle Null Pointer!!\n");
            return;
        }
    }
    tde_unlock_handle_list(&list_lock_flags);
    tde_spin_unlock(&g_tde_osi_job_list->lock, lockflags);
    return;
}

#ifdef CONFIG_GFX_PROC_SUPPORT
static td_void tde_osi_list_proc(struct seq_file *p)
{
    if ((TD_NULL != p) && (TD_NULL != g_tde_osi_job_list)) {
        OT_PROC_PRINT(p, "finish handle\t: %u\n", g_tde_osi_job_list->handle_finished);
    }
    return;
}
#endif
