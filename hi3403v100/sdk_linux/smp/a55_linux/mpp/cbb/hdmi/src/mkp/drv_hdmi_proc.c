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
#include "drv_hdmi_proc.h"
#include "hdmi_osal.h"
#include "drv_hdmi_intf.h"
#include "drv_hdmi_common.h"
#include "drv_hdmi_event.h"
#include "drv_hdmi_edid.h"
#include "drv_hdmi_compatibility.h"
#include "sys_ext.h"

/* Constants for reported magic-number values. */
#define HDMI_PROC_HW_PARAM_INDEX_2             2

#define PROC_FEILD_RATE_DEVIATION 50
#define PROC_PIXL_FREQ_DEVIATION 100
#define HDMI_TITLE_LEN 120
#define HDMI_VIC_PC_BASE ((HDMI_VIC_VIRTUAL_BASE) + 0x100)
#define STR_YES "YES"
#define STR_NO "NO"
#define STR_NA "N/A"
#define STR_ERROR "ERROR"
#define STR_RUN "RUN"
#define STR_STOP "STOP"
#define STR_ON "ON"
#define STR_OFF "OFF"
#define STR_LINE_COUNT 8
#define STR_LEN_SUPERADDITION 4
#define PROC_LENGTH_SIZE 21
#define PORC_ONE_LINE_STR_COUNT 4
#define PROC_TEMP_STR_LEN 25
#define HW_PARAM_100M_BAND 0
#define HW_PARAM_165M_BAND 1
#define HW_PARAM_300M_BAND 2
#define HW_PARAM_600M_BAND 3
#undef min
#define min(a, b) (((a) > (b)) ?  (b) : (a))

typedef union {
    struct {
        td_u8 scan_info            : 2;
        td_u8 bar_data_present     : 2;
        td_u8 act_fmt_info_present : 1;
        td_u8 color_space          : 3;
    } bits;
    td_u8 u8;
} hdmi_avi_info_byte1;

typedef union {
    struct {
        td_u8 act_asp_ratio        : 4;
        td_u8 picture_aspect_ratio : 2;
        td_u8 colorimetry          : 2;
    } bits;
    td_u8 u8;
} hdmi_avi_info_byte2;

typedef union {
    struct {
        td_u8 picture_scaling  : 2;
        td_u8 rgb_quantization : 2;
        td_u8 ext_colorimetry  : 3;
        td_u8 it_content_valid : 1;
    } bits;
    td_u8 u8;
} hdmi_avi_info_byte3;

typedef union {
    struct {
        td_u8 pixel_repetition       : 4;
        td_u8 it_content_type        : 2;
        td_u8 ycc_quantization_range : 2;
    } bits;
    td_u8 u8;
} hdmi_avi_info_byte5;

typedef union {
    struct {
        td_u8 audio_chanel_cnt  : 3;
        td_u8 f13               : 1;
        td_u8 aduio_coding_type : 4;
    } bits;
    td_u8 u8;
} hdmi_audio_info_byte1;

typedef union {
    struct {
        td_u8 audio_samping_size : 2;
        td_u8 audio_samping_freq : 3;
        td_u8 f25to_f27          : 3;
    } bits;
    td_u8 u8;
} hdmi_audio_info_byte2;

typedef union {
    struct {
        td_u8 lfe_play_back     : 2;
        td_u8 f52               : 1;
        td_u8 level_shift_value : 4;
        td_u8 down_mix_inhibit  : 1;
    } bits;
    td_u8 u8;
} hdmi_audio_info_byte5;

typedef struct {
    td_u32   vic;
    td_u32   pixl_freq; /* Khz */
    td_u32   hor_active;
    td_u32   ver_active;
    td_u32   field_rate; /* 0.01Hz */
    td_char *format;
} hdmi_fmt_name_param;

static const td_char *g_dither_mode_str[]  = { "12_10", "12_8", "10_8", "disable" };
static const td_char *g_lfe_playback_str[] = { "UNKNOWN", "0 dB", "+10 dB", "reserved" };
static const td_char *g_scan_info_str[]    = { "NONE", "OVER_SCAN", "UNDERS_SCAN", "reserved" };
static const td_char *g_colorimetry_str[]  = { "No Data", "SMPTE 170M", "ITU-R BT.709", "Extended" };
static const td_char *g_run_state_str[]    = { "INIT", "OPEN", "START", "STOP", "CLOSE", "DEINIT" };
static const td_char *g_tmds_mode_str[]    = { "NONE", "DVI", "HDMI1.4", "HDMI2.0", "AUTO", "HDMI2.1", "UNKNOWN" };
static const td_char *g_color_space_str[]  = { "RGB", "YCBCR422", "YCBCR444", "YCBCR420", "UNKNOWN" };
static const td_char *g_sound_intf_str[]   = { "I2S", "SPDIF", "HBRA", "UNKNOWN" };
static const td_char *g_deep_color[]       = { "24", "30", "36", "48", "OFF", "UNKNOWN" };
static const td_char *g_bit_depth_str[]    = { "08 bit", "10 bit", "12 bit", "16 bit", "08 bit", "UNKNOWN" };
static const td_char *g_deep_color_str[]   = { "24 bit", "30 bit", "36 bit", "48 bit", "24 bit(OFF)", "UNKNOWN" };
static const td_char *g_rgb_quantization_str[]   = { "DEFAULT", "LIMITED", "FULL", "UNKNOWN" };
static const td_char *g_pic_aspect_ratio_str[]   = { "NONE", "4:3", "16:9", "64:27", "256:135", "FUTURE", "UNKNOWN" };
static const td_char *g_it_content_type_str[]    = { "GRAPHICS", "PHOTO", "CINEMA", "GAME" };
static const td_char *g_ycc_quantization_str[]   = { "LIMITED", "FULL", "UNKNOWN", "UNKNOWN" };
static const td_char *g_transition_sate_str[]    = { "BOOT->MCE", "MCE->APP", "BOOT->APP" };
static const td_char *g_audio_samping_size_str[] = { "STR_HEADER", "16 bit", "20 bit", "24 bit" };
static const td_char *g_trace_str[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "DEFAULT"};

static const td_char *g_act_aspect_ratio_str[] = {
    "Reserved",
    "Reserved",
    "16:9_TOP",
    "14:9_TOP",
    "16:9_CENTER",
    "reserved",
    "reserved",
    "reserved",
    "PICTURE",
    "4:3",
    "16:9",
    "14:9",
    "reserved",
    "4:3_SP_14_9",
    "16:9_SP_14_9",
    "16:9_SP_4_3",
    "UNKNOWN"
};

static const td_char *g_bar_data_present_str[] = {
    "NONE", // bar data not present
    "HnVp", // vert. barinfo present
    "HpVn", // horiz. barinfo present
    "HpVp"  // vert and horiz. barinfo present
};

static const td_char *g_pixel_repetition_str[] = {
    "no repetition",
    "2 times",
    "3 times",
    "4 times",
    "5 times",
    "6 times",
    "7 times",
    "8 times",
    "9 times",
    "10 times",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved"
};

static const td_char *g_audio_code_type_str[] = {
    "STREAM",
    "L-PCM",
    "AC3",
    "MPEG1",
    "MP3",
    "MPEG2",
    "AAC_LC",
    "DTS",
    "ATRAC",
    "OneBitAudio",
    "EAC3",
    "DTS-HD",
    "MAT",
    "DST",
    "WMA_PRO",
    "reserved",
    "UNKNOWN"
};

static const td_char *g_audio_chn_str[] = {
    "STR_HEADER",
    "2_CH",
    "3_CH",
    "4_CH",
    "5_CH",
    "6_CH",
    "7_CH",
    "8_CH"
};

static const td_char *g_audio_samping_freq_str[] = {
    "STR_HEADER",
    "32 KHz",
    "44.1 kHz",
    "48 KHz",
    "88.2 KHz",
    "96 KHz",
    "176.4 KHz",
    "192 KHz"
};

static const td_char *g_picture_scaling_str[] = {
    "UNKNOWN",
    "HpVn",  // picture has been scaled horizontally
    "HnVp ", // picture has been scaled vertically
    "HpVp"   // picture has been scaled horizontally and vertically
};

static const td_char *g_ext_colorimetry_str[] = {
    "XV_YCC601",
    "XV_YCC709",
    "S_YCC601",
    "ADOBE_YCC601",
    "ADOBE_RGB",
    "BT2020_cYCC",
    "BT2020_RGB/YCC",
    "ADDITIONAL",
    "UNKNOWN"
};

static const td_char *g_str_speaker_str[] = {
    "FL_FR",
    "LFE",
    "FC",
    "RL_RR",
    "RC",
    "FLC_FRC",
    "RLC_RRC",
    "FLW_FRW",
    "FLH_FRH",
    "TC",
    "FCH",
    "UNKNOWN"
};

static const td_char *g_stere_mode_str[] = {
    "FRAME_PACK",  /* 3d type:Frame Packing */
    "FIELD_ALTER", /* 3d type:Field alternative */
    "LINE_ALTERN", /* 3d type:Line alternative */
    "SBS_FULL",    /* 3d type:Side by side full */
    "L_DEPTH",     /* 3d type:L+depth */
    "L_DEPTH_GGD", /* 3d type:L+depth+Graphics+Graphics-depth */
    "TAndB",       /* 3d type:Top and Bottom */
    "Reserved",
    "SByS_HALF",  /* HDMI_EDID_3D_SIDE_BY_SIDE_HALF */
    "NONE"
};

static const td_char *g_estab_timing_str[] = {
    "800X600_60",
    "800X600_56",
    "V640X480_75",
    "V640X480_72",
    "V640X480_67",
    "V640X480_60",
    "V720X400_88",
    "V720X400_70",
    "V1280X1024_75",
    "V1024X768_75",
    "V1024X768_70",
    "V1024X768_60",
    "V1024X768_87",
    "832X624_75",
    "800X600_75",
    "800X600_72",
    "V1152X870_75",
    "UNKNOWN"
};

static const td_char *g_disp_fmt_str[] = {
    "1080P@60",
    "1080P@50",
    "1080P@30",
    "1080P@25",
    "1080P@24",
    "1080I@60",
    "1080I@50",
    "720P@60",
    "720P@50",
    "576P@50",
    "480P@60",
    "PAL",
    "NTSC",
    "640X480@60",
    "800X600@60",
    "1024X768@60",
    "1280X800@60",
    "1280X1024@60",
    "1366X768@60",
    "1440X900@60",
    "1400X1050@60",
    "1600X1200@60",
    "1680X1050@60",
    "1920X1200@60",
    "2560x1440@30",
    "2560x1440@60",
    "2560x1600@60",
    "1920x2160@30",
    "3840X2160P@24",
    "3840X2160P@25",
    "3840X2160P@30",
    "3840X2160P@50",
    "3840X2160P@60",
    "4096X2160P@24",
    "4096X2160P@25",
    "4096X2160P@30",
    "4096X2160P@50",
    "4096X2160P@60",
    "3840X2160P@120",
    "4096X2160P@120",
    "7680X4320P@30",
    "CUSTOMER_DEFINE",
    "FMT_UNKNOWN"
};

static const td_char *g_audio_channel_str[] = {
    "reserved",
    "reserved",
    "2_CH",
    "3_CH",
    "4_CH",
    "5_CH",
    "6_CH",
    "7_CH",
    "8_CH",
    "UNKNOWN"
};

static const hdmi_fmt_name_param g_fmt_param[] = {
    /* VIC pix_ffeq HACT VACT fld_rate    fmt */
    {   1,   25175,  640,  480, 6000, "640X480_60 4:3"     },
    {   2,   27000,  720,  480, 6000, "480P_60 4:3"        },
    {   3,   27000,  720,  480, 6000, "480P_60 16:9"       },
    {   4,   74170, 1280,  720, 6000, "720P_60 16:9"       },
    {   5,   74170, 1920, 1080, 6000, "1080i_60 16:9"      },
    {   6,   27000,  720,  480, 6000, "NTSC 4:3"           },
    {   7,   27000,  720,  480, 6000, "NTSC 16:9"          },
    {   8,   27000, 1440,  240, 6000, "240P_60 4:3"        },
    {   9,   27000, 1440,  240, 6000, "240P_60 16:9"       },
    {  10,   54000,  720,  480, 6000, "2880_x480i_60 4:3"  },
    {  11,   54000,  720,  480, 6000, "2880_x480i_60 16:9" },
    {  12,   54000, 2880,  240, 6000, "2880X240P_60 4:3"   },
    {  13,   54000, 2880,  240, 6000, "2880X240P_60 16:9"  },
    {  14,   54000, 1440,  480, 6000, "1440X480P_60 4:3"   },
    {  15,   54000, 1440,  480, 6000, "1440X480P_60 16:9"  },
    {  16,  148500, 1920, 1080, 6000, "1080P_60 16:9"      },
    {  17,   27000,  720,  576, 5000, "576P_50 4:3"        },
    {  18,   27000,  720,  576, 5000, "576P_50 16:9"       },
    {  19,   74250, 1280,  720, 5000, "720P_50 16:9"       },
    {  20,   74250, 1920, 1080, 5000, "1080i_50 16:9"      },
    {  21,   27000, 1440,  576, 5000, "PAL 4:3"            },
    {  22,   27000, 1440,  576, 5000, "PAL 16:9"           },
    {  31,  148500, 1920, 1080, 5000, "1080P_50 16:9"      },
    {  32,   74250, 1920, 1080, 2400, "1080P_24 16:9"      },
    {  33,   74250, 1920, 1080, 2500, "1080P_25 16:9"      },
    {  34,   74250, 1920, 1080, 3000, "1080P_30 16:9"      },
    {  60,   59400, 1280,  720, 2400, "720P_24 16:9"       },
    {  61,   74250, 1280,  720, 2500, "720P_25 16:9"       },
    {  62,   74250, 1280,  720, 3000, "720P_30 16:9"       },
    {  63,  297000, 1920, 1080, 12000, "1080P_120 16:9"    },
    {  64,  297000, 1920, 1080, 10000, "1080P_100 16:9"    },
    {  93,  297000, 3840, 2160, 2400, "3840X2160P24 16:9"  },
    {  94,  297000, 3840, 2160, 2500, "3840X2160P25 16:9"  },
    {  95,  297000, 3840, 2160, 3000, "3840X2160P30 16:9"  },
    {  96,  594000, 3840, 2160, 5000, "3840X2160P50 16:9"  },
    {  97,  594000, 3840, 2160, 6000, "3840X2160P60 16:9"  },
    {  98,  297000, 4096, 2160, 2400, "4096X2160P24 256:135" },
    {  99,  297000, 4096, 2160, 2500, "4096X2160P25 256:135" },
    { 100,  297000, 4096, 2160, 3000, "4096X2160P30 256:135" },
    { 101,  594000, 4096, 2160, 5000, "4096X2160P50 256:135" },
    { 102,  594000, 4096, 2160, 6000, "4096X2160P60 256:135" },
    { 103,  297000, 3840, 2160, 2400, "3840X2160P24 64:27"   },
    { 104,  297000, 3840, 2160, 2500, "3840X2160P25 64:27"   },
    { 105,  297000, 3840, 2160, 3000, "3840X2160P30 64:27"   },
    { 106,  594000, 3840, 2160, 5000, "3840X2160P50 64:27"   },
    { 107,  594000, 3840, 2160, 6000, "3840X2160P60 64:27"   },
    { 117, 1188000, 3840, 2160, 10000, "3840X2160P100 16:9"  },
    { 118, 1188000, 3840, 2160, 12000, "3840X2160P120 16:9"  },
    { 194, 1188000, 7680, 4320, 2400, "7680X4320P24 16:9"    },
    { 195, 1188000, 7680, 4320, 2500, "7680X4320P25 16:9"    },
#if defined(HDMI_PRODUCT_SS626V100)
    // The 8K30 timing in SS626V100 is non-standard timing, and the pixel clock is only 594 MHz.
    { 196,  594000, 7680, 4320, 3000, "7680X4320P30 16:9"    },
#else
    { 196, 1188000, 7680, 4320, 3000, "7680X4320P30 16:9"    },
#endif
    { 198, 2376000, 7680, 4320, 5000, "7680X4320P50 16:9"    },
    { 199, 2376000, 7680, 4320, 6000, "7680X4320P60 16:9"    },
#ifdef DVI_SUPPORT
    { HDMI_VIC_PC_BASE +  9,  40000,  800,  600, 6000, "800X600_60"   },
    { HDMI_VIC_PC_BASE + 13,  65000, 1024,  768, 6000, "1024X768_60"  },
    { HDMI_VIC_PC_BASE + 18, 162000, 1600, 1200, 6000, "1600X1200_60" },
    { HDMI_VIC_PC_BASE + 25, 108000, 1280, 1024, 6000, "1280X1024_60" },
    { HDMI_VIC_PC_BASE + 28,  85500, 1360,  768, 6000, "1366X768_60"  },
    { HDMI_VIC_PC_BASE + 41, 154000, 1920, 1200, 6000, "1920X1200_60" },
#endif
    /* for 2k & 4k ,we use VIRTUAL VIC */
    { HDMI_EDID_VIRTUAL_VIC_3840X2160_30, 297000, 3840, 2160, 3000, "3840X2160P_30" },
    { HDMI_EDID_VIRTUAL_VIC_3840X2160_25, 297000, 3840, 2160, 2500, "3840X2160P_25" },
    { HDMI_EDID_VIRTUAL_VIC_3840X2160_24, 297000, 3840, 2160, 2400, "3840X2160P_24" },
    { HDMI_EDID_VIRTUAL_VIC_4096X2160_24, 297000, 4096, 2160, 2400, "4096X2160P_24" }
};

static const td_char *g_version = "HDMI version:   " MAKE_VERSION;

static td_void hdmi_print_proc_title(hdmi_proc_entry *file, const td_u8 *title, td_u32 len)
{
    td_u32 i;

    i = (td_u32)strlen("------------");
    osal_seq_printf(file, "------------%s", title);
    for (; i + len < HDMI_TITLE_LEN; i++) {
        osal_seq_printf(file, "%s", "-");
    }
    osal_seq_printf(file, "\n");

    return;
}

static td_char *proc_vicformat(td_u32 vic)
{
    td_u32 i;
    hdmi_fmt_name_param *fmt_param = TD_NULL;

    for (i = 0; i < hdmi_array_size(g_fmt_param); i++) {
        fmt_param = (hdmi_fmt_name_param *)&g_fmt_param[i];
        if ((fmt_param != TD_NULL) && (fmt_param->vic == vic)) {
            return fmt_param->format;
        }
    }
    return TD_NULL;
}

static td_char *proc_std_timingformat(const hdmi_edid_std_timing *std_timing)
{
    td_u32 i;
    hdmi_fmt_name_param *fmt_param = TD_NULL;

    if (std_timing == TD_NULL) {
        return TD_NULL;
    }

    for (i = 0; i < hdmi_array_size(g_fmt_param); i++) {
        fmt_param = (hdmi_fmt_name_param *)&g_fmt_param[i];
        if ((fmt_param != TD_NULL) &&
            (fmt_param->hor_active == std_timing->hor_active) &&
            (fmt_param->ver_active == std_timing->ver_active) &&
            (fmt_param->field_rate >= std_timing->refresh_rate) &&
            ((fmt_param->field_rate - PROC_FEILD_RATE_DEVIATION) <= std_timing->refresh_rate)) {
            return fmt_param->format;
        }
    }

    return TD_NULL;
}

static td_void proc_sw_version(hdmi_proc_entry *file)
{
    osal_seq_printf(file, "%s\n", g_version);
    return;
}

static td_void app_attr_proc_show(hdmi_proc_entry *file, hdmi_device_id hdmi_id)
{
    td_u32 index;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_app_attr *app_attr = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_warn_return_void(hdmi_dev);

    app_attr = &(hdmi_dev->attr.app_attr);
    /* construct a title */
    call_sys_print_proc_title(file, " app attr ");
    /* line 1 */
    osal_seq_printf(file, "%-20s: %-20s", "hdmi enable", app_attr->enable_hdmi ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s", "audio enable", app_attr->enable_audio ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s", "auth mode enable", app_attr->auth_mode ? STR_YES : STR_NO);
    osal_seq_printf(file, "\n");
    /* line 2 */
    index = min((td_s32)app_attr->deep_color_mode, HDMI_DEEP_COLOR_48BIT + 1);
    osal_seq_printf(file, "%-20s: %-20s", "deep color mode", g_deep_color[index]);
    osal_seq_printf(file, "%-20s: %-20s", "deep color adapt", app_attr->enable_deep_clr_adapt ? STR_YES : STR_NO);

    return;
}

static td_void sw_status_proc_show(hdmi_proc_entry *file, hdmi_device_id hdmi_id)
{
    td_u32 i;
    td_u32 index;
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_warn_return_void(hdmi_dev);

    /* construct a title */
    call_sys_print_proc_title(file, " sw status ");
    /* line 1 */
    osal_seq_printf(file, "%-20s: %-20s", "thread run",
        (hdmi_dev->thread_info.thread_timer_sate == HDMI_THREAD_STATE_RUN) ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s:", "run status");
    if (hdmi_dev->run_state > HDMI_RUN_STATE_NONE) {
        for (i = 0; i < hdmi_array_size(g_run_state_str); i++) {
            if ((td_u32)hdmi_dev->run_state & (1 << i)) {
                osal_seq_printf(file, " %s", g_run_state_str[i]);
            }
        }
    } else {
        osal_seq_printf(file, " %s", "NONE");
    }
    osal_seq_printf(file, "\n");
    /* line 2 */
    index = min((td_s32)hdmi_dev->tmds_mode, HDMI_TMDS_MODE_BUTT);
    osal_seq_printf(file, "%-20s: %-20s", "tmds mode", g_tmds_mode_str[index]);
    osal_seq_printf(file, "%-20s: %-20d", "kernel cnt", hdmi_dev->kernel_cnt);
    osal_seq_printf(file, "%-20s: %-20d", "user cnt", hdmi_dev->user_cnt);
    osal_seq_printf(file, "\n");
    /* line 3 */
    osal_seq_printf(file, "%-20s: %-20s", "kernel callback", (hdmi_dev->k_callback != TD_NULL) ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20d", "user callback cnt", hdmi_dev->user_callback_cnt);
    osal_seq_printf(file, "\n");
    osal_seq_printf(file, "%-20s:", "transit state");
    if (hdmi_dev->transition_state > HDMI_TRANSITION_NONE) {
        for (i = 0; i < hdmi_array_size(g_transition_sate_str); i++) {
            if ((td_u32)hdmi_dev->transition_state & (1 << i)) {
                osal_seq_printf(file, "%-20s", g_transition_sate_str[i]);
            }
        }
    } else {
        osal_seq_printf(file, "%-20s", "NONE");
    }
    osal_seq_printf(file, "%-20s: %-20d", "Thread Interval", hdmi_dev->debug.intf_status.event_thread_cycle_time);
    osal_seq_printf(file, "\n");
    /* line 4 */
    osal_seq_printf(file, "%-20s: %-20s", "emi enable", (hdmi_dev->mode_param.emi_en == TD_TRUE) ? STR_YES : STR_NO);
    index = hdmi_dev->mode_param.trace_len;
    index = (index < hdmi_array_size(g_trace_str)) ? index : hdmi_array_size(g_trace_str) - 1;
    osal_seq_printf(file, "%-20s: %-20s", "pcb len", g_trace_str[index]);

    return;
}

static td_void hw_scramble_status_show(const hdmi_device *hdmi_dev, hdmi_proc_entry *file)
{
#ifdef HDMI_SCDC_SUPPORT
    hdmi_scdc_status scdc_status = {0};

    hal_call_void(hdmi_dev, hal_hdmi_scdc_status_get, &scdc_status);
    osal_seq_printf(file, "%-20s: %-20s", "src_scramble", scdc_status.source_scramble_on ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s", "sink_scramble", scdc_status.sink_scramble_on ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20u", "tmds_bit_clk_ratio", scdc_status.tmds_bit_clk_ratio);
#else
    ot_unused(hdmi_dev);
    ot_unused(file);
#endif
}

static td_void hw_detect_timming_show(hdmi_proc_entry *file, const hdmi_video_status *video_stat)
{
    /* construct a title */
    call_sys_print_proc_title(file, " detect timing ");
    osal_seq_printf(file, "%-20s: %-20s", "sync sw enable", video_stat->sync_sw_enable ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s", "hsync polarity", video_stat->hsync_polarity ? "P" : "N");
    osal_seq_printf(file, "%-20s: %-20s", "progressive", video_stat->progressive ? STR_YES : STR_NO);
    osal_seq_printf(file, "\n");
    osal_seq_printf(file, "%-20s: %-20s", "vsync polarity", video_stat->vsync_polarity ? "P" : "N");
    osal_seq_printf(file, "%-20s: %-20u", "hsync total", video_stat->hsync_total);
    osal_seq_printf(file, "%-20s: %-20u", "hactive cnt", video_stat->hactive_cnt);
    osal_seq_printf(file, "\n");
    osal_seq_printf(file, "%-20s: %-20u", "vsync total", video_stat->vsync_total);
    osal_seq_printf(file, "%-20s: %-20u", "vactive cnt", video_stat->vactive_cnt);

    return;
}

static td_void hw_status_proc_show(hdmi_proc_entry *file, hdmi_device_id hdmi_id)
{
    td_u32 index;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_emi_status emi_status = {0};
    hdmi_hardware_status *hw_status = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_warn_return_void(hdmi_dev);

    hw_status = (hdmi_hardware_status *)osal_vmalloc(sizeof(hdmi_hardware_status));
    if (hw_status == TD_NULL) {
        hdmi_err("alloc hdmi_hardware_status struct memory fail\n");
        return;
    }
    (td_void)memset_s(hw_status, sizeof(hdmi_hardware_status), 0, sizeof(hdmi_hardware_status));

    hal_call_void(hdmi_dev, hal_hdmi_hardware_status_get, hw_status);
    /* construct a title */
    call_sys_print_proc_title(file, " hw status ");
    /* line 1 */
    osal_seq_printf(file, "%-20s: %-20s", "hot plug", hw_status->common_status.hotplug ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s", "rsen", hw_status->common_status.rsen ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s", "phy output enable", hw_status->phy_status.phy_oe ? STR_YES : STR_NO);
    osal_seq_printf(file, "\n");
    /* line 2 */
    osal_seq_printf(file, "%-20s: %-20s", "phy power enable", hw_status->phy_status.phy_power_on ? STR_YES : STR_NO);
    index = min((td_s32)hw_status->common_status.tmds_mode, HDMI_TMDS_MODE_BUTT);
    osal_seq_printf(file, "%-20s: %-20s", "tmds mode", g_tmds_mode_str[index]);
    osal_seq_printf(file, "%-20s: %-20s\n", "avmute", hw_status->common_status.avmute ? STR_YES : STR_NO);
    hal_call_void(hdmi_dev, hal_hdmi_emi_status_get, &emi_status);
    osal_seq_printf(file, "%-20s: %-20s", "emi enable", emi_status.hw_emi_enable ? STR_YES : STR_NO);
    osal_seq_printf(file, "\n");

    hw_scramble_status_show(hdmi_dev, file);
    hw_detect_timming_show(file, &hw_status->video_status);

    osal_vfree(hw_status);

    return;
}

static td_void proc_if_raw_data(hdmi_proc_entry *file, td_u32 *index, td_u32 *line, const td_u8 *data, td_u32 data_len)
{
    td_u32 len = data[AVI_OFFSET_LENGTH] + STR_LEN_SUPERADDITION;

    for (; (*index < len) && (*index < data_len); (*index)++) {
        if ((*index) >= *line) {
            *line += STR_LINE_COUNT;
            break;
        } else if ((*index) % 4 == 0) { /* 4, " " is printed every 4 index */
            osal_seq_printf(file, " ");
        }
        osal_seq_printf(file, "%02x ", data[(*index)]);
    }
    osal_seq_printf(file, "\n");

    return;
}

static td_void proc_vic_timing(hdmi_proc_entry *file, const hdmi_vo_attr *vo_attr,
    const hdmi_infoframe_status *info_status)
{
    td_u32 i;
    td_u32 index;
    td_u32 vic;
    td_u32 hdmi_vic;
    enum hdmi_video_format format;
    td_char *fmt_str      = TD_NULL;
    td_char *vsif_fmt_str = TD_NULL;
    hdmi_video_def *video = TD_NULL;
    hdmi_video_4k_def *video_4k = TD_NULL;

    index = min((td_s32)vo_attr->video_timing, HDMI_VIDEO_TIMING_BUTT);
    /* find vic according to voattr */
    vic = drv_hdmi_vic_search(index, vo_attr->picture_aspect, TD_FALSE);
    i = drv_hdmi_vic_to_index(vic);
    video = drv_hdmi_comm_format_param_get(vic);
    if (video != TD_NULL && i < CEA_VIDEO_CODE_MAX) {
        osal_seq_printf(file, "%-20s: %-20s |", "video timing", (video->fmt_str != TD_NULL) ? video->fmt_str : "NONE");
    } else {
        osal_seq_printf(file, "%-20s: %-20s |", "video timing", "NONE");
    }
    osal_seq_printf(file, "%-20s: %-20s \n", "avi infoframe enable", info_status->avi_enable ? STR_YES : STR_NO);
    /* line 2 */
    index = min((td_s32)vo_attr->disp_fmt, (td_s32)(hdmi_array_size(g_disp_fmt_str) - 1));
    osal_seq_printf(file, "%-20s: %-20s |", "disp fmt", g_disp_fmt_str[index]);
    /*
     * to modify the proc info when the video format is 4k x 2k
     * find vic according to info_frame
     * 4k x 2k
     */
    format = (info_status->vsif[VENDOR_OFFSET_VIDEO_FMT] >> 0x5) & VENDOR_FARAME_VIDEO_FMT_MASK; /* 5'b, BIT[3:1] */
    if (format == HDMI_VIDEO_FORMAT_4K) {
        hdmi_vic = info_status->vsif[VENDOR_OFFSET_VIC];
        video_4k = drv_hdmi_video_codes_4k_get(hdmi_vic);
        if (video_4k != TD_NULL &&  hdmi_vic >= 1 && hdmi_vic <= CEA861_F_VIDEO_CODES_MAX_4K) {
            vsif_fmt_str = video_4k->fmt_str;
        }
    } else {
        i = drv_hdmi_vic_to_index(info_status->avi[AVI_OFFSET_VIC]);
        if (video != TD_NULL && i < CEA_VIDEO_CODE_MAX) {
            fmt_str = video->fmt_str;
        } else {
            fmt_str = 0;
        }
    }
    osal_seq_printf(file, "%-20s: %s(vic=%2d)\n", "current format",
        (fmt_str == TD_NULL) ? "" : fmt_str, info_status->avi[AVI_OFFSET_VIC]);
    /* line 3 */
    osal_seq_printf(file, "%-20s: %-20d |", "pixel clk", vo_attr->clk_fs);
    osal_seq_printf(file, "%-20s: %s(HDMI vic=%2d)\n", "vsif format", (vsif_fmt_str == TD_NULL) ? "" : vsif_fmt_str,
        (format == HDMI_VIDEO_FORMAT_4K) ? info_status->vsif[VENDOR_OFFSET_VIC] : 0);

    return;
}

static td_void proc_video_attr(hdmi_proc_entry *file, const hdmi_vo_attr *vo_attr,
    const hdmi_infoframe_status *info_status)
{
    td_u32 index;
    hdmi_avi_info_byte1 byte1;
    hdmi_avi_info_byte2 byte2;
    hdmi_avi_info_byte3 byte3;
    hdmi_avi_info_byte5 byte5;

    byte1.u8 = info_status->avi[AVI_OFFSET_PB1];
    byte2.u8 = info_status->avi[AVI_OFFSET_PB2];
    byte3.u8 = info_status->avi[AVI_OFFSET_PB3];
    byte5.u8 = info_status->avi[AVI_OFFSET_PB5];

    /* line 1~3 */
    proc_vic_timing(file, vo_attr, info_status);
    /* line 4 */
    index = min((td_s32)vo_attr->in_bit_depth, HDMI_VIDEO_BITDEPTH_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "in bit depth", g_bit_depth_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "bar data present", g_bar_data_present_str[byte1.bits.bar_data_present]);
    /* line 5 */
    index = min((td_s32)vo_attr->in_color_space, HDMI_COLORSPACE_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "in color space", g_color_space_str[index]);
    index = byte1.bits.color_space & 0x3;
    osal_seq_printf(file, "%-20s: %-20s \n", "color space", g_color_space_str[index]);
    /* line 6 */
    index = min((td_s32)vo_attr->colorimetry, HDMI_COLORIMETRY_EXTEND);
    osal_seq_printf(file, "%-20s: %-20s |", "colorimetry", g_colorimetry_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "colorimetry", g_colorimetry_str[byte2.bits.colorimetry]);
    /* line 7 */
    index = min((td_s32)vo_attr->picture_aspect, HDMI_PICTURE_ASPECT_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "pic aspect ratio", g_pic_aspect_ratio_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "pic aspect ratio",
        g_pic_aspect_ratio_str[byte2.bits.picture_aspect_ratio]);
    /* line 8 */
    index = min((td_s32)vo_attr->active_aspect, HDMI_ACTIVE_ASPECT_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "act aspect ratio", g_act_aspect_ratio_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "act aspect ratio", g_act_aspect_ratio_str[byte2.bits.act_asp_ratio]);
    /* line 9 */
    osal_seq_printf(file, "%-20s: %-20d |", "pixel repeat", vo_attr->pixel_repeat);
    osal_seq_printf(file, "%-20s: %-20s \n", "pixel repeat", g_pixel_repetition_str[byte5.bits.pixel_repetition]);
    /* line 10 */
    index = min((td_s32)vo_attr->ycc_quantization, HDMI_YCC_QUANTIZATION_RANGE_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "ycc quantization", g_ycc_quantization_str[index]);

    osal_seq_printf(file, "%-20s: %-20s \n", "ycc quantization",
        g_ycc_quantization_str[byte5.bits.ycc_quantization_range]);
    /* line 11 */
    index = min((td_s32)vo_attr->rgb_quantization, HDMI_QUANTIZATION_RANGE_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "rgb quantization", g_rgb_quantization_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "rgb quantization", g_rgb_quantization_str[byte3.bits.rgb_quantization]);
    /* line 12 */
    index = min((td_s32)vo_attr->extended_colorimetry, HDMI_EXTENDED_COLORIMETRY_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "ext colorimetry", g_ext_colorimetry_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "ext colorimetry", g_ext_colorimetry_str[byte3.bits.ext_colorimetry]);
    /* line 13 */
    index = min((td_s32)vo_attr->stereo_mode, HDMI_3D_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "stereo mode", g_stere_mode_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "it content valid", byte3.bits.it_content_valid ? STR_YES : STR_NO);
    /* line 14 */
    osal_seq_printf(file, "%-20s: %-20d |", "vsync pol", vo_attr->v_sync_pol);
    osal_seq_printf(file, "%-20s: %-20d \n", "hsync pol", vo_attr->h_sync_pol);

    return;
}

static td_void proc_video_path(hdmi_proc_entry *file, const hdmi_hardware_status *hw_status)
{
    td_u32 i;
    td_u32 index;
    td_u32 line;
    hdmi_avi_info_byte1 avi_byte1;
    hdmi_avi_info_byte3 avi_byte3;
    hdmi_avi_info_byte5 avi_byte5;

    avi_byte1.u8 = hw_status->info_frame_status.avi[AVI_OFFSET_PB1];
    avi_byte3.u8 = hw_status->info_frame_status.avi[AVI_OFFSET_PB3];
    avi_byte5.u8 = hw_status->info_frame_status.avi[AVI_OFFSET_PB5];

    /* line 15 */
    osal_seq_printf(file, "%-20s: %-20s \n", "it content type", g_it_content_type_str[avi_byte5.bits.it_content_type]);
    /* line 16 */
    osal_seq_printf(file, "%-20s: %-20s |", "video mute", hw_status->video_status.video_mute ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s \n", "pic scaling", g_picture_scaling_str[avi_byte3.bits.picture_scaling]);
    /* line 17 */
    index = min((td_s32)hw_status->video_status.out_bit_depth, HDMI_VIDEO_BITDEPTH_BUTT);
    if (hw_status->video_status.out_color_space == HDMI_COLORSPACE_YCBCR422) {
        index = HDMI_VIDEO_BITDEPTH_12;
    }
    osal_seq_printf(file, "%-20s: %-20s |", "out bit depth", g_bit_depth_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "act fmt present", avi_byte1.bits.act_fmt_info_present ? STR_YES : STR_NO);
    /* line 18 */
    index = min((td_s32)hw_status->video_status.out_color_space, HDMI_COLORSPACE_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "out color space", g_color_space_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "scan info", g_scan_info_str[avi_byte1.bits.scan_info]);
    /* line 19 */
    osal_seq_printf(file, "%-20s: %-20s |", "ycbcr420_422", hw_status->video_status.ycbcr420_422 ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s:\n", "avi info raw data");
    /* line 20 */
    osal_seq_printf(file, "%-20s: %-20s |", "ycbcr422_444", hw_status->video_status.ycbcr422_444 ? STR_YES : STR_NO);
    line = STR_LINE_COUNT;
    i = 0;
    proc_if_raw_data(file, &i, &line, hw_status->info_frame_status.avi, HDMI_INFO_FRAME_MAX_SIZE);
    /* line 21 */
    osal_seq_printf(file, "%-20s: %-20s |", "ycbcr444_422", hw_status->video_status.ycbcr444_422 ? STR_YES : STR_NO);
    proc_if_raw_data(file, &i, &line, hw_status->info_frame_status.avi, HDMI_INFO_FRAME_MAX_SIZE);
    /* line 22 */
    osal_seq_printf(file, "%-20s: %-20s |", "ycbcr422_420", hw_status->video_status.ycbcr422_420 ? STR_YES : STR_NO);
    proc_if_raw_data(file, &i, &line, hw_status->info_frame_status.avi, HDMI_INFO_FRAME_MAX_SIZE);
    /* line 23 */
    osal_seq_printf(file, "%-20s: %-20s |", "rgb2ycbcr", hw_status->video_status.rgb2ycbcr ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s:\n", "vsif info raw data");
    /* line 24 */
    osal_seq_printf(file, "%-20s: %-20s |", "ycbcr2rgb", hw_status->video_status.ycbcr2rgb ? STR_YES : STR_NO);
    line = STR_LINE_COUNT;
    i = 0;
    proc_if_raw_data(file, &i, &line, hw_status->info_frame_status.vsif, HDMI_INFO_FRAME_MAX_SIZE);
    /* line 25 */
    index = min((td_s32)hw_status->video_status.dither, HDMI_VIDEO_DITHER_DISALBE);
    osal_seq_printf(file, "%-20s: %-20s |", "dither", g_dither_mode_str[index]);
    proc_if_raw_data(file, &i, &line, hw_status->info_frame_status.vsif, HDMI_INFO_FRAME_MAX_SIZE);
    index = min((td_s32)hw_status->video_status.out_bit_depth, HDMI_VIDEO_BITDEPTH_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "deep color mode", g_deep_color_str[index]);
    proc_if_raw_data(file, &i, &line, hw_status->info_frame_status.vsif, HDMI_INFO_FRAME_MAX_SIZE);
    osal_seq_printf(file, "%-42s |", " ");
    proc_if_raw_data(file, &i, &line, hw_status->info_frame_status.vsif, HDMI_INFO_FRAME_MAX_SIZE);
    osal_seq_printf(file, "\n");

    return;
}

static td_void video_info_proc_show(hdmi_proc_entry *file, hdmi_device_id hdmi_id)
{
    td_u32 len;
    hdmi_device  *hdmi_dev = TD_NULL;
    hdmi_vo_attr *vo_attr  = TD_NULL;
    hdmi_hardware_status *hw_status = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_warn_return_void(hdmi_dev);
    vo_attr = &(hdmi_dev->attr.vo_attr);

    hw_status = (hdmi_hardware_status *)osal_vmalloc(sizeof(hdmi_hardware_status));
    if (hw_status == TD_NULL) {
        hdmi_err("alloc hdmi_hardware_status struct memory fail\n");
        return;
    }
    (td_void)memset_s(hw_status, sizeof(hdmi_hardware_status), 0, sizeof(hdmi_hardware_status));
    hal_call_void(hdmi_dev, hal_hdmi_hardware_status_get, hw_status);

    /* construct a title */
    len = (td_u32)strlen(" video attr --------------------------- avi info ");
    hdmi_print_proc_title(file, " video attr --------------------------- avi info ", len);
    /* line 1~14 */
    proc_video_attr(file, vo_attr, &(hw_status->info_frame_status));

    /* line 15~25 */
    osal_seq_printf(file, "------------ video path -------------------|");
    proc_video_path(file, hw_status);

    osal_vfree(hw_status);
    return;
}

static td_void porc_auido_attr(hdmi_proc_entry *file, const hdmi_ao_attr *ao_attr,
    const hdmi_hardware_status *hw_status)
{
    td_u32 index;
    hdmi_audio_info_byte1 audio_byte1;
    hdmi_audio_info_byte2 audio_byte2;

    audio_byte1.u8 = hw_status->info_frame_status.audio[AUDIO_OFFSET_PB1];
    audio_byte2.u8 = hw_status->info_frame_status.audio[AUDIO_OFFSET_PB2];

    /* line 1 */
    index = min((td_s32)ao_attr->sound_intf, HDMI_AUDIO_INTF_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "sound intf", g_sound_intf_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "audio info enable",
        hw_status->info_frame_status.audio_enable ? STR_YES : STR_NO);
    /* line 2 */
    index = min((td_s32)ao_attr->audio_code, HDMI_AUDIO_CODING_TYPE_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "code type", g_audio_code_type_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "code type", g_audio_code_type_str[audio_byte1.bits.aduio_coding_type]);
    /* line 3 */
    index = min((td_s32)ao_attr->channels, HDMI_AUDIO_FORMAT_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "channel cnt", g_audio_channel_str[index]);
    osal_seq_printf(file, "%-20s: %-20s \n", "channel cnt", g_audio_chn_str[audio_byte1.bits.audio_chanel_cnt]);
    /* line 4 */
    osal_seq_printf(file, "%-20s: %-20d |", "sample freq", ao_attr->sample_fs);
    osal_seq_printf(file, "%-20s: %-20s \n", "sample freq",
        g_audio_samping_freq_str[audio_byte2.bits.audio_samping_freq]);
    /* line 5 */
    osal_seq_printf(file, "%-20s: %-20d |", "sample depth", ao_attr->sample_depth);
    osal_seq_printf(file, "%-20s: %-20d \n", "sample depth", hw_status->audio_status.sample_depth);
    /* line 6 */
    osal_seq_printf(file, "%-20s: %-20s |", "down sample", ao_attr->down_sample ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s \n", "sample size",
        g_audio_samping_size_str[audio_byte2.bits.audio_samping_size]);

    return;
}

static td_void proc_audio_path(hdmi_proc_entry *file, const hdmi_hardware_status *hw_status)
{
    td_u32 i;
    td_u32 len;
    td_u32 index;
    hdmi_audio_info_byte5 audio_byte5;

    audio_byte5.u8 = hw_status->info_frame_status.audio[AUDIO_OFFSET_PB5];
    osal_seq_printf(file, "%-20s: %-20s \n", "down mix inhibit", audio_byte5.bits.down_mix_inhibit ? STR_YES : STR_NO);
    /* line 8 */
    osal_seq_printf(file, "%-20s: %-20s |", "audio enable",
        hw_status->info_frame_status.audio_enable ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20d \n", "level shift value", audio_byte5.bits.level_shift_value);
    /* line 9 */
    osal_seq_printf(file, "%-20s: %-20s |", "audio mute", hw_status->audio_status.audio_mute ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s \n", "lfe playback", g_lfe_playback_str[audio_byte5.bits.lfe_play_back]);
    /* line 10 */
    index = min((td_s32)hw_status->audio_status.sound_intf, HDMI_AUDIO_INTF_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "sound intf", g_sound_intf_str[index]);
    osal_seq_printf(file, "%-20s: 0x%02x (%u)\n", "channel/speakeralloc",
        hw_status->info_frame_status.audio[AUDIO_OFFSET_CA], hw_status->info_frame_status.audio[AUDIO_OFFSET_CA]);
    /* line 11 */
    index = min((td_s32)hw_status->audio_status.layout, HDMI_AUDIO_FORMAT_BUTT);
    osal_seq_printf(file, "%-20s: %-20s |", "channel cnt", g_audio_channel_str[index]);
    osal_seq_printf(file, "%-20s: \n", "audio info raw data");
    /* line 12 */
    osal_seq_printf(file, "%-20s: %-20d |", "sample freq", hw_status->audio_status.sample_fs);
    len = hw_status->info_frame_status.audio[AUDIO_OFFSET_LENGHT] + STR_LEN_SUPERADDITION;
    for (i = 0; (i < len) && (i < HDMI_INFO_FRAME_MAX_SIZE); i++) {
        if (i >= 8) { /* the 8~31 chars length proc log none " " */
            break;
        } else if ((i % STR_LEN_SUPERADDITION) == 0) {
            osal_seq_printf(file, " ");
        }
        osal_seq_printf(file, "%02x ", hw_status->info_frame_status.audio[i]);
    }
    osal_seq_printf(file, "\n");
    /* line 13 */
    osal_seq_printf(file, "%-20s: %-20d |", "sample depth", hw_status->audio_status.sample_depth);
    len = hw_status->info_frame_status.audio[AUDIO_OFFSET_LENGHT] + STR_LEN_SUPERADDITION;
    for (; (i < len) && (i < HDMI_INFO_FRAME_MAX_SIZE); i++) {
        if (i >= 16) { /* the 16~31 chars length proc log none " " */
            break;
        } else if ((i % STR_LEN_SUPERADDITION) == 0) {
            osal_seq_printf(file, " ");
        }
        osal_seq_printf(file, "%02x ", hw_status->info_frame_status.audio[i]);
    }
    osal_seq_printf(file, "\n");
    /* line 14 */
    osal_seq_printf(file, "%-20s: %-20s |\n", "down sample", hw_status->audio_status.down_sample ? STR_YES : STR_NO);
    /* line 15 */
    osal_seq_printf(file, "%-20s: %-20u |\n", "ref cts", hw_status->audio_status.ref_cts);
    /* line 16 */
    osal_seq_printf(file, "%-20s: %-20u |\n", "reg cts", hw_status->audio_status.reg_cts);
    /* line 17 */
    osal_seq_printf(file, "%-20s: %-20u |\n", "ref n", hw_status->audio_status.ref_n);
    /* line 18 */
    osal_seq_printf(file, "%-20s: %-20u |\n", "reg n", hw_status->audio_status.reg_n);

    return;
}

static td_void audio_info_proc_show(hdmi_proc_entry *file, hdmi_device_id hdmi_id)
{
    td_u32 len;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_ao_attr *ao_attr = TD_NULL;
    hdmi_hardware_status *hw_status = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_warn_return_void(hdmi_dev);
    ao_attr = &(hdmi_dev->attr.ao_attr);

    hw_status = (hdmi_hardware_status *)osal_vmalloc(sizeof(hdmi_hardware_status));
    if (hw_status == TD_NULL) {
        hdmi_err("alloc hdmi_hardware_status struct memory fail\n");
        return;
    }
    (td_void)memset_s(hw_status, sizeof(hdmi_hardware_status), 0, sizeof(hdmi_hardware_status));
    hal_call_void(hdmi_dev, hal_hdmi_hardware_status_get, hw_status);

    /* construct a title */
    len = (td_u32)strlen(" audio attr --------------------------- audio info ");
    hdmi_print_proc_title(file, " audio attr --------------------------- audio info ", len);
    /* line 1~6 */
    porc_auido_attr(file, ao_attr, hw_status);

    /* line 7~18 */
    osal_seq_printf(file, "------------ audio path -------------------|");
    proc_audio_path(file, hw_status);

    osal_vfree(hw_status);
    return;
}

static td_void event_info_proc_show(hdmi_proc_entry *file, hdmi_device_id hdmi_id)
{
    td_u32 i;
    td_u32 j;
    td_s32 ret;
    td_u32 total_pool = 0;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_event_run_ctrl ctrl = {0};
    hdmi_event_run_cnt run_cnt = {0};

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_warn_return_void(hdmi_dev);

    ret = drv_hdmi_event_pool_total_get(hdmi_id, &total_pool);
    hdmi_if_failure_warn_return_void(ret);

    if (total_pool >= HDMI_EVENT_POOL_CNT) {
        hdmi_err("u32TotalPool(%u) must less than %u.\n", total_pool, HDMI_EVENT_POOL_CNT);
        return;
    }

    for (i = 0; i < total_pool; i++) {
        if (drv_hdmi_event_pool_status_get(hdmi_id, i + 1, &ctrl, &run_cnt) != TD_SUCCESS) {
            continue;
        }
        /* construct a title */
        call_sys_print_proc_title(file, " task id=%u event pool[%u] status ", ctrl.pool_id, i);
        /* line1 */
        osal_seq_printf(file, "cnt|%-9s|%-3s|%-5s|%-9s|%-8s|%-8s\n",
            "err total", "hpd", "unhpd", "edid fail", "rsen con", "rsen dis");
        /* line2 */
        osal_seq_printf(file, "wr:|%-9u|%-3u|%-5u|%-9u|%-8u|%-8u\n",
            run_cnt.err_wd_cnt, run_cnt.hpd_wr_cnt, run_cnt.unhpd_wr_cnt,
            run_cnt.edid_fail_wr_cnt, run_cnt.rsen_con_wr_cnt, run_cnt.rsen_dis_wr_cnt);
        /* line3 */
        osal_seq_printf(file, "rd:|%-9u|%-3u|%-5u|%-9u|%-8u|%-8u\n",
            run_cnt.err_rd_cnt, run_cnt.hpd_rd_cnt, run_cnt.unhpd_rd_cnt,
            run_cnt.edid_fail_rd_cnt, run_cnt.rsen_con_rd_cnt, run_cnt.rsen_dis_rd_cnt);
        /* line4 */
        osal_seq_printf(file, "memory[wkflg=%1d |rdable=%2d| rdptr=%-2d| wrptr=%-2d]:",
            ctrl.wakeup_flag, ctrl.readable_cnt, ctrl.read_ptr, ctrl.write_ptr);
        for (j = 0; j < hdmi_array_size(ctrl.event_pool); j++) {
            if (ctrl.event_pool[j] < HDMI_EVENT_BUTT) {
                osal_seq_printf(file, " 0x%02x", ctrl.event_pool[j]);
            }
        }
        osal_seq_printf(file, "\n");
    }

    return;
}

static td_void edid_raw_data_proc_show(hdmi_proc_entry *file, hdmi_device_id hdmi_id)
{
    td_s32 ret;
    td_u32 raw_len;
    td_u32 i;
    td_u8 *edid_raw = TD_NULL;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_edid_info *edid_info = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_warn_return_void(hdmi_dev);

    edid_info = &hdmi_dev->edid_info;
    edid_raw = (td_u8 *)osal_vmalloc(HDMI_EDID_TOTAL_SIZE);
    hdmi_if_null_warn_return_void(edid_raw);
    (td_void)memset_s(edid_raw, HDMI_EDID_TOTAL_SIZE, 0, HDMI_EDID_TOTAL_SIZE);

    ret = drv_hdmi_edid_raw_get(edid_info, edid_raw, HDMI_EDID_TOTAL_SIZE);
    if (ret == OT_ERR_HDMI_NULL_PTR || ret == OT_ERR_HDMI_INVALID_PARA || ret == TD_FAILURE) {
        hdmi_err("edid raw get fail!\n");
        osal_vfree(edid_raw);
        return;
    }
    raw_len = (td_u32)ret;

    /* construct a title */
    call_sys_print_proc_title(file, " edid raw data ");
    if (raw_len != 0xffffffff) {
        for (i = 0; i < raw_len && i < HDMI_EDID_TOTAL_SIZE; i++) {
            if (i == 0) {
                osal_seq_printf(file, "/* %02xH: */ ", i);
            }
            osal_seq_printf(file, "0x%02x,", edid_raw[i]);
            if ((((i + 1) % STR_LEN_SUPERADDITION) == 0) && ((i + 1) < raw_len)) {
                osal_seq_printf(file, "  ");
            }
            if ((((i + 1) % 16) == 0) && ((i + 1) < raw_len)) { /* 16 eq 4*4, means oneline display 4 EDID format */
                osal_seq_printf(file, "\n/* %02xH: */ ", i);
            }
        }
    }

    osal_vfree(edid_raw);

    return;
}

static td_void edid_status_proc_show(hdmi_proc_entry *file, hdmi_device_id hdmi_id)
{
    td_s32 ret;
    hdmi_edid_status status = {0};
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_edid_info *edid_info = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_warn_return_void(hdmi_dev);

    edid_info = &hdmi_dev->edid_info;
    ret = drv_hdmi_edid_status_get(edid_info, &status);
    if (ret != TD_SUCCESS) {
        hdmi_err("drv_hdmi_edid_status_get failed!!!\n");
        return;
    }

    /* construct a title */
    call_sys_print_proc_title(file, " sw status ");
    /* line 1 */
    osal_seq_printf(file, "%-20s: %-20s", "cap from sink", status.cap_sink ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20d \n", "raw update err cnt", status.raw_update_err_cnt);
    /* line 2 */
    osal_seq_printf(file, "%-20s: %-20s", "cap is valid", status.cap_valid ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20d \n", "parse error type", status.parse_err);
    /* line 3 */
    osal_seq_printf(file, "%-20s: %-20s", "raw is valid", status.raw_valid ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: 0x%08x\n", "parse warn type", status.parse_warn);
    /* line 4 */
    osal_seq_printf(file, "%-20s: %-20d", "raw get err cnt", status.raw_get_err_cnt);
    osal_seq_printf(file, "%-20s: %-20d ", "raw length", status.raw_len);

    return;
}

static td_s32 proc_get_cap_info(hdmi_device_id hdmi_id, hdmi_sink_capability **cap)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_edid_info *edid_info = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    if (hdmi_dev == TD_NULL) {
        hdmi_err("get hdmi_dev=null!\n");
        return TD_FAILURE;
    }

    edid_info = &hdmi_dev->edid_info;
    ret = drv_hdmi_edid_capability_get(edid_info, cap);
    if (ret == HDMI_EDID_DATA_INVALID) {
        hdmi_warn("current capability invalid, force to get raw!!!\n");
        ret = drv_hdmi_edid_update(edid_info, HDMI_EDID_UPDATE_SINK);
        if (ret != TD_SUCCESS) {
            hdmi_warn("please make sure HDMI cable connected & start!\n");
            return TD_FAILURE;
        }

        ret = drv_hdmi_edid_capability_get(edid_info, cap);
        if (ret == HDMI_EDID_DATA_INVALID) {
            hdmi_warn("get the edid capability error, ret=%d!\n", ret);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

static td_void sink_basic_cap_proc_show(hdmi_proc_entry *file, const hdmi_sink_capability *cap)
{
    td_u32 i;
    errno_t ret;
    td_char str_tmp_array[PROC_TEMP_STR_LEN] = {0};

    /* construct a title */
    call_sys_print_proc_title(file, " basic cap ");
    /* line 1 */
    osal_seq_printf(file, "%-20s: %-20s", "HDMI1.4 support", cap->support_hdmi ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-1d.%-1d\n", "1st block version", cap->version, cap->revision);
    /* line 2 */
    osal_seq_printf(file, "%-20s: %-20s", "HDMI2.0 support", cap->support_hdmi_20 ? STR_YES : STR_NO);
    ret = memcpy_s(str_tmp_array, PROC_TEMP_STR_LEN, cap->mfrs_info.mfrs_name, 4); /* 4, mfrs_name[4] size 4 type */
    if (ret != EOK) {
        hdmi_err("secure function error:%d\n", ret);
        return;
    }
    osal_seq_printf(file, "%-20s: %-20s\n", "manufacturer name", str_tmp_array);
    /* line 3 */
    osal_seq_printf(file, "%-20s: %-20d", "max tmds clock(MHz)", cap->max_tmds_clock);
    osal_seq_printf(file, "%-20s: %-20u\n", "product code", cap->mfrs_info.product_code);
    /* line 4 */
    osal_seq_printf(file, "%-20s: %-20u", "serial number", cap->mfrs_info.serial_number);
    osal_seq_printf(file, "%-20s: %-20u\n", "week of manufacture", cap->mfrs_info.week);
    /* line 5 */
    osal_seq_printf(file, "%-20s: %-20u", "max disp width", cap->basic_disp_para.max_image_width);
    osal_seq_printf(file, "%-20s: %-20u\n", "max disp height", cap->basic_disp_para.max_image_height);
    /* line 6 */
    osal_seq_printf(file, "%-20s: %-20s", "SCDC support", cap->support_scdc ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20u\n", "year of manufacture", cap->mfrs_info.year);
    /* line 7 */
    osal_seq_printf(file, "%-20s: %-20s", "dvi dual support", cap->support_dvi_dual ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s\n", "cec addr is valid", cap->cec_addr.phy_addr_valid ? STR_YES : STR_NO);
    /* line 8 */
    osal_seq_printf(file, "%-20s: %-20s", "AI support", cap->supports_ai ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %02x.%02x.%02x.%02x\n", "cec addr",
        cap->cec_addr.phy_addr_a, cap->cec_addr.phy_addr_b, cap->cec_addr.phy_addr_c, cap->cec_addr.phy_addr_d);
    /* line 9 */
    osal_seq_printf(file, "%-20s: %-20u", "ext block cnt", cap->ext_block_num);
    /* speaker */
    osal_seq_printf(file, "%-20s:", "speaker support");
    for (i = 0; i < hdmi_array_size(cap->support_audio_speaker); i++) {
        if (i < hdmi_array_size(g_str_speaker_str)) {
            osal_seq_printf(file, " %s", cap->support_audio_speaker[i] ? g_str_speaker_str[i] : "");
        }
    }
    osal_seq_printf(file, "\n");
    /* line 10 */
    osal_seq_printf(file, "%-20s: %-20s", "rgb quan selectable", cap->rgb_qrange_selectable ? STR_YES : STR_NO);
    osal_seq_printf(file, "%-20s: %-20s", "ycc quan selectable", cap->ycc_qrange_selectable ? STR_YES : STR_NO);

    return;
}

static td_void proc_sink_video_deepcolor(hdmi_proc_entry *file, const hdmi_sink_capability *cap)
{
    /* line 3 deep_color */
    osal_seq_printf(file, "%-20s:", "deepcolor");
    if (cap->deep_color.deep_color30_bit) {
        osal_seq_printf(file, " %-s", "rgb_30bit");
    }
    if (cap->deep_color.deep_color36_bit) {
        osal_seq_printf(file, " %-s", "rgb_36bit");
    }
    if (cap->deep_color.deep_color48_bit) {
        osal_seq_printf(file, " %-s", "rgb_48bit");
    }
    if (cap->deep_color.deep_color_y444) {
        osal_seq_printf(file, " %-s", "ycbcr444_samergb");
    }
    osal_seq_printf(file, "\n");
    /* line 4 deep_color_y420 */
    osal_seq_printf(file, "%-20s:", "ycbcr420deepcolor");
    if (cap->deep_color_y420.deep_color30_bit) {
        osal_seq_printf(file, " %-s", "30bit");
    }
    if (cap->deep_color_y420.deep_color36_bit) {
        osal_seq_printf(file, " %-s", "36bit");
    }
    if (cap->deep_color_y420.deep_color48_bit) {
        osal_seq_printf(file, " %-s", "48bit");
    }
    osal_seq_printf(file, "\n");

    return;
}

static td_void proc_sink_video_colorimetry(hdmi_proc_entry *file, const hdmi_edid_colorimetry *colorimetry)
{
    osal_seq_printf(file, "%-20s:", "colorimetry");
    if (colorimetry->xvycc601) {
        osal_seq_printf(file, " %-s", "xvycc601");
    }
    if (colorimetry->xvycc709) {
        osal_seq_printf(file, " %-s", "xvycc709");
    }
    if (colorimetry->sycc601) {
        osal_seq_printf(file, " %-s", "sycc601");
    }
    if (colorimetry->adoble_ycc601) {
        osal_seq_printf(file, " %-s", "adoble_ycc601");
    }
    if (colorimetry->adoble_rgb) {
        osal_seq_printf(file, " %-s", "adoble_rgb");
    }
    if (colorimetry->bt2020c_ycc) {
        osal_seq_printf(file, " %-s", "bt2020_cycc");
    }
    if (colorimetry->bt2020_ycc) {
        osal_seq_printf(file, " %-s", "bt2020_ycc");
    }
    if (colorimetry->bt2020_rgb) {
        osal_seq_printf(file, " %-s", "bt2020_rgb");
    }

    return;
}

static td_void sink_video_cap_proc_show(hdmi_proc_entry *file, const hdmi_sink_capability *cap)
{
    td_u32 i;
    td_char *tmp_fmt = TD_NULL;

    tmp_fmt = proc_vicformat(cap->native_format);
    hdmi_if_null_warn_return_void(tmp_fmt);

    /* construct a title */
    call_sys_print_proc_title(file, " video cap ");
    /* line 1 */
    osal_seq_printf(file, "%-20s: %-s(vic %2d)\n", "native format",
        ((tmp_fmt == TD_NULL) ? "" : tmp_fmt), cap->native_format);
    /* line 2 color_space */
    osal_seq_printf(file, "%-20s:", "color space");
    if (cap->color_space.rgb444) {
        osal_seq_printf(file, " %-s", "rgb444");
    }
    if (cap->color_space.ycbcr444) {
        osal_seq_printf(file, " %-s", "ycbcr444");
    }
    if (cap->color_space.ycbcr422) {
        osal_seq_printf(file, " %-s", "ycbcr422");
    }
    if (cap->only_support_y420_vic_num || cap->support_y420_vic_num) {
        osal_seq_printf(file, " %-s", "ycbcr420");
    }
    osal_seq_printf(file, "\n");
    /* line 3~4 deep_color */
    proc_sink_video_deepcolor(file, cap);

    /* line 5 y420_format[also] */
    osal_seq_printf(file, "%-20s:", "ycbcr420[also]");
    for (i = 0; (i < cap->support_y420_vic_num) && (i < HDMI_EDID_MAX_VIC_COUNT); i++) {
        osal_seq_printf(file, " %u", cap->support_y420_format[i]);
    }
    osal_seq_printf(file, "\n");
    /* line 6 y420_format[only] */
    osal_seq_printf(file, "%-20s:", "ycbcr420[only]");
    for (i = 0; (i < cap->only_support_y420_vic_num) && (i < HDMI_EDID_MAX_VIC_COUNT); i++) {
        osal_seq_printf(file, " %u", cap->only_support_y420_format[i]);
    }
    osal_seq_printf(file, "\n");
    /* line 7 colorimetry */
    proc_sink_video_colorimetry(file, &cap->color_metry);

    return;
}

static td_bool proc_sink_std_format(hdmi_proc_entry *file, const hdmi_edid_std_timing *std_timing)
{
    td_s32 n;
    errno_t ret;
    td_bool line_feed = TD_FALSE;
    td_char *fmt_str = TD_NULL;
    td_char tmp_array[PROC_TEMP_STR_LEN] = {0};

    fmt_str = proc_std_timingformat(std_timing);
    if (fmt_str != TD_NULL) {
        osal_seq_printf(file, "%-21s", fmt_str);
        line_feed = TD_TRUE;
    } else if ((std_timing->hor_active != 0) && (std_timing->ver_active != 0)) {
        n = snprintf_s(tmp_array, PROC_TEMP_STR_LEN, PROC_TEMP_STR_LEN - 1, "%ux%u",
            std_timing->hor_active, std_timing->ver_active);
        if (n < 0) {
            hdmi_err("snprintf_s err\n");
            return TD_FALSE;
        }
        if ((std_timing->refresh_rate) != 0) {
            ret = snprintf_s(tmp_array + n, PROC_LENGTH_SIZE - n,
                PROC_LENGTH_SIZE - n - 1, "_%u", std_timing->refresh_rate);
            if (ret < 0) {
                hdmi_err("snprintf_s err\n");
                return TD_FALSE;
            }
        }
        osal_seq_printf(file, "%-21s", tmp_array);
        line_feed = TD_TRUE;
    }

    return line_feed;
}

static td_void sink_format_cap_proc_show(hdmi_proc_entry *file, const hdmi_sink_capability *cap)
{
    td_u32 i;
    td_u32 j = 0;
    td_char *tmp_fmt = TD_NULL;

    tmp_fmt = proc_vicformat(cap->native_format);
    hdmi_if_null_warn_return_void(tmp_fmt);
    /* construct a title */
    call_sys_print_proc_title(file, " format cap ");
    /* line 1 */
    for (i = 0; (i < cap->support_vic_num) && (i < HDMI_EDID_MAX_VIC_COUNT); i++) {
        tmp_fmt = proc_vicformat(cap->support_format[i]);
        if (tmp_fmt != TD_NULL) {
            osal_seq_printf(file, "%-21s", tmp_fmt);
        } else {
            osal_seq_printf(file, "(vic)%-15u ", cap->support_format[i]);
        }

        j++;
        if ((j % PORC_ONE_LINE_STR_COUNT) == 0) {
            osal_seq_printf(file, "\n");
        }
    }
    for (i = 0; i < hdmi_array_size(cap->std_timing); i++) {
        if (proc_sink_std_format(file, &cap->std_timing[i]) == TD_TRUE) {
            j++;
            if ((j % PORC_ONE_LINE_STR_COUNT) == 0) {
                osal_seq_printf(file, "\n");
            }
        }
    }
    /* VESA enhanced EDID standard release A, rev.1 page 16 */
    for (i = 0;
         (i < cap->estab_num) &&
         (i < HDMI_EDID_ESTABTIMG_BUTT) &&
         (cap->estab_timing[i] < hdmi_array_size(g_estab_timing_str));
         i++) {
        osal_seq_printf(file, "%-21s", g_estab_timing_str[i]);
        j++;
        if ((j % PORC_ONE_LINE_STR_COUNT) == 0) {
            osal_seq_printf(file, "\n");
        }
    }

    return;
}

static td_void sink_audio_cap_proc_show(hdmi_proc_entry *file, const hdmi_sink_capability *cap)
{
    td_u32 i;
    td_u32 j;
    hdmi_audio_format_code aud_fmt_code;
    const hdmi_edid_audio_info *audio_info = TD_NULL;

    /* construct a title */
    call_sys_print_proc_title(file, " audio cap ");
    /* audio info */
    for (i = 0; (i < cap->audio_info_num) && (i < HDMI_EDID_MAX_AUDIO_CAP_COUNT); i++) {
        /* line 1 */
        osal_seq_printf(file, "%s.%-d:\n", "NO", i);
        aud_fmt_code = cap->audio_info[i].aud_fmt_code;
        audio_info = &cap->audio_info[i];
        /* line 2 */
        osal_seq_printf(file, "%-20s: %-20s", "code type", g_audio_code_type_str[aud_fmt_code]);
        osal_seq_printf(file, "%-20s: %-20d\n", "max channel num", audio_info->aud_channel);
        /* line 3 */
        if (audio_info->max_bit_rate) {
            osal_seq_printf(file, "%-20s: %-20d", "max bit rate(KHz)", audio_info->max_bit_rate);
        } else {
            osal_seq_printf(file, "%-20s: %-20s", "max bit rate(KHz)", STR_NA);
        }

        osal_seq_printf(file, "%-20s:", "bit depth");
        if (audio_info->support_bit_depth_num == 0) {
            osal_seq_printf(file, " %s", STR_NA);
        }
        for (j = 0; (j < audio_info->support_bit_depth_num) && (j < HDMI_EDID_MAX_BIT_DEPTH_NUM); j++) {
            osal_seq_printf(file, " %u", audio_info->support_bit_depth[j]);
        }
        osal_seq_printf(file, "\n");
        /* line 4 */
        osal_seq_printf(file, "%-20s:", "sample rate(Hz)");
        for (j = 0; (j < audio_info->support_sample_rate_num) && (j < HDMI_EDID_MAX_SAMPE_RATE_NUM); j++) {
            osal_seq_printf(file, " %u", audio_info->support_sample_rate[j]);
        }
        if (i < (cap->audio_info_num - 1)) {
            osal_seq_printf(file, "\n");
        }
    }

    return;
}

static td_void sink_detail_timing_proc_show(hdmi_proc_entry *file, const hdmi_sink_capability *cap)
{
    td_u32 i;

    /* detail timing */
    if (cap->perfer_timing_num > 0) {
        /* construct a title */
        call_sys_print_proc_title(file, " detail timing ");
        /* line2 */
        osal_seq_printf(file,
            "%-5s:%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s|%-4s\n",
            "[NO.]", "hact", "vact", "P/I", "pclk", "aspw", "asph", "hfb", "hpw",
            "hbb", "vfb", "vpw", "vbb", "img_w", "img_h", "ihs", "ivs", "idv");
        for (i = 0; (i < cap->perfer_timing_num) && (i < HDMI_EDID_MAX_DETAIL_TIMING_COUNT); i++) {
            osal_seq_printf(file,
                "[%3u]:%-4u|%-4u|%-4c|%-3um|%-4u|%-4u|%-4u|%-4u|%-4u|%-4u|%-4u|%-4u|%-5u|%-5u|%-4s|%-4s|%-5s\n",
                i,
                cap->perfer_timing[i].hact,
                cap->perfer_timing[i].vact,
                cap->perfer_timing[i].interlace ? 'I' : 'P',
                cap->perfer_timing[i].pixel_clk / HDMI_THOUSAND,
                cap->perfer_timing[i].aspect_ratio_w,
                cap->perfer_timing[i].aspect_ratio_h,
                cap->perfer_timing[i].hfb,
                cap->perfer_timing[i].hpw,
                cap->perfer_timing[i].hbb,
                cap->perfer_timing[i].vfb,
                cap->perfer_timing[i].vpw,
                cap->perfer_timing[i].vbb,
                cap->perfer_timing[i].image_width,
                cap->perfer_timing[i].image_height,
                cap->perfer_timing[i].ihs ? STR_YES : STR_NO,
                cap->perfer_timing[i].ivs ? STR_YES : STR_NO,
                cap->perfer_timing[i].idv ? STR_YES : STR_NO);
        }
    }
    return;
}

td_void spec_info_printf(hdmi_proc_entry *file, const hdmi_hw_spec *user,
                         const hdmi_hw_spec *def, const hdmi_hw_param *cur)
{
    osal_seq_printf(file, "%-14s|%-6u|%-6u|%-6u|%-7u|%-6u|%-6u|%-6u|%-6u|%-13u\n", "i_de_main_clk",
        user->hw_param[HW_PARAM_100M_BAND].i_de_main_clk, user->hw_param[HW_PARAM_165M_BAND].i_de_main_clk,
        user->hw_param[HW_PARAM_300M_BAND].i_de_main_clk, user->hw_param[HW_PARAM_600M_BAND].i_de_main_clk,
        def->hw_param[HW_PARAM_100M_BAND].i_de_main_clk, def->hw_param[HW_PARAM_165M_BAND].i_de_main_clk,
        def->hw_param[HW_PARAM_300M_BAND].i_de_main_clk, def->hw_param[HW_PARAM_600M_BAND].i_de_main_clk,
        cur->i_de_main_clk);

    osal_seq_printf(file, "%-14s|%-6u|%-6u|%-6u|%-7u|%-6u|%-6u|%-6u|%-6u|%-13u\n", "i_de_main_data",
        user->hw_param[HW_PARAM_100M_BAND].i_de_main_data, user->hw_param[HW_PARAM_165M_BAND].i_de_main_data,
        user->hw_param[HW_PARAM_300M_BAND].i_de_main_data, user->hw_param[HW_PARAM_600M_BAND].i_de_main_data,
        def->hw_param[HW_PARAM_100M_BAND].i_de_main_data, def->hw_param[HW_PARAM_165M_BAND].i_de_main_data,
        def->hw_param[HW_PARAM_300M_BAND].i_de_main_data, def->hw_param[HW_PARAM_600M_BAND].i_de_main_data,
        cur->i_de_main_data);

    osal_seq_printf(file, "%-14s|%-6u|%-6u|%-6u|%-7u|%-6u|%-6u|%-6u|%-6u|%-13u\n", "i_main_clk",
        user->hw_param[HW_PARAM_100M_BAND].i_main_clk, user->hw_param[HW_PARAM_165M_BAND].i_main_clk,
        user->hw_param[HW_PARAM_300M_BAND].i_main_clk, user->hw_param[HW_PARAM_600M_BAND].i_main_clk,
        def->hw_param[HW_PARAM_100M_BAND].i_main_clk, def->hw_param[HW_PARAM_165M_BAND].i_main_clk,
        def->hw_param[HW_PARAM_300M_BAND].i_main_clk, def->hw_param[HW_PARAM_600M_BAND].i_main_clk,
        cur->i_main_clk);

    osal_seq_printf(file, "%-14s|%-6u|%-6u|%-6u|%-7u|%-6u|%-6u|%-6u|%-6u|%-13u\n", "i_main_data",
        user->hw_param[HW_PARAM_100M_BAND].i_main_data, user->hw_param[HW_PARAM_165M_BAND].i_main_data,
        user->hw_param[HW_PARAM_300M_BAND].i_main_data, user->hw_param[HW_PARAM_600M_BAND].i_main_data,
        def->hw_param[HW_PARAM_100M_BAND].i_main_data, def->hw_param[HW_PARAM_165M_BAND].i_main_data,
        def->hw_param[HW_PARAM_300M_BAND].i_main_data, def->hw_param[HW_PARAM_600M_BAND].i_main_data,
        cur->i_main_data);

    osal_seq_printf(file, "%-14s|%-6u|%-6u|%-6u|%-7u|%-6u|%-6u|%-6u|%-6u|%-13u\n", "ft_cap_clk",
        user->hw_param[HW_PARAM_100M_BAND].ft_cap_clk, user->hw_param[HW_PARAM_165M_BAND].ft_cap_clk,
        user->hw_param[HDMI_PROC_HW_PARAM_INDEX_2].ft_cap_clk, user->hw_param[HW_PARAM_600M_BAND].ft_cap_clk,
        def->hw_param[HW_PARAM_100M_BAND].ft_cap_clk, def->hw_param[HW_PARAM_165M_BAND].ft_cap_clk,
        def->hw_param[HDMI_PROC_HW_PARAM_INDEX_2].ft_cap_clk, def->hw_param[HW_PARAM_600M_BAND].ft_cap_clk,
        cur->ft_cap_clk);

    osal_seq_printf(file, "%-14s|%-6u|%-6u|%-6u|%-7u|%-6u|%-6u|%-6u|%-6u|%-13u\n", "ft_cap_data",
        user->hw_param[HW_PARAM_100M_BAND].ft_cap_data, user->hw_param[HW_PARAM_165M_BAND].ft_cap_data,
        user->hw_param[HW_PARAM_300M_BAND].ft_cap_data, user->hw_param[HW_PARAM_600M_BAND].ft_cap_data,
        def->hw_param[HW_PARAM_100M_BAND].ft_cap_data, def->hw_param[HW_PARAM_165M_BAND].ft_cap_data,
        def->hw_param[HW_PARAM_300M_BAND].ft_cap_data, def->hw_param[HW_PARAM_600M_BAND].ft_cap_data,
        cur->ft_cap_data);

    return;
}

static td_void phy_spec_info_proc_show(hdmi_proc_entry *file, hdmi_device_id hdmi_id)
{
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_hardware_status *hwstatus = TD_NULL;
    hdmi_hw_spec *hwspec_user = TD_NULL;
    hdmi_hw_spec *hwspec_def = TD_NULL;
    hdmi_hw_param *hwparam_cur = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_warn_return_void(hdmi_dev);

    hwstatus = (hdmi_hardware_status *)osal_vmalloc(sizeof(hdmi_hardware_status));
    if (hwstatus == TD_NULL) {
        hdmi_err("alloc hdmi_hardware_status struct memory fail\n");
        return;
    }
    (td_void)memset_s(hwstatus, sizeof(hdmi_hardware_status), 0, sizeof(hdmi_hardware_status));

    hal_call_void(hdmi_dev, hal_hdmi_hardware_status_get, hwstatus);

    hwspec_user = &hwstatus->phy_hwspec.hwspec_user;
    hwspec_def = &hwstatus->phy_hwspec.hwspec_def;
    hwparam_cur = &hwstatus->phy_hwspec.hwparam_cur;

    /* construct a title */
    call_sys_print_proc_title(file, " spec info ");
    osal_seq_printf(file, "%-15s%-28s|%-27s|%-13s\n", " ", "user_seting", "default_seting", "cur_seting");
    osal_seq_printf(file, "%-14s|%-6s|%-6s|%-6s|%-7s|%-6s|%-6s|%-6s|%-6s|\n",
        "stage", "first", "second", "third", "fourth", "first", "second", "third", "fourth");

    spec_info_printf(file, hwspec_user, hwspec_def, hwparam_cur);

    osal_vfree(hwstatus);

    return;
}

static td_s32 drv_hdmi_proc_show_ckfn_sys(hdmi_proc_entry *file, const hdmi_device *hdmi_dev)
{
    hdmi_if_null_warn_return(file, TD_FAILURE);
    hdmi_if_null_warn_return(hdmi_dev, TD_FAILURE);

    if ((ckfn_sys_entry() == TD_FALSE) || (ckfn_sys_print_proc_title() == TD_FALSE)) {
        hdmi_err("no sys ko!\n");
        return TD_FAILURE;
    }

    /* to avoid the hal pointer is null when call the proc and deinit at the same time. */
    hdmi_mutex_lock(hdmi_dev->mutex_proc);
    osal_seq_printf(file, "\n%s\n\n\n", VERSION_STRING);
    proc_sw_version(file);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_proc_show(hdmi_proc_entry *file)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_warn_return(file, TD_FAILURE);
    hdmi_dev = (hdmi_device *)file->private;
    hdmi_if_null_warn_return(hdmi_dev, TD_FAILURE);

    ret = drv_hdmi_proc_show_ckfn_sys(file, hdmi_dev);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    app_attr_proc_show(file, hdmi_dev->hdmi_dev_id);
    sw_status_proc_show(file, hdmi_dev->hdmi_dev_id);
    hw_status_proc_show(file, hdmi_dev->hdmi_dev_id);
    event_info_proc_show(file, hdmi_dev->hdmi_dev_id);
    phy_spec_info_proc_show(file, hdmi_dev->hdmi_dev_id);
    hdmi_mutex_unlock(hdmi_dev->mutex_proc);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_vo_proc_show(hdmi_proc_entry *file)
{
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_warn_return(file, TD_FAILURE);
    hdmi_dev = (hdmi_device *)file->private;
    hdmi_if_null_warn_return(hdmi_dev, TD_FAILURE);

    /* to avoid the hal pointer is null when call the proc and deinit at the same time. */
    hdmi_mutex_lock(hdmi_dev->mutex_proc);
    osal_seq_printf(file, "\n%s\n\n\n", VERSION_STRING);
    proc_sw_version(file);
    video_info_proc_show(file, hdmi_dev->hdmi_dev_id);
    hdmi_mutex_unlock(hdmi_dev->mutex_proc);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_ao_proc_show(osal_proc_entry_t *file)
{
    hdmi_device *hdmi_dev = TD_NULL;

    hdmi_if_null_warn_return(file, TD_FAILURE);
    hdmi_dev = (hdmi_device *)file->private;
    hdmi_if_null_warn_return(hdmi_dev, TD_FAILURE);

    /* to avoid the hal pointer is null when call the proc and deinit at the same time. */
    hdmi_mutex_lock(hdmi_dev->mutex_proc);
    osal_seq_printf(file, "\n%s\n\n\n", VERSION_STRING);
    proc_sw_version(file);
    audio_info_proc_show(file, hdmi_dev->hdmi_dev_id);
    hdmi_mutex_unlock(hdmi_dev->mutex_proc);

    return TD_SUCCESS;
}

td_s32 drv_hdmi_sink_proc_show(osal_proc_entry_t *file)
{
    td_s32 ret;
    hdmi_device *hdmi_dev = TD_NULL;
    hdmi_sink_capability *cap = TD_NULL;

    hdmi_if_null_warn_return(file, TD_FAILURE);
    hdmi_dev = (hdmi_device *)file->private;
    hdmi_if_null_warn_return(hdmi_dev, TD_FAILURE);

    ret = drv_hdmi_proc_show_ckfn_sys(file, hdmi_dev);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    edid_raw_data_proc_show(file, hdmi_dev->hdmi_dev_id);
    edid_status_proc_show(file, hdmi_dev->hdmi_dev_id);
    ret = proc_get_cap_info(hdmi_dev->hdmi_dev_id, &cap);
    if (ret == TD_SUCCESS && cap != TD_NULL) {
        sink_basic_cap_proc_show(file, cap);
        sink_video_cap_proc_show(file, cap);
        sink_format_cap_proc_show(file, cap);
        sink_audio_cap_proc_show(file, cap);
        sink_detail_timing_proc_show(file, cap);
    } else {
        hdmi_warn("call proc_get_cap_info failed! ret: %d\n", ret);
    }
    hdmi_mutex_unlock(hdmi_dev->mutex_proc);

    return TD_SUCCESS;
}

