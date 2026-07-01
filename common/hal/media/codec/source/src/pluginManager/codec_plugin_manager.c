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

#include "codec_plugin_manager.h"
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <pthread.h>
#include "media_hal_common.h"

#define ELEMENT_MAX_LEN 50
#define MODULE_NAME "CODEC_PLUGIN_MGR"

typedef struct SimpleCapbility {
    ResizableArray supportMimes;
    bool isHardware;
} SimpleCapbility;


typedef void *(*GetImplFunNew)(void);

typedef int (*GetImplFun)(void *);
typedef int (*UnImplFun)(void);
typedef void (*GetCapbilityFun)(void);

static pthread_mutex_t g_vdecPluginListLock = PTHREAD_MUTEX_INITIALIZER;
static OtListHead g_vdecPliginList;

static pthread_mutex_t g_adecPluginListLock = PTHREAD_MUTEX_INITIALIZER;
static OtListHead g_adecPliginList;

static pthread_mutex_t g_aencPluginListLock = PTHREAD_MUTEX_INITIALIZER;
static OtListHead g_aencPliginList;

static pthread_mutex_t g_vencPluginListLock = PTHREAD_MUTEX_INITIALIZER;
static OtListHead g_vencPliginList;

static const char *g_codeType[INVALID_TYPE + 1] = {
    "vdec", "venc", "adec", "aenc", ".so"
};

static uint32_t CheckPluginCommomImpl(CodecPluginCommon *pluginCommon)
{
    CHK_NULL_RETURN(pluginCommon, MEDIA_HAL_INVALID_PARAM, "pluginCommon ptr is null");
    CHK_NULL_RETURN(pluginCommon->pfnCreate, MEDIA_HAL_INVALID_PARAM, "create fun ptr is null");
    CHK_NULL_RETURN(pluginCommon->pfnStart, MEDIA_HAL_INVALID_PARAM, "start fun ptr is null");
    CHK_NULL_RETURN(pluginCommon->pfnStop, MEDIA_HAL_INVALID_PARAM, "stop fun ptr is null");
    CHK_NULL_RETURN(pluginCommon->pfnReset, MEDIA_HAL_INVALID_PARAM, "reset fun ptr is null");
    CHK_NULL_RETURN(pluginCommon->pfnInvoke, MEDIA_HAL_INVALID_PARAM, "invoke fun ptr is null");
    return MEDIA_HAL_OK;
}

static td_s32 AddPluginToList(CodecType type, VoidPtr plugin, CodecPluginNode *node)
{
    switch (type) {
        case VIDEO_DECODER: {
            AvVideoDecoder *vdecPlugin = (AvVideoDecoder *)plugin;
            node->videoDecoder = *vdecPlugin;
            MEDIA_HAL_LOCK(g_vdecPluginListLock);
            OT_LIST_ADD(&node->listPtr, &g_vdecPliginList);
            MEDIA_HAL_UNLOCK(g_vdecPluginListLock);
            break;
        }
        case VIDEO_ENCODER: {
            AvVideoEncoder *vencPlugin = (AvVideoEncoder *)plugin;
            node->videoEncoder = *vencPlugin;
            MEDIA_HAL_LOCK(g_vencPluginListLock);
            OT_LIST_ADD(&node->listPtr, &g_vencPliginList);
            MEDIA_HAL_UNLOCK(g_vencPluginListLock);
            break;
        }
        case AUDIO_DECODER: {
            AvAudioDecoder *adecPlugin = (AvAudioDecoder *)plugin;
            node->audioDecoder = *adecPlugin;
            MEDIA_HAL_LOCK(g_adecPluginListLock);
            OT_LIST_ADD(&node->listPtr, &g_adecPliginList);
            MEDIA_HAL_UNLOCK(g_adecPluginListLock);
            break;
        }
        case AUDIO_ENCODER: {
            AvAudioEncoder *aencPlugin = (AvAudioEncoder *)plugin;
            node->audioEncoder = *aencPlugin;
            MEDIA_HAL_LOCK(g_aencPluginListLock);
            OT_LIST_ADD(&node->listPtr, &g_aencPliginList);
            MEDIA_HAL_UNLOCK(g_aencPluginListLock);
            break;
        }
        default: {
            MEDIA_HAL_LOGE(MODULE_NAME, "invalid type:%d", type);
            return TD_FAILURE;
        }
    }
    return MEDIA_HAL_OK;
}

static td_s32 RegisterCodecPlugin(VoidPtr plugin, VoidPtr libModule, const char *compeltePath, CodecType type)
{
    CHK_NULL_RETURN(plugin, TD_FAILURE, "plugin null");

    CodecPluginCommon *pluginCommon = (CodecPluginCommon *)plugin;
    if (CheckPluginCommomImpl(pluginCommon) != MEDIA_HAL_OK) {
        return MEDIA_HAL_ERR;
    }

    pluginCommon->module = libModule;
    if (strcpy_s(pluginCommon->pluginName,
        sizeof(pluginCommon->pluginName), compeltePath) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Copy plugin name err");
        return MEDIA_HAL_ERR;
    }

    CodecPluginNode *node = (CodecPluginNode *)malloc(sizeof(CodecPluginNode));
    if (node == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "malloc fail");
        return MEDIA_HAL_ERR;
    }
    if (memset_s(node, sizeof(CodecPluginNode), 0x00, sizeof(CodecPluginNode)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s failed");
    }

    if (AddPluginToList(type, plugin, node) != MEDIA_HAL_OK) {
        free(node);
        return TD_FAILURE;
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int InitList(CodecType type)
{
    switch (type) {
        case VIDEO_DECODER: {
            INIT_LIST_HEAD(&g_vdecPliginList);
            break;
        }
        case VIDEO_ENCODER: {
            INIT_LIST_HEAD(&g_vencPliginList);
            break;
        }
        case AUDIO_DECODER: {
            INIT_LIST_HEAD(&g_adecPliginList);
            break;
        }
        case AUDIO_ENCODER: {
            INIT_LIST_HEAD(&g_aencPliginList);
            break;
        }
        default: {
            MEDIA_HAL_LOGE(MODULE_NAME, "invalid type:%d", type);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static int GetAbsPath(const char *dirPath, char *absPath)
{
    if (strlen(dirPath) >= PATH_MAX || realpath(dirPath, absPath) == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "realpath error:%s", dirPath);
        return TD_FAILURE;
    }
    if (access(absPath, F_OK | R_OK) != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "file is not exist or read:%s", absPath);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static int GetCompeltePath(const char *absPath, const struct dirent* pdirent, char *compeltePath, int length)
{
    if (strcpy_s(compeltePath, length, absPath) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "copy path[%s] fail", absPath);
        return TD_FAILURE;
    }
    if (strcat_s(compeltePath, length, "/") != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "strcat_s fail");
        return TD_FAILURE;
    }
    if (strcat_s(compeltePath, length, pdirent->d_name) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "strcat_s[%s] fail", pdirent->d_name);
        return TD_FAILURE;
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "complete path is %s", compeltePath);
    return TD_SUCCESS;
}

static void CheckAddPlugin(const char *compeltePath, const char *getImplSymbol, CodecType type)
{
    void *soModule = MediaHalDLOpen(compeltePath);
    if (soModule == NULL) {
        return;
    }

    GetImplFunNew getImplFun = MediaHalDLSym(soModule, getImplSymbol);
    if (getImplFun == NULL) {
        MediaHalDLClose(soModule);
        return;
    }
    void *plugin = getImplFun();
    if (plugin == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "getImplFun fail");
        MediaHalDLClose(soModule);
        return;
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "find valid impl %s, plugin name is %s", getImplSymbol, compeltePath);
    if (RegisterCodecPlugin(plugin, soModule, compeltePath, type) != TD_SUCCESS) {
        MediaHalDLClose(soModule);
        MEDIA_HAL_LOGE(MODULE_NAME, "register plugin fail, type:%d path is:%s", type, compeltePath);
    }
    free(plugin);
}

int SearchLoadValidPlugin(const char *dirPath, const char *getImplSymbol, CodecType type)
{
    MEDIA_HAL_LOGI(MODULE_NAME, "type[%d] in", type);
    char absPath[PATH_MAX] = { 0 };
    CHK_FAILED_RETURN_NO_LOG(GetAbsPath(dirPath, absPath), MEDIA_HAL_OK, MEDIA_HAL_ERR);

    DIR *pdir = opendir(absPath);
    if (pdir == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "opendir:%s fail", absPath);
        return MEDIA_HAL_ERR;
    }

    InitList(type);
    struct dirent* pdirent = NULL;
    const char *keyStr = g_codeType[type];

    while ((pdirent = readdir(pdir)) != NULL) {
        if (strstr(pdirent->d_name, keyStr) == NULL || pdirent->d_name[0] == '.') {
            continue;
        }
        char compeltePath[PATH_MAX] = { 0 };
        if (GetCompeltePath(absPath, pdirent, compeltePath, PATH_MAX) != MEDIA_HAL_OK) {
            closedir(pdir);
            return MEDIA_HAL_ERR;
        }
        CheckAddPlugin(compeltePath, getImplSymbol, type);
    }
    closedir(pdir);
    MEDIA_HAL_LOGI(MODULE_NAME, "type[%d] out", type);
    return MEDIA_HAL_OK;
}

static void* FindSpecificPluginInternal(AvCodecMime mime, bool hardwareFirst,
    const OtListHead *pluginList, CodecType type)
{
    MEDIA_HAL_UNUSED(hardwareFirst);
    OtListHead *pos = NULL;
    CodecPluginNode *node = NULL;
    bool isMatch = false;
    OT_LIST_FOR_EACH(pos, pluginList)
    {
        node = OT_LIST_ENTRY(pos, CodecPluginNode, listPtr);
        CodecPluginCommon *pluginCommon =
            (type == VIDEO_DECODER) ? &(node->videoDecoder.pluginCommon) :
            ((type == VIDEO_ENCODER) ? &(node->videoEncoder.pluginCommon) :
            ((type == AUDIO_DECODER) ? &(node->audioDecoder.pluginCommon) :
            ((type == AUDIO_ENCODER) ? &(node->audioEncoder.pluginCommon) : NULL)));
        if (pluginCommon == NULL) {
            MEDIA_HAL_LOGE(MODULE_NAME, "invalid type");
            return NULL;
        }
        for (int i = 0; i < pluginCommon->validCapbilityNum; i++) {
            isMatch = pluginCommon->capbilites[i].mime == mime &&
                pluginCommon->isHardwarePlugin == true;
            if (isMatch) {
                break;
            }
        }
    }
    if (isMatch) {
        return (type == VIDEO_DECODER) ? (void *)(&node->videoDecoder) :
               ((type == AUDIO_DECODER) ? (void *)(&node->audioDecoder) :
               ((type == VIDEO_ENCODER) ? (void *)(&node->videoEncoder) :
               ((type == AUDIO_ENCODER) ? (void *)(&node->audioEncoder) : NULL)));
    }
    return NULL;
}

void* FindSpecificPlugin(AvCodecMime mime, bool hardwareFirst, CodecType type)
{
    void *pluginEntry = NULL;

    switch (type) {
        case VIDEO_DECODER: {
            MEDIA_HAL_LOCK(g_vdecPluginListLock);
            pluginEntry = FindSpecificPluginInternal(mime, hardwareFirst, &g_vdecPliginList, type);
            MEDIA_HAL_UNLOCK(g_vdecPluginListLock);
            break;
        }
        case VIDEO_ENCODER: {
            MEDIA_HAL_LOCK(g_vencPluginListLock);
            pluginEntry = FindSpecificPluginInternal(mime, hardwareFirst, &g_vencPliginList, type);
            MEDIA_HAL_UNLOCK(g_vencPluginListLock);
            break;
        }
        case AUDIO_DECODER: {
            MEDIA_HAL_LOCK(g_adecPluginListLock);
            pluginEntry = FindSpecificPluginInternal(mime, hardwareFirst, &g_adecPliginList, type);
            MEDIA_HAL_UNLOCK(g_adecPluginListLock);
            break;
        }
        case AUDIO_ENCODER: {
            MEDIA_HAL_LOCK(g_aencPluginListLock);
            pluginEntry = FindSpecificPluginInternal(mime, hardwareFirst, &g_aencPliginList, type);
            MEDIA_HAL_UNLOCK(g_aencPluginListLock);
            break;
        }
        default: {
            MEDIA_HAL_LOGE(MODULE_NAME, "invalid type");
            break;
        }
    }
    return pluginEntry;
}

static int UnloadVdecPlugin(void)
{
    OtListHead *pos = NULL;
    CodecPluginNode *node = NULL;
    OtListHead *tmp = NULL;

    MEDIA_HAL_LOCK(g_vdecPluginListLock);
    OT_LIST_FOR_EACH_SAFE(pos, tmp, &g_vdecPliginList)
    {
        node = OT_LIST_ENTRY(pos, CodecPluginNode, listPtr);
        dlclose(node->videoDecoder.pluginCommon.module);
        OT_LIST_DEL(&node->listPtr);
        free(node);
    }
    MEDIA_HAL_UNLOCK(g_vdecPluginListLock);
    return TD_SUCCESS;
}

static int UnloadAdecPlugin(void)
{
    OtListHead *pos = NULL;
    CodecPluginNode *node = NULL;
    OtListHead *tmp = NULL;
    MEDIA_HAL_LOCK(g_adecPluginListLock);
    OT_LIST_FOR_EACH_SAFE(pos, tmp, &g_adecPliginList)
    {
        node = OT_LIST_ENTRY(pos, CodecPluginNode, listPtr);
        UnImplFun unImplFun = dlsym(node->audioDecoder.pluginCommon.module, "ADecoderImplUnRegister");
        if (unImplFun == NULL) {
            MEDIA_HAL_LOGE(MODULE_NAME, "unImplFun null, plugin[%s]", node->audioDecoder.pluginCommon.pluginName);
            MEDIA_HAL_UNLOCK(g_adecPluginListLock);
            return TD_FAILURE;
        }
        int ret = unImplFun();
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "unimpl fun fail, plugin[%s]", node->audioDecoder.pluginCommon.pluginName);
            MEDIA_HAL_UNLOCK(g_adecPluginListLock);
            return TD_FAILURE;
        }

        dlclose(node->audioDecoder.pluginCommon.module);
        OT_LIST_DEL(&node->listPtr);
        free(node);
    }
    MEDIA_HAL_UNLOCK(g_adecPluginListLock);
    return TD_SUCCESS;
}

static int UnloadAencPlugin(void)
{
    OtListHead *pos = NULL;
    CodecPluginNode *node = NULL;
    OtListHead *tmp = NULL;
    MEDIA_HAL_LOCK(g_aencPluginListLock);
    OT_LIST_FOR_EACH_SAFE(pos, tmp, &g_aencPliginList)
    {
        node = OT_LIST_ENTRY(pos, CodecPluginNode, listPtr);
        UnImplFun unImplFun = dlsym(node->audioEncoder.pluginCommon.module, "AEncoderImplUnRegister");
        if (unImplFun == NULL) {
            MEDIA_HAL_LOGE(MODULE_NAME, "unImplFun null, plugin[%s]", node->audioEncoder.pluginCommon.pluginName);
            MEDIA_HAL_UNLOCK(g_aencPluginListLock);
            return TD_FAILURE;
        }
        int ret = unImplFun();
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "unimpl fun fail, plugin[%s]", node->audioEncoder.pluginCommon.pluginName);
            MEDIA_HAL_UNLOCK(g_aencPluginListLock);
            return TD_FAILURE;
        }

        dlclose(node->audioEncoder.pluginCommon.module);
        OT_LIST_DEL(&node->listPtr);
        free(node);
    }
    MEDIA_HAL_UNLOCK(g_aencPluginListLock);
    return TD_SUCCESS;
}

static int UnloadVencPlugin(void)
{
    OtListHead *pos = NULL;
    CodecPluginNode *node = NULL;
    OtListHead *tmp = NULL;
    MEDIA_HAL_LOCK(g_vencPluginListLock);
    OT_LIST_FOR_EACH_SAFE(pos, tmp, &g_vencPliginList)
    {
        node = OT_LIST_ENTRY(pos, CodecPluginNode, listPtr);
        UnImplFun unImplFun = dlsym(node->videoEncoder.pluginCommon.module, "VEncoderImplUnRegister");
        if (unImplFun == NULL) {
            MEDIA_HAL_LOGE(MODULE_NAME, "unImplFun null, plugin[%s]", node->videoEncoder.pluginCommon.pluginName);
            MEDIA_HAL_UNLOCK(g_vencPluginListLock);
            return TD_FAILURE;
        }
        int ret = unImplFun();
        if (ret != TD_SUCCESS) {
            MEDIA_HAL_LOGE(MODULE_NAME, "unimpl fun fail, plugin[%s]", node->videoEncoder.pluginCommon.pluginName);
            MEDIA_HAL_UNLOCK(g_vencPluginListLock);
            return TD_FAILURE;
        }

        dlclose(node->videoEncoder.pluginCommon.module);
        OT_LIST_DEL(&node->listPtr);
        free(node);
    }
    MEDIA_HAL_UNLOCK(g_vencPluginListLock);
    return TD_SUCCESS;
}

int UnLoadPlugin(CodecType type)
{
    switch (type) {
        case VIDEO_DECODER: {
            return UnloadVdecPlugin();
        }
        case VIDEO_ENCODER: {
            return UnloadVencPlugin();
        }
        case AUDIO_DECODER: {
            return UnloadAdecPlugin();
        }
        case AUDIO_ENCODER: {
            return UnloadAencPlugin();
        }
        default: {
            MEDIA_HAL_LOGE(MODULE_NAME, "invalid type:%d", type);
            return TD_FAILURE;
        }
    }
}

