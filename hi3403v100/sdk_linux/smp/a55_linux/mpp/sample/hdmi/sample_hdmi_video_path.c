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

#include <limits.h>
#include <string.h>

#include "sample_hdmi_video_path.h"

#define VIDIO_STREAM_PATH        "."
#define AACLC_SAMPLES_PER_FRAME  1024
#define AUDIO_STREAM_PATH        "./source_file/music_8kHz_mono_1min.pcm"
#define AUDIO_CHANNEL_CNT        2
#define ERRNO_SYS_INIT_FAILE    (-1)
#define ERRNO_VB_INIT_FAILE     (-2)
#define ERRNO_VDEC_START_FAILE  (-3)
#define ERRNO_VO_START_FAILE    (-5)
#define ERRNO_VPSS_START_FAILE  (-4)
#define ERRNO_VDEC_BIND_FAILE   (-6)
#define ERRNO_STREAM_SEND_FAILE (-7)

#define FRAME_RATE_24HZ 24
#define FRAME_RATE_25HZ 25
#define FRAME_RATE_30HZ 30
#define FRAME_RATE_50HZ 50
#define FRAME_RATE_60HZ 60

#define hdmi_if_not_success_return_void(express, name)                                                  \
    do {                                                                                                \
        td_s32 re;                                                                                      \
        re = express;                                                                                   \
        if (re != TD_SUCCESS) {                                                                         \
            printf("\033[0;31mtest case <%s>not pass at line:%d.\033[0;39m\n", __FUNCTION__, __LINE__); \
            return;                                                                                     \
        }                                                                                               \
    } while (0)

/* video info */
static ot_pic_size g_disp_pic_size;
static ot_vo_intf_type g_vo_intf_type = OT_VO_INTF_HDMI;
static ot_size g_disp_size;
static pthread_t g_vdec_thread[NUM_2 * OT_VDEC_MAX_CHN_NUM];
static sample_media_status g_media_status = SAMPLE_MEDIA_RUN;
static ot_hdmi_callback_func g_callback_func;
static hdmi_args g_hdmi_args;
static td_u32 g_vdec_chn_num = 1;
static vdec_thread_param g_vdec_send[OT_VDEC_MAX_CHN_NUM];
static td_u32 g_vpss_grp_num = 1;
static ot_vo_layer g_vo_layer;
static sample_vo_cfg g_vo_config;
static ot_vpss_grp g_vpss_grp;
static td_bool g_vpss_chn_enable[OT_VPSS_MAX_CHN_NUM];
/* audio info */
static ot_payload_type g_payload_type = OT_PT_LPCM;
static sample_adec g_sample_adec[OT_ADEC_MAX_CHN_NUM];
static ot_audio_snd_mode g_audio_snd_mode = OT_AUDIO_SOUND_MODE_MONO;
static ot_audio_sample_rate g_steam_smple_rate = OT_AUDIO_SAMPLE_RATE_BUTT;
static td_bool g_resample_en = TD_FALSE;
static aduio_input g_audio_input;
static td_bool g_audio_input_modified = TD_FALSE;
static const pic_size_param g_pic[] = {
    { OT_VO_OUT_480P60,       PIC_480P      },
    { OT_VO_OUT_576P50,       PIC_576P      },
    { OT_VO_OUT_800x600_60,   PIC_800X600   },
    { OT_VO_OUT_1024x768_60,  PIC_1024X768  },
    { OT_VO_OUT_1366x768_60,  PIC_1366X768  },
    { OT_VO_OUT_1280x1024_60, PIC_1280X1024 },
    { OT_VO_OUT_2560x1440_60, PIC_2560X1440 },
    { OT_VO_OUT_1440x900_60,  PIC_1440X900  },
    { OT_VO_OUT_1280x800_60,  PIC_1280X800  },
    { OT_VO_OUT_1600x1200_60, PIC_1600X1200 },
    { OT_VO_OUT_1680x1050_60, PIC_1680X1050 },
    { OT_VO_OUT_1920x1200_60, PIC_1920X1200 },
    { OT_VO_OUT_640x480_60,   PIC_640X480   },
    { OT_VO_OUT_1920x2160_30, PIC_1920X2160 },
    { OT_VO_OUT_2560x1440_30, PIC_2560X1440 },
    { OT_VO_OUT_2560x1600_60, PIC_2560X1600 },
    { OT_VO_OUT_1080P24,      PIC_1080P     },
    { OT_VO_OUT_1080P25,      PIC_1080P     },
    { OT_VO_OUT_1080P30,      PIC_1080P     },
    { OT_VO_OUT_1080P50,      PIC_1080P     },
    { OT_VO_OUT_1080P60,      PIC_1080P     },
    { OT_VO_OUT_1080I50,      PIC_1080P     },
    { OT_VO_OUT_1080I60,      PIC_1080P     },
    { OT_VO_OUT_720P50,       PIC_720P      },
    { OT_VO_OUT_720P60,       PIC_720P      },
    { OT_VO_OUT_3840x2160_24, PIC_3840X2160 },
    { OT_VO_OUT_3840x2160_25, PIC_3840X2160 },
    { OT_VO_OUT_3840x2160_30, PIC_3840X2160 },
    { OT_VO_OUT_3840x2160_50, PIC_3840X2160 },
    { OT_VO_OUT_3840x2160_60, PIC_3840X2160 },
    { OT_VO_OUT_4096x2160_24, PIC_4096X2160 },
    { OT_VO_OUT_4096x2160_25, PIC_4096X2160 },
    { OT_VO_OUT_4096x2160_30, PIC_4096X2160 },
    { OT_VO_OUT_4096x2160_50, PIC_4096X2160 },
    { OT_VO_OUT_4096x2160_60, PIC_4096X2160 },
    { OT_VO_OUT_7680x4320_30, PIC_7680X4320 },
};

static const vo_hdmi_fmt g_vo_hdmi_fmt[] = {
    { OT_VO_OUT_PAL,          OT_HDMI_VIDEO_FORMAT_PAL               },
    { OT_VO_OUT_NTSC,         OT_HDMI_VIDEO_FORMAT_NTSC              },
    { OT_VO_OUT_1080P24,      OT_HDMI_VIDEO_FORMAT_1080P_24          },
    { OT_VO_OUT_1080P25,      OT_HDMI_VIDEO_FORMAT_1080P_25          },
    { OT_VO_OUT_1080P30,      OT_HDMI_VIDEO_FORMAT_1080P_30          },
    { OT_VO_OUT_720P50,       OT_HDMI_VIDEO_FORMAT_720P_50           },
    { OT_VO_OUT_720P60,       OT_HDMI_VIDEO_FORMAT_720P_60           },
    { OT_VO_OUT_1080I50,      OT_HDMI_VIDEO_FORMAT_1080i_50          },
    { OT_VO_OUT_1080I60,      OT_HDMI_VIDEO_FORMAT_1080i_60          },
    { OT_VO_OUT_1080P50,      OT_HDMI_VIDEO_FORMAT_1080P_50          },
    { OT_VO_OUT_1080P60,      OT_HDMI_VIDEO_FORMAT_1080P_60          },
    { OT_VO_OUT_576P50,       OT_HDMI_VIDEO_FORMAT_576P_50           },
    { OT_VO_OUT_480P60,       OT_HDMI_VIDEO_FORMAT_480P_60           },
    { OT_VO_OUT_640x480_60,   OT_HDMI_VIDEO_FORMAT_861D_640X480_60   },
    { OT_VO_OUT_800x600_60,   OT_HDMI_VIDEO_FORMAT_VESA_800X600_60   },
    { OT_VO_OUT_1024x768_60,  OT_HDMI_VIDEO_FORMAT_VESA_1024X768_60  },
    { OT_VO_OUT_1280x1024_60, OT_HDMI_VIDEO_FORMAT_VESA_1280X1024_60 },
    { OT_VO_OUT_1366x768_60,  OT_HDMI_VIDEO_FORMAT_VESA_1366X768_60  },
    { OT_VO_OUT_1440x900_60,  OT_HDMI_VIDEO_FORMAT_VESA_1440X900_60  },
    { OT_VO_OUT_1280x800_60,  OT_HDMI_VIDEO_FORMAT_VESA_1280X800_60  },
    { OT_VO_OUT_1680x1050_60, OT_HDMI_VIDEO_FORMAT_VESA_1680X1050_60 },
    { OT_VO_OUT_1920x2160_30, OT_HDMI_VIDEO_FORMAT_1920x2160_30      },
    { OT_VO_OUT_1600x1200_60, OT_HDMI_VIDEO_FORMAT_VESA_1600X1200_60 },
    { OT_VO_OUT_1920x1200_60, OT_HDMI_VIDEO_FORMAT_VESA_1920X1200_60 },
    { OT_VO_OUT_2560x1440_30, OT_HDMI_VIDEO_FORMAT_2560x1440_30      },
    { OT_VO_OUT_2560x1440_60, OT_HDMI_VIDEO_FORMAT_2560x1440_60      },
    { OT_VO_OUT_2560x1600_60, OT_HDMI_VIDEO_FORMAT_2560x1600_60      },
    { OT_VO_OUT_3840x2160_24, OT_HDMI_VIDEO_FORMAT_3840X2160P_24     },
    { OT_VO_OUT_3840x2160_25, OT_HDMI_VIDEO_FORMAT_3840X2160P_25     },
    { OT_VO_OUT_3840x2160_30, OT_HDMI_VIDEO_FORMAT_3840X2160P_30     },
    { OT_VO_OUT_3840x2160_50, OT_HDMI_VIDEO_FORMAT_3840X2160P_50     },
    { OT_VO_OUT_3840x2160_60, OT_HDMI_VIDEO_FORMAT_3840X2160P_60     }
};

static ot_pic_size sample_vdec_get_diplay_cfg(ot_vo_intf_sync intf_sync)
{
    td_u32 i;
    ot_pic_size pic_size;

    for (i = 0; i < (sizeof(g_pic) / sizeof(g_pic[0])); i++) {
        if (intf_sync == g_pic[i].vo_sync) {
            break;
        }
    }

    if (i < (sizeof(g_pic) / sizeof(g_pic[0]))) {
        pic_size = g_pic[i].pic_size;
    } else {
        pic_size = PIC_3840X2160;
    }

    return pic_size;
}

static td_s32 step2_init_vb(sample_vdec_attr *sample_vdec, td_u32 size)
{
    td_u32 i;
    td_s32 ret;

    for (i = 0; i < g_vdec_chn_num && i < size; i++) {
        sample_vdec[i].type = OT_PT_H265;
        sample_vdec[i].width = FMT_3840_2160_WIDTH;
        sample_vdec[i].height = FMT_3840_2160_HEIGHT;
        sample_vdec[i].mode = OT_VDEC_SEND_MODE_FRAME;
        sample_vdec[i].sample_vdec_video.dec_mode = OT_VIDEO_DEC_MODE_IP;
        sample_vdec[i].sample_vdec_video.bit_width = OT_DATA_BIT_WIDTH_8;
        sample_vdec[i].sample_vdec_video.ref_frame_num = NUM_2;
        sample_vdec[i].display_frame_num = NUM_2;
        sample_vdec[i].frame_buf_cnt = sample_vdec[i].sample_vdec_video.ref_frame_num +
                                       sample_vdec[i].display_frame_num + 1;
    }

    ret = sample_comm_vdec_init_vb_pool(g_vdec_chn_num, &sample_vdec[0], OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        sample_prt("init mod common vb fail for %#x!\n", ret);
        return ERRNO_VB_INIT_FAILE;
    }

    return TD_SUCCESS;
}

static td_s32 step4_start_vpss(ot_vpss_chn_attr *vpss_chn_attr, td_u32 size)
{
    td_u32 i;
    td_s32 ret;
    ot_vpss_grp_attr vpss_grp_attr = {0};

    if (size < NUM_2) {
        sample_prt("param err %d!\n", size);
        return ERRNO_VPSS_START_FAILE;
    }

    sample_comm_vpss_get_default_grp_attr(&vpss_grp_attr);
    vpss_grp_attr.max_width = FMT_3840_2160_WIDTH;
    vpss_grp_attr.max_height = FMT_3840_2160_HEIGHT;

    (td_void)memset_s(g_vpss_chn_enable, sizeof(g_vpss_chn_enable), 0, sizeof(g_vpss_chn_enable));
    g_vpss_chn_enable[1] = TD_TRUE;

    sample_comm_vpss_get_default_chn_attr(&vpss_chn_attr[1]);
    vpss_chn_attr[1].width = g_disp_size.width;
    vpss_chn_attr[1].height = g_disp_size.height;
    vpss_chn_attr[1].compress_mode = OT_COMPRESS_MODE_NONE;

    for (i = 0; i < g_vdec_chn_num; i++) {
        g_vpss_grp = i;
        ret = sample_common_vpss_start(g_vpss_grp, &g_vpss_chn_enable[0],
            &vpss_grp_attr, &vpss_chn_attr[0], OT_VPSS_MAX_CHN_NUM);
        if (ret != TD_SUCCESS) {
            sample_prt("start VPSS fail for %#x!\n", ret);
            return ERRNO_VPSS_START_FAILE;
        }
    }

    return TD_SUCCESS;
}

static td_s32 step5_start_vo(ot_vo_intf_sync intf_sync)
{
    td_s32 ret;

    g_vo_config.vo_dev = SAMPLE_VO_DEV_UHD;
    g_vo_config.vo_intf_type = g_vo_intf_type;
    g_vo_config.intf_sync = intf_sync;
    g_vo_config.pic_size = g_disp_pic_size;
    g_vo_config.bg_color = COLOR_RGB_BLUE;
    g_vo_config.dis_buf_len = NUM_3;
    g_vo_config.dst_dynamic_range = OT_DYNAMIC_RANGE_SDR8;
    g_vo_config.vo_mode = VO_MODE_1MUX;
    g_vo_config.pix_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    g_vo_config.disp_rect.x = 0;
    g_vo_config.disp_rect.y = 0;
    g_vo_config.disp_rect.width = g_disp_size.width;
    g_vo_config.disp_rect.height = g_disp_size.height;
    g_vo_config.image_size.width = g_disp_size.width;
    g_vo_config.image_size.height = g_disp_size.height;
    g_vo_config.vo_part_mode = OT_VO_PARTITION_MODE_SINGLE;

    ret = sample_comm_vo_start_vo(&g_vo_config);
    if (ret != TD_SUCCESS) {
        sample_prt("start VO fail for %#x!\n", ret);
        return ERRNO_VO_START_FAILE;
    }

    return TD_SUCCESS;
}

static td_u64 vdec_frame_rate_get(ot_vo_intf_sync intf_sync)
{
    td_u64 fps;

    switch (intf_sync) {
        case OT_VO_OUT_576P50:
        case OT_VO_OUT_720P50:
        case OT_VO_OUT_1080P50:
        case OT_VO_OUT_3840x2160_50:
        case OT_VO_OUT_4096x2160_50:
        case OT_VO_OUT_240x320_50:
        case OT_VO_OUT_320x240_50:
        case OT_VO_OUT_800x600_50:
        case OT_VO_OUT_1080I50:
            fps = FRAME_RATE_50HZ;
            break;

        case OT_VO_OUT_1080P24:
        case OT_VO_OUT_3840x2160_24:
        case OT_VO_OUT_4096x2160_24:
            fps = FRAME_RATE_24HZ;
            break;

        case OT_VO_OUT_1080P25:
        case OT_VO_OUT_3840x2160_25:
        case OT_VO_OUT_4096x2160_25:
            fps = FRAME_RATE_25HZ;
            break;

        case OT_VO_OUT_1080P30:
        case OT_VO_OUT_2560x1440_30:
        case OT_VO_OUT_1920x2160_30:
        case OT_VO_OUT_3840x2160_30:
        case OT_VO_OUT_4096x2160_30:
            fps = FRAME_RATE_30HZ;
            break;

        default:
            fps = FRAME_RATE_60HZ;
            break;
    }

    return fps;
}

static td_s32 step8_send_stream_to_vdec(sample_vdec_attr *sample_vdec, td_u32 size, ot_vo_intf_sync intf_sync)
{
    td_u32 i;
    td_s32 ret;

    for (i = 0; i < g_vdec_chn_num && i < size; i++) {
        ret = snprintf_s(g_vdec_send[i].c_file_name, FILE_NAME_LEN, FILE_NAME_LEN - 1,
            "source_file/3840x2160_8bit.h265");
        if (ret < 0) {
            sample_prt("snprintf_s err\n");
            return ERRNO_STREAM_SEND_FAILE;
        }
        ret = snprintf_s(g_vdec_send[i].c_file_path, FILE_NAME_LEN, FILE_NAME_LEN - 1, "%s", VIDIO_STREAM_PATH);
        if (ret < 0) {
            sample_prt("snprintf_s err\n");
            return ERRNO_STREAM_SEND_FAILE;
        }
        g_vdec_send[i].type = sample_vdec[i].type;
        g_vdec_send[i].stream_mode = sample_vdec[i].mode;
        g_vdec_send[i].chn_id = i;
        g_vdec_send[i].interval_time = NUM_1000;
        g_vdec_send[i].pts_init = 0;
        g_vdec_send[i].pts_increase = 0;
        g_vdec_send[i].e_thread_ctrl = THREAD_CTRL_START;
        g_vdec_send[i].circle_send = TD_TRUE;
        g_vdec_send[i].milli_sec = 0;
        g_vdec_send[i].min_buf_size = (sample_vdec[i].width * sample_vdec[i].height * NUM_3) >> 1;
        g_vdec_send[i].fps = vdec_frame_rate_get(intf_sync);
    }

    sample_comm_vdec_start_send_stream(g_vdec_chn_num, &g_vdec_send[0], &g_vdec_thread[0],
        OT_VDEC_MAX_CHN_NUM, NUM_2 * OT_VDEC_MAX_CHN_NUM);

    return TD_SUCCESS;
}

static td_void error_handle(td_s32 err)
{
    td_u32 i;
    td_s32 ret;

    switch (err) {
        case ERRNO_STREAM_SEND_FAILE: {
            for (i = 0; i < g_vpss_grp_num; i++) {
                ret = sample_comm_vpss_un_bind_vo(i, 1, g_vo_layer, i);
                if (ret != TD_SUCCESS) {
                    sample_prt("vpss unbind vo fail for %#x!\n", ret);
                }
            }
        }; /* fall-through */
        case ERRNO_VDEC_BIND_FAILE: {
            for (i = 0; i < g_vdec_chn_num; i++) {
                ret = sample_comm_vdec_un_bind_vpss(i, i);
                if (ret != TD_SUCCESS) {
                    sample_prt("vdec unbind vpss fail for %#x!\n", ret);
                }
            }
        }; /* fall-through */
        case ERRNO_VO_START_FAILE: {
            sample_comm_vo_stop_vo(&g_vo_config);
        }; /* fall-through */
        case ERRNO_VPSS_START_FAILE:
        case ERRNO_VDEC_START_FAILE: {
            sample_comm_vdec_stop(g_vdec_chn_num);
        }; /* fall-through */
        case ERRNO_VB_INIT_FAILE: {
            sample_comm_vdec_exit_vb_pool();
        }; /* fall-through */
        case ERRNO_SYS_INIT_FAILE: {
            sample_comm_sys_exit();
        } break;
        default: {
            sample_prt("err errno :%d\n", err);
        } break;
    }

    return;
}

static td_s32 step1_init(ot_vo_intf_sync intf_sync)
{
    td_s32 ret;
    ot_vb_cfg vb_cfg = {0};
    ot_pic_buf_attr buf_attr = {0};

    g_disp_pic_size = sample_vdec_get_diplay_cfg(intf_sync);
    ret = sample_comm_sys_get_pic_size(g_disp_pic_size, &g_disp_size);
    if (ret != TD_SUCCESS) {
        sample_prt("sys get pic size fail for %#x!\n", ret);
        return ERRNO_SYS_INIT_FAILE;
    }

    buf_attr.align = 0;
    buf_attr.bit_width = OT_DATA_BIT_WIDTH_8;
    buf_attr.compress_mode = OT_COMPRESS_MODE_SEG;
    buf_attr.height = FMT_3840_2160_WIDTH;
    buf_attr.width = FMT_3840_2160_HEIGHT;
    buf_attr.pixel_format = OT_PIXEL_FORMAT_YVU_SEMIPLANAR_420;

    vb_cfg.max_pool_cnt = 1;
    vb_cfg.common_pool[0].blk_cnt = NUM_10 * g_vdec_chn_num;
    vb_cfg.common_pool[0].blk_size = ot_common_get_pic_buf_size(&buf_attr);

    ret = sample_comm_sys_init(&vb_cfg);
    if (ret != TD_SUCCESS) {
        sample_prt("init sys fail for %#x!\n", ret);
        return ERRNO_SYS_INIT_FAILE;
    }

    return TD_SUCCESS;
}

td_s32 sample_hdmi_vdec_vpss_vo_start(ot_vo_intf_sync intf_sync)
{
    td_u32 i;
    td_s32 ret;
    sample_vdec_attr sample_vdec[OT_VDEC_MAX_CHN_NUM] = {0};
    ot_vpss_chn_attr vpss_chn_attr[OT_VPSS_MAX_CHN_NUM] = {0};

    /* step1: init */
    ret = step1_init(intf_sync);
    if (ret != TD_SUCCESS) {
        goto end;
    }

    /* step2: init module VB or user VB(for VDEC) */
    ret = step2_init_vb(sample_vdec, OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto end;
    }

    /* step3: start VDEC */
    ret = sample_comm_vdec_start(g_vdec_chn_num, &sample_vdec[0], OT_VDEC_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        ret = ERRNO_VDEC_START_FAILE;
        goto end;
    }

    /* step4: start VPSS */
    ret = step4_start_vpss(vpss_chn_attr, OT_VPSS_MAX_CHN_NUM);
    if (ret != TD_SUCCESS) {
        goto end;
    }

    /* step5: start VO */
    ret = step5_start_vo(intf_sync);
    if (ret != TD_SUCCESS) {
        goto end;
    }

    /* step6:  VDEC bind VPSS */
    for (i = 0; i < g_vdec_chn_num; i++) {
        ret = sample_comm_vdec_bind_vpss(i, i);
        if (ret != TD_SUCCESS) {
            ret = ERRNO_VDEC_BIND_FAILE;
            goto end;
        }
    }

    /* step7:  VPSS bind VO */
    g_vo_layer = g_vo_config.vo_dev;
    for (i = 0; i < g_vpss_grp_num; i++) {
        ret = sample_comm_vpss_bind_vo(i, 1, g_vo_layer, i);
        if (ret != TD_SUCCESS) {
            ret = ERRNO_STREAM_SEND_FAILE;
            goto end;
        }
    }

    /* step8:  send stream to VDEC */
    ret = step8_send_stream_to_vdec(sample_vdec, OT_VDEC_MAX_CHN_NUM, intf_sync);
    if (ret != TD_SUCCESS) {
        goto end;
    }

    return TD_SUCCESS;

end:
    error_handle(ret);

    return TD_FAILURE;
}

td_s32 sample_hdmi_vdec_vpss_vo_stop(td_void)
{
    td_u32 i;
    td_s32 j;

    sample_comm_vdec_stop_send_stream(g_vdec_chn_num, &g_vdec_send[0], &g_vdec_thread[0],
        OT_VDEC_MAX_CHN_NUM, NUM_2 * OT_VDEC_MAX_CHN_NUM);

    for (i = 0; i < g_vpss_grp_num; i++) {
        sample_comm_vpss_un_bind_vo(i, 1, g_vo_layer, i);
    }

    for (i = 0; i < g_vdec_chn_num; i++) {
        sample_comm_vdec_un_bind_vpss(i, i);
    }

    sample_comm_vo_stop_vo(&g_vo_config);

    for (j = g_vpss_grp; j >= 0; j--) {
        sample_common_vpss_stop(j, &g_vpss_chn_enable[0], OT_VPSS_MAX_CHN_NUM);
    }

    sample_comm_vdec_stop(g_vdec_chn_num);
    sample_comm_vdec_exit_vb_pool();
    sample_comm_sys_exit();

    return TD_SUCCESS;
}

td_void sample_hdmi_media_set(td_bool run)
{
    if (run == TD_FALSE) {
        g_media_status = SAMPLE_MEDIA_STOP;
    } else {
        g_media_status = SAMPLE_MEDIA_RUN;
    }

    return;
}

static td_void hdmi_mst_hdmi_convert_sync(ot_vo_intf_sync intf_sync, ot_hdmi_video_format *video_format)
{
    td_u32 i;

    for (i = 0; i < (sizeof(g_vo_hdmi_fmt) / sizeof(g_vo_hdmi_fmt[0])); i++) {
        if (intf_sync == g_vo_hdmi_fmt[i].intf_sync) {
            break;
        }
    }

    if (i < (sizeof(g_vo_hdmi_fmt) / sizeof(g_vo_hdmi_fmt[0]))) {
        *video_format = g_vo_hdmi_fmt[i].hdmi_fmt;
    } else {
        *video_format = OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE;
    }

    return;
}

static td_void hdmi_hot_plug_proc(td_void *private_data)
{
    td_s32 ret;
    hdmi_args args = {0};
    ot_hdmi_attr attr = {0};
    ot_hdmi_sink_capability caps = {0};

    printf("\033[32;1mEVENT: HPD\033[0m\n");

    if (private_data == TD_NULL) {
        printf("\033[31;1m[%s:%d]null pointer!\033[0m\n", __FUNCTION__, __LINE__);
        return;
    }

    ret = memcpy_s(&args, sizeof(args), private_data, sizeof(hdmi_args));
    if (ret != EOK) {
        sample_prt("return; failed!\n");
        return;
    }
    ret = ss_mpi_hdmi_get_sink_capability(args.hdmi, &caps);
    if (ret != TD_SUCCESS) {
        sample_prt("get sink caps failed!\n");
    } else {
        sample_prt("get sink caps success!\n");
    }
    ret = ss_mpi_hdmi_get_attr(args.hdmi, &attr);
    if (ret != TD_SUCCESS) {
        sample_prt("get attr failed: 0x%x!\n", ret);
    }
    ret = ss_mpi_hdmi_set_attr(args.hdmi, &attr);
    if (ret != TD_SUCCESS) {
        sample_prt("set attr failed: 0x%x!\n", ret);
    }
    ss_mpi_hdmi_start(args.hdmi);

    return;
}

static td_void hdmi_un_plug_proc(td_void *private_data)
{
    td_s32 ret;
    hdmi_args args = {0};

    printf("\033[32;1mEVENT: UN-HPD\033[0m\n");

    if (private_data == TD_NULL) {
        printf("\033[31;1m[%s:%d]null pointer!\033[0m\n", __FUNCTION__, __LINE__);
        return;
    }

    ret = memcpy_s(&args, sizeof(args), private_data, sizeof(hdmi_args));
    if (ret != EOK) {
        sample_prt("memcpy_s err. /n");
        return;
    }

    ret = ss_mpi_hdmi_stop(args.hdmi);
    if (ret != TD_SUCCESS) {
        sample_prt("ret = %d./n", ret);
    }

    return;
}

static td_void hdmi_event_call_back(ot_hdmi_event_type event, td_void *private_data)
{
    switch (event) {
        case OT_HDMI_EVENT_HOTPLUG:
            hdmi_hot_plug_proc(private_data);
            break;
        case OT_HDMI_EVENT_NO_PLUG:
            hdmi_un_plug_proc(private_data);
            break;
        default:
            break;
    }

    return;
}

td_void sample_hdmi_start(ot_vo_intf_sync intf_sync)
{
    td_s32 ret;
    ot_hdmi_attr attr = {0};
    ot_vo_hdmi_param hdmi_param = {0};
    ot_hdmi_video_format video_format;
    ot_hdmi_sink_capability capability = {0};

    hdmi_mst_hdmi_convert_sync(intf_sync, &video_format);

    g_hdmi_args.hdmi = OT_HDMI_ID_0;
    g_callback_func.hdmi_event_callback = hdmi_event_call_back;
    g_callback_func.private_data = &g_hdmi_args;

    ret = ss_mpi_hdmi_init();
    sample_if_failure_return_void(ret);

    ret = ss_mpi_hdmi_open(OT_HDMI_ID_0);
    sample_if_failure_return_void(ret);

    ret = ss_mpi_hdmi_register_callback(OT_HDMI_ID_0, &g_callback_func);
    sample_if_failure_return_void(ret);

    ret = ss_mpi_hdmi_get_attr(OT_HDMI_ID_0, &attr);
    sample_if_failure_return_void(ret);

    attr.hdmi_en = TD_TRUE;
    attr.video_format = video_format;
    attr.deep_color_mode = OT_HDMI_DEEP_COLOR_24BIT;
    attr.bit_depth = OT_HDMI_BIT_DEPTH_16;
    attr.audio_en = TD_TRUE;
    attr.sample_rate = OT_HDMI_SAMPLE_RATE_48K;

    ret = ss_mpi_hdmi_set_attr(OT_HDMI_ID_0, &attr);
    sample_if_failure_return_void(ret);

    ret = ss_mpi_hdmi_get_sink_capability(OT_HDMI_ID_0, &capability);
    sample_if_failure_return_void(ret);

    if (capability.support_hdmi == TD_FALSE || capability.support_ycbcr == TD_FALSE) {
        ret = ss_mpi_vo_get_hdmi_param(SAMPLE_VO_DEV_DHD0, &hdmi_param);
        sample_if_failure_return_void(ret);
        hdmi_param.csc.csc_matrix = OT_VO_CSC_MATRIX_BT709FULL_TO_RGBFULL;
        ret = ss_mpi_vo_set_hdmi_param(SAMPLE_VO_DEV_DHD0, &hdmi_param);
        sample_if_failure_return_void(ret);
    }

    return;
}

static td_s32 sample_stop_vo(sample_vo_cfg *vo_config)
{
    td_s32 ret;
    ot_vo_dev vo_dev;
    ot_vo_layer vo_layer;
    sample_vo_mode vo_mode;

    if (vo_config == TD_NULL) {
        sample_print("error:argument can not be NULL\n");
        return TD_FAILURE;
    }

    vo_dev = vo_config->vo_dev;
    vo_layer = vo_config->vo_dev;
    vo_mode = vo_config->vo_mode;

    ret = sample_comm_vo_stop_chn(vo_layer, vo_mode);
    sample_if_failure_return(ret, TD_FAILURE);
    ret = sample_comm_vo_stop_layer(vo_layer);
    sample_if_failure_return(ret, TD_FAILURE);
    ret = sample_comm_vo_stop_dev(vo_dev);
    sample_if_failure_return(ret, TD_FAILURE);

    return TD_SUCCESS;
}

td_s32 sample_hdmi_fmt_change(ot_vo_intf_sync intf_sync, td_bool rgb_enable)
{
    td_s32 ret;
    ot_hdmi_attr attr = {0};
    td_bool rgb_en = TD_FALSE;

    ss_mpi_hdmi_stop(OT_HDMI_ID_0);
    ret = sample_stop_vo(&g_vo_config);
    if (ret != TD_SUCCESS) {
        sample_prt("sys get pic size fail for %#x!\n", ret);
        goto err1;
    }

    g_disp_pic_size = sample_vdec_get_diplay_cfg(intf_sync);
    ret = sample_comm_sys_get_pic_size(g_disp_pic_size, &g_disp_size);
    if (ret != TD_SUCCESS) {
        sample_prt("sys get pic size fail for %#x!\n", ret);
        goto err1;
    }

    ret = ss_mpi_hdmi_get_attr(OT_HDMI_ID_0, &attr);
    sample_if_failure_return(ret, TD_FAILURE);
    if (attr.hdmi_en == TD_FALSE || rgb_enable == TD_TRUE) {
        rgb_en = TD_TRUE;
    }
    sample_comm_vo_set_hdmi_rgb_mode(rgb_en);

    g_vo_config.intf_sync = intf_sync;
    g_vo_config.pic_size = g_disp_pic_size;
    g_vo_config.disp_rect.width = g_disp_size.width;
    g_vo_config.disp_rect.height = g_disp_size.height;
    g_vo_config.image_size.width = g_disp_size.width;
    g_vo_config.image_size.height = g_disp_size.height;
    ret = sample_comm_vo_start_vo(&g_vo_config);
    if (ret != TD_SUCCESS) {
        sample_prt("start VO fail for %#x!\n", ret);
        goto err2;
    }

    return TD_SUCCESS;

err2:
    sample_comm_vo_stop_vo(&g_vo_config);

err1:
    return TD_FAILURE;
}

td_s32 sample_hdmi_set_audio_sample_rate(td_u32 sample_rate)
{
    td_s32 ret;

    if (sample_rate != g_audio_input.sample_rate) {
        g_audio_input.sample_rate = sample_rate;
        g_audio_input_modified = TD_TRUE;

        ret = sample_hdmi_stop_audio();
        sample_if_failure_return(ret, TD_FAILURE);
        ret = sample_hdmi_start_audio();
        sample_if_failure_return(ret, TD_FAILURE);
    }

    return TD_SUCCESS;
}

static td_void hdmi_mst_audio_start_hdmi(ot_aio_attr *attr)
{
    ot_hdmi_attr hdmi_attr = {0};
    ot_hdmi_id hdmi = OT_HDMI_ID_0;

    hdmi_if_not_success_return_void(ss_mpi_hdmi_set_avmute(hdmi, TD_TRUE), "hdmi_set_av_mute");
    hdmi_if_not_success_return_void(ss_mpi_hdmi_get_attr(hdmi, &hdmi_attr), "hdmi_get_attr");

    hdmi_attr.audio_en = TD_TRUE;

    if (attr->sample_rate == OT_AUDIO_SAMPLE_RATE_48000) {
        hdmi_attr.sample_rate = OT_HDMI_SAMPLE_RATE_48K;
    } else if (attr->sample_rate == OT_AUDIO_SAMPLE_RATE_44100) {
        hdmi_attr.sample_rate = OT_HDMI_SAMPLE_RATE_44K;
    } else if (attr->sample_rate == OT_AUDIO_SAMPLE_RATE_32000) {
        hdmi_attr.sample_rate = OT_HDMI_SAMPLE_RATE_32K;
    }

    if (attr->bit_width == OT_AUDIO_BIT_WIDTH_16) {
        hdmi_attr.bit_depth = OT_HDMI_BIT_DEPTH_16;
    } else if (attr->bit_width == OT_AUDIO_BIT_WIDTH_8) {
        hdmi_attr.bit_depth = OT_HDMI_BIT_DEPTH_8;
    } else if (attr->bit_width == OT_AUDIO_BIT_WIDTH_24) {
        hdmi_attr.bit_depth = OT_HDMI_BIT_DEPTH_24;
    }
    hdmi_if_not_success_return_void(ss_mpi_hdmi_stop(hdmi), "hdmi_stop");
    hdmi_if_not_success_return_void(ss_mpi_hdmi_set_attr(hdmi, &hdmi_attr), "hdmi_set_attr");
    hdmi_if_not_success_return_void(ss_mpi_hdmi_start(hdmi), "hdmi_start");
    hdmi_if_not_success_return_void(ss_mpi_hdmi_set_avmute(hdmi, TD_FALSE), "hdmi_set_av_mute");

    return;
}

/*
 * function : start adec
 */
static td_s32 hdmi_mst_start_adec(ot_adec_chn ad_chn, ot_payload_type type)
{
    td_s32 ret;
    ot_adec_chn_attr adec_attr = {0};
    ot_adec_attr_adpcm adec_adpcm = {0};
    ot_adec_attr_lpcm adec_lpcm;

    adec_attr.type = type;
    adec_attr.buf_size = NUM_20;
    adec_attr.mode = OT_ADEC_MODE_STREAM; /* propose use pack mode in your app */

    if (adec_attr.type == OT_PT_ADPCMA) {
        adec_attr.value = &adec_adpcm;
        adec_adpcm.adpcm_type = OT_ADPCM_TYPE_DVI4;
    } else {
        adec_attr.value = &adec_lpcm;
        adec_attr.mode = OT_ADEC_MODE_PACK; /* lpcm must use pack mode */
    }

    /* create adec chn */
    ret = ss_mpi_adec_create_chn(ad_chn, &adec_attr);
    if (ret) {
        sample_prt("adec_create_chn(%d) failed with %#x!\n", ad_chn, ret);
        return ret;
    }
    return 0;
}

static td_s32 hdmi_mst_comm_start_ao(ot_audio_dev ao_dev_id, td_s32 ao_chn_cnt,
    ot_aio_attr *attr, ot_audio_sample_rate in_sample_rate, td_bool resample_en)
{
    td_s32 i;
    td_u32 ret;

    if (attr->chn_cnt == 0) {
        attr->chn_cnt = 1;
    }

    ret = ss_mpi_ao_set_pub_attr(ao_dev_id, attr);
    if (ret != TD_SUCCESS) {
        sample_prt("ao_set_pub_attr(%d) failed with %#x!\n", ao_dev_id, ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_ao_enable(ao_dev_id);
    if (ret != TD_SUCCESS) {
        sample_prt("ao_enable(%d) failed with %#x!\n", ao_dev_id, ret);
        return TD_FAILURE;
    }

    for (i = 0; i < ao_chn_cnt; i++) {
        ret = ss_mpi_ao_enable_chn(ao_dev_id, i);
        if (ret != TD_SUCCESS) {
            sample_prt("ao_enable_chn(%d) failed with %#x!\n", i, ret);
            return TD_FAILURE;
        }

        if (resample_en == TD_TRUE && attr->snd_mode == OT_AUDIO_SOUND_MODE_MONO) {
            ret = ss_mpi_ao_disable_resample(ao_dev_id, i);
            ret = ss_mpi_ao_enable_resample(ao_dev_id, i, in_sample_rate);
            if (ret != TD_SUCCESS) {
                sample_prt("ao_enable_re_smp(%d,%d) failed with %#x!\n", ao_dev_id, i, ret);
                return TD_FAILURE;
            }
        }
    }

    return TD_SUCCESS;
}

/*
 * function : ao bind adec
 */
static td_s32 hdmi_mst_comm_ao_bind_adec(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_adec_chn ad_chn)
{
    ot_mpp_chn src_chn;
    ot_mpp_chn dest_chn;

    src_chn.mod_id = OT_ID_ADEC;
    src_chn.dev_id = 0;
    src_chn.chn_id = ad_chn;
    dest_chn.mod_id = OT_ID_AO;
    dest_chn.dev_id = ao_dev;
    dest_chn.chn_id = ao_chn;

    return ss_mpi_sys_bind(&src_chn, &dest_chn);
}

/*
 * function : ao unbind adec
 */
static td_s32 hdmi_mst_comm_ao_unbind_adec(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_adec_chn ad_chn)
{
    ot_mpp_chn src_chn;
    ot_mpp_chn dest_chn;

    src_chn.mod_id = OT_ID_ADEC;
    src_chn.chn_id = ad_chn;
    src_chn.dev_id = 0;
    dest_chn.mod_id = OT_ID_AO;
    dest_chn.dev_id = ao_dev;
    dest_chn.chn_id = ao_chn;

    return ss_mpi_sys_unbind(&src_chn, &dest_chn);
}

/*
 * function : get stream from file, and send it  to adec
 */
static void *hdmi_mst_comm_adec_proc(void *parg)
{
    td_s32 ret;
    ot_audio_stream aud_stream;
    td_u32 len = NUM_1024;
    td_u32 read_len;
    td_s32 adec_chn;
    td_u8 *audio_stream = NULL;
    const size_t audio_stream_len = OT_MAX_AUDIO_STREAM_LEN;
    sample_adec *adec_ctl = (sample_adec *)parg;
    FILE *pfd = adec_ctl->pfd;
    adec_chn = adec_ctl->ad_chn;

    audio_stream = (td_u8 *)malloc(audio_stream_len);
    if (audio_stream == NULL) {
        sample_prt("malloc failed!\n");
        fclose(pfd);
        return NULL;
    }
    (td_void)memset_s(audio_stream, audio_stream_len, 0, audio_stream_len);

    while (adec_ctl->start == TD_TRUE) {
        /* read from file */
        aud_stream.stream = audio_stream;
        read_len = fread(aud_stream.stream, 1, len, pfd);
        if (read_len <= 0) {
            fseek(pfd, 0, SEEK_SET); /* read file again */
            continue;
        }

        /* here only demo adec streaming sending mode, but pack sending mode is commended */
        aud_stream.len = read_len;
        ret = ss_mpi_adec_send_stream(adec_chn, &aud_stream, TD_TRUE);
        if (ret != TD_SUCCESS) {
            sample_prt("%s: ss_mpi_adec_send_stream(%d) failed with %#x!\n",
                       __FUNCTION__, adec_chn, ret);
            break;
        }
    }

    free(audio_stream);
    audio_stream = NULL;
    if (fclose(pfd) != 0) {
        sample_prt("%s: fclose failed!\n", __FUNCTION__);
    }
    adec_ctl->start = TD_FALSE;

    return NULL;
}

/*
 * function : create the thread to get stream from file and send to adec
 */
static td_s32 hdmi_mst_comm_creat_trd_file_adec(ot_adec_chn ad_chn, td_char *src_path)
{
    td_char path[PATH_MAX] = {0};
    sample_adec *adec = NULL;

    if (src_path == NULL) {
        return TD_FAILURE;
    }

    if (ad_chn >= OT_ADEC_MAX_CHN_NUM) {
        sample_prt("param err!\n");
        return TD_FAILURE;
    }

    adec = &g_sample_adec[ad_chn];
    adec->ad_chn = ad_chn;

    if (strlen(src_path) >= PATH_MAX || realpath(src_path, path) == TD_NULL) {
        sample_prt("\033[31;1m realpath err! \033[0m\n");
        return TD_FAILURE;
    }

    adec->pfd = fopen(path, "rb");
    if (adec->pfd == NULL) {
        sample_prt("open file %s failed\n", path);
        return TD_FAILURE;
    }
    adec->start = TD_TRUE;
    pthread_create(&adec->ad_pid, 0, hdmi_mst_comm_adec_proc, adec);

    return TD_SUCCESS;
}

/*
 * function : file -> a_dec -> ao
 */
static td_void hdmi_mst_adec_ao(ot_aio_attr *attr, td_char *src_path)
{
    td_s32 i;
    td_s32 ao_chn_cnt;

    g_audio_input.ao_dev = 1;
    if (attr == NULL) {
        sample_prt("\033[31;1m%s: input point is invalid!\033[0m\n", __FUNCTION__);
        return;
    }
    hdmi_mst_audio_start_hdmi(attr);
    ao_chn_cnt = (td_s32)(attr->chn_cnt >> (td_u32)attr->snd_mode);
    for (i = 0; i < ao_chn_cnt; i++) {
        hdmi_if_not_success_return_void(hdmi_mst_start_adec(i, g_payload_type), "start_adec");
    }
    hdmi_if_not_success_return_void(hdmi_mst_comm_start_ao(g_audio_input.ao_dev, ao_chn_cnt, attr,
                                    g_steam_smple_rate, g_resample_en), "start_ao");
    for (i = 0; i < ao_chn_cnt; i++) {
        hdmi_if_not_success_return_void(hdmi_mst_comm_ao_bind_adec(g_audio_input.ao_dev, i, i), "ao_bind_adec");
        hdmi_if_not_success_return_void(hdmi_mst_comm_creat_trd_file_adec(i, src_path), "creat_trd_file_adec");
    }

    return;
}

td_s32 sample_hdmi_start_audio(td_void)
{
    td_u32 strm_smprate;
    td_u32 strm_point_num;
    ot_aio_attr attr = {0};

    if (g_audio_input_modified != TD_TRUE) {
        g_audio_input.src_path = AUDIO_STREAM_PATH;
        g_audio_input.chnl_num = AUDIO_CHANNEL_CNT;
        g_audio_input.sample_rate = OT_HDMI_SAMPLE_RATE_48K;
    }

    g_payload_type = OT_PT_LPCM;

    /* init aio. all of cases will use it */
    attr.bit_width = OT_AUDIO_BIT_WIDTH_16;
    attr.work_mode = OT_AIO_MODE_I2S_MASTER;
    attr.snd_mode = OT_AUDIO_SOUND_MODE_MONO;
    attr.expand_flag = 0;
    attr.frame_num = NUM_30;
    attr.chn_cnt = AUDIO_CHANNEL_CNT;
    attr.clk_share = 1;
    attr.i2s_type = OT_AIO_I2STYPE_INNERHDMI;
    attr.point_num_per_frame = AACLC_SAMPLES_PER_FRAME;

    if (g_audio_input.sample_rate == OT_HDMI_SAMPLE_RATE_48K) {
        attr.sample_rate = OT_AUDIO_SAMPLE_RATE_48000;
    } else if (g_audio_input.sample_rate == OT_HDMI_SAMPLE_RATE_44K) {
        attr.sample_rate = OT_AUDIO_SAMPLE_RATE_44100;
    } else if (g_audio_input.sample_rate == OT_HDMI_SAMPLE_RATE_32K) {
        attr.sample_rate = OT_AUDIO_SAMPLE_RATE_32000;
    } else {
        sample_prt("wrong audio sample rate: %d!\n", g_audio_input.sample_rate);
        return TD_FAILURE;
    }

    strm_smprate = OT_AUDIO_SAMPLE_RATE_8000;
    strm_point_num = NUM_320;
    attr.point_num_per_frame = strm_point_num * attr.sample_rate / strm_smprate;

    if (attr.point_num_per_frame > OT_MAX_AO_POINT_NUM || attr.point_num_per_frame < OT_MIN_AUDIO_POINT_NUM) {
        sample_prt("invalid OT_PT_num_per_frm:%d\n", attr.point_num_per_frame);
        return TD_FAILURE;
    }

    g_resample_en = TD_TRUE;
    g_steam_smple_rate = strm_smprate;
    hdmi_mst_adec_ao(&attr, g_audio_input.src_path);
    g_audio_snd_mode = attr.snd_mode;

    return TD_SUCCESS;
}

/*
 * function : destroy the thread to get stream from file and send to adec
 */
static td_s32 hdmi_mst_comm_destory_trd_file_adec(ot_adec_chn ad_chn)
{
    sample_adec *adec = NULL;

    if (ad_chn >= OT_ADEC_MAX_CHN_NUM) {
        printf("err param ad_chn: %d\n", ad_chn);
        return TD_FAILURE;
    }

    adec = &g_sample_adec[ad_chn];
    if (adec->start) {
        adec->start = TD_FALSE;
        pthread_join(adec->ad_pid, 0);
    }

    return TD_SUCCESS;
}

static td_s32 hdmi_mst_comm_stop_ao(ot_audio_dev ao_dev_id, td_s32 ao_chn_cnt, td_bool resample_en)
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < ao_chn_cnt; i++) {
        if (resample_en == TD_TRUE) {
            ret = ss_mpi_ao_disable_resample(ao_dev_id, i);
            if (ret != TD_SUCCESS) {
                return ret;
            }
        }
        ret = ss_mpi_ao_disable_chn(ao_dev_id, i);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }
    ret = ss_mpi_ao_disable(ao_dev_id);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}

/*
 * function : stop adec
 */
static td_s32 hdmi_mst_comm_stop_adec(ot_adec_chn ad_chn)
{
    td_s32 ret;

    ret = ss_mpi_adec_destroy_chn(ad_chn);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_hdmi_stop_audio(td_void)
{
    td_s32 ret;
    td_u32 i;
    td_u32 ao_chn_cnt = g_audio_input.chnl_num >> (td_u32)g_audio_snd_mode;

    for (i = 0; i < ao_chn_cnt; i++) {
        ret = hdmi_mst_comm_destory_trd_file_adec(i);
        ret = hdmi_mst_comm_ao_unbind_adec(g_audio_input.ao_dev, i, i);
    }
    ret = hdmi_mst_comm_stop_ao(g_audio_input.ao_dev, ao_chn_cnt, g_resample_en);

    for (i = 0; i < ao_chn_cnt; i++) {
        ret = hdmi_mst_comm_stop_adec(i);
    }

    return ret;
}

