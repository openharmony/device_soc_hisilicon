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

#ifndef MKP_SYS_H
#define MKP_SYS_H

#include "mkp_ioctl.h"
#include "ot_common.h"
#include "ot_common_sys.h"
#include "ot_common_video.h"
#include "sys_ext.h"
#include "ot_inner_sys_ipc.h"
#include "osal_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define sys_check_null_ptr_return(ptr)        \
    do {                                      \
        if ((ptr) == TD_NULL) {               \
            sys_err_trace("Null pointer \n"); \
            return OT_ERR_SYS_NULL_PTR;       \
        }                                     \
    } while (0)

typedef struct {
    ot_mpp_chn src_chn;
    ot_mpp_chn dest_chn;
} sys_bind_args;

typedef struct {
    ot_mpp_chn src_chn;
    ot_mpp_bind_dst dest_chns;
} sys_bind_src_args;

typedef struct {
    ot_mpp_chn mpp_chn;
    td_char mmz_name[OT_MAX_MMZ_NAME_LEN];
} sys_mem_args;

typedef struct {
    td_s32 hr_timer;
    td_s32 rr_mode;
} kernel_config;

typedef struct {
    td_u32 size;
    td_phys_addr_t phy_addr;
    td_void ATTRIBUTE *vir_addr;
} sys_mem_cache_info;

/* 数据目标记录的绑定关闭 */
typedef struct {
    td_bool is_bind_src; /* 此目标是否有绑定源 */
    ot_mpp_chn bind_src; /* 绑定的源通道 */
    td_u32 send_cnt;
    td_u32 reset_cnt;
} sys_bind_dest;

typedef enum {
    SYS_BINDER_SENDER,
    SYS_BINDER_RECEIVER,
    SYS_BINDER_BUTT
} sys_binder_type;

typedef enum {
    IOC_NR_SYS_INIT = 0,
    IOC_NR_SYS_EXIT,
    IOC_NR_SYS_SETCONFIG,
    IOC_NR_SYS_GETCONFIG,
    IOC_NR_SYS_INITPTSBASE,
    IOC_NR_SYS_SYNCPTS,
    IOC_NR_SYS_GETCURPTS,

    IOC_NR_SYS_BIND,
    IOC_NR_SYS_UNBIND,
    IOC_NR_SYS_GETBINDBYDEST,
    IOC_NR_SYS_GETBINDBYSRC,

    IOC_NR_SYS_MEM_SET,
    IOC_NR_SYS_MEM_GET,

    IOC_NR_SYS_GET_CUST_CODE,

    IOC_NR_SYS_GET_KERNELCONFIG,

    IOC_NR_SYS_GET_CHIPID,

    IOC_NR_SYS_SET_VI_VPSS_MODE,
    IOC_NR_SYS_GET_VI_VPSS_MODE,

    IOC_NR_SYS_SET_VI_VIDEO_MODE,
    IOC_NR_SYS_GET_VI_VIDEO_MODE,

    IOC_NR_SYS_GET_UNIQUE_ID,

    IOC_NR_SYS_SET_TUNING_CONNECT,
    IOC_NR_SYS_GET_TUNING_CONNECT,

    IOC_NR_SYS_MFLUSH_CACHE,

    IOC_NR_SYS_SET_SCALE_COEFF,
    IOC_NR_SYS_GET_SCALE_COEFF,

    IOC_NR_SYS_SET_TIME_ZONE,
    IOC_NR_SYS_GET_TIME_ZONE,

    IOC_NR_SYS_SET_GPS_INFO,
    IOC_NR_SYS_GET_GPS_INFO,

    IOC_NR_SYS_SET_RAW_FRAME_COMPRESS_RATE,
    IOC_NR_SYS_GET_RAW_FRAME_COMPRESS_RATE,

    IOC_NR_SYS_SET_SCHEDULE_MODE,
    IOC_NR_SYS_GET_SCHEDULE_MODE,

#ifdef OT_DEBUG
    IOC_NR_SYS_SET_COMPRESSV2_RATE,
    IOC_NR_SYS_GET_COMPRESSV2_RATE,
#endif
} ioc_nr_sys;

#define SYS_INIT_CTRL                   OSAL_IO(IOC_TYPE_SYS, IOC_NR_SYS_INIT)
#define SYS_EXIT_CTRL                   OSAL_IO(IOC_TYPE_SYS, IOC_NR_SYS_EXIT)
#define SYS_SET_CONFIG_CTRL             OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SETCONFIG, ot_mpp_sys_cfg)
#define SYS_GET_CONFIG_CTRL             OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GETCONFIG, ot_mpp_sys_cfg)
#define SYS_INIT_PTSBASE                OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_INITPTSBASE, td_u64)
#define SYS_SYNC_PTS                    OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SYNCPTS, td_u64)
#define SYS_GET_CURPTS                  OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GETCURPTS, td_u64)

#define SYS_BIND_CTRL                   OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_BIND, sys_bind_args)
#define SYS_UNBIND_CTRL                 OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_UNBIND, sys_bind_args)
#define SYS_GETBINDBYDEST               OSAL_IOWR(IOC_TYPE_SYS, IOC_NR_SYS_GETBINDBYDEST, sys_bind_args)
#define SYS_GETBINDBYSRC                OSAL_IOWR(IOC_TYPE_SYS, IOC_NR_SYS_GETBINDBYSRC, sys_bind_src_args)

#define SYS_MEM_SET_CTRL                OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_MEM_SET, sys_mem_args)
#define SYS_MEM_GET_CTRL                OSAL_IOWR(IOC_TYPE_SYS, IOC_NR_SYS_MEM_GET, sys_mem_args)

#define SYS_GET_CUST_CODE               OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_CUST_CODE, td_u32)

#define SYS_GET_KERNELCONFIG            OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_KERNELCONFIG, kernel_config)

#define SYS_GET_CHIPID                  OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_CHIPID, td_u32)

#define SYS_SET_TUNING_CONNECT          OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_TUNING_CONNECT, td_s32)
#define SYS_GET_TUNING_CONNECT          OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_TUNING_CONNECT, td_s32)

#define SYS_MFLUSH_CACHE                OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_MFLUSH_CACHE, sys_mem_cache_info)

#define SYS_SET_SCALE_COEFF             OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_SCALE_COEFF, ot_scale_coef_info)
#define SYS_GET_SCALE_COEFF             OSAL_IOWR(IOC_TYPE_SYS, IOC_NR_SYS_GET_SCALE_COEFF, ot_scale_coef_info)

#define SYS_SET_TIME_ZONE               OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_TIME_ZONE, td_s32)
#define SYS_GET_TIME_ZONE               OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_TIME_ZONE, td_s32)

#define SYS_SET_GPS_INFO                OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_GPS_INFO, ot_gps_info)
#define SYS_GET_GPS_INFO                OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_GPS_INFO, ot_gps_info)

#define SYS_SET_VI_VPSS_MODE            OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_VI_VPSS_MODE, ot_vi_vpss_mode)
#define SYS_GET_VI_VPSS_MODE            OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_VI_VPSS_MODE, ot_vi_vpss_mode)

#define SYS_SET_VI_VIDEO_MODE           OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_VI_VIDEO_MODE, ot_vi_video_mode)
#define SYS_GET_VI_VIDEO_MODE           OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_VI_VIDEO_MODE, ot_vi_video_mode)

#define SYS_GET_UNIQUE_ID               OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_UNIQUE_ID, ot_unique_id)

#define SYS_SET_RAW_FRAME_COMPRESS_RATE OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_RAW_FRAME_COMPRESS_RATE, \
    ot_raw_frame_compress_param)
#define SYS_GET_RAW_FRAME_COMPRESS_RATE OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_RAW_FRAME_COMPRESS_RATE, \
    ot_raw_frame_compress_param)
#define SYS_SET_SCHEDULE_MODE           OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_SCHEDULE_MODE, ot_schedule_mode)
#define SYS_GET_SCHEDULE_MODE           OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_SCHEDULE_MODE, ot_schedule_mode)

#ifdef OT_DEBUG
#define SYS_SET_COMPRESSV2_RATE         OSAL_IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_COMPRESSV2_RATE, sys_compress_v2_param)
#define SYS_GET_COMPRESSV2_RATE         OSAL_IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_COMPRESSV2_RATE, sys_compress_v2_param)
#endif

/*
 * cur sender:VIU,VOU,VDEC,VPSS,AI
 * cur receive:VOU,VPSS,GRP,AO
 */
#ifndef CONFIG_OT_VI_ALL_SUPPORT
#define bind_adjust_src_dev_id(mod_id, dev_id)                    \
    do {                                                          \
        if (((mod_id) == OT_ID_VI) || ((mod_id) == OT_ID_VDEC)) { \
            (dev_id) = 0;                                         \
        }                                                         \
    } while (0)
#else
#define bind_adjust_src_dev_id(mod_id, dev_id) \
    do {                                       \
        if ((mod_id) == OT_ID_VDEC) {          \
            (dev_id) = 0;                      \
        }                                      \
    } while (0)
#endif

#define bind_adjust_src_chn_id(mod_id, chn_id) \
    do {                                       \
        if ((mod_id) == OT_ID_VO) {            \
            (chn_id) = 0;                      \
        }                                      \
    } while (0)

#define bind_adjust_dest_chn_id(mod_id, chn_id) \
    do {                                        \
        if ((mod_id) == OT_ID_VPSS) {           \
            (chn_id) = 0;                       \
        }                                       \
    } while (0)

#define sys_emerg_trace(fmt, ...) \
    OT_EMERG_TRACE(OT_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define sys_alert_trace(fmt, ...) \
    OT_ALERT_TRACE(OT_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define sys_crit_trace(fmt, ...) \
    OT_CRIT_TRACE(OT_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define sys_err_trace(fmt, ...) \
    OT_ERR_TRACE(OT_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define sys_warn_trace(fmt, ...) \
    OT_WARN_TRACE(OT_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define sys_notice_trace(fmt, ...) \
    OT_NOTICE_TRACE(OT_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define sys_info_trace(fmt, ...) \
    OT_INFO_TRACE(OT_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define sys_debug_trace(fmt, ...) \
    OT_DEBUG_TRACE(OT_ID_SYS, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

td_s32 sys_bind(const ot_mpp_chn *bind_src, const ot_mpp_chn *bind_dest);
td_s32 sys_unbind(const ot_mpp_chn *bind_src, const ot_mpp_chn *bind_dest);
td_s32 sys_get_bind_by_dest(const ot_mpp_chn *dest_chn, ot_mpp_chn *src_chn, td_bool inner_call);

td_s32 sys_get_bind_num_by_src(const ot_mpp_chn *src_chn, td_u32 *bind_num);

td_s32 sys_get_bind_by_dest_inner(const ot_mpp_chn *dest_chn, ot_mpp_chn *src_chn);
td_s32 sys_get_bind_by_src(const ot_mpp_chn *src_chn, ot_mpp_bind_dst *bind_src);

td_s32 sys_bind_register_sender(const bind_sender_info *info);
td_s32 sys_bind_unregister_sender(ot_mod_id mod_id);
td_s32 sys_bind_send_data(ot_mpp_chn *bind_chn, td_u32 flag, mpp_data_type data_type, td_void *v_data);

td_s32 sys_bind_reset_data(ot_mod_id mod_id, td_s32 dev_id, td_s32 chn_id, td_void *v_data);

td_s32 sys_bind_register_receiver(const bind_receiver_info *info);
td_s32 sys_bind_unregister_receiver(ot_mod_id mod_id);

td_s32 sys_bind_init(td_void);
td_void sys_bind_exit(td_void);

td_s32 sys_bind_mod_init(td_void);
td_void sys_bind_mod_exit(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* MKP_SYS_H */
