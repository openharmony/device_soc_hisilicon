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

#include "sys_ext.h"
#include "securec.h"
#include "ot_math.h"
#include "mm_ext.h"
#include "ot_osal.h"
#include "osal_ioctl.h"
#include "dev_ext.h"
#include "ot_common_sys.h"
#include "mkp_sys.h"
#include "sys_drv.h"
#include "sys_bind.h"
#include "sys.h"
#include "sys_hal.h"

static td_s32 sys_ioctl_dispatch(unsigned int cmd, unsigned long arg);

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static sys_context g_sys_ctx = {0};
static td_bool g_conf = TD_FALSE;
static td_u64 g_global_pts_base = 0;
static td_u64 g_local_pts_base = 0;
static td_u64 g_global_pts_last = 0;
static sys_mem_ctx g_mem_ctx[OT_ID_BUTT] = {0};
static struct osal_list_head g_list_mmz;
static osal_spinlock_t g_sys_spin_lock;

#define sys_spin_lock(flags)   osal_spin_lock_irqsave(&g_sys_spin_lock, &(flags))
#define sys_spin_unlock(flags) osal_spin_unlock_irqrestore(&g_sys_spin_lock, &(flags))

#define SYS_STATE_STARTED    0
#define SYS_STATE_STOPPING   1
#define SYS_STATE_STOPPED    2

#define PROC_MAX_TITLE_FORMAT_LEN 120
#define PROC_TITLE_PREFIX_LEN     40
#define PROC_MAX_TITLE_LEN        (PROC_MAX_TITLE_FORMAT_LEN - PROC_TITLE_PREFIX_LEN)

#define INTERVAL_MS          60000
#define GAP_MS               15000
#define OT_SYS_MAX_EXIT_SLEEP_TIME 50

static td_u64 sys_get_sched_clock(td_void)
{
    return osal_sched_clock();
}

/*
 * the return value of sched_clock may loopback after some years.
 * but we should better deal with 'loopback'.
 */
static td_u64 sys_get_local_cur_pts(td_void)
{
    static td_u64 time_last = 0;
    td_u64 time_now;

    time_now = sys_drv_get_local_pts();
    if (time_last > time_now) {
        osal_printk("sys_get_local_cur_pts error time_now=%llu,time_last=%llu\n", time_now, time_last);
    }

    time_last = time_now;

    return time_now;
}

static td_u64 sys_get_local_time_stamp(td_void)
{
    td_u64 time_now;
    unsigned long flags;

    sys_spin_lock(flags);
    time_now = sys_get_local_cur_pts();
    sys_spin_unlock(flags);

    return time_now;
}

/* using example: pts = sys_get_time_stamp(), unit is us. */
static td_u64 sys_get_time_stamp(td_void)
{
    td_u64 pts_cur;
    unsigned long flags;

    sys_spin_lock(flags);
    pts_cur = g_global_pts_base + (sys_get_local_cur_pts() - g_local_pts_base);
    /*
     * the pts_cur will never overflow. but g_global_pts_base may be reduced.
     * the pts_cur shouldn't recycle, so we adjust it bigger than pts_last;
     */
    if ((pts_cur < g_global_pts_last) && (g_global_pts_last != 0)) {
        pts_cur = g_global_pts_last + 10;    /* 10 interval */
    }

    g_global_pts_last = pts_cur;
    sys_spin_unlock(flags);
    return pts_cur;
}

/*
 * sys_sync_time_stamp using sample:
 * if no media bussines running, we should set init=TD_TRUE.
 * but if media bussines is running, we only should use init=TD_FALSE to
 * gently adjust the PTS.
 * !!!!! if no necessary, don't call it !!!!
 */
static td_void sys_sync_time_stamp(td_u64 base, td_bool init)
{
    unsigned long flags;

    sys_spin_lock(flags);
    g_global_pts_base = base;
    g_local_pts_base = sys_get_local_cur_pts();
    if (init) {
        g_global_pts_last = 0;
    }
    sys_spin_unlock(flags);
}

#ifdef CONFIG_OT_VI_PTS
static td_u64 sys_get_vi_local_cur_pts(td_s32 dev_id)
{
    static td_u64 vi_time_last[OT_VI_MAX_DEV_NUM] = {0};
    td_u64 time_now;

    time_now = sys_drv_get_vi_local_pts(dev_id);
    if (vi_time_last[dev_id] > time_now) {
        /* pts回绕由os处理,此处不做运算 */
        osal_printk("sys_get_vi_local_cur_pts error time_now=%llu,time_last=%llu\n",
                    time_now, vi_time_last[dev_id]);
    }

    vi_time_last[dev_id] = time_now;

    return time_now;
}

/* use sys_pts calculate vi_pts, sync vi and sys when pts_cur recycle */
static td_u64 sys_get_vi_time_stamp(td_s32 dev_id)
{
    td_u64 pts_cur;
    td_u64 sys_pts;
    td_u64 vi_pts;
    unsigned long flags;

    sys_spin_lock(flags);
    sys_pts = sys_get_local_cur_pts();
    pts_cur = g_global_pts_base + (sys_pts - g_local_pts_base);
    /*
     * the pts_cur will never overflow. but g_global_pts_base may be reduced.
     * the pts_cur shouldn't recycle, so we adjust it bigger than pts_last;
     */
    if ((pts_cur < g_global_pts_last) && (g_global_pts_last != 0)) {
        pts_cur = g_global_pts_last + 10;    /* 10 interval */
    }

    g_global_pts_last = pts_cur;
    sys_spin_unlock(flags);

    vi_pts = sys_get_vi_local_cur_pts(dev_id);
    if (sys_pts < vi_pts) {
        return pts_cur;
    }
    if (pts_cur < (sys_pts - vi_pts)) {
        return 0;
    }
    return pts_cur - (sys_pts - vi_pts);
}
#endif

static td_s32 sys_user_init(td_void)
{
    unsigned long flags;

    if (g_sys_ctx.state == SYS_STATE_STARTED) {
        sys_info_trace("sys init again!\n");
        return TD_SUCCESS;
    }

    if (g_sys_ctx.state == SYS_STATE_STOPPING) {
        sys_err_trace("sys is busy!\n");
        return OT_ERR_SYS_BUSY;
    }

    /* init the PTS system */
    sys_spin_lock(flags);
    g_global_pts_base = sys_get_local_cur_pts();
    g_local_pts_base = g_global_pts_base;
    sys_spin_unlock(flags);

    if (cmpi_init_modules()) {
        sys_err_trace("init modules failed!\n");
        return OT_ERR_SYS_BUSY;
    }

    g_sys_ctx.state = SYS_STATE_STARTED;
    sys_debug_trace("sys init ok!\n");

    return TD_SUCCESS;
}

static td_s32 sys_user_exit(td_void)
{
    td_u32 try_times = 10;

    if (g_sys_ctx.state == SYS_STATE_STOPPED) {
        sys_info_trace("sys exit again!\n");
        return TD_SUCCESS;
    }

    /* notify all modules to stop only one time. */
    if (g_sys_ctx.state == SYS_STATE_STARTED) {
        cmpi_stop_modules();
    }

    while (try_times) {
        try_times--;
        if (!cmpi_query_modules()) {
            break;
        }
        osal_msleep(OT_SYS_MAX_EXIT_SLEEP_TIME);
    }

    /* if timeout */
    if (!try_times) {
        g_sys_ctx.state = SYS_STATE_STOPPING;
        return OT_ERR_SYS_BUSY;
    }
    osal_msleep(OT_SYS_MAX_EXIT_SLEEP_TIME);

    cmpi_exit_modules();
    g_sys_ctx.state = SYS_STATE_STOPPED;
    g_conf = TD_FALSE;

    sys_debug_trace("sys exit ok!\n");
    return TD_SUCCESS;
}

static td_void sys_init_audio_mem_cnt(td_void)
{
    g_mem_ctx[OT_ID_AIO].max_dev_cnt = OT_AIO_MAX_NUM;
    g_mem_ctx[OT_ID_AIO].max_chn_cnt = 1;

    g_mem_ctx[OT_ID_AI].max_dev_cnt = OT_AI_DEV_MAX_NUM;
    g_mem_ctx[OT_ID_AI].max_chn_cnt = OT_AI_MAX_CHN_NUM;

    g_mem_ctx[OT_ID_AO].max_dev_cnt = OT_AO_DEV_MAX_NUM;
    g_mem_ctx[OT_ID_AO].max_chn_cnt = OT_AO_MAX_CHN_NUM;

    g_mem_ctx[OT_ID_AENC].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_AENC].max_chn_cnt = OT_AENC_MAX_CHN_NUM;

    g_mem_ctx[OT_ID_ADEC].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_ADEC].max_chn_cnt = OT_ADEC_MAX_CHN_NUM;
}

static td_void sys_set_mem_cnt(td_u32 mod_id, td_u32 max_dev_cnt, td_u32 max_chn_cnt)
{
    g_mem_ctx[mod_id].max_dev_cnt = max_dev_cnt;
    g_mem_ctx[mod_id].max_chn_cnt = max_chn_cnt;
}

static td_void sys_init_codec_mem_cnt(td_void)
{
#ifdef CONFIG_OT_AVS_SUPPORT
    sys_set_mem_cnt(OT_ID_AVS, OT_AVS_MAX_GRP_NUM, OT_AVS_MAX_CHN_NUM);
#endif

#ifdef CONFIG_OT_VDEC_SUPPORT
    sys_set_mem_cnt(OT_ID_VDEC, 1, OT_VDEC_MAX_CHN_NUM);
    sys_set_mem_cnt(OT_ID_VFMW, 1, 1);
    sys_set_mem_cnt(OT_ID_VDEC_ADAPT, 1, OT_VDEC_MAX_CHN_NUM);
    sys_set_mem_cnt(OT_ID_VDEC_SERVER, 1, OT_VDEC_MAX_CHN_NUM);
#endif
}

static td_void sys_init_display_mem_cnt(td_void)
{
#ifdef CONFIG_OT_VO_SUPPORT
    sys_set_mem_cnt(OT_ID_VO, OT_VO_MAX_LAYER_NUM, OT_VO_MAX_CHN_NUM);
#endif

#ifdef CONFIG_OT_VI_SUPPORT
    sys_set_mem_cnt(OT_ID_VI, OT_VI_MAX_PIPE_NUM, OT_VI_MAX_CHN_NUM);
#endif

    sys_set_mem_cnt(OT_ID_VPSS, OT_VPSS_MAX_GRP_NUM, 1);
    sys_set_mem_cnt(OT_ID_VENC, OT_VENC_MAX_RECV_SRC, OT_VENC_MAX_CHN_NUM);
    sys_set_mem_cnt(OT_ID_RGN, 1, 1);

#ifdef CONFIG_OT_GFBG_SUPPORT
    sys_set_mem_cnt(OT_ID_FB, OT_VO_MAX_GFX_LAYER_NUM, 1);
#endif
}

static td_void sys_init_link_mem_cnt(td_void)
{
#ifdef CONFIG_OT_PCIV_SUPPORT
    sys_set_mem_cnt(OT_ID_PCIV, 1, OT_PCIV_MAX_CHN_NUM);
#endif

    sys_set_mem_cnt(OT_ID_VGS, 1, 1);

#ifdef CONFIG_OT_VDA_SUPPORT
    sys_set_mem_cnt(OT_ID_VDA, 1, OT_VDA_CHN_NUM_MAX);
#endif

#ifdef CONFIG_OT_GDC_SUPPORT
    sys_set_mem_cnt(OT_ID_GDC, 1, 1);
#endif
}

static td_void sys_init_vpp_mem_cnt(td_void)
{
#ifdef CONFIG_OT_VPP_SUPPORT
#ifdef CONFIG_OT_VI_SUPPORT
    sys_set_mem_cnt(OT_ID_VPP, OT_VI_MAX_PIPE_NUM + OT_VPSS_MAX_GRP_NUM,
        MAX2(OT_VI_MAX_CHN_NUM, OT_VPSS_MAX_CHN_NUM));
#else
    sys_set_mem_cnt(OT_ID_VPP, OT_VPSS_MAX_GRP_NUM, OT_VPSS_MAX_CHN_NUM);
#endif
#endif

#ifdef CONFIG_OT_MCF_SUPPORT
    sys_set_mem_cnt(OT_ID_MCF, OT_MCF_MAX_GRP_NUM, OT_MCF_MAX_CHN_NUM);
#endif

#ifdef CONFIG_OT_PQP_SUPPORT
    sys_set_mem_cnt(OT_ID_PQP, 1, 1);
#endif

#ifdef CONFIG_OT_DCC_SUPPORT
    sys_set_mem_cnt(OT_ID_DCC, 1, 1);
#endif

#ifdef CONFIG_OT_UVC_SUPPORT
    sys_set_mem_cnt(OT_ID_UVC, 1, OT_UVC_MAX_CHN_NUM);
#endif
}

static td_void sys_init_video_mem_cnt(td_void)
{
    sys_init_codec_mem_cnt();
    sys_init_display_mem_cnt();
    sys_init_link_mem_cnt();
    sys_init_vpp_mem_cnt();
}

static td_void sys_init_svp_mem_cnt(td_void)
{
#ifdef CONFIG_OT_SVP_SUPPORT
    g_mem_ctx[OT_ID_SVP].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_SVP].max_chn_cnt = 1;

#ifdef CONFIG_OT_SVP_DSP
    g_mem_ctx[OT_ID_SVP_DSP].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_SVP_DSP].max_chn_cnt = 1;
#endif

#ifdef CONFIG_OT_SVP_CNN
    g_mem_ctx[OT_ID_SVP_NNIE].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_SVP_NNIE].max_chn_cnt = 1;
#endif
#endif

#ifdef CONFIG_OT_SVP_DPU_RECT
    g_mem_ctx[OT_ID_DPU_RECT].max_dev_cnt = OT_DPU_RECT_MAX_GRP_NUM;
    g_mem_ctx[OT_ID_DPU_RECT].max_chn_cnt = 1;
#endif

#ifdef CONFIG_OT_SVP_DPU_MATCH
    g_mem_ctx[OT_ID_DPU_MATCH].max_dev_cnt = OT_DPU_MATCH_MAX_GRP_NUM;
    g_mem_ctx[OT_ID_DPU_MATCH].max_chn_cnt = 1;
#endif

#ifdef CONFIG_OT_SVP_MAU
    g_mem_ctx[OT_ID_SVP_MAU].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_SVP_MAU].max_chn_cnt = 1;
#endif

#ifdef CONFIG_OT_SVP_IVE
    g_mem_ctx[OT_ID_IVE].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_IVE].max_chn_cnt = 1;
#endif

#ifdef CONFIG_OT_SVP_IVE
#ifdef CONFIG_OT_SVP_IVE_MULTI_KCF
    g_mem_ctx[OT_ID_KCF].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_KCF].max_chn_cnt = 1;
#endif
#endif
#ifdef CONFIG_OT_SVP_FD
    g_mem_ctx[OT_ID_FD].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_FD].max_chn_cnt = 1;
#endif
}

static td_void sys_init_svp_npu_mem_cnt(td_void)
{
#ifdef CONFIG_OT_SVP_NPU_V1R1
    g_mem_ctx[OT_ID_NPUDEV].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_NPUDEV].max_chn_cnt = 1;

    g_mem_ctx[OT_ID_AICPU].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_AICPU].max_chn_cnt = 1;

    g_mem_ctx[OT_ID_NPUDFX].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_NPUDFX].max_chn_cnt = 1;

    g_mem_ctx[OT_ID_TSFW].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_TSFW].max_chn_cnt = 1;
#endif

#ifdef CONFIG_OT_SVP_NPU_V2R1
    g_mem_ctx[OT_ID_SVP_NPU].max_dev_cnt = 1;
    g_mem_ctx[OT_ID_SVP_NPU].max_chn_cnt = 1;
#endif
}

static td_s32 sys_malloc_mmz_tbl(td_void)
{
    td_s32 ret;
    td_s32 i;
    td_u32 size;

    for (i = 0; i < OT_ID_BUTT; i++) {
        if ((g_mem_ctx[i].max_dev_cnt != 0) &&
            (g_mem_ctx[i].max_chn_cnt != 0)) {
            size = sizeof(sys_mod_chn_mmz) * g_mem_ctx[i].max_dev_cnt * g_mem_ctx[i].max_chn_cnt;

            g_mem_ctx[i].mmz_tbl = osal_kmalloc(size, osal_gfp_kernel);
            if (g_mem_ctx[i].mmz_tbl == TD_NULL) {
                sys_err_trace("no memory for mmz_tbl!\n");
                ret = OT_ERR_SYS_NO_MEM;
                goto failed;
            }
            (td_void)memset_s(g_mem_ctx[i].mmz_tbl, size, 0, size);
        }
    }

    OSAL_INIT_LIST_HEAD(&g_list_mmz);
    return TD_SUCCESS;

failed:
    for (i = 0; i < OT_ID_BUTT; i++) {
        if ((g_mem_ctx[i].max_dev_cnt != 0) &&
            (g_mem_ctx[i].max_chn_cnt != 0)) {
            if (g_mem_ctx[i].mmz_tbl != TD_NULL) {
                osal_kfree(g_mem_ctx[i].mmz_tbl);
                g_mem_ctx[i].mmz_tbl = TD_NULL;
            }
        }
    }

    return ret;
}

td_s32 sys_init_mem_conf(td_void)
{
    (td_void)memset_s(g_mem_ctx, sizeof(g_mem_ctx), 0, sizeof(g_mem_ctx));
    sys_init_audio_mem_cnt();
    sys_init_video_mem_cnt();
    sys_init_svp_mem_cnt();
    sys_init_svp_npu_mem_cnt();
    return sys_malloc_mmz_tbl();
}

td_s32 sys_exit_mem_conf(td_void)
{
    td_s32 ret = TD_SUCCESS;
    td_s32 i;
    sys_mem_node *node = TD_NULL;
    struct osal_list_head *list_tmp = TD_NULL;
    struct osal_list_head *list_node = TD_NULL;

    for (i = 0; i < OT_ID_BUTT; i++) {
        if ((g_mem_ctx[i].max_dev_cnt != 0) &&
            (g_mem_ctx[i].max_chn_cnt != 0)) {
            if (g_mem_ctx[i].mmz_tbl != TD_NULL) {
                osal_kfree(g_mem_ctx[i].mmz_tbl);
                g_mem_ctx[i].mmz_tbl = TD_NULL;
            }
        }
    }

    osal_list_for_each_safe(list_node, list_tmp, &g_list_mmz) {
        node = osal_list_entry(list_node, sys_mem_node, list);
        osal_list_del(list_node);
        osal_vfree(node);
    }

    return ret;
}

static td_s32 sys_check_mem_chn(const ot_mpp_chn *chn)
{
    if (chn->mod_id >= OT_ID_BUTT) {
        sys_err_trace("mod_id:%d is invalid !\n", chn->mod_id);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    if ((chn->dev_id < 0) || (chn->dev_id >= (td_s32)g_mem_ctx[chn->mod_id].max_dev_cnt) ||
        (chn->chn_id < 0) || (chn->chn_id >= (td_s32)g_mem_ctx[chn->mod_id].max_chn_cnt)) {
        sys_err_trace("mod_id:%d mod dev or chn is invalid, dev:%d, chn:%d !\n",
            chn->mod_id, chn->dev_id, chn->chn_id);
        return OT_ERR_SYS_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_u32 sys_mem_get_idx(const sys_mem_ctx *mem_ctx, td_s32 dev_id, td_s32 chn_id)
{
    return (mem_ctx->max_chn_cnt * dev_id) + chn_id;
}

static td_s32 sys_get_valid_mmz_tbl(const ot_mpp_chn *mpp_chn, sys_mod_chn_mmz **mmz_tbl)
{
    td_s32 ret;
    td_u32 index;
    sys_mem_ctx *mem_ctx = TD_NULL;

    ret = sys_check_mem_chn(mpp_chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    mem_ctx = &g_mem_ctx[mpp_chn->mod_id];
    index = sys_mem_get_idx(mem_ctx, mpp_chn->dev_id, mpp_chn->chn_id);

    *mmz_tbl = &mem_ctx->mmz_tbl[index];

    return TD_SUCCESS;
}

td_s32 sys_set_mem_conf(const ot_mpp_chn *mpp_chn, const td_char *pc_mmz_name)
{
    td_s32 ret;
    td_bool find;
    sys_mod_chn_mmz *mmz_tbl = TD_NULL;
    sys_mem_node *node = TD_NULL;
    struct osal_list_head *list_tmp = TD_NULL;
    struct osal_list_head *list_node = TD_NULL;

    sys_check_null_ptr_return(mpp_chn);

    ret = sys_get_valid_mmz_tbl(mpp_chn, &mmz_tbl);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    if ((pc_mmz_name == TD_NULL) || (osal_strcmp(pc_mmz_name, "\0") == 0)) {
        mmz_tbl->mmz_name = TD_NULL;
    } else {
        find = TD_FALSE;
        osal_list_for_each_safe(list_node, list_tmp, &g_list_mmz) {
            node = osal_list_entry(list_node, sys_mem_node, list);
            if (!osal_strcmp(pc_mmz_name, node->mmz_name)) {
                find = TD_TRUE;
                mmz_tbl->mmz_name = node->mmz_name;
                break;
            }
        }

        if (!find) {
            node = osal_vmalloc(sizeof(sys_mem_node));
            if (node == TD_NULL) {
                sys_err_trace("no memory for set memconf!\n");
                return OT_ERR_SYS_NO_MEM;
            }

            (td_void)memset_s(node->mmz_name, sizeof(node->mmz_name), 0, sizeof(node->mmz_name));

            if (strncpy_s(node->mmz_name, sizeof(node->mmz_name), pc_mmz_name, sizeof(node->mmz_name) - 1) != EOK) {
                osal_vfree(node);
                return OT_ERR_SYS_ILLEGAL_PARAM;
            }
            osal_list_add_tail(&node->list, &g_list_mmz);
            mmz_tbl->mmz_name = node->mmz_name;
        }
    }

    return TD_SUCCESS;
}

static td_s32 sys_get_mem_conf(const ot_mpp_chn *mpp_chn, td_char *pc_mmz_name, td_u32 mmz_name_len)
{
    td_s32 ret;
    td_u32 index;
    const td_char *name = "\0";
    sys_mem_ctx *mem_ctx = TD_NULL;
    sys_mod_chn_mmz *mmz_tbl = TD_NULL;

    if (mpp_chn == TD_NULL) {
        sys_err_trace("mpp_chn is null!\n");
        return OT_ERR_SYS_NULL_PTR;
    }

    ret = sys_check_mem_chn(mpp_chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    mem_ctx = &g_mem_ctx[mpp_chn->mod_id];
    index = sys_mem_get_idx(mem_ctx, mpp_chn->dev_id, mpp_chn->chn_id);

    mmz_tbl = &mem_ctx->mmz_tbl[index];

    ot_assert(mmz_tbl != TD_NULL);

    if (mmz_tbl->mmz_name == TD_NULL) {
        if (strncpy_s(pc_mmz_name, mmz_name_len, name, OT_MAX_MMZ_NAME_LEN - 1) != EOK) {
            return OT_ERR_SYS_ILLEGAL_PARAM;
        }
    } else {
        if (strncpy_s(pc_mmz_name, mmz_name_len, mmz_tbl->mmz_name, OT_MAX_MMZ_NAME_LEN - 1) != EOK) {
            return OT_ERR_SYS_ILLEGAL_PARAM;
        }
    }

    return ret;
}

static td_s32 sys_get_mmz_name(const ot_mpp_chn *chn, td_void **pp_mmz_name)
{
    td_s32 ret;
    td_u32 index;
    sys_mem_ctx *mem_ctx = TD_NULL;
    sys_mod_chn_mmz *mmz_tbl = TD_NULL;

    if (chn == TD_NULL) {
        sys_err_trace("chn is null!\n");
        return OT_ERR_SYS_NULL_PTR;
    }

    if (pp_mmz_name == TD_NULL) {
        sys_err_trace("pp_mmz_name is null!\n");
        return OT_ERR_SYS_NULL_PTR;
    }

    ret = sys_check_mem_chn(chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    mem_ctx = &g_mem_ctx[chn->mod_id];
    index = sys_mem_get_idx(mem_ctx, chn->dev_id, chn->chn_id);

    mmz_tbl = &mem_ctx->mmz_tbl[index];

    ot_assert(mmz_tbl != TD_NULL);

    *pp_mmz_name = mmz_tbl->mmz_name;

    return ret;
}

static td_s32 sys_print_proc_title(osal_proc_entry_t *s, const td_char *format, ...)
{
    td_s32  i;
    td_s32 len;
    osal_va_list args;

    td_char title[PROC_MAX_TITLE_FORMAT_LEN + 1] = {0};
    td_char title_format[] = {
        [0 ... PROC_MAX_TITLE_FORMAT_LEN - 1] = '-', '\n', '\0'
    };

    osal_va_start(args, format);
    len = vsnprintf_s(title, sizeof(title), sizeof(title) - 1, format, args);
    osal_va_end(args);

    if (len < 0 || len > PROC_MAX_TITLE_LEN) {
        return TD_FAILURE;
    }

    for (i = PROC_TITLE_PREFIX_LEN; i < PROC_TITLE_PREFIX_LEN + len; i++) {
        title_format[i] = title[i - PROC_TITLE_PREFIX_LEN];
    }
    osal_seq_printf(s, "\n");
    osal_seq_printf(s, "%s", title_format);

    return TD_SUCCESS;
}

#ifdef CONFIG_OT_SYS_MUTEX_REG_SUPPORT
td_s32 sys_get_mutex(td_u32 vaule)
{
    return sys_drv_get_mutex(vaule);
}

static td_void sys_release_mutex(td_u32 vaule)
{
    sys_drv_release_mutex(vaule);
}
#endif

static td_s32 sys_ioctl_pts(unsigned int cmd, unsigned long arg)
{
    td_u64 *pts = (td_u64 *)(td_uintptr_t)arg;

    if (pts == TD_NULL) {
        return TD_FAILURE;
    }
    if (cmd == SYS_GET_CURPTS) {
        *pts = sys_get_time_stamp();
    } else {
        sys_sync_time_stamp(*pts, (cmd == SYS_INIT_PTSBASE) ? TD_TRUE : TD_FALSE);
    }
    return TD_SUCCESS;
}

static td_s32 sys_ioctl_bind(unsigned int cmd, unsigned long arg)
{
    sys_bind_args *bind_arg = (sys_bind_args *)(td_uintptr_t)arg;
    sys_bind_src_args *bind_src_arg = (sys_bind_src_args *)(td_uintptr_t)arg;

    if (cmd == SYS_BIND_CTRL || cmd == SYS_UNBIND_CTRL || cmd == SYS_GETBINDBYDEST) {
        if (bind_arg == TD_NULL) {
            return TD_FAILURE;
        }
        if (cmd == SYS_BIND_CTRL) {
            return sys_bind(&bind_arg->src_chn, &bind_arg->dest_chn);
        }
        if (cmd == SYS_UNBIND_CTRL) {
            return sys_unbind(&bind_arg->src_chn, &bind_arg->dest_chn);
        }
        return sys_get_bind_by_dest(&bind_arg->dest_chn, &bind_arg->src_chn, TD_FALSE);
    }

    if (bind_src_arg == TD_NULL) {
        return TD_FAILURE;
    }
    return sys_get_bind_by_src(&bind_src_arg->src_chn, &bind_src_arg->dest_chns);
}

static td_s32 sys_ioctl_mem(unsigned int cmd, unsigned long arg)
{
    sys_mem_args *mem_arg = (sys_mem_args *)(td_uintptr_t)arg;

    if (mem_arg == TD_NULL) {
        return TD_FAILURE;
    }
    if (cmd == SYS_MEM_SET_CTRL) {
        return sys_set_mem_conf(&mem_arg->mpp_chn, mem_arg->mmz_name);
    }
    return sys_get_mem_conf(&mem_arg->mpp_chn, mem_arg->mmz_name, OT_MAX_MMZ_NAME_LEN);
}

static td_s32 sys_ioctl_dispatch(unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
        case SYS_INIT_CTRL:
            return sys_user_init();
        case SYS_GET_CURPTS:
        case SYS_INIT_PTSBASE:
        case SYS_SYNC_PTS:
            return sys_ioctl_pts(cmd, arg);
        case SYS_BIND_CTRL:
        case SYS_UNBIND_CTRL:
        case SYS_GETBINDBYDEST:
        case SYS_GETBINDBYSRC:
            return sys_ioctl_bind(cmd, arg);
        case SYS_MEM_SET_CTRL:
        case SYS_MEM_GET_CTRL:
            return sys_ioctl_mem(cmd, arg);
        default:
            sys_err_trace("ioctl cmd %x is not supported!\n", cmd);
            return TD_FAILURE;
    }
}

static long sys_ioctl(unsigned int cmd, unsigned long arg, td_void *private_data)
{
    td_s32 ret;
    ot_unused(private_data);

    if (osal_down(&g_sys_ctx.sem)) {
        return (-ERESTARTSYS);
    }

    if (cmd == SYS_EXIT_CTRL) {
        ret = sys_user_exit();
    } else {
        osal_atomic_inc_return(&g_sys_ctx.user_ref);
        ret = sys_ioctl_dispatch(cmd, arg);
        osal_atomic_dec_return(&g_sys_ctx.user_ref);
    }

    osal_up(&g_sys_ctx.sem);
    return ret;
}

#ifdef CONFIG_COMPAT
static long sys_compat_ioctl(unsigned int cmd, unsigned long arg, td_void *private_data)
{
    switch (cmd) {
        case SYS_MFLUSH_CACHE: {
            sys_mem_cache_info *mem_info = (sys_mem_cache_info *)(td_uintptr_t)arg;
            if (mem_info != TD_NULL) {
                OT_COMPAT_POINTER(mem_info->vir_addr, td_void *);
            }
            break;
        }

        default: {
            break;
        }
    }

    return sys_ioctl(cmd, arg, private_data);
}
#endif

static td_s32 open(td_void *private_data)
{
    ot_unused(private_data);
    return TD_SUCCESS;
}

static td_s32 close(td_void *private_data)
{
    ot_unused(private_data);
    return TD_SUCCESS;
}

#define SYS_PAGE_SHIFT       12

static int sys_mmap(osal_vm_t *vm, unsigned long start, unsigned long end,
                    unsigned long vm_pgoff, td_void *private_data)
{
    td_ulong size = end - start;
    td_phys_addr_t phy_addr;
    ot_unused(private_data);

    phy_addr = (td_phys_addr_t)(vm_pgoff << SYS_PAGE_SHIFT);
    if (cmpi_check_mmz_phy_addr(phy_addr, size) != TD_SUCCESS) {
        sys_err_trace("addr: %#lx, size: %lu, invalid phyaddr!\n", (td_ulong)phy_addr, size);
        return TD_FAILURE;
    }

    osal_pgprot_cached(vm);
    if (osal_remap_pfn_range(vm, start, vm_pgoff, size)) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
static td_void sys_mem_proc_show(osal_proc_entry_t *s)
{
    td_u32 i;
    td_u32 j;
    td_u32 tbl_size;
    sys_mem_ctx *mem_ctx = TD_NULL;

    call_sys_print_proc_title(s, "mem table");
    osal_seq_printf(s, "%8s" "%17s" "%8s" "%8s" "%17s" "\n", "mod", "mod_name", "dev", "chn", "mmz_name");
    for (i = 0; i < OT_ID_BUTT; i++) {
        mem_ctx = &g_mem_ctx[i];
        if ((mem_ctx->max_dev_cnt == 0) || (mem_ctx->max_chn_cnt == 0)) {
            continue;
        }

        tbl_size = mem_ctx->max_dev_cnt * mem_ctx->max_chn_cnt;
        for (j = 0; j < tbl_size; j++) {
            if (mem_ctx->mmz_tbl[j].mmz_name != TD_NULL) {
                osal_seq_printf(s, "%8u" "%17s" "%8u" "%8u" "%17s" "\n",
                    i, cmpi_get_module_name(i), (j / (mem_ctx->max_chn_cnt)),
                    (j % (mem_ctx->max_chn_cnt)), mem_ctx->mmz_tbl[j].mmz_name);
            }
        }
    }
}

char *g_sys_state_string[] = { "run", "exiting", "exited" };

static td_s32 sys_proc_show(osal_proc_entry_t *s)
{
    osal_seq_printf(s, "\n[SYS] Version: [" OT_MPP_VERSION "], Build Time["__DATE__
                    ", "__TIME__ "]\n\n\n");
    if (g_sys_ctx.state != SYS_STATE_STARTED) {
        return TD_SUCCESS;
    }

    call_sys_print_proc_title(s, "module status");
    osal_seq_printf(s, "  status\n");
    osal_seq_printf(s, "%8s\n", g_sys_state_string[g_sys_ctx.state]);

    sys_mem_proc_show(s);
    sys_bind_proc_show(s);

    return TD_SUCCESS;
}

#endif

static osal_fileops_t file_op = {
    .open = open,
    .unlocked_ioctl = sys_ioctl,
    .release = close,
    .mmap = sys_mmap,

#ifdef CONFIG_COMPAT
    .compat_ioctl = sys_compat_ioctl,
#endif
};

static osal_dev_t *g_sys_device = TD_NULL;

static sys_export_func s_export_funcs = {
    .pfn_sys_get_time_stamp = sys_get_time_stamp,
    .pfn_sys_get_local_time_stamp = sys_get_local_time_stamp,
    .pfn_sys_sync_time_stamp = sys_sync_time_stamp,
    .pfn_sys_drv_ioctrl = sys_drv_drv_ioctrl,
    .pfn_sys_register_sender = sys_bind_register_sender,
    .pfn_sys_unregister_sender = sys_bind_unregister_sender,
    .pfn_sys_register_receiver = sys_bind_register_receiver,
    .pfn_sys_unregister_receiver = sys_bind_unregister_receiver,
    .pfn_sys_send_data = sys_bind_send_data,
    .pfn_sys_reset_data = sys_bind_reset_data,
    .pfn_get_bind_by_src = sys_get_bind_by_src_with_lock,
    .pfn_get_bind_num_by_src = sys_get_bind_num_by_src,
    .pfn_get_bind_by_dest = sys_get_bind_by_dest_inner,
    .pfn_get_mmz_name = sys_get_mmz_name,
    .pfn_sys_get_sched_clock = sys_get_sched_clock,
    .pfn_sys_get_sys_base_addr = sys_hal_get_sys_base_addr,
    .pfn_sys_print_proc_title = sys_print_proc_title,
    .pfn_sys_is_tee_enable_otp = TD_NULL,
#ifdef CONFIG_OT_VI_PTS
    .pfn_sys_get_vi_time_stamp = sys_get_vi_time_stamp,
#endif
#ifdef CONFIG_OT_SYS_MUTEX_REG_SUPPORT
    .pfn_sys_get_mutex = sys_get_mutex,
    .pfn_sys_release_mutex = sys_release_mutex,
#endif
};

td_s32 sys_do_init(td_void *arg)
{
    ot_unused(arg);
    return TD_SUCCESS;
}

td_void sys_do_exit(td_void)
{
    sys_bind_exit();
    sys_debug_trace("sys mod exit ok!\n");
}

static td_void sys_query_state(mod_state *state)
{
    if (osal_atomic_read(&g_sys_ctx.user_ref) == 0) {
        *state = MOD_STATE_FREE;
    } else {
        *state = MOD_STATE_BUSY;
    }
}

static td_void sys_notify(mod_notice_id notice)
{
    ot_unused(notice);
}

static umap_module s_module = {
    .mod_id = OT_ID_SYS,
    .mod_name = "sys",

    .pfn_init = sys_do_init,
    .pfn_exit = sys_do_exit,
    .pfn_query_state = sys_query_state,
    .pfn_notify = sys_notify,
    .pfn_ver_checker = TD_NULL,
    .export_funcs = &s_export_funcs,
    .data = TD_NULL,
};

static td_s32 sys_lock_init(td_void)
{
    if (osal_spin_lock_init(&g_sys_spin_lock) < 0) {
        return TD_FAILURE;
    }

    if (osal_atomic_init(&g_sys_ctx.user_ref) < 0) {
        osal_spin_lock_destroy(&g_sys_spin_lock);
        return TD_FAILURE;
    }

    osal_atomic_set(&g_sys_ctx.user_ref, 0);

    if (osal_sema_init(&g_sys_ctx.sem, 1) < 0) {
        osal_atomic_destroy(&g_sys_ctx.user_ref);
        osal_spin_lock_destroy(&g_sys_spin_lock);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void sys_lock_destory(td_void)
{
    osal_atomic_destroy(&g_sys_ctx.user_ref);
    osal_sema_destroy(&g_sys_ctx.sem);
    osal_spin_lock_destroy(&g_sys_spin_lock);
}

static inline td_void sys_device_deinit(td_void)
{
    osal_deregisterdevice(g_sys_device);
    osal_destroydev(g_sys_device);
    g_sys_device = TD_NULL;
}

static td_s32 sys_device_init(td_void)
{
    g_sys_device = osal_createdev(UMAP_DEVNAME_SYSCTL);
    if (g_sys_device == TD_NULL) {
        sys_err_trace("SYS createdev failed!\n");
        return TD_FAILURE;
    }
    g_sys_device->fops = &file_op;
    g_sys_device->minor = UMAP_SYS_MINOR_BASE;
    if (osal_registerdevice(g_sys_device)) {
        sys_err_trace("register system device failed!\n");
        osal_destroydev(g_sys_device);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_void sys_do_mpp_deinit(td_void)
{
    sys_bind_mod_exit();
    sys_drv_exit();
    sys_exit_mem_conf();
}

static td_s32 sys_do_mpp_init(td_void)
{
    if (sys_init_mem_conf() != TD_SUCCESS) {
        sys_err_trace("sys_init_mem_conf failed!\n");
        return TD_FAILURE;
    }

    if (sys_drv_init()) {
        sys_err_trace("sys_drv_init failed!\n");
        goto mem_exit;
    }

    if (sys_bind_mod_init() != TD_SUCCESS) {
        sys_err_trace("sys_bind_mod_init failed!\n");
        goto drv_exit;
    }

    sys_bind_init();
    return TD_SUCCESS;

drv_exit:
    sys_drv_exit();
mem_exit:
    sys_exit_mem_conf();
    return TD_FAILURE;
}

td_s32 sys_do_mod_init(td_void)
{
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_proc_entry_t *proc = TD_NULL;
#endif
    if (sys_lock_init() != TD_SUCCESS) {
        sys_err_trace("SYS lock init failed!\n");
        return TD_FAILURE;
    }

    g_conf = TD_FALSE;
    g_sys_ctx.state = SYS_STATE_STOPPED;
    g_sys_ctx.sys_cfg.align = OT_DEFAULT_ALIGN;

    if (sys_device_init()) {
        goto lock_destroy;
    }

    if (cmpi_register_module(&s_module)) {
        sys_err_trace("register mod failed!\n");
        goto dev_deinit;
    }

#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    proc = osal_create_proc_entry(PROC_ENTRY_SYS, TD_NULL);
    if (proc == TD_NULL) {
        sys_err_trace("create proc failed!\n");
        goto mod_unregister;
    }
    proc->read = sys_proc_show;
#endif

    if (sys_do_mpp_init() != TD_SUCCESS) {
        goto proc_remove;
    }

    OT_PRINT("load sys.ko ....OK!\n");
    return TD_SUCCESS;

proc_remove:
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_remove_proc_entry(PROC_ENTRY_SYS, TD_NULL);
mod_unregister:
#endif
    cmpi_unregister_module(OT_ID_SYS);
dev_deinit:
    sys_device_deinit();
lock_destroy:
    sys_lock_destory();
    OT_PRINT("load sys.ko ....FAILURE!\n");
    return TD_FAILURE;
}

td_void sys_do_mod_exit(td_void)
{
    sys_do_mpp_deinit();

    if (g_sys_ctx.state != SYS_STATE_STOPPED) {
        return;
    }

    cmpi_unregister_module(OT_ID_SYS);
#ifdef CONFIG_OT_PROC_SHOW_SUPPORT
    osal_remove_proc_entry(PROC_ENTRY_SYS, TD_NULL);
#endif
    sys_device_deinit();
    sys_lock_destory();
    OT_PRINT("unload sys.ko ....OK!\n");
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
