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

#ifndef SYS_EXT_H
#define SYS_EXT_H

#include "ot_type.h"
#include "mod_ext.h"
#include "ot_common_video.h"
#include "ot_common_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define SYS_DDR_MAXNUM                16

#define SYS_SEND_DATA_BLOCK_MASK      0x1
#define SYS_SEND_DATA_BLOCK_OFFSET    0
#define SYS_SEND_DATA_NOBLOCK         ((0 << SYS_SEND_DATA_BLOCK_OFFSET) & SYS_SEND_DATA_BLOCK_MASK)
#define SYS_SEND_DATA_BLOCK           ((1 << SYS_SEND_DATA_BLOCK_OFFSET) & SYS_SEND_DATA_BLOCK_MASK)

#define SYS_SEND_DATA_DELAY_MASK      0x6
#define SYS_SEND_DATA_DELAY_OFFSET    1
#define SYS_SEND_DATA_FINISH          ((0 << SYS_SEND_DATA_DELAY_OFFSET) & SYS_SEND_DATA_DELAY_MASK)
#define SYS_SEND_DATA_LOWDELAY        ((1 << SYS_SEND_DATA_DELAY_OFFSET) & SYS_SEND_DATA_DELAY_MASK)
#define SYS_SEND_DATA_LOWDELAY_FINISH ((2 << SYS_SEND_DATA_DELAY_OFFSET) & SYS_SEND_DATA_DELAY_MASK)

typedef struct {
    td_u32 num;
    td_void *mmz_name[SYS_DDR_MAXNUM];
} mpp_sys_ddr_name;

typedef enum {
    MPP_DATA_VI_FRAME,
    MPP_DATA_VOU_FRAME,
    MPP_DATA_VDEC_FRAME,
    MPP_DATA_VIDEO_FRAME,
    MPP_DATA_VOU_WBC_FRAME,
    MPP_DATA_AUDIO_FRAME,
    MPP_DATA_VPSS_FRAME,
    MPP_DATA_DPU_RECT_FRAME,
    MPP_DATA_AVS_FRAME,
    MPP_DATA_MCF_FRAME,
    MPP_DATA_VENC_STREAM,
    MPP_DATA_UVC_FRAME,
    MPP_DATA_BUTT
} mpp_data_type;

typedef struct {
    ot_mod_id mod_id;
    td_u32 max_dev_cnt;
    td_u32 max_chn_cnt;
    td_s32 (*give_bind_call_back)(td_s32 dev_id, td_s32 chn_id, ot_mpp_bind_dst *bind_send);
} bind_sender_info;

typedef struct {
    ot_mod_id mod_id;
    td_u32 max_dev_cnt;
    td_u32 max_chn_cnt;
    td_s32 (*call_back)(td_s32 dev_id, td_s32 chn_id, td_bool block, mpp_data_type data_type, td_void *pv_data);
    td_s32 (*reset_call_back)(td_s32 dev_id, td_s32 chn_id, td_void *pv_data);
    td_bool support_delay_data;
} bind_receiver_info;

typedef struct {
    ot_mod_id mod_id;
    ot_pixel_format pixel_format;
    td_u32 width;
    td_u32 height;
    td_u32 buf_line;
    td_u32 align;  /* 0:auto */
    ot_dynamic_range dynamic_range;
    ot_compress_mode compress_mode;
    ot_video_format video_format;
    td_bool is_3dnr_buffer;
} vb_base_info;

typedef struct {
    ot_pixel_format pixel_format;
    td_u32 image_width;
    td_u32 image_height;
    td_u32 head_vi_width;
    td_u32 head_vi_height;
    td_u32 head_vpss_width;
    td_u32 head_vpss_height;
    td_u32 buf_line;
    td_u32 align;  /* 0:auto */
    ot_dynamic_range dynamic_range;
    ot_compress_mode compress_mode;
    ot_video_format video_format;
    td_bool is_3dnr_buffer;
} vb_share_base_info;

typedef struct {
    td_u32 vb_size;

    td_u32 head_vi_stride;
    td_u32 head_vi_size;
    td_u32 head_vi_y_size;

    td_u32 head_vpss_stride;
    td_u32 head_vpss_size;
    td_u32 head_vpss_y_size;

    td_u32 main_stride;
    td_u32 main_size;
    td_u32 main_y_size;
} vb_share_calc_cfg;

typedef enum {
    SYS_AI_SAMPLECLK_DIVSEL,
    SYS_AI_BITCKL_DIVSEL,
    SYS_AI_SYSCKL_SEL,
    SYS_AI_MASTER,
    SYS_AI_RESET_SEL,
    SYS_AI_CLK_EN,

    SYS_AO_SAMPLECLK_DIVSEL,
    SYS_AO_BITCKL_DIVSEL,
    SYS_AO_SYSCKL_SEL,
    SYS_AO_MASTER,
    SYS_AO_RESET_SEL,
    SYS_AO_CLK_EN,

    SYS_AIO_RESET_SEL,
    SYS_AIO_CLK_EN,
    SYS_AIO_CLK_SEL,

    SYS_AIO_RX0SD_SEL,
    SYS_AIO_HDMISD_SEL,
    SYS_AIO_PADSD_SEL,
    SYS_AIO_CODECSD_SEL,
    SYS_AIO_HDMICLK_SEL,
    SYS_AIO_PADCLK_SEL,
    SYS_AIO_CODECCLK_SEL,

    SYS_ACODEC_RESET_SEL,

    SYS_VI_PT_INPUT_SEL,
    SYS_VI_RESET_SEL,
    SYS_VI_BUS_CLK_EN,
    SYS_VI_PPC_CLK_EN,
    SYS_VI_DEV_RESET_SEL,
    SYS_VI_DEV_CLK_EN,
    SYS_VI_CMOS_CLK_DOUBLE,
    SYS_VI_CMOS_CLK_EN,
    SYS_VI_CMOS_RESET_SEL,
    SYS_VI_APB_CLK_EN,
    SYS_VI_DLL_EN,
    SYS_VI_THERMO_WORK_MODE_SEL,
    SYS_VI_IR_CLK_GET,
    SYS_VI_BUS_RESET,
    SYS_VI_PPC_CLK_GET,
    SYS_VI_PPC_RESET,
    SYS_VI_PPC_PT_CLK_EN,
    SYS_VI_PPC_CHN_CLK_EN,
    SYS_VI_ISP_CLK_EN,
    SYS_VI_ISP_CORE_RESET,
    SYS_VI_ISP_CFG_RESET,
    SYS_VI_PT_CLK_EN,
    SYS_VI_PT_RESET,
    SYS_VI_PROC_CLK_GET,
    SYS_VI_PROC_PRE_CLK_EN,
    SYS_VI_PROC_PRE_RESET,
    SYS_VI_PROC_POST_CLK_SEL,
    SYS_VI_PROC_POST_CLK_EN,
    SYS_VI_PROC_POST_RESET,
    SYS_VI_CAP_PROC_PRE_ONLINE_SEL,
    SYS_VI_PROC_PRE_POST_ONLINE_SEL,
    SYS_VI_PROC_POST_VPSS_ONLINE_SEL,
    SYS_VI_SYS_CNT_GET,

    SYS_VO_PLL_POSTDIV2_SET,
    SYS_VO_PLL_POSTDIV1_SET,
    SYS_VO_PLL_FRAC_SET,
    SYS_VO_PLL_REFDIV_SET,
    SYS_VO_PLL_FBDIV_SET,
    SYS_VO_PLL_POWER_CTRL,
    SYS_VO_BUS_RESET_SEL,
    SYS_VO_CFG_CLK_EN,
    SYS_VO_APB_CLK_EN,
    SYS_VO_BUS_CLK_EN,
    SYS_VO_HD_CLKOUT_PHASIC_REVERSE_EN,
    SYS_VO_OUT_DLY_TUNE,
    SYS_VO_OUT_HD_DIV,
    SYS_VO_DEV_DIV_MOD,
    SYS_VO_PPC_CFG_CLK_SEL,
    SYS_VO_PPC_CLK_SEL,
    SYS_VO_PPC_SC_CLK_SEL,
    SYS_VO_HD_CLK_SEL,
    SYS_VO_CORE_CLK_EN,
    SYS_VO_DEV_CLK_EN,
    SYS_VO_DEV_DATE_CLK_EN,
    SYS_VO_SD_DATE_CLK_EN,
    SYS_VO_SD_CLK_SEL,
    SYS_VO_LCD_CLK_EN,
    SYS_VO_LCD_MCLK_DIV,
    SYS_VO_BT1120_CLK_EN,
    SYS_VO_BT1120_CLK_SEL,
    SYS_VO_HD_DAC_SEL,
    SYS_VO_DEV_DAC_EN,
    SYS_VO_DEV_DAC_DLY_TUNE,
    SYS_VO_HDMI_SSC_VDP_DIV,
    SYS_VO_HDMI_CLK_EN,
    SYS_VO_HDMI_CLK_SEL,
    SYS_VO_HDMI1_CLK_EN,
    SYS_VO_HDMI1_CLK_SEL,
    SYS_VO_MIPI_CLK_EN,
    SYS_VO_MIPI_CLK_SEL,

    SYS_VEDU_RESET_SEL,
    SYS_VEDU_CLK_EN,
    SYS_VEDU_CLK_SEL,
    SYS_VEDU_SED_RESET_SEL,
    SYS_VEDU_PWR_EN,
    SYS_VEDU_PWR_ISO,
    SYS_VEDU_PWR_STATE,

    SYS_VPSS_CLK_GET,
    SYS_VPSS_CLK_SEL,
    SYS_VPSS_RESET_SEL,
    SYS_VPSS_CLK_EN,
    SYS_VPSS_GET_CLK_SEL,
    SYS_VPSS_SET_CLK_SEL,
    SYS_AVS_RESET_SEL,
    SYS_AVS_CLK_EN,
    SYS_AVS_PWR_EN,
    SYS_AVS_PWR_ISO,
    SYS_AVS_PWR_STATE,

    SYS_TDE_RESET_SEL,
    SYS_TDE_CLK_EN,

    SYS_JPGE_RESET_SEL,
    SYS_JPGE_CLK_EN,
    SYS_JPGE_CLK_SEL,

    SYS_JPGD_CLOCK_SEL,
    SYS_JPGD_RESET_SEL,
    SYS_JPGD_CLK_EN,

    SYS_MD_RESET_SEL,
    SYS_MD_CLK_EN,
    SYS_MD_CLK_SEL,

    SYS_IVE_RESET_SEL,
    SYS_IVE_CLK_EN,
    SYS_IVE_CLK_SET,
    SYS_IVE_CLK_GET,
    SYS_IVE_SRC_CLK_EN,

    SYS_FD_RESET_SEL,
    SYS_FD_CLK_EN,

    SYS_WK_CNN_RESET_SEL,
    SYS_WK_CNN_CLK_EN,
    SYS_WK_CNN_CLK_SET,
    SYS_WK_CNN_CLK_GET,

    SYS_MAU_CLK_SET,
    SYS_MAU_RESET_SEL,
    SYS_MAU_CLK_EN,
    SYS_MAU_CLK_GET,

    SYS_DPU_CLK_SET,
    SYS_DPU_RESET_SEL,
    SYS_DPU_CLK_EN,

    SYS_PQP_RESET_SEL,
    SYS_PQP_CLK_EN,
    SYS_PQP_CLK_SET,
    SYS_PQP_CLK_GET,

    SYS_SVP_NPU0_RESET_SEL,
    SYS_SVP_NPU0_CLK_EN,
    SYS_SVP_NPU0_CLK_SET,
    SYS_SVP_NPU0_CLK_GET,

    SYS_PCIE_RESET_SEL,
    SYS_PCIE_CLK_EN,

    SYS_CIPHER_RESET_SEL,
    SYS_CIPHER_CLK_EN,

    SYS_VGS_RESET_SEL,
    SYS_VGS_CLK_EN,
    SYS_VGS_GET_CLK_SEL,
    SYS_VGS_SET_CLK_SEL,
    SYS_VGS_BOOTROOM_RAM_USE,

    SYS_GDC_RESET_SEL,
    SYS_GDC_CLK_EN,
    SYS_GDC_GET_CLK_STATE,
    SYS_GDC_NNIE_RAM_USE,
    SYS_GDC_GET_CLKSEL,
    SYS_GDC_SET_CLKSEL,

    SYS_GDC_VGS_PWR_EN,
    SYS_GDC_VGS_PWR_ISO,
    SYS_GDC_VGS_PWR_STATE,

    SYS_VPSS_PWR_EN,
    SYS_VPSS_PWR_ISO,
    SYS_VPSS_PWR_STATE,

    SYS_DIS_RESET_SEL,
    SYS_DIS_CLK_EN,

    SYS_GDC_NNIE_MUTEX_SEL,
    SYS_VENC_NNIE_MUTEX_SEL,
    SYS_NNIE_GET_MUTEX_STATE,
    SYS_NNIE_GDC_GET_MUTEX_STATE,

    SYS_GET_SYS,
    SYS_GET_SCT,

    SYS_KCF0_RESET_SEL,
    SYS_KCF0_CLK_EN,
    SYS_KCF0_CLK_SET,
    SYS_KCF0_CLK_GET,

    SYS_KCF1_RESET_SEL,
    SYS_KCF1_CLK_EN,
    SYS_KCF1_CLK_SET,
    SYS_KCF1_CLK_GET,

    SYS_DSP_CLK_SET,
    SYS_DSP_CORE_RESET_SEL,
    SYS_DSP_CFG_RESET_SEL,
    SYS_DSP_DBG_RESET_SEL,
    SYS_DSP_JTAG_RESET_SEL,
    SYS_DSP_CLK_EN,

    SYS_NPU_SRST_RESET,
    SYS_NPU_DIV_SRST_RESET,
    SYS_NPU_CLK_EN,
    SYS_NPU_CLK_SEL,
    SYS_NPU_CLK_GET,
    SYS_NPU_SW_BYPASS_CKSEL,
    SYS_NPU_CLK_SDIV,
    SYS_NPU_CPM_CKEN,
    SYS_CHECK_APLL_CLK,
    SYS_VENC_GET_APLL_CLK,

    SYS_HDMI0_CTRL_RESET_SEL,
    SYS_HDMI0_PHY_RESET_SEL,
    SYS_HDMI1_CTRL_RESET_SEL,
    SYS_HDMI1_PHY_RESET_SEL,

    SYS_VI_CHN_RESET,

    SYS_BUTT,
} sys_func;

#ifdef CONFIG_OT_TZASC_SUPPORT
typedef enum {
    NON_TEE_VERSION = 0,
    TEE_VERSION = 1,
} sys_security_version;
#endif

typedef struct {
    td_u64 (*pfn_sys_get_time_stamp)(td_void);
    td_u64 (*pfn_sys_get_local_time_stamp)(td_void);
    td_void (*pfn_sys_sync_time_stamp)(td_u64 base, td_bool init);
    td_s32 (*pfn_sys_get_stride)(td_u32 width, td_u32 *stride);
    td_s32 (*pfn_sys_drv_ioctrl)(const ot_mpp_chn *mpp_chn, sys_func func_id, td_void *io_args);

    td_s32 (*pfn_sys_register_sender)(const bind_sender_info *info);
    td_s32 (*pfn_sys_unregister_sender)(ot_mod_id mod_id);
    td_s32 (*pfn_sys_register_receiver)(const bind_receiver_info *info);
    td_s32 (*pfn_sys_unregister_receiver)(ot_mod_id mod_id);
    td_s32 (*pfn_sys_send_data)(ot_mpp_chn *bind_chn, td_u32 flag, mpp_data_type data_type, td_void *pv_data);
    td_s32 (*pfn_sys_reset_data)(ot_mod_id mod_id, td_s32 dev_id, td_s32 chn_id, td_void *private);

    td_s32 (*pfn_get_bind_by_src)(const ot_mpp_chn *src_chn, ot_mpp_bind_dst *bind_src);
    td_s32 (*pfn_get_bind_num_by_src)(const ot_mpp_chn *src_chn, td_u32 *bind_num);
    td_s32 (*pfn_get_bind_by_dest)(const ot_mpp_chn *dest_chn, ot_mpp_chn *src_chn);
    td_s32 (*pfn_get_mmz_name)(const ot_mpp_chn *chn, td_void **pp_mmz_name);
    td_s32 (*pfn_get_mem_ddr)(const ot_mpp_chn *chn, mpp_sys_ddr_name *mem_ddr);
    td_s32 (*pfn_get_spin_rec)(td_u32 *spin_rec);

    td_u32 (*pfn_sys_vreg_read)(td_u32 addr, td_u32 bytes);
    td_s32 (*pfn_sys_vreg_write)(td_u32 addr, td_u32 value, td_u32 bytes);
    td_u32 (*pfn_sys_get_vreg_addr)(td_void);
    td_u32 (*pfn_sys_get_scale_coef)(ot_mod_id mod_id, const td_void *opt, const td_void *rate, td_void *cfg);
    td_u64 (*pfn_sys_get_sched_clock)(td_void);
    td_s32 (*pfn_sys_get_full_chip_id)(td_u32 *chip_id);

    td_u32 (*pfn_sys_get_chip_id)(td_void);
    td_phys_addr_t (*pfn_sys_get_efuse_addr)(td_void);
    td_phys_addr_t (*pfn_sys_get_sys_base_addr)(td_void);

    td_s32 (*pfn_sys_get_time_zone)(td_s32 *time_zone);
    td_s32 (*pfn_sys_get_now_string)(td_u8 *now_time);
    ot_gps_info *(*pfn_sys_get_gps_info)(td_void);

    td_void (*pfn_sys_get_vb_cfg)(const td_void *vb_base_info, td_void *vb_cal_config);
    td_void (*pfn_sys_get_share_vb_cfg)(const td_void *vb_base_info, td_void *vb_cal_config, td_bool is_share_head);

    td_void (*pfn_sys_get_cmp_seg_cfg)(const td_void *sys_cmp_in, td_void *y_cmp_cfg, td_void *cmp_cfg);
    td_void (*pfn_sys_get_cmp_bayer_cfg)(const td_void *sys_cmp_in, td_void *bayer_cmp_cfg,
        const td_void *bayer_dcmp_cfg);
    td_void (*pfn_sys_get_cmp_bnr_cfg)(const td_void *sys_cmp_in, td_void *bnr_cmp_cfg);
    td_void (*pfn_sys_get_cmp_3dnr_cfg)(const td_void *sys_cmp_in, td_void *y_cmp_cfg, td_void *cmp_cfg);
    td_void (*pfn_sys_get_cmp_tile16x8_cfg)(const td_void *sys_cmp_in, td_void *cmp_out);

    td_void (*pfn_sys_get_dci_coef) (td_phys_addr_t *phy_addr);

    td_s32 (*pfn_sys_print_proc_title)(osal_proc_entry_t *s, const td_char *format, ...) \
        __attribute__((format(printf, 2, 3)));

    td_bool (*pfn_sys_is_tee_enable_otp)(td_void);

#ifdef CONFIG_OT_TZASC_SUPPORT
    td_s32 (*pfn_sys_get_security_version)(sys_security_version *version);
#endif
#ifdef CONFIG_OT_VI_PTS
    td_u64 (*pfn_sys_get_vi_time_stamp)(td_s32 dev_id);
#endif
#ifdef CONFIG_OT_SYS_MUTEX_REG_SUPPORT
    td_s32 (*pfn_sys_get_mutex)(td_u32 vaule);
    td_void (*pfn_sys_release_mutex)(td_u32 vaule);
#endif
} sys_export_func;

#define ckfn_sys_entry() check_func_entry(OT_ID_SYS)

#define ckfn_sys_get_sched_clock() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_sched_clock != TD_NULL)
#define call_sys_get_sched_clock() \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_sched_clock()

#define ckfn_sys_get_time_stamp() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_time_stamp != TD_NULL)
#define call_sys_get_time_stamp() \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_time_stamp()

#ifdef CONFIG_OT_VI_PTS
#define ckfn_sys_get_vi_time_stamp() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_vi_time_stamp != TD_NULL)
#define call_sys_get_vi_time_stamp(dev_id) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_vi_time_stamp(dev_id)
#endif

#ifdef CONFIG_OT_SYS_MUTEX_REG_SUPPORT
#define ckfn_pfn_sys_get_mutex() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_mutex != TD_NULL)
#define call_pfn_sys_get_mutex(vaule) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_mutex(vaule)

#define ckfn_pfn_sys_release_mutex() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_release_mutex != TD_NULL)
#define call_pfn_sys_release_mutex(vaule) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_release_mutex(vaule)
#endif

#define ckfn_sys_get_local_time_stamp() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_local_time_stamp != TD_NULL)
#define call_sys_get_local_time_stamp() \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_local_time_stamp()

#define ckfn_sys_sync_time_stamp() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_sync_time_stamp != TD_NULL)
#define call_sys_sync_time_stamp(base, init) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_sync_time_stamp(base, init)

#define ckfn_sys_drv_ioctrl() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_drv_ioctrl != TD_NULL)
#define call_sys_drv_ioctrl(mpp_chn, func_id, io_args) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_drv_ioctrl(mpp_chn, func_id, io_args)

#define ckfn_sys_register_sender() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_register_sender != TD_NULL)
#define call_sys_register_sender(info) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_register_sender(info)

#define ckfn_sys_unregister_sender() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_unregister_sender != TD_NULL)
#define call_sys_unregister_sender(mod_id) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_unregister_sender(mod_id)

#define ckfn_sys_register_receiver() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_register_receiver != TD_NULL)
#define call_sys_register_receiver(info) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_register_receiver(info)

#define ckfn_sys_unregister_receiver() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_unregister_receiver != TD_NULL)
#define call_sys_unregister_receiver(mod_id) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_unregister_receiver(mod_id)

#define ckfn_sys_send_data() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_send_data != TD_NULL)
#define call_sys_send_data(bind_chn, flag, data_type, pv_data) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_send_data(bind_chn, flag, data_type, pv_data)

#define ckfn_sys_reset_data() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_reset_data != TD_NULL)
#define call_sys_reset_data(mod_id, dev_id, chn_id, private) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_reset_data(mod_id, dev_id, chn_id, private)

#define ckfn_sys_get_bind_by_src() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_get_bind_by_src != TD_NULL)
#define call_sys_get_bind_by_src(src_chn, bind_dest) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_get_bind_by_src(src_chn, bind_dest)

#define ckfn_sys_get_bind_num_by_src() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_get_bind_num_by_src != TD_NULL)
#define call_sys_get_bind_num_by_src(src_chn, bind_num) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_get_bind_num_by_src(src_chn, bind_num)

#define ckfn_sys_get_bind_by_dest() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_get_bind_by_dest != TD_NULL)
#define call_sys_get_bind_by_dest(dest_chn, src_chn) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_get_bind_by_dest(dest_chn, src_chn)

#define ckfn_sys_get_mmz_name() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_get_mmz_name != TD_NULL)
#define call_sys_get_mmz_name(src_chn, pp_mmz_name) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_get_mmz_name(src_chn, pp_mmz_name)

#define call_sys_get_efuse_addr() \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_efuse_addr()
#define ckfn_sys_get_sys_base_addr() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_sys_base_addr != TD_NULL)
#define call_sys_get_sys_base_addr() \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_sys_base_addr()

#define ckfn_sys_print_proc_title() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_print_proc_title != TD_NULL)
#define call_sys_print_proc_title(s, format, args...) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_print_proc_title(s, format, ##args)

#define ckfn_sys_is_tee_enable_otp() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_is_tee_enable_otp != TD_NULL)
#define call_sys_is_tee_enable_otp() \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_is_tee_enable_otp()

#define ckfn_sys_get_vb_cfg() \
    (func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_vb_cfg != TD_NULL)
#define call_sys_get_vb_cfg(vb_base_info, vb_cal_config) \
    func_entry(sys_export_func, OT_ID_SYS)->pfn_sys_get_vb_cfg(vb_base_info, vb_cal_config)

#define call_sys_set_mpp_chn(mpp_chn, mod, dev, chn) \
    do {                                             \
        (mpp_chn)->mod_id = (mod);                   \
        (mpp_chn)->dev_id = (dev);                   \
        (mpp_chn)->chn_id = (chn);                   \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* SYS_EXT_H */
