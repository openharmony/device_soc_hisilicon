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
#include "hdmi_hal_machine.h"
#include "hdmi_osal.h"
#include "drv_hdmi_common.h"

#define MACHINE_MAX_ID_NUM     19
#define MACHIME_NULL_TINTERVAL 20
#define MACHIME_MIN_TINTERVAL  5

typedef struct {
    osal_semaphore_t mach_mutex;
    td_bool init;
    td_u32 total;
    hdmi_mach_elem mach_elem[MACHINE_MAX_ID_NUM];
} hdmi_mach_info;

static hdmi_mach_info g_machine_info;

td_s32 hal_hdmi_mach_init(td_void)
{
    hdmi_mach_info *mach_info = &g_machine_info;

    if (!mach_info->init) {
        osal_sema_init(&mach_info->mach_mutex, 1);
        (td_void)memset_s(mach_info->mach_elem, sizeof(mach_info->mach_elem), 0, sizeof(mach_info->mach_elem));
        mach_info->total = 0;
        mach_info->init  = TD_TRUE;
    }

    return TD_SUCCESS;
}

td_s32 hal_hdmi_mach_deinit(td_void)
{
    hdmi_mach_info *mach_info = &g_machine_info;

    hdmi_if_false_return(mach_info->init, TD_FAILURE);

    hdmi_mutex_lock(mach_info->mach_mutex);
    (td_void)memset_s(mach_info->mach_elem, sizeof(mach_info->mach_elem), 0, sizeof(mach_info->mach_elem));
    mach_info->total = 0;
    mach_info->init = TD_FALSE;
    hdmi_mutex_unlock(mach_info->mach_mutex);
    osal_sema_destroy(&mach_info->mach_mutex);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_mach_invoke(td_void)
{
    td_u32 i;
    hdmi_mach_elem *tmp_elem  = TD_NULL;
    hdmi_mach_info *mach_info = &g_machine_info;

    hdmi_if_false_return(mach_info->init, TD_FAILURE);

    if (mach_info->total == 0) {
        osal_msleep(MACHIME_NULL_TINTERVAL);
    } else {
        for (i = 0; i < MACHINE_MAX_ID_NUM; ++i) {
            tmp_elem = &mach_info->mach_elem[i];
            if (!(tmp_elem->mach_run.valid_id) || !(tmp_elem->mach_run.enable) ||
                ((hdmi_osal_get_time_in_ms() - tmp_elem->mach_run.last_time) <= tmp_elem->mach_ctrl.interval)) {
                continue;
            }
            if (tmp_elem->mach_ctrl.callback != TD_NULL) {
                tmp_elem->mach_ctrl.callback(tmp_elem->mach_ctrl.data);
            }
            hdmi_mutex_lock(mach_info->mach_mutex);
            tmp_elem->mach_run.last_time = hdmi_osal_get_time_in_ms();
            tmp_elem->mach_run.timestamp[tmp_elem->mach_run.stamp_idx] = hdmi_osal_get_time_in_ms();
            tmp_elem->mach_run.stamp_idx = (tmp_elem->mach_run.stamp_idx + 1) % HDMI_MACH_MAX_STAMPE_NUM;
            tmp_elem->mach_run.run_cnt++;
            hdmi_mutex_unlock(mach_info->mach_mutex);
        }
    }

    return TD_SUCCESS;
}

td_s32 hal_hdmi_mach_register(const hdmi_mach_ctrl *mach_ctrl, td_u32 *mach_id)
{
    td_u32 i;
    errno_t errno;
    td_s32 name_len, ret;
    hdmi_mach_elem *tmp_elem  = TD_NULL;
    hdmi_mach_info *mach_info = &g_machine_info;

    hdmi_if_null_return(mach_ctrl, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(mach_id, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_false_return(mach_info->init, TD_FAILURE);

    if (mach_info->total >= MACHINE_MAX_ID_NUM) {
        hdmi_warn("FULL num=%u! register machine fail! \n", MACHINE_MAX_ID_NUM);
        ret = TD_FAILURE;
    } else {
        hdmi_mutex_lock(mach_info->mach_mutex);
        for (i = 0; i < MACHINE_MAX_ID_NUM; ++i) {
            tmp_elem = &mach_info->mach_elem[i];
            if (!tmp_elem->mach_run.valid_id) {
                (td_void)memset_s(&tmp_elem->mach_run, sizeof(tmp_elem->mach_run), 0, sizeof(hdmi_mach_run));
                errno = memcpy_s(&tmp_elem->mach_ctrl, sizeof(tmp_elem->mach_ctrl), mach_ctrl, sizeof(hdmi_mach_ctrl));
                hdmi_unlock_unequal_eok_return(errno, mach_info->mach_mutex, OT_ERR_HDMI_INVALID_PARA);
                hdmi_unlock_if_null_return(tmp_elem->mach_ctrl.name, mach_info->mach_mutex, TD_FAILURE);
                name_len = osal_strlen(tmp_elem->mach_ctrl.name);
                name_len = (name_len < HDMI_MACH_MAX_NAME_SIZE) ? name_len : (HDMI_MACH_MAX_NAME_SIZE - 1);
                errno = memcpy_s(tmp_elem->mach_run.name, HDMI_MACH_MAX_NAME_SIZE,
                    tmp_elem->mach_ctrl.name, name_len);
                hdmi_unlock_unequal_eok_return(errno, mach_info->mach_mutex, OT_ERR_HDMI_INVALID_PARA);
                tmp_elem->mach_run.name[name_len] = '\0';
                tmp_elem->mach_ctrl.name = tmp_elem->mach_run.name;
                mach_info->total++;
                tmp_elem->mach_run.valid_id = TD_TRUE;
                *mach_id = i;
                break;
            }
        }
        hdmi_mutex_unlock(mach_info->mach_mutex);

        if (i < MACHINE_MAX_ID_NUM) {
            hdmi_info("register new machine id=%u success!\n", *mach_id);
            ret = TD_SUCCESS;
        } else {
            hdmi_info("register new machine id=%u fail!\n", *mach_id);
            ret = TD_FAILURE;
        }
    }

    return ret;
}

td_s32 hal_hdmi_mach_unregister(td_u32 mach_id)
{
    hdmi_mach_elem *tmp_elem = TD_NULL;
    hdmi_mach_info *mach_info = &g_machine_info;

    hdmi_if_false_return(mach_info->init, TD_FAILURE);
    hdmi_check_max_return(mach_id, MACHINE_MAX_ID_NUM - 1, TD_FAILURE);

    hdmi_mutex_lock(mach_info->mach_mutex);
    tmp_elem = &mach_info->mach_elem[mach_id];
    (td_void)memset_s(&tmp_elem->mach_run, sizeof(tmp_elem->mach_run), 0, sizeof(hdmi_mach_run));
    (td_void)memset_s(&tmp_elem->mach_ctrl, sizeof(tmp_elem->mach_ctrl), 0, sizeof(hdmi_mach_ctrl));
    mach_info->total--;
    hdmi_mutex_unlock(mach_info->mach_mutex);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_mach_start(td_u32 mach_id)
{
    hdmi_mach_elem *tmp_elem = TD_NULL;
    hdmi_mach_info *mach_info = &g_machine_info;

    hdmi_if_false_return(mach_info->init, TD_FAILURE);
    hdmi_check_max_return(mach_id, MACHINE_MAX_ID_NUM - 1, TD_FAILURE);

    hdmi_mutex_lock(mach_info->mach_mutex);
    tmp_elem = &mach_info->mach_elem[mach_id];
    tmp_elem->mach_run.enable = TD_TRUE;
    tmp_elem->mach_run.enable_time = hdmi_osal_get_time_in_ms();
    hdmi_mutex_unlock(mach_info->mach_mutex);

    return TD_SUCCESS;
}

td_s32 hal_hdmi_mach_stop(td_u32 mach_id)
{
    hdmi_mach_elem *tmp_elem = TD_NULL;
    hdmi_mach_info *mach_info = &g_machine_info;

    hdmi_if_false_return(mach_info->init, TD_FAILURE);
    hdmi_check_max_return(mach_id, MACHINE_MAX_ID_NUM - 1, TD_FAILURE);

    hdmi_mutex_lock(mach_info->mach_mutex);
    tmp_elem = &mach_info->mach_elem[mach_id];
    tmp_elem->mach_run.enable = TD_FALSE;
    tmp_elem->mach_run.disable_time = hdmi_osal_get_time_in_ms();
    hdmi_mutex_unlock(mach_info->mach_mutex);

    return TD_SUCCESS;
}

td_u64 hal_hdmi_mach_ms_get(td_void)
{
    return (td_u64)hdmi_osal_get_time_in_ms();
}

