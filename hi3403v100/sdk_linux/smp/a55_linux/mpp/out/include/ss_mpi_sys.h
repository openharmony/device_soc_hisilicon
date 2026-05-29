/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SS_MPI_SYS_H__
#define __SS_MPI_SYS_H__

#include "ot_type.h"
#include "ot_common.h"
#include "ot_common_sys.h"
#include "ot_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_log_set_level_cfg(const ot_log_level_cfg *level_cfg);
td_s32 ss_mpi_log_get_level_cfg(ot_log_level_cfg *level_cfg);

td_s32 ss_mpi_log_set_wait_flag(td_bool flag);
td_s32 ss_mpi_log_read(td_char *buf, td_u32 size);
td_void ss_mpi_log_close(td_void);

td_s32 ss_mpi_sys_init(td_void);
td_s32 ss_mpi_sys_exit(td_void);

td_s32 ss_mpi_sys_set_cfg(const ot_mpp_sys_cfg *sys_cfg);
td_s32 ss_mpi_sys_get_cfg(ot_mpp_sys_cfg *sys_cfg);

td_s32 ss_mpi_sys_bind(const ot_mpp_chn *src_chn, const ot_mpp_chn *dst_chn);
td_s32 ss_mpi_sys_unbind(const ot_mpp_chn *src_chn, const ot_mpp_chn *dst_chn);

td_s32 ss_mpi_sys_get_bind_by_dst(const ot_mpp_chn *dst_chn, ot_mpp_chn *src_chn);
td_s32 ss_mpi_sys_get_bind_by_src(const ot_mpp_chn *src_chn, ot_mpp_bind_dst *bind_dst);

td_s32 ss_mpi_sys_get_version(ot_mpp_version *version);

td_s32 ss_mpi_sys_get_cur_pts(td_u64 *cur_pts);
td_s32 ss_mpi_sys_init_pts_base(td_u64 pts_base);
td_s32 ss_mpi_sys_sync_pts(td_u64 pts_base);

td_void *ss_mpi_sys_mmap(td_phys_addr_t phys_addr, td_u32 size);
td_s32 ss_mpi_sys_munmap(const td_void *virt_addr, td_u32 size);
td_void *ss_mpi_sys_mmap_cached(td_phys_addr_t phys_addr, td_u32 size);

td_s32 ss_mpi_sys_close_fd(td_void);

/* alloc mmz memory in user context */
td_s32 ss_mpi_sys_mmz_alloc(td_phys_addr_t *phys_addr, td_void **virt_addr, const td_char *mmb, const td_char *zone,
    td_u32 len);
/* alloc mmz memory with cache */
td_s32 ss_mpi_sys_mmz_alloc_cached(td_phys_addr_t *phys_addr, td_void **virt_addr, const td_char *mmb,
    const td_char *zone, td_u32 len);
/* free mmz memory in user context */
td_s32 ss_mpi_sys_mmz_free(td_phys_addr_t phys_addr, const td_void *virt_addr);

/* flush cache */
td_s32 ss_mpi_sys_flush_cache(td_phys_addr_t phys_addr, td_void *virt_addr, td_u32 size);

/* get virtual meminfo according to virtual addr, should be in one process */
td_s32 ss_mpi_sys_get_virt_mem_info(const void *virt_addr, ot_sys_virt_mem_info *mem_info);

td_s32 ss_mpi_sys_mem_share(const td_void *mem_handle, td_s32 pid);
td_s32 ss_mpi_sys_mem_unshare(const td_void *mem_handle, td_s32 pid);
td_s32 ss_mpi_sys_mem_share_all(const td_void *mem_handle);
td_s32 ss_mpi_sys_mem_unshare_all(const td_void *mem_handle);

td_s32 ss_mpi_sys_get_mem_info_by_virt(const td_void *virt_addr, ot_sys_mem_info *mem_info);
td_s32 ss_mpi_sys_get_mem_info_by_phys(td_phys_addr_t phys_addr, ot_sys_mem_info *mem_info);
td_s32 ss_mpi_sys_get_mem_info_by_handle(const td_void *mem_handle, ot_sys_mem_info *mem_info);

td_s32 ss_mpi_sys_set_mem_cfg(const ot_mpp_chn *mpp_chn, const td_char *mmz_name);
td_s32 ss_mpi_sys_get_mem_cfg(const ot_mpp_chn *mpp_chn, td_char *mmz_name);

td_s32 ss_mpi_sys_set_scale_coef_level(const ot_scale_range *scale_range, const ot_scale_coef_level *scale_coef_level);
td_s32 ss_mpi_sys_get_scale_coef_level(const ot_scale_range *scale_range, ot_scale_coef_level *scale_coef_level);

td_s32 ss_mpi_sys_set_time_zone(td_s32 time_zone);
td_s32 ss_mpi_sys_get_time_zone(td_s32 *time_zone);

td_s32 ss_mpi_sys_set_gps_info(const ot_gps_info *gps_info);
td_s32 ss_mpi_sys_get_gps_info(ot_gps_info *gps_info);

td_s32 ss_mpi_sys_set_vi_vpss_mode(const ot_vi_vpss_mode *vi_vpss_mode);
td_s32 ss_mpi_sys_get_vi_vpss_mode(ot_vi_vpss_mode *vi_vpss_mode);

td_s32 ss_mpi_sys_set_vi_video_mode(ot_vi_video_mode video_mode);
td_s32 ss_mpi_sys_get_vi_video_mode(ot_vi_video_mode *video_mode);

td_s32 ss_mpi_sys_get_chip_id(td_u32 *chip_id);
td_s32 ss_mpi_sys_get_custom_code(td_u32 *custom_code);
td_s32 ss_mpi_sys_get_unique_id(ot_unique_id *unique_id);

td_s32 ss_mpi_sys_set_raw_frame_compress_param(const ot_raw_frame_compress_param *compress_param);
td_s32 ss_mpi_sys_get_raw_frame_compress_param(ot_raw_frame_compress_param *compress_param);

td_s32 ss_mpi_sys_set_tuning_connect(td_s32 connect);
td_s32 ss_mpi_sys_get_tuning_connect(td_s32 *connect);

td_s32 ss_mpi_sys_set_schedule_mode(const ot_schedule_mode *schedule_mode);
td_s32 ss_mpi_sys_get_schedule_mode(ot_schedule_mode *schedule_mode);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SS_MPI_SYS_H__ */
