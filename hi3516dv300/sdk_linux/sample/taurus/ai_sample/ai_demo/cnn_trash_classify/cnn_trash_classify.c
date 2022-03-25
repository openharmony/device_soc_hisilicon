/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/prctl.h>

#include "sample_comm_nnie.h"
#include "sample_media_ai.h"
#include "ai_infer_process.h"
#include "vgs_img.h"
#include "ive_img.h"
#include "posix_help.h"
#include "audio_aac_adp.h"
#include "base_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define MODEL_FILE_TRASH    "/userdata/models/cnn_trash_classify/resnet_inst.wk" // Open source model conversion
#define SCORE_MAX           4096    // The score corresponding to the maximum probability
#define DETECT_OBJ_MAX      32
#define RET_NUM_MAX         4
#define THRESH_MIN          30      // Acceptable probability threshold (over this value will be returned to the app)

#define FRM_WIDTH           256
#define FRM_HEIGHT          256

static int g_num = 108;
static int g_count = 0;
#define AUDIO_CASE_TWO     2
#define AUDIO_SCORE        40       // Confidence can be configured by yourself
#define AUDIO_FRAME        14       // Recognize once every 15 frames, can be configured by yourself

#define MULTIPLE_OF_EXPANSION 100   // Multiple of expansion
#define UNKOWN_WASTE          20    // Unkown Waste
#define BUFFER_SIZE           16    // buffer size
#define MIN_OF_BOX            16    // min of box
#define MAX_OF_BOX            240   // max of box

static HI_BOOL g_bAudioProcessStopSignal = HI_FALSE;
static pthread_t g_audioProcessThread = 0;

static SkPair g_stmChn = {
    .in = -1,
    .out = -1
};

static HI_VOID PlayAudio(const RecogNumInfo items)
{
    if  (g_count < AUDIO_FRAME) {
        g_count++;
        return;
    }

    const RecogNumInfo *item = &items;
    uint32_t score = item->score * MULTIPLE_OF_EXPANSION / SCORE_MAX;
    if ((score > AUDIO_SCORE) && (g_num != item->num)) {
        g_num = item->num;
        if (g_num != UNKOWN_WASTE) {
            AudioTest(AUDIO_CASE_TWO, g_num, -1);
        }
    }
    g_count = 0;
}

static HI_VOID* GetAudioFileName(HI_VOID* arg)
{
    RecogNumInfo resBuf = {0};
    int ret;

    while (g_bAudioProcessStopSignal == false) {
        ret = FdReadMsg(g_stmChn.in, &resBuf, sizeof(RecogNumInfo));
        if (ret == sizeof(RecogNumInfo)) {
            PlayAudio(resBuf);
        }
    }

    return NULL;
}

HI_S32 CnnTrashClassifyLoadModel(uintptr_t* model)
{
    SAMPLE_SVP_NNIE_CFG_S *self = NULL;
    HI_S32 ret;
    HI_CHAR audioThreadName[BUFFER_SIZE] = {0};

    ret = CnnCreate(&self, MODEL_FILE_TRASH);
    *model = ret < 0 ? 0 : (uintptr_t)self;
    SAMPLE_PRT("laod cnn trash classify model, ret:%d\n", ret);

    if (GetCfgBool("audio_player:support_audio", true)) {
        ret = SkPairCreate(&g_stmChn);
        HI_ASSERT(ret == 0);
        if (snprintf_s(audioThreadName, BUFFER_SIZE, BUFFER_SIZE - 1, "AudioProcess") < 0) {
            HI_ASSERT(0);
        }
        prctl(PR_SET_NAME, (unsigned long)audioThreadName, 0, 0, 0);
        ret = pthread_create(&g_audioProcessThread, NULL, GetAudioFileName, NULL);
        if (ret != 0) {
            SAMPLE_PRT("audio proccess thread creat fail:%s\n", strerror(ret));
            return ret;
        }
    }

    return ret;
}

HI_S32 CnnTrashClassifyUnloadModel(uintptr_t model)
{
    CnnDestroy((SAMPLE_SVP_NNIE_CFG_S*)model);
    SAMPLE_PRT("unload trash classify model success\n");

    if (GetCfgBool("audio_player:support_audio", true)) {
        SkPairDestroy(&g_stmChn);
        SAMPLE_PRT("SkPairDestroy success\n");
        g_bAudioProcessStopSignal = HI_TRUE;
        pthread_join(g_audioProcessThread, NULL);
        g_audioProcessThread = 0;
    }

    return HI_SUCCESS;
}

static HI_S32 CnnTrashClassifyFlag(const RecogNumInfo items[], HI_S32 itemNum)
{
    HI_CHAR *trashName = NULL;
    for (HI_U32 i = 0; i < itemNum; i++) {
        const RecogNumInfo *item = &items[i];
        uint32_t score = item->score * HI_PER_BASE / SCORE_MAX;
        if (score < THRESH_MIN) {
            break;
        }
        SAMPLE_PRT("----trash item flag----num:%d, score:%d\n", item->num, score);
        switch (item->num) {
            case 0u:
            case 1u:
            case 2u:
            case 3u:
            case 4u:
            case 5u:
                trashName = "Kitchen Waste";
                SAMPLE_PRT("----trash name----:%s\n", trashName);
                break;
            case 6u:
            case 7u:
            case 8u:
            case 9u:
                trashName = "Harmful Waste";
                SAMPLE_PRT("----trash name----:%s\n", trashName);
                break;
            case 10u:
            case 11u:
            case 12u:
            case 13u:
            case 14u:
            case 15u:
                trashName = "Recyle Waste";
                SAMPLE_PRT("----trash name----:%s\n", trashName);
                break;
            case 16u:
            case 17u:
            case 18u:
            case 19u:
                trashName = "Other Waste";
                SAMPLE_PRT("----trash name----:%s\n", trashName);
                break;
            default:
                trashName = "Unkown Waste";
                SAMPLE_PRT("----trash name----:%s\n", trashName);
                break;
        }
    }

    return HI_SUCCESS;
}

HI_S32 CnnTrashClassifyCal(uintptr_t model, VIDEO_FRAME_INFO_S *srcFrm, VIDEO_FRAME_INFO_S *resFrm)
{
    SAMPLE_PRT("begin CnnTrashClassifyCal\n");
    SAMPLE_SVP_NNIE_CFG_S *self = (SAMPLE_SVP_NNIE_CFG_S*)model; // reference to SDK sample_comm_nnie.h Line 99
    IVE_IMAGE_S img; // referece to SDK hi_comm_ive.h Line 143
    RectBox cnnBoxs[DETECT_OBJ_MAX] = {0};
    VIDEO_FRAME_INFO_S resizeFrm;  // Meet the input frame of the plug
    /*
        01-Kitchen_Watermelon_rind    02_Kitchen_Egg_shell
        03_Kitchen_Fishbone           04_Kitchen_Eggplant
        05_Kitchen_Scallion           06_Kitchen_Mushromm
        07_Hazardous_Waste_battery    08_Hazardous_Expired_cosmetrics
        09_Hazardous_Woundplast       10_Hazardous_Medical_gauze
        11_Recyclabel_Old_dolls       12_Recyclabel_Old_clip
        13_Recyclabel_Toothbrush      14_Recyclabel_Milk_box
        15_Recyclabel_Old_handbag     16_Recyclabel_Zip_top_can
        17_other_Ciggrate_end         18_Other_Bad_closestool
        19_other_Brick                20_Other_Dish
        21_unkown_waste_or_background
    */
    RecogNumInfo resBuf[RET_NUM_MAX] = {0};
    HI_S32 resLen = 0;
    HI_S32 ret;
    IVE_IMAGE_S imgIn;

    cnnBoxs[0].xmin = MIN_OF_BOX;
    cnnBoxs[0].xmax = MAX_OF_BOX;
    cnnBoxs[0].ymin = MIN_OF_BOX;
    cnnBoxs[0].ymax = MAX_OF_BOX;

    ret = MppFrmResize(srcFrm, &resizeFrm, FRM_WIDTH, FRM_HEIGHT);  // resize 256*256
    if (ret != HI_SUCCESS) {
        SAMPLE_PRT("for resize FAIL, ret=%x\n", ret);
        return ret;
    }

    ret = FrmToOrigImg(&resizeFrm, &img);
    if (ret != HI_SUCCESS) {
        SAMPLE_PRT("for Frm2Img FAIL, ret=%x\n", ret);
        return ret;
    }

    ret = ImgYuvCrop(&img, &imgIn, &cnnBoxs[0]); // Crop the image to classfication network
    if (ret < 0) {
        SAMPLE_PRT("ImgYuvCrop FAIL, ret=%x\n", ret);
        return ret;
    }
    // Follow the reasoning logic
    ret = CnnCalU8c1Img(self, &imgIn, resBuf, sizeof(resBuf) / sizeof((resBuf)[0]), &resLen);
    if (ret < 0) {
        SAMPLE_PRT("cnn cal FAIL, ret=%x\n", ret);
        return ret;
    }
    HI_ASSERT(resLen <= sizeof(resBuf) / sizeof(resBuf[0]));
    ret = CnnTrashClassifyFlag(resBuf, resLen);
    if (ret < 0) {
        SAMPLE_PRT("CnnTrashClassifyFlag cal FAIL, ret=%x\n", ret);
        return ret;
    }

    if (GetCfgBool("audio_player:support_audio", true)) {
        if (FdWriteMsg(g_stmChn.out, &resBuf[0], sizeof(RecogNumInfo)) != sizeof(RecogNumInfo)) {
            SAMPLE_PRT("FdWriteMsg FAIL\n");
        }
    }

    IveImgDestroy(&imgIn);
    MppFrmDestroy(&resizeFrm);

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
