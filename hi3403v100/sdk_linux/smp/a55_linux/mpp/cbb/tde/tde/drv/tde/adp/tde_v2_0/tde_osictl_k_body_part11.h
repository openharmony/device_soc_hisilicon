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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART11_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART11_H_

/*
 * Function:      tde_osi_adj_clip_para
 * Description:   set clip parameter to hardware node
 * Input:         hw_node: node need to set
 */
static td_void tde_osi_adj_clip_para(tde_hw_node *hw_node)
{
    td_u32 clip_start_x;
    td_u32 clip_start_y;
    td_u32 clip_end_x;
    td_u32 clip_end_y;
    td_u32 out_start_x;
    td_u32 out_start_y;
    td_u32 out_end_x;
    td_u32 out_end_y;
    td_u32 start_x;
    td_u32 start_y;
    td_u32 end_x;
    td_u32 end_y;

    if (!(hw_node->des_alpha.bits.des_crop_en)) {
        return;
    }

    clip_start_x = hw_node->des_crop_pos_st.bits.des_crop_start_x & 0xfff;
    clip_start_y = hw_node->des_crop_pos_st.bits.des_crop_start_y & 0xfff;
    clip_end_x = hw_node->des_crop_pos_ed.bits.des_crop_end_x & 0xfff;
    clip_end_y = hw_node->des_crop_pos_ed.bits.des_crop_end_y & 0xfff;

    out_start_x = 0;
    out_start_y = 0;
    out_end_x = out_start_x + ((hw_node->des_imgsize.bits.des_width + 1) & 0xfff) - 1;
    out_end_y = out_start_y + ((hw_node->des_imgsize.bits.des_height + 1) & 0xfff) - 1;

    start_x = (clip_start_x > out_start_x) ? clip_start_x : out_start_x;
    start_y = (clip_start_y > out_start_y) ? clip_start_y : out_start_y;
    end_x = (clip_end_x < out_end_x) ? clip_end_x : out_end_x;
    end_y = (clip_end_y < out_end_y) ? clip_end_y : out_end_y;

    if ((start_x > end_x) || (start_y > end_y)) {
        hw_node->des_alpha.bits.des_crop_en = 0;
    } else {
        hw_node->des_crop_pos_st.bits.des_crop_start_x = start_x;
        hw_node->des_crop_pos_st.bits.des_crop_start_y = start_y;
        hw_node->des_crop_pos_ed.bits.des_crop_end_x = end_x;
        hw_node->des_crop_pos_ed.bits.des_crop_end_y = end_y;
    }

    return;
}

static td_void tde_osi_none_cmd(tde_hw_node *hw_node, tde_sw_node *cmd, const tde_sw_job *job)
{
    tde_hw_node *hw_tail_node = TD_NULL;

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    tde_proc_record_node(hw_node);
#endif

#ifndef __RTOS__
    cmd->node_buf.buf = (td_void *)hw_node - TDE_NODE_HEAD_BYTE;
#else
    cmd->node_buf.buf = (td_u8 *)hw_node - TDE_NODE_HEAD_BYTE;
#endif

    cmd->node_buf.node_sz = sizeof(tde_hw_node);
    cmd->node_buf.update = (0xffffffff) | ((td_u64)0x000003ff << 32); /* 32 alg data */
    cmd->node_buf.phy_addr = wgetphy(cmd->node_buf.buf) + TDE_NODE_HEAD_BYTE;
    /*
     * If the tail node of the job is not null,add the current job node to the tail of the job list of hardware.
     * That is to say the next node address of the tail\
     * node is the current node address.
     */
    if (job->tail_node != TD_NULL) {
        hw_tail_node = (tde_hw_node *)((td_u8 *)job->tail_node->node_buf.buf + TDE_NODE_HEAD_BYTE);
        hw_tail_node->tde_pnext_low.bits.p_next_low = get_low_addr(cmd->node_buf.phy_addr);
        hw_tail_node->tde_pnext_hi.bits.p_next_hi = get_high_addr(cmd->node_buf.phy_addr);
    }
    return;
}

/*
 * Function:      tde_osi_set_node_finish
 * Description:   complete node config, add node to list signed by handle
 * Input:         handle: task handle, which is sumbit list
 *                hw_node: set node
 *                work_buf_num: temporary buffer number
 *                subm_type: submit node type
 * Return:        return slice number
 * Others:        node struct is following:
 *                 ----------------------
 *                 |  software node pointer(4)   |
 *                 ----------------------
 *                 |  config parameter          |
 *                 ----------------------
 *                 |  physical address of next node(4) |
 *                 ----------------------
 *                 |  update flag of next node(4) |
 *                 ----------------------
 */
static td_s32 tde_osi_set_node_finish(td_s32 handle, tde_hw_node *hw_node,
                                      td_u32 work_buf_num, tde_node_subm_type subm_type)
{
    tde_handle_mgr *handle_mgr = TD_NULL;
    tde_sw_job *job = TD_NULL;
    tde_sw_node *cmd = TD_NULL;
    td_bool valid;

    tde_osi_adj_clip_para(hw_node);
    valid = tde_query_handle(handle, &handle_mgr);
    if (!valid) {
        tde_error("invalid handle %d!\n", handle);
        return DRV_ERR_TDE_INVALID_HANDLE;
    }
    job = (tde_sw_job *)handle_mgr->res;
    if (job->has_submitted) {
        tde_error("job %d already submitted!\n", handle);
        return DRV_ERR_TDE_INVALID_HANDLE;
    }
    cmd = (tde_sw_node *)tde_malloc(sizeof(tde_sw_node));
    if (cmd == TD_NULL) {
        tde_error("malloc failed!\n");
        return DRV_ERR_TDE_NO_MEM;
    }
    if (subm_type != TDE_NODE_SUBM_CHILD) {
        job->cmd_num++;
        if (job->cmd_num == 1) {
            job->first_cmd = cmd;

            OSAL_INIT_LIST_HEAD(&cmd->list_head);
        }
        job->last_cmd = cmd;
    }

    tde_osi_none_cmd(hw_node, cmd, job);
    cmd->handle = job->handle;
    cmd->index = job->cmd_num;
    cmd->submit_type = subm_type;
    cmd->phy_buf_num = work_buf_num;
    *(((td_u64 *)cmd->node_buf.buf) + 1) = handle;
    osal_list_add_tail(&cmd->list_head, &job->first_cmd->list_head); /* Add the cmd to the job list */
    job->tail_node = cmd;
    job->node_num++;
    if (cmd->phy_buf_num != 0) {
        job->aq_use_buf = TD_TRUE;
    }

    return TD_SUCCESS;
}

/*
 * Function:      tde_osi_begin_job
 * Description:   get TDE task handle
 * Return:        created task handle
 */
td_s32 tde_osi_begin_job(td_s32 *handle, td_void *private_data)
{
    return tde_osi_list_begin_job(handle, private_data);
}

td_s32 tde_osi_begin_job_ex(td_s32 *handle)
{
    return tde_osi_list_begin_job(handle, TD_NULL);
}

td_s32 tde_osi_end_job(drv_tde_end_job_cmd *end_job, drv_tde_func_callback func_compl_cb, td_void *func_para)
{
    tde_notify_mode noti_type;

    if (end_job == TD_NULL) {
        tde_error("null pointer!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (((end_job->is_block != TD_TRUE) && (end_job->is_block != TD_FALSE)) ||
        ((end_job->is_sync != TD_TRUE) && (end_job->is_sync != TD_FALSE))) {
        tde_error("bool should be TRUE or FALSE!!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    if (end_job->is_block) {
        if (osal_in_interrupt()) {
            tde_error("can not be block in interrupt!\n");
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
        noti_type = TDE_JOB_WAKE_NOTIFY;
    } else {
        noti_type = TDE_JOB_COMPL_NOTIFY;
    }
    return tde_osi_list_submit_job(end_job, func_compl_cb, func_para, noti_type, end_job->is_sync);
}

/*
 * Function:      tde_osi_cancel_job
 * Description:   delete created TDE task, only effective for call before endjob
 *                use to release software resource of list of task
 * Input:         handle: task handle
 * Return:        success/fail
 */
td_s32 tde_osi_cancel_job(td_s32 handle)
{
    return tde_osi_list_cancel_job(handle);
}

/*
 * Function:      tde_osi_wait_for_done
 * Description:   wait for completion of submit TDE operate
 * Input:         handle: task handle
 * Return:        success/fail
 */
td_s32 tde_osi_wait_for_done(td_s32 handle, td_u32 time_out)
{
    if (osal_in_interrupt()) {
        tde_error("can not be block in interrupt!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return tde_osi_list_wait_for_done(handle, time_out);
}

/*
 * Function:      tde_osi_wait_all_done
 * Description:   wait for all TDE operate completion
 * Return:        success/fail
 */
td_s32 tde_osi_wait_all_done(td_bool is_sync)
{
    ot_unused(is_sync);
    if (osal_in_interrupt()) {
        tde_error("can not wait in interrupt!\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    return tde_osi_list_wait_all_done();
}

/*
 * Function:      tde_osi_quick_copy
 * Description:   quick blit source to target, no any functional operate, the size of source and target are the same
 *                format is not MB format
 */
td_s32 tde_osi_quick_copy(td_s32 handle, const drv_tde_single_src *single_src)
{
    drv_tde_double_src double_src;

    if (single_src == TD_NULL) {
        tde_error("null pointer!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    double_src.bg_surface  = TD_NULL;
    double_src.bg_rect  = TD_NULL;
    double_src.fg_surface = single_src->src_surface;
    double_src.fg_rect = single_src->src_rect;
    double_src.dst_surface = single_src->dst_surface;
    double_src.dst_rect = single_src->dst_rect;

    return tde_osi_blit(handle, &double_src, TD_NULL);
}

td_void tde_set_resize_filter(td_bool is_resize_filter)
{
    g_is_resize_filter = is_resize_filter;
}

#if (TDE_CAPABILITY & RESIZE)
/*
 * Function:      tde_osi_quick_resize
 * Description:   zoom the size of source bitmap to the size aasigned by target bitmap,
                  of which source and target can be the same
 * Input:         src_surface: source bitmap info struct
 *                dst_surface: target bitmap info struct
 *                pFuncComplCB: callback function pointer when operate is over;if null, to say to no need to notice
 * Others:        add support for YCbCr422
 */
td_s32 tde_osi_quick_resize(td_s32 handle, drv_tde_surface *src_surface, drv_tde_rect *src_rect,
                            drv_tde_surface *dst_surface, drv_tde_rect *dst_rect)
{
    drv_tde_opt option = { 0 };
    drv_tde_double_src double_src;
    option.resize = TD_TRUE;

    if (src_surface == TD_NULL) {
        tde_error("null pointer!\n");
        return DRV_ERR_TDE_NULL_PTR;
    }

    if (g_is_resize_filter) {
        option.filter_mode = DRV_TDE_FILTER_MODE_COLOR;
    } else {
        option.filter_mode = DRV_TDE_FILTER_MODE_NONE;
    }
    if (src_surface->color_format >= DRV_TDE_COLOR_FMT_JPG_YCBCR400MBP) {
        tde_error("This operation doesn't support Semi-plannar!\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }

    double_src.bg_surface  = TD_NULL;
    double_src.bg_rect  = TD_NULL;
    double_src.fg_surface = src_surface;
    double_src.fg_rect = src_rect;
    double_src.dst_surface = dst_surface;
    double_src.dst_rect = dst_rect;
    return tde_osi_blit(handle, &double_src, &option);
}
#endif


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_OSICTL_K_BODY_PART11_H_ */
