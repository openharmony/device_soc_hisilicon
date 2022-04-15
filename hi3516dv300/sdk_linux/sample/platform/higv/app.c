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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mount.h>
#include "hi_gv.h"
#include "higv_cextfile.h"
#include "hi_gv_parser.h"
#include "higv_mw_media.h"
#include "sample_utils.h"
#include "sdk.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */

#define PROJECT_ID  LABEL_WIN     // Window Id
#define HIGV_CONTROL_PAGENUM  10  // 10 the maximum number of characters to read at one time

static HIGV_HANDLE g_sysFontHandle = INVALID_HANDLE;
static HIGV_HANDLE g_layer = INVALID_HANDLE;
static HI_S32 g_appHandle = 0;

#ifdef __HuaweiLite__
static const HI_CHAR* MOUNT_POINT = "/home";
static const HI_CHAR* SDCARD_DEV = "/dev/mmcblk0p0";
#endif

static HI_S32 WidgetAppCreateSysFont(HIGV_HANDLE* fontHandle)
{
    HI_S32 ret;
    HI_RESID sbFont, mbFont;
    HIGV_HANDLE fontHandleTmp;
    HIGV_FONT_S fontInfo = {0};
    ret = HI_GV_Res_CreateID(SBFONT_FILE, HIGV_RESTYPE_FONT, &sbFont);
    if (ret != HI_SUCCESS) {
        return ret;
    }

    ret = HI_GV_Res_CreateID(MBFONT_FILE, HIGV_RESTYPE_FONT, &mbFont);
    if (ret != HI_SUCCESS) {
        HI_GV_Res_DestroyID(sbFont);
        return ret;
    }

    fontInfo.MbFontID = mbFont;
    fontInfo.SbFontID = sbFont;
    fontInfo.Size = 20;  //  20 is font size
    fontInfo.bBold = HI_FALSE;
    fontInfo.bItalic = HI_FALSE;
    fontInfo.lineSpace = 0;
    ret = HI_GV_Font_Create((const HIGV_FONT_S*)&fontInfo, &fontHandleTmp);
    if (ret != HI_SUCCESS) {
        HI_GV_Res_DestroyID(sbFont);
        HI_GV_Res_DestroyID(mbFont);
        return ret;
    }

    *fontHandle = fontHandleTmp;
    return HI_SUCCESS;
}

static HI_S32 InitDepend(HI_VOID)
{
    HI_S32 ret;

    ret = HI_MW_DISP_Open();
    HIGV_CHECK("HI_MW_DISP_Open", ret);

    ret = HI_GV_Init();
    if (ret != HI_SUCCESS) {
        printf("HI_GV_Init failed! Return: %d\n", ret);
        return ret;
    }

    ret = HI_GV_PARSER_Init();
    if (ret != HI_SUCCESS) {
        HI_GV_Deinit();
        printf("HI_GV_PARSER_Init failed! Return: %d\n", ret);
        return ret;
    }

    HI_GV_PARSER_SetWidgetEventFunc(g_pfunHIGVAppEventFunc, sizeof(g_pfunHIGVAppEventFunc) / sizeof(HI_CHAR*));
#ifdef __HuaweiLite__
    ret = mount(SDCARD_DEV, MOUNT_POINT, "vfat", 0, 0);
    if (ret != HI_SUCCESS) {
        printf("mount failed! ret:%d \n", ret);
    }
#endif

    ret = HI_GV_PARSER_LoadFile(RES_PATH"../higv.bin");
    if (ret != HI_SUCCESS) {
        printf("HI_GV_PARSER_LoadFile failed! Return: %x  \n", ret);
        HI_GV_PARSER_Deinit();
        HI_GV_Deinit();
        return ret;
    }

    return HI_SUCCESS;
}

static HI_S32 CreateLayer(HI_VOID)
{
    HI_S32 ret;

    HIGO_LAYER_INFO_S LayerInfo = {SCREEN_WIDTH, SCREEN_HEIGHT, 320, 240, SCREEN_WIDTH, SCREEN_HEIGHT,
        (HIGO_LAYER_FLUSHTYPE_E)(HIGO_LAYER_FLUSH_OVER), HIGO_LAYER_DEFLICKER_AUTO, HIGO_PF_1555, HIGO_LAYER_HD_0};

    ret = HI_GV_Layer_Create(&LayerInfo, &g_layer);
    if (ret != HI_SUCCESS) {
        printf("HI_GV_Layer_Create failed! Return: %x  \n", ret);
        return ret;
    }

#ifndef __HuaweiLite__
    ret = HI_GV_Layer_SetRotateMode(g_layer, HIGV_ROTATE_90);
    if (ret != HI_SUCCESS) {
        printf("HI_GV_Layer_SetRotateMode failed! Return: %x  \n", ret);
        return ret;
    }
#endif

    return HI_SUCCESS;
}

static HI_S32 RegisterLanuage(HI_VOID)
{
    HI_S32 ret;

    ret = WidgetAppCreateSysFont(&g_sysFontHandle);
    if (ret != HI_SUCCESS) {
        printf("WidgetAppCreateSysFont failed!: %x\n", ret);
    }

    HI_GV_Lan_Register(RES_PATH"lan/en.lang",  g_sysFontHandle, LAN_EN);
    HI_GV_Lan_Register(RES_PATH"lan/zh.lang",  g_sysFontHandle, LAN_ZH);
    HI_GV_Lan_Change(LAN_EN);

    return HI_SUCCESS;
}

static HI_S32 LoadAndShow(HI_VOID)
{
    HI_S32 ret;

    ret = HI_GV_PARSER_LoadViewById(PROJECT_ID);
    if (ret != HI_SUCCESS) {
        printf("HI_GV_PARSER_LoadViewById failed! Return: %x\n", ret);
        return ret;
    }

    ret = HI_GV_App_Create("MainApp", (HIGV_HANDLE*)&g_appHandle);
    if (ret != HI_SUCCESS) {
        printf("HI_GV_App_Create failed! Return: %d\n", ret);
        return ret;
    }

    ret = HI_GV_Widget_Show(PROJECT_ID);
    if (ret != HI_SUCCESS) {
        printf("HI_GV_Widget_Show() failed ....: %d \n", ret);
        return ret;
    }

    ret = HI_GV_Widget_Active(PROJECT_ID);
    if (ret != HI_SUCCESS) {
        printf("HI_GV_Widget_Active() failed ....: %d \n", ret);
        return ret;
    }

    return HI_SUCCESS;
}

static void* START_HIGV_App(const void* arg)
{
    HIGV_UNUSED(arg);
    HI_S32 ret;

    ret = InitDepend();
    if (ret != HI_SUCCESS) {
        return NULL;
    }

    ret = CreateLayer();
    if (ret != HI_SUCCESS) {
        return NULL;
    }

    ret = RegisterLanuage();
    if (ret != HI_SUCCESS) {
        goto HANDLE_ERR;
    }

    ret = LoadAndShow();
    if (ret != HI_SUCCESS) {
        goto HANDLE_ERR;
    }

    HI_MW_DISP_SetDisplayGraphicCSC();

    (HI_VOID)HI_GV_App_Start(g_appHandle);

    (HI_VOID)HI_GV_App_Destroy(g_appHandle);
HANDLE_ERR:
    (HI_VOID)HI_GV_Layer_Destroy(g_layer);
    (HI_VOID)HI_GV_PARSER_Deinit();
    (HI_VOID)HI_GV_Deinit();
    (HI_VOID)HI_MW_DISP_Close();

    return NULL;
}

#ifdef __HuaweiLite__
HI_S32 app_main(HI_S32 argc, HI_CHAR* argv[])
#else
HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
#endif
{
    sdk_init();
    HIGV_UNUSED(argc);
    HIGV_UNUSED(argv);
    HI_CHAR cmd[64]; // 64 is the length of buffer
    pthread_t threadId = 0;
    HI_S32 ret = pthread_create(&threadId, NULL, START_HIGV_App, NULL);
    if (ret != HI_SUCCESS) {
        printf("pthread_create exec failed \n");
    }
    printf("\nInput CMD: 'quit' or 'q' for quit the program!\n");
    while (fgets(cmd, HIGV_CONTROL_PAGENUM, stdin) != 0) {
        cmd[10] = '\0'; // 10 is the max length of cmd buffer
        if (strncmp(cmd, "quit", 4) == 0) { // 4 is the length of cmd string
            HI_GV_App_Stop(g_appHandle);
            break;
        } else if (strncmp(cmd, "q", 1) == 0) {
            HI_GV_App_Stop(g_appHandle);
            break;
        } else {
            printf("Input CMD: 'quit' or 'q' for quit the program!\n");
        }
    }
    ret = pthread_join(threadId, NULL);
    if (ret != HI_SUCCESS) {
        printf("pthread_create exec failed \n");
    }

    sdk_exit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */
