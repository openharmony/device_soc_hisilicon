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
#include "drv_hdmi_edid.h"
#include "hdmi_product_define.h"
#include "drv_hdmi_intf.h"

/* Constants for reported magic-number values. */
#define HDMI_EDID_DATA_INDEX_127               127
#define HDMI_EDID_DATA_INDEX_2                 2
#define HDMI_EDID_DATA_INDEX_3                 3
#define HDMI_EDID_DATA_INDEX_32                32
#define HDMI_EDID_DATA_INDEX_4                 4
#define HDMI_EDID_DATA_INDEX_5                 5
#define HDMI_EDID_DATA_INDEX_6                 6
#define HDMI_EDID_DATA_INDEX_7                 7

/* private define */
#define BIT0_MASK                           0x01
#define BIT1_MASK                           0x02
#define BIT2_MASK                           0x04
#define BIT3_MASK                           0x08
#define BIT4_MASK                           0x10
#define BIT5_MASK                           0x20
#define BIT6_MASK                           0x40
#define BIT7_MASK                           0x80
#define EDID_EXTENSION_BLK_ADDR             0x7e
/* VENDOR  INFO */
#define EDID_VEND_NAME_CHAR_MASK            0x1F
#define EDID_VEND_CHAR_LOW_INVALID          0
#define EDID_VEND_CHAR_HIGH_INVALID         27
#define EDID_VEND_YEAR_BASE                 1990
/* STD TIMING */
#define EDID_STDTMING_UNUSED_FLAG           0x01
#define EDID_STDTMING_RATE_BASE             60
#define EDID_HORACTIVE_FACTOR               8
#define EDID_HORACTIVE_BASE                 31
#define EDID_REFRESH_RATE_MASK              0x3F
#define EDID_ASPECT_RATIO_MASK              0xC0
#define EDID_ASPECT_RATIO_16_10             0
#define EDID_ASPECT_RATIO_4_3               1
#define EDID_ASPECT_RATIO_5_4               2
#define EDID_ASPECT_RATIO_16_9              3
/* detailed timing descriptor */
#define EDID_FST_BLK_DTD_OFFSET             0x36
#define EDID_DTD_SIZE                       18
#define EDID_PIXCLK_KHZ_FACTOR              10
#define EDID_UPPER_NIBBLE_MASK              0xF0
#define EDID_LOWER_NIBBLE_MASK              0x0F
#define EDID_HSO_MASK                       0xC0
#define EDID_HSPW_MASK                      0x30
#define EDID_VSO_MASK                       0x0C
#define EDID_VSPW_MASK                      0x03
#define EDID_STEREO_MASK                    0x60
#define EDID_STEREO_SEQUENTIAL_R            0x02
#define EDID_STEREO_SEQUENTIAL_L            0x04
#define EDID_STEREO_INTERLEAVED_2R          0x03
#define EDID_STEREO_INTERLEAVED_2L          0x05
#define EDID_STEREO_INTERLEAVED_4           0x06
#define EDID_STEREO_INTERLEAVED_SBS         0x07
#define EDID_SYNC_SIGNAL_TYPE_MASK          0x0E
#define EDID_SYNC_DCS_WS_0                  0x00
#define EDID_SYNC_DCS_WS_1                  0x01
#define EDID_SYNC_DCS_DS_2                  0x02
#define EDID_SYNC_DCS_DS_3                  0x03
#define EDID_SYNC_DSS_VN_HN_4               0x04
#define EDID_SYNC_DSS_VN_HP_5               0x05
#define EDID_SYNC_DSS_VP_HN_6               0x06
#define EDID_SYNC_DSS_VP_HP_7               0x07
/* monitor descriptor */
#define EDID_TYPE_MONITOR_SERNUM            0xFF
#define EDID_TYPE_MONITOR_ASIIC             0xFE
#define EDID_TYPE_MONITOR_RANGE             0xFD
#define EDID_TYPE_MONITOR_NAME              0xFC
/* EXTENSION BLOCK */
#define EDID_CEA_EXTVERSION3_TAG            0x02
#define EDID_CEA_EXTVERSION3_REVISION       0x03
#define EDID_DB_LEN_MASK                    0x1F
#define EDID_DB_TAG_CODE_MASK               0xE0
/* tag code */
#define EDID_REVERSED_DATA_BLOCK            0x00
#define EDID_AUDIO_DATA_BLOCK               0x01
#define EDID_VIDEO_DATA_BLOCK               0x02
#define EDID_VENDOR_DATA_BLOCK              0x03
#define EDID_SPEAKER_DATA_BLOCK             0x04
#define EDID_VESA_DTC_DATA_BLOCK            0x05
#define EDID_USE_EXT_DATA_BLOCK             0x07
/* ext tag code */
#define EDID_VIDEO_CAPABILITY_DB            0x00
#define EDID_COLORIMETRY_DB                 0x05
#define EDID_VIDEO_FMT_PREFER_DB            0x0d
#define EDID_Y420_VIDEO_DB                  0x0e
#define EDID_Y420_CAPABILITY_MAP_DB         0x0f
#define EDID_RESERVED_HDMI_AUDIO_DB         0x12
#define EDID_INFOFRAME_DB                   0x20
/* AUDIO DATA BLOCK */
#define EDID_AUDIO_FORMAT_MASK              0x78
#define EDID_AUDIO_CHANNEL_MASK             0x07
#define EDID_AUDIO_EXT_TYPE_CODE            0xf8
#define EDID_AUDIO_BITRATE_FACTOR           8
/* VIDEO DATA BLOCK */
#define EDID_VIC_NATIVE_MAX                 64
#define EDID_VIC_LOWER7_MASK                0x7F
#define EDID_VIC_INVALID_LOW                128
#define EDID_VIC_INVALID_HIGH               192
#define EDID_VIC_INVALID_ZERO               0
/* HFVSDB */
#define EDID_HFVSDB_VERSION                 1
#define EDID_MAX_HDMI14_TMDS_RATE           340
#define EDID_TMDS_FACTOR                    5
#define EDID_IEEE_VSDB_1ST                  0x03
#define EDID_IEEE_VSDB_2ND                  0x0c
#define EDID_IEEE_VSDB_3RD                  0x00
#define EDID_IEEE_HFVSDB_1ST                0xd8
#define EDID_IEEE_HFVSDB_2ND                0x5d
#define EDID_IEEE_HFVSDB_3RD                0xc4
/* VSDB */
#define EDID_CEC_INVALID_ADDR               0xF
#define EDID_IMG_SIZE_MASK                  0x18
#define EDID_HDMI_3D_LEN_MASK               0x1F
#define EDID_HDMI_VIC_LEN_MASK              0xE0
#define EDID_3DMULTI_PRESENT_LOWER8         0x01
#define EDID_3DMULTI_PRESENT_UPPER8         0x02
#define EDID_COLOR_OFFSET_NUM_1             2
#define EDID_COLOR_OFFSET_NUM_2             4
#define EDID_COLOR_OFFSET_NUM_3             6
#define EDID_COLOR_LOW_BIT_MASK             0x03
#define EDID_SHORT_AUDIO_DESCRIPTOR_SIZE    3
#define EDID_HEAD_BLOCK_SIZE                8
#define EDID_DOLBY_CAP_OFFSET_1             1
#define EDID_DOLBY_CAP_OFFSET_2             2
#define EDID_DOLBY_CAP_OFFSET_3             4
#define EDID_DOLBY_CAP_OFFSET_4             5
#define EDID_ESTABLISH_TIMING               3
#define EDID_AUDIO_DESCRIPTOR_SIZE          3
#define EDID_EXT_BLOCK_OFFSET               4
#define EDID_VENDOR_BLOCK_LEN               3
#define EDID_MONITOR_LEN                    3

typedef struct {
    td_u8 pixel_clk[2];
    td_u8 h_active;
    td_u8 h_blank;
    td_u8 h_active_blank;
    td_u8 v_active;
    td_u8 v_blank;
    td_u8 v_active_blank;
    td_u8 h_sync_offset;
    td_u8 h_sync_pulse_width;
    td_u8 vs_offset_pulse_width;
    td_u8 hs_offset_vs_offset;
    td_u8 h_image_size;
    td_u8 v_image_size;
    td_u8 h_v_image_size;
    td_u8 h_border;
    td_u8 v_border;
    td_u8 flags;
} __attribute__((packed)) edid_dtd_timing_block;

typedef struct {
    td_u8 flags[3];
    td_u8 data_tag;
    td_u8 flag;
    td_u8 des_data[13];
} __attribute__((packed)) edid_monitor_des_block;

typedef struct {
    td_u8 header[8];
    /* vendor & product info */
    td_u8 mfg_id[2];
    td_u8 prod_code[2];
    /* byte order */
    td_u8 serial[4];
    td_u8 mfg_week;
    td_u8 mfg_year;
    /* EDID version */
    td_u8 version;
    td_u8 revision;
    /* display info: */
    td_u8 input;
    td_u8 width_cm;
    td_u8 height_cm;
    td_u8 gamma;
    td_u8 features;
    /* color characteristics */
    td_u8 red_green_low;
    td_u8 black_white_low;
    td_u8 red_x;
    td_u8 red_y;
    td_u8 green_x;
    td_u8 green_y;
    td_u8 blue_x;
    td_u8 blue_y;
    td_u8 white_x;
    td_u8 white_y;
    td_u8 est_timing[3];
    td_u8 std_timing[16];
    td_u8 detailed_timing1[18];
    td_u8 detailed_timing2[18];
    td_u8 detailed_timing3[18];
    td_u8 detailed_timing4[18];
    /* number of 128 byte ext. blocks */
    td_u8 ext_blocks;
    /* checksum */
    td_u8 chk_sum;
} __attribute__((packed)) edid_first_block_info;

__attribute__((unused)) static const td_char *g_establish_timing_str[] = {
    "(IBM) 720x400p_70Hz 9:5",
    "(IBM-XGA2) 720x400p_88Hz 9:5",
    "(IBM-VGA) 640x480p_60Hz 4:3",
    "(VESA) 640x480p_72Hz 4:3",
    "(VESA) 640x480p_75Hz 4:3",
    "(VESA) 800x600p_56Hz 4:3",
    "(VESA) 800x600p_60Hz 4:3",
    "(VESA) 800x600p_72Hz 4:3",
    "(VESA) 800x600p_75Hz 4:3",
    "(VESA) 1024x768i_87Hz 4:3",
    "(VESA) 1024x768p_60Hz 4:3",
    "(VESA) 1024x768p_70Hz 4:3",
    "(VESA) 1024x768p_75Hz 4:3",
    "(VESA) 1280x1024p_75Hz 5:4",
    "(Apple-Mac II) 640x480p_67Hz 4:3",
    "(Apple-Mac II) 832x624p_75Hz 4:3",
    "(Apple-Mac II) 1152x870p_75Hz 4:3"
};

static td_bool edid_flags_mask_check(td_u8 data, td_u8 mask)
{
    return (data & mask) ? TD_TRUE : TD_FALSE;
}

static td_s32 edid_block_check_sum(const td_u8 *data, hdmi_edid_status *status)
{
    td_u8 i;
    td_u32 check_sum = 0;

    for (i = 0; i < HDMI_EDID_BLOCK_SIZE; i++) {
        check_sum += data[i];
    }

    /* EDID header */
    edid_info("    %-20s : %02X\n", "check sum", data[HDMI_EDID_DATA_INDEX_127]);

    if ((check_sum & 0xff) != 0x00) {
        if (data[HDMI_EDID_DATA_INDEX_32] != 0x00) {
            status->parse_err = EDID_PARSE_ERR_CHECKSUM;
            edid_err("check sum fail\n");
            return TD_FAILURE;
        } else {
            /*
             * if the data is 0x00 at 0x20 in block0, which is modified by clear scramble in boot.
             * but the real EDID is crrect.
             */
            edid_info("EDID data maybe changed by clr scramble.\n");
        }
    }

    return TD_SUCCESS;
}

static td_s32 edid_header_check(const td_u8 *data, hdmi_edid_status *status)
{
    td_u32 i;
    const td_u8 blkheader[] = { 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00 };

    edid_info("EDID header\n");
    for (i = 0; i < EDID_HEAD_BLOCK_SIZE; i++) {
        if (data[i] != blkheader[i]) {
            status->parse_err = EDID_PARSE_ERR_HEADER;
            edid_err("header chk: DATA[%u]=0x%x\n", i, data[i]);
        }
    }

    return TD_SUCCESS;
}

static td_s32 edid_vendor_info(hdmi_edid_info *info)
{
    td_u16 i;
    td_u16 data;
    hdmi_edid_status *status = TD_NULL;
    edid_first_block_info *fst_blk = TD_NULL;
    hdmi_edid_manufacture_info *vendor = TD_NULL;

    edid_info("vendor / product identification\n");
    fst_blk = (edid_first_block_info *)info->edid_raw;
    status = &info->status;
    vendor = &info->capability.mfrs_info;

    /* 8, offset. express dada 15-8 bits */
    data = (fst_blk->mfg_id[0] << 8) | (fst_blk->mfg_id[1]);

    /* 2, word width */
    for (i = 0; i <= 2; i++) {
        /* 2, word width */
        vendor->mfrs_name[2 - i] = ((data & (EDID_VEND_NAME_CHAR_MASK << (5 * i))) >> (5 * i)); /* 5, bit a character */

        /* 'A'~'Z' is referred to 1~26 */
        if ((vendor->mfrs_name[2 - i] > EDID_VEND_CHAR_LOW_INVALID) &&
            (vendor->mfrs_name[2 - i] < EDID_VEND_CHAR_HIGH_INVALID)) { /* 2, word width */
            vendor->mfrs_name[2 - i] += 'A' - 1; /* 2, word width */
        } else {
            status->parse_warn |= 1 << EDID_PARSE_WARN_VENDOR_INVALID;
        }
    }
    /* 8, offset. express product code 15-8 bits */
    vendor->product_code = (fst_blk->prod_code[1] << 8) | fst_blk->prod_code[0];
    /* 24, 16, is serial number 3 and 4 byte */
    vendor->serial_number = (fst_blk->serial[3] << 24) | (fst_blk->serial[2] << 16) |
                            (fst_blk->serial[1] << 8) | (fst_blk->serial[0]); /* 8, 0, is serial number 1 and 0 byte */
    vendor->week = fst_blk->mfg_week;
    vendor->year = fst_blk->mfg_year + EDID_VEND_YEAR_BASE;

    edid_info("    %-20s : %s\n", "ID manufacturer name", vendor->mfrs_name);
    edid_info("    %-20s : 0x%x\n", "ID product code", vendor->product_code);
    edid_info("    %-20s : 0x%x\n", "ID serial number", vendor->serial_number);
    edid_info("    %-20s : %02u\n", "week of manufacture", vendor->week);
    edid_info("    %-20s : %u\n", "year of manufacture", vendor->year);

    return TD_SUCCESS;
}

static td_s32 edid_version(hdmi_edid_info *info)
{
    hdmi_sink_capability *cap = TD_NULL;
    edid_first_block_info *edid_fst_info = TD_NULL;
    hdmi_edid_status *status = TD_NULL;

    cap = &info->capability;
    edid_fst_info = (edid_first_block_info *)info->edid_raw;
    status = &info->status;
    cap->version = edid_fst_info->version;
    cap->revision = edid_fst_info->revision;
    edid_info("    EDID version\n");
    /* 1, is info sink version, 3 is reversion count */
    if ((cap->version != 1) || (cap->revision != 3)) {
        edid_err("EDID first blk is not version1.3:%u.%u\n!", cap->version, cap->revision);
        status->parse_err = EDID_PARSE_ERR_FST_BLK_VER;
    }
    edid_info("    %-20s : %02u\n", "version no.", cap->version);
    edid_info("    %-20s : %02u\n", "revision no.", cap->revision);

    return TD_SUCCESS;
}

static td_s32 edid_basic_disp_para(hdmi_edid_info *info)
{
    hdmi_sink_capability *cap = TD_NULL;
    edid_first_block_info *fst_blk = TD_NULL;

    cap = &info->capability;
    fst_blk = (edid_first_block_info *)info->edid_raw;
    cap->basic_disp_para.max_image_width = fst_blk->width_cm;
    cap->basic_disp_para.max_image_height = fst_blk->height_cm;

    return TD_SUCCESS;
}

static td_s32 edid_phosphor(hdmi_edid_info *info)
{
    hdmi_sink_capability *cap = TD_NULL;
    edid_first_block_info *fst_blk = TD_NULL;

    edid_info("    phosphor or filter chromaticity\n");
    cap = &info->capability;

    fst_blk = (edid_first_block_info *)info->edid_raw;
    cap->phos_or_chrom_cap.red_x   = (fst_blk->red_x << EDID_COLOR_OFFSET_NUM_1) |
                                     ((fst_blk->red_green_low >> EDID_COLOR_OFFSET_NUM_3) & EDID_COLOR_LOW_BIT_MASK);
    cap->phos_or_chrom_cap.red_y   = (fst_blk->red_y << EDID_COLOR_OFFSET_NUM_1) |
                                     ((fst_blk->red_green_low >> EDID_COLOR_OFFSET_NUM_2) & EDID_COLOR_LOW_BIT_MASK);
    cap->phos_or_chrom_cap.green_x = (fst_blk->green_x << EDID_COLOR_OFFSET_NUM_1) |
                                     ((fst_blk->red_green_low >> EDID_COLOR_OFFSET_NUM_1) & EDID_COLOR_LOW_BIT_MASK);
    cap->phos_or_chrom_cap.green_y = (fst_blk->green_y << EDID_COLOR_OFFSET_NUM_1) |
                                     (fst_blk->red_green_low & EDID_COLOR_LOW_BIT_MASK);
    cap->phos_or_chrom_cap.blue_x  = (fst_blk->blue_x << EDID_COLOR_OFFSET_NUM_1) |
                                     ((fst_blk->black_white_low >> EDID_COLOR_OFFSET_NUM_3) & EDID_COLOR_LOW_BIT_MASK);
    cap->phos_or_chrom_cap.blue_y  = (fst_blk->blue_y << EDID_COLOR_OFFSET_NUM_1) |
                                     ((fst_blk->black_white_low >> EDID_COLOR_OFFSET_NUM_2) & EDID_COLOR_LOW_BIT_MASK);
    cap->phos_or_chrom_cap.white_x = (fst_blk->white_x << EDID_COLOR_OFFSET_NUM_1) |
                                     ((fst_blk->black_white_low >> EDID_COLOR_OFFSET_NUM_1) & EDID_COLOR_LOW_BIT_MASK);
    cap->phos_or_chrom_cap.white_y = (fst_blk->white_y << EDID_COLOR_OFFSET_NUM_1) |
                                     (fst_blk->black_white_low & EDID_COLOR_LOW_BIT_MASK);

    return TD_SUCCESS;
}

static td_s32 edid_std_timing(hdmi_edid_info *info)
{
    td_u8 i;
    td_u8 asp_ratio;
    td_u8                 *data    = TD_NULL;
    hdmi_sink_capability  *cap     = TD_NULL;
    edid_first_block_info *fst_blk = TD_NULL;

    cap = &info->capability;
    fst_blk = (edid_first_block_info *)info->edid_raw;
    data = fst_blk->std_timing;

    (td_void)memset_s(cap->std_timing, sizeof(cap->std_timing), 0, sizeof(cap->std_timing));

    edid_info("    standard VESA timing identifications\n");
    for (i = 0; i < HDMI_EDID_MAX_STDTIMNG_COUNT; i++, data += 2) { /* std_timing data length is 2 */
        if ((data[0] != EDID_STDTMING_UNUSED_FLAG) || (data[1] != EDID_STDTMING_UNUSED_FLAG)) {
            cap->std_timing[i].hor_active = (data[0] + EDID_HORACTIVE_BASE) * EDID_HORACTIVE_FACTOR;
            cap->std_timing[i].refresh_rate = (data[1] & EDID_REFRESH_RATE_MASK) + EDID_STDTMING_RATE_BASE;
            asp_ratio = (data[1] & EDID_ASPECT_RATIO_MASK) >> 6; /* asp_ratio offset is 6 */

            switch (asp_ratio) {
                case EDID_ASPECT_RATIO_16_10:
                    cap->std_timing[i].ver_active = cap->std_timing[i].hor_active * 10 / 16; /* aspect ratio, 16 : 10 */
                    break;
                case EDID_ASPECT_RATIO_5_4:
                    cap->std_timing[i].ver_active = cap->std_timing[i].hor_active * 4 / 5; /* aspect ratio, 5 : 4 */
                    break;
                case EDID_ASPECT_RATIO_4_3:
                    cap->std_timing[i].ver_active = cap->std_timing[i].hor_active * 3 / 4; /* aspect ratio, 4 : 3 */
                    break;
                case EDID_ASPECT_RATIO_16_9:
                    cap->std_timing[i].ver_active = cap->std_timing[i].hor_active * 9 / 16; /* aspect ratio, 16 : 9 */
                    break;
                default:
                    break;
            }
            edid_info("STDTIMNG field[%u] hor_active:%u\n", i, cap->std_timing[i].hor_active);
            edid_info("STDTIMNG field[%u] ver_active:%u\n", i, cap->std_timing[i].ver_active);
            edid_info("STDTIMNG field[%u] refresh_rate:%u\n", i, cap->std_timing[i].refresh_rate);
        }
    }

    return TD_SUCCESS;
}

static td_s32 edid_establish_timing(hdmi_edid_info *info)
{
    td_u32 i;
    td_u32                 dat32   = 0;
    hdmi_sink_capability  *cap     = TD_NULL;
    edid_first_block_info *fst_blk = TD_NULL;

    cap = &info->capability;
    fst_blk = (edid_first_block_info *)info->edid_raw;

    for (i = 0; i <= HDMI_EDID_ESTABTIMG_BUTT; i++) {
        if (cap->estab_num >= HDMI_EDID_ESTABTIMG_BUTT) {
            edid_warn("establish_timing estab_num over:%u!\n", cap->estab_num);
            break;
        }
        /* if BYTE(i/8), BIT[i%8] is 1 */
        if (fst_blk->est_timing[i / 8] & (0x01 << (i % 8))) {
            cap->estab_timing[cap->estab_num++] = i;
        }
    }

    edid_info("    established VESA timings\n");
    for (i = 0; i < EDID_ESTABLISH_TIMING; i++) {
        /* 8'b, 16'b, 24'b, is data bit[16:9], bit[24:17], bit[32~25] */
        dat32 |= ((td_u32)fst_blk->est_timing[i]) << (8 * (EDID_ESTABLISH_TIMING - i));
    }
    i = 0;
    while (dat32) {
        if (dat32 & 0x80000000) {
            if (i < hdmi_array_size(g_establish_timing_str)) {
                edid_info("    Established Timing: %s\n", g_establish_timing_str[i]);
            }
        }
        dat32 <<= 1;
        i++;
    }

    return TD_SUCCESS;
}

static td_void edid_digital_sync_signal_def(const edid_dtd_timing_block *dtd, hdmi_edid_pre_timing *cap_pre_timing)
{
    switch ((dtd->flags & EDID_SYNC_SIGNAL_TYPE_MASK) >> 1) { /* 1, sync_signal flags offset */
        /* DCS: digital composite sync */
        case EDID_SYNC_DCS_WS_0:
        case EDID_SYNC_DCS_WS_1:
            cap_pre_timing->ihs = TD_FALSE;
            cap_pre_timing->ivs = TD_FALSE;
            break;
        case EDID_SYNC_DCS_DS_2:
        case EDID_SYNC_DCS_DS_3:
            break;
        /* DSS: digital separate sync */
        case EDID_SYNC_DSS_VN_HN_4:
            cap_pre_timing->ihs = TD_FALSE;
            cap_pre_timing->ivs = TD_FALSE;
            break;
        case EDID_SYNC_DSS_VN_HP_5:
            cap_pre_timing->ihs = TD_TRUE;
            cap_pre_timing->ivs = TD_FALSE;
            break;
        case EDID_SYNC_DSS_VP_HN_6:
            cap_pre_timing->ihs = TD_FALSE;
            cap_pre_timing->ivs = TD_TRUE;
            break;
        case EDID_SYNC_DSS_VP_HP_7:
            cap_pre_timing->ihs = TD_TRUE;
            cap_pre_timing->ivs = TD_TRUE;
            break;
        default:
            break;
    }

    return;
}

static td_void edid_timing_param(const edid_dtd_timing_block *dtd, hdmi_edid_pre_timing *pre_timing)
{
    td_u32 temp;

    /* VFB, 2+4 bits */
    temp = dtd->hs_offset_vs_offset & EDID_VSO_MASK;
    temp <<= 2; /* 2'b, bit[6:5] */
    temp |= (dtd->vs_offset_pulse_width & EDID_UPPER_NIBBLE_MASK) >> 4; /* 4'b, bit[4:1] */
    pre_timing->vfb = temp;

    /* VBB, 4+8 bits */
    temp = dtd->v_active_blank & EDID_LOWER_NIBBLE_MASK;
    temp <<= 8; /* 8'b, bit[12:9] */
    temp |= dtd->v_blank;
    pre_timing->vbb = temp - pre_timing->vfb;

    /* VACT, 4+8 bits */
    temp = dtd->v_active_blank & EDID_UPPER_NIBBLE_MASK;
    temp <<= 4; /* 4'b, bit[12:9] */
    temp |= dtd->v_active;
    pre_timing->vact = temp;

    /* HFB, 2+8 bits */
    temp = dtd->hs_offset_vs_offset & EDID_HSO_MASK;
    temp <<= 2; /* 2'b, bit[12:9] */
    temp |= dtd->h_sync_offset;
    pre_timing->hfb = temp;

    /* HBB, 4+8 bits */
    temp = (dtd->h_active_blank & EDID_LOWER_NIBBLE_MASK);
    temp <<= 8; /* 8'b, bit[12:9] */
    temp |= dtd->h_blank;
    pre_timing->hbb = temp - pre_timing->hfb;

    /* HACT, 4+8 bits */
    temp = dtd->h_active_blank & EDID_UPPER_NIBBLE_MASK;
    temp <<= 4; /* 4'b, bit[12:9] */
    temp |= dtd->h_active;
    pre_timing->hact = temp;

    /* VPW, 2+4 bits */
    temp = dtd->hs_offset_vs_offset & EDID_VSPW_MASK;
    temp <<= 4; /* 4'b, bit[6:5] */
    temp |= (dtd->vs_offset_pulse_width & EDID_LOWER_NIBBLE_MASK);
    pre_timing->vpw = temp;

    /* HPW, 2+8 bits */
    temp = dtd->hs_offset_vs_offset & EDID_HSPW_MASK;
    temp <<= 4; /* 4'b, bit[10:9] */
    temp |= dtd->h_sync_pulse_width;
    pre_timing->hpw = temp;

    /* H image size(mm), 4+8 bits */
    temp = dtd->h_v_image_size & EDID_UPPER_NIBBLE_MASK;
    temp <<= 4; /* 4'b, bit[12:9] */
    temp |= dtd->h_image_size;
    pre_timing->image_width = temp;

    /* V image size(mm), 4+8 bits */
    temp = dtd->h_v_image_size & EDID_LOWER_NIBBLE_MASK;
    temp <<= 8; /* 8'b, bit[12:9] */
    temp |= dtd->v_image_size;
    pre_timing->image_height = temp;

    /* interlaced flag */
    pre_timing->interlace = (dtd->flags & BIT7_MASK) ? TD_TRUE : TD_FALSE;

    /* digital sync signal definitions */
    if ((dtd->flags & BIT4_MASK) != 0) {
        edid_digital_sync_signal_def(dtd, pre_timing);
    }
    pre_timing->idv = TD_FALSE;

    return;
}

static td_s32 edid_detail_timing(hdmi_sink_capability *cap, hdmi_edid_status *status, const td_u8 *data, td_u8 len)
{
    errno_t ret;
    td_u32 temp;
    td_u8 monitor_flag[EDID_MONITOR_LEN] = { 0x00, 0x00, 0x00 };
    hdmi_edid_pre_timing *cap_pre_timing = TD_NULL;
    edid_dtd_timing_block *dtd = TD_NULL;
    edid_monitor_des_block *monitor_desc = TD_NULL;

    if (cap->perfer_timing_num >= HDMI_EDID_MAX_DETAIL_TIMING_COUNT ||
        len < EDID_DTD_SIZE) {
        status->parse_warn |= 1 << EDID_PARSE_WARN_DTD_OVER;
        edid_warn("dt_ds over max dt_ds cnt or less len:%u!\n", len);
        return TD_SUCCESS;
    }

    /* case1: monitor descriptor */
    if (osal_memcmp(data, monitor_flag, sizeof(monitor_flag)) == 0) {
        /* case 1 */
        if (data[HDMI_EDID_DATA_INDEX_3] == EDID_TYPE_MONITOR_NAME) { /* display product name */
            monitor_desc = (edid_monitor_des_block *)data;
            ret = memcpy_s(cap->mfrs_info.sink_name, HDMI_EDID_MAX_SINKNAME_COUNT,
                monitor_desc->des_data, HDMI_EDID_MAX_SINKNAME_COUNT - 1);
            hdmi_unequal_eok_return(ret, OT_ERR_HDMI_INVALID_PARA);
            cap->mfrs_info.sink_name[HDMI_EDID_MAX_SINKNAME_COUNT - 1] = '\0';
        }

        return TD_SUCCESS;
    }

    /* case2: detailed timing description */
    dtd = (edid_dtd_timing_block *)data;
    cap_pre_timing = &cap->perfer_timing[cap->perfer_timing_num];

    /* pixel clock, k_hz */
    temp = dtd->pixel_clk[1];
    temp <<= 8; /* 8'b, BIT[32:9] */
    temp |= dtd->pixel_clk[0];
    temp *= EDID_PIXCLK_KHZ_FACTOR;
    cap_pre_timing->pixel_clk = temp;

    if (temp > 0) {
        cap->perfer_timing_num++;
    } else {
        edid_info("dt_ds empty dt_ds, cur_num:%u\n", cap->perfer_timing_num);
        return TD_SUCCESS;
    }
    edid_timing_param(dtd, cap_pre_timing);

    return TD_SUCCESS;
}

static td_s32 edid_ext_num(hdmi_edid_info *info)
{
    hdmi_edid_status *status = TD_NULL;
    td_u8 *data = TD_NULL;
    hdmi_sink_capability *cap = TD_NULL;

    status = &info->status;
    data = info->edid_raw;
    cap = &info->capability;

    cap->ext_block_num = (td_u8)data[EDID_EXTENSION_BLK_ADDR];
    cap->support_hdmi = TD_FALSE;
    edid_info("    extension flag\n");
    edid_info("    %-20s : %02u\n", "extensions", cap->ext_block_num);

    if (cap->ext_block_num == 0) {
        status->parse_warn |= 1 << EDID_PARSE_WARN_EXT_BLK_ZERO;
        edid_warn("ext-BLK zero: 0x%02x \n", cap->ext_block_num);
    } else if (cap->ext_block_num > (HDMI_EDID_MAX_BLOCK_NUM - 1)) {
        status->parse_warn |= 1 << EDID_PARSE_WARN_EXT_BLK_OVER;
        cap->ext_block_num = HDMI_EDID_MAX_BLOCK_NUM - 1;
        edid_warn("ext-BLK cnt over: 0x%02x \n", cap->ext_block_num);
    }

    return TD_SUCCESS;
}

static td_s32 edid_first_blk_parse(hdmi_edid_info *info)
{
    td_s32                 ret;
    td_u8                 *data     = TD_NULL;
    hdmi_edid_status      *status   = TD_NULL;
    hdmi_sink_capability  *cap      = TD_NULL;
    edid_first_block_info *fst_info = TD_NULL;

    edid_info("   first block parse\n");
    status = &info->status;
    data = info->edid_raw;
    cap = &info->capability;
    fst_info = (edid_first_block_info *)data;

    ret = edid_block_check_sum(data, status);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* 00h-07h header */
    ret = edid_header_check(data, status);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* 08h-11h vendor info */
    ret = edid_vendor_info(info);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* 12h-13h version */
    ret = edid_version(info);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* 14h-18h basic display parameters/features */
    ret = edid_basic_disp_para(info);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* 19h-22h color characteristics */
    ret = edid_phosphor(info);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* 23h-25h established timings */
    ret = edid_establish_timing(info);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* 26h-35h standard timing identification */
    ret = edid_std_timing(info);
    hdmi_if_failure_return(ret, TD_FAILURE);
    /* 36h-47h first detailed timing descriptions */
    ret = edid_detail_timing(cap, status, fst_info->detailed_timing1, EDID_DTD_SIZE);
    hdmi_if_failure_return(ret, TD_FAILURE);
    ret = edid_detail_timing(cap, status, fst_info->detailed_timing2, EDID_DTD_SIZE);
    hdmi_if_failure_return(ret, TD_FAILURE);
    ret = edid_detail_timing(cap, status, fst_info->detailed_timing3, EDID_DTD_SIZE);
    hdmi_if_failure_return(ret, TD_FAILURE);
    ret = edid_detail_timing(cap, status, fst_info->detailed_timing4, EDID_DTD_SIZE);
    hdmi_if_failure_return(ret, TD_FAILURE);
    ret = edid_ext_num(info);
    hdmi_if_failure_return(ret, TD_FAILURE);

    return ret;
}

#if (!defined(CONFIG_HDMI_DEBUG_DISABLE) && !defined(HDMI_LOG_SUPPORT))
#ifndef OT_ADVCA_FUNCTION_RELEASE
static td_char *g_audio_code_str[] = {
    "reserved",
    "linear PCM",
    "AC3",
    "MPEG1 (layers 1 & 2)",
    "MP3 (MPEG1 layer 2)",
    "MPEG2 (multichannel)",
    "AAC",
    "DTS",
    "ATREC",
    "one bit audio",
    "dolby digital+",
    "DTS-HD",
    "MAT (MLP)",
    "DST",
    "WMA pro",
    "reserved"
};
#else
static td_char *g_audio_code_str[] = {};
static td_char *g_video_code_str[] = {};
static td_char *g_video_code_4k_str[] = {};
#endif
#endif

static td_void edid_audio_rate(td_u8 byte, td_u8 frm_code, hdmi_edid_audio_info *audio_info)
{
    td_u32 count = 0;

    if (byte & BIT0_MASK) {
        audio_info->support_sample_rate[count] = HDMI_SAMPLE_RATE_32K;
        edid_info("    %-20s : %u hz\n", "sample frequencies", audio_info->support_sample_rate[count]);
        count++;
    }
    if (byte & BIT1_MASK) {
        audio_info->support_sample_rate[count] = HDMI_SAMPLE_RATE_44K;
        edid_info("    %-20s : %u hz\n", "sample frequencies", audio_info->support_sample_rate[count]);
        count++;
    }
    if (byte & BIT2_MASK) {
        audio_info->support_sample_rate[count] = HDMI_SAMPLE_RATE_48K;
        edid_info("    %-20s : %u hz\n", "sample frequencies", audio_info->support_sample_rate[count]);
        count++;
    }
    if (byte & BIT3_MASK) {
        audio_info->support_sample_rate[count] = HDMI_SAMPLE_RATE_88K;
        edid_info("    %-20s : %u hz\n", "sample frequencies", audio_info->support_sample_rate[count]);
        count++;
    }
    if (byte & BIT4_MASK) {
        audio_info->support_sample_rate[count] = HDMI_SAMPLE_RATE_96K;
        edid_info("    %-20s : %u hz\n", "sample frequencies", audio_info->support_sample_rate[count]);
        count++;
    }
    /* CEA-861-F add for HDMI2.0 */
    if (frm_code >= 1 && frm_code <= 14) { /* frm_code size extent 1~14 */
        if (byte & BIT5_MASK) {
            audio_info->support_sample_rate[count] = HDMI_SAMPLE_RATE_176K;
            edid_info("    %-20s : %u hz\n", "sample frequencies", audio_info->support_sample_rate[count]);
            count++;
        }
        if (byte & BIT6_MASK) {
            audio_info->support_sample_rate[count] = HDMI_SAMPLE_RATE_192K;
            edid_info("    %-20s : %u hz\n", "sample frequencies", audio_info->support_sample_rate[count]);
            count++;
        }
    }
    audio_info->support_sample_rate_num = count;

    return;
}

static td_void edid_audio_depth_and_max_rate(td_u8 byte, td_u8 frm_code, hdmi_edid_audio_info *audio_info)
{
    td_u32 count = 0;

    if (frm_code == 1) {
        if (byte & BIT0_MASK) {
            audio_info->support_bit_depth[count] = HDMI_AUDIO_BIT_DEPTH_16;
            edid_info("    %-20s : %u bit\n", "bit depth", audio_info->support_bit_depth[count]);
            count++;
        }
        if (byte & BIT1_MASK) {
            audio_info->support_bit_depth[count] = HDMI_AUDIO_BIT_DEPTH_20;
            edid_info("    %-20s : %u bit\n", "bit depth", audio_info->support_bit_depth[count]);
            count++;
        }
        if (byte & BIT2_MASK) {
            audio_info->support_bit_depth[count] = HDMI_AUDIO_BIT_DEPTH_24;
            edid_info("    %-20s : %u bit\n", "bit depth", audio_info->support_bit_depth[count]);
            count++;
        }
        audio_info->support_bit_depth_num = count;
    } else if ((frm_code > 1) && (frm_code < 9)) { /* 9, audio format type 2~8 */
        audio_info->max_bit_rate = byte * EDID_AUDIO_BITRATE_FACTOR;
        edid_info("    %-20s : %u k_hz\n", "maximum bit rate", audio_info->max_bit_rate);
    } else {
        edid_info("  EDID audio format type %u byte3 reserved!\n", frm_code);
    }

    return;
}

static td_s32 edid_audio_db(hdmi_sink_capability *cap, hdmi_edid_status *status, const td_u8 *data, td_u8 len)
{
    td_u8 i;
    td_u8 frm_code;
    td_u8 cur_num;
    hdmi_edid_audio_info *audio_info = TD_NULL;
#if (!defined(CONFIG_HDMI_DEBUG_DISABLE) && !defined(HDMI_LOG_SUPPORT))
    td_char *audio_code_str = TD_NULL;
#endif

    audio_info = cap->audio_info;
    edid_info("    audio data block (length=%02u)\n", len);
    /* each short audio descriptor is 3-bytes long */
    if ((len / EDID_AUDIO_DESCRIPTOR_SIZE) > 10) { /* 10, audio capability count */
        edid_err(" audio capability count(%u) is error!\n", len);
        status->parse_warn |= (1 << EDID_PARSE_WARN_AUDIO_CNT_OVER);
        return TD_FAILURE;
    }

    for (i = 0; i < (len / EDID_AUDIO_DESCRIPTOR_SIZE); i++) {
        cur_num = cap->audio_info_num;
        if (cur_num >= HDMI_EDID_MAX_AUDIO_CAP_COUNT) {
            status->parse_warn |= 1 << EDID_PARSE_WARN_AUDIO_CNT_OVER;
            edid_warn(" audio capability count over max-num:%u \n", HDMI_EDID_MAX_AUDIO_CAP_COUNT);
            break;
        }
        /* byte-0 */
        edid_info("    %-20s : 0x%02X, 0x%02X, 0x%02X\n", "short audio descriptor",
            data[i * EDID_AUDIO_DESCRIPTOR_SIZE], data[i * EDID_AUDIO_DESCRIPTOR_SIZE + 1],
            data[i * EDID_AUDIO_DESCRIPTOR_SIZE + 2]); /* 2, offset */

        frm_code = (data[i * EDID_AUDIO_DESCRIPTOR_SIZE] & EDID_AUDIO_FORMAT_MASK) >> 3; /* 3, bit[4:1] */
        audio_info[cur_num].aud_channel = (data[i * EDID_AUDIO_DESCRIPTOR_SIZE] & EDID_AUDIO_CHANNEL_MASK) + 1;
        audio_info[cur_num].aud_fmt_code = (hdmi_audio_format_code)frm_code;

#if (!defined(CONFIG_HDMI_DEBUG_DISABLE) && !defined(HDMI_LOG_SUPPORT))
        audio_code_str = g_audio_code_str[frm_code];
        edid_info("    %-20s : %02u\n", "max. number of channels", audio_info[cur_num].aud_channel);
        edid_info("    %-20s : %s\n", "audio format code", audio_code_str);
#endif
        if (frm_code > HDMI_AUDIO_CODING_TYPE_STREAM && frm_code < HDMI_AUDIO_CODING_TYPE_BUTT) {
            cap->audio_info_num++;
        } else {
            status->parse_warn |= 1 << EDID_PARSE_WARN_AUDIO_FMT_INVALID;
            edid_warn(" CEA-861-F not support audio frm:%u \n", frm_code);
            break;
        }
        /* byte-1 */
        edid_audio_rate(data[i * EDID_AUDIO_DESCRIPTOR_SIZE + 1], frm_code, &audio_info[cur_num]);
        /* byte-2 */
        edid_audio_depth_and_max_rate(data[i * EDID_AUDIO_DESCRIPTOR_SIZE + 2], frm_code, &audio_info[cur_num]);
    }

    return TD_SUCCESS;
}

static td_s32 edid_video_db(hdmi_sink_capability *cap, hdmi_edid_status *status, const td_u8 *data, td_u8 len)
{
    td_u8  i;
    td_u32 inplicit_native = EDID_VIC_INVALID_ZERO;
    td_u32 vic_all         = EDID_VIC_INVALID_ZERO;
    td_u32 vic_lower       = EDID_VIC_INVALID_ZERO;

    edid_info("    video data block (length=%02u)\n", len);
    for (i = 0; i < len; i++) {
        vic_all = data[i];
        vic_lower = data[i] & EDID_VIC_LOWER7_MASK;

        /* avoid full over */
        if (cap->support_vic_num >= HDMI_EDID_MAX_VIC_COUNT) {
            status->parse_warn |= 1 << EDID_PARSE_WARN_VIC_CNT_OVER;
            edid_warn("VDB vic count over:%u !\n", HDMI_EDID_MAX_VIC_COUNT);
            break;
        }

        /* avoid invalid vic */
        if (vic_all == EDID_VIC_INVALID_ZERO) {
            status->parse_warn |= 1 << EDID_PARSE_WARN_VIC_INVALID;
            edid_warn("VDB the %u vic: %u ! \n", i, vic_all);
            continue;
        }

        /* explicit native */
        if ((cap->native_format == EDID_VIC_INVALID_ZERO) &&
            (vic_all & BIT7_MASK) && (vic_lower <= EDID_VIC_NATIVE_MAX)) {
            cap->native_format = vic_lower;
        }

        /* set the first valid VIC as implicit native */
        if (inplicit_native == EDID_VIC_INVALID_ZERO) {
            inplicit_native = vic_all;
        }

        if ((vic_all & BIT7_MASK) && (vic_lower <= EDID_VIC_NATIVE_MAX)) {
            cap->support_format[cap->support_vic_num] = vic_lower;
        } else {
            cap->support_format[cap->support_vic_num] = vic_all;
        }
        cap->support_vic_num++;
    }

    if (cap->native_format == EDID_VIC_INVALID_ZERO) {
        cap->native_format = inplicit_native;
    }

    return TD_SUCCESS;
}

static td_void edid_hfvs21_db(hdmi_sink_capability *cap, const td_u8 *data)
{
    cap->fapa_start_location     = (data[0] & BIT0_MASK) ? TD_TRUE : TD_FALSE;
    cap->allm                    = (data[0] & BIT1_MASK) ? TD_TRUE : TD_FALSE;
    cap->fva                     = (data[0] & BIT2_MASK) ? TD_TRUE : TD_FALSE;
    cap->cnm_vrr                 = (data[0] & BIT3_MASK) ? TD_TRUE : TD_FALSE;
    cap->cinema_vrr              = (data[0] & BIT4_MASK) ? TD_TRUE : TD_FALSE;
    cap->m_delta                 = (data[0] & BIT5_MASK) ? TD_TRUE : TD_FALSE;
    cap->vrr_min                 = data[1] & 0x3F;
    cap->vrr_max                 = data[1] & 0xC0;
    cap->vrr_max                 = (cap->vrr_max << 2) | data[2]; /* 2'b, bit[6:5] */
    cap->dsc_info.dsc_1p2        = (data[HDMI_EDID_DATA_INDEX_3] & BIT7_MASK) ? TD_TRUE : TD_FALSE;
    cap->dsc_info.dsc_native_420 = (data[HDMI_EDID_DATA_INDEX_3] & BIT6_MASK) ? TD_TRUE : TD_FALSE;
    cap->dsc_info.dsc_all_bpp    = (data[HDMI_EDID_DATA_INDEX_3] & BIT3_MASK) ? TD_TRUE : TD_FALSE;
    cap->dsc_info.dsc_16bpc      = (data[HDMI_EDID_DATA_INDEX_3] & BIT2_MASK) ? TD_TRUE : TD_FALSE;
    cap->dsc_info.dsc_12bpc      = (data[HDMI_EDID_DATA_INDEX_3] & BIT1_MASK) ? TD_TRUE : TD_FALSE;
    cap->dsc_info.dsc_10bpc      = (data[HDMI_EDID_DATA_INDEX_3] & BIT0_MASK) ? TD_TRUE : TD_FALSE;
    cap->dsc_info.dsc_max_slices = (data[HDMI_EDID_DATA_INDEX_4] & EDID_LOWER_NIBBLE_MASK);
    cap->dsc_info.dsc_max_frl_rate = (data[HDMI_EDID_DATA_INDEX_4] & EDID_UPPER_NIBBLE_MASK);
    /* 0x3F, dsc total chunk k byte mask */
    cap->dsc_info.dsc_total_chunk_k_bytes = (data[HDMI_EDID_DATA_INDEX_5] & 0x3F);

    return;
}

static td_s32 edid_hfvs_db(hdmi_sink_capability *cap, hdmi_edid_status *status, const td_u8 *data, td_u8 len)
{
    if (len < 7 || len > 31) { /* 7 31, hfvs db length extent 7~31 */
        status->parse_warn |= 1 << EDID_PARSE_WARN_BLOCKLEN_INVALID;
        edid_warn("HFVSDB len:%u\n", len);
        return TD_SUCCESS;
    }

    /* byte-3 ,version */
    if (data[3] != EDID_HFVSDB_VERSION) {
        status->parse_warn |= 1 << EDID_PARSE_WARN_HFVSDB_INVALID;
        edid_warn("    HFVSDB version:%u\n", data[HDMI_EDID_DATA_INDEX_3]);
    }
    edid_info("    HDMI forum vendor specific data block        %-36s : %02u\n", "version",
        data[HDMI_EDID_DATA_INDEX_3]);
    /* byte-4 */
    cap->max_tmds_clock  = data[4];
    cap->max_tmds_clock *= EDID_TMDS_FACTOR;
    cap->support_hdmi_20 = (cap->max_tmds_clock >= EDID_MAX_HDMI14_TMDS_RATE) ? TD_TRUE : TD_FALSE;
    edid_info("    %-20s : %u m_hz\n", "max_tmds_character_rate", cap->max_tmds_clock);
    /* byte-5 */
    cap->support3d_osd_disparity       = edid_flags_mask_check(data[5], BIT0_MASK);
    cap->support3d_dual_view           = edid_flags_mask_check(data[HDMI_EDID_DATA_INDEX_5], BIT1_MASK);
    cap->support3d_independent_view    = edid_flags_mask_check(data[HDMI_EDID_DATA_INDEX_5], BIT2_MASK);
    cap->support_lte340_mcsc_scrameble = edid_flags_mask_check(data[HDMI_EDID_DATA_INDEX_5], BIT3_MASK);
    cap->ccbpci                        = edid_flags_mask_check(data[HDMI_EDID_DATA_INDEX_5], BIT4_MASK);
    cap->support_rr_capable            = edid_flags_mask_check(data[HDMI_EDID_DATA_INDEX_5], BIT6_MASK);
    cap->support_scdc                  = edid_flags_mask_check(data[HDMI_EDID_DATA_INDEX_5], BIT7_MASK);

    edid_info("    %-20s : %s\n", "scdc_present", cap->support_scdc ? "yes" : "no");
    edid_info("    %-20s : %s\n", "rr_capable", cap->support_rr_capable ? "yes" : "no");
    edid_info("    %-20s : %s\n", "lte_340_mcsc_scramble", cap->support_lte340_mcsc_scrameble ? "yes" : "no");
    edid_info("    %-20s : %s\n", "independent_view", cap->support3d_independent_view ? "yes" : "no");
    edid_info("    %-20s : %s\n", "dual_view", cap->support3d_dual_view ? "yes" : "no");
    edid_info("    %-20s : %s\n", "3_d_osd_disparity", cap->support3d_osd_disparity ? "yes" : "no");
    /* byte-6 */
    cap->deep_color_y420.deep_color30_bit = edid_flags_mask_check(data[6], BIT0_MASK);
    cap->deep_color_y420.deep_color36_bit = edid_flags_mask_check(data[HDMI_EDID_DATA_INDEX_6], BIT1_MASK);
    cap->deep_color_y420.deep_color48_bit = edid_flags_mask_check(data[HDMI_EDID_DATA_INDEX_6], BIT2_MASK);
    edid_info("    %-20s : %s\n", "dc_48bit_420", cap->deep_color_y420.deep_color48_bit ? "yes" : "no");
    edid_info("    %-20s : %s\n", "dc_36bit_420", cap->deep_color_y420.deep_color36_bit ? "yes" : "no");
    edid_info("    %-20s : %s\n", "dc_30bit_420", cap->deep_color_y420.deep_color30_bit ? "yes" : "no");
    cap->max_frl_rate = (data[HDMI_EDID_DATA_INDEX_6] & EDID_UPPER_NIBBLE_MASK) >> 4; /* 4, bit[4:1] */

    if (len > 7) { /* 7, hfvs db length extent 7~31 */
        edid_hfvs21_db(cap, &data[7]);
    }

    return TD_SUCCESS;
}

static td_u32 g_common_4k_vic[] = {
    0,
    HDMI_3840X2160P30_16_9,
    HDMI_3840X2160P25_16_9,
    HDMI_3840X2160P24_16_9,
    HDMI_4096X2160P24_256_135
};

static td_void edid_latency_parse(hdmi_sink_capability *cap, const td_u8 *data, td_u8 *offset)
{
    if (cap->latency_fields_present) {
        cap->video_latency = data[(*offset)++];
        cap->audio_latency = data[(*offset)++];
        edid_info("    %-20s : 0x%02X\n", "video latency", cap->video_latency);
        edid_info("    %-20s : 0x%02X\n", "audio latency", cap->audio_latency);
    }
    if (cap->i_latency_fields_present) {
        cap->interlaced_video_latency = data[(*offset)++];
        cap->interlaced_audio_latency = data[(*offset)++];
        edid_info("    %-20s : 0x%02X\n", "interlaced video latency", cap->interlaced_video_latency);
        edid_info("    %-20s : 0x%02X\n", "interlaced audio latency", cap->interlaced_audio_latency);
    }

    return;
}

static td_void edid_vic_parse(hdmi_sink_capability *cap, hdmi_edid_status *status,
    td_u8 hdmi_vic_len, td_u8 *offset, const td_u8 *data)
{
    td_u32 i;
    td_u8 byte;

    for (i = 0; i < hdmi_vic_len; i++) {
        byte = data[(*offset)++];
        if (cap->support_vic_num < HDMI_EDID_MAX_VIC_COUNT) {
            if (byte < hdmi_array_size(g_common_4k_vic)) {
                cap->support_format[cap->support_vic_num++] = g_common_4k_vic[byte];
            }
        } else {
            status->parse_warn |= 1 << EDID_PARSE_WARN_VIC_CNT_OVER;
            break;
        }
    }

    return;
}

static td_void edid_3d_parse(hdmi_sink_capability *cap, const td_u8 *data, td_u8 *offset)
{
    td_u8 byte;

    /* 3d structure_all_15...8 reserved */
    byte = data[(*offset)++] & 0xFF;
    cap->_3d_info.support_3d_type[HDMI_EDID_3D_SIDE_BY_SIDE_HALF] = (byte & BIT0_MASK) ? TD_TRUE : TD_FALSE;
    byte = data[(*offset)++] & 0xFF;
    cap->_3d_info.support_3d_type[HDMI_EDID_3D_FRAME_PACKETING]   = (byte & BIT0_MASK) ? TD_TRUE : TD_FALSE;
    cap->_3d_info.support_3d_type[HDMI_EDID_3D_FIELD_ALTERNATIVE] = (byte & BIT1_MASK) ? TD_TRUE : TD_FALSE;
    cap->_3d_info.support_3d_type[HDMI_EDID_3D_LINE_ALTERNATIVE]  = (byte & BIT2_MASK) ? TD_TRUE : TD_FALSE;
    cap->_3d_info.support_3d_type[HDMI_EDID_3D_SIDE_BY_SIDE_FULL] = (byte & BIT3_MASK) ? TD_TRUE : TD_FALSE;
    cap->_3d_info.support_3d_type[HDMI_EDID_3D_L_DEPTH]           = (byte & BIT4_MASK) ? TD_TRUE : TD_FALSE;
    cap->_3d_info.support_3d_type[HDMI_EDID_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH] =
        (byte & BIT5_MASK) ? TD_TRUE : TD_FALSE;
    cap->_3d_info.support_3d_type[HDMI_EDID_3D_TOP_AND_BOTTOM]    = (byte & BIT6_MASK) ? TD_TRUE : TD_FALSE;

    return;
}

static td_void edid_cec_phy_addr_prase(hdmi_sink_capability *cap, const td_u8 *data)
{
    cap->cec_addr.phy_addr_a = (data[0] & EDID_UPPER_NIBBLE_MASK) >> 4; /* 4, offset for phy addr a */
    cap->cec_addr.phy_addr_b = (data[0] & EDID_LOWER_NIBBLE_MASK);
    cap->cec_addr.phy_addr_c = (data[1] & EDID_UPPER_NIBBLE_MASK) >> 4; /* 4, offset for phy addr c */
    cap->cec_addr.phy_addr_d = (data[1] & EDID_LOWER_NIBBLE_MASK);

    cap->cec_addr.phy_addr_valid = ((cap->cec_addr.phy_addr_a != EDID_CEC_INVALID_ADDR) &&
                                    (cap->cec_addr.phy_addr_b != EDID_CEC_INVALID_ADDR) &&
                                    (cap->cec_addr.phy_addr_c != EDID_CEC_INVALID_ADDR) &&
                                    (cap->cec_addr.phy_addr_d != EDID_CEC_INVALID_ADDR));

    edid_info("    %-20s : %02d.%02d.%02d.%02d\n", "CEC Address",
        cap->cec_addr.phy_addr_a, cap->cec_addr.phy_addr_b, cap->cec_addr.phy_addr_c, cap->cec_addr.phy_addr_d);

    return;
}

static td_void edid_deep_color_prase(hdmi_sink_capability *cap, td_u8 data)
{
    edid_info("    %-20s : 0x%02X\n", "color depth flags", data);

    cap->support_dvi_dual            = (data & BIT0_MASK) ? TD_TRUE : TD_FALSE;
    cap->deep_color.deep_color_y444  = (data & BIT3_MASK) ? TD_TRUE : TD_FALSE;
    cap->deep_color.deep_color30_bit = (data & BIT4_MASK) ? TD_TRUE : TD_FALSE;
    cap->deep_color.deep_color36_bit = (data & BIT5_MASK) ? TD_TRUE : TD_FALSE;
    cap->deep_color.deep_color48_bit = (data & BIT6_MASK) ? TD_TRUE : TD_FALSE;
    cap->supports_ai                 = (data & BIT7_MASK) ? TD_TRUE : TD_FALSE;

    /* If Y444 is TRUE, the sink device should supports YUV444. */
    if (cap->deep_color.deep_color_y444 == TD_TRUE) {
        cap->color_space.ycbcr444 = TD_TRUE;
    }

    return;
}

static td_void edid_vsdb_prase_step9(hdmi_sink_capability *cap, const td_u8 *data, hdmi_edid_status *status, td_u8 len)
{
    td_u8 offset;
    td_u8 byte;
    td_u8 hdmi_vic_len;
    td_u8 hdmi_3d_len;
    td_bool _3d_parse;
    td_bool _3d_present = TD_FALSE;
    td_u8 _3d_multi_present = 0;

    offset = 8; /* offset 8 bit, data byte count + 1 */
    edid_latency_parse(cap, data, &offset);

    byte = data[offset++]; /* data[8] */
    if (cap->hdmi_video_present) {
        _3d_multi_present = (byte & (BIT6_MASK | BIT5_MASK)) >> 5; /* 5'b, BIT[3:2] */
        _3d_present = (byte & BIT7_MASK) ? TD_TRUE : TD_FALSE;
        cap->_3d_info.support_3d = (byte & BIT7_MASK) ? TD_TRUE : TD_FALSE;
        edid_info("    %-20s : %s\n", "3D-support", _3d_present ? "yes" : "no");
        edid_info("    %-20s : %02u\n", "3_d_multi_present", _3d_multi_present);
        edid_info("    %-20s : %02u\n", "image_size", (byte & (BIT4_MASK | BIT3_MASK)) >> 3); /* 3'b, BIT[1] */
    }

    if (len < 10) { /* 10 offset for 3D */
        return;
    }
    /* 10, byte-13, VIC & 3D len */
    byte = data[offset];
    hdmi_3d_len = (byte & EDID_HDMI_3D_LEN_MASK);
    hdmi_vic_len = (byte & EDID_HDMI_VIC_LEN_MASK) >> 5; /* 5'b, BIT[3:1] */
    edid_info("    %-20s : %02u\n", "HDMI_VIC_LEN", hdmi_vic_len);
    edid_info("    %-20s : %02u\n", "HDMI_3D_LEN", hdmi_3d_len);
    /* byte-14 & following,VIC */
    offset++;
    if (hdmi_vic_len + offset <= len) {
        edid_vic_parse(cap, status, hdmi_vic_len, &offset, data);
    }

    /* byte-following, 3D */
    _3d_parse = ((_3d_present == TD_TRUE) && (hdmi_3d_len > 0) && (len >= (hdmi_3d_len + offset)) &&
        ((_3d_multi_present == EDID_3DMULTI_PRESENT_LOWER8) || (_3d_multi_present == EDID_3DMULTI_PRESENT_UPPER8)));
    if (_3d_parse == TD_TRUE) {
        edid_3d_parse(cap, data, &offset);
    }

    return;
}

static td_s32 edid_vsdb(hdmi_sink_capability *cap, hdmi_edid_status *status, const td_u8 *data, td_u8 len)
{
    /* byte-0~2, IEEE have been parsed outside the func */
    edid_info("    HDMI-LLC vendor specific data block\n");

    if (len < 5) { /* 5, byte-3~4, CEC addr */
        edid_warn("VSDB len:%u\n", len);
        status->parse_warn |= 1 << EDID_PARSE_WARN_BLOCKLEN_INVALID;
        return TD_SUCCESS;
    } else {
        edid_cec_phy_addr_prase(cap, &data[HDMI_EDID_DATA_INDEX_3]);
    }

    if (len >= 6) { /* 6, byte-5, color depth flags */
        edid_deep_color_prase(cap, data[5]);
    }

    if (len >= 7) { /* 7, byte-6, max tmds clock */
        cap->max_tmds_clock  = data[6];
        cap->max_tmds_clock *= EDID_TMDS_FACTOR;
        cap->support_hdmi_20 = (cap->max_tmds_clock >= EDID_MAX_HDMI14_TMDS_RATE) ? TD_TRUE : TD_FALSE;
        edid_info("    %-20s : %u m_hz\n", "max TMDS clock", cap->max_tmds_clock);
    }

    if (len >= 8) { /* 8, byte-7, sink present */
        cap->hdmi_video_present       = (data[7] & BIT5_MASK) ? TD_TRUE : TD_FALSE;
        cap->i_latency_fields_present = (data[HDMI_EDID_DATA_INDEX_7] & BIT6_MASK) ? TD_TRUE : TD_FALSE;
        cap->latency_fields_present   = (data[HDMI_EDID_DATA_INDEX_7] & BIT7_MASK) ? TD_TRUE : TD_FALSE;
    }

    if (len >= 9) { /* 9 offset for 3D */
        edid_vsdb_prase_step9(cap, data, status, len);
    }

    return TD_SUCCESS;
}

static td_s32 edid_speaker_db(hdmi_sink_capability *cap, hdmi_edid_status *status, const td_u8 *data, td_u8 len)
{
    td_u8 byte;

    if (len < 2) { /* 2, speaker_db min length */
        status->parse_warn |= 1 << EDID_PARSE_WARN_BLOCKLEN_INVALID;
        status->parse_warn |= 1 << EDID_PARSE_WARN_SPKDB_INVALID;
        edid_warn("SPKDB len:%u\n", len);
        return TD_SUCCESS;
    }
    edid_info("    speaker alloc data block (length=%02u)\n", len);
    /* byte-0 */
    byte = data[0];
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_FL_FR]   = (byte & BIT0_MASK) ? TD_TRUE : TD_FALSE;
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_LFE]     = (byte & BIT1_MASK) ? TD_TRUE : TD_FALSE;
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_FC]      = (byte & BIT2_MASK) ? TD_TRUE : TD_FALSE;
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_RL_RR]   = (byte & BIT3_MASK) ? TD_TRUE : TD_FALSE;
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_RC]      = (byte & BIT4_MASK) ? TD_TRUE : TD_FALSE;
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_FLC_FRC] = (byte & BIT5_MASK) ? TD_TRUE : TD_FALSE;
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_RLC_RRC] = (byte & BIT6_MASK) ? TD_TRUE : TD_FALSE;
    /* NOTE: the following is CEA-861-F add for HDMI2.0 */
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_FLW_FRW] = (byte & BIT7_MASK) ? TD_TRUE : TD_FALSE;
    /* byte-1 */
    byte = data[1];
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_FLH_FRH] = (byte & BIT0_MASK) ? TD_TRUE : TD_FALSE;
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_TC]      = (byte & BIT1_MASK) ? TD_TRUE : TD_FALSE;
    cap->support_audio_speaker[HDMI_EDID_AUDIO_SPEAKER_FCH]     = (byte & BIT2_MASK) ? TD_TRUE : TD_FALSE;

    return TD_SUCCESS;
}

static td_s32 edid_colorimetry_db(hdmi_sink_capability *cap, hdmi_edid_status *status, const td_u8 *data, td_u8 len)
{
    td_u8 byte;

    if (len < 3) { /* 3, is colorimetry db 1 ext tag code + 2 bytes payload */
        status->parse_warn |= 1 << EDID_PARSE_WARN_BLOCKLEN_INVALID;
        edid_err("colormetry_db len!\n");
        return TD_SUCCESS;
    }

    /* byte-0 */
    byte = data[0];
    cap->color_metry.xvycc601      = (byte & BIT0_MASK) ? TD_TRUE : TD_FALSE;
    cap->color_metry.xvycc709      = (byte & BIT1_MASK) ? TD_TRUE : TD_FALSE;
    cap->color_metry.sycc601       = (byte & BIT2_MASK) ? TD_TRUE : TD_FALSE;
    cap->color_metry.adoble_ycc601 = (byte & BIT3_MASK) ? TD_TRUE : TD_FALSE;
    cap->color_metry.adoble_rgb    = (byte & BIT4_MASK) ? TD_TRUE : TD_FALSE;
    cap->color_metry.bt2020c_ycc   = (byte & BIT5_MASK) ? TD_TRUE : TD_FALSE;
    cap->color_metry.bt2020_ycc    = (byte & BIT6_MASK) ? TD_TRUE : TD_FALSE;
    cap->color_metry.bt2020_rgb    = (byte & BIT7_MASK) ? TD_TRUE : TD_FALSE;

    /* byte-1 */
    cap->md_bit = data[1];

    return TD_SUCCESS;
}

static td_void edid_y420_video_db(hdmi_sink_capability *cap, hdmi_edid_status *status, const td_u8 *data, td_u8 len)
{
    td_u8 i;
    td_u32 vic;

    for (i = 0; i < len - 1; i++) {
        vic = data[i];
        if (cap->only_support_y420_vic_num >= HDMI_EDID_MAX_VIC_COUNT) {
            status->parse_warn |= 1 << EDID_PARSE_WARN_Y420VDB_VIC_OVER;
            edid_warn("y420_video_db vic count over:%u !\n", HDMI_EDID_MAX_VIC_COUNT);
            break;
        }

        if ((vic == EDID_VIC_INVALID_ZERO) || ((vic >= EDID_VIC_INVALID_LOW) && (vic <= EDID_VIC_INVALID_HIGH))) {
            status->parse_warn |= 1 << EDID_PARSE_WARN_VIC_INVALID;
            edid_warn("y420_video_db the %u is invalid vic: %u ! \n", i, vic);
            continue;
        }

        cap->only_support_y420_format[cap->only_support_y420_vic_num++] = vic;
        cap->color_space.ycbcr420 = TD_TRUE;
    }

    return;
}

static td_void edid_y420_cap_map_db(hdmi_sink_capability *cap, const td_u8 *data, td_u8 len)
{
    td_u32 i;
    td_u32 loop;

    /*
     * cap->support_y420_format is sv_ds parsed from video data block(s) in order.
     * when the length field is 1, indicates that all the sv_ds support YUV420.
     */
    if (len == 1) {
        for (i = 0; (i < cap->support_vic_num) && (i < HDMI_EDID_MAX_VIC_COUNT); i++) {
            if ((cap->support_format[i] <= HDMI_EDID_MAX_REAL_VIC) &&
                (cap->support_y420_vic_num < HDMI_EDID_MAX_VIC_COUNT)) {
                cap->support_y420_format[cap->support_y420_vic_num++] = cap->support_format[i];
                cap->color_space.ycbcr420 = TD_TRUE;
            }
        }
    } else { /* or, for sv_ds which support YUV420, the corresponding bit shall be set to 1 here. */
        loop = len * 8; /* 8bit eq 1 byte  */
        for (i = 0; (i < loop) && (i < cap->support_vic_num) && (i < HDMI_EDID_MAX_VIC_COUNT); i++) {
            /* BYTE n(i/8), BIT x(i%8) */
            if ((data[i / 8] & (0x01 << (i % 8))) &&
                (cap->support_y420_vic_num < HDMI_EDID_MAX_VIC_COUNT)) {
                cap->support_y420_format[cap->support_y420_vic_num++] = cap->support_format[i];
                cap->color_space.ycbcr420 = TD_TRUE;
            }
        }
    }
    return;
}

static td_void edid_video_cap_db(hdmi_sink_capability *cap, const td_u8 *data, td_u8 len)
{
    if (len == 0) {
        return;
    }

    cap->ycc_qrange_selectable = (data[0] & BIT7_MASK) ? TD_TRUE : TD_FALSE;
    cap->rgb_qrange_selectable = (data[0] & BIT6_MASK) ? TD_TRUE : TD_FALSE;

    return;
}

static td_s32 edid_ext_data_block_parse(hdmi_sink_capability *cap, hdmi_edid_status *status,
                                        const td_u8 *data, td_u8 len)
{
    td_s32 ret;
    td_u8 ext_tag_code;

    ext_tag_code = data[0];
    data++;

    switch (ext_tag_code) {
        case EDID_COLORIMETRY_DB:
            ret = edid_colorimetry_db(cap, status, data, len);
            hdmi_if_failure_return(ret, TD_FAILURE);
            break;
        case EDID_Y420_VIDEO_DB:
            edid_y420_video_db(cap, status, data, len);
            break;
        case EDID_Y420_CAPABILITY_MAP_DB:
            edid_y420_cap_map_db(cap, data, len);
            break;
        case EDID_VIDEO_CAPABILITY_DB:
            edid_video_cap_db(cap, data, len);
            break;
        default:
            edid_warn("EXT-BLK un-parse ext data block!ext-tag code:%u!\n", ext_tag_code);
            break;
    }

    return TD_SUCCESS;
}

static td_s32 edid_vendor_data_block_parse(hdmi_sink_capability *cap,
    hdmi_edid_status *status, const td_u8 *data, td_u8 blk_len)
{
    td_s32 ret = TD_SUCCESS;
    td_bool is_vsdb;
    td_bool is_hfvsdb;

    is_vsdb = ((blk_len >= EDID_VENDOR_BLOCK_LEN) && (data[0] == EDID_IEEE_VSDB_1ST) &&
               (data[1] == EDID_IEEE_VSDB_2ND) && (data[HDMI_EDID_DATA_INDEX_2] == EDID_IEEE_VSDB_3RD));

    is_hfvsdb = ((blk_len >= EDID_VENDOR_BLOCK_LEN) && (data[0] == EDID_IEEE_HFVSDB_1ST) &&
                 (data[1] == EDID_IEEE_HFVSDB_2ND) && (data[HDMI_EDID_DATA_INDEX_2] == EDID_IEEE_HFVSDB_3RD));

    if (is_vsdb == TD_TRUE) {
        /* IEEE code indicate whether it is HDMI1.4b or HDMI2.0 */
        cap->support_hdmi = TD_TRUE;
        ret = edid_vsdb(cap, status, data, blk_len);
        hdmi_if_failure_return(ret, TD_FAILURE);
    } else if (is_hfvsdb == TD_TRUE) {
        ret = edid_hfvs_db(cap, status, data, blk_len);
        hdmi_if_failure_return(ret, TD_FAILURE);
    } else {
        if (blk_len >= EDID_VENDOR_BLOCK_LEN) {
            status->parse_warn |= 1 << EDID_PARSE_WARN_VSDB_INVALID;
            edid_warn("(HF)VSDB IEEE:0x%02x, 0x%02x, 0x%02x\n", data[0], data[1], data[HDMI_EDID_DATA_INDEX_2]);
        } else {
            status->parse_warn |= 1 << EDID_PARSE_WARN_BLOCKLEN_INVALID;
            edid_warn("(HF)VSDB  blk_len:%u\n", blk_len);
        }
    }

    return ret;
}

static td_s32 edid_db_parse(hdmi_sink_capability *cap, td_u8 db_tag,
                            hdmi_edid_status *status, const td_u8 *data, td_u8 len)
{
    td_s32 ret = TD_SUCCESS;

    switch (db_tag) {
        case EDID_AUDIO_DATA_BLOCK:
            ret = edid_audio_db(cap, status, data, len);
            break;
        case EDID_VIDEO_DATA_BLOCK:
            ret = edid_video_db(cap, status, data, len);
            break;
        case EDID_VENDOR_DATA_BLOCK:
            ret = edid_vendor_data_block_parse(cap, status, data, len);
            break;
        case EDID_SPEAKER_DATA_BLOCK:
            ret = edid_speaker_db(cap, status, data, len);
            break;
        case EDID_USE_EXT_DATA_BLOCK:
            /* ext tag code */
            ret = edid_ext_data_block_parse(cap, status, data, len);
            break;
        default:
            edid_warn("EXT_BLK resvered/un-parse block tag code!\n");
            break;
    }

    return ret;
}

static td_void edid_extention_info_print(const td_u8 *data)
{
    edid_info("    %-20s : %02X\n", "CEA EDID timing extension tag", data[0]);
    edid_info("    %-20s : %02X\n", "CEA 861B version", data[1]);
    edid_info("    %-20s : %02u\n", "number of native DTD's", data[HDMI_EDID_DATA_INDEX_3] & 0x0F);
    edid_info("    %-20s : %s\n", "basic audio support", (data[HDMI_EDID_DATA_INDEX_3] & 0x40) ? "yes" : "no");
    edid_info("    %-20s : RGB444 %s %s\n", "pixel format",
        (data[HDMI_EDID_DATA_INDEX_3] & 0x20) ? "ycbcr444" : "",
        (data[HDMI_EDID_DATA_INDEX_3] & 0x10) ? "ycbcr422" : "");

    ot_unused(data);
    return;
}

static td_void edid_ext_several_block_parse(hdmi_sink_capability *cap, hdmi_edid_status *status, const td_u8 *data)
{
    td_s32 ret;
    const td_u8 *tmp = TD_NULL;
    td_u8 blk_offset = EDID_EXT_BLOCK_OFFSET;
    td_u8 db_tag_code;
    td_u8 blk_len;
    td_u8 dtd_offset = data[2];

    tmp = data + blk_offset;

    /* several data block */
    for (blk_len = 0;
         (blk_offset < dtd_offset) && (tmp != TD_NULL);
         blk_offset += blk_len + 1) {
        tmp += blk_len;
        blk_len = (*tmp) & EDID_DB_LEN_MASK;
        db_tag_code = ((*tmp) & EDID_DB_TAG_CODE_MASK) >> 5; /* 5'b, bit[3:1] */
        tmp++;
        /* tag code */
        ret = edid_db_parse(cap, db_tag_code, status, tmp, blk_len);
        hdmi_if_failure_return_void(ret);
    }
    /* dt_ds */
    tmp += blk_len;
    while (((HDMI_EDID_BLOCK_SIZE - 1) - blk_offset) >= EDID_DTD_SIZE) {
        ret = edid_detail_timing(cap, status, tmp, EDID_DTD_SIZE);
        hdmi_if_failure_return_void(ret);
        blk_offset += EDID_DTD_SIZE;
        tmp += EDID_DTD_SIZE;
    }

    return;
}

static td_s32 edid_extention_block_parse(hdmi_edid_info *edid_info, td_u8 blk_num)
{
    td_s32 ret;
    td_u8 dtd_offset;
    td_u8 *data = TD_NULL;
    hdmi_edid_status *status = TD_NULL;
    hdmi_sink_capability *cap = TD_NULL;
    hdmi_edid_color_space *clr_space = TD_NULL;

    if (blk_num > (HDMI_EDID_TOTAL_BLOCKS - 1)) {
        edid_err("EXT-BLK = %u\n", blk_num);
        return TD_FAILURE;
    }
    status = &edid_info->status;
    data = edid_info->edid_raw + (blk_num * HDMI_EDID_BLOCK_SIZE);
    cap = &edid_info->capability;

    ret = edid_block_check_sum(data, status);
    hdmi_if_failure_return(ret, TD_FAILURE);
    edid_info("    basic audio support_check for 861B (block %i)\n", blk_num);

    /* byte-0 :TAG */
    if (data[0] != EDID_CEA_EXTVERSION3_TAG) {
        status->parse_err = EDID_PARSE_ERR_TAG_UNKNOWN;
        edid_err("EXT_BLK CEA TAG:0x%02x\n", data[0]);
    }

    /* byte-1: REVISION */
    if (data[1] != EDID_CEA_EXTVERSION3_REVISION) {
        status->parse_err = EDID_PARSE_ERR_CEA_REVERION;
        edid_err("EXT_BLK CEA861 REVISION: 0x%02x\n", data[1]);
    }
    /* byte-2: dt_ds offset */
    dtd_offset = data[2];
    if (dtd_offset < EDID_EXT_BLOCK_OFFSET) {
        edid_info("EXT_BLK NO dt_ds & reserved data blks :d=%u\n", dtd_offset);
        return TD_SUCCESS;
    }

    edid_extention_info_print(data);

    /* byte-3: basic audio, color space, dt_ds cnt */
    clr_space = &cap->color_space;
    clr_space->rgb444   = TD_TRUE;
    clr_space->ycbcr422 = ((data[HDMI_EDID_DATA_INDEX_3] & BIT4_MASK) ? TD_TRUE : TD_FALSE);
    clr_space->ycbcr444 = ((data[HDMI_EDID_DATA_INDEX_3] & BIT5_MASK) ? TD_TRUE : TD_FALSE);

    edid_ext_several_block_parse(cap, status, data);

    return TD_SUCCESS;
}

static td_s32 edid_raw_parse(hdmi_edid_info *edid_info)
{
    td_s32 ret;
    td_u8 blk_num;
    hdmi_sink_capability *cap = TD_NULL;

    cap = &edid_info->capability;
    ret = edid_first_blk_parse(edid_info);
    hdmi_if_failure_return(ret, TD_FAILURE);
    for (blk_num = 1; blk_num <= cap->ext_block_num; blk_num++) {
        ret = edid_extention_block_parse(edid_info, blk_num);
        hdmi_if_failure_return(ret, TD_FAILURE);
    }

    return TD_SUCCESS;
}

td_s32 drv_hdmi_edid_reset(hdmi_edid_info *edid_info)
{
    td_u8 *data = TD_NULL;
    hdmi_edid_status *status = TD_NULL;
    hdmi_sink_capability *cap = TD_NULL;

    hdmi_if_null_return(edid_info, OT_ERR_HDMI_NULL_PTR);
    status = &edid_info->status;
    cap = &edid_info->capability;
    data = edid_info->edid_raw;

    (td_void)memset_s(cap, sizeof(edid_info->capability), 0, sizeof(hdmi_sink_capability));
    (td_void)memset_s(data, sizeof(edid_info->edid_raw), 0, sizeof(edid_info->edid_raw));

    status->cap_sink = TD_FALSE;
    status->cap_valid = TD_FALSE;
    status->raw_valid = TD_FALSE;
    status->raw_len = 0;
    status->parse_err = EDID_PARSE_ERR_NONE;
    status->parse_warn = EDID_PARSE_WARN_NONE;

    return TD_SUCCESS;
}

td_s32 drv_hdmi_edid_update(hdmi_edid_info *edid_info, hdmi_edid_updata_mode mode)
{
    td_s32 ret = TD_SUCCESS;
    td_u8 *data = TD_NULL;
    hdmi_edid_status *status = TD_NULL;
    hdmi_sink_capability *cap = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;
    td_bool hotplug = TD_FALSE;

    hdmi_if_null_return(edid_info, OT_ERR_HDMI_NULL_PTR);
    status = &edid_info->status;
    cap = &edid_info->capability;
    data = edid_info->edid_raw;

    if (mode == HDMI_EDID_UPDATE_SINK) {
        status->raw_len = 0;
        status->cap_sink = TD_FALSE;
        status->cap_valid = TD_FALSE;
        status->raw_valid = TD_FALSE;
        status->parse_err = EDID_PARSE_ERR_NONE;
        status->parse_warn = EDID_PARSE_WARN_NONE;

        (td_void)memset_s(cap, sizeof(edid_info->capability), 0, sizeof(hdmi_sink_capability));
        (td_void)memset_s(data, HDMI_EDID_TOTAL_SIZE, 0, sizeof(edid_info->edid_raw));

        hdmi_dev = osal_container_of(edid_info, __typeof__(*hdmi_dev), edid_info);
        hal_call_void(hdmi_dev, hal_hdmi_hot_plug_status_get, &hotplug);
        if (hotplug) {
            hal_call_ret(hdmi_dev, ret, hal_hdmi_edid_raw_data_read, HDMI_EDID_TOTAL_SIZE, data);
            status->cap_sink = TD_TRUE;
            if ((ret == 0) || (ret % HDMI_EDID_BLOCK_SIZE)) {
                status->raw_update_err_cnt++;
                edid_err("\nread EDID raw data fail(0x%x)!\n", ret);
                return OT_ERR_HDMI_READ_EDID_FAILED;
            }
            status->raw_len = (td_u32)ret;
            status->raw_valid = TD_TRUE;
        } else {
            status->cap_sink = TD_FALSE;
            edid_warn("sink do not connect, forbid to read edid\n");
            return OT_ERR_HDMI_DEV_NOT_CONNECT;
        }
    }

    ret = edid_raw_parse(edid_info);
    if (ret != TD_SUCCESS) {
        edid_err("\nedid raw parse fail(0x%x)!\n", ret);
        return OT_ERR_HDMI_READ_EDID_FAILED;
    }
    status->cap_valid = TD_TRUE;

    return TD_SUCCESS;
}

hdmi_edid_data drv_hdmi_edid_capability_get(hdmi_edid_info *edid_info, hdmi_sink_capability **cap_dest)
{
    hdmi_edid_status *status = TD_NULL;
    hdmi_edid_data edid_status;

    hdmi_if_null_return(cap_dest, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(edid_info, OT_ERR_HDMI_NULL_PTR);

    status = &edid_info->status;
    *cap_dest = &edid_info->capability;

    if (status->cap_valid) {
        edid_status = (status->cap_sink) ? HDMI_EDID_DATA_VALIDSINK : HDMI_EDID_DATA_VALIDTEST;
    } else {
        status->cap_get_err_cnt++;
        edid_status = HDMI_EDID_DATA_INVALID;
    }

    return edid_status;
}

td_s32 drv_hdmi_edid_raw_get(hdmi_edid_info *edid_info, td_u8 *raw_data_dest, td_u32 len)
{
    errno_t ret;
    td_u32 tmp_len;
    td_u8 *data_src = TD_NULL;
    hdmi_edid_status *status = TD_NULL;

    hdmi_if_null_return(raw_data_dest, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(edid_info, OT_ERR_HDMI_NULL_PTR);

    status = &edid_info->status;
    data_src = edid_info->edid_raw;

    if ((len > HDMI_EDID_TOTAL_SIZE) || (status->raw_valid == TD_FALSE)) {
        status->raw_get_err_cnt++;
        return TD_FAILURE;
    }

    tmp_len = (len <= status->raw_len) ? len : status->raw_len;
    ret = memcpy_s(raw_data_dest, len, data_src, tmp_len);
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    return (td_s32)tmp_len;
}

td_s32 drv_hdmi_edid_status_get(const hdmi_edid_info *edid_info, hdmi_edid_status *status)
{
    errno_t ret;

    hdmi_if_null_return(edid_info, OT_ERR_HDMI_NULL_PTR);
    hdmi_if_null_return(status, OT_ERR_HDMI_NULL_PTR);

    ret = memcpy_s(status, sizeof(*status), &edid_info->status, sizeof(hdmi_edid_status));
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    return TD_SUCCESS;
}

