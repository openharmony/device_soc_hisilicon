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

#include "sample_hdmi_cmd.h"

#define HDMI_MAX_ARGC          20
#define HDMI_MAX_PAMRM_SIZE    30
#define HDMI_MAX_ASPECTRATIO   2
#define HDMI_MAX_AUTHMODE      2
#define HDMI_MAX_AUDIO_FREQNCE 3
#define INDEX_720P60           4
#define INDEX_1080P60          8
#define INVAILD_IN_PARAMETERS  0xFFFFFFFF

#define valid_char_to_args(i, j, p)                                                                     \
    do {                                                                                                \
        if (((p) >= 'a' && (p) <= 'z') || ((p) >= '0' && (p) <= '9') || ((p) == '_') || ((p) == '-')) { \
            g_args[i][j] = (p);                                                                         \
            (j)++;                                                                                      \
        }                                                                                               \
    } while (0)

static ot_hdmi_id g_test_hdmi_id = OT_HDMI_ID_0;
static td_bool g_vo_hdmi_rgb_mode = TD_FALSE;
static td_u8 g_args[HDMI_MAX_ARGC][HDMI_MAX_PAMRM_SIZE] = {0};
static hdmi_input_param g_hdmi_timing[] = {
    { OT_VO_OUT_1080P24,      "1080P_24" },
    { OT_VO_OUT_1080P25,      "1080P_25" },
    { OT_VO_OUT_1080P30,      "1080P_30" },
    { OT_VO_OUT_720P50,       "720P_50" },
    { OT_VO_OUT_720P60,       "720P_60" },
    { OT_VO_OUT_1080I50,      "1080i_50" },
    { OT_VO_OUT_1080I60,      "1080i_60" },
    { OT_VO_OUT_1080P50,      "1080P_50" },
    { OT_VO_OUT_1080P60,      "1080P_60" },
    { OT_VO_OUT_576P50,       "576P_50" },
    { OT_VO_OUT_480P60,       "480P_60" },
    { OT_VO_OUT_640x480_60,   "640x480p_60" },
    { OT_VO_OUT_800x600_60,   "800x600" },
    { OT_VO_OUT_1024x768_60,  "1024X768" },
    { OT_VO_OUT_1280x1024_60, "1280X1024" },
    { OT_VO_OUT_1366x768_60,  "1366X768" },
    { OT_VO_OUT_1440x900_60,  "1440X900" },
    { OT_VO_OUT_1280x800_60,  "1280X800" },
    { OT_VO_OUT_1680x1050_60, "1680X1050" },
    { OT_VO_OUT_1920x2160_30, "1920X2160" },
    { OT_VO_OUT_1600x1200_60, "1600X1200" },
    { OT_VO_OUT_1920x1200_60, "1920X1200_60" },
    { OT_VO_OUT_2560x1440_30, "2560X1440_30" },
    { OT_VO_OUT_2560x1440_60, "2560X1440_60" },
    { OT_VO_OUT_2560x1600_60, "2560X1600" },
    { OT_VO_OUT_3840x2160_24, "3840X2160_24" },
    { OT_VO_OUT_3840x2160_25, "3840X2160_25" },
    { OT_VO_OUT_3840x2160_30, "3840X2160_30" },
#ifndef HDMI_SUPPORT_1_4
    { OT_VO_OUT_3840x2160_50, "3840X2160_50" },
    { OT_VO_OUT_3840x2160_60, "3840X2160_60" },
#endif
    { OT_VO_OUT_BUTT,         "BUTT" }
};

static hdmi_input_param g_hdmi_v_color_mode[] = {
    { 0, "RGB" },
    { 1, "YUV422" },
    { 2, "YUV444" },
#ifndef HDMI_SUPPORT_1_4
    { 3, "YUV420" }
#endif
};

static hdmi_input_param g_hdmi_v_aspect_ratio[HDMI_MAX_ASPECTRATIO] = {
    { 0, "4:3" },
    { 1, "16:9" }
};

static hdmi_input_param g_hdmi_a_freq[HDMI_MAX_AUDIO_FREQNCE] = {
    { 0, "32000" },
    { 1, "44100" },
    { 2, "48000" }
};

static td_u32 hdmi_string_to_integer(td_u8 *ptr)
{
    td_u32 index;
    td_u32 string_len;

    if (ptr == NULL) {
        return INVAILD_IN_PARAMETERS;
    }
    string_len = strlen((char *)ptr);
    if (string_len > NUM_16) {
        return INVAILD_IN_PARAMETERS;
    }
    for (index = 0; index < string_len; index++) {
        if ((ptr[index] >= '0') && (ptr[index] <= '9')) {
            continue;
        }
        return INVAILD_IN_PARAMETERS;
    }

    return (atoi((char *)ptr));
}

static td_void check_valid_character(td_char *ptr, td_u32 len)
{
    td_u32 i;

    /* search the first character char */
    while (*ptr == ' ' || *ptr == '\0' || *ptr == '\t') {
        ptr++;
    }
    /* change to little case character */
    for (i = (len - 1); i > 0; i--) {
        if ((*(ptr + i - 1) >= 'A') && (*(ptr + i - 1) <= 'Z')) {
            *(ptr + i - 1) = 'a' + (*(ptr + i - 1) - 'A');
        }
    }

    return;
}

/* adjust input parameter */
static td_u8 hdmi_adjust_string(td_char *ptr, td_u32 len)
{
    td_u32 i;
    td_u32 j;

    check_valid_character(ptr, len);

    for (i = 0; i < HDMI_MAX_ARGC; i++) {
        (td_void)memset_s(g_args[i], HDMI_MAX_PAMRM_SIZE, 0, HDMI_MAX_PAMRM_SIZE);
    }
    /* fill g_args[][] with input string */
    for (i = 0; i < HDMI_MAX_ARGC; i++) {
        j = 0;
        while (*ptr == ' ' || *ptr == '\t') {
            ptr++;
        }

        while ((*ptr != ' ') && (*ptr != '\0') && (*ptr != '\t') && j < HDMI_MAX_PAMRM_SIZE) {
            valid_char_to_args(i, j, *ptr);
            ptr++;
        }

        if (j >= HDMI_MAX_PAMRM_SIZE) {
            printf("\033[32;1mparam err!\033[0m\n");
            return 0;
        }
        if ((i != 0) && (j == 0)) {
            printf("Invalid arg[%d]!\n", i);
            return 0;
        }

        g_args[i][j] = '\0';
        /* LF : 0xA, NL line feed, new line */
        while (*ptr == ' ' || *ptr == '\t' || *ptr == 0xA) {
            ptr++;
        }
        if (*ptr == '\0') {
            i++;
            break;
        }
    }

    return i;
}

static td_void hdmi_hdmi_force(td_void)
{
    td_s32 ret;
    ot_hdmi_attr attr = {0};

    printf("hdmi_hdmi_force  with ss_mpi_hdmi_set_attr\n");
    ret = ss_mpi_hdmi_stop(g_test_hdmi_id);
    sample_if_failure_return_void(ret);
    ret = ss_mpi_hdmi_get_attr(g_test_hdmi_id, &attr);
    sample_if_failure_return_void(ret);
    attr.hdmi_en = TD_TRUE;
    attr.audio_en = TD_TRUE;
    ret = ss_mpi_hdmi_set_attr(g_test_hdmi_id, &attr);
    sample_if_failure_return_void(ret);
    ret = ss_mpi_hdmi_start(g_test_hdmi_id);
    sample_if_failure_return_void(ret);

    return;
}

static td_void hdmi_dvi_force(td_void)
{
    td_s32 ret;
    ot_hdmi_attr attr = {0};
    ot_vo_hdmi_param hdmi_param = {0};

    printf("hdmi_dvi_force with ot_hdmi_set_attr\n");
    ret = ss_mpi_hdmi_stop(g_test_hdmi_id);

    ret = ss_mpi_vo_get_hdmi_param(SAMPLE_VO_DEV_DHD0, &hdmi_param);
    sample_if_failure_return_void(ret);
    hdmi_param.csc.csc_matrix = OT_VO_CSC_MATRIX_BT601LIMIT_TO_RGBFULL;
    ret = ss_mpi_vo_set_hdmi_param(SAMPLE_VO_DEV_DHD0, &hdmi_param);
    sample_if_failure_return_void(ret);
    ret = ss_mpi_hdmi_get_attr(g_test_hdmi_id, &attr);
    sample_if_failure_return_void(ret);
    attr.hdmi_en = TD_FALSE;
    attr.audio_en = TD_FALSE;
    ret = ss_mpi_hdmi_set_attr(g_test_hdmi_id, &attr);
    sample_if_failure_return_void(ret);
    ret = ss_mpi_hdmi_start(g_test_hdmi_id);
    sample_if_failure_return_void(ret);

    return;
}

static td_s32 hdmi_deepcolor(td_u32 deep_color_flag)
{
    td_s32 ret;
    ot_hdmi_attr attr = {0};
    ot_hdmi_sink_capability sink_cap = {0};

    ret = ss_mpi_hdmi_get_sink_capability(g_test_hdmi_id, &sink_cap);
    if (ret == TD_SUCCESS) {
        if (deep_color_flag == OT_HDMI_DEEP_COLOR_30BIT) {
            if (sink_cap.support_deepcolor_30bit != TD_TRUE) {
                printf("OT_HDMI_DEEP_COLOR_30BIT do not support in sink device\n");
            }
        } else if (deep_color_flag == OT_HDMI_DEEP_COLOR_36BIT) {
            if (sink_cap.support_deepcolor_36bit != TD_TRUE) {
                printf("OT_HDMI_DEEP_COLOR_36BIT do not support in sink device\n");
            }
        } else {
            printf("deep_color:0x%x do not support in current environment\n", deep_color_flag);
        }
    } else {
        printf("can't get capability, force set attr \n");
    }

    printf("hdmi_deepcolor deep_color_flag:%d\n", deep_color_flag);

    ret = ss_mpi_hdmi_stop(g_test_hdmi_id);
    sample_if_failure_return(ret, TD_FAILURE);
    ret = ss_mpi_hdmi_get_attr(g_test_hdmi_id, &attr);
    sample_if_failure_return(ret, TD_FAILURE);
    attr.deep_color_mode = deep_color_flag;
    ret = ss_mpi_hdmi_set_attr(g_test_hdmi_id, &attr);
    if (ret == OT_ERR_HDMI_FEATURE_NO_SUPPORT) {
        printf("feature no support.\n");
    }
    printf("return :%d\n", ret);
    ret = ss_mpi_hdmi_start(g_test_hdmi_id);
    sample_if_failure_return(ret, TD_FAILURE);

    return ret;
}

static td_void hdmi_video_timing(td_u32 index, td_u32 timing)
{
    td_s32 ret;

    printf("set %s, timing index(%d)\n", g_hdmi_timing[index].index_string, timing);
    ret = sample_hdmi_fmt_change(timing, g_vo_hdmi_rgb_mode);
    sample_if_failure_return_void(ret);
    printf("hdmi_video_timing video_timing:%d\n", timing);

    return;
}

static td_s32 hdmi_color_mode(td_u32 color_index)
{
    td_s32 ret;
    ot_vo_hdmi_param hdmi_param = {0};

    printf("hdmi_color_mode color_index:%d\n", color_index);

    ret = ss_mpi_vo_get_hdmi_param(SAMPLE_VO_DEV_DHD0, &hdmi_param);
    sample_if_failure_return(ret, TD_FAILURE);
    ret = ss_mpi_hdmi_stop(OT_HDMI_ID_0);
    sample_if_failure_return(ret, TD_FAILURE);

    switch (color_index) {
        case NUM_1: /* ycbcr422 */
            hdmi_param.csc.csc_matrix = OT_VO_CSC_MATRIX_BT709FULL_TO_BT709FULL;
            ret = ss_mpi_vo_set_hdmi_param(SAMPLE_VO_DEV_DHD0, &hdmi_param);
            sample_if_failure_return(ret, TD_FAILURE);
            g_vo_hdmi_rgb_mode = TD_FALSE;
            system("echo outclrspace 1 > /proc/umap/hdmi0;");
            break;
        case NUM_2: /* ycbcr444 */
            hdmi_param.csc.csc_matrix = OT_VO_CSC_MATRIX_BT709FULL_TO_BT709FULL;
            ret = ss_mpi_vo_set_hdmi_param(SAMPLE_VO_DEV_DHD0, &hdmi_param);
            sample_if_failure_return(ret, TD_FAILURE);
            g_vo_hdmi_rgb_mode = TD_FALSE;
            system("echo outclrspace 2 > /proc/umap/hdmi0;");
            break;
        case NUM_3: /* ycbcr420 */
            hdmi_param.csc.csc_matrix = OT_VO_CSC_MATRIX_BT709FULL_TO_BT709FULL;
            ret = ss_mpi_vo_set_hdmi_param(SAMPLE_VO_DEV_DHD0, &hdmi_param);
            sample_if_failure_return(ret, TD_FAILURE);
            g_vo_hdmi_rgb_mode = TD_FALSE;
            system("echo outclrspace 3 > /proc/umap/hdmi0;");
            break;
        default: /* rgb */
            hdmi_param.csc.csc_matrix = OT_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT;
            ret = ss_mpi_vo_set_hdmi_param(SAMPLE_VO_DEV_DHD0, &hdmi_param);
            sample_if_failure_return(ret, TD_FAILURE);
            g_vo_hdmi_rgb_mode = TD_TRUE;
            break;
    }

    ret = ss_mpi_hdmi_start(OT_HDMI_ID_0);
    sample_if_failure_return(ret, TD_FAILURE);

    return ret;
}

static td_s32 hdmi_aspectratio(td_u32 aspectratio_index)
{
    td_s32 ret_error;
    ot_hdmi_infoframe info_frame = {0};
    ot_hdmi_avi_infoframe *vid_infoframe = TD_NULL;

    ret_error = ss_mpi_hdmi_stop(g_test_hdmi_id);
    sample_if_failure_return(ret_error, TD_FAILURE);
    ret_error = ss_mpi_hdmi_get_infoframe(g_test_hdmi_id, OT_INFOFRAME_TYPE_AVI, &info_frame);
    sample_if_failure_return(ret_error, TD_FAILURE);

    vid_infoframe = &(info_frame.infoframe_unit.avi_infoframe);

    switch (aspectratio_index) {
        case NUM_0:
            printf("4:3\n");
            vid_infoframe->aspect_ratio = OT_HDMI_PIC_ASPECT_RATIO_4TO3;
            vid_infoframe->active_aspect_ratio = OT_HDMI_ACTIVE_ASPECT_RATIO_SAME_PIC;
            break;
        case NUM_1:
            printf("16:9\n");
            vid_infoframe->aspect_ratio = OT_HDMI_PIC_ASPECT_RATIO_16TO9;
            vid_infoframe->active_aspect_ratio = OT_HDMI_ACTIVE_ASPECT_RATIO_SAME_PIC;
            break;
        default:
            printf("16:9\n");
            vid_infoframe->aspect_ratio = OT_HDMI_PIC_ASPECT_RATIO_16TO9;
            vid_infoframe->active_aspect_ratio = OT_HDMI_ACTIVE_ASPECT_RATIO_SAME_PIC;
            break;
    }
    ret_error = ss_mpi_hdmi_set_infoframe(g_test_hdmi_id, &info_frame);
    sample_if_failure_return(ret_error, TD_FAILURE);
    ret_error = ss_mpi_hdmi_start(g_test_hdmi_id);
    sample_if_failure_return(ret_error, TD_FAILURE);

    return ret_error;
}

static td_void hdmi_authmode(td_bool bauthmode_enable)
{
    td_s32 ret;
    ot_hdmi_attr attr = {0};

    ret = ss_mpi_hdmi_stop(g_test_hdmi_id);
    sample_if_failure_return_void(ret);
    ret = ss_mpi_hdmi_get_attr(g_test_hdmi_id, &attr);
    sample_if_failure_return_void(ret);
    attr.auth_mode_en = bauthmode_enable;
    ret = ss_mpi_hdmi_set_attr(g_test_hdmi_id, &attr);
    sample_if_failure_return_void(ret);
    ret = ss_mpi_hdmi_start(g_test_hdmi_id);
    sample_if_failure_return_void(ret);

    return;
}

static td_u32 hdmi_freq(td_u32 audio_freq_index)
{
    td_s32 ret;
    td_u32 sample_rate = 0;

    switch (audio_freq_index) {
        case NUM_0:
            sample_rate = OT_HDMI_SAMPLE_RATE_32K;
            break;
        case NUM_1:
            sample_rate = OT_HDMI_SAMPLE_RATE_44K;
            break;
        case NUM_2:
            sample_rate = OT_HDMI_SAMPLE_RATE_48K;
            break;
        default:
            printf("wrong input audio_freq index:%d!\n", audio_freq_index);
            return TD_FAILURE;
    }

    ret = sample_hdmi_set_audio_sample_rate(sample_rate);
    sample_if_failure_return(ret, TD_FAILURE);

    return TD_SUCCESS;
}

static td_void print_help_info(td_void)
{
    printf("\t help                 list all command we provide\n");
    printf("\t q                    quit sample test\n");
    printf("\t hdmi_hdmi_force      force to hdmi output\n");
    printf("\t hdmi_dvi_force       force to enter dvi output mode\n");
    printf("\t hdmi_deepcolor       set video deepcolor mode\n");
    printf("\t hdmi_video_timing    set video output timing format\n");
    printf("\t hdmi_color_mode      set video color output(RGB/ycbcr)\n");
    printf("\t hdmi_aspectratio     set video aspectratio\n");
    printf("\t hdmi_a_freq          set audio output frequence\n");
    printf("\t hdmi_authmode        authmode enable or disable\n");

    return;
}

static td_s32 hdmi_deep_color(td_u32 argc)
{
    td_s32 ret_error;
    td_u32 deep_color_index;

    if (argc <= 1) {
        printf("usage:hdmi_deepcolor deep_colorflag\n");
        printf("deep_colorflag: 0:deep color 24bit\n");
        printf("deep_colorflag: 1:deep color 30bit\n");
        printf("deep_colorflag: 2:deep color 36bit\n");
        return TD_FAILURE;
    }
    deep_color_index = hdmi_string_to_integer(g_args[1]);
    if (deep_color_index >= OT_HDMI_DEEP_COLOR_BUTT) {
        printf("Invalid input parameters: %s\n", g_args[1]);
        printf("usage:hdmi_deepcolor deep_colorflag\n");
        printf("deep_colorflag: 0:deep color 24bit\n");
        printf("deep_colorflag: 1:deep color 30bit\n");
        printf("deep_colorflag: 2:deep color 36bit\n");
        return TD_FAILURE;
    }
    ret_error = hdmi_deepcolor(deep_color_index);
    sample_if_failure_return(ret_error, TD_FAILURE);

    return ret_error;
}

static td_s32 hdmi_videotiming(td_u32 argc)
{
    td_u32 index;
    td_u32 timing_index;
    td_u32 vo_timing;
    td_u32 len = (sizeof(g_hdmi_timing) / sizeof(hdmi_input_param)) - 1;

    if (argc < NUM_2) {
        printf("usage:hdmi_video_timing timeingmode\n");
        for (index = 0; index < len; index++) {
            printf("timemode:%02d   %s\n", index, g_hdmi_timing[index].index_string);
        }
        return TD_FAILURE;
    }
    timing_index = hdmi_string_to_integer(g_args[1]);
    if (timing_index > (len - 1)) {
        printf("Invalid input parameters: %s\n", g_args[1]);
        printf("usage:hdmi_video_timing timeingmode\n");
        for (index = 0; index < len; index++) {
            printf("timemode:%02d   %s\n", index, g_hdmi_timing[index].index_string);
        }
        return TD_FAILURE;
    }

    vo_timing = g_hdmi_timing[timing_index].index;
    hdmi_video_timing(timing_index, vo_timing);

    return TD_SUCCESS;
}

static td_s32 hdmi_colormode(td_u32 argc)
{
    td_s32 ret_error;
    td_u8 index;
    td_u8 arry_size;
    td_u32 color_index;

    arry_size = sizeof(g_hdmi_v_color_mode) / sizeof(g_hdmi_v_color_mode[0]);

    if (argc <= 1) {
        printf("usage:hdmi_color_mode colormode\n");
        for (index = 0; index < arry_size; index++) {
            printf("colormode:%02d   %s\n", index, g_hdmi_v_color_mode[index].index_string);
        }
        return TD_FAILURE;
    }
    color_index = hdmi_string_to_integer(g_args[1]);
    if (color_index >= arry_size) {
        printf("Invalid input parameters: %s\n", g_args[1]);
        printf("usage:hdmi_color_mode colormode\n");
        for (index = 0; index < arry_size; index++) {
            printf("colormode:%02d   %s\n", index, g_hdmi_v_color_mode[index].index_string);
        }
        return TD_FAILURE;
    }
    ret_error = hdmi_color_mode(color_index);
    sample_if_failure_return(ret_error, TD_FAILURE);

    return ret_error;
}

static td_s32 hdmi_aspect_ratio(td_u32 argc)
{
    td_s32 ret_error;
    td_u32 aspectratio_index;
    td_u8 index;

    if (argc <= 1) {
        printf("usage:hdmi_aspectratio aspectratio\n");
        for (index = 0; index < HDMI_MAX_ASPECTRATIO; index++) {
            printf("apectratio_index:%02d   %s\n", g_hdmi_v_aspect_ratio[index].index,
                   g_hdmi_v_aspect_ratio[index].index_string);
        }
        return TD_FAILURE;
    }
    aspectratio_index = hdmi_string_to_integer(g_args[1]);
    if ((aspectratio_index >= HDMI_MAX_ASPECTRATIO)) {
        printf("Invalid input parameters: %s\n", g_args[1]);
        printf("usage:hdmi_aspectratio aspectratio\n");
        for (index = 0; index < HDMI_MAX_ASPECTRATIO; index++) {
            printf("apectratio_index:%02d   %s\n", g_hdmi_v_aspect_ratio[index].index,
                g_hdmi_v_aspect_ratio[index].index_string);
        }
        return TD_FAILURE;
    }
    ret_error = hdmi_aspectratio(aspectratio_index);
    sample_if_failure_return(ret_error, TD_FAILURE);

    return ret_error;
}

static td_s32 hdmi_auth_mode(td_u32 argc)
{
    td_u32 authmode;

    if (argc <= 1) {
        printf("usage:hdmi_authmode mode\n");
        printf("hdmi_authmode: 0  disable authmode\n");
        printf("hdmi_authmode: 1  enable authmode\n");
        return TD_FAILURE;
    }

    authmode = hdmi_string_to_integer(g_args[1]);
    if ((authmode >= HDMI_MAX_AUTHMODE)) {
        printf("Invalid input parameters: %s\n", g_args[1]);
        printf("usage:hdmi_authmode mode\n");
        printf("hdmi_authmode: 0  disable authmode\n");
        printf("hdmi_authmode: 1  enable authmode\n");
        return TD_FAILURE;
    }
    hdmi_authmode(authmode);

    return TD_SUCCESS;
}

static td_s32 hdmi_audio_freq(td_u32 argc)
{
    td_s32 ret_error;
    td_u32 a_freq_index;
    td_u8 index;

    if (argc <= 1) {
        for (index = 0; index < HDMI_MAX_AUDIO_FREQNCE; index++) {
            printf("audio freq:%02d   %s\n", index, g_hdmi_a_freq[index].index_string);
        }
        return TD_FAILURE;
    }
    a_freq_index = hdmi_string_to_integer(g_args[1]);
    if (a_freq_index >= HDMI_MAX_AUDIO_FREQNCE) {
        printf("Invalid input parameters: %s\n", g_args[1]);
        printf("usage:hdmi_audio_freq mode\n");
        for (index = 0; index < HDMI_MAX_AUDIO_FREQNCE; index++) {
            printf("audio freq:%02d   %s\n", index, g_hdmi_a_freq[index].index_string);
        }
        return TD_FAILURE;
    }
    ret_error = hdmi_freq(a_freq_index);
    sample_if_failure_return(ret_error, TD_FAILURE);

    return ret_error;
}

static td_s32 hdmi_parse_arg(td_u32 argc)
{
    td_u32 i;
    td_s32 ret_error = TD_SUCCESS;

    printf("input parameter num:%d ", argc);
    for (i = 0; i < argc; i++) {
        printf("argv[%d]:%s, ", i, g_args[i]);
    }
    printf("\n");

    if ((strcmp("help", (char *)g_args[0]) == 0) || (strcmp("h", (char *)g_args[0]) == 0)) {
        print_help_info();
    } else if (strcmp("hdmi_hdmi_force", (char *)g_args[0]) == 0) {
        hdmi_hdmi_force();
    } else if (strcmp("hdmi_dvi_force", (char *)g_args[0]) == 0) {
        hdmi_dvi_force();
    } else if (strcmp("hdmi_deepcolor", (char *)g_args[0]) == 0) {
        ret_error = hdmi_deep_color(argc);
    } else if (strcmp("hdmi_video_timing", (char *)g_args[0]) == 0) {
        ret_error = hdmi_videotiming(argc);
    } else if (strcmp("hdmi_color_mode", (char *)g_args[0]) == 0) {
        ret_error = hdmi_colormode(argc);
    } else if (strcmp("hdmi_aspectratio", (char *)g_args[0]) == 0) {
        ret_error = hdmi_aspect_ratio(argc);
    } else if (strcmp("hdmi_authmode", (char *)g_args[0]) == 0) {
        ret_error = hdmi_auth_mode(argc);
    } else if (strcmp("hdmi_a_freq", (char *)g_args[0]) == 0) {
        ret_error = hdmi_audio_freq(argc);
    }

    return ret_error;
}

td_s32 hdmi_test_cmd(td_char *string, td_u32 len)
{
    td_u8 argc;

    if (string == NULL) {
        return TD_FAILURE;
    }
    (td_void)memset_s(g_args, sizeof(g_args), 0, sizeof(g_args));
    printf("string %s\n", string);
    argc = hdmi_adjust_string(string, len);
    if (argc == 0) {
        return TD_FAILURE;
    }

    if ((strcmp("q", (char *)g_args[0]) == 0) || (strcmp("Q", (char *)g_args[0]) == 0) ||
        (strcmp("quit", (char *)g_args[0]) == 0) || (strcmp("QUIT", (char *)g_args[0]) == 0)) {
        printf("quit the program, use extran interface to quit\n");
        return TD_FAILURE;
    }

    return hdmi_parse_arg(argc);
}

