 /*
 * Copyright (c) 2026 RKH Corp.
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

#include "audio_usb_port_plugin.h"

#include <stdlib.h>
#include "securec.h"
#include <dirent.h>
#include "audio_usb_sub_port_in.h"
#include "audio_usb_sub_port_out.h"

#include "media_hal_common.h"
#include "audio_port_plugin_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MODULE_NAME "AudioUsbPortPlugin"

typedef struct {
    /* sub port track context */
    AudioHandle subPortTrackHandle;
} UsbPortTrackContext;

typedef struct {
    /* port context */
    bool isOpened;
    int cardUsbNum;
    /* sub port context */
    AudioHandle subPortInHandle;
    AudioHandle subPortOutHandle;
} UsbPortContext;

static int32_t UsbPortGetPortPluginCapability(const struct AudioPort *port,
    struct AudioPortCapability *capability)
{
    CHK_NULL_RETURN(port, MEDIA_HAL_INVALID_PARAM, "Input param port is null");
    CHK_NULL_RETURN(capability, MEDIA_HAL_INVALID_PARAM, "Input param capability is null");

    int32_t ret;
    if (port->dir == PORT_IN) {
        ret = AudioUsbSubPortInGetCapability(port, capability);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioUsbSubPortInGetCapability failed");
    } else if (port->dir == PORT_OUT) {
        ret = AudioUsbSubPortOutGetCapability(port, capability);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioUsbSubPortOutGetCapability failed");
    } else {
        MEDIA_HAL_LOGE(MODULE_NAME, "port direction: 0x%x is invalid", port->dir);
        return MEDIA_HAL_INVALID_PARAM;
    }

    MEDIA_HAL_LOGD(MODULE_NAME, "%s get capability success", port->portName);
    return MEDIA_HAL_OK;
}
// check is or not exist usb audio devices
int IsUsbAudioDevice(const char *cardDir)
{
    char usbbusPath[256];
    FILE *usbbusFile;

    int snprintfRet = snprintf_s(usbbusPath, sizeof(usbbusPath), sizeof(usbbusPath) - 1, "%s/usbbus", cardDir);
    if (snprintfRet < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s failed");
        return 0;
    }

    usbbusFile = fopen(usbbusPath, "r");
    if (usbbusFile) {
        fclose(usbbusFile);
        return 1;
    }

    return 0;
}

#define CARD_PREFIX_LEN 4

static bool CheckDeviceExists(int card, const char *deviceStr)
{
    char cardDir[256];
    FILE *pcm_file;
    
    int32_t ret = snprintf_s(
        cardDir, sizeof(cardDir), sizeof(cardDir) - 1,
        "/proc/asound/card%d/%s/sub0",
        card, deviceStr);
    if (ret < 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s failed to format cardDir");
        return false;
    }
    
    pcm_file = fopen(cardDir, "r");
    if (!pcm_file) {
        return false;
    }
    
    if (fclose(pcm_file) != 0) {
        MEDIA_HAL_LOGE(MODULE_NAME, "fclose failed");
    }
    
    return true;
}

int ListUsbAudioDevices(int *cardNum, const char *deviceStr)
{
    DIR *dir;
    struct dirent *entry;
    char cardDir[256];
    int card = -2;
    int usbcardInvaild = 0;
    
    dir = opendir("/proc/asound");
    if (dir == NULL) {
        MEDIA_HAL_LOGE(MODULE_NAME, "Failed to open /proc/asound");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR || strncmp(entry->d_name, "card", CARD_PREFIX_LEN) != 0) {
            continue;
        }
        
        int32_t ret = snprintf_s(cardDir, sizeof(cardDir), sizeof(cardDir) - 1, "/proc/asound/%s", entry->d_name);
        if (ret < 0) {
            MEDIA_HAL_LOGE(MODULE_NAME, "snprintf_s failed to format cardDir");
            continue;
        }

        if (!IsUsbAudioDevice(cardDir)) {
            continue;
        }
        
        card = atoi(entry->d_name + CARD_PREFIX_LEN);
        MEDIA_HAL_LOGE(MODULE_NAME, "ListUsbAudioDevices cardNum = %d", *cardNum);
        
        if (CheckDeviceExists(card, deviceStr)) {
            *cardNum = card;
            MEDIA_HAL_LOGE(MODULE_NAME, "Card: %d, Device: 0 (Playback supported)\n", *cardNum);
            usbcardInvaild = 1;
        }
    }
    
    closedir(dir);
    
    return usbcardInvaild ? 0 : -1;
}
static int32_t UsbPortOpen(AudioHandle *portHandle)
{
    int32_t cardUsb = -1;
    int32_t ret = 0;
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "Input param portHandle is null");

    UsbPortContext *priPortCtx = (UsbPortContext *)malloc(sizeof(UsbPortContext));
    if (priPortCtx == NULL) {
        MEDIA_HAL_LOGI(MODULE_NAME, "malloc UsbPortContext failed");
        return MEDIA_HAL_NO_MEM;
    }
    if (memset_s(priPortCtx, sizeof(UsbPortContext), 0, sizeof(UsbPortContext)) != EOK) {
        MEDIA_HAL_LOGE(MODULE_NAME, "memset_s UsbPortContext failed");
        free(priPortCtx);
        return MEDIA_HAL_ERR;
    }

    if (!ListUsbAudioDevices(&cardUsb, "pcm0c")) {
        priPortCtx->cardUsbNum = cardUsb;
        ret = AudioUsbSubPortInOpen(&priPortCtx->subPortInHandle, cardUsb);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "AudioUsbSubPortInOpen failed ret 0x%x", ret);
            free(priPortCtx);
            return ret;
        }
    }
    if (!ListUsbAudioDevices(&cardUsb, "pcm0p")) {
        priPortCtx->cardUsbNum = cardUsb;
        ret = AudioUsbSubPortOutOpen(&priPortCtx->subPortOutHandle, cardUsb);
        if (ret != MEDIA_HAL_OK) {
            MEDIA_HAL_LOGE(MODULE_NAME, "AudioUsbSubPortOutOpen failed ret 0x%x", ret);
            free(priPortCtx);
            return ret;
        }
    }
    priPortCtx->isOpened = true;
    *portHandle = priPortCtx;
    MEDIA_HAL_LOGD(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static bool AudioPluginIsValidPin(enum AudioPortPin pin, const char *pluginName)
{
    if (strcmp("Usb", pluginName) == 0) {
        if (pin == PIN_IN_USB_EXT || pin == PIN_OUT_USB_EXT || pin == PIN_OUT_USB) {
            return true;
        }
    }
    return false;
}

static int32_t UsbPortCreateTrack(AudioHandle portHandle, enum AudioPortPin audioPortPin,
    struct PortPluginAttr *portAttr, AudioHandle *trackHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "Input param portHandle is null");
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "Input param trackHandle is null");
    CHK_NULL_RETURN(portAttr, MEDIA_HAL_INVALID_PARAM, "Input param portAttr is null");

    if (!AudioPluginIsValidPin(audioPortPin, "Usb")) {
        MEDIA_HAL_LOGE(MODULE_NAME, "pin 0x%x is invalid", audioPortPin);
        return MEDIA_HAL_INVALID_PARAM;
    }

    UsbPortContext *priPortCtx = (UsbPortContext *)portHandle;
    CHK_FAILED_RETURN(priPortCtx->isOpened, true, MEDIA_HAL_ERR, "port not open");

    int32_t ret;
    MEDIA_HAL_LOGI(MODULE_NAME, "audioPortPin=%d", audioPortPin);
    if (audioPortPin == PIN_IN_USB_EXT) {
        ret = AudioUsbSubPortInCreateTrack(priPortCtx->subPortInHandle,
            trackHandle, portAttr, audioPortPin);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioUsbSubPortInCreateTrack failed");
    } else {
        ret = AudioUsbSubPortOutCreateTrack(priPortCtx->subPortOutHandle,
            trackHandle, portAttr, audioPortPin);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioUsbSubPortOutCreateTrack failed");
    }

    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int32_t UsbPortDestroyTrack(AudioHandle portHandle, AudioHandle trackHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "Input param portHandle is null");
    CHK_NULL_RETURN(trackHandle, MEDIA_HAL_INVALID_PARAM, "Input param trackHandle is null");

    UsbPortContext *priPortCtx = (UsbPortContext *)portHandle;
    CHK_FAILED_RETURN(priPortCtx->isOpened, true, MEDIA_HAL_ERR, "port not open");

    int32_t ret;
    AudioPortTrack *track = (AudioPortTrack *)trackHandle;
    if (track->audioPin == PIN_IN_USB_EXT) {
        ret = AudioUsbSubPortInDestroyTrack(priPortCtx->subPortInHandle, trackHandle);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioUsbSubPortInDestroyTrack failed");
    } else {
        ret = AudioUsbSubPortOutDestroyTrack(priPortCtx->subPortOutHandle, trackHandle);
        CHK_FAILED_RETURN(ret, MEDIA_HAL_OK, ret, "AudioUsbSubPortOutDestroyTrack failed");
    }
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static int32_t UsbPortClose(AudioHandle portHandle)
{
    CHK_NULL_RETURN(portHandle, MEDIA_HAL_INVALID_PARAM, "Input param portHandle is null");
    UsbPortContext *priPortCtx = (UsbPortContext *)portHandle;
    CHK_FAILED_RETURN(priPortCtx->isOpened, true, MEDIA_HAL_ERR, "port not open");

    int32_t ret = AudioUsbSubPortInClose(priPortCtx->subPortInHandle);
    CHK_FAILED_NO_RETURN(ret, MEDIA_HAL_OK, "AudioUsbSubPortInClose failed");
    ret = AudioUsbSubPortOutClose(priPortCtx->subPortOutHandle);
    CHK_FAILED_NO_RETURN(ret, MEDIA_HAL_OK, "AudioUsbSubPortOutClose failed");

    priPortCtx->isOpened = false;
    free(priPortCtx);
    MEDIA_HAL_LOGI(MODULE_NAME, "success");
    return MEDIA_HAL_OK;
}

static AudioPortPlugin g_usbPortPlugin = {
    .audioPortDir = PORT_OUT_IN,
    .audioPortPins = PIN_IN_USB_EXT | PIN_OUT_USB_EXT | PIN_OUT_USB,
    .GetPortPluginCapability = UsbPortGetPortPluginCapability,
    .Open = UsbPortOpen,
    .CreateTrack = UsbPortCreateTrack,
    .DestroyTrack = UsbPortDestroyTrack,
    .Close = UsbPortClose,
    .SetMute = NULL,
    .GetMute = NULL,
    .SetVolume = NULL,
    .GetVolume = NULL,
};

const AudioPortPlugin *GetPortPluginFuncs(AudioPluginType pluginType)
{
    if (pluginType == AUDIO_PLUGIN_BLUETOOTH) {
        return NULL;
    }
    return &g_usbPortPlugin;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
