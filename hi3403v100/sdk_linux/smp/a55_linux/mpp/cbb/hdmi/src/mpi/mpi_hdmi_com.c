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
#include "mpi_hdmi_com.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ot_common.h>
#include "drv_hdmi_ioctl.h"
#include "list.h"
#include "ot_math.h"
#include "mkp_hdmi.h"

/* Constants for reported magic-number values. */
#define MPI_HDMI_COM_SLEEP_US_1000             1000

#define UMAP_DEVNAME_HDMI    "hdmi"
#define MAX_DELAY_TIME_MS    10000
#define TV_SEC_MULTIPLE      1000
#define THREAD_FUNC_NAMR_LEN 32
#define THREAD_EXIT_TIMEOUT  200

#define hdmi_check_lock_return(mutex, ret)                      \
    do {                                                        \
        if (pthread_mutex_lock(&(mutex)) != 0) {                \
            hdmi_info_trace("pthread mutex lock is failed!\n"); \
            return ret;                                         \
        }                                                       \
    } while (0)

#define hdmi_unlock(mutex)                    \
    do {                                      \
        (void)pthread_mutex_unlock(&(mutex)); \
    } while (0)

#define hdmi_check_init_return(dev, mutex)              \
    do {                                                \
        if ((dev) < 0) {                                \
            hdmi_fatal_trace("HDMI device not init\n"); \
            hdmi_unlock(mutex);                         \
            return OT_ERR_HDMI_NOT_INIT;                \
        }                                               \
    } while (0)

#define hdmi_check_chn_open_return(hdmi_id, mutex)                       \
    do {                                                                 \
        if (g_hdmi_chn_user_param[(hdmi_id)].open != TD_TRUE) {          \
            hdmi_warn_trace("hdmi:%u do NOT open\n", (td_u32)(hdmi_id)); \
            hdmi_unlock(mutex);                                          \
            return OT_ERR_HDMI_DEV_NOT_OPEN;                             \
        }                                                                \
    } while (0)

#define hdmi_check_id_return(hdmi_id)                                      \
    do {                                                                   \
        if ((hdmi_id) >= HDMI_MPI_ID_MAX) {                                \
            hdmi_err_trace("hdmi_id %u is invalid.\n", (td_u32)(hdmi_id)); \
            return OT_ERR_HDMI_INVALID_PARA;                               \
        }                                                                  \
    } while (0)

#define hdmi_check_failure_return(ret)                     \
    do {                                                   \
        if ((ret) == TD_FAILURE) {                         \
            hdmi_err_trace("Hdmi Kernel Strategy fail\n"); \
            ret = OT_ERR_HDMI_STRATEGY_FAILED;             \
        }                                                  \
    } while (0)

typedef struct {
    struct list_head list;
    ot_hdmi_callback_func callback_func;
} hdmi_callback_node;

typedef struct {
    td_bool open;
    td_bool start;
    ot_hdmi_id hdmi_id;
    hdmi_base_attr attr;
    ot_hdmi_avi_infoframe avi_infoframe;
    ot_hdmi_audio_infoframe audio_infoframe;
} hdmi_chn_user_attr;

typedef struct {
    td_bool hdmi_init;
    td_bool enable_timer;  /* Timer thread Flag */
    pthread_t event_timer; /* Timer thread ID */
    td_bool hdmi_exit;     /* HDMI Exit Flag */
    td_u32 param;          /* Timer thread Param */
} hdmi_comm_user_attr;

static td_s32              g_hdmi_dev_fd = -1;
static hdmi_callback_node  g_hdmi_callback_list[HDMI_MPI_ID_MAX];
static hdmi_comm_user_attr g_hdmi_comm_user_param[HDMI_MPI_ID_MAX];
static hdmi_chn_user_attr  g_hdmi_chn_user_param[HDMI_MPI_ID_MAX];
static pthread_mutex_t     g_hdmi_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t     g_callback_mutex = PTHREAD_MUTEX_INITIALIZER;
static td_bool             g_event_thread_run = TD_FALSE;

static td_u64 mpi_hdmi_get_time_ms(td_void)
{
    td_u64 time;
    struct timeval tv = {0};

    gettimeofday(&tv, TD_NULL);
    time = ((td_u32)tv.tv_sec * TV_SEC_MULTIPLE) + ((td_u32)tv.tv_usec / TV_SEC_MULTIPLE);

    return time;
}

static void hdmi_event_callback_invoke(ot_hdmi_id hdmi, ot_hdmi_event_type event)
{
    hdmi_callback_node *tmp = TD_NULL;
    struct list_head *pos = TD_NULL;
    struct list_head *q = TD_NULL;

    if (hdmi >= HDMI_MPI_ID_MAX) {
        hdmi_err_trace("hdmi %u, event %u is invalid\n", hdmi, event);
        return;
    }
    /* camera only support HPD/UNHPD */
    if (event != OT_HDMI_EVENT_HOTPLUG && event != OT_HDMI_EVENT_NO_PLUG) {
        return;
    }
    if (pthread_mutex_lock(&g_callback_mutex) != 0) {
        hdmi_info_trace("pthread mutex lock is failed!\n");
        return;
    }
    list_for_each_safe(pos, q, &g_hdmi_callback_list[hdmi].list) {
        tmp = list_entry(pos, hdmi_callback_node, list);
        if (tmp != TD_NULL && tmp->callback_func.hdmi_event_callback != TD_NULL) {
            tmp->callback_func.hdmi_event_callback(event, tmp->callback_func.private_data);
        }
    }
    hdmi_unlock(g_callback_mutex);

    return;
}

static ot_hdmi_event_type event_type_drv2user(hdmi_event kernel_event)
{
    ot_hdmi_event_type ret;

    switch (kernel_event) {
        case HDMI_EVENT_HOTPLUG:
            ret = OT_HDMI_EVENT_HOTPLUG;
            break;
        case HDMI_EVENT_HOTUNPLUG:
            ret = OT_HDMI_EVENT_NO_PLUG;
            break;
        case HDMI_EVENT_EDID_FAIL:
            ret = OT_HDMI_EVENT_EDID_FAIL;
            break;
        default:
            ret = OT_HDMI_EVENT_BUTT;
            break;
    }

    return ret;
}

static td_void *hdmi_event_poll_thread(void *param)
{
    td_s32 ret;
    ot_hdmi_id hdmi;
    ot_hdmi_event_type event_type;
    drv_hdmi_event curr_event = {0};
    td_char func_name[THREAD_FUNC_NAMR_LEN] = {0};

    ret = snprintf_s(func_name, THREAD_FUNC_NAMR_LEN, 14, "HDMI_Event"); /* 14, the max len of thread name */
    if (ret < 0) {
        hdmi_err_trace("snprintf_s err\n");
        return TD_NULL;
    }

    prctl(PR_SET_NAME, (uintptr_t)func_name, 0, 0, 0);
    if (param == TD_NULL) {
        hdmi_err_trace("param is invalid!\n");
        g_event_thread_run = TD_FALSE;
        return TD_NULL;
    }
    hdmi = *(ot_hdmi_id *)param;
    if (hdmi >= HDMI_MPI_ID_MAX) {
        hdmi_err_trace("hdmi %u is invalid!\n", hdmi);
        g_event_thread_run = TD_FALSE;
        return TD_NULL;
    }
    g_event_thread_run = TD_TRUE;
    while (g_hdmi_comm_user_param[hdmi].hdmi_exit == TD_FALSE) {
        if (g_hdmi_chn_user_param[hdmi].open == TD_FALSE) {
            ot_usleep(100 * MPI_HDMI_COM_SLEEP_US_1000); /* sleep 100ms if hdmi not open */
            continue;
        }
        (td_void)memset_s(&curr_event, sizeof(curr_event), 0, sizeof(curr_event));
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
        curr_event.hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
        ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_POLL_EVENT, &curr_event);
        if (ret == TD_SUCCESS && curr_event.event != HDMI_EVENT_BUTT) {
            event_type = event_type_drv2user(curr_event.event);
            hdmi_event_callback_invoke(hdmi, event_type);
        }
        ot_usleep(10 * MPI_HDMI_COM_SLEEP_US_1000); /* event read interval 10ms */
    }
    g_event_thread_run = TD_FALSE;

    return TD_NULL;
}

static td_s32 creat_event_poll_pthread(ot_hdmi_id hdmi)
{
    td_s32         ret;
    td_void       *param = TD_NULL;
    pthread_attr_t thread_attr = {0};

    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    g_hdmi_comm_user_param[hdmi].param = hdmi;
    param = (td_void *)(&(g_hdmi_comm_user_param[hdmi].param));
    ret = pthread_create(&g_hdmi_comm_user_param[hdmi].event_timer, &thread_attr, hdmi_event_poll_thread, param);
    pthread_attr_destroy(&thread_attr);

    return ret;
}

td_s32 mpi_hdmi_com_init(void)
{
    td_u32 hdmi_id;
    ot_hdmi_id hdmi = OT_HDMI_ID_0;
    const td_char hdmi_dev_name[] = "/dev/"UMAP_DEVNAME_HDMI;
    static td_u64 open_times = 0;

    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);

    if (g_hdmi_dev_fd >= 0 || g_hdmi_chn_user_param[hdmi].open == TD_TRUE) {
        hdmi_unlock(g_hdmi_mutex);
        return TD_SUCCESS;
    }
    if (g_event_thread_run == TD_FALSE) {
        for (hdmi_id = 0; hdmi_id < HDMI_MPI_ID_MAX; hdmi_id++) {
            INIT_LIST_HEAD(&g_hdmi_callback_list[hdmi_id].list);
        }
    }
    (td_void)memset_s(&g_hdmi_chn_user_param, sizeof(g_hdmi_chn_user_param), 0, sizeof(g_hdmi_chn_user_param));
    (td_void)memset_s(&g_hdmi_comm_user_param, sizeof(g_hdmi_comm_user_param), 0, sizeof(g_hdmi_comm_user_param));
    g_hdmi_dev_fd = open(hdmi_dev_name, O_RDWR);
    if (g_hdmi_dev_fd < 0) {
        hdmi_fatal_trace("open HDMI err.\n");
        perror("error opening file\n");
        hdmi_unlock(g_hdmi_mutex);
        goto exit;
    }
    open_times++;
    hdmi_fatal_trace("open HDMI SUCCESS!!!(fd = %d)(times = %llu).\n", g_hdmi_dev_fd, open_times);
    hdmi_unlock(g_hdmi_mutex);

    return TD_SUCCESS;

exit:
    return OT_ERR_HDMI_INIT_FAILED;
}

static td_void mpi_hdmi_wait_event_thread_exit(td_s32 hdmi_id)
{
    td_u64 start_time;

    if (g_hdmi_comm_user_param[hdmi_id].enable_timer != TD_TRUE) {
        return;
    }

    g_hdmi_comm_user_param[hdmi_id].enable_timer = TD_FALSE;
    start_time = mpi_hdmi_get_time_ms();
    while (g_event_thread_run == TD_TRUE &&
        ((mpi_hdmi_get_time_ms() - start_time) <= THREAD_EXIT_TIMEOUT)) {
        usleep(1000); /* 1000: 1 ms */
    }
}

static td_s32 mpi_hdmi_close_one_chn(td_s32 hdmi_id)
{
    td_s32 ret = 0;

    if (g_hdmi_chn_user_param[hdmi_id].open != TD_TRUE) {
        return TD_SUCCESS;
    }

    if (g_hdmi_chn_user_param[hdmi_id].start == TD_TRUE) {
        ret += ioctl(g_hdmi_dev_fd, CMD_HDMI_STOP, &hdmi_id);
        g_hdmi_chn_user_param[hdmi_id].start = TD_FALSE;
    }
    g_hdmi_chn_user_param[hdmi_id].open = TD_FALSE;
    g_hdmi_comm_user_param[hdmi_id].hdmi_exit = TD_TRUE;

    mpi_hdmi_wait_event_thread_exit(hdmi_id);
    ret += ioctl(g_hdmi_dev_fd, CMD_HDMI_CLOSE, &hdmi_id);
    return (ret != TD_SUCCESS) ? TD_FAILURE : TD_SUCCESS;
}

td_s32 mpi_hdmi_com_deinit(void)
{
    td_s32 ret = 0;
    td_s32 hdmi_id;
    static td_u64 close_times = 0;

    if (pthread_mutex_lock(&g_hdmi_mutex) != 0) {
        hdmi_info_trace("pthread mutex lock is failed!!\n");
        return OT_ERR_HDMI_MUTEX_LOCK_FAILED;
    }
    if (g_hdmi_dev_fd < 0) {
        hdmi_unlock(g_hdmi_mutex);
        return TD_SUCCESS;
    }

    for (hdmi_id = 0; hdmi_id < HDMI_MPI_ID_MAX; hdmi_id++) {
        ret = mpi_hdmi_close_one_chn(hdmi_id);
        if (ret != TD_SUCCESS) {
            hdmi_unlock(g_hdmi_mutex);
            hdmi_check_failure_return(ret);
            return ret;
        }
    }

    ret = close(g_hdmi_dev_fd);
    if (ret == 0) {
        close_times++;
        hdmi_fatal_trace("close HDMI SUCCESS+++(fd = %d)(times = %llu)\n", g_hdmi_dev_fd, close_times);
    } else {
        hdmi_fatal_trace("close HDMI err+++(%d)\n", g_hdmi_dev_fd);
        perror("error closing file\n");
    }
    g_hdmi_dev_fd = -1;
    (td_void)memset_s(&g_hdmi_comm_user_param, sizeof(g_hdmi_comm_user_param), 0, sizeof(g_hdmi_comm_user_param));
    hdmi_unlock(g_hdmi_mutex);

    return TD_SUCCESS;
}

td_s32 mpi_hdmi_com_open(ot_hdmi_id hdmi, const hdmi_open *open_param)
{
    td_s32    ret;
    td_u64    start_time;
    hdmi_open open = {0};

    hdmi_check_id_return(hdmi);
    if (pthread_mutex_lock(&g_hdmi_mutex) != 0) {
        hdmi_info_trace("pthread mutex lock is failed!\n");
        return OT_ERR_HDMI_NOT_INIT;
    }
    hdmi_check_init_return(g_hdmi_dev_fd, g_hdmi_mutex);
    if (g_hdmi_chn_user_param[hdmi].open) {
        hdmi_unlock(g_hdmi_mutex);
        return TD_SUCCESS;
    }

    /* The user thread must be reset when the user is re-opened. Otherwise, the user thread exits. */
    g_hdmi_comm_user_param[hdmi].hdmi_exit = TD_FALSE;
    ret = creat_event_poll_pthread(hdmi);
    if (ret != TD_SUCCESS) {
        hdmi_fatal_trace("timer task return:0x%x\n", ret);
        hdmi_unlock(g_hdmi_mutex);
        return OT_ERR_HDMI_PTHREAD_CREATE_FAILED;
    }
    g_hdmi_comm_user_param[hdmi].enable_timer = TD_TRUE;
    open.hdmi_id = (hdmi_device_id)hdmi;
    open.default_mode = HDMI_DEFAULT_ACTION_HDMI;
    if (open_param != TD_NULL) {
        open.default_mode = open_param->default_mode;
    }
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_OPEN, &open);
    if (ret != TD_SUCCESS) {
        hdmi_info_trace("hdmi open err:0x%x\n", ret);
        g_hdmi_comm_user_param[hdmi].enable_timer = TD_FALSE;
        g_hdmi_comm_user_param[hdmi].hdmi_exit = TD_TRUE;
        start_time = mpi_hdmi_get_time_ms();
        while (g_event_thread_run == TD_TRUE) {
            if ((mpi_hdmi_get_time_ms() - start_time) > 120) { /* 120, wait thread exit time(ms) */
                hdmi_err_trace("Thread exit timeout: %u\n", g_event_thread_run);
                break;
            }
        }
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);

        return ret;
    }
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_UNREGISTER_CALLBACK, &hdmi);
    g_hdmi_chn_user_param[hdmi].open = TD_TRUE;
    hdmi_unlock(g_hdmi_mutex);

    return TD_SUCCESS;
}

td_s32 mpi_hdmi_com_close(ot_hdmi_id hdmi)
{
    td_s32 ret = TD_SUCCESS;
    td_u64 start_time;

    hdmi_check_id_return(hdmi);

    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);

    if (g_hdmi_chn_user_param[hdmi].open != TD_TRUE) {
        hdmi_unlock(g_hdmi_mutex);
        return ret;
    }
    if (g_hdmi_chn_user_param[hdmi].start == TD_TRUE) {
        ret += ioctl(g_hdmi_dev_fd, CMD_HDMI_STOP, &hdmi);
        g_hdmi_chn_user_param[hdmi].start = TD_FALSE;
    }
    g_hdmi_chn_user_param[hdmi].open = TD_FALSE;
    g_hdmi_comm_user_param[hdmi].hdmi_exit = TD_TRUE;
    if (g_hdmi_comm_user_param[hdmi].enable_timer == TD_TRUE) {
        g_hdmi_comm_user_param[hdmi].enable_timer = TD_FALSE;
        start_time = mpi_hdmi_get_time_ms();
        /* wait thread exit and the resources recycled by system. */
        while (g_event_thread_run == TD_TRUE) {
            if ((mpi_hdmi_get_time_ms() - start_time) > 200) { /* 200, wait thread exit time(ms) */
                break;
            }
        }
    }
    ret += ioctl(g_hdmi_dev_fd, CMD_HDMI_CLOSE, &hdmi);
    ret = (ret != TD_SUCCESS) ? TD_FAILURE : TD_SUCCESS;
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    hdmi_unlock(g_hdmi_mutex);

    return ret;
}

td_s32 mpi_hdmi_com_start(ot_hdmi_id hdmi)
{
    td_s32 ret;

    hdmi_check_id_return(hdmi);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);

    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_START, &hdmi);
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    g_hdmi_chn_user_param[hdmi].start = TD_TRUE;
    hdmi_unlock(g_hdmi_mutex);

    return TD_SUCCESS;
}

td_s32 mpi_hdmi_com_stop(ot_hdmi_id hdmi)
{
    td_s32 ret;

    hdmi_check_id_return(hdmi);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);

    if (g_hdmi_chn_user_param[hdmi].start != TD_TRUE) {
        hdmi_unlock(g_hdmi_mutex);
        return TD_SUCCESS;
    }
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_STOP, &hdmi);
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    g_hdmi_chn_user_param[hdmi].start = TD_FALSE;
    hdmi_unlock(g_hdmi_mutex);

    return TD_SUCCESS;
}

td_s32 mpi_hdmi_com_set_infoframe(ot_hdmi_id hdmi, drv_hdmi_infoframe *user_infoframe)
{
    td_s32 ret;

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(user_infoframe, OT_ERR_HDMI_NULL_PTR);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    user_infoframe->hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_SET_INFOFRAME, user_infoframe);
    hdmi_unlock(g_hdmi_mutex);
    if (ret != TD_SUCCESS) {
        hdmi_check_failure_return(ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 mpi_hdmi_com_get_infoframe(ot_hdmi_id hdmi, drv_hdmi_infoframe *drv_infoframe)
{
    td_s32 ret;

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(drv_infoframe, OT_ERR_HDMI_NULL_PTR);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    drv_infoframe->hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_GET_INFOFRAME, drv_infoframe);
    hdmi_unlock(g_hdmi_mutex);
    if (ret != TD_SUCCESS) {
        hdmi_check_failure_return(ret);
        return ret;
    }

    return ret;
}

td_s32 mpi_hdmi_com_get_status(ot_hdmi_id hdmi, ot_hdmi_status *status)
{
    td_s32 ret;
    drv_hdmi_status drv_status = {0};

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(status, OT_ERR_HDMI_NULL_PTR);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);

    (td_void)memset_s(&drv_status, sizeof(drv_hdmi_status), 0, sizeof(drv_hdmi_status));
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    drv_status.hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_GET_STATUS, &drv_status);
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    status->is_connected = drv_status.status.connected;
    status->is_sink_power_on = drv_status.status.sink_power_on;
    hdmi_unlock(g_hdmi_mutex);

    return ret;
}

td_s32 mpi_hdmi_com_get_sink_capability(ot_hdmi_id hdmi, drv_hdmi_sink_capability *drv_cap)
{
    td_s32 ret;

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(drv_cap, OT_ERR_HDMI_NULL_PTR);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    drv_cap->hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_GET_SINK_CAPABILITY, drv_cap);
    hdmi_unlock(g_hdmi_mutex);
    hdmi_check_failure_return(ret);

    return ret;
}

td_s32 mpi_hdmi_com_force_get_edid(ot_hdmi_id hdmi, td_u8 *edid, td_u32 *edid_len)
{
    td_s32 ret;
    drv_hdmi_edid_raw_data edid_data = {0};

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(edid, OT_ERR_HDMI_NULL_PTR);
    hdmi_check_null_return(edid_len, OT_ERR_HDMI_NULL_PTR);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);

    *edid_len = 0;
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    edid_data.hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_UPDATE_EDID, &edid_data);
    hdmi_unlock(g_hdmi_mutex);
    if ((ret == TD_SUCCESS) && (edid_data.edid_raw.edid_valid == TD_TRUE) && edid_data.edid_raw.edid_len) {
        *edid_len = (edid_data.edid_raw.edid_len > HDMI_EDID_SIZE) ? HDMI_EDID_SIZE : edid_data.edid_raw.edid_len;
        /* the maximum of edid size (4 blocks, each block has 128 bytes) */
        ret = memcpy_s(edid, HDMI_EDID_SIZE, edid_data.edid_raw.edid, *edid_len);
        if (ret != EOK) {
            hdmi_err_trace("memcpy_s fail!\n");
            return OT_ERR_HDMI_INVALID_PARA;
        }
    } else {
        hdmi_err_trace("Force get edid fail!\n");
        ret = OT_ERR_HDMI_READ_EDID_FAILED;
    }

    return ret;
}

td_s32 mpi_hdmi_com_reg_callback_func(ot_hdmi_id hdmi, const ot_hdmi_callback_func *callback_func)
{
    td_s32 ret;
    hdmi_callback_node *tmp = NULL;
    struct list_head *pos = TD_NULL;
    struct list_head *q = TD_NULL;

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(callback_func, OT_ERR_HDMI_NULL_PTR);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
    hdmi_unlock(g_hdmi_mutex);

    hdmi_check_lock_return(g_callback_mutex, OT_ERR_HDMI_MUTEX_LOCK_FAILED);
    if (callback_func->hdmi_event_callback != TD_NULL) {
        list_for_each_safe(pos, q, &g_hdmi_callback_list[hdmi].list) {
            tmp = list_entry(pos, hdmi_callback_node, list);
            if (tmp != TD_NULL &&
                tmp->callback_func.hdmi_event_callback == callback_func->hdmi_event_callback) {
                hdmi_warn_trace("This callback has been registered.\n");
                hdmi_unlock(g_callback_mutex);
                return OT_ERR_HDMI_CALLBACK_ALREADY;
            }
        }
        tmp = (hdmi_callback_node *)malloc(sizeof(hdmi_callback_node));
        if (tmp == TD_NULL) {
            hdmi_err_trace("Callback malloc failed\n");
            hdmi_unlock(g_callback_mutex);
            return OT_ERR_HDMI_MALLOC_FAILED;
        }
        (td_void)memset_s(tmp, sizeof(hdmi_callback_node), 0, sizeof(hdmi_callback_node));
        tmp->callback_func = *callback_func;
    } else {
        hdmi_err_trace("Callbackaddr is NULL!\n");
        hdmi_unlock(g_callback_mutex);
        return OT_ERR_HDMI_INVALID_CALLBACK;
    }
    list_add_tail(&(tmp->list), &(g_hdmi_callback_list[hdmi].list));
    hdmi_unlock(g_callback_mutex);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_REGISTER_CALLBACK, &hdmi);
    hdmi_unlock(g_hdmi_mutex);
    hdmi_check_failure_return(ret);

    return ret;
}

td_s32 mpi_hdmi_com_unreg_callback_func(ot_hdmi_id hdmi, const ot_hdmi_callback_func *callback_func)
{
    td_s32 ret;
    hdmi_callback_node *tmp = TD_NULL;
    struct list_head *pos = TD_NULL;
    struct list_head *q = TD_NULL;

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(callback_func, OT_ERR_HDMI_NULL_PTR);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
    hdmi_unlock(g_hdmi_mutex);

    hdmi_check_lock_return(g_callback_mutex, OT_ERR_HDMI_MUTEX_LOCK_FAILED);
    list_for_each_safe(pos, q, &g_hdmi_callback_list[hdmi].list) {
        tmp = list_entry(pos, hdmi_callback_node, list);
        if (tmp != TD_NULL && callback_func->hdmi_event_callback != TD_NULL &&
            tmp->callback_func.hdmi_event_callback == callback_func->hdmi_event_callback) {
            list_del(pos);
            free(tmp);
            ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_UNREGISTER_CALLBACK, &hdmi);
            hdmi_unlock(g_callback_mutex);
            hdmi_check_failure_return(ret);

            return ret;
        }
    }
    hdmi_unlock(g_callback_mutex);
    hdmi_err_trace("No this Callbackfunc\n");

    return OT_ERR_HDMI_CALLBACK_NOT_REGISTER;
}

td_s32 mpi_hdmi_com_set_hw_spec(ot_hdmi_id hdmi, const ot_hdmi_hw_spec *hw_spec)
{
    td_s32 ret;
    drv_hdmi_hw_spec spec = {0};

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(hw_spec, OT_ERR_HDMI_NULL_PTR);

    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    spec.hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = memcpy_s(&spec.hw_spec, sizeof(hdmi_hw_spec), hw_spec, sizeof(ot_hdmi_hw_spec));
    if (ret != EOK) {
        hdmi_err_trace("memcpy_s fail.\n");
        hdmi_unlock(g_hdmi_mutex);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_SET_HW_SPEC, &spec);
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    hdmi_unlock(g_hdmi_mutex);

    return ret;
}

td_s32 mpi_hdmi_com_get_hw_spec(ot_hdmi_id hdmi, ot_hdmi_hw_spec *hw_spec)
{
    td_s32 ret;
    drv_hdmi_hw_spec spec = {0};

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(hw_spec, OT_ERR_HDMI_NULL_PTR);

    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    spec.hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_GET_HW_SPEC, &spec);
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    ret = memcpy_s(hw_spec, sizeof(ot_hdmi_hw_spec), &spec.hw_spec, sizeof(hdmi_hw_spec));
    if (ret != EOK) {
        hdmi_err_trace("memcpy_s fail.\n");
        hdmi_unlock(g_hdmi_mutex);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    hdmi_unlock(g_hdmi_mutex);

    return ret;
}

td_s32 mpi_hdmi_com_set_attr(ot_hdmi_id hdmi, drv_hdmi_property *property)
{
    td_s32 ret;

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(property, OT_ERR_HDMI_NULL_PTR);

    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    property->hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_SET_ATTR, property);
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    hdmi_unlock(g_hdmi_mutex);

    return ret;
}

td_s32 mpi_hdmi_com_get_attr(ot_hdmi_id hdmi, drv_hdmi_property *property)
{
    td_s32 ret;

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(property, OT_ERR_HDMI_NULL_PTR);

    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    property->hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_GET_ATTR, property);
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    hdmi_unlock(g_hdmi_mutex);

    return ret;
}

td_s32 mpi_hdmi_com_set_avmute(ot_hdmi_id hdmi, td_bool enable_avmute)
{
    td_s32 ret;
    drv_hdmi_avmute drv_avmute = {0};

    hdmi_check_id_return(hdmi);
    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    drv_avmute.hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    drv_avmute.avmute = enable_avmute;
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_SET_AVMUTE, &drv_avmute);
    hdmi_unlock(g_hdmi_mutex);
    if (ret != TD_SUCCESS) {
        hdmi_check_failure_return(ret);
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 mpi_hdmi_com_mod_param_set(ot_hdmi_id hdmi, const ot_hdmi_mod_param *user_mod_param)
{
    td_s32 ret;
    drv_hdmi_mod_param mod_param = {0};

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(user_mod_param, OT_ERR_HDMI_NULL_PTR);

    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    mod_param.hdmi_id = hdmi;
    mod_param.emi_en = user_mod_param->emi_en;
    mod_param.trace_len = user_mod_param->trace_len;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_SET_MOD_PARAM, &mod_param);
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    hdmi_unlock(g_hdmi_mutex);

    return ret;
}

td_s32 mpi_hdmi_com_mod_param_get(ot_hdmi_id hdmi, ot_hdmi_mod_param *user_mod_param)
{
    td_s32 ret;
    drv_hdmi_mod_param mod_param = {0};

    hdmi_check_id_return(hdmi);
    hdmi_check_null_return(user_mod_param, OT_ERR_HDMI_NULL_PTR);

    hdmi_check_lock_return(g_hdmi_mutex, OT_ERR_HDMI_DEV_NOT_OPEN);
    hdmi_check_chn_open_return(hdmi, g_hdmi_mutex);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    mod_param.hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    ret = ioctl(g_hdmi_dev_fd, CMD_HDMI_GET_MOD_PARAM, &mod_param);
    if (ret != TD_SUCCESS) {
        hdmi_unlock(g_hdmi_mutex);
        hdmi_check_failure_return(ret);
        return ret;
    }
    user_mod_param->emi_en = mod_param.emi_en;
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    user_mod_param->trace_len = mod_param.trace_len;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    hdmi_unlock(g_hdmi_mutex);

    return ret;
}

hdmi_deep_color deep_color_user2drv(ot_hdmi_deep_color user_deep_color)
{
    hdmi_deep_color ret;

    switch (user_deep_color) {
        case OT_HDMI_DEEP_COLOR_24BIT:
            ret = HDMI_DEEP_COLOR_24BIT;
            break;
        case OT_HDMI_DEEP_COLOR_30BIT:
            ret = HDMI_DEEP_COLOR_30BIT;
            break;
        case OT_HDMI_DEEP_COLOR_36BIT:
            ret = HDMI_DEEP_COLOR_36BIT;
            break;
        default:
            ret = HDMI_DEEP_COLOR_BUTT;
            break;
    }

    return ret;
}

ot_hdmi_deep_color deep_color_drv2user(hdmi_deep_color kernel_deep_color)
{
    ot_hdmi_deep_color ret;

    switch (kernel_deep_color) {
        case HDMI_DEEP_COLOR_24BIT:
            ret = OT_HDMI_DEEP_COLOR_24BIT;
            break;
        case HDMI_DEEP_COLOR_30BIT:
            ret = OT_HDMI_DEEP_COLOR_30BIT;
            break;
        case HDMI_DEEP_COLOR_36BIT:
            ret = OT_HDMI_DEEP_COLOR_36BIT;
            break;
        case HDMI_DEEP_COLOR_OFF:
            ret = OT_HDMI_DEEP_COLOR_24BIT;
            break;
        default:
            ret = OT_HDMI_DEEP_COLOR_BUTT;
            break;
    }

    return ret;
}

