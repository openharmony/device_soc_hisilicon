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

#ifndef TDE_OSILIST_H
#define TDE_OSILIST_H

#include "tde_define.h"
#include "ot_drv_tde.h"
#include "osal_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Data struct of software list node */
typedef struct {
    struct osal_list_head list_head;
    td_s32 handle;                   /* Job handle of the instruct */
    td_s32 index;                    /*
                                      * Instruct serial number in job,
                                      * form one on start, the same number is the same instruct
                                      */
    tde_node_subm_type submit_type; /* current node type */
    tde_node_buf node_buf;        /* Node of operate config */
    td_u32 phy_buf_num;            /* Number of physical buffer distributed */
} tde_sw_node;

/* Job definition */
typedef struct {
    struct osal_list_head list_head;
    td_s32 handle;             /* Job handle */
    drv_tde_func_callback func_complete_callback;     /* Pointer of callback function */
    td_void *func_para;           /* Arguments of callback function */
    tde_notify_mode notify_type; /* Notice type after node completed */
    td_u32 cmd_num;             /* Instruct number of job */
    td_u32 node_num;            /* Node number of job */
    tde_sw_node *first_cmd;    /* Software node of first instruct in job */
    tde_sw_node *last_cmd;     /* Software node of last instruct in job  */
    tde_sw_node *tail_node;    /* Last software node of job */
    osal_wait_t query; /* Wait queue used in query */
    td_bool has_submitted;       /* If have submitted */
    td_bool aq_use_buf;       /* If using temporary buffer */
    td_u8 wait_for_done_count; /* wait job count */
    td_void *private_data;
} tde_sw_job;

/*
 * Function:      tde_osi_list_init
 * Description:   Initialize all lists inside software to use as TDE operation cache
 * Input:         null
 * Return:        Success/fail
 */
td_s32 tde_osi_list_init(td_void);

/*
 * Function:      tde_osi_list_term
 * Description:   Release all lists inside software to use as TDE operation cache
 * Input:         TD_NULL
 */
td_void tde_osi_list_term(td_void);

td_s32 tde_osi_list_begin_job(td_s32 *handle, td_void *private_data);

/*
 * Function:      tde_osi_list_cancel_job
 * Description:   Delete list of handle point
 * Input:         handle: the list wait for delete
 * Return:        success/fail
 */
td_s32 tde_osi_list_cancel_job(td_s32 handle);

/*
 * Function:      tde_osi_list_submit_job
 * Description:   Create list, return head pointer of list
 * Input:         handle: Head pointer of job needing submit
 *                is_block: if block
 *                time_out: time out
 *                func_compl_cb: callback function of complete operate
 * Return:        success/fail/time out
 */
td_s32 tde_osi_list_submit_job(drv_tde_end_job_cmd *end_job, drv_tde_func_callback func_compl_cb, td_void *func_para,
                               tde_notify_mode noti_type, td_bool is_sync);
/*
 * Prototype    : func_compl_cb
 * Description  : wait for all TDE operate completed
 * Return Value : TD_SUCCESS,TDE operate completed
 */
td_s32 tde_osi_list_wait_all_done(td_void);

/*
 * Function:      tde_osi_list_wait_for_done
 * Description:   Query if submitted TDE operate is completed
 * Input:         handle: task handle
 *                time_out: if Time out
 * Return:        Success/fail
 */
td_s32 tde_osi_list_wait_for_done(td_s32 handle, td_u32 time_out);

/*
 * Function:      tde_osi_list_comp_proc
 * Description:   Interrupt to handling all list operate is completed
 * Return:        task handle is created
 */
td_void tde_osi_list_node_comp(td_void);

/*
 * Function:      tde_osi_list_get_phy_buff
 * Description:   Get one physical buffer, used in deflicker and zoom
 * Return:        physical address assigned
 */
td_phys_addr_t tde_osi_list_get_phy_buff(td_u32 cbcr_offset);

td_void tde_osi_list_put_phy_buff(td_u32 buf_num);

td_void tde_list_free_pending_job(td_void *private_data);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TDE_OSILIST_H */
