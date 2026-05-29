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

#include "audio_aac_adp.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "audio_dl_adp.h"
#include "securec.h"

#define aac_check_false_return(x) \
    do { \
        if ((x) != TD_TRUE) { \
            return (-1); \
        } \
    } while (0)

#define aac_bitrate_sel(is_mono, mono_bitrate, stereo_bitrate) ((is_mono) ? (mono_bitrate) : (stereo_bitrate))

#define AAC_ENC_LIB_NAME "libaac_enc.so"
#define AAC_DEC_LIB_NAME "libaac_dec.so"

#define AACENC_BITRATE_16K 16000
#define AACENC_BITRATE_24K 24000
#define AACENC_BITRATE_32K 32000
#define AACENC_BITRATE_44K 44000
#define AACENC_BITRATE_48K 48000
#define AACENC_BITRATE_64K 64000
#define AACENC_BITRATE_96K 96000
#define AACENC_BITRATE_128K 128000
#define AACENC_BITRATE_132K 132000
#define AACENC_BITRATE_144K 144000
#define AACENC_BITRATE_192K 192000
#define AACENC_BITRATE_256K 256000
#define AACENC_BITRATE_265K 265000
#define AACENC_BITRATE_288K 288000
#define AACENC_BITRATE_320K 320000

#define AACENC_CHANNEL_SINGLE   1
#define AACENC_CHANNEL_STEREO   2

#define DUMP_PATH_NAME_MAX_BYTES 256
#define DUMP_MAX_TIMES 10000

/* aac enc lib */
typedef td_s32 (*aacenc_get_version_callback)(ot_aacenc_version *version);
typedef td_s32 (*aacenc_init_default_config_callback)(ot_aacenc_config *config);
typedef td_s32 (*aac_encoder_open_callback)(ot_aac_encoder **ph_aac_plus_enc, const ot_aacenc_config *config);
typedef td_s32 (*aac_encoder_frame_callback)(ot_aac_encoder *aac_plus_enc, td_s16 *pcm_buf,
                                             td_u8 *outbuf, td_s32 *num_out_bytes);
typedef td_void (*aac_encoder_close_callback)(ot_aac_encoder *aac_plus_enc);

/* aac dec lib */
typedef td_s32 (*aacdec_get_version_callback)(ot_aacdec_version *version);
typedef ot_aac_decoder (*aac_init_decoder_callback)(ot_aacdec_transport_type tran_type);
typedef td_void (*aac_free_decoder_callback)(ot_aac_decoder aac_decoder);
typedef td_s32 (*aac_set_raw_mode_callback)(ot_aac_decoder aac_decoder, td_s32 chans, td_s32 samp_rate);
typedef td_s32 (*aac_decode_find_sync_header_callback)(
    ot_aac_decoder aac_decoder, td_u8 **pp_inbuf_ptr, td_s32 *bytes_left);
typedef td_s32 (*aac_decode_frame_callback)(ot_aac_decoder aac_decoder,
                                            td_u8 **pp_inbuf_ptr, td_s32 *bytes_left, td_s16 *out_pcm);
typedef td_s32 (*aac_get_last_frame_info_callback)(ot_aac_decoder aac_decoder, ot_aacdec_frame_info *aac_frame_info);
typedef td_s32 (*aac_decoder_set_eos_flag_callback)(ot_aac_decoder aac_decoder, td_s32 eosflag);
typedef td_s32 (*aac_flush_codec_callback)(ot_aac_decoder aac_decoder);

typedef struct {
    td_s32 open_cnt;
    td_void *lib_handle;

    aacenc_get_version_callback ot_aacenc_get_version;
    aacenc_init_default_config_callback ot_aacenc_init_default_config;
    aac_encoder_open_callback ot_aacenc_open;
    aac_encoder_frame_callback ot_aacenc_frame;
    aac_encoder_close_callback ot_aacenc_close;
} aacenc_fun;

typedef struct {
    td_s32 open_cnt;
    td_void *lib_handle;

    aacdec_get_version_callback ot_aacdec_get_version;
    aac_init_decoder_callback ot_aacdec_init_decoder;
    aac_free_decoder_callback ot_aacdec_free_decoder;
    aac_set_raw_mode_callback ot_aacdec_set_raw_mode;
    aac_decode_find_sync_header_callback ot_aacdec_find_sync_header;
    aac_decode_frame_callback ot_aacdec_frame;
    aac_get_last_frame_info_callback ot_aacdec_get_last_frame_info;
    aac_decoder_set_eos_flag_callback ot_aacdec_set_eos_flag;
    aac_flush_codec_callback ot_aacdec_flush_codec;
} aacdec_fun;

/* if need, define DUMP_AACENC */
#ifdef DUMP_AACENC
FILE *g_in_enc_fd = NULL;
FILE *g_out_enc_fd = NULL;
#endif

/* if need, define DUMP_AACDEC */
#ifdef DUMP_AACDEC
FILE *g_in_dec_fd = NULL;
FILE *g_out_dec_fd = NULL;
FILE *g_out_dec_left_fd = NULL;
#endif

int g_cnt_aenc = 100000;
int g_cnt_adec = 100000;

static td_s32 g_aac_enc_handle = 0;
static td_s32 g_aac_dec_handle = 0;

#if defined(OT_AAC_USE_STATIC_MODULE_REGISTER) && defined(OT_AAC_HAVE_SBR_LIB)
static td_bool g_aac_enc_static_reg = TD_FALSE;
static td_bool g_aac_dec_static_reg = TD_FALSE;
#endif

static aacenc_fun g_aac_enc_func = {0};
static aacdec_fun g_aac_dec_func = {0};

#if defined(OT_AAC_USE_STATIC_MODULE_REGISTER) && defined(OT_AAC_HAVE_SBR_LIB)
static td_s32 aac_init_sbr_enc_lib(void)
{
    td_s32 ret;
    td_void *sbr_enc_handle = ot_aac_sbrenc_get_handle();

    if (g_aac_enc_static_reg == TD_TRUE) {
        return TD_SUCCESS;
    }

    ret = ot_aacenc_register_mod(sbr_enc_handle);
    if (ret != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n",
            __FUNCTION__, __LINE__, "init sbr_enc lib fail!\n");
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    g_aac_enc_static_reg = TD_TRUE;

    return TD_SUCCESS;
}

static td_s32 aac_init_sbr_dec_lib(void)
{
    td_s32 ret;
    td_void *sbr_dec_handle = ot_aac_sbrdec_get_handle();

    if (g_aac_dec_static_reg == TD_TRUE) {
        return TD_SUCCESS;
    }

    ret = ot_aacdec_register_mod(sbr_dec_handle);
    if (ret != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "init sbr_dec lib fail!\n");
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    g_aac_dec_static_reg = TD_TRUE;

    return TD_SUCCESS;
}
#endif

#ifdef OT_AAC_USE_STATIC_MODULE_REGISTER
static td_s32 aac_init_enc_lib(void)
{
    g_aac_enc_func.ot_aacenc_get_version = ot_aacenc_get_version;
    g_aac_enc_func.ot_aacenc_init_default_config = ot_aacenc_init_default_config;
    g_aac_enc_func.ot_aacenc_open = ot_aacenc_open;
    g_aac_enc_func.ot_aacenc_frame = ot_aacenc_frame;
    g_aac_enc_func.ot_aacenc_close = ot_aacenc_close;
#ifdef OT_AAC_HAVE_SBR_LIB
    return aac_init_sbr_enc_lib();
#endif
    return TD_SUCCESS;
}
#else
static td_s32 aac_enc_lib_dlsym(aacenc_fun *aac_enc_func)
{
    td_s32 ret;

    ret = audio_dlsym((td_void **)&(aac_enc_func->ot_aacenc_get_version),
        aac_enc_func->lib_handle, "ot_aacenc_get_version");
    if (ret != TD_SUCCESS) {
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_enc_func->ot_aacenc_init_default_config),
        aac_enc_func->lib_handle, "ot_aacenc_init_default_config");
    if (ret != TD_SUCCESS) {
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_enc_func->ot_aacenc_open),
        aac_enc_func->lib_handle, "ot_aacenc_open");
    if (ret != TD_SUCCESS) {
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_enc_func->ot_aacenc_frame),
        aac_enc_func->lib_handle, "ot_aacenc_frame");
    if (ret != TD_SUCCESS) {
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_enc_func->ot_aacenc_close),
        aac_enc_func->lib_handle, "ot_aacenc_close");
    if (ret != TD_SUCCESS) {
        return OT_ERR_AENC_NOT_SUPPORT;
    }

    return TD_SUCCESS;
}

static td_s32 aac_init_enc_lib(void)
{
    if (g_aac_enc_func.open_cnt == 0) {
        td_s32 ret;
        aacenc_fun aac_enc_func = {0};

        ret = audio_dlopen(&(aac_enc_func.lib_handle), AAC_ENC_LIB_NAME);
        if (ret != TD_SUCCESS) {
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "load aenc lib fail!\n");
            return OT_ERR_AENC_NOT_SUPPORT;
        }

        ret = aac_enc_lib_dlsym(&aac_enc_func);
        if (ret != TD_SUCCESS) {
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "find symbol error!\n");
            audio_dlclose(aac_enc_func.lib_handle);
            aac_enc_func.lib_handle = TD_NULL;
            return OT_ERR_AENC_NOT_SUPPORT;
        }

        (td_void)memcpy_s(&g_aac_enc_func, sizeof(g_aac_enc_func), &aac_enc_func, sizeof(aac_enc_func));
    }
    g_aac_enc_func.open_cnt++;
    return TD_SUCCESS;
}
#endif

td_void aac_deinit_enc_lib(td_void)
{
    td_s32 ret;
    if (g_aac_enc_func.open_cnt != 0) {
        g_aac_enc_func.open_cnt--;
    }

    if (g_aac_enc_func.open_cnt == 0) {
#ifndef OT_AAC_USE_STATIC_MODULE_REGISTER
        if (g_aac_enc_func.lib_handle != TD_NULL) {
            audio_dlclose(g_aac_enc_func.lib_handle);
        }
#endif
        ret = memset_s(&g_aac_enc_func, sizeof(aacenc_fun), 0, sizeof(aacenc_fun));
        if (ret != EOK) {
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "memset_s aenc fun fail!\n");
            return;
        }
    }

    return;
}

td_s32 aacenc_get_version_adp(ot_aacenc_version *version)
{
    if (g_aac_enc_func.ot_aacenc_get_version == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
            __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_AENC_NOT_SUPPORT;
    }
    return g_aac_enc_func.ot_aacenc_get_version(version);
}

td_s32 aac_init_default_config_adp(ot_aacenc_config *config)
{
    if (g_aac_enc_func.ot_aacenc_init_default_config == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_AENC_NOT_SUPPORT;
    }
    return g_aac_enc_func.ot_aacenc_init_default_config(config);
}

td_s32 aac_encoder_open_adp(ot_aac_encoder **ph_aac_plus_enc, const ot_aacenc_config *config)
{
    if (g_aac_enc_func.ot_aacenc_open == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_AENC_NOT_SUPPORT;
    }
    return g_aac_enc_func.ot_aacenc_open(ph_aac_plus_enc, config);
}

td_s32 aac_encoder_frame_adp(ot_aac_encoder *aac_plus_enc, td_s16 *pcm_buf, td_u8 *outbuf, td_s32 *num_out_bytes)
{
    if (g_aac_enc_func.ot_aacenc_frame == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_AENC_NOT_SUPPORT;
    }
    return g_aac_enc_func.ot_aacenc_frame(aac_plus_enc, pcm_buf, outbuf, num_out_bytes);
}

td_void aac_encoder_close_adp(ot_aac_encoder *aac_plus_enc)
{
    if (g_aac_enc_func.ot_aacenc_close == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return;
    }
    return g_aac_enc_func.ot_aacenc_close(aac_plus_enc);
}

#ifdef OT_AAC_USE_STATIC_MODULE_REGISTER
static td_s32 aac_init_dec_lib(void)
{
    g_aac_dec_func.ot_aacdec_get_version = ot_aacdec_get_version;
    g_aac_dec_func.ot_aacdec_init_decoder = ot_aacdec_init_decoder;
    g_aac_dec_func.ot_aacdec_free_decoder = ot_aacdec_free_decoder;
    g_aac_dec_func.ot_aacdec_set_raw_mode = ot_aacdec_set_raw_mode;
    g_aac_dec_func.ot_aacdec_find_sync_header = ot_aacdec_find_sync_header;
    g_aac_dec_func.ot_aacdec_frame = ot_aacdec_frame;
    g_aac_dec_func.ot_aacdec_get_last_frame_info = ot_aacdec_get_last_frame_info;
    g_aac_dec_func.ot_aacdec_set_eos_flag = ot_aacdec_set_eos_flag;
    g_aac_dec_func.ot_aacdec_flush_codec = ot_aacdec_flush_codec;
#ifdef OT_AAC_HAVE_SBR_LIB
    return aac_init_sbr_dec_lib();
#endif
    return TD_SUCCESS;
}
#else
static td_s32 aac_dec_lib_dlsym(aacdec_fun *aac_dec_func)
{
    td_s32 ret;
    ret = audio_dlsym((td_void **)&(aac_dec_func->ot_aacdec_get_version),
                      aac_dec_func->lib_handle, "ot_aacdec_get_version");
    if (ret != TD_SUCCESS) {
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_dec_func->ot_aacdec_init_decoder),
                      aac_dec_func->lib_handle, "ot_aacdec_init_decoder");
    if (ret != TD_SUCCESS) {
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_dec_func->ot_aacdec_free_decoder),
                      aac_dec_func->lib_handle, "ot_aacdec_free_decoder");
    if (ret != TD_SUCCESS) {
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_dec_func->ot_aacdec_set_raw_mode),
                      aac_dec_func->lib_handle, "ot_aacdec_set_raw_mode");
    if (ret != TD_SUCCESS) {
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_dec_func->ot_aacdec_find_sync_header),
                      aac_dec_func->lib_handle, "ot_aacdec_find_sync_header");
    if (ret != TD_SUCCESS) {
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_dec_func->ot_aacdec_frame), aac_dec_func->lib_handle, "ot_aacdec_frame");
    if (ret != TD_SUCCESS) {
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_dec_func->ot_aacdec_get_last_frame_info),
                      aac_dec_func->lib_handle, "ot_aacdec_get_last_frame_info");
    if (ret != TD_SUCCESS) {
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_dec_func->ot_aacdec_set_eos_flag),
                      aac_dec_func->lib_handle, "ot_aacdec_set_eos_flag");
    if (ret != TD_SUCCESS) {
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    ret = audio_dlsym((td_void **)&(aac_dec_func->ot_aacdec_flush_codec),
                      aac_dec_func->lib_handle, "ot_aacdec_flush_codec");
    if (ret != TD_SUCCESS) {
        return OT_ERR_ADEC_NOT_SUPPORT;
    }

    return TD_SUCCESS;
}

static td_s32 aac_init_dec_lib(void)
{
    if (g_aac_dec_func.open_cnt == 0) {
        td_s32 ret;
        aacdec_fun aac_dec_func = {0};

        ret  = audio_dlopen(&(aac_dec_func.lib_handle), AAC_DEC_LIB_NAME);
        if (ret != TD_SUCCESS) {
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "load aenc lib fail!\n");
            return OT_ERR_ADEC_NOT_SUPPORT;
        }

        ret = aac_dec_lib_dlsym(&aac_dec_func);
        if (ret != TD_SUCCESS) {
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "find symbol error!\n");
            audio_dlclose(aac_dec_func.lib_handle);
            aac_dec_func.lib_handle = TD_NULL;
            return OT_ERR_ADEC_NOT_SUPPORT;
        }

        (td_void)memcpy_s(&g_aac_dec_func, sizeof(g_aac_dec_func), &aac_dec_func, sizeof(aac_dec_func));
    }
    g_aac_dec_func.open_cnt++;

    return TD_SUCCESS;
}
#endif

td_void aac_deinit_dec_lib(td_void)
{
    td_s32 ret;
    if (g_aac_dec_func.open_cnt != 0) {
        g_aac_dec_func.open_cnt--;
    }

    if (g_aac_dec_func.open_cnt == 0) {
#ifndef OT_AAC_USE_STATIC_MODULE_REGISTER
        if (g_aac_dec_func.lib_handle != TD_NULL) {
            audio_dlclose(g_aac_dec_func.lib_handle);
        }
#endif
        ret = memset_s(&g_aac_dec_func, sizeof(aacdec_fun), 0, sizeof(aacdec_fun));
        if (ret != EOK) {
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "memset_s adec lib fail!\n");
            return;
        }
    }

    return;
}

td_s32 aacdec_get_version_adp(ot_aacdec_version *version)
{
    if (g_aac_dec_func.ot_aacdec_get_version == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_ADEC_NOT_SUPPORT;
    }
    return g_aac_dec_func.ot_aacdec_get_version(version);
}

ot_aac_decoder aac_init_decoder_adp(ot_aacdec_transport_type tran_type)
{
    if (g_aac_dec_func.ot_aacdec_init_decoder == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return TD_NULL;
    }
    return g_aac_dec_func.ot_aacdec_init_decoder(tran_type);
}

td_void aac_free_decoder_adp(ot_aac_decoder aac_decoder)
{
    if (g_aac_dec_func.ot_aacdec_free_decoder == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return;
    }
    return g_aac_dec_func.ot_aacdec_free_decoder(aac_decoder);
}

td_s32 aac_set_raw_mode_adp(ot_aac_decoder aac_decoder, td_s32 chans, td_s32 samp_rate)
{
    if (g_aac_dec_func.ot_aacdec_set_raw_mode == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_ADEC_NOT_SUPPORT;
    }
    return g_aac_dec_func.ot_aacdec_set_raw_mode(aac_decoder, chans, samp_rate);
}

td_s32 aac_decode_find_sync_header_adp(ot_aac_decoder aac_decoder, td_u8 **pp_inbuf_ptr, td_s32 *bytes_left)
{
    if (g_aac_dec_func.ot_aacdec_find_sync_header == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_ADEC_NOT_SUPPORT;
    }
    return g_aac_dec_func.ot_aacdec_find_sync_header(aac_decoder, pp_inbuf_ptr, bytes_left);
}

td_s32 aac_decode_frame_adp(ot_aac_decoder aac_decoder, td_u8 **pp_inbuf_ptr, td_s32 *bytes_left, td_s16 *out_pcm)
{
    if (g_aac_dec_func.ot_aacdec_frame == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_ADEC_NOT_SUPPORT;
    }
    return g_aac_dec_func.ot_aacdec_frame(aac_decoder, pp_inbuf_ptr, bytes_left, out_pcm);
}

td_s32 aac_get_last_frame_info_adp(ot_aac_decoder aac_decoder, ot_aacdec_frame_info *aac_frame_info)
{
    if (g_aac_dec_func.ot_aacdec_get_last_frame_info == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_ADEC_NOT_SUPPORT;
    }
    return g_aac_dec_func.ot_aacdec_get_last_frame_info(aac_decoder, aac_frame_info);
}

td_s32 aac_decoder_set_eos_flag_adp(ot_aac_decoder aac_decoder, td_s32 eosflag)
{
    if (g_aac_dec_func.ot_aacdec_set_eos_flag == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_ADEC_NOT_SUPPORT;
    }
    return g_aac_dec_func.ot_aacdec_set_eos_flag(aac_decoder, eosflag);
}

td_s32 aac_flush_codec_adp(ot_aac_decoder aac_decoder)
{
    if (g_aac_dec_func.ot_aacdec_flush_codec == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "call aac function fail!\n");
        return OT_ERR_ADEC_NOT_SUPPORT;
    }
    return g_aac_dec_func.ot_aacdec_flush_codec(aac_decoder);
}

static td_s32 aenc_check_aac_attr(const ot_aenc_attr_aac *aac_attr)
{
    if (aac_attr->bit_width != OT_AUDIO_BIT_WIDTH_16) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "invalid bitwidth for AAC encoder");
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (aac_attr->snd_mode >= OT_AUDIO_SOUND_MODE_BUTT) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "invalid sound mode for AAC encoder");
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if ((aac_attr->aac_type == OT_AAC_TYPE_EAACPLUS) && (aac_attr->snd_mode != OT_AUDIO_SOUND_MODE_STEREO)) {
        printf("[func]:%s [line]:%d [info]:%s\n",
               __FUNCTION__, __LINE__, "invalid sound mode for AAC encoder");
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (aac_attr->transport_type == OT_AAC_TRANSPORT_TYPE_ADTS) {
        if ((aac_attr->aac_type == OT_AAC_TYPE_AACLD) ||
            (aac_attr->aac_type == OT_AAC_TYPE_AACELD)) {
                printf("[func]:%s [line]:%d [info]:%s\n",
                       __FUNCTION__, __LINE__, "OT_AACLD or OT_AACELD not support OT_AAC_TRANSPORT_TYPE_ADTS");
                return OT_ERR_AENC_ILLEGAL_PARAM;
            }
    }

    return TD_SUCCESS;
}

static td_s32 aenc_get_aaclc_bitrate(const ot_aacenc_config *config, td_s32 *min_rate,
                                     td_s32 *max_rate, td_s32 *recommemd_rate)
{
    td_bool chn_single = (config->num_channels_in == AACENC_CHANNEL_SINGLE) ? TD_TRUE : TD_FALSE;

    if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_32000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_192K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_128K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_44100) {
        *min_rate = AACENC_BITRATE_48K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_265K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_48000) {
        *min_rate = AACENC_BITRATE_48K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_288K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_16000) {
        *min_rate = AACENC_BITRATE_24K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *recommemd_rate = AACENC_BITRATE_48K;
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_8000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_24K, AACENC_BITRATE_32K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_24000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_144K, AACENC_BITRATE_288K);
        *recommemd_rate = AACENC_BITRATE_48K;
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_22050) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_132K, AACENC_BITRATE_265K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_48K);
    } else {
        printf("OT_AACLC invalid samplerate(%d)\n", config->sample_rate);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_check_aaclc_config(const ot_aacenc_config *config)
{
    td_s32 min_bit_rate = 0;
    td_s32 max_bit_rate = 0;
    td_s32 recommemd_rate = 0;
    td_s32 ret;

    if (config->coder_format == OT_AACLC) {
        if (config->num_channels_out != config->num_channels_in) {
            printf("OT_AACLC num_channels_out(%d) in not equal to num_channels_in(%d)\n",
                config->num_channels_out, config->num_channels_in);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_aaclc_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        if ((config->bit_rate < min_bit_rate) || (config->bit_rate > max_bit_rate)) {
            printf("OT_AACLC %d Hz bit_rate(%d) should be %d ~ %d, recommed %d\n",
                config->sample_rate, config->bit_rate, min_bit_rate, max_bit_rate, recommemd_rate);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_get_eaac_bitrate(const ot_aacenc_config *config, td_s32 *min_rate,
                                    td_s32 *max_rate, td_s32 *recommemd_rate)
{
    td_bool chn_single = (config->num_channels_in == AACENC_CHANNEL_SINGLE) ? TD_TRUE : TD_FALSE;

    if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_32000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_44100) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_48000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_16000) {
        *min_rate = AACENC_BITRATE_24K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
        *recommemd_rate = AACENC_BITRATE_32K;
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_22050) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_24000) {
        *min_rate = AACENC_BITRATE_32K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
    } else {
        printf("OT_EAAC invalid samplerate(%d)\n", config->sample_rate);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_check_eaac_config(const ot_aacenc_config *config)
{
    td_s32 min_bit_rate = 0;
    td_s32 max_bit_rate = 0;
    td_s32 recommemd_rate = 0;
    td_s32 ret;

    if (config->coder_format == OT_EAAC) {
        if (config->num_channels_out != config->num_channels_in) {
            printf("OT_EAAC num_channels_out(%d) is not equal to num_channels_in(%d)\n",
                config->num_channels_out, config->num_channels_in);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_eaac_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        if ((config->bit_rate < min_bit_rate) || (config->bit_rate > max_bit_rate)) {
            printf("OT_EAAC %d Hz bit_rate(%d) should be %d ~ %d, recommed %d\n",
                config->sample_rate, config->bit_rate, min_bit_rate, max_bit_rate, recommemd_rate);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_get_eaacplus_bitrate(const ot_aacenc_config *config, td_s32 *min_rate,
                                        td_s32 *max_rate, td_s32 *recommemd_rate)
{
    if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_32000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_32K;
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_44100) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_48K;
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_48000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_48K;
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_16000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_48K;
        *recommemd_rate = AACENC_BITRATE_32K;
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_22050) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_32K;
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_24000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = AACENC_BITRATE_64K;
        *recommemd_rate = AACENC_BITRATE_32K;
    } else {
        printf("OT_EAACPLUS invalid samplerate(%d)\n", config->sample_rate);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_check_eaacplus_config(const ot_aacenc_config *config)
{
    td_s32 min_bit_rate = 0;
    td_s32 max_bit_rate = 0;
    td_s32 recommemd_rate = 0;
    td_s32 ret;

    if (config->coder_format == OT_EAACPLUS) {
        if ((config->num_channels_out != AACENC_CHANNEL_STEREO) ||
            (config->num_channels_in != AACENC_CHANNEL_STEREO)) {
            printf("OT_EAACPLUS num_channels_out(%d) and num_channels_in(%d) should be 2\n",
                config->num_channels_out, config->num_channels_in);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_eaacplus_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        if ((config->bit_rate < min_bit_rate) || (config->bit_rate > max_bit_rate)) {
            printf("OT_EAACPLUS %d Hz bit_rate(%d) should be %d ~ %d, recommed %d\n",
                config->sample_rate, config->bit_rate, min_bit_rate, max_bit_rate, recommemd_rate);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_get_aacld_bitrate(const ot_aacenc_config *config, td_s32 *min_rate,
                                     td_s32 *max_rate, td_s32 *recommemd_rate)
{
    td_bool chn_single = (config->num_channels_in == AACENC_CHANNEL_SINGLE) ? TD_TRUE : TD_FALSE;

    if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_32000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_64K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_44100) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_44K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_128K, AACENC_BITRATE_256K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_48000) {
        *min_rate = AACENC_BITRATE_64K;
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_128K, AACENC_BITRATE_256K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_16000) {
        *min_rate = (chn_single) ? AACENC_BITRATE_24K : AACENC_BITRATE_32K;
        *max_rate = (chn_single) ? AACENC_BITRATE_192K : AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_8000) {
        *min_rate = AACENC_BITRATE_16K;
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_24K, AACENC_BITRATE_48K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_24000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_48K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_22050) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_48K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
    } else {
        printf("OT_AACLD invalid samplerate(%d)\n", config->sample_rate);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_check_aacld_config(const ot_aacenc_config *config)
{
    td_s32 min_bit_rate = 0;
    td_s32 max_bit_rate = 0;
    td_s32 recommemd_rate = 0;
    td_s32 ret;

    if (config->coder_format == OT_AACLD) {
        if (config->num_channels_out != config->num_channels_in) {
            printf("OT_AACLD num_channels_out(%d) in not equal to num_channels_in(%d)\n",
                config->num_channels_out, config->num_channels_in);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_aacld_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        if ((config->bit_rate < min_bit_rate) || (config->bit_rate > max_bit_rate)) {
            printf("OT_AACLD %d Hz bit_rate(%d) should be %d ~ %d, recommed %d\n",
                config->sample_rate, config->bit_rate, min_bit_rate, max_bit_rate, recommemd_rate);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_get_aaceld_bitrate(const ot_aacenc_config *config, td_s32 *min_rate,
                                      td_s32 *max_rate, td_s32 *recommemd_rate)
{
    td_bool chn_single = (config->num_channels_in == AACENC_CHANNEL_SINGLE) ? TD_TRUE : TD_FALSE;

    if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_32000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_64K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_44100) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_128K, AACENC_BITRATE_256K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_48000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *max_rate = AACENC_BITRATE_320K;
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_128K, AACENC_BITRATE_256K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_16000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_16K, AACENC_BITRATE_32K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_8000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_64K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_96K, AACENC_BITRATE_192K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_32K, AACENC_BITRATE_64K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_24000) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_24K, AACENC_BITRATE_32K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_64K, AACENC_BITRATE_128K);
    } else if (config->sample_rate == OT_AUDIO_SAMPLE_RATE_22050) {
        *min_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_24K, AACENC_BITRATE_32K);
        *max_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_256K, AACENC_BITRATE_320K);
        *recommemd_rate = aac_bitrate_sel(chn_single, AACENC_BITRATE_48K, AACENC_BITRATE_96K);
    } else {
        printf("OT_AACELD invalid samplerate(%d)\n", config->sample_rate);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_check_aaceld_config(const ot_aacenc_config *config)
{
    td_s32 min_bit_rate = 0;
    td_s32 max_bit_rate = 0;
    td_s32 recommemd_rate = 0;
    td_s32 ret;

    if (config->coder_format == OT_AACELD) {
        if (config->num_channels_out != config->num_channels_in) {
            printf("OT_AACELD num_channels_out(%d) in not equal to num_channels_in(%d)\n",
                config->num_channels_out, config->num_channels_in);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }

        ret = aenc_get_aaceld_bitrate(config, &min_bit_rate, &max_bit_rate, &recommemd_rate);
        if (ret != TD_SUCCESS) {
            return ret;
        }

        if ((config->bit_rate < min_bit_rate) || (config->bit_rate > max_bit_rate)) {
            printf("AACELD %d Hz bit_rate(%d) should be %d ~ %d, recommed %d\n",
                config->sample_rate, config->bit_rate, min_bit_rate, max_bit_rate, recommemd_rate);
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
    } else {
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

static td_s32 aenc_aac_check_coder_format(ot_aacenc_format coder_format)
{
    if (coder_format != OT_AACLC && coder_format != OT_EAAC &&
        coder_format != OT_EAACPLUS && coder_format != OT_AACLD &&
        coder_format != OT_AACELD) {
        printf("aacenc coder_format(%d) invalid\n", coder_format);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 aenc_aac_check_config(const ot_aacenc_config *config)
{
    td_s32 ret = 0;

    if (config == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "config is null");
        return OT_ERR_AENC_NULL_PTR;
    }

    if (aenc_aac_check_coder_format(config->coder_format) != TD_SUCCESS) {
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (config->quality != OT_AACENC_QUALITY_EXCELLENT && config->quality != OT_AACENC_QUALITY_HIGH &&
        config->quality != OT_AACENC_QUALITY_MEDIUM && config->quality != OT_AACENC_QUALITY_LOW) {
        printf("aacenc quality(%d) invalid\n", config->quality);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (config->bits_per_sample != 16) { /* 16: 16bit */
        printf("aacenc bits_per_sample(%d) should be 16\n", config->bits_per_sample);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if ((config->trans_type < 0) || (config->trans_type > OT_AACENC_LATM_MCP1)) {
        printf("invalid trans_type(%d), not in [0, 2]\n", config->trans_type);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if ((config->band_width != 0) && ((config->band_width < 1000) || /* 1000: band width */
        (config->band_width > config->sample_rate / 2))) { /* 2: half */
        printf("AAC band_width(%d) should be 0, or 1000 ~ %d\n",
            config->band_width, config->sample_rate / 2); /* 2: half */
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    if (config->coder_format == OT_AACLC) {
        ret = aenc_check_aaclc_config(config);
    } else if (config->coder_format == OT_EAAC) {
        ret = aenc_check_eaac_config(config);
    } else if (config->coder_format == OT_EAACPLUS) {
        ret = aenc_check_eaacplus_config(config);
    } else if (config->coder_format == OT_AACLD) {
        ret = aenc_check_aacld_config(config);
    } else if (config->coder_format == OT_AACELD) {
        ret = aenc_check_aaceld_config(config);
    }

    return ret;
}

static td_void aenc_aac_init_config(const ot_aenc_attr_aac *attr, ot_aacenc_config *config)
{
    config->coder_format   = (ot_aacenc_format)attr->aac_type;
    config->bit_rate       = attr->bit_rate;
    config->bits_per_sample = 8 * (1 << ((td_u32)attr->bit_width)); /* 8: 8bit */
    config->sample_rate    = attr->sample_rate;
    config->band_width     = attr->band_width;
    config->trans_type     = (ot_aacenc_transport_type)attr->transport_type;

    if ((attr->snd_mode == OT_AUDIO_SOUND_MODE_MONO) && (attr->aac_type != OT_AAC_TYPE_EAACPLUS)) {
        config->num_channels_in   = AACENC_CHANNEL_SINGLE;
        config->num_channels_out  = AACENC_CHANNEL_SINGLE;
    } else {
        config->num_channels_in   = AACENC_CHANNEL_STEREO;
        config->num_channels_out  = AACENC_CHANNEL_STEREO;
    }

    config->quality = OT_AACENC_QUALITY_HIGH;
}

#ifdef DUMP_AACENC
static td_void init_encoder_dump_file(const ot_aacenc_config *config)
{
    td_s32 ret;
    td_char name_in[DUMP_PATH_NAME_MAX_BYTES] = {'\0'};
    td_char name_out[DUMP_PATH_NAME_MAX_BYTES] = {'\0'};

    ret = snprintf_s(name_in, DUMP_PATH_NAME_MAX_BYTES, DUMP_PATH_NAME_MAX_BYTES - 1,
        "aacenc_sin_t%d_t%d.raw", config->coder_format, config->trans_type);
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get encoder in dump name failed");
        return;
    }

    ret = snprintf_s(name_out, DUMP_PATH_NAME_MAX_BYTES, DUMP_PATH_NAME_MAX_BYTES - 1,
        "aacenc_sout_t%d_t%d.aac", config->coder_format, config->trans_type);
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get encoder in dump name failed");
        return;
    }

    g_in_enc_fd = fopen(name_in, "w+");
    if (g_in_enc_fd == TD_NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "open encoder in dump name failed");
    }

    g_out_enc_fd = fopen(name_out, "w+");
    if (g_out_enc_fd == TD_NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "open encoder out dump name failed");
    }

    g_cnt_aenc = DUMP_MAX_TIMES;
}
#endif

static td_s32 open_aac_encoder_check_and_init(ot_aenc_attr_aac *attr, ot_aacenc_config *config)
{
    td_s32 ret;

    /* check attribute of encoder */
    ret = aenc_check_aac_attr(attr);
    if (ret) {
        return ret;
    }

    /* set default config to encoder */
    ret = aac_init_default_config_adp(config);
    if (ret) {
        printf("[func]:%s [line]:%d ret:0x%x.#########\n", __FUNCTION__, __LINE__, ret);
        return ret;
    }

    aenc_aac_init_config(attr, config);

    ret = aenc_aac_check_config(config);
    if (ret) {
        printf("[func]:%s [line]:%d ret:0x%x.#########\n", __FUNCTION__, __LINE__, ret);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    return TD_SUCCESS;
}

td_s32 open_aac_encoder(td_void *encoder_attr, td_void **pp_encoder)
{
    aenc_aac_encoder *encoder = NULL;
    ot_aenc_attr_aac *attr = (ot_aenc_attr_aac *)encoder_attr;
    td_s32 ret;
    ot_aacenc_config config = {0};

    aac_check_false_return(encoder_attr != NULL);
    aac_check_false_return(pp_encoder != NULL);

    ret = open_aac_encoder_check_and_init(attr, &config);
    if (ret != TD_SUCCESS) {
        return ret;
    }

    /* allocate memory for encoder */
    encoder = (aenc_aac_encoder *)malloc(sizeof(aenc_aac_encoder));
    if (encoder == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "no memory");
        return OT_ERR_AENC_NO_MEM;
    }
    ret = memset_s(encoder, sizeof(aenc_aac_encoder), 0, sizeof(aenc_aac_encoder));
    if (ret != EOK) {
        free(encoder);
        printf("[func]:%s [line]:%d memset_s fail, ret:0x%x.#########\n", __FUNCTION__, __LINE__, ret);
        return OT_ERR_AENC_NOT_PERM;
    }

    *pp_encoder = (td_void *)encoder;

    /* create encoder */
    ret = aac_encoder_open_adp(&encoder->aac_state, &config);
    if (ret) {
        free(encoder);
        printf("[func]:%s [line]:%d ret:0x%x.#########\n", __FUNCTION__, __LINE__, ret);
        return ret;
    }

    ret = memcpy_s(&encoder->aac_attr, sizeof(encoder->aac_attr), attr, sizeof(*attr));
    if (ret != EOK) {
        aac_encoder_close_adp(encoder->aac_state);
        free(encoder);
        printf("[func]:%s [line]:%d memcpy_s ret:0x%x.#########\n", __FUNCTION__, __LINE__, ret);
        return OT_ERR_AENC_NOT_PERM;
    }

#ifdef DUMP_AACENC
    init_encoder_dump_file(&config);
#endif

    return TD_SUCCESS;
}

static td_s32 encode_frm_check_and_init(aenc_aac_encoder *aac_encoder, const ot_audio_frame *frame,
    td_s16 *in_data, td_s16 in_len)
{
    td_s32 i;
    td_u32 pt_nums, water_line;

    if (aac_encoder->aac_attr.snd_mode == OT_AUDIO_SOUND_MODE_STEREO) {
        /* whether the sound mode of frame and channel is match */
        if (frame->snd_mode != OT_AUDIO_SOUND_MODE_STEREO) {
            printf("[func]:%s [line]:%d [info]:%s\n",
                   __FUNCTION__, __LINE__, "AAC encode receive a frame which not match its soundmode");
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
    }

    /* water_line, equals to the frame sample frame of protocol */
    if (aac_encoder->aac_attr.aac_type == OT_AAC_TYPE_AACLC) {
        water_line = AACLC_SAMPLES_PER_FRAME;
    } else if ((aac_encoder->aac_attr.aac_type == OT_AAC_TYPE_EAAC) ||
        (aac_encoder->aac_attr.aac_type == OT_AAC_TYPE_EAACPLUS)) {
        water_line = AACPLUS_SAMPLES_PER_FRAME;
    } else if ((aac_encoder->aac_attr.aac_type == OT_AAC_TYPE_AACLD) ||
        (aac_encoder->aac_attr.aac_type == OT_AAC_TYPE_AACELD)) {
        water_line = AACLD_SAMPLES_PER_FRAME;
    } else {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "invalid AAC coder type");
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    /* calculate point number */
    pt_nums = frame->len / (frame->bit_width + 1);

    /* if frame sample larger than protocol sample, reject to receive, or buffer will be overflow */
    if (pt_nums != water_line) {
        printf("[func]:%s [line]:%d [info]:invalid pt_nums%d for aac_type:%d\n",
            __FUNCTION__, __LINE__, pt_nums, aac_encoder->aac_attr.aac_type);
        return OT_ERR_AENC_ILLEGAL_PARAM;
    }

    /*
     * AAC encoder need interleaved data,here change LLLRRR to LRLRLR.
     * OT_AACLC will encode 1024*2 point, and aa_cplus encode 2048*2 point
     */
    if (aac_encoder->aac_attr.snd_mode == OT_AUDIO_SOUND_MODE_STEREO) {
        if ((td_u32)(in_len / 2) < (water_line * 2)) { /* 2: 16bit */
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "invalid in_len");
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
        for (i = water_line - 1; i >= 0; i--) {
            in_data[2 * i] = *((td_s16 *)frame->virt_addr[0] + i); /* 2: stereo */
            in_data[2 * i + 1] = *((td_s16 *)frame->virt_addr[1] + i); /* 2: stereo */
        }
    } else {
        /* if inbuf is momo, copy left to right */
        if ((td_u32)(in_len / 2) < water_line) { /* 2: 16bit */
            printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "invalid in_len");
            return OT_ERR_AENC_ILLEGAL_PARAM;
        }
        for (i = water_line - 1; i >= 0; i--) {
            in_data[i] = *((td_s16 *)frame->virt_addr[0] + i);
        }
    }

    return TD_SUCCESS;
}

td_s32 encode_aac_frm(td_void *encoder, const ot_audio_frame *data,
                      td_u8 *outbuf, td_u32 *out_len)
{
    td_s32 ret;
    aenc_aac_encoder *encoder_tmp = NULL;
    td_s16 data_tmp[OT_AACENC_BLOCK_SIZE * 2 * OT_AACENC_MAX_CHANNELS]; /* 2: 16bit = 2byte */

    aac_check_false_return(encoder != NULL);
    aac_check_false_return(data != NULL);
    aac_check_false_return(outbuf != NULL);
    aac_check_false_return(out_len != NULL);

    encoder_tmp = (aenc_aac_encoder *)encoder;

    ret = encode_frm_check_and_init(encoder_tmp, data, data_tmp, sizeof(data_tmp));
    if (ret != TD_SUCCESS) {
        return ret;
    }

#ifdef DUMP_AACENC
    if ((g_cnt_aenc > 0) && (g_in_enc_fd != TD_NULL)) {
        fwrite((td_u8 *)data_tmp, 1, (encoder_tmp->aac_attr.snd_mode == OT_AUDIO_SOUND_MODE_STEREO) ?
            (data->len * 2) : data->len, g_in_enc_fd); /* 2:stereo */
    }
#endif

    ret = aac_encoder_frame_adp(encoder_tmp->aac_state, data_tmp, outbuf, (td_s32*)out_len);
    if (ret != TD_SUCCESS) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "AAC encode failed");
    }

#ifdef DUMP_AACENC
    if (g_cnt_aenc > 0) {
        if (g_out_enc_fd != TD_NULL) {
            fwrite((td_u8 *)outbuf, 1, *((td_s32*)out_len), g_out_enc_fd);
        }
        g_cnt_aenc--;
    }
#endif
    return ret;
}

td_s32 close_aac_encoder(td_void *encoder)
{
    aenc_aac_encoder *encoder_tmp = NULL;

    aac_check_false_return(encoder != NULL);
    encoder_tmp = (aenc_aac_encoder *)encoder;

    aac_encoder_close_adp(encoder_tmp->aac_state);

    free(encoder_tmp);

#ifdef DUMP_AACENC
    if (g_in_enc_fd != TD_NULL) {
        fclose(g_in_enc_fd);
        g_in_enc_fd = TD_NULL;
    }

    if (g_out_enc_fd != TD_NULL) {
        fclose(g_out_enc_fd);
        g_out_enc_fd = TD_NULL;
    }
#endif
    return TD_SUCCESS;
}

#ifdef DUMP_AACDEC
static td_void init_decoder_dump_file(const ot_adec_attr_aac *attr)
{
    td_s32 ret;
    td_char name_in[DUMP_PATH_NAME_MAX_BYTES] = {'\0'};
    td_char name_out[DUMP_PATH_NAME_MAX_BYTES] = {'\0'};
    td_char name_out_l[DUMP_PATH_NAME_MAX_BYTES] = {'\0'};

    ret = snprintf_s(name_in, DUMP_PATH_NAME_MAX_BYTES, DUMP_PATH_NAME_MAX_BYTES - 1,
        "aacdec_sin_t%d.aac", attr->transport_type);
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get decoder in dump name failed");
        return;
    }

    ret = snprintf_s(name_out, DUMP_PATH_NAME_MAX_BYTES, DUMP_PATH_NAME_MAX_BYTES - 1,
        "aacdec_sout_t%d.raw", attr->transport_type);
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get decoder out dump name failed");
        return;
    }

    ret = snprintf_s(name_out_l, DUMP_PATH_NAME_MAX_BYTES, DUMP_PATH_NAME_MAX_BYTES - 1,
        "aacdec_sout_t%d_l.raw", attr->transport_type);
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get decoder out left dump name failed");
        return;
    }

    g_in_dec_fd = fopen(name_in, "w+");
    if (g_in_dec_fd == TD_NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "open decoder in dump name failed");
    }

    g_out_dec_fd = fopen(name_out, "w+");
    if (g_out_dec_fd == TD_NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "open decoder out dump name failed");
    }

    g_out_dec_left_fd = fopen(name_out_l, "w+");
    if (g_out_dec_left_fd == TD_NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "open decoder out left dump name failed");
    }

    g_cnt_adec = DUMP_MAX_TIMES;
}

static td_void write_decoder_dump_file(td_u8 *inbuf, td_s32 in_len,
    td_u16 *outbuf, td_u32 out_len, td_u32 chns)
{
    if (g_cnt_adec > 0) {
        if (g_in_dec_fd != TD_NULL) {
            fwrite((td_u8 *)inbuf, 1, in_len, g_in_dec_fd);
        }
        if (g_out_dec_fd != TD_NULL) {
            fwrite((td_u8 *)outbuf, 1, out_len * chns, g_out_dec_fd);
        }
        if (g_out_dec_left_fd != TD_NULL) {
            fwrite((td_u8 *)outbuf, 1, out_len, g_out_dec_left_fd);
        }
        g_cnt_adec--;
    }
}
#endif

td_s32 open_aac_decoder(td_void *decoder_attr, td_void **pp_decoder)
{
    adec_aac_decoder *decoder = NULL;
    ot_adec_attr_aac *attr = NULL;
    td_s32 ret;

    aac_check_false_return(decoder_attr != NULL);
    aac_check_false_return(pp_decoder != NULL);

    attr = (ot_adec_attr_aac *)decoder_attr;

    /* allocate memory for decoder */
    decoder = (adec_aac_decoder *)malloc(sizeof(adec_aac_decoder));
    if (decoder == NULL) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "no memory");
        return OT_ERR_ADEC_NO_MEM;
    }
    ret = memset_s(decoder, sizeof(adec_aac_decoder), 0, sizeof(adec_aac_decoder));
    if (ret != EOK) {
        free(decoder);
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "memset_s fail");
        return OT_ERR_ADEC_NOT_PERM;
    }

    *pp_decoder = (td_void *)decoder;

    /* create decoder */
    decoder->aac_state = aac_init_decoder_adp((ot_aacdec_transport_type)attr->transport_type);
    if (!decoder->aac_state) {
        free(decoder);
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "aac_init_decoder failed");
        return OT_ERR_ADEC_DECODER_ERR;
    }

    ret = memcpy_s(&decoder->aac_attr, sizeof(decoder->aac_attr), attr, sizeof(*attr));
    if (ret != EOK) {
        aac_free_decoder_adp(decoder->aac_state);
        free(decoder);
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "memcpy_s failed");
        return OT_ERR_ADEC_NOT_PERM;
    }

#ifdef DUMP_AACDEC
    init_decoder_dump_file(attr);
#endif
    return TD_SUCCESS;
}

td_s32 decode_aac_frm(td_void *decoder, td_u8 **inbuf, td_s32 *left_byte,
                      td_u16 *outbuf, td_u32 *out_len, td_u32 *chns)
{
    td_s32 ret;
    adec_aac_decoder *decoder_tmp = NULL;
    td_s32 samples, frm_len, sample_bytes;
    ot_aacdec_frame_info aac_frame_info = { 0 };

    aac_check_false_return(decoder != NULL);
    aac_check_false_return(inbuf != NULL);
    aac_check_false_return(left_byte != NULL);
    aac_check_false_return(outbuf != NULL);
    aac_check_false_return(out_len != NULL);
    aac_check_false_return(chns != NULL);

    *chns = 1; /* voice encoder only one channel */

    decoder_tmp = (adec_aac_decoder *)decoder;

    frm_len = aac_decode_find_sync_header_adp(decoder_tmp->aac_state, inbuf, left_byte);
    if (frm_len < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "AAC decoder can't find sync header");
        return OT_ERR_ADEC_BUF_LACK;
    }

#ifdef DUMP_AACDEC
    td_u32 pre_len = *left_byte;
#endif

    /* notes: inbuf will updated */
    ret = aac_decode_frame_adp(decoder_tmp->aac_state, inbuf, left_byte, (td_s16 *)outbuf);
    if (ret) {
        printf("aac decoder failed!, ret:0x%x\n", ret);
        return ret;
    }

    ret = aac_get_last_frame_info_adp(decoder_tmp->aac_state, &aac_frame_info);
    if (ret != TD_SUCCESS) {
        printf("aac get last frame info failed!\n");
        return OT_ERR_ADEC_DECODER_ERR;
    }

    aac_frame_info.chn_num = ((aac_frame_info.chn_num != 0) ? aac_frame_info.chn_num : 1);
    /* samples per frame of one sound track */
    samples = aac_frame_info.output_samples / aac_frame_info.chn_num;

    if ((samples != AACLC_SAMPLES_PER_FRAME) && (samples != AACPLUS_SAMPLES_PER_FRAME) &&
        (samples != AACLD_SAMPLES_PER_FRAME)) {
        printf("aac decoder failed!\n");
        return OT_ERR_ADEC_DECODER_ERR;
    }

    sample_bytes = samples * sizeof(td_u16);
    *chns = aac_frame_info.chn_num;
    *out_len = sample_bytes;

#ifdef DUMP_AACDEC
    write_decoder_dump_file(*inbuf, pre_len - (*left_byte), outbuf, *out_len, *chns);
#endif

    return ret;
}

td_s32 get_aac_frm_info(td_void *decoder, td_void *info)
{
    td_s32 ret;
    adec_aac_decoder *decoder_tmp = NULL;
    ot_aacdec_frame_info aac_frame_info = { 0 };
    aacdec_frame_info *aac_frm = NULL;

    aac_check_false_return(decoder != NULL);
    aac_check_false_return(info != NULL);

    decoder_tmp = (adec_aac_decoder *)decoder;
    aac_frm = (aacdec_frame_info *)info;

    ret = aac_get_last_frame_info_adp(decoder_tmp->aac_state, &aac_frame_info);
    if (ret != TD_SUCCESS) {
        printf("aac_get_last_frame_info failed with ret = %d!\n", ret);
        return OT_ERR_ADEC_DECODER_ERR;
    }

    aac_frm->sample_rate = aac_frame_info.sample_rate_out;
    aac_frm->bit_rate = aac_frame_info.bit_rate;
    aac_frm->profile = aac_frame_info.profile;
    aac_frm->tns_used = aac_frame_info.tns_used;
    aac_frm->pns_used = aac_frame_info.pns_used;
    aac_frm->chn_num = aac_frame_info.chn_num;

    return TD_SUCCESS;
}


td_s32 close_aac_decoder(td_void *decoder)
{
    adec_aac_decoder *decoder_tmp = NULL;

    aac_check_false_return(decoder != NULL);
    decoder_tmp = (adec_aac_decoder *)decoder;

    aac_free_decoder_adp(decoder_tmp->aac_state);

    free(decoder_tmp);

#ifdef DUMP_AACDEC
    if (g_in_dec_fd != TD_NULL) {
        fclose(g_in_dec_fd);
        g_in_dec_fd = TD_NULL;
    }

    if (g_out_dec_fd != TD_NULL) {
        fclose(g_out_dec_fd);
        g_out_dec_fd = TD_NULL;
    }

    if (g_out_dec_left_fd != TD_NULL) {
        fclose(g_out_dec_left_fd);
        g_out_dec_left_fd = TD_NULL;
    }
#endif

    return TD_SUCCESS;
}

td_s32 reset_aac_decoder(td_void *decoder)
{
    adec_aac_decoder *decoder_tmp = NULL;

    aac_check_false_return(decoder != NULL);
    decoder_tmp = (adec_aac_decoder *)decoder;

    aac_free_decoder_adp(decoder_tmp->aac_state);

    /* create decoder */
    decoder_tmp->aac_state = aac_init_decoder_adp((ot_aacdec_transport_type)decoder_tmp->aac_attr.transport_type);
    if (!decoder_tmp->aac_state) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "aac_reset_decoder failed");
        return OT_ERR_ADEC_DECODER_ERR;
    }

    return TD_SUCCESS;
}

td_s32 ss_mpi_aenc_aac_init(td_void)
{
    td_s32 handle, ret;
    ot_aenc_encoder aac;
    ret = aac_init_enc_lib();
    if (ret) {
        return ret;
    }

    aac.type = OT_PT_AAC;
    ret = snprintf_s(aac.name, OT_MAX_ENCODER_NAME_LEN, OT_MAX_ENCODER_NAME_LEN - 1, "aac");
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get encoder name failed");
        return ret;
    }
    aac.max_frame_len = MAX_AAC_MAINBUF_SIZE;
    aac.func_open_encoder = open_aac_encoder;
    aac.func_enc_frame = encode_aac_frm;
    aac.func_close_encoder = close_aac_encoder;
    ret = ss_mpi_aenc_register_encoder(&handle, &aac);
    if (ret) {
        return ret;
    }
    g_aac_enc_handle = handle;

    return TD_SUCCESS;
}

td_s32 ss_mpi_aenc_aac_deinit(td_void)
{
    td_s32 ret;
    ret = ss_mpi_aenc_unregister_encoder(g_aac_enc_handle);
    if (ret) {
        return ret;
    }

    aac_deinit_enc_lib();

    return TD_SUCCESS;
}

td_s32 ss_mpi_adec_aac_init(td_void)
{
    td_s32 handle, ret;
    ot_adec_decoder aac;

    ret = aac_init_dec_lib();
    if (ret) {
        return ret;
    }

    aac.type = OT_PT_AAC;
    ret = snprintf_s(aac.name, OT_MAX_DECODER_NAME_LEN, OT_MAX_DECODER_NAME_LEN - 1, "aac");
    if (ret < 0) {
        printf("[func]:%s [line]:%d [info]:%s\n", __FUNCTION__, __LINE__, "get decoder name failed");
        return ret;
    }
    aac.func_open_decoder = open_aac_decoder;
    aac.func_dec_frame = decode_aac_frm;
    aac.func_get_frame_info = get_aac_frm_info;
    aac.func_close_decoder = close_aac_decoder;
    aac.func_reset_decoder = reset_aac_decoder;
    ret = ss_mpi_adec_register_decoder(&handle, &aac);
    if (ret) {
        return ret;
    }
    g_aac_dec_handle = handle;

    return TD_SUCCESS;
}

td_s32 ss_mpi_adec_aac_deinit(td_void)
{
    td_s32 ret;
    ret = ss_mpi_adec_unregister_decoder(g_aac_dec_handle);
    if (ret) {
        return ret;
    }

    aac_deinit_dec_lib();

    return TD_SUCCESS;
}
