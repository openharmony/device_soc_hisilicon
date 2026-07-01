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

#ifndef SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART01_H_
#define SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART01_H_


#include "ot_type.h"
#include "drv_hdmi_infoframe.h"
#include "hdmi_ext.h"
#include "securec.h"
#include "ot_common_hdmi.h"

#define HDMI_VER_MAJOR    2
#define HDMI_VER_MINOR    0
#define HDMI_VER_REVISE   0
#define HDMI_VER_DATE     20240402
#define HDMI_VER_TIMES    0

#define make_ver_bit(x)     #x
#define make_macro2str(exp) make_ver_bit(exp)
#define MAKE_VERSION                    \
    make_macro2str(HDMI_VER_MAJOR) "."  \
    make_macro2str(HDMI_VER_MINOR) "."  \
    make_macro2str(HDMI_VER_REVISE) "." \
    make_macro2str(HDMI_VER_DATE) "."   \
    make_macro2str(HDMI_VER_TIMES)

#define AEN_TX_FFE_LEN              4
#define MAX_FRL_RATE                6
#define HDMI_FRL_LANE_MAX_NUM       4
#define CEA_VIDEO_CODE_MAX          44
#define VESA_VIDEO_CODE_MAX         31
#define CEA861_F_VIDEO_CODES_MAX_4K 4
#define HDMI_INFO_FRAME_MAX_SIZE    31
#define SCDC_TMDS_BIT_CLK_RATIO_10X 10
#define SCDC_TMDS_BIT_CLK_RATIO_40X 40
#define HDMI_DECIMAL                10
#define HDMI_HUNDRED                100
#define HDMI_THOUSAND               1000
#define FMT_PIX_CLK_13400           13400
#define FMT_PIX_CLK_74250           74250
#define FMT_PIX_CLK_165000          165000
#define FMT_PIX_CLK_190000          190000
#define FMT_PIX_CLK_297000          297000
#define FMT_PIX_CLK_340000          340000
#define ZERO_DRMIF_SEND_TIME        2000 /* unit: ms */
#define HDRMODE_CHANGE_TIME         500  /* unit: ms */
#define HDMI_EDID_BLOCK_SIZE        128
#define HDMI_EDID_TOTAL_BLOCKS      4
#define HDMI_EDID_SIZE             (HDMI_EDID_BLOCK_SIZE * HDMI_EDID_TOTAL_BLOCKS)
#define HDMI_REGISTER_SIZE          4
#define HDMI_EDID_BLOCK_SIZE        128
#define HDMI_EDID_MAX_BLOCK_NUM     4
#define HDMI_HW_PARAM_LEN           4
#define HDMI_EDID_TOTAL_SIZE       ((HDMI_EDID_BLOCK_SIZE) * (HDMI_EDID_MAX_BLOCK_NUM))
#define hdmi_array_size(a)         ((sizeof(a)) / (sizeof((a)[0])))
#ifdef HDMI_FPGA_SUPPORT
#define FPGA_SUPPORT                TD_TRUE
#else
#define FPGA_SUPPORT                TD_FALSE
#endif
#define DEBUG_MAX_ARGV_NUM          10
#define hdmi_unused(x)              (x) = (x)

#define PRT_RED                     "\033[31;1m"
#define PRT_GREEN                   "\033[32;1m"
#define PRT_CLEAN                   "\033[0m"

#define FRL_CTRL_TYPE_COMPRESS_ALL   0x00
#define FRL_CTRL_TYPE_COMPRESS_HW    0x01
#define FRL_CTRL_TYPE_COMPRESS_NON   0x03
#define HDMI_FRL_COMPRESS_DEBUG_MASK 0x4

/* AVI InfoFrame Packet byte offset define */
#define AVI_OFFSET_TYPE 0
#define AVI_OFFSET_VERSION 1
#define AVI_OFFSET_LENGTH 2
#define AVI_OFFSET_CHECKSUM 3
/*
 * include :
 * color space
 * active information present
 * bar Info data valid
 * scan Information
 */
#define AVI_OFFSET_PB1 4
/*
 * include :
 * colorimetry
 * picture aspect ratio
 * active format aspect ratio
 */
#define AVI_OFFSET_PB2 5
/*
 * include :
 * IT content
 * extended colorimetry
 * quantization range
 * non-uniform picture scaling
 */
#define AVI_OFFSET_PB3 6
#define AVI_OFFSET_VIC 7
/*
 * include :
 * YCC quantization range
 * content type
 * pixel repetition factor
 */
#define AVI_OFFSET_PB5 8
#define AVI_OFFSET_TOP_BAR_LOWER 9
#define AVI_OFFSET_TOP_BAR_UPPER 10
#define AVI_OFFSET_BOTTOM_BAR_LOWER 11
#define AVI_OFFSET_BOTTOM_BAR_UPPER 12
#define AVI_OFFSET_LEFT_BAR_LOWER 13
#define AVI_OFFSET_LEFT_BAR_UPPER 14
#define AVI_OFFSET_RIGHT_BAR_LOWER 15
#define AVI_OFFSET_RIGHT_BAR_UPPER 16
#define AVI_OFFSET_PB14 17
#define AVI_OFFSET_PB15 18
#define AVI_OFFSET_PB16 19
#define AVI_OFFSET_PB17 20
#define AVI_OFFSET_PB18 21
#define AVI_OFFSET_PB19 22
#define AVI_OFFSET_PB20 23
#define AVI_OFFSET_PB21 24
#define AVI_OFFSET_PB22 25
#define AVI_OFFSET_PB23 26
#define AVI_OFFSET_PB24 27
#define AVI_OFFSET_PB25 28
#define AVI_OFFSET_PB26 29
#define AVI_OFFSET_PB27 30
#define AVI_FRAME_COLORIMETRY_MASK 0x3
#define AVI_FRAME_PIC_ASPECT_MASK 0x3
#define AVI_FRAME_ACTIVE_ASPECT_MASK 0xF
#define AVI_FRAME_EXT_COLORIMETRY_MASK 0x7
#define AVI_FRAME_QUANT_RANGE_MASK 0x3
#define AVI_FRAME_YCC_QUANT_RANGE_MASK 0x3
#define AVI_FRAME_PIXEL_REPET_MASK 0xF

/* audio infoFrame packet byte offset define */
#define AUDIO_OFFSET_TYPE 0
#define AUDIO_OFFSET_VERSION 1
#define AUDIO_OFFSET_LENGHT 2
#define AUDIO_OFFSET_CHECKSUM 3
/*
 * include :
 * channel count
 * coding type
 */
#define AUDIO_OFFSET_PB1 4
/*
 * include :
 * sample size
 * sample frequency
 */
#define AUDIO_OFFSET_PB2 5
#define AUDIO_OFFSET_FORMAT 6
#define AUDIO_OFFSET_CA 7
/*
 * include :
 * level shift value
 * downmix inhibit
 * LFE playback level information
 */
#define AUDIO_OFFSET_PB5 8
#define AUDIO_OFFSET_PB6 9
#define AUDIO_OFFSET_PB7 10
#define AUDIO_OFFSET_PB8 11
#define AUDIO_OFFSET_PB9 12
#define AUDIO_OFFSET_PB10 13
#define AUDIO_OFFSET_PB11 14
#define AUDIO_OFFSET_PB12 15
#define AUDIO_OFFSET_PB13 16
#define AUDIO_OFFSET_PB14 17
#define AUDIO_OFFSET_PB15 18
#define AUDIO_OFFSET_PB16 19
#define AUDIO_OFFSET_PB17 20
#define AUDIO_OFFSET_PB18 21
#define AUDIO_OFFSET_PB19 22
#define AUDIO_OFFSET_PB20 23
#define AUDIO_OFFSET_PB21 24
#define AUDIO_OFFSET_PB22 25
#define AUDIO_OFFSET_PB23 26
#define AUDIO_OFFSET_PB24 27
#define AUDIO_OFFSET_PB25 28
#define AUDIO_OFFSET_PB26 29
#define AUDIO_OFFSET_PB27 30
#define AUDIO_FRAME_CODE_TYPE_MASK 0xF

/* gdb infoFrame packet byte offset define */
#define GDB_OFFSET_HB0 0
#define GDB_OFFSET_HB1 1
#define GDB_OFFSET_HB2 2
#define GDB_OFFSET_CHECKSUM 3
#define GDB_OFFSET_PB1 4
#define GDB_OFFSET_PB2 5
#define GDB_OFFSET_PB3 6
#define GDB_OFFSET_PB4 7
#define GDB_OFFSET_PB5 8
#define GDB_OFFSET_PB6 9
#define GDB_OFFSET_PB7 10
#define GDB_OFFSET_PB8 11
#define GDB_OFFSET_PB9 12
#define GDB_OFFSET_PB10 13
#define GDB_OFFSET_PB11 14
#define GDB_OFFSET_PB12 15
#define GDB_OFFSET_PB13 16
#define GDB_OFFSET_PB14 17
#define GDB_OFFSET_PB15 18
#define GDB_OFFSET_PB16 19
#define GDB_OFFSET_PB17 20
#define GDB_OFFSET_PB18 21
#define GDB_OFFSET_PB19 22
#define GDB_OFFSET_PB20 23
#define GDB_OFFSET_PB21 24
#define GDB_OFFSET_PB22 25
#define GDB_OFFSET_PB23 26
#define GDB_OFFSET_PB24 27
#define GDB_OFFSET_PB25 28
#define GDB_OFFSET_PB26 29
#define GDB_OFFSET_PB27 30

/* vendor infoFrame packet byte offset define */
#define VENDOR_OFFSET_TYPE 0
#define VENDOR_OFFSET_VERSION 1
#define VENDOR_OFFSET_LENGHT 2
#define VENDOR_OFFSET_CHECSUM 3
#define VENDOR_OFFSET_IEEE_LOWER 4
#define VENDOR_OFFSET_IEEE_UPPER 5
#define VENDOR_OFFSET_IEEE 6
#define VENDOR_OFFSET_VIDEO_FMT 7
#define VENDOR_OFFSET_VIC 8
#define VENDOR_OFFSET_3D_STRUCT 9
#define VENDOR_OFFSET_3D_EXT_DATA 10
#define VENDOR_OFFSET_PB7 11
#define VENDOR_OFFSET_PB8 12
#define VENDOR_OFFSET_PB9 13
#define VENDOR_OFFSET_PB10 14
#define VENDOR_OFFSET_PB11 15
#define VENDOR_OFFSET_PB12 16
#define VENDOR_OFFSET_PB13 17
#define VENDOR_OFFSET_PB14 18
#define VENDOR_OFFSET_PB15 19
#define VENDOR_OFFSET_PB16 20
#define VENDOR_OFFSET_PB17 21
#define VENDOR_OFFSET_PB18 22
#define VENDOR_OFFSET_PB19 23
#define VENDOR_OFFSET_PB20 24
#define VENDOR_OFFSET_PB21 25
#define VENDOR_OFFSET_PB22 26
#define VENDOR_OFFSET_PB23 27
#define VENDOR_OFFSET_PB24 28
#define VENDOR_OFFSET_PB25 29
#define VENDOR_OFFSET_PB26 30
#define VENDOR_FARAME_VIDEO_FMT_MASK 0x7
#define VENDOR_3D_STRUCT_MASK 0xF

#define HDMI_BKSV_LEN 5
#define HDMI_AKSV_LEN 5

#define hdmi_unlock_if_null_return(p, mutex, ret)  \
    do {                                           \
        if ((p) == TD_NULL) {                      \
            hdmi_err("%s is null pointer!\n", #p); \
            hdmi_mutex_unlock((mutex));            \
            return (ret);                          \
        }                                          \
    } while (0)

#define hdmi_if_null_return(p, ret)                \
    do {                                           \
        if ((p) == TD_NULL) {                      \
            hdmi_err("%s is null pointer!\n", #p); \
            return (ret);                          \
        }                                          \
    } while (0)

#define hdmi_if_null_return_void(p)                \
    do {                                           \
        if ((p) == TD_NULL) {                      \
            hdmi_err("%s is null pointer!\n", #p); \
            return;                                \
        }                                          \
    } while (0)

#define hdmi_if_null_warn_return(p, ret)            \
    do {                                            \
        if ((p) == TD_NULL) {                       \
            hdmi_warn("%s is null pointer!\n", #p); \
            return (ret);                           \
        }                                           \
    } while (0)

#define hdmi_if_null_warn_return_void(p)            \
    do {                                            \
        if ((p) == TD_NULL) {                       \
            hdmi_warn("%s is null pointer!\n", #p); \
            return;                                 \
        }                                           \
    } while (0)

#define hdmi_if_false_return_void(b)        \
    do {                                    \
        if ((b) != TD_TRUE) {               \
            hdmi_err("%s is FALSE!\n", #b); \
            return;                         \
        }                                   \
    } while (0)

#define hdmi_if_false_return(tmp, ret)        \
    do {                                      \
        if ((tmp) != TD_TRUE) {               \
            hdmi_err("%s is FALSE!\n", #tmp); \
            return (ret);                     \
        }                                     \
    } while (0)

#define hdmi_if_false_warn_return(tmp, ret)    \
    do {                                       \
        if ((tmp) != TD_TRUE) {                \
            hdmi_warn("%s is FALSE!\n", #tmp); \
            return (ret);                      \
        }                                      \
    } while (0)

#define hdmi_if_false_warn_return_void(tmp)    \
    do {                                       \
        if ((tmp) != TD_TRUE) {                \
            hdmi_warn("%s is FALSE!\n", #tmp); \
            return;                            \
        }                                      \
    } while (0)

#define hdmi_check_is_change_return(tmp0, tmp1, ret)                                            \
    do {                                                                                        \
        if ((tmp0) != (tmp1)) {                                                                 \
            hdmi_info("%s change, old(%u)->new(%u) \n", #tmp0, (td_u32)(tmp0), (td_u32)(tmp1)); \
            return (ret);                                                                       \
        }                                                                                       \
    } while (0)

#define hdmi_check_max_return(value, max, ret)                    \
    do {                                                          \
        if ((value) > (max)) {                                    \
            hdmi_warn("value %u exceed max!\n", (td_u32)(value)); \
            return (ret);                                         \
        }                                                         \
    } while (0)

#define hdmi_if_failure_return(tmp, ret)        \
    do {                                        \
        if ((tmp) != TD_SUCCESS) {              \
            hdmi_err("%s is failure!\n", #tmp); \
            return (ret);                       \
        }                                       \
    } while (0)

#define hdmi_if_failure_return_void(tmp)        \
    do {                                        \
        if ((tmp) != TD_SUCCESS) {              \
            hdmi_err("%s is failure!\n", #tmp); \
            return;                             \
        }                                       \
    } while (0)
#define hdmi_if_failure_warn_return_void(tmp)    \
    do {                                         \
        if ((tmp) != TD_SUCCESS) {               \
            hdmi_warn("%s is failure!\n", #tmp); \
            return;                              \
        }                                        \
    } while (0)

#define hdmi_set_bit(var, bit) \
    do {                       \
        (var) |= 1 << (bit);   \
    } while (0)

#define hdmi_clr_bit(var, bit)  \
    do {                        \
        (var) &= ~(1 << (bit)); \
    } while (0)

#ifdef HDMI_FPGA_SUPPORT
#define hdmi_if_fpga_return(ret)      \
    do {                              \
        if (FPGA_SUPPORT) {           \
            hdmi_warn("FPGA CFG!\n"); \
            return(ret);              \
        }                             \
    } while (0)

#define hdmi_if_fpga_return_void()    \
    do {                              \
        if (FPGA_SUPPORT) {           \
            hdmi_warn("FPGA CFG!\n"); \
            return;                   \
        }                             \
    } while (0)
#else
#define hdmi_if_fpga_return(ret)
#define hdmi_if_fpga_return_void()
#endif

#define is_bit_set(var, bit) ({ (var) & (0x1 << (bit)) ? TD_TRUE : TD_FALSE; })

#define hal_call_ret(dev, ret, func, param...)             \
    do {                                                   \
        if ((dev) != TD_NULL && (dev)->hal != TD_NULL &&   \
            (dev)->hal->func != TD_NULL) {                 \
            (ret) = (dev)->hal->func((dev)->hal, ##param); \
        } else {                                           \
            (ret) = OT_ERR_HDMI_NULL_PTR;                  \
        }                                                  \
    } while (0)

#define hal_call_void(dev, func, param...)                 \
    do {                                                   \
        if ((dev) != TD_NULL && (dev)->hal != TD_NULL &&   \
            (dev)->hal->func != TD_NULL) {                 \
            (dev)->hal->func((dev)->hal, ##param);         \
        } else {                                           \
            hdmi_warn("null pointer! \n");                 \
        }                                                  \
    } while (0)

#define hdmi_if_zero_return_void(x)        \
    do {                                   \
        if ((x) == 0) {                    \
            hdmi_err("%s is zero!\n", #x); \
            return;                        \
        }                                  \
    } while (0)


#endif /* SMP_A55_LINUX_MPP_CBB_HDMI_SRC_MKP_DRV_HDMI_COMMON_H_BODY_PART01_H_ */
