/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

/** @addtogroup   MODULES  */
/** @{ */ /** <!-- [MODULES] */
#ifndef __HI_MODULE_H__
#define __HI_MODULE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define HI_INVALID_MODULE_ID      (0xffffffff)
#define HI_MAX_USER_MODULE_NUMBER (256)

/** Module ID flags */
typedef enum hiMOD_ID_E {
    HI_ID_STB = 0,

    /**< common. */ /**< CNcomment: System general enumeration data constants */
    HI_ID_SYS = 1,
    HI_ID_MODULE,
    HI_ID_LOG,
    HI_ID_PROC,
    HI_ID_MEM,
    HI_ID_STAT,
    HI_ID_PDM,
    HI_ID_MEMDEV,
    HI_ID_CUSTOM,
    HI_ID_COMMON_BUTT,

    HI_ID_DEMUX = HI_ID_COMMON_BUTT + 1,
    HI_ID_DEMUX_BUTT,

    /**< audio. */ /**< CNcomment: Audio part constant area */
    HI_ID_ADEC = HI_ID_DEMUX_BUTT + 1,
    HI_ID_AO,
    HI_ID_SIO_AI,
    HI_ID_SIO_AO,
    HI_ID_SIO,
    HI_ID_AI,
    HI_ID_AENC,
    HI_ID_SRC,
    HI_ID_AIAO,
    HI_ID_AFLT,
    HI_ID_ADSP,
    HI_ID_AMP,
    HI_ID_SIF,
    HI_ID_TTS,
    HI_ID_AUDIO_BUTT,

    /**< video. */ /**< CNcomment: Video part constant area */
    HI_ID_VFMW = HI_ID_AUDIO_BUTT + 1,
    HI_ID_SVDEC,
    HI_ID_DISP,
    HI_ID_HDMI,
    HI_ID_VO,
    HI_ID_FRC,
    HI_ID_VPSS,
    HI_ID_VDEC,
    HI_ID_VI,
    HI_ID_VENC,
    HI_ID_PQ,
    HI_ID_EDID,
    HI_ID_MEMC,
    HI_ID_VICAP,
    HI_ID_PANEL,
    HI_ID_FDMNG,
    HI_ID_RM,
    HI_ID_VPLUGIN,
    HI_ID_VIDEO_BUTT,

    /**< graphics. */ /**< CNcomment: Graphics part constant area */
    HI_ID_TDE = HI_ID_VIDEO_BUTT + 1,
    HI_ID_JPGDEC,
    HI_ID_JPGENC,
    HI_ID_FB,
    HI_ID_PNG,
    HI_ID_HIGO,
    HI_ID_GRAPHICS_BUTT,

    /**< player. */ /**< CNcomment: Play the relevant part of the constant area */
    HI_ID_PVR = HI_ID_GRAPHICS_BUTT + 1,
    HI_ID_AVPLAY,
    HI_ID_SYNC,
    HI_ID_VSYNC,
    HI_ID_ASYNC,
    HI_ID_FASTPLAY,
    HI_ID_PLAYER_BUTT,

    /**< ecs. */ /**< CNcomment: Peripheral part constant area */
    HI_ID_FLASH = HI_ID_PLAYER_BUTT + 1,
    HI_ID_IR,
    HI_ID_RTC,
    HI_ID_I2C,
    HI_ID_SCI,
    HI_ID_ETH,
    HI_ID_USB_PROTECT,
    HI_ID_WDG, /* watch dog used 'W' */
    HI_ID_GPIO,
    HI_ID_GPIO_I2C,
    HI_ID_DMAC,
    HI_ID_PMOC,
    HI_ID_TUNER,
    HI_ID_KEYLED,
    HI_ID_CIPHER,
    HI_ID_OTP,
    HI_ID_CA,
    HI_ID_PM,
    HI_ID_CI,
    HI_ID_CIMAXPLUS,
    HI_ID_TVP5150,
    HI_ID_SIL9293,
    HI_ID_PWM,
    HI_ID_LSADC,
    HI_ID_SPI,
    HI_ID_MDDRC,
    HI_ID_ECS_BUTT,

    /**< ATV */ /**<  CNcomment: ATV front part constant area */
    HI_ID_VFE = HI_ID_ECS_BUTT + 1,
    HI_ID_TVD,
    HI_ID_HDDEC,
    HI_ID_HDMIRX,
    HI_ID_VBI,
    HI_ID_ATV_BUTT,

    /**< voip, bluetooth,alsa. */ /**<  CNcomment: VOIP¡¢Bluetooth part constant area */
    HI_ID_VOIP_HME = HI_ID_ATV_BUTT + 1,
    HI_ID_NDPT,
    HI_ID_AUDADP,
    HI_ID_BT,
    HI_ID_ALSA,
    HI_ID_3G,
    HI_ID_KARAOKE,
    HI_ID_VOIP_BUTT,

    /**< vp. */ /**<  CNcomment: VP constant area */
    HI_ID_VP = HI_ID_VOIP_BUTT + 1,
    HI_ID_HDCP,
    HI_ID_VP_BUTT,

    /**< subtitle. */ /**<  CNcomment: Subtitle constant area */
    HI_ID_SUBT = HI_ID_VP_BUTT + 1,
    HI_ID_TTX,
    HI_ID_CC,
    HI_ID_SUBTITLE_BUTT,

    /**< loader. */ /**< CNcomment: loader */
    HI_ID_LOADER = HI_ID_SUBTITLE_BUTT + 1,
    HI_ID_LOADER_BUTT,

    /**< user definition. */ /**< CNcomment: Custom area reserved for the whole machine */
    HI_ID_USR_START = HI_ID_LOADER_BUTT + 1,
    HI_ID_USER,
    HI_ID_IRQ,
    HI_ID_USR_END = 0xFE, /* Caution: to avoid overflow */

    HI_ID_BUTT = 0xFF
} HI_MOD_ID_E;


#ifndef __DRV_STRUCT_IOCTL_H__
#define __DRV_STRUCT_IOCTL_H__

#ifndef __HI_DRV_STRUCT_H__
#define __HI_DRV_STRUCT_H__

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

    /*************************** Structure Definition ****************************/
    /** \addtogroup      H_2_1_2 */
    /** @{ */ /** <!-- ¡¾Common data structure. CNcomment: General data structure */

#define UMAP_NAME "hi_"

#define HI_MOD_SYS         "sys"
#define UMAP_DEVNAME_SYS   UMAP_NAME HI_MOD_SYS
#define UMAP_DEV_NUM_SYS   1
#define UMAP_MIN_MINOR_SYS 1
#define UMAP_MAX_MINOR_SYS (UMAP_MIN_MINOR_SYS + UMAP_DEV_NUM_SYS - 1)

#define HI_MOD_LOG         "log"
#define UMAP_DEVNAME_LOG   UMAP_NAME HI_MOD_LOG
#define UMAP_DEV_NUM_LOG   1
#define UMAP_MIN_MINOR_LOG (UMAP_MAX_MINOR_SYS + 1)
#define UMAP_MAX_MINOR_LOG (UMAP_MIN_MINOR_LOG + UMAP_DEV_NUM_LOG - 1)

#define HI_MOD_EVENT         "low_delay"
#define UMAP_DEVNAME_EVENT   UMAP_NAME HI_MOD_EVENT
#define UMAP_DEV_NUM_EVENT   1
#define UMAP_MIN_MINOR_EVENT (UMAP_MAX_MINOR_LOG + 1)
#define UMAP_MAX_MINOR_EVENT (UMAP_MIN_MINOR_EVENT + UMAP_DEV_NUM_EVENT - 1)

#define HI_MOD_STAT         "stat"
#define UMAP_DEVNAME_STAT   UMAP_NAME HI_MOD_STAT
#define UMAP_DEV_NUM_STAT   1
#define UMAP_MIN_MINOR_STAT (UMAP_MAX_MINOR_EVENT + 1)
#define UMAP_MAX_MINOR_STAT (UMAP_MIN_MINOR_STAT + UMAP_DEV_NUM_STAT - 1)

#define HI_MOD_MEM "mem"

#define HI_MOD_PROC "proc"

    /* End of CMPI */
    /* Other modules */
    /* CNcomment:  other modules */
#define HI_MOD_CIPHER         "cipher"
#define UMAP_DEVNAME_CIPHER   UMAP_NAME HI_MOD_CIPHER
#define UMAP_DEV_NUM_CIPHER   1
#define UMAP_MIN_MINOR_CIPHER (UMAP_MAX_MINOR_STAT + 1)
#define UMAP_MAX_MINOR_CIPHER (UMAP_MIN_MINOR_CIPHER + UMAP_DEV_NUM_CIPHER - 1)

#define HI_MOD_DISP         "disp"
#define UMAP_DEVNAME_DISP   UMAP_NAME HI_MOD_DISP
#define UMAP_DEV_NUM_DISP   1
#define UMAP_MIN_MINOR_DISP (UMAP_MAX_MINOR_CIPHER + 1)
#define UMAP_MAX_MINOR_DISP (UMAP_MIN_MINOR_DISP + UMAP_DEV_NUM_DISP - 1)

#define HI_MOD_VO         "vo"
#define UMAP_DEVNAME_VO   UMAP_NAME HI_MOD_VO
#define UMAP_DEV_NUM_VO   1
#define UMAP_MIN_MINOR_VO (UMAP_MAX_MINOR_DISP + 1)
#define UMAP_MAX_MINOR_VO (UMAP_MIN_MINOR_VO + UMAP_DEV_NUM_VO - 1)

#define HI_MOD_VPSS         "vpss"
#define UMAP_DEVNAME_VPSS   UMAP_NAME HI_MOD_VPSS
#define UMAP_DEV_NUM_VPSS   1
#define UMAP_MIN_MINOR_VPSS (UMAP_MAX_MINOR_VO + 1)
#define UMAP_MAX_MINOR_VPSS (UMAP_MIN_MINOR_VPSS + UMAP_DEV_NUM_VPSS - 1)

#define HI_MOD_VDEC         "vdec"
#define UMAP_DEVNAME_VDEC   UMAP_NAME HI_MOD_VDEC
#define UMAP_DEV_NUM_VDEC   2
#define UMAP_MIN_MINOR_VDEC (UMAP_MAX_MINOR_VPSS + 1)
#define UMAP_MAX_MINOR_VDEC (UMAP_MIN_MINOR_VDEC + UMAP_DEV_NUM_VDEC - 1)

#define HI_MOD_AO         "ao"
#define UMAP_DEVNAME_AO   UMAP_NAME HI_MOD_AO
#define UMAP_DEV_NUM_AO   2
#define UMAP_MIN_MINOR_AO (UMAP_MAX_MINOR_VDEC + 1)
#define UMAP_MAX_MINOR_AO (UMAP_MIN_MINOR_AO + UMAP_DEV_NUM_AO - 1)

#define HI_MOD_AI         "ai"
#define UMAP_DEVNAME_AI   UMAP_NAME HI_MOD_AI
#define UMAP_DEV_NUM_AI   2
#define UMAP_MIN_MINOR_AI (UMAP_MAX_MINOR_AO + 1)
#define UMAP_MAX_MINOR_AI (UMAP_MIN_MINOR_AI + UMAP_DEV_NUM_AI - 1)

#define HI_MOD_AIAO         "aiao"
#define UMAP_DEVNAME_AIAO   UMAP_NAME HI_MOD_AIAO
#define UMAP_DEV_NUM_AIAO   2
#define UMAP_MIN_MINOR_AIAO (UMAP_MAX_MINOR_AI + 1)
#define UMAP_MAX_MINOR_AIAO (UMAP_MIN_MINOR_AIAO + UMAP_DEV_NUM_AIAO - 1)

#define HI_MOD_AENC         "aenc"
#define UMAP_DEVNAME_AENC   UMAP_NAME HI_MOD_AENC
#define UMAP_DEV_NUM_AENC   2
#define UMAP_MIN_MINOR_AENC (UMAP_MAX_MINOR_AIAO + 1)
#define UMAP_MAX_MINOR_AENC (UMAP_MIN_MINOR_AENC + UMAP_DEV_NUM_AENC - 1)

#define HI_MOD_AFLT         "aflt"
#define UMAP_DEVNAME_AFLT   UMAP_NAME HI_MOD_AFLT
#define UMAP_DEV_NUM_AFLT   2
#define UMAP_MIN_MINOR_AFLT (UMAP_MAX_MINOR_AENC + 1)
#define UMAP_MAX_MINOR_AFLT (UMAP_MIN_MINOR_AFLT + UMAP_DEV_NUM_AFLT - 1)

#define HI_MOD_ADSP         "adsp"
#define UMAP_DEVNAME_ADSP   UMAP_NAME HI_MOD_ADSP
#define UMAP_DEV_NUM_ADSP   2
#define UMAP_MIN_MINOR_ADSP (UMAP_MAX_MINOR_AFLT + 1)
#define UMAP_MAX_MINOR_ADSP (UMAP_MIN_MINOR_ADSP + UMAP_DEV_NUM_ADSP - 1)

#define HI_MOD_ADEC         "adec"
#define UMAP_DEVNAME_ADEC   UMAP_NAME HI_MOD_ADEC
#define UMAP_DEV_NUM_ADEC   2
#define UMAP_MIN_MINOR_ADEC (UMAP_MAX_MINOR_ADSP + 1)
#define UMAP_MAX_MINOR_ADEC (UMAP_MIN_MINOR_ADEC + UMAP_DEV_NUM_ADEC - 1)

#define HI_MOD_DEMUX         "demux"
#define UMAP_DEVNAME_DEMUX   UMAP_NAME HI_MOD_DEMUX
#define UMAP_DEV_NUM_DEMUX   1
#define UMAP_MIN_MINOR_DEMUX (UMAP_MAX_MINOR_ADEC + 1)
#define UMAP_MAX_MINOR_DEMUX (UMAP_MIN_MINOR_DEMUX + UMAP_DEV_NUM_DEMUX - 1)

#define HI_MOD_JPGD         "jpgd"
#define UMAP_DEVNAME_JPGD   UMAP_NAME HI_MOD_JPGD
#define UMAP_DEV_NUM_JPGD   1
#define UMAP_MIN_MINOR_JPGD (UMAP_MAX_MINOR_DEMUX + 1)
#define UMAP_MAX_MINOR_JPGD (UMAP_MIN_MINOR_JPGD + UMAP_DEV_NUM_JPGD - 1)

#define HI_MOD_SYNC         "sync"
#define UMAP_DEVNAME_SYNC   UMAP_NAME HI_MOD_SYNC
#define UMAP_DEV_NUM_SYNC   1
#define UMAP_MIN_MINOR_SYNC (UMAP_MAX_MINOR_JPGD + 1)
#define UMAP_MAX_MINOR_SYNC (UMAP_MIN_MINOR_SYNC + UMAP_DEV_NUM_SYNC - 1)

#define HI_MOD_AVPLAY         "avplay"
#define UMAP_DEVNAME_AVPLAY   UMAP_NAME HI_MOD_AVPLAY
#define UMAP_DEV_NUM_AVPLAY   1
#define UMAP_MIN_MINOR_AVPLAY (UMAP_MAX_MINOR_SYNC + 1)
#define UMAP_MAX_MINOR_AVPLAY (UMAP_MIN_MINOR_AVPLAY + UMAP_DEV_NUM_AVPLAY - 1)

#define HI_MOD_PVR         "pvr"
#define UMAP_DEVNAME_PVR   UMAP_NAME HI_MOD_PVR
#define UMAP_DEV_NUM_PVR   1
#define UMAP_MIN_MINOR_PVR (UMAP_MAX_MINOR_AVPLAY + 1)
#define UMAP_MAX_MINOR_PVR (UMAP_MIN_MINOR_PVR + UMAP_DEV_NUM_PVR - 1)

#define HI_MOD_HDMI         "hdmi"
#define UMAP_DEVNAME_HDMI   UMAP_NAME HI_MOD_HDMI
#define UMAP_DEV_NUM_HDMI   1
#define UMAP_MIN_MINOR_HDMI (UMAP_MAX_MINOR_PVR + 1)
#define UMAP_MAX_MINOR_HDMI (UMAP_MIN_MINOR_HDMI + UMAP_DEV_NUM_HDMI - 1)

#define HI_MOD_VI         "vi"
#define UMAP_DEVNAME_VI   UMAP_NAME HI_MOD_VI
#define UMAP_DEV_NUM_VI   1
#define UMAP_MIN_MINOR_VI (UMAP_MAX_MINOR_HDMI + 1)
#define UMAP_MAX_MINOR_VI (UMAP_MIN_MINOR_VI + UMAP_DEV_NUM_VI - 1)

#define HI_MOD_VENC         "venc"
#define UMAP_DEVNAME_VENC   UMAP_NAME HI_MOD_VENC
#define UMAP_DEV_NUM_VENC   2
#define UMAP_MIN_MINOR_VENC (UMAP_MAX_MINOR_VI + 1)
#define UMAP_MAX_MINOR_VENC (UMAP_MIN_MINOR_VENC + UMAP_DEV_NUM_VENC - 1)

#define HI_MOD_SIO_AI         "sio_ai"
#define UMAP_DEVNAME_SIO_AI   UMAP_NAME HI_MOD_SIO_AI
#define UMAP_DEV_NUM_SIO_AI   2
#define UMAP_MIN_MINOR_SIO_AI (UMAP_MAX_MINOR_VENC + 1)
#define UMAP_MAX_MINOR_SIO_AI (UMAP_MIN_MINOR_SIO_AI + UMAP_DEV_NUM_SIO_AI - 1)

#define HI_MOD_SIO_AO         "sio_ao"
#define UMAP_DEVNAME_SIO_AO   UMAP_NAME HI_MOD_SIO_AO
#define UMAP_DEV_NUM_SIO_AO   2
#define UMAP_MIN_MINOR_SIO_AO (UMAP_MAX_MINOR_SIO_AI + 1)
#define UMAP_MAX_MINOR_SIO_AO (UMAP_MIN_MINOR_SIO_AO + UMAP_DEV_NUM_SIO_AO - 1)

#define HI_MOD_DMA         "dma"
#define UMAP_DEVNAME_DMA   UMAP_NAME HI_MOD_DMA
#define UMAP_DEV_NUM_DMA   1
#define UMAP_MIN_MINOR_DMA (UMAP_MAX_MINOR_SIO_AO + 1)
#define UMAP_MAX_MINOR_DMA (UMAP_MIN_MINOR_DMA + UMAP_DEV_NUM_DMA - 1)

#define HI_MOD_AUDADP         "audadp"
#define UMAP_DEVNAME_AUDADP   UMAP_NAME HI_MOD_AUDADP
#define UMAP_DEV_NUM_AUDADP   1
#define UMAP_MIN_MINOR_AUDADP (UMAP_MAX_MINOR_DMA + 1)
#define UMAP_MAX_MINOR_AUDADP (UMAP_MIN_MINOR_AUDADP + UMAP_DEV_NUM_AUDADP - 1)

#define HI_MOD_HME         "hme"
#define UMAP_DEVNAME_HME   UMAP_NAME HI_MOD_HME
#define UMAP_DEV_NUM_HME   1
#define UMAP_MIN_MINOR_HME (UMAP_MAX_MINOR_AUDADP + 1)
#define UMAP_MAX_MINOR_HME (UMAP_MIN_MINOR_HME + UMAP_DEV_NUM_HME - 1)

#define HI_MOD_NDPT         "ndpt"
#define UMAP_DEVNAME_NDPT   UMAP_NAME HI_MOD_NDPT
#define UMAP_DEV_NUM_NDPT   1
#define UMAP_MIN_MINOR_NDPT (UMAP_MAX_MINOR_HME + 1)
#define UMAP_MAX_MINOR_NDPT (UMAP_MIN_MINOR_NDPT + UMAP_DEV_NUM_NDPT - 1)

#define HI_MOD_VP         "vp"
#define UMAP_DEVNAME_VP   UMAP_NAME HI_MOD_VP
#define UMAP_DEV_NUM_VP   1
#define UMAP_MIN_MINOR_VP (UMAP_MAX_MINOR_NDPT + 1)
#define UMAP_MAX_MINOR_VP (UMAP_MIN_MINOR_VP + UMAP_DEV_NUM_VP - 1)

    /*                 ECS                */
#define HI_MOD_IR         "ir"
#define UMAP_DEVNAME_IR   UMAP_NAME HI_MOD_IR
#define UMAP_DEV_NUM_IR   1
#define UMAP_MIN_MINOR_IR (UMAP_MAX_MINOR_VP + 1)
#define UMAP_MAX_MINOR_IR (UMAP_MIN_MINOR_IR + UMAP_DEV_NUM_IR - 1)

#define HI_MOD_KEYLED         "keyled"
#define UMAP_DEVNAME_KEYLED   UMAP_NAME HI_MOD_KEYLED
#define UMAP_DEV_NUM_KEYLED   1
#define UMAP_MIN_MINOR_KEYLED (UMAP_MAX_MINOR_IR + 1)
#define UMAP_MAX_MINOR_KEYLED (UMAP_MIN_MINOR_KEYLED + UMAP_DEV_NUM_KEYLED - 1)

#define HI_MOD_LSADC         "lsadc"
#define UMAP_DEVNAME_LSADC   UMAP_NAME HI_MOD_LSADC
#define UMAP_DEV_NUM_LSADC   1
#define UMAP_MIN_MINOR_LSADC (UMAP_MAX_MINOR_KEYLED + 1)
#define UMAP_MAX_MINOR_LSADC (UMAP_MIN_MINOR_LSADC + UMAP_DEV_NUM_LSADC - 1)

#define HI_MOD_I2C         "i2c"
#define UMAP_DEVNAME_I2C   UMAP_NAME HI_MOD_I2C
#define UMAP_DEV_NUM_I2C   1
#define UMAP_MIN_MINOR_I2C (UMAP_MAX_MINOR_LSADC + 1)
#define UMAP_MAX_MINOR_I2C (UMAP_MIN_MINOR_I2C + UMAP_DEV_NUM_I2C - 1)

#define HI_MOD_TUNER         "tuner"
#define UMAP_DEVNAME_TUNER   UMAP_NAME HI_MOD_TUNER
#define UMAP_DEV_NUM_TUNER   1
#define UMAP_MIN_MINOR_TUNER (UMAP_MAX_MINOR_I2C + 1)
#define UMAP_MAX_MINOR_TUNER (UMAP_MIN_MINOR_TUNER + UMAP_DEV_NUM_TUNER - 1)

#define HI_MOD_SCI         "sci"
#define UMAP_DEVNAME_SCI   UMAP_NAME HI_MOD_SCI
#define UMAP_DEV_NUM_SCI   1
#define UMAP_MIN_MINOR_SCI (UMAP_MAX_MINOR_TUNER + 1)
#define UMAP_MAX_MINOR_SCI (UMAP_MIN_MINOR_SCI + UMAP_DEV_NUM_SCI - 1)

#define HI_MOD_PM         "pm"
#define UMAP_DEVNAME_PM   UMAP_NAME HI_MOD_PM
#define UMAP_DEV_NUM_PM   1
#define UMAP_MIN_MINOR_PM (UMAP_MAX_MINOR_SCI + 1)
#define UMAP_MAX_MINOR_PM (UMAP_MIN_MINOR_PM + UMAP_DEV_NUM_PM - 1)

#define HI_MOD_CA         "ca"
#define UMAP_DEVNAME_CA   UMAP_NAME HI_MOD_CA
#define UMAP_DEV_NUM_CA   1
#define UMAP_MIN_MINOR_CA (UMAP_MAX_MINOR_PM + 1)
#define UMAP_MAX_MINOR_CA (UMAP_MIN_MINOR_CA + UMAP_DEV_NUM_CA - 1)

#define HI_MOD_OTP         "otp"
#define UMAP_DEVNAME_OTP   UMAP_NAME HI_MOD_OTP
#define UMAP_DEV_NUM_OTP   1
#define UMAP_MIN_MINOR_OTP (UMAP_MAX_MINOR_CA + 1)
#define UMAP_MAX_MINOR_OTP (UMAP_MIN_MINOR_OTP + UMAP_DEV_NUM_OTP - 1)

#define HI_MOD_SVDEC         "svdec"
#define UMAP_DEVNAME_SVDEC   UMAP_NAME HI_MOD_SVDEC
#define UMAP_DEV_NUM_SVDEC   1
#define UMAP_MIN_MINOR_SVDEC (UMAP_MAX_MINOR_OTP + 1)
#define UMAP_MAX_MINOR_SVDEC (UMAP_MIN_MINOR_SVDEC + UMAP_DEV_NUM_SVDEC - 1)

#define HI_MOD_SETHDCP         "sethdcp"
#define UMAP_DEVNAME_SETHDCP   UMAP_NAME HI_MOD_SETHDCP
#define UMAP_DEV_NUM_SETHDCP   1
#define UMAP_MIN_MINOR_SETHDCP (UMAP_MAX_MINOR_SVDEC + 1)
#define UMAP_MAX_MINOR_SETHDCP (UMAP_MIN_MINOR_SETHDCP + UMAP_DEV_NUM_SETHDCP - 1)

#define HI_MOD_CI         "ci"
#define UMAP_DEVNAME_CI   UMAP_NAME HI_MOD_CI
#define UMAP_DEV_NUM_CI   1
#define UMAP_MIN_MINOR_CI (UMAP_MAX_MINOR_SETHDCP + 1)
#define UMAP_MAX_MINOR_CI (UMAP_MIN_MINOR_CI + UMAP_DEV_NUM_CI - 1)

#define HI_MOD_SUBT         "subt"
#define UMAP_DEVNAME_SUBT   UMAP_NAME HI_MOD_SUBT
#define UMAP_DEV_NUM_SUBT   1
#define UMAP_MIN_MINOR_SUBT (UMAP_MAX_MINOR_CI + 1)
#define UMAP_MAX_MINOR_SUBT (UMAP_MIN_MINOR_SUBT + UMAP_DEV_NUM_SUBT - 1)

#define HI_MOD_MEM2         "memmgr"
#define UMAP_DEVNAME_MEM2   UMAP_NAME HI_MOD_MEM2
#define UMAP_DEV_NUM_MEM2   1
#define UMAP_MIN_MINOR_MEM2 (UMAP_MAX_MINOR_SUBT + 1)
#define UMAP_MAX_MINOR_MEM2 (UMAP_MIN_MINOR_MEM2 + UMAP_DEV_NUM_MEM2 - 1)

#define HI_MOD_MODULE         "module"
#define UMAP_DEVNAME_MODULE   UMAP_NAME HI_MOD_MODULE
#define UMAP_DEV_NUM_MODULE   1
#define UMAP_MIN_MINOR_MODULE (UMAP_MAX_MINOR_MEM2 + 1)
#define UMAP_MAX_MINOR_MODULE (UMAP_MIN_MINOR_MODULE + UMAP_DEV_NUM_MODULE - 1)

#define HI_MOD_MLOG         "mlog"
#define UMAP_DEVNAME_MLOG   UMAP_NAME HI_MOD_MLOG
#define UMAP_DEV_NUM_MLOG   1
#define UMAP_MIN_MINOR_MLOG (UMAP_MAX_MINOR_MODULE + 1)
#define UMAP_MAX_MINOR_MLOG (UMAP_MIN_MINOR_MLOG + UMAP_DEV_NUM_MLOG - 1)

#define HI_MOD_MCE         "mce"
#define UMAP_DEVNAME_MCE   UMAP_NAME HI_MOD_MCE
#define UMAP_DEV_NUM_MCE   1
#define UMAP_MIN_MINOR_MCE (UMAP_MAX_MINOR_MLOG + 1)
#define UMAP_MAX_MINOR_MCE (UMAP_MIN_MINOR_MCE + UMAP_DEV_NUM_MCE - 1)

#define HI_MOD_PDM         "pdm"
#define UMAP_DEVNAME_PDM   UMAP_NAME HI_MOD_PDM
#define UMAP_DEV_NUM_PDM   1
#define UMAP_MIN_MINOR_PDM (UMAP_MAX_MINOR_MCE + 1)
#define UMAP_MAX_MINOR_PDM (UMAP_MIN_MINOR_PDM + UMAP_DEV_NUM_PDM - 1)

#define UMAP_DEV_NUM_TDE   1
#define UMAP_MIN_MINOR_TDE (UMAP_MAX_MINOR_PDM + 1)
#define UMAP_MAX_MINOR_TDE (UMAP_MIN_MINOR_TDE + UMAP_DEV_NUM_TDE - 1)

#define UMAP_DEV_NUM_HIFB   1
#define UMAP_MIN_MINOR_HIFB (UMAP_MAX_MINOR_TDE + 1)
#define UMAP_MAX_MINOR_HIFB (UMAP_MIN_MINOR_HIFB + UMAP_DEV_NUM_HIFB - 1)

#define UMAP_DEV_NUM_PNG   2
#define UMAP_MIN_MINOR_PNG (UMAP_MAX_MINOR_HIFB + 1)
#define UMAP_MAX_MINOR_PNG (UMAP_MIN_MINOR_PNG + UMAP_DEV_NUM_PNG - 1)

#define UMAP_DEV_NUM_JPEG   1
#define UMAP_MIN_MINOR_JPEG (UMAP_MAX_MINOR_PNG + 1)
#define UMAP_MAX_MINOR_JPEG (UMAP_MIN_MINOR_JPEG + UMAP_DEV_NUM_JPEG - 1)

#define UMAP_DEV_NUM_JPGE   1
#define UMAP_MIN_MINOR_JPGE (UMAP_MAX_MINOR_JPEG + 1)
#define UMAP_MAX_MINOR_JPGE (UMAP_MIN_MINOR_JPGE + UMAP_DEV_NUM_JPGE - 1)

#define HI_MOD_WDG         "wdg"
#define UMAP_DEVNAME_WDG   UMAP_NAME HI_MOD_WDG
#define UMAP_DEV_NUM_WDG   1
#define UMAP_MIN_MINOR_WDG (UMAP_MAX_MINOR_JPGE + 1)
#define UMAP_MAX_MINOR_WDG (UMAP_MIN_MINOR_WDG + UMAP_DEV_NUM_WDG - 1)

#define HI_MOD_GPIO         "gpio"
#define UMAP_DEVNAME_GPIO   UMAP_NAME HI_MOD_GPIO
#define UMAP_DEV_NUM_GPIO   1
#define UMAP_MIN_MINOR_GPIO (UMAP_MAX_MINOR_WDG + 1)
#define UMAP_MAX_MINOR_GPIO (UMAP_MIN_MINOR_GPIO + UMAP_DEV_NUM_GPIO - 1)

#define HI_MOD_SIL9293         "sil9293"
#define UMAP_DEVNAME_SIL9293   UMAP_NAME HI_MOD_SIL9293
#define UMAP_DEV_NUM_SIL9293   1
#define UMAP_MIN_MINOR_SIL9293 (UMAP_MAX_MINOR_GPIO + 1)
#define UMAP_MAX_MINOR_SIL9293 (UMAP_MIN_MINOR_SIL9293 + UMAP_DEV_NUM_SIL9293 - 1)

#define HI_MOD_PQ         "pq"
#define UMAP_DEVNAME_PQ   UMAP_NAME HI_MOD_PQ
#define UMAP_DEV_NUM_PQ   1
#define UMAP_MIN_MINOR_PQ (UMAP_MAX_MINOR_SIL9293 + 1)
#define UMAP_MAX_MINOR_PQ (UMAP_MIN_MINOR_PQ + UMAP_DEV_NUM_PQ - 1)

#define HI_MOD_PWM         "pwm"
#define UMAP_DEVNAME_PWM   UMAP_NAME HI_MOD_PWM
#define UMAP_DEV_NUM_PWM   1
#define UMAP_MIN_MINOR_PWM (UMAP_MAX_MINOR_PQ + 1)
#define UMAP_MAX_MINOR_PWM (UMAP_MIN_MINOR_PWM + UMAP_DEV_NUM_PWM - 1)

#define HI_MOD_TVD         "tvd"
#define UMAP_DEVNAME_TVD   UMAP_NAME HI_MOD_TVD
#define UMAP_DEV_NUM_TVD   1
#define UMAP_MIN_MINOR_TVD (UMAP_MAX_MINOR_PWM + 1)
#define UMAP_MAX_MINOR_TVD (UMAP_MIN_MINOR_TVD + UMAP_DEV_NUM_TVD - 1)

#define HI_MOD_PANEL         "panel"
#define UMAP_DEVNAME_PANEL   UMAP_NAME HI_MOD_PANEL
#define UMAP_DEV_NUM_PANEL   1
#define UMAP_MIN_MINOR_PANEL (UMAP_MAX_MINOR_TVD + 1)
#define UMAP_MAX_MINOR_PANEL (UMAP_MIN_MINOR_PANEL + UMAP_DEV_NUM_PANEL - 1)

#define HI_MOD_VICAP         "vicap"
#define UMAP_DEVNAME_VICAP   UMAP_NAME HI_MOD_VICAP
#define UMAP_DEV_NUM_VICAP   1
#define UMAP_MIN_MINOR_VICAP (UMAP_MAX_MINOR_PANEL + 1)
#define UMAP_MAX_MINOR_VICAP (UMAP_MIN_MINOR_VICAP + UMAP_DEV_NUM_VICAP - 1)

#define HI_MOD_SIF         "sif"
#define UMAP_DEVNAME_SIF   UMAP_NAME HI_MOD_SIF
#define UMAP_DEV_NUM_SIF   1
#define UMAP_MIN_MINOR_SIF (UMAP_MIN_MINOR_VICAP + 1)
#define UMAP_MAX_MINOR_SIF (UMAP_MIN_MINOR_SIF + UMAP_DEV_NUM_SIF - 1)

#define HI_MOD_VFE         "vfe"
#define UMAP_DEVNAME_VFE   UMAP_NAME HI_MOD_VFE
#define UMAP_DEV_NUM_VFE   1
#define UMAP_MIN_MINOR_VFE (UMAP_MAX_MINOR_SIF + 1)
#define UMAP_MAX_MINOR_VFE (UMAP_MIN_MINOR_VFE + UMAP_DEV_NUM_VFE - 1)

#define HI_MOD_HDDEC         "hddec"
#define UMAP_DEVNAME_HDDEC   UMAP_NAME HI_MOD_HDDEC
#define UMAP_DEV_NUM_HDDEC   1
#define UMAP_MIN_MINOR_HDDEC (UMAP_MAX_MINOR_VFE + 1)
#define UMAP_MAX_MINOR_HDDEC (UMAP_MIN_MINOR_HDDEC + UMAP_DEV_NUM_HDDEC - 1)

#define HI_MOD_HDMIRX         "hdmirx"
#define UMAP_DEVNAME_HDMIRX   UMAP_NAME HI_MOD_HDMIRX
#define UMAP_DEV_NUM_HDMIRX   1
#define UMAP_MIN_MINOR_HDMIRX (UMAP_MAX_MINOR_HDDEC + 1)
#define UMAP_MAX_MINOR_HDMIRX (UMAP_MIN_MINOR_HDMIRX + UMAP_DEV_NUM_HDMIRX - 1)

#define HI_MOD_AMP         "amp"
#define UMAP_DEVNAME_AMP   UMAP_NAME HI_MOD_AMP
#define UMAP_DEV_NUM_AMP   1
#define UMAP_MIN_MINOR_AMP (UMAP_MAX_MINOR_HDMIRX + 1)
#define UMAP_MAX_MINOR_AMP (UMAP_MIN_MINOR_AMP + UMAP_DEV_NUM_AMP - 1)

#define HI_MOD_VPU         "vpu"
#define UMAP_DEVNAME_VPU   UMAP_NAME HI_MOD_VPU
#define UMAP_DEV_NUM_VPU   1
#define UMAP_MIN_MINOR_VPU (UMAP_MAX_MINOR_AMP + 1)
#define UMAP_MAX_MINOR_VPU (UMAP_MIN_MINOR_VPU + UMAP_DEV_NUM_VPU - 1)

#define HI_MOD_FDMNG         "fdmng"
#define UMAP_DEVNAME_FDMNG   UMAP_NAME HI_MOD_FDMNG
#define UMAP_DEV_NUM_FDMNG   1
#define UMAP_MIN_MINOR_FDMNG (UMAP_MAX_MINOR_VPU + 1)
#define UMAP_MAX_MINOR_FDMNG (UMAP_MIN_MINOR_FDMNG + UMAP_DEV_NUM_FDMNG - 1)

#define HI_MOD_CUSTOM         "custom"
#define UMAP_DEVNAME_CUSTOM   UMAP_NAME HI_MOD_CUSTOM
#define UMAP_DEV_NUM_CUSTOM   1
#define UMAP_MIN_MINOR_CUSTOM (UMAP_MIN_MINOR_FDMNG + 1)
#define UMAP_MAX_MINOR_CUSTOM (UMAP_MIN_MINOR_CUSTOM + UMAP_DEV_NUM_CUSTOM - 1)

#define HI_MOD_VBI         "vbi"
#define UMAP_DEVNAME_VBI   UMAP_NAME HI_MOD_VBI
#define UMAP_DEV_NUM_VBI   1
#define UMAP_MIN_MINOR_VBI (UMAP_MAX_MINOR_CUSTOM + 1)
#define UMAP_MAX_MINOR_VBI (UMAP_MIN_MINOR_VBI + UMAP_DEV_NUM_VBI - 1)

#define HI_MOD_RM         "rm"
#define UMAP_DEVNAME_RM   UMAP_NAME HI_MOD_RM
#define UMAP_DEV_NUM_RM   1
#define UMAP_MIN_MINOR_RM (UMAP_MAX_MINOR_VBI + 1)
#define UMAP_MAX_MINOR_RM (UMAP_MIN_MINOR_RM + UMAP_DEV_NUM_RM - 1)

#define HI_MOD_SPI         "spi"
#define UMAP_DEVNAME_SPI   UMAP_NAME HI_MOD_SPI
#define UMAP_DEV_NUM_SPI   1
#define UMAP_MIN_MINOR_SPI (UMAP_MIN_MINOR_RM + 1)
#define UMAP_MAX_MINOR_SPI (UMAP_MIN_MINOR_SPI + UMAP_DEV_NUM_SPI - 1)

#define HI_MOD_MEMC         "memc"
#define UMAP_DEVNAME_MEMC   UMAP_NAME HI_MOD_MEMC
#define UMAP_DEV_NUM_MEMC   1
#define UMAP_MIN_MINOR_MEMC (UMAP_MIN_MINOR_SPI + 1)
#define UMAP_MAX_MINOR_MEMC (UMAP_MIN_MINOR_MEMC + UMAP_DEV_NUM_MEMC - 1)

#define HI_MOD_VFMW         "vfmw"
#define UMAP_DEVNAME_VFMW   UMAP_NAME HI_MOD_VFMW
#define UMAP_DEV_NUM_VFMW   1
#define UMAP_MIN_MINOR_VFMW (UMAP_MIN_MINOR_MEMC + 1)
#define UMAP_MAX_MINOR_VFMW (UMAP_MIN_MINOR_VFMW + UMAP_DEV_NUM_VFMW - 1)

#define HI_MOD_MDDRC          "mddrc"
#define UMAP_DEVNAME_MDDRC    UMAP_NAME HI_MOD_MDDRC
#define UMAP_DEV_NUM_MDDRC   1
#define UMAP_MIN_MINOR_MDDRC  (UMAP_MIN_MINOR_VFMW + 1)
#define UMAP_MAX_MINOR_MDDRC  (UMAP_MIN_MINOR_MDDRC + UMAP_DEV_NUM_MDDRC - 1)
#define UMAP_DEV_NUM_TOTAL    UMAP_MAX_MINOR_MDDRC

#define HI_MOD_VPLUGIN          "vplugin"
#define UMAP_DEVNAME_VPLUGIN    UMAP_NAME HI_MOD_VPLUGIN
#define UMAP_DEV_NUM_VPLUGIN   1
#define UMAP_MIN_MINOR_VPLUGIN  (UMAP_MIN_MINOR_MDDRC + 1)
#define UMAP_MAX_MINOR_VPLUGIN  (UMAP_MIN_MINOR_VPLUGIN + UMAP_DEV_NUM_VPLUGIN - 1)

    /** @} */ /** <!-- ==== Structure Definition End ==== */

#define IOC_TYPE_DEMUX   'I'
#define IOC_TYPE_MEM     'J'
#define IOC_TYPE_RELEASE 'K'
#define IOC_TYPE_EVENT   'L'
#define IOC_TYPE_SYS     'M'
#define IOC_TYPE_STAT    'N'
#define IOC_TYPE_MCC     'O'
#define IOC_TYPE_TDE     'P'

#define IOC_TYPE_CIPHER 'Q'
#define IOC_TYPE_AVPLAY 'R'

#define IOC_TYPE_VI   'S'
#define IOC_TYPE_VENC 'T'
#define IOC_TYPE_CA   'U'
#define IOC_TYPE_OTP  'V'

#define IOC_TYPE_VP    'W'
#define IOC_TYPE_VICAP 'X'
#define IOC_TYPE_SIF   'Y'

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif /* __cplusplus */

#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

/** @} */ /** <!-- ==== group Definition end ==== */

