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
#include "drv_hdmi_compatibility.h"
#include "hdmi_osal.h"

/* Constants for reported magic-number values. */
#define HDMI_COMPAT_MFRS_NAME_INDEX_2          2
#define HDMI_COMPAT_MFRS_NAME_INDEX_3          3

#define DEF_FILE_NAMELENGTH 32

typedef struct {
    td_char mfrs_name[4];
    td_u32  product_code;
    td_u32  serial_number;
    td_u32  week;
    td_u32  year;
} compat_edid_manufacture_info;

typedef struct {
    compat_edid_manufacture_info edid;
    td_u64 ms_under340_m;
    td_u64 ms_over340_m;
} compat_hdcp_ms;

typedef struct {
    compat_edid_manufacture_info edid;
    hdmi_video_timing timing;
    hdmi_avmute_cfg avmute_cfg;
} compat_sink_avmute;

typedef struct {
    compat_edid_manufacture_info edid;
    hdmi_video_timing timing;
    td_u32 fmt_delay;
    td_u32 mute_delay;
} compat_sink_delay;

typedef struct {
    compat_edid_manufacture_info edid;
    td_u32 stop_delay;
} compat_sink_stop_delay;

static compatibility_info g_compat_info[HDMI_ID_MAX];

static compat_sink_avmute g_sink_avmute[] = {
    /*   name  prod_code serial_no week year           timing      mute_pkg_en set(no use) mute_clr rpt rpt_cnt */
    /* default config for all other TV have no problem. */
    {{ "DEF",     0,        0,   0,    0 }, HDMI_VIDEO_TIMING_BUTT, { TD_TRUE,  TD_TRUE,  TD_TRUE,  TD_TRUE, 17 }},
    /* skyworth LED49EC520UA */
    {{ "HEC",    48,        1,  35, 2010 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* samsung UA48JU5900CXXZ */
    {{ "SAM",  3140, 16777217,  42, 2014 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* sharp LCD-60TX85A */
    {{ "SHP",  4452,        0, 255, 2016 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    {{ "MST",    48,        1,   0, 2007 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    {{ "GSM",     1, 16843009,   1, 2016 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* changhong LT19629 */
    {{ "CHD", 36897,        1,  32, 2008 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* hisense LED55EC680US */
    {{ "HEC",    48,        1,  28, 2016 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* SHARP LCD-60DS7008A */
    {{ "SHP", 17446,        0, 255, 2015 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* skyworth 43M9 */
    {{ "SKY",     1, 16843009,  34, 2017 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* hair 40DH6000 skyworth 166 13F-B 127 */
    {{ "HRE",  1691,        1,  51, 2010 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* skyworth 50M5 */
    {{ "SKY",     1, 16843009,   2, 2016 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* GSM 22EA63VB */
    {{ "GSM", 22928,   524660,   9, 2013 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* GSM 23EA63VB */
    {{ "GSM", 22925,   181207,  10, 2013 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }},
    /* UA65MUF30EJXXZ */
    {{ "SAM", 3574,  16780800,   1, 2017 }, HDMI_VIDEO_TIMING_BUTT, { TD_FALSE, TD_FALSE, TD_FALSE, TD_FALSE, 0 }}
};

static compat_sink_delay g_sink_delay[] = {
    /* name   prod_code serial_no week year       timing           fmt mute */
    {{ "SKW",    48,         1,  1, 2008 }, HDMI_VIDEO_TIMING_BUTT, 500, 900}, /* SKY 24E60HR */
    {{ "SKY",    48,         1, 10, 2010 }, HDMI_VIDEO_TIMING_BUTT, 200,   0},
    {{ "TCL",     0,         0, 20, 2013 }, HDMI_VIDEO_TIMING_BUTT, 500, 500},
    {{ "DEL", 41124, 894580556,  8, 2016 }, HDMI_VIDEO_TIMING_BUTT, 500, 500}, /* DEL U2414HB */
    {{ "DWE",   533, 825241648,  7, 2019 }, HDMI_VIDEO_TIMING_BUTT, 600, 500},
    {{ "ZDH",  8950,  16843009,  0, 2015 }, HDMI_VIDEO_TIMING_BUTT, 500, 500},
    {{ "XXX",  0,  0,  9, 2020 }, HDMI_VIDEO_TIMING_BUTT, 500, 0} /* skyworth */
};

static compat_sink_stop_delay g_sink_stop_delay[] = {
    /* name prod_code serial_no week year stop_delay */
    {{ "SKW", 48,        1, 1, 2008 }, 1000},
    {{ "GSM",  1, 16843009, 1, 2011 }, 1000},
    {{ "LTV", 48,        1, 6, 2014 }, 1000} // le_tv x50air
};

static compat_edid_manufacture_info g_avi_use_gen5[] = {
    /* name  prod_code serial_no week  year */
    {"SHP",   4485,     0,       255, 2018}, /* Sharp */
    {"SHP",   4488,     0,       255, 2018}, /* Sharp */
    {"SHP",   4491,     0,       255, 2018}, /* Sharp */
    {"SHP",   4494,     0,       255, 2018}, /* Sharp */
    {"SHP",   4506,     0,       255, 2018}, /* Sharp */
    {"SHP",   4507,     0,       255, 2018}, /* Sharp */
    {"SHP",   9301,     0,       255, 2018}, /* Sharp */
    {"SHP",   9304,     0,       255, 2018}, /* Sharp */
    {"ATO",  45098,     0,        13, 2019}  /* Astro */
};

static td_bool compat_edid_match_chk(const compat_edid_manufacture_info *tmp_manu_info,
    const hdmi_edid_manufacture_info *sink_manu_info)
{
    td_bool match;

    match = (tmp_manu_info->mfrs_name[0] == sink_manu_info->mfrs_name[0]) &&
            (tmp_manu_info->mfrs_name[1] == sink_manu_info->mfrs_name[1]) &&
            (tmp_manu_info->mfrs_name[HDMI_COMPAT_MFRS_NAME_INDEX_2] ==
            sink_manu_info->mfrs_name[HDMI_COMPAT_MFRS_NAME_INDEX_2]) &&
            (tmp_manu_info->mfrs_name[HDMI_COMPAT_MFRS_NAME_INDEX_3] ==
            sink_manu_info->mfrs_name[HDMI_COMPAT_MFRS_NAME_INDEX_3]) &&
            (tmp_manu_info->product_code == sink_manu_info->product_code) &&
            (tmp_manu_info->serial_number == sink_manu_info->serial_number) &&
            (tmp_manu_info->week == sink_manu_info->week) &&
            (tmp_manu_info->year == sink_manu_info->year);

    return match;
}

static td_bool compat_avi_use_gen5(const hdmi_sink_capability *sink_caps)
{
    td_u32 i;
    compat_edid_manufacture_info *tmp_manuinfo = TD_NULL;

    hdmi_if_null_return(sink_caps, TD_FALSE);

    for (i = 0, tmp_manuinfo = &g_avi_use_gen5[0];
         i < hdmi_array_size(g_avi_use_gen5);
         tmp_manuinfo++, i++) {
        if (compat_edid_match_chk(tmp_manuinfo, &sink_caps->mfrs_info) ||
            ((sink_caps->mfrs_info.mfrs_name[0] == 'S') &&
            (sink_caps->mfrs_info.mfrs_name[1] == 'H') &&
            (sink_caps->mfrs_info.mfrs_name[HDMI_COMPAT_MFRS_NAME_INDEX_2] == 'P') &&
            (tmp_manuinfo->product_code == sink_caps->mfrs_info.product_code))) {
            return TD_TRUE;
        }
    }

    return TD_FALSE;
}

compatibility_info *compat_info_get(hdmi_device_id hdmi_id)
{
    if ((td_s32)hdmi_id < HDMI_ID_MAX) {
        return &g_compat_info[hdmi_id];
    }

    return TD_NULL;
}

td_s32 drv_hdmi_compat_avmute_get(hdmi_device_id hdmi_id, td_bool avmute, hdmi_avmute_cfg *avmute_cfg)
{
    td_u32 i;
    hdmi_sink_capability         *sink_cap       = TD_NULL;
    compat_sink_avmute           *sink_avmute    = TD_NULL;
    compat_edid_manufacture_info *tmp_manu_info  = TD_NULL;
    hdmi_edid_manufacture_info   *sink_manu_info = TD_NULL;
    hdmi_device                  *hdmi_dev       = TD_NULL;
    compatibility_info           *compat_info    = TD_NULL;

    hdmi_if_null_return(avmute_cfg, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    compat_info = compat_info_get(hdmi_dev->hdmi_dev_id);
    hdmi_if_null_return(compat_info, OT_ERR_HDMI_NULL_PTR);

    /* default value */
    sink_avmute = &g_sink_avmute[0];
    avmute_cfg->mute_pkg_en = sink_avmute->avmute_cfg.mute_pkg_en;
    avmute_cfg->mute_set    = avmute;
    avmute_cfg->mute_clr    = (!avmute) && sink_avmute->avmute_cfg.mute_clr;
    avmute_cfg->mute_rpt_en = (!avmute);
    avmute_cfg->rpt_cnt     = sink_avmute->avmute_cfg.rpt_cnt;

    /* debug value */
    if (compat_info->setmute_send == TD_TRUE) {
        (td_void)memset_s(avmute_cfg, sizeof(hdmi_avmute_cfg), 0, sizeof(hdmi_avmute_cfg));
        hdmi_info("debug config, not send avmute.\n");
        return TD_SUCCESS;
    }

    if (drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_cap) == HDMI_EDID_DATA_INVALID) {
        hdmi_warn("get sink capability fail\n");
        return TD_FAILURE;
    }

    sink_manu_info = &sink_cap->mfrs_info;

    for (i = 1, sink_avmute = &g_sink_avmute[1];
         i < hdmi_array_size(g_sink_avmute);
         sink_avmute++, i++) {
        tmp_manu_info = &sink_avmute->edid;

        if (compat_edid_match_chk(tmp_manu_info, sink_manu_info) == TD_FALSE) {
            continue;
        }
        if ((hdmi_dev->attr.vo_attr.video_timing == sink_avmute->timing) ||
            (sink_avmute->timing == HDMI_VIDEO_TIMING_BUTT)) {
            /* HDMI_VIDEO_TIMING_BUTT means all timing use the same config to the TV. */
            avmute_cfg->mute_pkg_en = sink_avmute->avmute_cfg.mute_pkg_en;
            avmute_cfg->mute_set    = avmute;
            avmute_cfg->mute_clr    = (!avmute) && sink_avmute->avmute_cfg.mute_clr;
            avmute_cfg->mute_rpt_en = sink_avmute->avmute_cfg.mute_rpt_en;
            avmute_cfg->rpt_cnt     = sink_avmute->avmute_cfg.rpt_cnt;
            break;
        }
    }

    return TD_SUCCESS;
}

td_s32 drv_hdmi_compat_delay_get(hdmi_device_id hdmi_id, hdmi_delay *delay)
{
    td_u32 i;
    hdmi_sink_capability         *sink_cap       = TD_NULL;
    compat_sink_delay            *sink_delay     = TD_NULL;
    compat_edid_manufacture_info *tmp_manu_info  = TD_NULL;
    hdmi_edid_manufacture_info   *sink_manu_info = TD_NULL;
    hdmi_device                  *hdmi_dev       = TD_NULL;

    hdmi_if_null_return(delay, OT_ERR_HDMI_NULL_PTR);
    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    if (drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_cap) == HDMI_EDID_DATA_INVALID) {
        hdmi_warn("get sink capability fail\n");
        return TD_FAILURE;
    }
    sink_manu_info = &sink_cap->mfrs_info;
    for (i = 0, sink_delay = &g_sink_delay[0]; i < hdmi_array_size(g_sink_delay); sink_delay++, i++) {
        tmp_manu_info = &sink_delay->edid;
        if (compat_edid_match_chk(tmp_manu_info, sink_manu_info) == TD_FALSE) {
            continue;
        }
        if ((hdmi_dev->attr.vo_attr.video_timing == sink_delay->timing) ||
            (sink_delay->timing == HDMI_VIDEO_TIMING_BUTT)) {
            /* HDMI_VIDEO_TIMING_BUTT means all timing use the same config to the TV. */
            delay->fmt_delay = sink_delay->fmt_delay;
            delay->mute_delay = sink_delay->mute_delay;
            break;
        }
    }

    return TD_SUCCESS;
}

td_s32 drv_hdmi_compat_stop_delay(hdmi_device_id hdmi_id)
{
    td_u32 i;
    hdmi_sink_capability         *sink_cap        = TD_NULL;
    compat_sink_stop_delay       *sink_stop_delay = TD_NULL;
    compat_edid_manufacture_info *tmp_manu_info   = TD_NULL;
    hdmi_edid_manufacture_info   *sink_manu_info  = TD_NULL;
    hdmi_device                  *hdmi_dev        = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);

    if (drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_cap) == HDMI_EDID_DATA_INVALID) {
        hdmi_warn("get sink capability fail\n");
        return TD_FAILURE;
    } else {
        sink_manu_info = &sink_cap->mfrs_info;

        for (i = 0, sink_stop_delay = &g_sink_stop_delay[0];
             i < hdmi_array_size(g_sink_stop_delay);
             sink_stop_delay++, i++) {
            tmp_manu_info = &sink_stop_delay->edid;

            if (compat_edid_match_chk(tmp_manu_info, sink_manu_info)) {
                hdmi_info("set stop delay = %ums.\n", sink_stop_delay->stop_delay);
                osal_msleep(sink_stop_delay->stop_delay);
                break;
            }
        }
    }

    return TD_SUCCESS;
}

td_void compatibility_info_default_set(hdmi_device_id hdmi_id)
{
    compatibility_info *compat_info = TD_NULL;

    compat_info = compat_info_get(hdmi_id);
    hdmi_if_null_return_void(compat_info);

    compat_info->setmute_send = TD_FALSE;
    compat_info->avi_use_gen5 = TD_FALSE;
    compat_info->avi_use_gen5_debug = TD_FALSE;
    compat_info->frl_compress_mode = FRL_CTRL_TYPE_COMPRESS_HW;

    return;
}

td_s32 compatibility_info_update(hdmi_device_id hdmi_id)
{
    hdmi_device          *hdmi_dev    = TD_NULL;
    hdmi_sink_capability *sink_caps   = TD_NULL;
    compatibility_info   *compat_info = TD_NULL;

    hdmi_dev = get_hdmi_device(hdmi_id);
    hdmi_if_null_return(hdmi_dev, OT_ERR_HDMI_NULL_PTR);
    compat_info = compat_info_get(hdmi_id);
    hdmi_if_null_return(compat_info, OT_ERR_HDMI_NULL_PTR);

    if (drv_hdmi_edid_capability_get(&hdmi_dev->edid_info, &sink_caps) == HDMI_EDID_DATA_INVALID) {
        hdmi_warn("get sink capability failed.\n");
        compatibility_info_default_set(hdmi_id);
        return TD_FAILURE;
    } else {
        if (compat_avi_use_gen5(sink_caps)) {
            if (compat_info->avi_use_gen5_debug == TD_FALSE) {
                hdmi_info("use gen5 send avi.\n");
                compat_info->avi_use_gen5 = TD_TRUE;
            }
            if (!(compat_info->frl_compress_mode & HDMI_FRL_COMPRESS_DEBUG_MASK)) {
                hdmi_info("frl not compress.\n");
                compat_info->frl_compress_mode = FRL_CTRL_TYPE_COMPRESS_NON;
            }
        } else {
            compat_info->avi_use_gen5 = TD_FALSE;
            compat_info->frl_compress_mode = FRL_CTRL_TYPE_COMPRESS_HW;
        }
    }

    return TD_SUCCESS;
}

