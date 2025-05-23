/*
 * Copyright (C) 2021 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef __AUTOCONF_H__
#define __AUTOCONF_H__

#define AUTOCONF_TIMESTAMP "2021-04-03 13:57:17 UTC"

/*
 * General Setup
 */
#define CONFIG_HI3516CV500 1
#define CONFIG_HI_CHIP_TYPE 0x3516C500
#define CONFIG_HI_ARCH "hi3516cv500"
#define CONFIG_HI_SUBARCH "hi3516cv500"
#define CONFIG_HI_SUBCHIP_TYPE 0x3516C500
#define CONFIG_HI_SUBARCH "hi3516cv500"
#define CONFIG_HI_SUBCHIP_TYPE 0x3516C500
#define CONFIG_SUBCHIP_HI3516CV500 1
#define CONFIG_HI_SUBARCH "hi3516cv500"
#define CONFIG_HI_SUBCHIP_TYPE 0x3516C500
#define CONFIG_SMP 1
#define CONFIG_ARM_ARCH_TYPE "smp"
#define CONFIG_A7 1
#define CONFIG_CPU_TYPE "a7"
#define CONFIG_VERSION_ASIC 1
#define CONFIG_HI_FPGA "n"
#define CONFIG_OHOS_OS 1
#define CONFIG_OS_TYPE "OHOS"
#define CONFIG_KERNEL_VERSION "OHOS"
#define CONFIG_KERNEL_LLVM 1
#define CONFIG_HI_CROSS "llvm-"
#define CONFIG_LIBC_TYPE "musl_llvm"
#define CONFIG_KERNEL_BIT "KERNEL_BIT_32"
#define CONFIG_USER_LLVM 1
#define CONFIG_HI_CROSS_LIB "llvm-"
#define CONFIG_USER_BIT "USER_BIT_32"
#define CONFIG_RELEASE_TYPE_RELEASE 1
#define CONFIG_HI_RLS_MODE "HI_RELEASE"
#define CONFIG_CUSTOMER_VERSION_COMMON 1
#define CONFIG_CUSTOMER_VERSION "COMMON"
#define CONFIG_VERSION_NUMBER "1.0.0.1.10"

/*
 * Media Modules Setup
 */

/*
 * media base config
 */

/*
 * media sys config
 */
#define CONFIG_HI_SYS_SUPPORT 1
#define CONFIG_HI_SYS_SCALE_COEF_SUPPORT 1

/*
 * media vi config
 */
#define CONFIG_HI_VI_SUPPORT 1
#define CONFIG_HI_VI_BT656 1
#define CONFIG_HI_VI_MIPI 1
#define CONFIG_HI_VI_LDCV1_SUPPORT 1
#define CONFIG_HI_VI_LDCV2_SUPPORT 1
#define CONFIG_HI_VI_SENDYUV_SUPPORT 1
#define CONFIG_HI_VI_3DNRX_SUPPORT 1
#define CONFIG_HI_VI_USERPIC_SUPPORT 1
#define CONFIG_HI_VI_COVEREX_OVERLAYEX_SUPPORT 1
#define CONFIG_HI_VI_LUMA_SUPPORT 1
#define CONFIG_HI_VI_FPN_SUPPORT 1
#define CONFIG_HI_VI_EXTCHN_SUPPORT 1
#define CONFIG_HI_SNAP_SUPPORT 1

/*
 * media isp config
 */
#define CONFIG_HI_ISP_SUPPORT 1
#define CONFIG_HI_ISP_AF_SUPPORT 1
#define CONFIG_HI_ISP_CR_SUPPORT 1
#define CONFIG_HI_ISP_PREGAMMA_SUPPORT 1
#define CONFIG_HI_ISP_GCAC_SUPPORT 1
#define CONFIG_HI_ISP_CA_SUPPORT 1
#define CONFIG_HI_ISP_EDGEMARK_SUPPORT 1
#define CONFIG_HI_ISP_HLC_SUPPORT 1
#define CONFIG_HI_ISP_SPECAWB_SUPPORT 1
#define CONFIG_HI_ISP_DPC_STATIC_TABLE_SUPPORT 1

/*
 * media dis config
 */
#define CONFIG_HI_DIS_SUPPORT 1

/*
 * media vpss config
 */
#define CONFIG_HI_VPSS_SUPPORT 1
#define CONFIG_HI_VPSS_3DNR_SUPPORT 1
#define CONFIG_HI_VPSS_LUMA_STAT_SUPPORT 1
#define CONFIG_HI_VPSS_COVER_SUPPORT 1
#define CONFIG_HI_VPSS_MOSAIC_SUPPORT 1
#define CONFIG_HI_VPSS_BUF_SHARE_SUPPORT 1
#define CONFIG_HI_VPSS_SPREAD_SUPPORT 1

/*
 * media avs config
 */

/*
 * media gdc config
 */
#define CONFIG_HI_GDC_SUPPORT 1
#define CONFIG_HI_GDC_LOWDELAY_SUPPORT 1
#define CONFIG_HI_GDC_FISHEYE_SUPPORT 1
#define CONFIG_HI_GDC_NNIE_SHARE_RAM 1

/*
 * media vgs config
 */
#define CONFIG_HI_VGS_SUPPORT 1
#define CONFIG_HI_VGS_LUMA_STAT_SUPPORT 1

/*
 * media chnl config
 */
#define CONFIG_HI_CHNL_SUPPORT 1

/*
 * media venc config
 */
#define CONFIG_HI_VENC_SUPPORT 1
#define CONFIG_HI_H265E_SUPPORT 1
#define CONFIG_HI_H265E_USERDATA_SUPPORT 1
#define CONFIG_HI_H265E_INTRA_REFRESH_SUPPORT 1
#define CONFIG_HI_H264E_SUPPORT 1
#define CONFIG_HI_H264E_SVC_SUPPORT 1
#define CONFIG_HI_H264E_USERDATA_SUPPORT 1
#define CONFIG_HI_H264E_INTRA_REFRESH_SUPPORT 1
#define CONFIG_HI_JPEGE_SUPPORT 1
#define CONFIG_HI_MJPEGE_SUPPORT 1
#define CONFIG_HI_JPEGE_DCF_SUPPORT 1
#define CONFIG_HI_JPEGE_USERDATA_SUPPORT 1
#define CONFIG_HI_VENC_LOWDELAY_SUPPORT 1
#define CONFIG_HI_VENC_VPSSAUTO_SUPPORT 1
#define CONFIG_HI_VENC_FRAMEBUF_RECYCLE_SUPPORT 1
#define CONFIG_HI_VENC_MPF_VGS_SUPPORT 1
#define CONFIG_HI_VENC_VGS_SUPPORT 1
#define CONFIG_HI_VENC_SMARTP_SUPPORT 1
#define CONFIG_HI_VENC_DUALP_SUPPORT 1
#define CONFIG_HI_VENC_RCNREF_SHARE_SUPPORT 1
#define CONFIG_HI_VENC_DEBREATH_SUPPORT 1
#define CONFIG_HI_VENC_SKIPREF_SUPPORT 1
#define CONFIG_HI_VENC_SCENE0_SUPPORT 1
#define CONFIG_HI_VENC_SCENE1_SUPPORT 1
#define CONFIG_HI_VENC_SCENE2_SUPPORT 1
#define CONFIG_HI_RC_AVBR_SUPPORT 1
#define CONFIG_HI_RC_QPMAP_SUPPORT 1
#define CONFIG_HI_RC_QVBR_SUPPORT 1
#define CONFIG_HI_RC_CVBR_SUPPORT 1

/*
 * media vdec config
 */
#define CONFIG_HI_VDEC_SUPPORT 1
#define CONFIG_HI_H265D_SUPPORT 1
#define CONFIG_HI_H264D_SUPPORT 1
#define CONFIG_VDEC_IP "VDEC_IP_VEDU"
#define CONFIG_HI_JPEGD_SUPPORT 1
#define CONFIG_HI_JPEGD_PROGRESSIVE 1
#define CONFIG_VDEC_USERDATA_SUPPORT 1

/*
 * media vo config
 */
#define CONFIG_HI_VO_SUPPORT 1
#define CONFIG_HI_VO_FB_SEPARATE 1
#define CONFIG_HI_VO_COVER_OSD_SUPPORT 1
#define CONFIG_HI_VO_VGS 1
#define CONFIG_HI_VO_GRAPH 1

/*
 * media region config
 */
#define CONFIG_HI_REGION_SUPPORT 1
#define CONFIG_HI_RGN_VGS_QUICKCOPY_SUPPORT 1

/*
 * media audio config
 */
#define CONFIG_HI_AUDIO_SUPPORT 1
#define CONFIG_HI_ACODEC_SUPPORT 1
#define CONFIG_HI_ACODEC_VERSION "V660"
#define CONFIG_HI_ACODEC_MAX_GAIN 56
#define CONFIG_HI_ACODEC_MIN_GAIN 0
#define CONFIG_HI_ACODEC_GAIN_STEP 3
#define CONFIG_HI_ACODEC_FAST_POWER_SUPPORT 1
#define CONFIG_HI_DOWNVQE_SUPPORT 1
#define CONFIG_HI_TALKVQE_SUPPORT 1
#define CONFIG_HI_RECORDVQE_SUPPORT 1
#define CONFIG_HI_AUDIO_STATIC_REGISTER_SUPPORT 1
#define CONFIG_HI_AUDIO_H7_NEW_VOICE_SUPPORT 1
#define CONFIG_HI_AUDIO_H7_NEW_VQE_SUPPORT 1

/*
 * media hdr config
 */

/*
 * media mcf config
 */

/*
 * Device Driver Setup
 */

/*
 * drv config
 */
#define CONFIG_DRV 1
#define CONFIG_EXTDRV 1
#define CONFIG_INTERDRV 1
#define CONFIG_CIPHER 1
#define CONFIG_HIUSER 1
#define CONFIG_MIPI_TX 1
#define CONFIG_MIPI_RX 1
#define CONFIG_HI_IR 1
#define CONFIG_HI_WDG 1
#define CONFIG_HI_SYSCFG 1
#define CONFIG_HI_LSADC 1

/*
 * Component Setup
 */

/*
 * Component hdmi Config
 */
#define CONFIG_HI_HDMI_SUPPORT 1

/*
 * Component hifb Config
 */
#define CONFIG_HI_HIFB_SUPPORT 1

/*
 * Component svp Config
 */
#define CONFIG_HI_SVP_SUPPORT 1
#define CONFIG_HI_SVP_CNN 1
#define CONFIG_HI_SVP_IVE 1
#define CONFIG_HI_SVP_IVE_CSC 1
#define CONFIG_HI_SVP_IVE_FILTER_AND_CSC 1
#define CONFIG_HI_SVP_IVE_NCC 1
#define CONFIG_HI_SVP_IVE_LBP 1
#define CONFIG_HI_SVP_IVE_NORM_GRAD 1
#define CONFIG_HI_SVP_IVE_ST_CANDI_CORNER 1
#define CONFIG_HI_SVP_IVE_RESIZE 1
#define CONFIG_HI_SVP_IVE_PERSP_TRANS 1
#define CONFIG_HI_SVP_IVE_KCF 1
#define CONFIG_HI_SVP_IVE_HOG 1
#define CONFIG_HI_SVP_MD 1

/*
 * Component photo Config
 */

/*
 * Component tde Config
 */
#define CONFIG_HI_TDE_SUPPORT 1

/*
 * Component pciv Config
 */

/*
 * Component avs lut Config
 */

/*
 * Component pos_query Config
 */

/*
 * Component tzasc Config
 */

/*
 * Component motionfusion config
 */

/*
 * Component pm Config
 */

/*
 * Component gpu Config
 */

/*
 * HISYSLINK Setup
 */

/*
 * hisyslink config
 */

/*
 * Debug Config
 */
#define CONFIG_HI_GDB_NO 1
#define CONFIG_HI_GDB "n"
#define CONFIG_HI_PROC_SHOW_SUPPORT 1
#define CONFIG_HI_LOG_TRACE_SUPPORT 1
#define CONFIG_HI_LOG_TRACE_ALL 1
#define CONFIG_HI_LOG_TRACE_LEVEL 7

/*
 * Test Config
 */

#endif /* __AUTOCONF_H__ */

