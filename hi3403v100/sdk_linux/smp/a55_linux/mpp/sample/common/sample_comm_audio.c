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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <sys/select.h>
#include "sample_comm.h"
#include "audio_aac_adp.h"
#include "ot_osal_user.h"
#ifdef OT_ACODEC_TYPE_INNER
#include "ot_acodec.h"
#endif
#ifdef OT_ACODEC_TYPE_ES8388
#include "es8388.h"
#endif
#ifdef OT_ACODEC_TYPE_NVP6158
#include "common.h"
#endif
#ifdef OT_ACODEC_TYPE_TP2828
#include "tp2802.h"
#endif

#define AUDIO_ADPCM_TYPE OT_ADPCM_TYPE_DVI4 /* ADPCM_TYPE_IMA, ADPCM_TYPE_DVI4 */
#define G726_BPS OT_MEDIA_G726_40K          /* MEDIA_G726_16K, MEDIA_G726_24K ... */

#define AUDIO_MICIN_GAIN_OPEN 0             /* should be 1 when micin */

static ot_aac_type g_aac_type = OT_AAC_TYPE_AACLC;
static ot_aac_bps g_aac_bps = OT_AAC_BPS_96K;
static ot_aac_transport_type g_aac_transport_type = OT_AAC_TRANSPORT_TYPE_ADTS;

typedef struct {
    td_bool start;
    pthread_t aenc_pid;
    td_s32 ae_chn;
    td_s32 ad_chn;
    FILE *fd;
    td_bool send_ad_chn;
} sample_aenc;

typedef struct {
    td_bool start;
    td_s32 ai_dev;
    td_s32 ai_chn;
    td_s32 aenc_chn;
    td_s32 ao_dev;
    td_s32 ao_chn;
    td_bool send_aenc;
    td_bool send_ao;
    pthread_t ai_pid;
} sample_ai;

typedef struct {
    td_bool start;
    td_s32 ad_chn;
    FILE *fd;
    pthread_t ad_pid;
} sample_adec;

typedef struct {
    ot_audio_dev ao_dev;
    td_bool start;
    pthread_t ao_pid;
} sample_ao;

static sample_ai g_sample_ai[OT_AI_DEV_MAX_NUM * OT_AI_MAX_CHN_NUM];
static sample_aenc g_sample_aenc[OT_AENC_MAX_CHN_NUM];
static sample_adec g_sample_adec[OT_ADEC_MAX_CHN_NUM];
static sample_ao g_sample_ao[OT_AO_DEV_MAX_NUM];

#ifdef OT_ACODEC_TYPE_ES8388
td_s32 sample_es8388_get_clk_ratio(ot_audio_sample_rate sample_rate, td_u32 *fsclk_ratio, td_u32 *bclk_ratio)
{
    switch (sample_rate) {
        case OT_AUDIO_SAMPLE_RATE_8000:
            *fsclk_ratio = ES8388_FSCLK_DIV_1024;
            *bclk_ratio = ES8388_BCLK_DIV_4;
            break;

        case OT_AUDIO_SAMPLE_RATE_12000:
            *fsclk_ratio = ES8388_FSCLK_DIV_1024;
            *bclk_ratio = ES8388_BCLK_DIV_4;
            break;

        case OT_AUDIO_SAMPLE_RATE_16000:
            *fsclk_ratio = ES8388_FSCLK_DIV_512;
            *bclk_ratio = ES8388_BCLK_DIV_4;
            break;

        case OT_AUDIO_SAMPLE_RATE_24000:
            *fsclk_ratio = ES8388_FSCLK_DIV_512;
            *bclk_ratio = ES8388_BCLK_DIV_4;
            break;

        case OT_AUDIO_SAMPLE_RATE_32000:
            *fsclk_ratio = ES8388_FSCLK_DIV_256;
            *bclk_ratio = ES8388_BCLK_DIV_4;
            break;

        case OT_AUDIO_SAMPLE_RATE_48000:
            *fsclk_ratio = ES8388_FSCLK_DIV_256;
            *bclk_ratio = ES8388_BCLK_DIV_4;
            break;

        default:
            printf("[Func]:%s [Line]:%d [Info]:not support sample_rate:%d.\n",
                __FUNCTION__, __LINE__, sample_rate);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_soft_reset(int codec_fd)
{
    td_s32 ret;
    ot_es8388_audio_ctrl audio_ctrl;
    audio_ctrl.chip_num = ES8388_CHIP_ID;

    ret = ioctl(codec_fd, OT_ES8388_SOFT_RESET, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es8388 reset failed");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_set_input_ctrl(int codec_fd)
{
    td_s32 ret;
    ot_es8388_audio_ctrl audio_ctrl;
    audio_ctrl.chip_num = ES8388_CHIP_ID;

    /* set input select */
    audio_ctrl.audio_in_select = ES8388_INPUT2; /* refer to hardware */
    ret = ioctl(codec_fd, OT_ES8388_SET_LEFT_INPUT_SELECT, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left input fail.");
        return ret;
    }
    audio_ctrl.audio_in_select = ES8388_INPUT2; /* refer to hardware */
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_INPUT_SELECT, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right input fail.");
        return ret;
    }

    /* power on input */
    audio_ctrl.if_powerup = ES8388_POWER_UP;
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_INPUT_POWER, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left in power fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_INPUT_POWER, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right in power fail.");
        return ret;
    }

    /* power on ADC */
    audio_ctrl.if_powerup = ES8388_POWER_UP;
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_ADC_POWER, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left ADC power fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_ADC_POWER, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right ADC power fail.");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_set_clk_mode(int codec_fd, ot_aio_mode work_mode, ot_audio_sample_rate sample_rate)
{
    td_s32 ret;
    td_u32 master_ctrl = ES8388_SLAVE_MODE;
    ot_es8388_audio_ctrl audio_ctrl;
    audio_ctrl.chip_num = ES8388_CHIP_ID;

    /* set master clk */
    if (work_mode == OT_AIO_MODE_I2S_SLAVE || work_mode == OT_AIO_MODE_PCM_SLAVE_STD ||
        work_mode == OT_AIO_MODE_PCM_SLAVE_NON_STD) {
        td_u32 fsclk_ratio;
        td_u32 bclk_ratio;
        master_ctrl = ES8388_MASTER_MODE;

        ret = sample_es8388_get_clk_ratio(sample_rate, &fsclk_ratio, &bclk_ratio);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        audio_ctrl.clk_div = bclk_ratio;
        ret = ioctl(codec_fd, OT_ES8388_SET_MCLK_BCLK_RATIO, &audio_ctrl);
        if (ret != TD_SUCCESS) {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set ADC LRCLK ratio fail.");
            return ret;
        }

        audio_ctrl.clk_div = fsclk_ratio;
        ret = ioctl(codec_fd, OT_ES8388_SET_ADC_MCLK_LRCLK_RATIO, &audio_ctrl);
        if (ret != TD_SUCCESS) {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set ADC LRCLK ratio fail.");
            return ret;
        }

        audio_ctrl.clk_div = fsclk_ratio;
        ret = ioctl(codec_fd, OT_ES8388_SET_DAC_MCLK_LRCLK_RATIO, &audio_ctrl);
        if (ret != TD_SUCCESS) {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set DAC LRCLK ratio fail.");
            return ret;
        }
    }

    /* set master/slave */
    audio_ctrl.ctrl_mode = master_ctrl;
    ret = ioctl(codec_fd, OT_ES8388_SET_MASTER_MODE, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set master/slave fail.");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_set_clk_polarity(int codec_fd, ot_aio_mode work_mode)
{
    td_s32 ret;
    td_u32 pcm_offset;
    td_u32 clk_dir;

    ot_es8388_audio_ctrl audio_ctrl;
    audio_ctrl.chip_num = ES8388_CHIP_ID;

    if (work_mode == OT_AIO_MODE_PCM_MASTER_STD || work_mode == OT_AIO_MODE_PCM_SLAVE_STD) {
        /* PCM offset is 1 */
        pcm_offset = ES8388_PCM_OFFSET_2ND;
        clk_dir = ES8388_BCLK_DIR_INVERT;
    } else if (work_mode == OT_AIO_MODE_PCM_MASTER_NON_STD || work_mode == OT_AIO_MODE_PCM_SLAVE_NON_STD) {
        /* PCM offset is 0 */
        pcm_offset = ES8388_PCM_OFFSET_1ST;
        clk_dir = ES8388_BCLK_DIR_INVERT;
    } else {
        /* I2S use normal polarity */
        pcm_offset = ES8388_I2S_POLARITY_NORMAL;
        clk_dir = ES8388_BCLK_DIR_NORMAL;
    }

    /* set PCM offset or I2S polarity */
    audio_ctrl.clk_polarity = pcm_offset;
    ret = ioctl(codec_fd, OT_ES8388_SET_ADC_POLARITY_AND_OFFSET, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set ADC polarity or offset fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_DAC_POLARITY_AND_OFFSET, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set DAC polarity or offset fail.");
        return ret;
    }

    /* set bclk dir */
    audio_ctrl.clk_polarity = clk_dir;
    ret = ioctl(codec_fd, OT_ES8388_SET_BCLK_DIR, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set bclk dir fail.");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_set_clk_ctrl(int codec_fd, ot_aio_mode work_mode, ot_audio_sample_rate sample_rate)
{
    td_s32 ret;

    ret = sample_es8388_set_clk_mode(codec_fd, work_mode, sample_rate);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    ret = sample_es8388_set_clk_polarity(codec_fd, work_mode);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_set_data_ctrl(int codec_fd, ot_aio_mode work_mode, ot_audio_bit_width bit_width)
{
    td_s32 ret;
    td_u32 format_ctrl;
    td_u32 width_ctrl;
    ot_es8388_audio_ctrl audio_ctrl;
    audio_ctrl.chip_num = ES8388_CHIP_ID;

    /* set data format */
    if (work_mode == OT_AIO_MODE_I2S_MASTER || work_mode == OT_AIO_MODE_I2S_SLAVE) {
        /* i2s */
        format_ctrl = ES8388_DATA_FORMAT_I2S;
    } else if (work_mode == OT_AIO_MODE_PCM_MASTER_STD || work_mode == OT_AIO_MODE_PCM_SLAVE_STD ||
               work_mode == OT_AIO_MODE_PCM_MASTER_NON_STD || work_mode == OT_AIO_MODE_PCM_SLAVE_NON_STD) {
        /* pcm */
        format_ctrl = ES8388_DATA_FORMAT_PCM;
    } else {
        printf("[Func]:%s [Line]:%d [Info]:not support work_mode:%d.\n",
            __FUNCTION__, __LINE__, work_mode);
        return TD_FAILURE;
    }
    audio_ctrl.data_format = format_ctrl;
    ret = ioctl(codec_fd, OT_ES8388_SET_ADC_DATA_FORMAT, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set ADC format fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_DAC_DATA_FORMAT, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set DAC format fail.");
        return ret;
    }

    /* set data width */
    if (bit_width == OT_AUDIO_BIT_WIDTH_16) {
        width_ctrl = ES8388_DATA_LENGTH_16BIT;
    } else if (bit_width == OT_AUDIO_BIT_WIDTH_24) {
        width_ctrl = ES8388_DATA_LENGTH_24BIT;
    } else {
        printf("[Func]:%s [Line]:%d [Info]:not support bit_width:%d.\n", __FUNCTION__, __LINE__, bit_width);
        return TD_FAILURE;
    }
    audio_ctrl.data_length = width_ctrl;
    ret = ioctl(codec_fd, OT_ES8388_SET_ADC_DATA_WIDTH, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set ADC width fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_DAC_DATA_WIDTH, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set DAC width fail.");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_set_output_ctrl(int codec_fd)
{
    td_s32 ret;
    ot_es8388_audio_ctrl audio_ctrl;
    audio_ctrl.chip_num = ES8388_CHIP_ID;

    /* power on DAC */
    audio_ctrl.if_powerup = ES8388_POWER_UP;
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_DAC_POWER, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left DAC power fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_DAC_POWER, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right DAC power fail.");
        return ret;
    }

    /* enable output */
    audio_ctrl.if_powerup = ES8388_OUT_ENABLE;
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_OUTPUT1_ENABLE, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left out1 enable fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_OUTPUT1_ENABLE, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right out1 enable fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_OUTPUT2_ENABLE, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left out2 enable fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_OUTPUT2_ENABLE, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right out2 enable fail.");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_set_input_volume(int codec_fd)
{
    td_s32 ret;
    ot_es8388_audio_ctrl audio_ctrl;
    audio_ctrl.chip_num = ES8388_CHIP_ID;

    /* set PGA gain */
    audio_ctrl.volume = ES8388_PGA_GAIN_0DB;
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_INPUT_GAIN, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left gain fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_INPUT_GAIN, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right gain fail.");
        return ret;
    }

    /* set ADC volume : 0dB */
    audio_ctrl.volume = ES8388_ADC_VOLUME_0DB;
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_ADC_VOLUME, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left ADC vol fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_ADC_VOLUME, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right ADC vol fail.");
        return ret;
    }

    /* set ADC unmute */
    audio_ctrl.if_mute = ES8388_MUTE_DISABLE;
    ret = ioctl(codec_fd, OT_ES8388_SET_ADC_MUTE, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set ADC unmute fail.");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_set_output_volume(int codec_fd)
{
    td_s32 ret;
    ot_es8388_audio_ctrl audio_ctrl;
    audio_ctrl.chip_num = ES8388_CHIP_ID;

    /* set output volume : 0dB */
    audio_ctrl.volume = ES8388_OUTPUT_VOLUME_0DB;
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_OUTPUT1_VOLUME, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left out1 vol fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_OUTPUT1_VOLUME, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right out1 vol fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_OUTPUT2_VOLUME, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left out2 vol fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_OUTPUT2_VOLUME, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right out2 vol fail.");
        return ret;
    }

    /* set DAC volume : 0dB */
    audio_ctrl.volume = ES8388_DAC_VOLUME_0DB;
    ret = ioctl(codec_fd, OT_ES8388_SET_LFET_DAC_VOLUME, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set left DAC vol fail.");
        return ret;
    }
    ret = ioctl(codec_fd, OT_ES8388_SET_RIGHT_DAC_VOLUME, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set right DAC vol fail.");
        return ret;
    }

    /* set DAC unmute */
    audio_ctrl.if_mute = ES8388_MUTE_DISABLE;
    ret = ioctl(codec_fd, OT_ES8388_SET_DAC_MUTE, &audio_ctrl);
    if (ret != TD_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "es83888 set DAC unmute fail.");
        return ret;
    }

    return TD_SUCCESS;
}

td_s32 sample_es8388_cfg_audio(ot_aio_mode work_mode, ot_audio_sample_rate sample_rate, ot_audio_bit_width bit_width)
{
    td_s32 ret;
    int es8388_fd = -1;

    es8388_fd = open(ES8388_FILE, O_RDWR);
    if (es8388_fd < 0) {
        printf("can't open es8388(%s).\n", ES8388_FILE);
        return TD_FAILURE;
    }

    /* soft reset */
    ret = sample_es8388_soft_reset(es8388_fd);
    if (ret != TD_SUCCESS) {
        goto es8388_err;
    }

    /* set input ctrl */
    ret = sample_es8388_set_input_ctrl(es8388_fd);
    if (ret != TD_SUCCESS) {
        goto es8388_err;
    }

    /* set clk */
    ret = sample_es8388_set_clk_ctrl(es8388_fd, work_mode, sample_rate);
    if (ret != TD_SUCCESS) {
        goto es8388_err;
    }

    /* set data */
    ret = sample_es8388_set_data_ctrl(es8388_fd, work_mode, bit_width);
    if (ret != TD_SUCCESS) {
        goto es8388_err;
    }

    /* set output ctrl */
    ret = sample_es8388_set_output_ctrl(es8388_fd);
    if (ret != TD_SUCCESS) {
        goto es8388_err;
    }

    /* set input volume */
    ret = sample_es8388_set_input_volume(es8388_fd);
    if (ret != TD_SUCCESS) {
        goto es8388_err;
    }

    /* set output volume */
    ret = sample_es8388_set_output_volume(es8388_fd);
    if (ret != TD_SUCCESS) {
        goto es8388_err;
    }

    close(es8388_fd);
    printf("Set es8388 ok: work_mode = %d, sample_rate = %d, bit_width = %d\n", work_mode, sample_rate, bit_width);
    return TD_SUCCESS;

es8388_err:
    close(es8388_fd);
    return ret;
}

td_s32 sample_es8388_disable(td_void)
{
    td_s32 ret;
    int es8388_fd = -1;

    es8388_fd = open(ES8388_FILE, O_RDWR);
    if (es8388_fd < 0) {
        printf("can't open es8388(%s).\n", ES8388_FILE);
        return TD_FAILURE;
    }

    /* soft reset */
    ret = sample_es8388_soft_reset(es8388_fd);

    close(es8388_fd);

    return ret;
}
#endif

#ifdef OT_ACODEC_TYPE_INNER
static td_s32 inner_codec_get_i2s_fs(ot_audio_sample_rate sample_rate, ot_acodec_fs *i2s_fs)
{
    ot_acodec_fs i2s_fs_sel;

    switch (sample_rate) {
        case OT_AUDIO_SAMPLE_RATE_8000:
            i2s_fs_sel = OT_ACODEC_FS_8000;
            break;

        case OT_AUDIO_SAMPLE_RATE_16000:
            i2s_fs_sel = OT_ACODEC_FS_16000;
            break;

        case OT_AUDIO_SAMPLE_RATE_32000:
            i2s_fs_sel = OT_ACODEC_FS_32000;
            break;

        case OT_AUDIO_SAMPLE_RATE_11025:
            i2s_fs_sel = OT_ACODEC_FS_11025;
            break;

        case OT_AUDIO_SAMPLE_RATE_22050:
            i2s_fs_sel = OT_ACODEC_FS_22050;
            break;

        case OT_AUDIO_SAMPLE_RATE_44100:
            i2s_fs_sel = OT_ACODEC_FS_44100;
            break;

        case OT_AUDIO_SAMPLE_RATE_12000:
            i2s_fs_sel = OT_ACODEC_FS_12000;
            break;

        case OT_AUDIO_SAMPLE_RATE_24000:
            i2s_fs_sel = OT_ACODEC_FS_24000;
            break;

        case OT_AUDIO_SAMPLE_RATE_48000:
            i2s_fs_sel = OT_ACODEC_FS_48000;
            break;

        case OT_AUDIO_SAMPLE_RATE_64000:
            i2s_fs_sel = OT_ACODEC_FS_64000;
            break;

        case OT_AUDIO_SAMPLE_RATE_96000:
            i2s_fs_sel = OT_ACODEC_FS_96000;
            break;

        default:
            printf("%s: not support sample_rate:%d\n", __FUNCTION__, sample_rate);
            return TD_FAILURE;
    }

    *i2s_fs = i2s_fs_sel;
    return TD_SUCCESS;
}

td_s32 sample_inner_codec_cfg_audio(ot_audio_sample_rate sample_rate)
{
    td_s32 ret;
    td_s32 fd_acodec = -1;
    ot_acodec_fs i2s_fs_sel;
    ot_acodec_mixer input_mode;

    fd_acodec = OSAL_OPEN(ACODEC_FILE, O_RDWR);
    if (fd_acodec < 0) {
        printf("%s: can't open audio codec,%s\n", __FUNCTION__, ACODEC_FILE);
        return TD_FAILURE;
    }

    ret = OSAL_IOCTL(fd_acodec, OT_ACODEC_SOFT_RESET_CTRL);
    if (ret != TD_SUCCESS) {
        printf("reset audio codec error\n");
        goto cfg_fail;
    }

    ret = inner_codec_get_i2s_fs(sample_rate, &i2s_fs_sel);
    if (ret != TD_SUCCESS) {
        goto cfg_fail;
    }

    ret = OSAL_IOCTL(fd_acodec, OT_ACODEC_SET_I2S1_FS, &i2s_fs_sel);
    if (ret != TD_SUCCESS) {
        printf("%s: set acodec sample rate failed\n", __FUNCTION__);
        goto cfg_fail;
    }

    /* refer to hardware, demo board is pseudo-differential (IN_D), socket board is single-ended (IN1) */
    input_mode = OT_ACODEC_MIXER_IN_D;
    ret = OSAL_IOCTL(fd_acodec, OT_ACODEC_SET_MIXER_MIC, &input_mode);
    if (ret != TD_SUCCESS) {
        printf("%s: select acodec input_mode failed\n", __FUNCTION__);
        goto cfg_fail;
    }

#if AUDIO_MICIN_GAIN_OPEN
    /*
     * The input volume range is [-78, 80]. Both the analog gain and digital gain are adjusted.
     * A larger value indicates higher volume.
     * For example, the value 80 indicates the maximum volume of 80 dB,
     * and the value -78 indicates the minimum volume (muted status).
     * The volume adjustment takes effect simultaneously in the audio-left and audio-right channels.
     * The recommended volume range is [20, 50].
     * Within this range, the noises are lowest because only the analog gain is adjusted,
     * and the voice quality can be guaranteed.
     */
    int acodec_input_vol;

    acodec_input_vol = 30; /* 30dB */
    ret = OSAL_IOCTL(fd_acodec,  OT_ACODEC_SET_INPUT_VOLUME, &acodec_input_vol);
    if (ret != TD_SUCCESS) {
        printf("%s: set acodec micin volume failed\n", __FUNCTION__);
        goto cfg_fail;
    }
#endif

    printf("set inner audio codec ok: sample_rate = %d.\n", sample_rate);

cfg_fail:
    OSAL_CLOSE(fd_acodec);
    return ret;
}
#endif

/* config codec */
td_s32 sample_comm_audio_cfg_acodec(const ot_aio_attr *aio_attr)
{
    td_bool codec_cfg = TD_FALSE;

#ifdef OT_ACODEC_TYPE_ES8388
    td_s32 ret = sample_es8388_cfg_audio(aio_attr->work_mode, aio_attr->sample_rate, aio_attr->bit_width);
    if (ret != TD_SUCCESS) {
        printf("%s: sample_es8388_cfg_audio failed\n", __FUNCTION__);
        return ret;
    }

    codec_cfg = TD_TRUE;
#endif

#ifdef OT_ACODEC_TYPE_INNER
    /* INNER AUDIO CODEC */
    td_s32 ret = sample_inner_codec_cfg_audio(aio_attr->sample_rate);
    if (ret != TD_SUCCESS) {
        printf("%s:sample_inner_codec_cfg_audio failed\n", __FUNCTION__);
        return ret;
    }
    codec_cfg = TD_TRUE;
#endif

    if (codec_cfg == TD_FALSE) {
        printf("can not find the right codec.\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 audio_ai_get_frame_and_send(sample_ai *ai_ctl)
{
    td_s32 ret;
    ot_audio_frame frame = {0};
    ot_aec_frame aec_frm = {0};

    /* get frame from ai chn */
    ret = ss_mpi_ai_get_frame(ai_ctl->ai_dev, ai_ctl->ai_chn, &frame, &aec_frm, TD_FALSE);
    if (ret != TD_SUCCESS) {
        /* continue */
        return TD_SUCCESS;
    }

    /* send frame to encoder */
    if (ai_ctl->send_aenc == TD_TRUE) {
        ret = ss_mpi_aenc_send_frame(ai_ctl->aenc_chn, &frame, &aec_frm);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_aenc_send_frame(%d), failed with %#x!\n", __FUNCTION__, ai_ctl->aenc_chn, ret);
            ss_mpi_ai_release_frame(ai_ctl->ai_dev, ai_ctl->ai_chn, &frame, &aec_frm);
            return TD_FAILURE;
        }
    }

    /* send frame to ao */
    if (ai_ctl->send_ao == TD_TRUE) {
        ret = ss_mpi_ao_send_frame(ai_ctl->ao_dev, ai_ctl->ao_chn, &frame, 1000); /* 1000:1000ms */
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_ao_send_frame(%d, %d), failed with %#x!\n", __FUNCTION__, ai_ctl->ao_dev, ai_ctl->ao_chn,
                ret);
            ss_mpi_ai_release_frame(ai_ctl->ai_dev, ai_ctl->ai_chn, &frame, &aec_frm);
            return TD_FAILURE;
        }
    }

    /* finally you must release the stream */
    ret = ss_mpi_ai_release_frame(ai_ctl->ai_dev, ai_ctl->ai_chn, &frame, &aec_frm);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_ai_release_frame(%d, %d), failed with %#x!\n", __FUNCTION__, ai_ctl->ai_dev, ai_ctl->ai_chn,
            ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* get frame from ai, send it to aenc or ao */
static td_s32 sample_comm_audio_ai_set_depth(sample_ai *ai_ctl)
{
    td_s32 ret;
    ot_ai_chn_param ai_chn_para;

    ret = ss_mpi_ai_get_chn_param(ai_ctl->ai_dev, ai_ctl->ai_chn, &ai_chn_para);
    if (ret != TD_SUCCESS) {
        printf("%s: get ai chn param failed\n", __FUNCTION__);
        return TD_FAILURE;
    }

    ai_chn_para.usr_frame_depth = 30; /* 30:usr frame depth */

    ret = ss_mpi_ai_set_chn_param(ai_ctl->ai_dev, ai_ctl->ai_chn, &ai_chn_para);
    if (ret != TD_SUCCESS) {
        printf("%s: set ai chn param failed\n", __FUNCTION__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

#ifndef CONFIG_USER_SPACE
static td_s32 sample_comm_audio_ai_wait_frame(td_s32 ai_fd, fd_set *read_fds)
{
    td_s32 ret;
    struct timeval timeout_val;

    timeout_val.tv_sec = 1;
    timeout_val.tv_usec = 0;
    FD_ZERO(read_fds);
    FD_SET(ai_fd, read_fds);
    ret = select(ai_fd + 1, read_fds, NULL, NULL, &timeout_val);
    if (ret < 0) {
        return TD_FAILURE;
    } else if (ret == 0) {
        printf("%s: get ai frame select time out\n", __FUNCTION__);
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}
#endif

static td_s32 sample_comm_audio_ai_prepare(sample_ai *ai_ctl, td_s32 *ai_fd, fd_set *read_fds)
{
    if (sample_comm_audio_ai_set_depth(ai_ctl) != TD_SUCCESS) {
        return TD_FAILURE;
    }

#ifndef CONFIG_USER_SPACE
    FD_ZERO(read_fds);
    *ai_fd = ss_mpi_ai_get_fd(ai_ctl->ai_dev, ai_ctl->ai_chn);
    if (*ai_fd < 0) {
        printf("%s: get ai fd failed\n", __FUNCTION__);
        return TD_FAILURE;
    }
    FD_SET(*ai_fd, read_fds);
#else
    ot_unused(ai_fd);
    ot_unused(read_fds);
#endif
    return TD_SUCCESS;
}

static td_s32 sample_comm_audio_ai_process_once(sample_ai *ai_ctl, td_s32 ai_fd, fd_set *read_fds)
{
#ifndef CONFIG_USER_SPACE
    td_s32 ret = sample_comm_audio_ai_wait_frame(ai_fd, read_fds);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (!FD_ISSET(ai_fd, read_fds)) {
        return TD_SUCCESS;
    }
#else
    ot_unused(ai_fd);
    ot_unused(read_fds);
#endif
    return audio_ai_get_frame_and_send(ai_ctl);
}

void *sample_comm_audio_ai_proc(void *parg)
{
    td_s32 ret;
    sample_ai *ai_ctl = (sample_ai *)parg;
    td_s32 ai_fd = -1;
    fd_set read_fds;

    if (sample_comm_audio_ai_prepare(ai_ctl, &ai_fd, &read_fds) != TD_SUCCESS) {
        return NULL;
    }

    while (ai_ctl->start) {
        ret = sample_comm_audio_ai_process_once(ai_ctl, ai_fd, &read_fds);
        if (ret != TD_SUCCESS) {
            break;
        }
    }

    ai_ctl->start = TD_FALSE;
    return NULL;
}

static td_s32 audio_aenc_get_stream_and_send(sample_aenc *aenc_ctl)
{
    td_s32 ret;
    ot_audio_stream stream = { 0 };

    /* get stream from aenc chn */
    ret = ss_mpi_aenc_get_stream(aenc_ctl->ae_chn, &stream, -1);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_aenc_get_stream(%d), failed with %#x!\n", __FUNCTION__, aenc_ctl->ae_chn, ret);
        return TD_FAILURE;
    }

    /* send stream to decoder and play for testing */
    if (aenc_ctl->send_ad_chn == TD_TRUE) {
        ret = ss_mpi_adec_send_stream(aenc_ctl->ad_chn, &stream, TD_TRUE);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_adec_send_stream(%d), failed with %#x!\n", __FUNCTION__, aenc_ctl->ad_chn, ret);
            ss_mpi_aenc_release_stream(aenc_ctl->ae_chn, &stream);
            return TD_FAILURE;
        }
    }

    /* save audio stream to file */
    if (fwrite(stream.stream, 1, stream.len, aenc_ctl->fd) != stream.len) {
        ss_mpi_aenc_release_stream(aenc_ctl->ae_chn, &stream);
        return TD_FAILURE;
    }
    if (fflush(aenc_ctl->fd) != 0) {
        ss_mpi_aenc_release_stream(aenc_ctl->ae_chn, &stream);
        return TD_FAILURE;
    }

    /* finally you must release the stream */
    ret = ss_mpi_aenc_release_stream(aenc_ctl->ae_chn, &stream);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_aenc_release_stream(%d), failed with %#x!\n", __FUNCTION__, aenc_ctl->ae_chn, ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* get stream from aenc, send it to adec & save it to file */
void *sample_comm_audio_aenc_proc(void *parg)
{
    td_s32 ret;
    sample_aenc *aenc_ctl = (sample_aenc *)parg;
#ifndef CONFIG_USER_SPACE
    td_s32 aenc_fd;
    fd_set read_fds;
    struct timeval timeout_val;

    FD_ZERO(&read_fds);
    aenc_fd = ss_mpi_aenc_get_fd(aenc_ctl->ae_chn);
    if (aenc_fd < 0) {
        printf("%s: get aenc fd failed\n", __FUNCTION__);
        goto get_fd_fail;
    }
    FD_SET(aenc_fd, &read_fds);
#endif

    while (aenc_ctl->start) {
#ifndef CONFIG_USER_SPACE
        timeout_val.tv_sec = 1;
        timeout_val.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(aenc_fd, &read_fds);

        ret = select(aenc_fd + 1, &read_fds, NULL, NULL, &timeout_val);
        if (ret < 0) {
            break;
        } else if (ret == 0) {
            printf("%s: get aenc stream select time out\n", __FUNCTION__);
            break;
        }

        if (FD_ISSET(aenc_fd, &read_fds)) {
#endif
            /* get stream from aenc chn, send and release */
            ret = audio_aenc_get_stream_and_send(aenc_ctl);
            if (ret != TD_SUCCESS) {
                break;
            }
#ifndef CONFIG_USER_SPACE
        }
#endif
    }

#ifndef CONFIG_USER_SPACE
get_fd_fail:
#endif
    if (fclose(aenc_ctl->fd) != 0) {
        printf("%s: fclose aenc file failed\n", __FUNCTION__);
    }
    aenc_ctl->fd = TD_NULL;
    aenc_ctl->start = TD_FALSE;
    return NULL;
}

/* get stream from file, and send it  to adec */
void *sample_comm_audio_adec_proc(void *parg)
{
    td_s32 ret;
    ot_audio_stream audio_stream;
    const td_u32 len = 640; /* 640: stream length */
    td_u32 read_len;
    td_s32 adec_chn;
    td_u8 *audio_stream_tmp = NULL;
    sample_adec *adec_ctl = (sample_adec *)parg;
    FILE *fd = adec_ctl->fd;
    adec_chn = adec_ctl->ad_chn;

    audio_stream_tmp = (td_u8 *)malloc(sizeof(td_u8) * OT_MAX_AUDIO_STREAM_LEN);
    if (audio_stream_tmp == NULL) {
        printf("%s: malloc failed!\n", __FUNCTION__);
        goto stream_malloc_fail;
    }

    while (adec_ctl->start == TD_TRUE) {
        /* read from file */
        audio_stream.stream = audio_stream_tmp;
        read_len = fread(audio_stream.stream, 1, len, fd);
        if (read_len <= 0) {
            ret = ss_mpi_adec_send_end_of_stream(adec_chn, TD_FALSE);
            if (ret != TD_SUCCESS) {
                printf("%s: ss_mpi_adec_send_end_of_stream failed!\n", __FUNCTION__);
            }
            (td_void)fseek(fd, 0, SEEK_SET); /* read file again */
            continue;
        }

        /* here only demo adec streaming sending mode, but pack sending mode is commended */
        audio_stream.len = read_len;
        ret = ss_mpi_adec_send_stream(adec_chn, &audio_stream, TD_TRUE);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_adec_send_stream(%d) failed with %#x!\n", __FUNCTION__, adec_chn, ret);
            break;
        }
    }

    free(audio_stream_tmp);
    audio_stream_tmp = NULL;

stream_malloc_fail:
    if (fclose(adec_ctl->fd) != 0) {
        printf("%s: fclose adec file failed\n", __FUNCTION__);
    }
    adec_ctl->fd = TD_NULL;
    adec_ctl->start = TD_FALSE;
    return NULL;
}

/* set ao volume */
void *sample_comm_audio_ao_vol_proc(void *parg)
{
    td_s32 ret;
    td_s32 volume;
    ot_audio_fade fade;
    sample_ao *ao_ctl = (sample_ao *)parg;
    ot_audio_dev ao_dev = ao_ctl->ao_dev;

    while (ao_ctl->start) {
        for (volume = 0; volume <= 6; volume++) { /* 0,6:test range */
            ret = ss_mpi_ao_set_volume(ao_dev, volume);
            if (ret != TD_SUCCESS) {
                printf("%s: ss_mpi_ao_set_volume(%d), failed with %#x!\n", __FUNCTION__, ao_dev, ret);
            }
            printf("\rset volume %d          ", volume);
            sleep(2); /* 2:2s */
        }

        for (volume = 5; volume >= -15; volume--) { /* -15,5:test range */
            ret = ss_mpi_ao_set_volume(ao_dev, volume);
            if (ret != TD_SUCCESS) {
                printf("%s: ss_mpi_ao_set_volume(%d), failed with %#x!\n", __FUNCTION__, ao_dev, ret);
            }
            printf("\rset volume %d          ", volume);
            sleep(2); /* 2:2s */
        }

        for (volume = -14; volume <= 0; volume++) { /* -14,0:test range */
            ret = ss_mpi_ao_set_volume(ao_dev, volume);
            if (ret != TD_SUCCESS) {
                printf("%s: ss_mpi_ao_set_volume(%d), failed with %#x!\n", __FUNCTION__, ao_dev, ret);
            }
            printf("\rset volume %d          ", volume);
            sleep(2); /* 2:2s */
        }

        fade.fade = TD_TRUE;
        fade.fade_in_rate = OT_AUDIO_FADE_RATE_128;
        fade.fade_out_rate = OT_AUDIO_FADE_RATE_128;

        ret = ss_mpi_ao_set_mute(ao_dev, TD_TRUE, &fade);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_ao_set_volume(%d), failed with %#x!\n", __FUNCTION__, ao_dev, ret);
        }
        printf("\rset ao mute            ");
        sleep(2); /* 2:2s */

        ret = ss_mpi_ao_set_mute(ao_dev, TD_FALSE, NULL);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_ao_set_volume(%d), failed with %#x!\n", __FUNCTION__, ao_dev, ret);
        }
        printf("\rset ao unmute          ");
        sleep(2); /* 2:2s */
    }

    return NULL;
}

/* create the thread to get frame from ai and send to ao */
td_s32 sample_comm_audio_creat_trd_ai_ao(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_dev ao_dev, ot_ao_chn ao_chn)
{
    sample_ai *ai = NULL;

    if ((ai_dev >= OT_AI_DEV_MAX_NUM) || (ai_dev < 0) ||
        (ai_chn >= OT_AI_MAX_CHN_NUM) || (ai_chn < 0)) {
        printf("%s: ai_dev = %d, ai_chn = %d error.\n", __FUNCTION__, ai_dev, ai_chn);
        return TD_FAILURE;
    }

    ai = &g_sample_ai[ai_dev * OT_AI_MAX_CHN_NUM + ai_chn];
    ai->send_aenc = TD_FALSE;
    ai->send_ao = TD_TRUE;
    ai->start = TD_TRUE;
    ai->ai_dev = ai_dev;
    ai->ai_chn = ai_chn;
    ai->ao_dev = ao_dev;
    ai->ao_chn = ao_chn;

    pthread_create(&ai->ai_pid, 0, sample_comm_audio_ai_proc, ai);

    return TD_SUCCESS;
}

/* create the thread to get frame from ai and send to aenc */
td_s32 sample_comm_audio_creat_trd_ai_aenc(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_aenc_chn ae_chn)
{
    sample_ai *ai = NULL;

    if ((ai_dev >= OT_AI_DEV_MAX_NUM) || (ai_dev < 0) ||
        (ai_chn >= OT_AI_MAX_CHN_NUM) || (ai_chn < 0)) {
        printf("%s: ai_dev = %d, ai_chn = %d error.\n", __FUNCTION__, ai_dev, ai_chn);
        return TD_FAILURE;
    }

    ai = &g_sample_ai[ai_dev * OT_AI_MAX_CHN_NUM + ai_chn];
    ai->send_aenc = TD_TRUE;
    ai->send_ao = TD_FALSE;
    ai->start = TD_TRUE;
    ai->ai_dev = ai_dev;
    ai->ai_chn = ai_chn;
    ai->aenc_chn = ae_chn;
    pthread_create(&ai->ai_pid, 0, sample_comm_audio_ai_proc, ai);

    return TD_SUCCESS;
}

/* create the thread to get stream from aenc and send to adec */
td_s32 sample_comm_audio_creat_trd_aenc_adec(ot_aenc_chn ae_chn, ot_adec_chn ad_chn, FILE *aenc_fd)
{
    sample_aenc *aenc = NULL;

    if (aenc_fd == NULL) {
        return TD_FAILURE;
    }

    if ((ae_chn >= OT_AENC_MAX_CHN_NUM) || (ae_chn < 0)) {
        printf("%s: ae_chn = %d error.\n", __FUNCTION__, ae_chn);
        return TD_FAILURE;
    }

    aenc = &g_sample_aenc[ae_chn];
    aenc->ae_chn = ae_chn;
    aenc->ad_chn = ad_chn;
    aenc->send_ad_chn = TD_TRUE;
    aenc->fd = aenc_fd;
    aenc->start = TD_TRUE;
    pthread_create(&aenc->aenc_pid, 0, sample_comm_audio_aenc_proc, aenc);

    return TD_SUCCESS;
}

/* create the thread to get stream from file and send to adec */
td_s32 sample_comm_audio_creat_trd_file_adec(ot_adec_chn ad_chn, FILE *adec_fd)
{
    sample_adec *adec = NULL;

    if (adec_fd == NULL) {
        return TD_FAILURE;
    }

    if ((ad_chn >= OT_ADEC_MAX_CHN_NUM) || (ad_chn < 0)) {
        printf("%s: ad_chn = %d error.\n", __FUNCTION__, ad_chn);
        return TD_FAILURE;
    }

    adec = &g_sample_adec[ad_chn];
    adec->ad_chn = ad_chn;
    adec->fd = adec_fd;
    adec->start = TD_TRUE;
    pthread_create(&adec->ad_pid, 0, sample_comm_audio_adec_proc, adec);

    return TD_SUCCESS;
}

/* create the thread to set ao volume */
td_s32 sample_comm_audio_creat_trd_ao_vol_ctrl(ot_audio_dev ao_dev)
{
    sample_ao *ao_ctl = NULL;

    if ((ao_dev >= OT_AO_DEV_MAX_NUM) || (ao_dev < 0)) {
        printf("%s: ao_dev = %d error.\n", __FUNCTION__, ao_dev);
        return TD_FAILURE;
    }

    ao_ctl = &g_sample_ao[ao_dev];
    ao_ctl->ao_dev = ao_dev;
    ao_ctl->start = TD_TRUE;
    pthread_create(&ao_ctl->ao_pid, 0, sample_comm_audio_ao_vol_proc, ao_ctl);

    return TD_SUCCESS;
}

/* destroy the thread to get frame from ai and send to ao or aenc */
td_s32 sample_comm_audio_destory_trd_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn)
{
    sample_ai *ai = NULL;

    if ((ai_dev >= OT_AI_DEV_MAX_NUM) || (ai_dev < 0) ||
        (ai_chn >= OT_AI_MAX_CHN_NUM) || (ai_chn < 0)) {
        printf("%s: ai_dev = %d, ai_chn = %d error.\n", __FUNCTION__, ai_dev, ai_chn);
        return TD_FAILURE;
    }

    ai = &g_sample_ai[ai_dev * OT_AI_MAX_CHN_NUM + ai_chn];
    if (ai->start) {
        ai->start = TD_FALSE;
        pthread_join(ai->ai_pid, 0);
    }

    return TD_SUCCESS;
}

/* destroy the thread to get stream from aenc and send to adec */
td_s32 sample_comm_audio_destory_trd_aenc_adec(ot_aenc_chn ae_chn)
{
    sample_aenc *aenc = NULL;

    if ((ae_chn >= OT_AENC_MAX_CHN_NUM) || (ae_chn < 0)) {
        printf("%s: ae_chn = %d error.\n", __FUNCTION__, ae_chn);
        return TD_FAILURE;
    }

    aenc = &g_sample_aenc[ae_chn];
    if (aenc->start) {
        aenc->start = TD_FALSE;
        pthread_join(aenc->aenc_pid, 0);
    }

    if (aenc->fd != TD_NULL) {
        fclose(aenc->fd);
        aenc->fd = TD_NULL;
    }

    return TD_SUCCESS;
}

/* destroy the thread to get stream from file and send to adec */
td_s32 sample_comm_audio_destory_trd_file_adec(ot_adec_chn ad_chn)
{
    sample_adec *adec = NULL;

    if ((ad_chn >= OT_ADEC_MAX_CHN_NUM) || (ad_chn < 0)) {
        printf("%s: ad_chn = %d error.\n", __FUNCTION__, ad_chn);
        return TD_FAILURE;
    }

    adec = &g_sample_adec[ad_chn];
    if (adec->start) {
        adec->start = TD_FALSE;
        pthread_join(adec->ad_pid, 0);
    }

    if (adec->fd != TD_NULL) {
        fclose(adec->fd);
        adec->fd = TD_NULL;
    }

    return TD_SUCCESS;
}

/* destroy the thread to set ao volume */
td_s32 sample_comm_audio_destory_trd_ao_vol_ctrl(ot_audio_dev ao_dev)
{
    sample_ao *ao_ctl = NULL;

    if ((ao_dev >= OT_AO_DEV_MAX_NUM) || (ao_dev < 0)) {
        printf("%s: ao_dev = %d error.\n", __FUNCTION__, ao_dev);
        return TD_FAILURE;
    }

    ao_ctl = &g_sample_ao[ao_dev];
    if (ao_ctl->start) {
        ao_ctl->start = TD_FALSE;
        pthread_cancel(ao_ctl->ao_pid);
        pthread_join(ao_ctl->ao_pid, 0);
    }

    return TD_SUCCESS;
}

/* ao bind adec */
td_s32 sample_comm_audio_ao_bind_adec(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_adec_chn ad_chn)
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

/* ao unbind adec */
td_s32 sample_comm_audio_ao_unbind_adec(ot_audio_dev ao_dev, ot_ao_chn ao_chn, ot_adec_chn ad_chn)
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

/* ao bind ai */
td_s32 sample_comm_audio_ao_bind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_dev ao_dev, ot_ao_chn ao_chn)
{
    ot_mpp_chn src_chn;
    ot_mpp_chn dest_chn;

    src_chn.mod_id = OT_ID_AI;
    src_chn.chn_id = ai_chn;
    src_chn.dev_id = ai_dev;
    dest_chn.mod_id = OT_ID_AO;
    dest_chn.dev_id = ao_dev;
    dest_chn.chn_id = ao_chn;

    return ss_mpi_sys_bind(&src_chn, &dest_chn);
}

/* ao unbind ai */
td_s32 sample_comm_audio_ao_unbind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_audio_dev ao_dev, ot_ao_chn ao_chn)
{
    ot_mpp_chn src_chn;
    ot_mpp_chn dest_chn;

    src_chn.mod_id = OT_ID_AI;
    src_chn.chn_id = ai_chn;
    src_chn.dev_id = ai_dev;
    dest_chn.mod_id = OT_ID_AO;
    dest_chn.dev_id = ao_dev;
    dest_chn.chn_id = ao_chn;

    return ss_mpi_sys_unbind(&src_chn, &dest_chn);
}

/* aenc bind ai */
td_s32 sample_comm_audio_aenc_bind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_aenc_chn ae_chn)
{
    ot_mpp_chn src_chn;
    ot_mpp_chn dest_chn;

    src_chn.mod_id = OT_ID_AI;
    src_chn.dev_id = ai_dev;
    src_chn.chn_id = ai_chn;
    dest_chn.mod_id = OT_ID_AENC;
    dest_chn.dev_id = 0;
    dest_chn.chn_id = ae_chn;

    return ss_mpi_sys_bind(&src_chn, &dest_chn);
}

/* aenc unbind ai */
td_s32 sample_comm_audio_aenc_unbind_ai(ot_audio_dev ai_dev, ot_ai_chn ai_chn, ot_aenc_chn ae_chn)
{
    ot_mpp_chn src_chn;
    ot_mpp_chn dest_chn;

    src_chn.mod_id = OT_ID_AI;
    src_chn.dev_id = ai_dev;
    src_chn.chn_id = ai_chn;
    dest_chn.mod_id = OT_ID_AENC;
    dest_chn.dev_id = 0;
    dest_chn.chn_id = ae_chn;

    return ss_mpi_sys_unbind(&src_chn, &dest_chn);
}

static td_s32 sample_comm_audio_start_ai_vqe(ot_audio_dev ai_dev_id, ot_ai_chn ai_chn,
    const sample_comm_ai_vqe_param *ai_vqe_param, ot_audio_dev ao_dev_id)
{
    td_s32 ret;

    if (ai_vqe_param->ai_vqe_attr != NULL) {
        td_bool ai_vqe = TD_TRUE;
        switch (ai_vqe_param->ai_vqe_type) {
            case 0: /* 0:no vqe */
                ret = TD_SUCCESS;
                ai_vqe = TD_FALSE;
                break;
            case 1: /* 1:record vqe */
                ret = ss_mpi_ai_set_record_vqe_attr(ai_dev_id, ai_chn,
                    (ot_ai_record_vqe_cfg *)ai_vqe_param->ai_vqe_attr);
                break;
            case 2: /* 2:talk vqe */
                ret = ss_mpi_ai_set_talk_vqe_attr(ai_dev_id, ai_chn, ao_dev_id, ai_chn,
                    (ot_ai_talk_vqe_cfg *)ai_vqe_param->ai_vqe_attr);
                break;
            case 3: /* 3:talkv2 vqe */
                ret = ss_mpi_ai_set_talk_vqe_v2_attr(ai_dev_id, ai_chn, ao_dev_id, ai_chn,
                    (ot_ai_talk_vqe_v2_cfg *)ai_vqe_param->ai_vqe_attr);
                break;
            default:
                ret = TD_FAILURE;
                break;
        }
        if (ret != TD_SUCCESS) {
            printf("%s: set_ai_vqe%d(%d,%d) failed with %#x\n", __FUNCTION__, ai_vqe_param->ai_vqe_type,
                ai_dev_id, ai_chn, ret);
            return ret;
        }

        if (ai_vqe == TD_TRUE) {
            ret = ss_mpi_ai_enable_vqe(ai_dev_id, ai_chn);
            if (ret) {
                printf("%s: ss_mpi_ai_enable_vqe(%d,%d) failed with %#x\n", __FUNCTION__, ai_dev_id, ai_chn, ret);
                return ret;
            }
        }
    }

    return TD_SUCCESS;
}

/* start ai */
td_s32 sample_comm_audio_start_ai(ot_audio_dev ai_dev_id, td_u32 ai_chn_cnt, ot_aio_attr *aio_attr,
    const sample_comm_ai_vqe_param *ai_vqe_param, ot_audio_dev ao_dev_id)
{
    td_s32 i;
    td_s32 ret;
    td_u32 chn_cnt;

    ret = ss_mpi_ai_set_pub_attr(ai_dev_id, aio_attr);
    if (ret) {
        printf("%s: ss_mpi_ai_set_pub_attr(%d) failed with %#x\n", __FUNCTION__, ai_dev_id, ret);
        return ret;
    }

    ret = ss_mpi_ai_enable(ai_dev_id);
    if (ret) {
        printf("%s: ss_mpi_ai_enable(%d) failed with %#x\n", __FUNCTION__, ai_dev_id, ret);
        return ret;
    }

    chn_cnt = ai_chn_cnt >> ((td_u32)aio_attr->snd_mode);
    for (i = 0; i < (td_s32)chn_cnt; i++) {
        ret = ss_mpi_ai_enable_chn(ai_dev_id, i);
        if (ret) {
            printf("%s: ss_mpi_ai_enable_chn(%d,%d) failed with %#x\n", __FUNCTION__, ai_dev_id, i, ret);
            return ret;
        }

        if (ai_vqe_param->resample_en == TD_TRUE) {
            ret = ss_mpi_ai_enable_resample(ai_dev_id, i, ai_vqe_param->out_sample_rate);
            if (ret) {
                printf("%s: ss_mpi_ai_enable_re_smp(%d,%d) failed with %#x\n", __FUNCTION__, ai_dev_id, i, ret);
                return ret;
            }
        }

        ret = sample_comm_audio_start_ai_vqe(ai_dev_id, i, ai_vqe_param, ao_dev_id);
        if (ret != TD_SUCCESS) {
            return ret;
        }
    }

    return TD_SUCCESS;
}

/* stop ai */
td_s32 sample_comm_audio_stop_ai(ot_audio_dev ai_dev_id, td_u32 ai_chn_cnt, td_bool resample_en, td_bool vqe_en)
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < (td_s32)ai_chn_cnt; i++) {
        if (resample_en == TD_TRUE) {
            ret = ss_mpi_ai_disable_resample(ai_dev_id, i);
            if (ret != TD_SUCCESS) {
                printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "failed");
                return ret;
            }
        }

        if (vqe_en == TD_TRUE) {
            ret = ss_mpi_ai_disable_vqe(ai_dev_id, i);
            if (ret != TD_SUCCESS) {
                printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "failed");
                return ret;
            }
        }

        ret = ss_mpi_ai_disable_chn(ai_dev_id, i);
        if (ret != TD_SUCCESS) {
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "failed");
            return ret;
        }
    }

    ret = ss_mpi_ai_disable(ai_dev_id);
    if (ret != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return ret;
    }

    return TD_SUCCESS;
}

#ifdef OT_ACODEC_TYPE_HDMI
static td_s32 hdmi_set_audio_param(const ot_aio_attr *aio_attr, ot_hdmi_attr *hdmi_attr)
{
   /* if enable audio */
    hdmi_attr->audio_en = TD_TRUE;

    /* sampling rate of PCM audio, the parameter must be consistent with the input of AO */
    switch (aio_attr->sample_rate) {
        case OT_AUDIO_SAMPLE_RATE_32000:
            hdmi_attr->sample_rate = OT_HDMI_SAMPLE_RATE_32K;
            break;

        case OT_AUDIO_SAMPLE_RATE_44100:
            hdmi_attr->sample_rate = OT_HDMI_SAMPLE_RATE_44K;
            break;

        case OT_AUDIO_SAMPLE_RATE_48000:
            hdmi_attr->sample_rate = OT_HDMI_SAMPLE_RATE_48K;
            break;

        default:
            printf("[func]:%s [line]:%d [info]:%s %d\n", __FUNCTION__, __LINE__,
                "invalid sample rate = ", aio_attr->sample_rate);
            return TD_FAILURE;
    }

    /* bitwidth of audio, default :16, the parameter must be consistent with the config of AO */
    switch (aio_attr->bit_width) {
        case OT_AUDIO_BIT_WIDTH_8:
            hdmi_attr->bit_depth = OT_HDMI_BIT_DEPTH_8;
            break;

        case OT_AUDIO_BIT_WIDTH_16:
            hdmi_attr->bit_depth = OT_HDMI_BIT_DEPTH_16;
            break;

        case OT_AUDIO_BIT_WIDTH_24:
            hdmi_attr->bit_depth = OT_HDMI_BIT_DEPTH_24;
            break;

        default:
            printf("[func]:%s [line]:%d [info]:%s %d\n", __FUNCTION__, __LINE__,
                "invalid bit width = ", aio_attr->bit_width);
            return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_audio_start_hdmi(const ot_aio_attr *aio_attr)
{
    td_s32 ret;
    ot_hdmi_attr hdmi_attr = {0};
    ot_hdmi_id hdmi = OT_HDMI_ID_0;
    ot_vo_pub_attr pub_attr = {0};
    const ot_vo_dev vo_dev = 0;

    pub_attr.bg_color = 0x000000ff; /* 0x000000ff:bg color */
    pub_attr.intf_type = OT_VO_INTF_HDMI;
    pub_attr.intf_sync = OT_VO_OUT_1080P60;

    if (sample_comm_vo_start_dev(vo_dev, &pub_attr, TD_NULL, 0) != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "sample_comm_vo_start_dev failed");
        return TD_FAILURE;
    }

    ret = sample_comm_vo_hdmi_start(pub_attr.intf_sync);
    if (ret != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "sample_comm_vo_hdmi_start failed");
        return TD_FAILURE;
    }

    ret = ss_mpi_hdmi_stop(hdmi);
    if (ret != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "ss_mpi_hdmi_stop failed");
        return TD_FAILURE;
    }

    ret = ss_mpi_hdmi_get_attr(hdmi, &hdmi_attr);
    if (ret != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "ss_mpi_hdmi_get_attr failed");
        return TD_FAILURE;
    }

    ret = hdmi_set_audio_param(aio_attr, &hdmi_attr);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }

    ret = ss_mpi_hdmi_set_attr(hdmi, &hdmi_attr);
    if (ret != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "ss_mpi_hdmi_set_attr failed");
        return TD_FAILURE;
    }

    ret = ss_mpi_hdmi_start(hdmi);
    if (ret != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "ss_mpi_hdmi_start failed");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

td_s32 sample_comm_audio_stop_hdmi(td_void)
{
    td_s32 ret;
    const ot_vo_dev vo_dev = 0;

    ret = sample_comm_vo_hdmi_stop();
    if (ret != TD_SUCCESS) {
        printf("%s: sample_comm_vo_hdmi_stop failed with %#x!\n", __FUNCTION__, ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_vo_disable(vo_dev);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_vo_disable failed with %#x!\n", __FUNCTION__, ret);
        return TD_FAILURE;
    }

    return ret;
}
#endif

/* start ao */
td_s32 sample_comm_audio_start_ao(ot_audio_dev ao_dev_id, td_u32 ao_chn_cnt, ot_aio_attr *aio_attr,
    ot_audio_sample_rate in_sample_rate, td_bool resample_en)
{
    td_s32 i;
    td_s32 ret;
    td_u32 chn_cnt;

    if (ao_dev_id == SAMPLE_AUDIO_INNER_HDMI_AO_DEV) {
#ifdef OT_ACODEC_TYPE_HDMI
        aio_attr->clk_share = 0;

        sample_comm_audio_start_hdmi(aio_attr);
#endif
    }

    ret = ss_mpi_ao_set_pub_attr(ao_dev_id, aio_attr);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_ao_set_pub_attr(%d) failed with %#x!\n", __FUNCTION__, ao_dev_id, ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_ao_enable(ao_dev_id);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_ao_enable(%d) failed with %#x!\n", __FUNCTION__, ao_dev_id, ret);
        return TD_FAILURE;
    }

    chn_cnt = ao_chn_cnt >> ((td_u32)aio_attr->snd_mode);
    for (i = 0; i < (td_s32)chn_cnt; i++) {
        ret = ss_mpi_ao_enable_chn(ao_dev_id, i);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_ao_enable_chn(%d) failed with %#x!\n", __FUNCTION__, i, ret);
            return TD_FAILURE;
        }

        if (resample_en == TD_TRUE) {
            ret = ss_mpi_ao_disable_resample(ao_dev_id, i);
            if (ret != TD_SUCCESS) {
                printf("%s: ss_mpi_ao_disable_resample (%d,%d) failed with %#x!\n", __FUNCTION__, ao_dev_id, i, ret);
                return TD_FAILURE;
            }

            ret = ss_mpi_ao_enable_resample(ao_dev_id, i, in_sample_rate);
            if (ret != TD_SUCCESS) {
                printf("%s: ss_mpi_ao_enable_resample(%d,%d) failed with %#x!\n", __FUNCTION__, ao_dev_id, i, ret);
                return TD_FAILURE;
            }
        }
    }

    ret = ss_mpi_ao_enable_chn(ao_dev_id, OT_AO_SYS_CHN_ID);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_ao_enable_chn(%d) failed with %#x!\n", __FUNCTION__, i, ret);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

/* stop ao */
td_s32 sample_comm_audio_stop_ao(ot_audio_dev ao_dev_id, td_u32 ao_chn_cnt, td_bool resample_en)
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < (td_s32)ao_chn_cnt; i++) {
        if (resample_en == TD_TRUE) {
            ret = ss_mpi_ao_disable_resample(ao_dev_id, i);
            if (ret != TD_SUCCESS) {
                printf("%s: ss_mpi_ao_disable_re_smp failed with %#x!\n", __FUNCTION__, ret);
                return ret;
            }
        }

        ret = ss_mpi_ao_disable_chn(ao_dev_id, i);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_ao_disable_chn failed with %#x!\n", __FUNCTION__, ret);
            return ret;
        }
    }

    ret = ss_mpi_ao_disable_chn(ao_dev_id, OT_AO_SYS_CHN_ID);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_ao_disable_chn(%d) failed with %#x!\n", __FUNCTION__, i, ret);
        return TD_FAILURE;
    }

    ret = ss_mpi_ao_disable(ao_dev_id);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_ao_disable failed with %#x!\n", __FUNCTION__, ret);
        return ret;
    }

    if (ao_dev_id == SAMPLE_AUDIO_INNER_HDMI_AO_DEV) {
#ifdef OT_ACODEC_TYPE_HDMI
        ret = sample_comm_audio_stop_hdmi();
        if (ret != TD_SUCCESS) {
            printf("%s: sample_comm_audio_stop_hdmi failed with %#x!\n", __FUNCTION__, ret);
            return ret;
        }
#endif
    }

    return TD_SUCCESS;
}

/* start aenc */
td_s32 sample_comm_audio_start_aenc(td_u32 aenc_chn_cnt, const ot_aio_attr *aio_attr, ot_payload_type type)
{
    ot_aenc_chn ae_chn;
    td_s32 ret;
    td_s32 i;
    ot_aenc_chn_attr aenc_attr;
    ot_aenc_attr_adpcm adpcm_aenc;
    ot_aenc_attr_g711 aenc_g711;
    ot_aenc_attr_g726 aenc_g726;
    ot_aenc_attr_lpcm aenc_lpcm;
    ot_aenc_attr_aac aenc_aac;

    /* set AENC chn attr */
    aenc_attr.type = type;
    aenc_attr.buf_size = 30; /* 30:size */
    aenc_attr.point_num_per_frame = aio_attr->point_num_per_frame;

    if (aenc_attr.type == OT_PT_ADPCMA) {
        aenc_attr.value = &adpcm_aenc;
        adpcm_aenc.adpcm_type = AUDIO_ADPCM_TYPE;
    } else if ((aenc_attr.type == OT_PT_G711A) || (aenc_attr.type == OT_PT_G711U)) {
        aenc_attr.value = &aenc_g711;
    } else if (aenc_attr.type == OT_PT_G726) {
        aenc_attr.value = &aenc_g726;
        aenc_g726.g726bps = G726_BPS;
    } else if (aenc_attr.type == OT_PT_LPCM) {
        aenc_attr.value = &aenc_lpcm;
    } else if (aenc_attr.type == OT_PT_AAC) {
        aenc_attr.value = &aenc_aac;
        aenc_aac.aac_type = g_aac_type;
        aenc_aac.bit_rate = g_aac_bps;
        aenc_aac.bit_width = OT_AUDIO_BIT_WIDTH_16;
        aenc_aac.sample_rate = aio_attr->sample_rate;
        aenc_aac.snd_mode = aio_attr->snd_mode;
        aenc_aac.transport_type = g_aac_transport_type;
        aenc_aac.band_width = 0;
    } else {
        printf("%s: invalid aenc payload type:%d\n", __FUNCTION__, aenc_attr.type);
        return TD_FAILURE;
    }

    for (i = 0; i < (td_s32)aenc_chn_cnt; i++) {
        ae_chn = i;
        /* create aenc chn */
        ret = ss_mpi_aenc_create_chn(ae_chn, &aenc_attr);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_aenc_create_chn(%d) failed with %#x!\n", __FUNCTION__, ae_chn, ret);
            return ret;
        }
    }

    return TD_SUCCESS;
}

/* stop aenc */
td_s32 sample_comm_audio_stop_aenc(td_u32 aenc_chn_cnt)
{
    td_s32 i;
    td_s32 ret;

    for (i = 0; i < (td_s32)aenc_chn_cnt; i++) {
        ret = ss_mpi_aenc_destroy_chn(i);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_aenc_destroy_chn(%d) failed with %#x!\n", __FUNCTION__, i, ret);
            return ret;
        }
    }

    return TD_SUCCESS;
}

/* destroy the all thread */
td_s32 sample_comm_audio_destory_all_trd(void)
{
    td_u32 dev_id;
    td_u32 chn_id;

    for (dev_id = 0; dev_id < OT_AI_DEV_MAX_NUM; dev_id++) {
        for (chn_id = 0; chn_id < OT_AI_MAX_CHN_NUM; chn_id++) {
            if (sample_comm_audio_destory_trd_ai(dev_id, chn_id) != TD_SUCCESS) {
                printf("%s: sample_comm_audio_destory_trd_ai(%d,%d) failed!\n", __FUNCTION__, dev_id, chn_id);
                return TD_FAILURE;
            }
        }
    }

    for (chn_id = 0; chn_id < OT_AENC_MAX_CHN_NUM; chn_id++) {
        if (sample_comm_audio_destory_trd_aenc_adec(chn_id) != TD_SUCCESS) {
            printf("%s: sample_comm_audio_destory_trd_aenc_adec(%d) failed!\n", __FUNCTION__, chn_id);
            return TD_FAILURE;
        }
    }

    for (chn_id = 0; chn_id < OT_ADEC_MAX_CHN_NUM; chn_id++) {
        if (sample_comm_audio_destory_trd_file_adec(chn_id) != TD_SUCCESS) {
            printf("%s: sample_comm_audio_destory_trd_file_adec(%d) failed!\n", __FUNCTION__, chn_id);
            return TD_FAILURE;
        }
    }

    for (chn_id = 0; chn_id < OT_AO_DEV_MAX_NUM; chn_id++) {
        if (sample_comm_audio_destory_trd_ao_vol_ctrl(chn_id) != TD_SUCCESS) {
            printf("%s: sample_comm_audio_destory_trd_ao_vol_ctrl(%d) failed!\n", __FUNCTION__, chn_id);
            return TD_FAILURE;
        }
    }

    return TD_SUCCESS;
}

/* start adec */
td_s32 sample_comm_audio_start_adec(td_u32 adec_chn_cnt, ot_payload_type type)
{
    td_s32 ret;
    td_s32 i;
    ot_adec_chn ad_chn;
    ot_adec_chn_attr adec_attr;
    ot_adec_attr_adpcm adpcm;
    ot_adec_attr_g711 adec_g711;
    ot_adec_attr_g726 adec_g726;
    ot_adec_attr_lpcm adec_lpcm;
    ot_adec_attr_aac adec_aac;

    adec_attr.type = type;
    adec_attr.buf_size = 20; /* 20: adec buf size */
    adec_attr.mode = OT_ADEC_MODE_STREAM; /* propose use pack mode in your app */

    if (adec_attr.type == OT_PT_ADPCMA) {
        adec_attr.value = &adpcm;
        adpcm.adpcm_type = AUDIO_ADPCM_TYPE;
    } else if ((adec_attr.type == OT_PT_G711A) || (adec_attr.type == OT_PT_G711U)) {
        adec_attr.value = &adec_g711;
    } else if (adec_attr.type == OT_PT_G726) {
        adec_attr.value = &adec_g726;
        adec_g726.g726bps = G726_BPS;
    } else if (adec_attr.type == OT_PT_LPCM) {
        adec_attr.value = &adec_lpcm;
        adec_attr.mode = OT_ADEC_MODE_PACK; /* lpcm must use pack mode */
    } else if (adec_attr.type == OT_PT_AAC) {
        adec_attr.value = &adec_aac;
        adec_attr.mode = OT_ADEC_MODE_STREAM; /* aac should be stream mode */
        adec_aac.transport_type = g_aac_transport_type;
    } else {
        printf("%s: invalid aenc payload type:%d\n", __FUNCTION__, adec_attr.type);
        return TD_FAILURE;
    }

    /* create adec chn */
    for (i = 0; i < (td_s32)adec_chn_cnt; i++) {
        ad_chn = i;
        ret = ss_mpi_adec_create_chn(ad_chn, &adec_attr);
        if (ret != TD_SUCCESS) {
            printf("%s: ss_mpi_adec_create_chn(%d) failed with %#x!\n", __FUNCTION__, ad_chn, ret);
            return ret;
        }
    }
    return TD_SUCCESS;
}

/* stop adec */
td_s32 sample_comm_audio_stop_adec(ot_adec_chn ad_chn)
{
    td_s32 ret;

    ret = ss_mpi_adec_destroy_chn(ad_chn);
    if (ret != TD_SUCCESS) {
        printf("%s: ss_mpi_adec_destroy_chn(%d) failed with %#x!\n", __FUNCTION__, ad_chn, ret);
        return ret;
    }

    return TD_SUCCESS;
}
