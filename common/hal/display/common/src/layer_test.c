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

#include <stdio.h>
#include <string.h>
#include "display_layer.h"
#include "display_gfx.h"

static LayerFuncs* g_layerFunc = NULL;
static GfxFuncs* g_gfxFunc = NULL;

static LayerInfo g_layerInfo = {};
static uint32_t g_layerId = 0;
static LayerBuffer g_fbBuf = {};

const static uint8_t DISPALY_DEV_ID = 0;
const static uint8_t OPA_OPAQUE = 255;
const static uint16_t DEFAULT_WIDTH = 720;
const static uint16_t DEFAULT_HEIGHT = 480;
const static uint8_t DEFAULT_BPP = 32;

static void FillArea(void)
{
    ISurface surface = {};
    surface.phyAddr = g_fbBuf.data.phyAddr;
    surface.height = g_fbBuf.height;
    surface.width = g_fbBuf.width;
    surface.stride = g_fbBuf.pitch;
    surface.enColorFmt = g_fbBuf.pixFormat;
    surface.clutPhyAddr = 0;
    surface.bYCbCrClut = false;
    surface.bAlphaMax255 = true;
    surface.bAlphaExt1555 = false;
    surface.alpha0 = 0;
    surface.alpha1 = OPA_OPAQUE;

    IRect rect = {0, 0, g_layerInfo.width, g_layerInfo.height};  // 2 half of width and height

    GfxOpt opt = {};
    opt.enGlobalAlpha = true;
    opt.globalAlpha = OPA_OPAQUE;

    if (g_gfxFunc->FillRect(&surface, &rect, 0xFFFFFF00, &opt) == DISPLAY_FAILURE) {
        printf("fill rect failed!\n");
        return;
    }
}

static void CloseLayer(void)
{
    if (g_layerFunc->CloseLayer != NULL) {
        g_layerFunc->CloseLayer(DISPALY_DEV_ID, g_layerId);
    }
}

static bool OpenLayer(void)
{
    g_layerInfo.width = DEFAULT_WIDTH;
    g_layerInfo.height = DEFAULT_HEIGHT;
    g_layerInfo.bpp = DEFAULT_BPP;
    g_layerInfo.pixFormat = PIXEL_FMT_RGBA_8888;
    g_layerInfo.type = LAYER_TYPE_GRAPHIC;

    int32_t ret = -1;
    if (g_layerFunc->CreateLayer != NULL) {
        ret = g_layerFunc->CreateLayer(DISPALY_DEV_ID, &g_layerInfo, &g_layerId);
        if (ret != DISPLAY_SUCCESS) {
            printf("CreateLayer fail\n");
            return false;
        }
    }

    if (g_layerFunc->SetLayerVisible != NULL) {
        ret = g_layerFunc->SetLayerVisible(DISPALY_DEV_ID, g_layerId, true);
        if (ret != DISPLAY_SUCCESS) {
            printf("CreateLayer fail\n");
            return false;
        }
    }

    if (g_layerFunc->SetLayerDirtyRegion != NULL) {
        IRect rect = {0, 0, g_layerInfo.width, g_layerInfo.height};
        ret = g_layerFunc->SetLayerDirtyRegion(DISPALY_DEV_ID, g_layerId, &rect);
        if (ret != DISPLAY_SUCCESS) {
            printf("setLayerDirtyRegion fail\n");
            return false;
        }
    }
    return true;
}

static void Deinit(void)
{
    if (g_gfxFunc->DeinitGfx != NULL) {
        g_gfxFunc->DeinitGfx();
    }

    if (g_layerFunc->DeinitDisplay != NULL) {
        g_layerFunc->DeinitDisplay(DISPALY_DEV_ID);
    }

    if (g_gfxFunc != NULL) {
        GfxUninitialize(g_gfxFunc);
    }

    if (g_layerFunc != NULL) {
        LayerUninitialize(g_layerFunc);
    }
}

static bool Init()
{
    int32_t ret = LayerInitialize(&g_layerFunc);
    if (ret != DISPLAY_SUCCESS) {
        return false;
    }
    if (g_layerFunc->InitDisplay != NULL) {
        ret = g_layerFunc->InitDisplay(DISPALY_DEV_ID);
        if (ret != DISPLAY_SUCCESS) {
            printf("InitDisplay fail\n");
            return false;
        }
    }

    ret = GfxInitialize(&g_gfxFunc);
    if (ret != DISPLAY_SUCCESS) {
        return false;
    }

    if (g_gfxFunc->InitGfx != NULL) {
        g_gfxFunc->InitGfx();
    }
    return true;
}


int main(int argc, char* argv[])
{
    Init();
    OpenLayer();
    if (g_layerFunc->GetLayerBuffer == NULL) {
        goto ERROR;
    }
    if (g_layerFunc->GetLayerBuffer(DISPALY_DEV_ID, g_layerId, &g_fbBuf) != DISPLAY_SUCCESS) {
        goto ERROR;
    }

    FillArea();
    if (g_layerFunc->Flush != NULL) {
        g_layerFunc->Flush(DISPALY_DEV_ID, g_layerId, &g_fbBuf);
    }

    char cmd[32]; // 32 is the length of buffer
    while (fgets(cmd, 10, stdin) != NULL) {  // 10: max get input char
        cmd[10] = '\0'; // 10 is the max length of cmd buffer
        if (strncmp(cmd, "quit", 4) == 0) { // 4 is the length of cmd string
            break;
        } else if (strncmp(cmd, "q", 1) == 0) {
            break;
        } else {
            printf("Input CMD: 'quit' or 'q' for quit the program!\n");
        }
    }

ERROR:
    CloseLayer();
    Deinit();
    return 0;
}
