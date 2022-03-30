/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/select.h>
#include <errno.h>

#include "sample_comm.h"
#include "acodec.h"
#include "audio_aac_adp.h"
#include "hi_osal_user.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ACODEC_FILE "/dev/acodec"

#define AUDIO_ADPCM_TYPE ADPCM_TYPE_DVI4 /* ADPCM_TYPE_IMA, ADPCM_TYPE_DVI4 */
#define G726_BPS MEDIA_G726_40K          /* MEDIA_G726_16K, MEDIA_G726_24K ... */
#define HI_ACODEC_TYPE_INNER 1           /* ACODEC_TYPE_INNER */ 

#define AUDIO_MICIN_GAIN_OPEN 0 /* should be 1 when micin */

static AAC_TYPE_E gs_enAacType = AAC_TYPE_AACLC;
static AAC_BPS_E gs_enAacBps = AAC_BPS_128K;
static AAC_TRANS_TYPE_E gs_enAacTransType = AAC_TRANS_TYPE_ADTS;

typedef struct tagSAMPLE_AENC_S {
    HI_BOOL bStart;
    pthread_t stAencPid;
    HI_S32 AeChn;
    HI_S32 AdChn;
    FILE *pfd;
    HI_BOOL bSendAdChn;
} SAMPLE_AENC_S;

typedef struct tagSAMPLE_AI_S {
    HI_BOOL bStart;
    HI_S32 AiDev;
    HI_S32 AiChn;
    HI_S32 AencChn;
    HI_S32 AoDev;
    HI_S32 AoChn;
    HI_BOOL bSendAenc;
    HI_BOOL bSendAo;
    pthread_t stAiPid;
} SAMPLE_AI_S;

typedef struct tagSAMPLE_ADEC_S {
    HI_BOOL bStart;
    HI_S32 AdChn;
    FILE *pfd;
    pthread_t stAdPid;
} SAMPLE_ADEC_S;

typedef struct tagSAMPLE_AO_S {
    AUDIO_DEV AoDev;
    HI_BOOL bStart;
    pthread_t stAoPid;
} SAMPLE_AO_S;

static SAMPLE_AI_S gs_stSampleAi[AI_DEV_MAX_NUM * AI_MAX_CHN_NUM];
static SAMPLE_AENC_S gs_stSampleAenc[AENC_MAX_CHN_NUM];
static SAMPLE_ADEC_S gs_stSampleAdec[ADEC_MAX_CHN_NUM];
static SAMPLE_AO_S gs_stSampleAo[AO_DEV_MAX_NUM];

static hi_s32 acodec_get_i2s_fs_sel(AUDIO_SAMPLE_RATE_E sample_rate, ACODEC_FS_E *i2s_fs_sel)
{
    switch (sample_rate) {
        case AUDIO_SAMPLE_RATE_8000:
            *i2s_fs_sel = ACODEC_FS_8000;
            break;

        case AUDIO_SAMPLE_RATE_16000:
            *i2s_fs_sel = ACODEC_FS_16000;
            break;

        case AUDIO_SAMPLE_RATE_32000:
            *i2s_fs_sel = ACODEC_FS_32000;
            break;

        case AUDIO_SAMPLE_RATE_11025:
            *i2s_fs_sel = ACODEC_FS_11025;
            break;

        case AUDIO_SAMPLE_RATE_22050:
            *i2s_fs_sel = ACODEC_FS_22050;
            break;

        case AUDIO_SAMPLE_RATE_44100:
            *i2s_fs_sel = ACODEC_FS_44100;
            break;

        case AUDIO_SAMPLE_RATE_12000:
            *i2s_fs_sel = ACODEC_FS_12000;
            break;

        case AUDIO_SAMPLE_RATE_24000:
            *i2s_fs_sel = ACODEC_FS_24000;
            break;

        case AUDIO_SAMPLE_RATE_48000:
            *i2s_fs_sel = ACODEC_FS_48000;
            break;

        case AUDIO_SAMPLE_RATE_64000:
            *i2s_fs_sel = ACODEC_FS_64000;
            break;

        case AUDIO_SAMPLE_RATE_96000:
            *i2s_fs_sel = ACODEC_FS_96000;
            break;

        default:
            printf("%s: not support sample_rate:%d\n", __FUNCTION__, sample_rate);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 SAMPLE_INNER_CODEC_CfgAudio(AUDIO_SAMPLE_RATE_E enSample)
{
    HI_S32 fdAcodec = -1;
    HI_S32 ret = HI_SUCCESS;
    ACODEC_FS_E i2s_fs_sel = 0;
    ACODEC_MIXER_E input_mode;

    fdAcodec = OSAL_OPEN(ACODEC_FILE, O_RDWR);
    if (fdAcodec < 0) {
        printf("%s: can't open Acodec,%s\n", __FUNCTION__, ACODEC_FILE);
        return HI_FAILURE;
    }
    if (OSAL_IOCTL(fdAcodec, ACODEC_SOFT_RESET_CTRL)) {
        printf("Reset audio codec error\n");
    }

    if (acodec_get_i2s_fs_sel(enSample, &i2s_fs_sel) != HI_SUCCESS) {
        ret = HI_FAILURE;
        goto exit;
    }

    if (OSAL_IOCTL(fdAcodec, ACODEC_SET_I2S1_FS, &i2s_fs_sel)) {
        printf("%s: set acodec sample rate failed\n", __FUNCTION__);
        ret = HI_FAILURE;
        goto exit;
    }

    input_mode = ACODEC_MIXER_IN1;
    if (OSAL_IOCTL(fdAcodec, ACODEC_SET_MIXER_MIC, &input_mode)) {
        printf("%s: select acodec input_mode failed\n", __FUNCTION__);
        ret = HI_FAILURE;
        goto exit;
    }

    /*
     * The input volume range is [-87, +86]. Both the analog gain and digital gain are adjusted.
     * A larger value indicates higher volume.
     * For example, the value 86 indicates the maximum volume of 86 dB,
     * and the value -87 indicates the minimum volume (muted status).
     * The volume adjustment takes effect simultaneously in the audio-left and audio-right channels.
     * The recommended volume range is [+10, +56].
     * Within this range, the noises are lowest because only the analog gain is adjusted,
     * and the voice quality can be guaranteed.
     */
    int iAcodecInputVol = 50; /* 50: mic gain */
    if (OSAL_IOCTL(fdAcodec, ACODEC_SET_INPUT_VOL, &iAcodecInputVol)) {
        printf("%s: set acodec micin volume failed\n", __FUNCTION__);
        ret = HI_FAILURE;
        goto exit;
    }

    int iAcodecOutputVol = 0;
    if (OSAL_IOCTL(fdAcodec, ACODEC_SET_OUTPUT_VOL, &iAcodecOutputVol)) {
        printf("%s: set acodec micin volume failed\n", __FUNCTION__);
        ret = HI_FAILURE;
        goto exit;
    }

exit:
    OSAL_CLOSE(fdAcodec);
    return ret;
}

/* config codec */
HI_S32 SAMPLE_COMM_AUDIO_CfgAcodec(const AIO_ATTR_S *pstAioAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bCodecCfg;

    CHECK_NULL_PTR(pstAioAttr);

#ifdef HI_ACODEC_TYPE_INNER
    /* ** INNER AUDIO CODEC ** */
    s32Ret = SAMPLE_INNER_CODEC_CfgAudio(pstAioAttr->enSamplerate);
    if (s32Ret != HI_SUCCESS) {
        printf("%s:SAMPLE_INNER_CODEC_CfgAudio failed\n", __FUNCTION__);
        return s32Ret;
    }
    bCodecCfg = HI_TRUE;
#else
    bCodecCfg = HI_FALSE;
#endif

    if (!bCodecCfg) {
        printf("Can not find the right codec.\n");
        return HI_FAILURE;
    }
    return s32Ret;
}

static hi_s32 audio_ai_get_frame_and_send(const SAMPLE_AI_S *ai_ctl)
{
    hi_s32 ret;
    AUDIO_FRAME_S frame = { 0 };
    AEC_FRAME_S aec_frame = { 0 };

    /* get frame from ai chn */
    ret = HI_MPI_AI_GetFrame(ai_ctl->AiDev, ai_ctl->AiChn, &frame, &aec_frame, HI_FALSE);
    if (ret != HI_SUCCESS) {
        return HI_SUCCESS; /* continue get frame */
    }

    /* send frame to encoder */
    if (ai_ctl->bSendAenc == HI_TRUE) {
        ret = HI_MPI_AENC_SendFrame(ai_ctl->AencChn, &frame, &aec_frame);
        if (ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AENC_SendFrame(%d), failed with %#x!\n", __FUNCTION__, ai_ctl->AencChn, ret);
            HI_MPI_AI_ReleaseFrame(ai_ctl->AiDev, ai_ctl->AiChn, &frame, &aec_frame);
            return ret;
        }
    }

    /* send frame to ao */
    if (ai_ctl->bSendAo == HI_TRUE) {
        ret = HI_MPI_AO_SendFrame(ai_ctl->AoDev, ai_ctl->AoChn, &frame, 1000); /* 1000: 1000ms */
        if (ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AO_SendFrame(%d, %d), failed with %#x!\n", __FUNCTION__, ai_ctl->AoDev,
                ai_ctl->AoChn, ret);
            HI_MPI_AI_ReleaseFrame(ai_ctl->AiDev, ai_ctl->AiChn, &frame, &aec_frame);
            return ret;
        }
    }

    /* finally you must release the stream */
    ret = HI_MPI_AI_ReleaseFrame(ai_ctl->AiDev, ai_ctl->AiChn, &frame, &aec_frame);
    if (ret != HI_SUCCESS) {
        printf("%s: HI_MPI_AI_ReleaseFrame(%d, %d), failed with %#x!\n", __FUNCTION__,
            ai_ctl->AiDev, ai_ctl->AiChn, ret);
        return ret;
    }

    return HI_SUCCESS;
}

/* function : get frame from Ai, send it to Aenc or Ao */
static void *SAMPLE_COMM_AUDIO_AiProc(void *parg)
{
    HI_S32 s32Ret;
    HI_S32 AiFd;
    SAMPLE_AI_S *pstAiCtl = (SAMPLE_AI_S *)parg;
    fd_set read_fds;
    struct timeval TimeoutVal;
    AI_CHN_PARAM_S stAiChnPara;

    s32Ret = HI_MPI_AI_GetChnParam(pstAiCtl->AiDev, pstAiCtl->AiChn, &stAiChnPara);
    if (s32Ret != HI_SUCCESS) {
        printf("%s: Get ai chn param failed\n", __FUNCTION__);
        return NULL;
    }

    stAiChnPara.u32UsrFrmDepth = 30; /* 30: frame depth */

    s32Ret = HI_MPI_AI_SetChnParam(pstAiCtl->AiDev, pstAiCtl->AiChn, &stAiChnPara);
    if (s32Ret != HI_SUCCESS) {
        printf("%s: set ai chn param failed\n", __FUNCTION__);
        return NULL;
    }

    FD_ZERO(&read_fds);
    AiFd = HI_MPI_AI_GetFd(pstAiCtl->AiDev, pstAiCtl->AiChn);
    if (AiFd < 0) {
        printf("%s: get AI fd failed\n", __FUNCTION__);
        return NULL;
    }

    FD_SET(AiFd, &read_fds);

    while (pstAiCtl->bStart) {
        TimeoutVal.tv_sec = 1;
        TimeoutVal.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(AiFd, &read_fds);

        s32Ret = select(AiFd + 1, &read_fds, NULL, NULL, &TimeoutVal);
        if (s32Ret < 0) {
            break;
        } else if (s32Ret == 0) {
            printf("%s: get ai frame select time out\n", __FUNCTION__);
            break;
        }

        if (FD_ISSET(AiFd, &read_fds)) {
            if (audio_ai_get_frame_and_send(pstAiCtl) != HI_SUCCESS) {
                break;
            }
        }
    }

    pstAiCtl->bStart = HI_FALSE;
    return NULL;
}

static hi_s32 audio_aenc_get_stream_and_send(const SAMPLE_AENC_S *aenc_ctl)
{
    hi_s32 ret;
    AUDIO_STREAM_S stream = {0};

    /* get stream from aenc chn */
    ret = HI_MPI_AENC_GetStream(aenc_ctl->AeChn, &stream, HI_FALSE);
    if (ret != HI_SUCCESS) {
        printf("%s: HI_MPI_AENC_GetStream(%d), failed with %#x!\n", __FUNCTION__, aenc_ctl->AeChn, ret);
        return ret;
    }

    /* send stream to decoder and play for testing */
    if (aenc_ctl->bSendAdChn == HI_TRUE) {
        ret = HI_MPI_ADEC_SendStream(aenc_ctl->AdChn, &stream, HI_TRUE);
        if (ret != HI_SUCCESS) {
            printf("%s: SendStream(%d) failed with %#x!\n", __FUNCTION__, aenc_ctl->AdChn, ret);
            HI_MPI_AENC_ReleaseStream(aenc_ctl->AeChn, &stream);
            return ret;
        }
    }

    /* save audio stream to file */
    (HI_VOID)fwrite(stream.pStream, 1, stream.u32Len, aenc_ctl->pfd);
    (HI_VOID)fflush(aenc_ctl->pfd);

    /* finally you must release the stream */
    ret = HI_MPI_AENC_ReleaseStream(aenc_ctl->AeChn, &stream);
    if (ret != HI_SUCCESS) {
        printf("%s: ReleaseStream(%d) failed with %#x!\n", __FUNCTION__, aenc_ctl->AeChn, ret);
        return ret;
    }

    return HI_SUCCESS;
}

/* function : get stream from Aenc, send it to Adec & save it to file */
static void *SAMPLE_COMM_AUDIO_AencProc(void *parg)
{
    HI_S32 s32Ret;
    SAMPLE_AENC_S *pstAencCtl = (SAMPLE_AENC_S *)parg;
    HI_S32 AencFd = HI_MPI_AENC_GetFd(pstAencCtl->AeChn);
    fd_set read_fds;
    struct timeval TimeoutVal;

    if (AencFd < 0) {
        printf("%s: get aenc fd failed\n", __FUNCTION__);
        return NULL;
    }

    while (pstAencCtl->bStart) {
        TimeoutVal.tv_sec = 1;
        TimeoutVal.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(AencFd, &read_fds);

        s32Ret = select(AencFd + 1, &read_fds, NULL, NULL, &TimeoutVal);
        if (s32Ret < 0) {
            break;
        } else if (s32Ret == 0) {
            printf("%s: get aenc stream select time out\n", __FUNCTION__);
            break;
        }

        if (FD_ISSET(AencFd, &read_fds)) {
            if (audio_aenc_get_stream_and_send(pstAencCtl) != HI_SUCCESS) {
                break;
            }
        }
    }

    (HI_VOID)fclose(pstAencCtl->pfd);
    pstAencCtl->pfd = HI_NULL;
    pstAencCtl->bStart = HI_FALSE;
    return NULL;
}

/* function : get stream from file, and send it to Adec */
static void *SAMPLE_COMM_AUDIO_AdecProc(void *parg)
{
    HI_S32 s32Ret;
    AUDIO_STREAM_S stAudioStream;
    const HI_U32 u32Len = 640;
    HI_U32 u32ReadLen;
    HI_S32 s32AdecChn;
    HI_U8 *pu8AudioStream = NULL;
    SAMPLE_ADEC_S *pstAdecCtl = (SAMPLE_ADEC_S *)parg;
    FILE *pfd = pstAdecCtl->pfd;
    s32AdecChn = pstAdecCtl->AdChn;

    pu8AudioStream = (HI_U8 *)malloc(sizeof(HI_U8) * MAX_AUDIO_STREAM_LEN);
    if (pu8AudioStream == NULL) {
        printf("%s: malloc failed!\n", __FUNCTION__);
        (HI_VOID)fclose(pstAdecCtl->pfd);
        pstAdecCtl->pfd = HI_NULL;
        return NULL;
    }

    while (pstAdecCtl->bStart == HI_TRUE) {
        /* read from file */
        stAudioStream.pStream = pu8AudioStream;
        u32ReadLen = fread(stAudioStream.pStream, 1, u32Len, pfd);
        if (u32ReadLen <= 0) {
            s32Ret = HI_MPI_ADEC_SendEndOfStream(s32AdecChn, HI_FALSE);
            if (s32Ret != HI_SUCCESS) {
                printf("%s: HI_MPI_ADEC_SendEndOfStream failed!\n", __FUNCTION__);
            }
            (HI_VOID)fseek(pfd, 0, SEEK_SET); /* read file again */
            continue;
        }

        /* here only demo adec streaming sending mode, but pack sending mode is commended */
        stAudioStream.u32Len = u32ReadLen;
        s32Ret = HI_MPI_ADEC_SendStream(s32AdecChn, &stAudioStream, HI_TRUE);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_ADEC_SendStream(%d) failed with %#x!\n", __FUNCTION__, s32AdecChn, s32Ret);
            break;
        }
    }

    free(pu8AudioStream);
    pu8AudioStream = HI_NULL;
    (HI_VOID)fclose(pstAdecCtl->pfd);
    pstAdecCtl->pfd = HI_NULL;
    pstAdecCtl->bStart = HI_FALSE;
    return NULL;
}

/* function : set ao volume */
static HI_VOID AoSetVolumeProc(AUDIO_DEV AoDev)
{
    HI_S32 s32Ret, s32Volume;

    for (s32Volume = 0; s32Volume <= 6; s32Volume++) { /* 6: max volume */
        s32Ret = HI_MPI_AO_SetVolume(AoDev, s32Volume);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AO_SetVolume(%d), failed with %#x!\n", __FUNCTION__, AoDev, s32Ret);
        }
        printf("\rset volume %d          ", s32Volume);
        (HI_VOID)fflush(stdout);
        sleep(2); /* 2: 2s */
    }

    for (s32Volume = 5; s32Volume >= -15; s32Volume--) { /* 5, -15: volume */
        s32Ret = HI_MPI_AO_SetVolume(AoDev, s32Volume);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AO_SetVolume(%d), failed with %#x!\n", __FUNCTION__, AoDev, s32Ret);
        }
        printf("\rset volume %d          ", s32Volume);
        (HI_VOID)fflush(stdout);
        sleep(2); /* 2: 2s */
    }

    for (s32Volume = -14; s32Volume <= 0; s32Volume++) { /* -14: volume */
        s32Ret = HI_MPI_AO_SetVolume(AoDev, s32Volume);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AO_SetVolume(%d), failed with %#x!\n", __FUNCTION__, AoDev, s32Ret);
        }
        printf("\rset volume %d          ", s32Volume);
        (HI_VOID)fflush(stdout);
        sleep(2); /* 2: 2s */
    }
}

static void *SAMPLE_COMM_AUDIO_AoVolProc(void *parg)
{
    HI_S32 s32Ret;
    AUDIO_FADE_S stFade;
    SAMPLE_AO_S *pstAoCtl = (SAMPLE_AO_S *)parg;
    AUDIO_DEV AoDev = pstAoCtl->AoDev;

    while (pstAoCtl->bStart) {
        AoSetVolumeProc(AoDev);

        stFade.bFade = HI_TRUE;
        stFade.enFadeInRate = AUDIO_FADE_RATE_128;
        stFade.enFadeOutRate = AUDIO_FADE_RATE_128;

        s32Ret = HI_MPI_AO_SetMute(AoDev, HI_TRUE, &stFade);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AO_SetVolume(%d), failed with %#x!\n", __FUNCTION__, AoDev, s32Ret);
        }
        printf("\rset Ao mute            ");
        (HI_VOID)fflush(stdout);
        sleep(2); /* 2: 2s */

        s32Ret = HI_MPI_AO_SetMute(AoDev, HI_FALSE, NULL);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AO_SetVolume(%d), failed with %#x!\n", __FUNCTION__, AoDev, s32Ret);
        }
        printf("\rset Ao unmute          ");
        (HI_VOID)fflush(stdout);
        sleep(2); /* 2: 2s */
    }

    return NULL;
}

/* function : Create the thread to get frame from ai and send to ao */
HI_S32 SAMPLE_COMM_AUDIO_CreateTrdAiAo(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn)
{
    SAMPLE_AI_S *pstAi = NULL;

    pstAi = &gs_stSampleAi[AiDev * AI_MAX_CHN_NUM + AiChn];
    pstAi->bSendAenc = HI_FALSE;
    pstAi->bSendAo = HI_TRUE;
    pstAi->bStart = HI_TRUE;
    pstAi->AiDev = AiDev;
    pstAi->AiChn = AiChn;
    pstAi->AoDev = AoDev;
    pstAi->AoChn = AoChn;

    pthread_create(&pstAi->stAiPid, 0, SAMPLE_COMM_AUDIO_AiProc, pstAi);

    return HI_SUCCESS;
}

/* function : Create the thread to get frame from ai and send to aenc */
HI_S32 SAMPLE_COMM_AUDIO_CreateTrdAiAenc(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn)
{
    SAMPLE_AI_S *pstAi = NULL;

    pstAi = &gs_stSampleAi[AiDev * AI_MAX_CHN_NUM + AiChn];
    pstAi->bSendAenc = HI_TRUE;
    pstAi->bSendAo = HI_FALSE;
    pstAi->bStart = HI_TRUE;
    pstAi->AiDev = AiDev;
    pstAi->AiChn = AiChn;
    pstAi->AencChn = AeChn;
    pthread_create(&pstAi->stAiPid, 0, SAMPLE_COMM_AUDIO_AiProc, pstAi);

    return HI_SUCCESS;
}

/* function : Create the thread to get stream from aenc and send to adec */
HI_S32 SAMPLE_COMM_AUDIO_CreateTrdAencAdec(AENC_CHN AeChn, ADEC_CHN AdChn, FILE *pAecFd)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    if (pAecFd == NULL) {
        return HI_FAILURE;
    }

    pstAenc = &gs_stSampleAenc[AeChn];
    pstAenc->AeChn = AeChn;
    pstAenc->AdChn = AdChn;
    pstAenc->bSendAdChn = HI_TRUE;
    pstAenc->pfd = pAecFd;
    pstAenc->bStart = HI_TRUE;
    pthread_create(&pstAenc->stAencPid, 0, SAMPLE_COMM_AUDIO_AencProc, pstAenc);

    return HI_SUCCESS;
}

/* function : Create the thread to get stream from file and send to adec */
HI_S32 SAMPLE_COMM_AUDIO_CreateTrdFileAdec(ADEC_CHN AdChn, FILE *pAdcFd)
{
    SAMPLE_ADEC_S *pstAdec = NULL;

    if (pAdcFd == NULL) {
        return HI_FAILURE;
    }

    pstAdec = &gs_stSampleAdec[AdChn];
    pstAdec->AdChn = AdChn;
    pstAdec->pfd = pAdcFd;
    pstAdec->bStart = HI_TRUE;
    pthread_create(&pstAdec->stAdPid, 0, SAMPLE_COMM_AUDIO_AdecProc, pstAdec);

    return HI_SUCCESS;
}

/* function : Create the thread to set Ao volume */
HI_S32 SAMPLE_COMM_AUDIO_CreateTrdAoVolCtrl(AUDIO_DEV AoDev)
{
    SAMPLE_AO_S *pstAoCtl = NULL;

    pstAoCtl = &gs_stSampleAo[AoDev];
    pstAoCtl->AoDev = AoDev;
    pstAoCtl->bStart = HI_TRUE;
    pthread_create(&pstAoCtl->stAoPid, 0, SAMPLE_COMM_AUDIO_AoVolProc, pstAoCtl);

    return HI_SUCCESS;
}

/* function : Destroy the thread to get frame from ai and send to ao or aenc */
HI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAi(AUDIO_DEV AiDev, AI_CHN AiChn)
{
    SAMPLE_AI_S *pstAi = NULL;

    pstAi = &gs_stSampleAi[AiDev * AI_MAX_CHN_NUM + AiChn];
    if (pstAi->bStart) {
        pstAi->bStart = HI_FALSE;
        pthread_join(pstAi->stAiPid, 0);
    }

    return HI_SUCCESS;
}

/* function : Destroy the thread to get stream from aenc and send to adec */
HI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAencAdec(AENC_CHN AeChn)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    pstAenc = &gs_stSampleAenc[AeChn];
    if (pstAenc->bStart) {
        pstAenc->bStart = HI_FALSE;
        pthread_join(pstAenc->stAencPid, 0);
    }

    if (pstAenc->pfd != HI_NULL) {
        (HI_VOID)fclose(pstAenc->pfd);
        pstAenc->pfd = HI_NULL;
    }

    return HI_SUCCESS;
}

/* function : Destroy the thread to get stream from file and send to adec */
HI_S32 SAMPLE_COMM_AUDIO_DestroyTrdFileAdec(ADEC_CHN AdChn)
{
    SAMPLE_ADEC_S *pstAdec = NULL;

    pstAdec = &gs_stSampleAdec[AdChn];
    if (pstAdec->bStart) {
        pstAdec->bStart = HI_FALSE;
        pthread_join(pstAdec->stAdPid, 0);
    }

    if (pstAdec->pfd != HI_NULL) {
        (HI_VOID)fclose(pstAdec->pfd);
        pstAdec->pfd = HI_NULL;
    }

    return HI_SUCCESS;
}

/* function : Destroy the thread to set Ao volume */
HI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAoVolCtrl(AUDIO_DEV AoDev)
{
    SAMPLE_AO_S *pstAoCtl = NULL;

    pstAoCtl = &gs_stSampleAo[AoDev];
    if (pstAoCtl->bStart) {
        pstAoCtl->bStart = HI_FALSE;
        pthread_cancel(pstAoCtl->stAoPid);
        pthread_join(pstAoCtl->stAoPid, 0);
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_AoBindAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn)
{
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = HI_ID_ADEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = AdChn;
    stDestChn.enModId = HI_ID_AO;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;

    return HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
}

HI_S32 SAMPLE_COMM_AUDIO_AoUnbindAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn)
{
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = HI_ID_ADEC;
    stSrcChn.s32ChnId = AdChn;
    stSrcChn.s32DevId = 0;
    stDestChn.enModId = HI_ID_AO;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;

    return HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
}

HI_S32 SAMPLE_COMM_AUDIO_AoBindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn)
{
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = HI_ID_AI;
    stSrcChn.s32ChnId = AiChn;
    stSrcChn.s32DevId = AiDev;
    stDestChn.enModId = HI_ID_AO;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;

    return HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
}

HI_S32 SAMPLE_COMM_AUDIO_AoUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn)
{
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = HI_ID_AI;
    stSrcChn.s32ChnId = AiChn;
    stSrcChn.s32DevId = AiDev;
    stDestChn.enModId = HI_ID_AO;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;

    return HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
}

HI_S32 SAMPLE_COMM_AUDIO_AencBindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn)
{
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = HI_ID_AI;
    stSrcChn.s32DevId = AiDev;
    stSrcChn.s32ChnId = AiChn;
    stDestChn.enModId = HI_ID_AENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = AeChn;

    return HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
}

HI_S32 SAMPLE_COMM_AUDIO_AencUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn)
{
    MPP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = HI_ID_AI;
    stSrcChn.s32DevId = AiDev;
    stSrcChn.s32ChnId = AiChn;
    stDestChn.enModId = HI_ID_AENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = AeChn;

    return HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
}

static hi_s32 sample_comm_audio_start_ai_vqe(AUDIO_DEV ai_dev, AI_CHN ai_chn, const hi_void *ai_vqe_attr,
    hi_u32 ai_vqe_type)
{
    hi_s32 ret;
    hi_bool ai_vqe = HI_TRUE;

    switch (ai_vqe_type) {
        case 0:
            ret = HI_SUCCESS;
            ai_vqe = HI_FALSE;
            break;
        case 1:
            CHECK_NULL_PTR(ai_vqe_attr);
            ret = HI_MPI_AI_SetRecordVqeAttr(ai_dev, ai_chn, (AI_RECORDVQE_CONFIG_S *)ai_vqe_attr);
            break;
        case 3: /* 3: TalkV2 VQE */
            CHECK_NULL_PTR(ai_vqe_attr);
            ret = HI_MPI_AI_SetTalkVqeV2Attr(ai_dev, ai_chn, ai_dev, ai_chn, (AI_TALKVQEV2_CONFIG_S *)ai_vqe_attr);
            break;
        default:
            ret = HI_FAILURE;
            break;
    }

    if (ret) {
        printf("%s: SetAiVqe%d(%d,%d) failed with %#x\n", __FUNCTION__, ai_vqe_type, ai_dev, ai_chn, ret);
        return ret;
    }

    if (ai_vqe) {
        ret = HI_MPI_AI_EnableVqe(ai_dev, ai_chn);
        if (ret) {
            printf("%s: HI_MPI_AI_EnableVqe(%d,%d) failed with %#x\n", __FUNCTION__, ai_dev, ai_chn, ret);
            return ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_StartAi(AUDIO_DEV AiDevId, HI_S32 s32AiChnCnt, AIO_ATTR_S *pstAioAttr,
    AUDIO_SAMPLE_RATE_E enOutSampleRate, HI_BOOL bResampleEn, const HI_VOID *pstAiVqeAttr, HI_U32 u32AiVqeType)
{
    HI_S32 s32Ret;

    CHECK_NULL_PTR(pstAioAttr);

    s32Ret = HI_MPI_AI_SetPubAttr(AiDevId, pstAioAttr);
    if (s32Ret) {
        printf("%s: HI_MPI_AI_SetPubAttr(%d) failed with %#x\n", __FUNCTION__, AiDevId, s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_AI_Enable(AiDevId);
    if (s32Ret) {
        printf("%s: HI_MPI_AI_Enable(%d) failed with %#x\n", __FUNCTION__, AiDevId, s32Ret);
        return s32Ret;
    }

    for (hi_u32 i = 0; i < ((HI_U32)s32AiChnCnt >> (HI_U32)pstAioAttr->enSoundmode); i++) {
        s32Ret = HI_MPI_AI_EnableChn(AiDevId, i);
        if (s32Ret) {
            printf("%s: HI_MPI_AI_EnableChn(%d,%d) failed with %#x\n", __FUNCTION__, AiDevId, i, s32Ret);
            return s32Ret;
        }

        if (bResampleEn == HI_TRUE) {
            s32Ret = HI_MPI_AI_EnableReSmp(AiDevId, i, enOutSampleRate);
            if (s32Ret) {
                printf("%s: HI_MPI_AI_EnableReSmp(%d,%d) failed with %#x\n", __FUNCTION__, AiDevId, i, s32Ret);
                return s32Ret;
            }
        }

        s32Ret = sample_comm_audio_start_ai_vqe(AiDevId, i, pstAiVqeAttr, u32AiVqeType);
        if (s32Ret) {
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_StopAi(AUDIO_DEV AiDevId, HI_S32 s32AiChnCnt, HI_BOOL bResampleEn, HI_BOOL bVqeEn)
{
    HI_S32 i;
    HI_S32 s32Ret;

    for (i = 0; i < s32AiChnCnt; i++) {
        if (bResampleEn == HI_TRUE) {
            s32Ret = HI_MPI_AI_DisableReSmp(AiDevId, i);
            if (s32Ret != HI_SUCCESS) {
                printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
                return s32Ret;
            }
        }

        if (bVqeEn == HI_TRUE) {
            s32Ret = HI_MPI_AI_DisableVqe(AiDevId, i);
            if (s32Ret != HI_SUCCESS) {
                printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
                return s32Ret;
            }
        }

        s32Ret = HI_MPI_AI_DisableChn(AiDevId, i);
        if (s32Ret != HI_SUCCESS) {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
            return s32Ret;
        }
    }

    s32Ret = HI_MPI_AI_Disable(AiDevId);
    if (s32Ret != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return s32Ret;
    }

    return HI_SUCCESS;
}

#ifdef HI_ACODEC_TYPE_HDMI
HI_S32 SAMPLE_COMM_AUDIO_StartHdmi(const AIO_ATTR_S *pstAioAttr)
{
    HI_S32 s32Ret;
    HI_HDMI_ATTR_S stHdmiAttr;
    HI_HDMI_ID_E enHdmi = HI_HDMI_ID_0;
    VO_PUB_ATTR_S stPubAttr;
    const VO_DEV VoDev = 0;

    CHECK_NULL_PTR(pstAioAttr);

    stPubAttr.u32BgColor = 0x000000ff;
    stPubAttr.enIntfType = VO_INTF_HDMI;
    stPubAttr.enIntfSync = VO_OUTPUT_1080P30;

    if (SAMPLE_COMM_VO_StartDev(VoDev, &stPubAttr) != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return HI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_VO_HdmiStart(stPubAttr.enIntfSync);
    if (s32Ret != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_HDMI_Stop(enHdmi);
    if (s32Ret != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_HDMI_GetAttr(enHdmi, &stHdmiAttr);
    if (s32Ret != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return HI_FAILURE;
    }

    stHdmiAttr.bEnableAudio = HI_TRUE; /* if enable audio */
    stHdmiAttr.enSoundIntf = HI_HDMI_SND_INTERFACE_I2S;
    stHdmiAttr.enSampleRate = pstAioAttr->enSamplerate;
    stHdmiAttr.u8DownSampleParm = HI_FALSE; /* parameter of downsampling  rate of PCM audio, default :0 */
    stHdmiAttr.enBitDepth = 8 * (pstAioAttr->enBitwidth + 1); /* bitwidth of audio, default :16, 8: 1byte = 8bits */
    stHdmiAttr.u8I2SCtlVbit = 0; /* reserved, should be 0, I2S control (0x7A:0x1D) */
    stHdmiAttr.bEnableAviInfoFrame = HI_TRUE; /* if enable  AVI InfoFrame */
    stHdmiAttr.bEnableAudInfoFrame = HI_TRUE;

    s32Ret = HI_MPI_HDMI_SetAttr(enHdmi, &stHdmiAttr);
    if (s32Ret != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_HDMI_Start(enHdmi);
    if (s32Ret != HI_SUCCESS) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_StopHdmi(HI_VOID)
{
    hi_s32 ret;
    const VO_DEV vo_dev = 0;

    ret = SAMPLE_COMM_VO_HdmiStop();
    if (ret != HI_SUCCESS) {
        printf("%s: SAMPLE_COMM_VO_HdmiStop failed with %#x!\n", __FUNCTION__, ret);
        return HI_FAILURE;
    }

    ret = HI_MPI_VO_Disable(vo_dev);
    if (ret != HI_SUCCESS) {
        printf("%s: HI_MPI_VO_Disable failed with %#x!\n", __FUNCTION__, ret);
        return HI_FAILURE;
    }

    return ret;
}
#endif

HI_S32 SAMPLE_COMM_AUDIO_StartAo(AUDIO_DEV AoDevId, HI_S32 s32AoChnCnt, AIO_ATTR_S *pstAioAttr,
    AUDIO_SAMPLE_RATE_E enInSampleRate, HI_BOOL bResampleEn)
{
    hi_u32 i;
    HI_S32 s32Ret;

    CHECK_NULL_PTR(pstAioAttr);

    if (SAMPLE_AUDIO_INNER_HDMI_AO_DEV == AoDevId) {
#ifdef HI_ACODEC_TYPE_HDMI
        pstAioAttr->u32ClkSel = 0;

        SAMPLE_COMM_AUDIO_StartHdmi(pstAioAttr);
#endif
    }

    s32Ret = HI_MPI_AO_SetPubAttr(AoDevId, pstAioAttr);
    if (s32Ret != HI_SUCCESS) {
        printf("%s: HI_MPI_AO_SetPubAttr(%d) failed with %#x!\n", __FUNCTION__, AoDevId, s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_AO_Enable(AoDevId);
    if (s32Ret != HI_SUCCESS) {
        printf("%s: HI_MPI_AO_Enable(%d) failed with %#x!\n", __FUNCTION__, AoDevId, s32Ret);
        return HI_FAILURE;
    }

    for (i = 0; i < ((HI_U32)s32AoChnCnt >> (HI_U32)pstAioAttr->enSoundmode); i++) {
        s32Ret = HI_MPI_AO_EnableChn(AoDevId, i);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AO_EnableChn(%d) failed with %#x!\n", __FUNCTION__, i, s32Ret);
            return HI_FAILURE;
        }

        if (bResampleEn == HI_TRUE) {
            s32Ret = HI_MPI_AO_DisableReSmp(AoDevId, i);
            if (s32Ret != HI_SUCCESS) {
                printf("%s: HI_MPI_AO_DisableReSmp(%d,%d) failed with %#x!\n", __FUNCTION__, AoDevId, i, s32Ret);
                return HI_FAILURE;
            }

            s32Ret = HI_MPI_AO_EnableReSmp(AoDevId, i, enInSampleRate);
            if (s32Ret != HI_SUCCESS) {
                printf("%s: HI_MPI_AO_EnableReSmp(%d,%d) failed with %#x!\n", __FUNCTION__, AoDevId, i, s32Ret);
                return HI_FAILURE;
            }
        }
    }

    s32Ret = HI_MPI_AO_EnableChn(AoDevId, AO_SYSCHN_CHNID);
    if (s32Ret != HI_SUCCESS) {
        printf("%s: HI_MPI_AO_EnableChn(%d) failed with %#x!\n", __FUNCTION__, i, s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_StopAo(AUDIO_DEV AoDevId, HI_S32 s32AoChnCnt, HI_BOOL bResampleEn)
{
    HI_S32 i;
    HI_S32 s32Ret;

    for (i = 0; i < s32AoChnCnt; i++) {
        if (bResampleEn == HI_TRUE) {
            s32Ret = HI_MPI_AO_DisableReSmp(AoDevId, i);
            if (s32Ret != HI_SUCCESS) {
                printf("%s: HI_MPI_AO_DisableReSmp failed with %#x!\n", __FUNCTION__, s32Ret);
                return s32Ret;
            }
        }

        s32Ret = HI_MPI_AO_DisableChn(AoDevId, i);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AO_DisableChn failed with %#x!\n", __FUNCTION__, s32Ret);
            return s32Ret;
        }
    }

    s32Ret = HI_MPI_AO_DisableChn(AoDevId, AO_SYSCHN_CHNID);
    if (s32Ret != HI_SUCCESS) {
        printf("%s: HI_MPI_AO_DisableChn(%d) failed with %#x!\n", __FUNCTION__, i, s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_AO_Disable(AoDevId);
    if (s32Ret != HI_SUCCESS) {
        printf("%s: HI_MPI_AO_Disable failed with %#x!\n", __FUNCTION__, s32Ret);
        return s32Ret;
    }

    if (SAMPLE_AUDIO_INNER_HDMI_AO_DEV == AoDevId) {
#ifdef HI_ACODEC_TYPE_HDMI
        s32Ret = SAMPLE_COMM_AUDIO_StopHdmi();
        if (s32Ret != HI_SUCCESS) {
            printf("%s: SAMPLE_COMM_AUDIO_StopHdmi failed with %#x!\n", __FUNCTION__, s32Ret);
            return s32Ret;
        }
#endif
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_StartAenc(HI_S32 s32AencChnCnt, const AIO_ATTR_S *pstAioAttr, PAYLOAD_TYPE_E enType)
{
    AENC_CHN AeChn;
    HI_S32 s32Ret, i;
    AENC_CHN_ATTR_S stAencAttr;
    AENC_ATTR_ADPCM_S stAdpcmAenc;
    AENC_ATTR_G711_S stAencG711;
    AENC_ATTR_G726_S stAencG726;
    AENC_ATTR_LPCM_S stAencLpcm;
    AENC_ATTR_AAC_S stAencAac;

    CHECK_NULL_PTR(pstAioAttr);

    /* set AENC chn attr */
    stAencAttr.enType = enType;
    stAencAttr.u32BufSize = 30; /* 30: buf size */
    stAencAttr.u32PtNumPerFrm = pstAioAttr->u32PtNumPerFrm;

    if (PT_ADPCMA == stAencAttr.enType) {
        stAencAttr.pValue = &stAdpcmAenc;
        stAdpcmAenc.enADPCMType = AUDIO_ADPCM_TYPE;
    } else if (PT_G711A == stAencAttr.enType || PT_G711U == stAencAttr.enType) {
        stAencAttr.pValue = &stAencG711;
    } else if (PT_G726 == stAencAttr.enType) {
        stAencAttr.pValue = &stAencG726;
        stAencG726.enG726bps = G726_BPS;
    } else if (PT_LPCM == stAencAttr.enType) {
        stAencAttr.pValue = &stAencLpcm;
    } else if (PT_AAC == stAencAttr.enType) {
        stAencAttr.pValue = &stAencAac;
        stAencAac.enAACType = gs_enAacType;
        stAencAac.enBitRate = gs_enAacBps;
        stAencAac.enBitWidth = AUDIO_BIT_WIDTH_16;
        stAencAac.enSmpRate = pstAioAttr->enSamplerate;
        stAencAac.enSoundMode = pstAioAttr->enSoundmode;
        stAencAac.enTransType = gs_enAacTransType;
        stAencAac.s16BandWidth = 0;
    } else {
        printf("%s: invalid aenc payload type:%d\n", __FUNCTION__, stAencAttr.enType);
        return HI_FAILURE;
    }

    for (i = 0; i < s32AencChnCnt; i++) {
        AeChn = i;

        /* create aenc chn */
        s32Ret = HI_MPI_AENC_CreateChn(AeChn, &stAencAttr);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AENC_CreateChn(%d) failed with %#x!\n", __FUNCTION__, AeChn, s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_StopAenc(HI_S32 s32AencChnCnt)
{
    HI_S32 i;
    HI_S32 s32Ret;

    for (i = 0; i < s32AencChnCnt; i++) {
        s32Ret = HI_MPI_AENC_DestroyChn(i);
        if (s32Ret != HI_SUCCESS) {
            printf("%s: HI_MPI_AENC_DestroyChn(%d) failed with %#x!\n", __FUNCTION__, i, s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_DestroyAllTrd(void)
{
    HI_U32 u32DevId, u32ChnId;

    for (u32DevId = 0; u32DevId < AI_DEV_MAX_NUM; u32DevId++) {
        for (u32ChnId = 0; u32ChnId < AI_MAX_CHN_NUM; u32ChnId++) {
            if (SAMPLE_COMM_AUDIO_DestroyTrdAi(u32DevId, u32ChnId) != HI_SUCCESS) {
                printf("%s: SAMPLE_COMM_AUDIO_DestroyTrdAi(%d,%d) failed!\n", __FUNCTION__, u32DevId, u32ChnId);
                return HI_FAILURE;
            }
        }
    }

    for (u32ChnId = 0; u32ChnId < AENC_MAX_CHN_NUM; u32ChnId++) {
        if (SAMPLE_COMM_AUDIO_DestroyTrdAencAdec(u32ChnId) != HI_SUCCESS) {
            printf("%s: SAMPLE_COMM_AUDIO_DestroyTrdAencAdec(%d) failed!\n", __FUNCTION__, u32ChnId);
            return HI_FAILURE;
        }
    }

    for (u32ChnId = 0; u32ChnId < ADEC_MAX_CHN_NUM; u32ChnId++) {
        if (SAMPLE_COMM_AUDIO_DestroyTrdFileAdec(u32ChnId) != HI_SUCCESS) {
            printf("%s: SAMPLE_COMM_AUDIO_DestroyTrdFileAdec(%d) failed!\n", __FUNCTION__, u32ChnId);
            return HI_FAILURE;
        }
    }

    for (u32ChnId = 0; u32ChnId < AO_DEV_MAX_NUM; u32ChnId++) {
        if (SAMPLE_COMM_AUDIO_DestroyTrdAoVolCtrl(u32ChnId) != HI_SUCCESS) {
            printf("%s: SAMPLE_COMM_AUDIO_DestroyTrdAoVolCtrl(%d) failed!\n", __FUNCTION__, u32ChnId);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_StartAdec(ADEC_CHN AdChn, PAYLOAD_TYPE_E enType)
{
    HI_S32 s32Ret;
    ADEC_CHN_ATTR_S stAdecAttr;
    ADEC_ATTR_ADPCM_S stAdpcm;
    ADEC_ATTR_G711_S stAdecG711;
    ADEC_ATTR_G726_S stAdecG726;
    ADEC_ATTR_LPCM_S stAdecLpcm;
    ADEC_ATTR_AAC_S stAdecAac;

    stAdecAttr.enType = enType;
    stAdecAttr.u32BufSize = 20; /* 20: buf size */
    stAdecAttr.enMode = ADEC_MODE_STREAM; /* propose use pack mode in your app */

    if (PT_ADPCMA == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdpcm;
        stAdpcm.enADPCMType = AUDIO_ADPCM_TYPE;
    } else if (PT_G711A == stAdecAttr.enType || PT_G711U == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdecG711;
    } else if (PT_G726 == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdecG726;
        stAdecG726.enG726bps = G726_BPS;
    } else if (PT_LPCM == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdecLpcm;
        stAdecAttr.enMode = ADEC_MODE_PACK; /* lpcm must use pack mode */
    } else if (PT_AAC == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdecAac;
        stAdecAttr.enMode = ADEC_MODE_STREAM; /* aac should be stream mode */
        stAdecAac.enTransType = gs_enAacTransType;
    } else {
        printf("%s: invalid aenc payload type:%d\n", __FUNCTION__, stAdecAttr.enType);
        return HI_FAILURE;
    }

    /* create adec chn */
    s32Ret = HI_MPI_ADEC_CreateChn(AdChn, &stAdecAttr);
    if (s32Ret != HI_SUCCESS) {
        printf("%s: HI_MPI_ADEC_CreateChn(%d) failed with %#x!\n", __FUNCTION__, AdChn, s32Ret);
        return s32Ret;
    }
    return HI_SUCCESS;
}

HI_S32 SAMPLE_COMM_AUDIO_StopAdec(ADEC_CHN AdChn)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_ADEC_DestroyChn(AdChn);
    if (s32Ret != HI_SUCCESS) {
        printf("%s: HI_MPI_ADEC_DestroyChn(%d) failed with %#x!\n", __FUNCTION__, AdChn, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
