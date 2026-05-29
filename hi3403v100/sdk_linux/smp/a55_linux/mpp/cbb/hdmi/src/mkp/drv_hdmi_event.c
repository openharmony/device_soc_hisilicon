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
#include "drv_hdmi_event.h"
#include "ot_common_hdmi.h"

#define HDMI_EVENT_NOT_MACH_SLEEP 500
#define HDMI_EVENT_READ_SLEEP     100
#define HDMI_EVENT_DEINIT_SLEEP   200

static hdmi_event_info g_event_info[HDMI_ID_MAX];

static hdmi_event_info *event_info_ptr_get(hdmi_device_id hdmi)
{
    if ((td_s32)hdmi < HDMI_ID_MAX) {
        return &g_event_info[hdmi];
    }
    return TD_NULL;
}

static td_s32 event_type_counter(hdmi_event_pool *pool, hdmi_event event, td_bool write)
{
    switch (event) {
        case HDMI_EVENT_HOTPLUG:
            write ? pool->run_cnt.hpd_wr_cnt++ : pool->run_cnt.hpd_rd_cnt++;
            break;
        case HDMI_EVENT_HOTUNPLUG:
            write ? pool->run_cnt.unhpd_wr_cnt++ : pool->run_cnt.unhpd_rd_cnt++;
            break;
        case HDMI_EVENT_EDID_FAIL:
            write ? pool->run_cnt.edid_fail_wr_cnt++ : pool->run_cnt.edid_fail_rd_cnt++;
            break;
        case HDMI_EVENT_RSEN_CONNECT:
            write ? pool->run_cnt.rsen_con_wr_cnt++ : pool->run_cnt.rsen_con_rd_cnt++;
            break;
        case HDMI_EVENT_RSEN_DISCONNECT:
            write ? pool->run_cnt.rsen_dis_wr_cnt++ : pool->run_cnt.rsen_dis_rd_cnt++;
            break;
        default:
            break;
    }

    return TD_SUCCESS;
}

static td_u32 event_mach_id(const hdmi_event_info *evt_info, td_u32 pool_id)
{
    td_u32 i;
    const hdmi_event_pool *tmp_pool = TD_NULL;

    hdmi_mutex_lock(evt_info->event_mutex);
    if (evt_info->total == 0) {
        hdmi_warn("event pool list is empty\n");
        hdmi_mutex_unlock(evt_info->event_mutex);
        return TD_SUCCESS;
    }

    /* find a match proc */
    for (i = 0, tmp_pool = &evt_info->pool[0]; i < HDMI_EVENT_POOL_CNT; i++, tmp_pool++) {
        if (tmp_pool->ctrl.pool_id == pool_id) {
            break;
        }
    }
    hdmi_mutex_unlock(evt_info->event_mutex);

    return i;
}

static td_s32 drv_hdmi_event_callback(const td_void *param)
{
    const hdmi_event_wait_callback *tmp = TD_NULL;
    td_s32 result;

    tmp = (const hdmi_event_wait_callback *)param;
    result =
        (tmp->evt_info->wakeup_all || tmp->tmp_pool->ctrl.wakeup_flag || (tmp->tmp_pool->ctrl.readable_cnt > 0));

    return result;
}

td_s32 drv_hdmi_event_init(hdmi_device_id hdmi_id)
{
    hdmi_event_info *evt_info = TD_NULL;

    hdmi_check_max_return(hdmi_id, HDMI_ID_MAX - 1, TD_FAILURE);
    evt_info = event_info_ptr_get(hdmi_id);
    hdmi_if_null_warn_return(evt_info, TD_FAILURE);

    if (evt_info->init != TD_TRUE) {
        (td_void)memset_s(evt_info, sizeof(hdmi_event_info), 0, sizeof(hdmi_event_info));
        /* init */
        osal_wait_init(&evt_info->wr_queue);
        osal_sema_init(&evt_info->event_mutex, 1);
        evt_info->total = 0;
        evt_info->init = TD_TRUE;
        evt_info->wakeup_all = TD_FALSE;
    }

    return TD_SUCCESS;
}

td_s32 drv_hdmi_event_deinit(hdmi_device_id hdmi_id)
{
    hdmi_event_info *evt_info = TD_NULL;

    hdmi_check_max_return(hdmi_id, HDMI_ID_MAX - 1, TD_FAILURE);
    evt_info = event_info_ptr_get(hdmi_id);
    hdmi_if_null_warn_return(evt_info, TD_FAILURE);
    hdmi_if_false_warn_return(evt_info->init, TD_FAILURE);

    /* wake up all process which is waiting for event */
    hdmi_mutex_lock(evt_info->event_mutex);
    evt_info->wakeup_all = TD_TRUE;
    hdmi_mutex_unlock(evt_info->event_mutex);

    osal_wakeup(&evt_info->wr_queue);
    osal_msleep(HDMI_EVENT_DEINIT_SLEEP);

    hdmi_mutex_lock(evt_info->event_mutex);
    (td_void)memset_s(evt_info->pool, sizeof(evt_info->pool), 0, sizeof(evt_info->pool));
    evt_info->total = 0;
    evt_info->init = TD_FALSE;
    hdmi_mutex_unlock(evt_info->event_mutex);
    osal_sema_destroy(&evt_info->event_mutex);
    osal_wait_destroy(&evt_info->wr_queue);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_event_pool_malloc(hdmi_device_id hdmi_id, td_u32 *pool_id)
{
    td_u32 i;
    td_u32 cur_gid;
    hdmi_event_pool *tmp_pool = TD_NULL;
    hdmi_event_info *evt_info = TD_NULL;

    hdmi_check_max_return(hdmi_id, HDMI_ID_MAX - 1, TD_FAILURE);
    evt_info = event_info_ptr_get(hdmi_id);
    hdmi_if_null_warn_return(evt_info, TD_FAILURE);
    hdmi_if_false_warn_return(evt_info->init, TD_FAILURE);

    cur_gid = (td_u32)hdmi_get_current_id();
    /* malloc a new pool element & init */
    hdmi_mutex_lock(evt_info->event_mutex);

    /* check ID exist */
    for (i = 0, tmp_pool = &evt_info->pool[0]; i < HDMI_EVENT_POOL_CNT; i++, tmp_pool++) {
        if (tmp_pool->ctrl.pool_id == cur_gid) {
            hdmi_warn("proc ID=%u  exist!\n", cur_gid);
            hdmi_mutex_unlock(evt_info->event_mutex);
            return HDMI_EVENT_ID_EXIST;
        }
    }

    for (i = 0, tmp_pool = &evt_info->pool[0]; i < HDMI_EVENT_POOL_CNT; i++, tmp_pool++) {
        if (tmp_pool->ctrl.pool_id == 0) {
            break;
        }
    }

    if (i >= HDMI_EVENT_POOL_CNT) {
        hdmi_warn("proc ID=%u no event sercer,pool max cnt = %u!\n", cur_gid, i);
        hdmi_mutex_unlock(evt_info->event_mutex);
        return TD_FAILURE;
    }

    (td_void)memset_s(&tmp_pool->ctrl, sizeof(tmp_pool->ctrl), 0, sizeof(hdmi_event_run_ctrl));
    (td_void)memset_s(&tmp_pool->run_cnt, sizeof(tmp_pool->run_cnt), 0, sizeof(hdmi_event_run_cnt));
    tmp_pool->ctrl.pool_id = cur_gid;
    if (pool_id != TD_NULL) {
        *pool_id = tmp_pool->ctrl.pool_id;
    }
    tmp_pool->ctrl.wakeup_flag = TD_FALSE;
    for (i = 0; i < HDMI_EVENT_POOL_SIZE; i++) {
        tmp_pool->ctrl.event_pool[i] = HDMI_EVENT_BUTT;
    }
    evt_info->total++;
    hdmi_mutex_unlock(evt_info->event_mutex);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_event_pool_free(hdmi_device_id hdmi_id, td_u32 pool_id)
{
    td_u32 i;
    hdmi_event_pool *tmp_pool = TD_NULL;
    hdmi_event_info *evt_info = TD_NULL;

    hdmi_check_max_return(hdmi_id, HDMI_ID_MAX - 1, TD_FAILURE);
    evt_info = event_info_ptr_get(hdmi_id);
    hdmi_if_null_warn_return(evt_info, TD_FAILURE);
    hdmi_if_false_warn_return(evt_info->init, TD_FAILURE);

    hdmi_mutex_lock(evt_info->event_mutex);
    /* find a match ID and free it */
    for (i = 0, tmp_pool = &evt_info->pool[0]; i < HDMI_EVENT_POOL_CNT; i++, tmp_pool++) {
        if (tmp_pool->ctrl.pool_id == pool_id) {
            evt_info->total--;
            tmp_pool->ctrl.wakeup_flag = TD_TRUE;
            osal_wakeup(&evt_info->wr_queue);
            tmp_pool->ctrl.pool_id = 0;
            hdmi_info("delete proc(%u) node\n", pool_id);
            break;
        }
    }
    hdmi_mutex_unlock(evt_info->event_mutex);

    /* can't find a match ID */
    if (i >= HDMI_EVENT_POOL_CNT) {
        hdmi_warn("proc id(%u) free fail\n", pool_id);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 drv_hdmi_event_pool_write(hdmi_device_id hdmi_id, hdmi_event event)
{
    td_u32 i;
    hdmi_event_pool *tmp_pool = TD_NULL;
    hdmi_event_info *evt_info = TD_NULL;

    hdmi_check_max_return(hdmi_id, HDMI_ID_MAX - 1, TD_FAILURE);
    evt_info = event_info_ptr_get(hdmi_id);
    hdmi_if_null_warn_return(evt_info, TD_FAILURE);
    hdmi_if_false_warn_return(evt_info->init, TD_FAILURE);

    if ((event < HDMI_EVENT_HOTPLUG) || (event > HDMI_EVENT_HOTUNPLUG)) {
        hdmi_warn("the event(0x%x) invalid ,we don't write!\n", event);
        return TD_FAILURE;
    }

    hdmi_mutex_lock(evt_info->event_mutex);
    if (evt_info->total == 0) {
        hdmi_warn("event pool is empty\n");
        hdmi_mutex_unlock(evt_info->event_mutex);
        return TD_SUCCESS;
    }

    /* write event into all event pool in the list */
    for (i = 0, tmp_pool = &evt_info->pool[0]; i < HDMI_EVENT_POOL_CNT; i++, tmp_pool++) {
        if (tmp_pool->ctrl.pool_id) {
            /* the event pool is overflow */
            if ((tmp_pool->ctrl.event_pool[tmp_pool->ctrl.write_ptr] >= HDMI_EVENT_HOTPLUG) &&
                (tmp_pool->ctrl.event_pool[tmp_pool->ctrl.write_ptr] <= HDMI_EVENT_HOTUNPLUG)) {
                tmp_pool->run_cnt.err_wd_cnt++;
                tmp_pool->ctrl.readable_cnt--;
                tmp_pool->ctrl.read_ptr = (tmp_pool->ctrl.write_ptr + 1) % HDMI_EVENT_POOL_SIZE;
                hdmi_warn("the event pool of proc(%u) is overflow\n", tmp_pool->ctrl.pool_id);
            }
            tmp_pool->ctrl.event_pool[tmp_pool->ctrl.write_ptr++] = event;
            tmp_pool->ctrl.write_ptr %= HDMI_EVENT_POOL_SIZE;
            event_type_counter(tmp_pool, event, TD_TRUE);
            tmp_pool->ctrl.readable_cnt++;
            osal_wakeup(&evt_info->wr_queue);
            hdmi_info("the event(0x%x) is writed into event pool of proc(%u) success\n",
                      event, tmp_pool->ctrl.pool_id);
        }
    }
    hdmi_mutex_unlock(evt_info->event_mutex);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_event_pool_read(hdmi_device_id hdmi_id, td_u32 pool_id, hdmi_event *event)
{
    td_u32 i;
    td_s32 ret;
    hdmi_event_pool         *tmp_pool = TD_NULL;
    hdmi_event_info         *evt_info = TD_NULL;
    hdmi_event_wait_callback callback = {0};

    evt_info = event_info_ptr_get(hdmi_id);
    hdmi_if_null_warn_return(evt_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_warn_return(event, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_warn_return(evt_info->init, OT_ERR_HDMI_NOT_INIT);

    *event = HDMI_EVENT_BUTT;
    i = event_mach_id(evt_info, pool_id);
    if (i >= HDMI_EVENT_POOL_CNT) {
        hdmi_err("the proc(%u) is not find\n", pool_id);
        osal_msleep(HDMI_EVENT_NOT_MACH_SLEEP);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    tmp_pool = &evt_info->pool[i];
    callback.evt_info = evt_info;
    callback.tmp_pool = tmp_pool;
    ret = osal_wait_event_timeout_interruptible(&evt_info->wr_queue, drv_hdmi_event_callback,
        (td_void *)&callback, HDMI_EVENT_READ_SLEEP);
    if (ret <= 0) {
        return TD_SUCCESS;
    }

    hdmi_mutex_lock(evt_info->event_mutex);
    if (tmp_pool->ctrl.readable_cnt == 0) {
        hdmi_mutex_unlock(evt_info->event_mutex);
        return TD_SUCCESS;
    } else {
        *event = tmp_pool->ctrl.event_pool[tmp_pool->ctrl.read_ptr];
        tmp_pool->ctrl.event_pool[tmp_pool->ctrl.read_ptr] = HDMI_EVENT_BUTT;
        tmp_pool->ctrl.read_ptr = (tmp_pool->ctrl.read_ptr + 1) % HDMI_EVENT_POOL_SIZE;
        tmp_pool->ctrl.wakeup_flag = TD_FALSE;
        if ((*event >= HDMI_EVENT_HOTPLUG) && (*event <= HDMI_EVENT_HOTUNPLUG)) {
            tmp_pool->ctrl.readable_cnt--;
            event_type_counter(tmp_pool, *event, TD_FALSE);
            ret = TD_SUCCESS;
            hdmi_info("the proc(%u) poll event(0x%x) success\n", tmp_pool->ctrl.pool_id, *event);
        } else {
            tmp_pool->run_cnt.err_rd_cnt++;
            ret = OT_ERR_HDMI_READ_EVENT_FAILED;
        }
    }
    hdmi_mutex_unlock(evt_info->event_mutex);

    return ret;
}

td_s32 drv_hdmi_event_pool_status_get(hdmi_device_id hdmi_id, td_u32 pool_num,
                                      hdmi_event_run_ctrl *ctrl, hdmi_event_run_cnt *cnt)
{
    td_u32 i;
    td_u32 j = 0;
    errno_t ret;
    hdmi_event_pool *tmp_pool = TD_NULL;
    hdmi_event_info *evt_info = TD_NULL;

    hdmi_check_max_return(hdmi_id, HDMI_ID_MAX - 1, TD_FAILURE);
    evt_info = event_info_ptr_get(hdmi_id);
    hdmi_if_null_warn_return(evt_info, TD_FAILURE);
    hdmi_if_null_warn_return(ctrl, TD_FAILURE);
    hdmi_if_null_warn_return(cnt, TD_FAILURE);
    hdmi_if_false_warn_return(evt_info->init, TD_FAILURE);

    (td_void)memset_s(ctrl, sizeof(hdmi_event_run_ctrl), 0, sizeof(hdmi_event_run_ctrl));
    (td_void)memset_s(cnt, sizeof(hdmi_event_run_cnt), 0, sizeof(hdmi_event_run_cnt));

    hdmi_mutex_lock(evt_info->event_mutex);
    if ((pool_num > evt_info->total) || (pool_num == 0)) {
        hdmi_warn("the input pool num(%u) is wrong,event pool range is [1~%u]\n", pool_num, evt_info->total);
        hdmi_mutex_unlock(evt_info->event_mutex);
        return TD_FAILURE;
    }

    if (evt_info->total == 0) {
        hdmi_warn("event pool is empty\n");
        hdmi_mutex_unlock(evt_info->event_mutex);
        return TD_SUCCESS;
    }

    /* find a match ID and copy status */
    for (i = 0, tmp_pool = &evt_info->pool[0]; i < HDMI_EVENT_POOL_CNT; i++, tmp_pool++) {
        if (tmp_pool->ctrl.pool_id) {
            j++;
            if (j == pool_num) {
                ret = memcpy_s(ctrl, sizeof(*ctrl), &tmp_pool->ctrl, sizeof(hdmi_event_run_ctrl));
                hdmi_unlock_unequal_eok_return(ret, evt_info->event_mutex, OT_ERR_HDMI_INVALID_PARA);
                ret = memcpy_s(cnt, sizeof(*cnt), &tmp_pool->run_cnt, sizeof(hdmi_event_run_cnt));
                hdmi_unlock_unequal_eok_return(ret, evt_info->event_mutex, OT_ERR_HDMI_INVALID_PARA);
            }
        }
    }
    hdmi_mutex_unlock(evt_info->event_mutex);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_event_pool_total_get(hdmi_device_id hdmi_id, td_u32 *total)
{
    hdmi_event_info *evt_info = TD_NULL;

    hdmi_check_max_return(hdmi_id, HDMI_ID_MAX - 1, TD_FAILURE);
    evt_info = event_info_ptr_get(hdmi_id);
    hdmi_if_null_warn_return(evt_info, TD_FAILURE);
    hdmi_if_null_warn_return(total, TD_FAILURE);
    hdmi_if_false_warn_return(evt_info->init, TD_FAILURE);

    hdmi_mutex_lock(evt_info->event_mutex);
    *total = evt_info->total;
    hdmi_mutex_unlock(evt_info->event_mutex);

    return TD_SUCCESS;
}

