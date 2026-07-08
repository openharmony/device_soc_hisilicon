/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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
#include "hal_camera.h"
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <memory>

#include "securec.h"
#include "display_layer.h"
#include "dcamera_decoder.h"
#include "dcamera.h"
#include "constants.h"
#include <condition_variable>
#include "dcamera_utils.h"
#include "distributed_hardware_log.h"
#include "dcamera_provider.h"
#include "dbuffer_manager.h"
#include "metadata_utils.h"
#ifdef ENABLE_DFX
#include "media_utils.h"
#endif

/* Constants for reported magic-number values. */
#define DCAMERA_HAL_SHIFT_24                   24

using namespace std;
using namespace OHOS::DistributedHardware;
using namespace OHOS::HDI::DistributedCamera::V1_0;

#define TAG "VirtualCamHal"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct {
    bool isMirror;
    bool isFlip;
    uint32_t fps;
    uint32_t width;
    uint32_t height;
    uint32_t maxW;
    uint32_t maxH;
} ControlInfo;

typedef enum {
    HAL_CENTER_MIRROR = 0,     /**< Central mirroring */
    HAL_HORIZONTAL_MIRROR = 1, /**< Horizontal mirroring */
    HAL_VETICALLY_MIRROR = 2,  /**< Vertical mirroring */
    HAL_ROTATION_90 = 3,       /**< 90-degree rotation */
    HAL_ROTATION_180 = 4,      /**< 180-degree rotation  */
    HAL_ROTATION_BUTT,
} CameraHalInvertMode;

typedef struct {
    PosInfo pos;
    bool isUsed = false;
    bool isOpened = false;
    uint32_t layerId;
    StreamAttr streamAttr;
} InternalStreamInfo;

typedef struct {
    BufferAvailable bufferCallBack;
    CameraResultCb resultCb;
    InternalStreamInfo internalStreamInfo[CAEMRA_MAX_STREAM_NUM];
    DCameraDecoder decoder;
    DBufferManager bufferManager;
    bool isStreamConfigured_ = false;
    bool isCapturing_ = false;
    DCameraProvider *provider = nullptr;
    IDCameraProviderCallback *providerCallback = nullptr;
    bool isDevOpened = false;
    CameraType type;
    string abilityInfo;
    DHBase dhBase_;
    std::mutex openSesslock_;
    std::condition_variable openSessCV_;
    bool isOpenSessFailed_ = false;
    bool isSessOpened_ = false;
    std::mutex isOpenSessFailedlock_;
} CameraDevice;

typedef struct {
    bool inited;
    LayerFuncs *displayLayer;
    CameraDetectCb cameraDetect;
    DCameraProvider *dCameraProvider;
    unordered_map<string, CameraDevice*> deviceInfo;
    vector<string> destoryCamera;
} DistributedHalContext;

DistributedHalContext g_distributedHalMgr = {
    .inited = false,
    .displayLayer = nullptr,
    .cameraDetect = nullptr,
    .dCameraProvider = nullptr,
};

static int32_t GetFreeStreamIndex(const CameraDevice *cameraDevice, uint32_t *streamIndex)
{
    for (int32_t i = 0; i < CAEMRA_MAX_STREAM_NUM; i++) {
        if (!cameraDevice->internalStreamInfo[i].isUsed) {
            *streamIndex = i;
            return HAL_SUCCESS;
        }
    }
    DHLOGE("get free stream index failed");
    return HAL_FAILURE;
}

static std::vector<int32_t> GetStreamIds(const CameraDevice *cameraDevice)
{
    DHLOGI("begin get stream id.");
    std::vector<int> streamIds;
    std::string idString = "";
    for (int32_t i = 0; i < CAEMRA_MAX_STREAM_NUM; i++) {
        if (cameraDevice->internalStreamInfo[i].isUsed) {
            streamIds.push_back(i);
            idString += (std::to_string(i) + ", ");
        }
    }
    DHLOGI("ids=[%s]", idString.c_str());
    return streamIds;
}

static uint32_t GetPreStreamId(const CameraDevice *cameraDevice, uint32_t curStreamId)
{
    DHLOGI("begin get Pre stream id.");
    uint32_t i = CAEMRA_MAX_STREAM_NUM;
    for (; i < CAEMRA_MAX_STREAM_NUM; i++) {
        if (cameraDevice->internalStreamInfo[i].isUsed && curStreamId != i) {
            return i;
        }
    }
    return i;
}

static void AddStream(CameraDevice *cameraDevice, uint32_t streamIndex, const StreamAttr *stream)
{
    LOG_CHK_RETURN(streamIndex >= CAEMRA_MAX_STREAM_NUM);
    cameraDevice->internalStreamInfo[streamIndex].streamAttr = *stream;
    cameraDevice->internalStreamInfo[streamIndex].isUsed = true;
    DHLOGI("stream %u", streamIndex);
}

static void DeleteStream(CameraDevice *cameraDevice, uint32_t streamIndex)
{
    HAL_LOG_DOFUNC(memset_s(
        &cameraDevice->internalStreamInfo[streamIndex], sizeof(InternalStreamInfo), 0, sizeof(InternalStreamInfo)));
    for (uint8_t i = 0; i < CAEMRA_MAX_STREAM_NUM; i++) {
        if (cameraDevice->internalStreamInfo[i].isUsed) {
        }
    }
}

int32_t DistributedHalCameraInit(void)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(g_distributedHalMgr.inited == true, HAL_SUCCESS);
    HAL_LOG_DOFUNC(LayerInitialize(&g_distributedHalMgr.displayLayer));
    g_distributedHalMgr.inited = true;
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraDeinit(void)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(g_distributedHalMgr.inited == false, HAL_SUCCESS);
    HAL_LOG_DOFUNC(LayerUninitialize(g_distributedHalMgr.displayLayer));

    g_distributedHalMgr.inited = false;
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraGetModeNum(uint8_t *num)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(num == nullptr, HAL_FAILURE);
    DHLOGE("NOT Support GetModeNum");
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraSetMode(uint8_t index)
{
    HAL_ENTER();
    DHLOGE("NOT Support SetMode");
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraSetDeviceDetectCb(const CameraDetectCb cb)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(g_distributedHalMgr.inited == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cb == nullptr || g_distributedHalMgr.cameraDetect != nullptr, HAL_FAILURE);
    g_distributedHalMgr.cameraDetect = cb;
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraGetDeviceNum(uint8_t *num)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(g_distributedHalMgr.inited == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(num == nullptr, HAL_FAILURE);
    *num = static_cast<uint8_t>(g_distributedHalMgr.deviceInfo.size());
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraGetDeviceList(char cameraList[][CAMERA_NAME_MAX_LEN], uint8_t listNum)
{
    HAL_ENTER();
    HAL_CHECK_NULL_PTR(cameraList);
    uint8_t sz = static_cast<uint8_t>(g_distributedHalMgr.deviceInfo.size());
    uint8_t list = 0;
    for (auto &dev : g_distributedHalMgr.deviceInfo) {
        if (list >= listNum) {
            return HAL_SUCCESS;
        }
        if (strncpy_s(cameraList[list], CAMERA_NAME_MAX_LEN, dev.first.c_str(), dev.first.size()) != EOK) {
            DHLOGE("strncpy_s failed\n");
            return HAL_FAILURE;
        }
        list++;
    }
    HAL_EXIT();
    return HAL_SUCCESS;
}

static bool CheckValidCamera(const char *camera)
{
    string cameraStr(camera);
    if (g_distributedHalMgr.deviceInfo.count(cameraStr) == 0) {
        return false;
    }
    return true;
}

static void ReadRangCap(StreamCap *streamCap, uint32_t streamNum)
{
    for (uint32_t i = 0; i < streamNum; i++) {
        streamCap[i].format = FORMAT_AVC;
        streamCap[i].u.range.maxFps = MAX_FPS;
        streamCap[i].u.range.minFps = MIN_FPS;
        streamCap[i].u.range.maxHeight = RESOLUTION_1080P_H;
        streamCap[i].u.range.minHeight = MIN_HEIGHT;
        streamCap[i].u.range.maxWidth = RESOLUTION_1080P_W;
        streamCap[i].u.range.minWidth = MIN_WIDTH;
    }
}

static void ReadEnumCap(StreamCap *streamCap, uint32_t streamNum)
{
    for (uint32_t i = 0; i < streamNum; i++) {
        streamCap[i].format = FORMAT_AVC;
        streamCap[i].u.formatEnum.frame_rate[0] = MAX_FPS;
        streamCap[i].u.formatEnum.frame_rate_num = 1;
        if (i == 0) {
            streamCap[i].u.formatEnum.width = RESOLUTION_2592X1944_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_2592X1944_H;
        } else if (i == 1) {
            streamCap[i].u.formatEnum.width = RESOLUTION_1080P_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_1080P_H;
        } else if (i == 0x2) {
            streamCap[i].u.formatEnum.width = RESOLUTION_720P_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_720P_H;
        } else if (i == 0x3) {
            streamCap[i].u.formatEnum.width = RESOLUTION_D1_NTSC_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_D1_NTSC_H;
        } else if (i == 0x4) {
            streamCap[i].u.formatEnum.width = RESOLUTION_360P_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_360P_H;
        } else {
            streamCap[i].u.formatEnum.width = RESOLUTION_1080P_W;
            streamCap[i].u.formatEnum.height = RESOLUTION_1080P_H;
        }
    }
}

int32_t DistributedHalCameraGetStreamCapNum(const char *camera, uint32_t *num)
{
    HAL_CHECK_NULL_PTR(num);
    HAL_CHECK_NULL_PTR(camera);
    LOG_CHK_RETURN_ERR(g_distributedHalMgr.inited == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(CheckValidCamera(camera) == false, HAL_FAILURE);
    *num = CAEMRA_MAX_STREAM_NUM;
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraGetStreamCap(const char *camera, StreamCap *streamCap, uint32_t streamNum)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(streamCap == nullptr || camera == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(g_distributedHalMgr.inited == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(CheckValidCamera(camera) == false, HAL_FAILURE);
    uint32_t num = 0;
    HAL_LOG_DOFUNC_RETURN(DistributedHalCameraGetStreamCapNum(camera, &num));
    LOG_CHK_RETURN_ERR(streamNum < num, HAL_FAILURE);
    if (streamCap->type == CAP_DESC_RANGE) {
        ReadRangCap(streamCap, num);
    } else if (streamCap->type == CAP_DESC_ENUM) {
        ReadEnumCap(streamCap, num);
    }
    HAL_EXIT();
    return HAL_SUCCESS;
}

static CameraDevice *GetCameraDevice(const char *camera)
{
    LOG_CHK_RETURN_ERR(camera == nullptr, nullptr);
    LOG_CHK_RETURN_ERR(g_distributedHalMgr.inited == false, nullptr);
    LOG_CHK_RETURN_ERR(CheckValidCamera(camera) == false, nullptr);
    string cameraStr(camera);
    return g_distributedHalMgr.deviceInfo[cameraStr];
}

int32_t DistributedHalCameraDeviceOpen(const char *camera)
{
    HAL_ENTER();
    DHLOGI("open camera = %s", camera);
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == true, HAL_SUCCESS);

    int32_t ret = cameraDev->decoder.Init();
    if (ret != HAL_SUCCESS) {
        DHLOGE("decoder Init.(ret=%d)", ret);
        return ret;
    }
    const std::string videoName = "codec.avc.hardware.decoder";
    VdecAttr attr = {};
    attr.type = VIDEO_DECODER;
    attr.mime = MEDIA_MIMETYPE_VIDEO_AVC;
    attr.priv = nullptr;
    attr.bufSize = 0;
    attr.maxWidth = MAX_SUPPORT_WIDTH;
    attr.maxHeight = MAX_SUPPORT_HEIGHT;
    ret = cameraDev->decoder.CreateHandle(videoName, attr);
    if (ret != HAL_SUCCESS) {
        DHLOGE("CreateHandle.(ret=%d)", ret);
        return ret;
    }
    DHLOGI("CreateHandle succeed.");

    cameraDev->isDevOpened = true;
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraDeviceClose(const char *camera)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_SUCCESS);
    int32_t ret = cameraDev->decoder.DestroyHandle();
    if (ret != HAL_SUCCESS) {
        DHLOGE("DestroyHandle.(ret=%d)", ret);
        return ret;
    }
    DHLOGI("DestroyHandle succeed.");

    cameraDev->isDevOpened = false;
    HAL_EXIT();
    return HAL_SUCCESS;
}

static int32_t ParamCheck(const StreamAttr *stream, const ControlInfo *config)
{
    if (stream->invertMode >= HAL_ROTATION_BUTT) {
        DHLOGE("rotation param invalid\n");
        return HAL_FAILURE;
    }
    if (stream->fps > MAX_FPS || stream->fps == 0) {
        DHLOGE("fps param invalid\n");
        return HAL_FAILURE;
    }
    if (stream->width <= 0 || stream->width > MAX_FRAME_WIDTH || stream->height <= 0 ||
        stream->height > MAX_FRAME_HEIGHT) {
        DHLOGE("width height param invalid\n");
        return HAL_FAILURE;
    }
    if (stream->crop.x == 0 && stream->crop.y == 0 && stream->crop.w == 0 && stream->crop.h == 0) {
        return HAL_SUCCESS;
    }
    if (stream->crop.x + stream->crop.w > config->maxW || stream->crop.y + stream->crop.h > config->maxH) {
        DHLOGE("crop param invalid\n");
        return HAL_FAILURE;
    }
    if (stream->invertMode == HAL_ROTATION_90) {
        if (stream->crop.w < stream->height || stream->crop.h < stream->width) {
            DHLOGE("crop param invalid\n");
            return HAL_FAILURE;
        }
    } else if (stream->crop.h < stream->height || stream->crop.w < stream->width) {
        DHLOGE("crop param invalid\n");
        return HAL_FAILURE;
    }
    return HAL_SUCCESS;
}

static int32_t DistributedOpenSession(CameraDevice *cameraDev)
{
    if (cameraDev->isSessOpened_) {
        DHLOGI("isSessOpened_");
        return HAL_SUCCESS;
    }
    int32_t ret = cameraDev->provider->OpenSession(cameraDev->dhBase_);
    if (ret != DCamRetCode::SUCCESS) {
        DHLOGE("Open distributed camera control session failed, ret = %d.", ret);
        return ret;
    }

    std::unique_lock<std::mutex> lock(cameraDev->openSesslock_);
    auto st = cameraDev->openSessCV_.wait_for(lock, chrono::seconds(WAIT_OPEN_TIMEOUT_SEC));
    if (st == std::cv_status::timeout) {
        DHLOGE("Wait for distributed camera session open timeout.");
        return DCamRetCode::FAILED;
    }
    {
       std::unique_lock<std::mutex> openStateLock(cameraDev->isOpenSessFailedlock_);
       if (cameraDev->isOpenSessFailed_) {
           DHLOGE("Open distributed camera session failed.");
           return DCamRetCode::FAILED;
       }
    }
    cameraDev->isSessOpened_ = true;
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraStreamCreate(const char *camera, const StreamAttr *stream, uint32_t *streamId)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(stream == nullptr || streamId == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);
    ControlInfo config = {0};
    config.maxW = MAX_FRAME_WIDTH;
    config.maxH = MAX_FRAME_HEIGHT;
    HAL_LOG_DOFUNC_RETURN(ParamCheck(stream, &config));
    uint32_t streamIndex = 0;
    HAL_LOG_DOFUNC_RETURN(GetFreeStreamIndex(cameraDev, &streamIndex));
    // start
    int32_t ret = DistributedOpenSession(cameraDev);
    if (ret != HAL_SUCCESS) {
        DHLOGE("Open distributed camera control session failed, ret = %d.", ret);
        return ret;
    }
    AddStream(cameraDev, streamIndex, stream);
    *streamId = streamIndex;
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraGetDeviceId(const char *camera, uint32_t streamId, uint32_t *deviceId)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM || deviceId == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->internalStreamInfo[streamId].isUsed == false, HAL_FAILURE);
    // *deviceId = GetDeviceId((uint32_t)cameraDev->internalStreamInfo[streamId].vpssChn);
    HAL_EXIT();
    return HAL_SUCCESS;
}

static int32_t DistributedDoCapture(CameraDevice *cameraDev)
{
    if (cameraDev->isCapturing_) {
        DHLOGI("isCapturing_");
        return HAL_SUCCESS;
    }
    std::vector<int32_t> streamIds = GetStreamIds(cameraDev);
    if (streamIds.empty()) {
        DHLOGI("Distributed camera streamIds is empty.");
        return HAL_FAILURE;
    }

    constexpr uint32_t dataspace = 8;
    int32_t streamId = streamIds[0];
    StreamAttr *streamAttr = &cameraDev->internalStreamInfo[streamId].streamAttr;

    std::vector<DCCaptureInfo> captureInfos;
    DCCaptureInfo capture;
    capture.streamIds_.emplace_back(streamId);
    capture.width_ = streamAttr->width;
    capture.height_ = streamAttr->height;
    capture.stride_ = 0;
    capture.format_ = FORMAT_YVU420;
    capture.dataspace_ = dataspace;
    capture.isCapture_ = true;
    capture.encodeType_ = DCEncodeType::ENCODE_TYPE_H264;
    capture.type_ = DCStreamType::CONTINUOUS_FRAME;
    DCameraSettings setting = {FPS_RANGE, std::to_string(streamAttr->fps)};
    capture.captureSettings_.push_back(setting);
    captureInfos.emplace_back(capture);

    int32_t ret = cameraDev->provider->StartCapture(cameraDev->dhBase_, captureInfos);
    if (ret != HAL_SUCCESS) {
        DHLOGE("ConfigureStreams Failed:%d", ret);
        return ret;
    }
    cameraDev->isCapturing_ = true;
    return HAL_SUCCESS;
}

static void GetLayerInfo(IRect &attr, LayerInfo &lInfo, InternalStreamInfo *streamInfo)
{
    attr.x = streamInfo->pos.x;
    attr.y = streamInfo->pos.y;
    attr.w = streamInfo->pos.w;
    attr.h = streamInfo->pos.h;

    lInfo.width = streamInfo->pos.w;
    lInfo.height = streamInfo->pos.h;
    lInfo.type = LAYER_TYPE_OVERLAY;
    lInfo.bpp = NUMBER_OF_BITS;
    lInfo.pixFormat = PIXEL_FMT_YCRCB_420_SP;
    lInfo.fps = streamInfo->streamAttr.fps;
    DHLOGI("attr.x:%d attr.y:%d attr.w:%d attr.h:%d", attr.x, attr.y, attr.w, attr.h);
}

static int32_t CreateStreamLayer(InternalStreamInfo *streamInfo, uint32_t displayDevId)
{
    IRect attr = {};
    LayerInfo lInfo = {};

    GetLayerInfo(attr, lInfo, streamInfo);
    int32_t ret = g_distributedHalMgr.displayLayer->CreateLayer(displayDevId, &lInfo, &streamInfo->layerId);
    if (ret != HAL_SUCCESS) {
        DHLOGE("CreateLayer Failed:%d", ret);
        return ret;
    }
    ret = g_distributedHalMgr.displayLayer->SetLayerSize(displayDevId, streamInfo->layerId, &attr);
    if (ret != HAL_SUCCESS) {
        DHLOGE("SetLayerSize Failed:%d", ret);
        return ret;
    }
    return HAL_SUCCESS;
}

static uint32_t GetStreamBindDeviceId(CameraDevice *cameraDev, uint32_t streamId, uint32_t deviceId)
{
    InternalStreamInfo *streamInfo = &cameraDev->internalStreamInfo[streamId];
    uint32_t preStreamId = GetPreStreamId(cameraDev, streamId);
    if (preStreamId < CAEMRA_MAX_STREAM_NUM) {
        InternalStreamInfo *preStreamInfo = &cameraDev->internalStreamInfo[preStreamId];
        uint32_t preDeviceId;
        g_distributedHalMgr.displayLayer->GetLayerDeviceId(0, preStreamInfo->layerId, &preDeviceId);
        g_distributedHalMgr.displayLayer->InvokeLayerCmd(
            0, streamInfo->layerId, OVERLAYER_CMD_VO_UNBIND_VPSS, CHAN_ID(deviceId), DEVICE_ID(deviceId));

        g_distributedHalMgr.displayLayer->InvokeLayerCmd(
            0, streamInfo->layerId, OVERLAYER_CMD_VO_BIND_VPSS, CHAN_ID(preDeviceId), DEVICE_ID(preDeviceId));
        return preDeviceId;
    }
    return deviceId;
}

static int32_t StartStreamDecoder(CameraDevice *cameraDev, uint32_t bindDeviceId)
{
    Param param = {.key = KEY_DEVICE_ID, .val = (void *)&bindDeviceId, .size = sizeof(uint32_t)};
    int32_t ret = cameraDev->decoder.SetMetadata(&param, 1);
    if (ret != 0) {
        DHLOGE("Set codec source failed.(ret=%d)", ret);
        return HAL_FAILURE;
    }
    ret = cameraDev->decoder.StartDec();
    if (ret != HAL_SUCCESS) {
        DHLOGE("StartDec Failed:%d", ret);
        return ret;
    }
    return HAL_SUCCESS;
}

static int32_t DistributedProcForStreamOn(CameraDevice *cameraDev, uint32_t streamId)
{
    if (!cameraDev->decoder.HandleCreated()) {
        DHLOGE("HandleCreated false");
        return HAL_FAILURE;
    }
    if (cameraDev->decoder.Started()) {
        DHLOGI("StopDec ");
        cameraDev->decoder.StopDec();
    }
    InternalStreamInfo *streamInfo = &cameraDev->internalStreamInfo[streamId];
    uint32_t displayDevId = 0;
    int32_t ret = CreateStreamLayer(streamInfo, displayDevId);
    if (ret != HAL_SUCCESS) {
        return ret;
    }

    uint32_t deviceId;
    g_distributedHalMgr.displayLayer->GetLayerDeviceId(0, streamInfo->layerId, &deviceId);
    DHLOGI("GetLayerDeviceId = %u", deviceId);
    uint32_t bindDeviceId = GetStreamBindDeviceId(cameraDev, streamId, deviceId);
    return StartStreamDecoder(cameraDev, bindDeviceId);
}

int32_t DistributedHalCameraStreamOn(const char *camera, uint32_t streamId)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->internalStreamInfo[streamId].isUsed == false, HAL_FAILURE);
    int32_t ret = DistributedDoCapture(cameraDev);
    if (ret != HAL_SUCCESS) {
        DHLOGE("DistributedDoCapture Failed:%d", ret);
        return ret;
    }
    ret = DistributedProcForStreamOn(cameraDev, streamId);
    if (ret != HAL_SUCCESS) {
        DHLOGE("DistributedProcForStreamOn Failed:%d", ret);
        return ret;
    }
    cameraDev->internalStreamInfo[streamId].isOpened = true;
    HAL_EXIT();
    return HAL_SUCCESS;
}

static int32_t DistributedCancelCapture(CameraDevice *cameraDev)
{
    if (!cameraDev->isCapturing_) {
        DHLOGI("not isCapturing_");
        return HAL_SUCCESS;
    }
    std::vector<int32_t> streamIds = GetStreamIds(cameraDev);
    if (streamIds.empty()) {
        DHLOGI("Distributed camera streamIds is empty.");
        return HAL_FAILURE;
    }
    int32_t ret = cameraDev->provider->StopCapture(cameraDev->dhBase_, streamIds);
    if (ret != HAL_SUCCESS) {
        DHLOGE("StopCapture Failed:%d", ret);
        return ret;
    }
    cameraDev->isCapturing_ = false;
    return HAL_SUCCESS;
}

static int32_t DistributedProcForStreamOff(CameraDevice *cameraDev, uint32_t streamId)
{
    if (!cameraDev->decoder.HandleCreated()) {
        DHLOGE("HandleCreated false");
        return HAL_FAILURE;
    }
    int32_t ret;
    InternalStreamInfo *streamInfo = &cameraDev->internalStreamInfo[streamId];
    std::vector<int32_t> streamIds = GetStreamIds(cameraDev);
    DHLOGI("streamIds size:%lu", streamIds.size());

    // unbind
    uint32_t deviceId;
    g_distributedHalMgr.displayLayer->GetLayerDeviceId(0, streamInfo->layerId, &deviceId);
    DHLOGI("GetLayerDeviceId = %u", deviceId);
    deviceId = ((1 << DCAMERA_HAL_SHIFT_24) & 0xff000000) + deviceId;
    Param param = {.key = KEY_DEVICE_ID, .val = (void *)&deviceId, .size = sizeof(uint32_t)};
    ret = cameraDev->decoder.SetMetadata(&param, 1);
    if (ret != 0) {
        DHLOGE("unbind codec source failed.(ret=%d)", ret);
    }

    if (streamIds.size() <= 1) {
        DHLOGI("StopDec");
        ret = cameraDev->decoder.StopDec();
        if (ret != HAL_SUCCESS) {
            DHLOGE("StopDec Failed:%d", ret);
        }
    }
    uint32_t displayDevId = 0;
    ret = g_distributedHalMgr.displayLayer->CloseLayer(displayDevId, streamInfo->layerId);
    if (ret != HAL_SUCCESS) {
        DHLOGE("CloseLayer Failed:%d", ret);
        return ret;
    }
    return HAL_SUCCESS;
}

static int32_t DistributedReleaseStreams(CameraDevice *cameraDev)
{
    if (!cameraDev->isStreamConfigured_) {
        DHLOGI("isStreamConfigured_ fasle");
        return HAL_SUCCESS;
    }
    std::vector<int32_t> streamIds = GetStreamIds(cameraDev);
    int32_t ret = cameraDev->provider->ReleaseStreams(cameraDev->dhBase_, streamIds);
    if (ret != HAL_SUCCESS) {
        DHLOGE("ReleaseStreams Failed:%d", ret);
    }
    cameraDev->isStreamConfigured_ = false;
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraStreamOff(const char *camera, uint32_t streamId)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->internalStreamInfo[streamId].isUsed == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->internalStreamInfo[streamId].isOpened == false, HAL_FAILURE);
    int32_t ret = DistributedProcForStreamOff(cameraDev, streamId);
    if (ret != HAL_SUCCESS) {
        DHLOGE("DistributedProcForStreamOn Failed:%d", ret);
    }
    ret = DistributedCancelCapture(cameraDev);
    if (ret != HAL_SUCCESS) {
        DHLOGE("DistributedCancelCapture Failed:%d", ret);
    }
    ret = DistributedReleaseStreams(cameraDev);
    if (ret != HAL_SUCCESS) {
        DHLOGE("DistributedReleaseStreams Failed:%d", ret);
    }
    cameraDev->internalStreamInfo[streamId].isOpened = false;
    HAL_EXIT();
    return HAL_SUCCESS;
}

static int32_t DistributedCloseSession(CameraDevice *cameraDev)
{
    if (!cameraDev->isSessOpened_) {
        DHLOGI("isSessOpened_");
        return HAL_SUCCESS;
    }
    int32_t ret = cameraDev->provider->CloseSession(cameraDev->dhBase_);
    if (ret != HAL_SUCCESS) {
        DHLOGE("CloseSession failed:%d", ret);
    }
    cameraDev->isOpenSessFailed_ = false;
    cameraDev->isSessOpened_ = false;
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraStreamDestroy(const char *camera, uint32_t streamId)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->internalStreamInfo[streamId].isUsed == false, HAL_FAILURE);
    DHLOGI("DCameraDevice::Close distributed camera: %s", camera);

    int32_t ret = DistributedCloseSession(cameraDev);
    if (ret != HAL_SUCCESS) {
        DHLOGE("DistributedCloseSession Failed:%d", ret);
        return ret;
    }
    DeleteStream(cameraDev, streamId);
    for (auto it = g_distributedHalMgr.destoryCamera.begin(); it != g_distributedHalMgr.destoryCamera.end();) {
        if (*it == string(camera)) {
            g_distributedHalMgr.deviceInfo.erase(string(camera));
            delete cameraDev;
            it = g_distributedHalMgr.destoryCamera.erase(it);
        } else {
            ++it;
        }
    }
    DHLOGI("remove camera device[%s] success!", camera);
    HAL_EXIT();
    return HAL_SUCCESS;
}


int32_t DistributedHalCameraDequeueBuf(const char *camera, uint32_t streamId, HalBuffer *buffer)
{
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);

    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM || buffer == nullptr, HAL_FAILURE);

    return HAL_SUCCESS;
}

int32_t DistributedHalCameraQueueBuf(const char *camera, uint32_t streamId, const HalBuffer *buffer)
{
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);

    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM || buffer == nullptr, HAL_FAILURE);
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraSetBufferCallback(const char *camera, const BufferAvailable callback)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(callback == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->bufferCallBack != nullptr, HAL_FAILURE);
    cameraDev->bufferCallBack = callback;
    HAL_EXIT();
    return HAL_SUCCESS;
}

static int32_t DistributedConfigureStreams(CameraDevice *cameraDev, uint32_t streamId)
{
    if (cameraDev->isStreamConfigured_) {
        DHLOGI("isStreamConfigured_");
        return HAL_SUCCESS;
    }
    constexpr uint32_t dataspace = 8;
    StreamAttr *streamAttr = &cameraDev->internalStreamInfo[streamId].streamAttr;
    DCStreamInfo dcStreamInfo;
    dcStreamInfo.streamId_ = streamId;
    dcStreamInfo.width_ = streamAttr->width;
    dcStreamInfo.height_ = streamAttr->height;
    dcStreamInfo.stride_ = 0;
    dcStreamInfo.format_ = FORMAT_YVU420;
    dcStreamInfo.dataspace_ = dataspace;
    dcStreamInfo.encodeType_ = DCEncodeType::ENCODE_TYPE_H264;
    dcStreamInfo.type_ = DCStreamType::CONTINUOUS_FRAME;
    std::vector<DCStreamInfo> streamInfos;
    streamInfos.push_back(dcStreamInfo);
    int32_t ret = cameraDev->provider->ConfigureStreams(cameraDev->dhBase_, streamInfos);
    if (ret != HAL_SUCCESS) {
        DHLOGE("ConfigureStreams Failed:%d", ret);
        return ret;
    }
    cameraDev->isStreamConfigured_ = true;
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraStreamSetInfo(const char *camera, uint32_t streamId, const StreamInfo *info)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM || info == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->internalStreamInfo[streamId].isUsed == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->internalStreamInfo[streamId].isOpened == true, HAL_SUCCESS);
    DHLOGI("info->type :%d", info->type);
    if (info->type == STREAM_INFO_POS) {
        cameraDev->internalStreamInfo[streamId].pos.x = info->u.pos.x;
        cameraDev->internalStreamInfo[streamId].pos.y = info->u.pos.y;
        cameraDev->internalStreamInfo[streamId].pos.w = info->u.pos.w;
        cameraDev->internalStreamInfo[streamId].pos.h = info->u.pos.h;
    }
    int32_t ret = DistributedConfigureStreams(cameraDev, streamId);
    if (ret != HAL_SUCCESS) {
        DHLOGE("DistributedConfigureStreams Failed:%d", ret);
        return ret;
    }
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraStreamGetInfo(const char *camera, uint32_t streamId, StreamInfo *info)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM || info == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->internalStreamInfo[streamId].isUsed == false, HAL_FAILURE);
    if (info->type == STREAM_INFO_POS) {
        info->u.pos.x = cameraDev->internalStreamInfo[streamId].pos.x;
        info->u.pos.y = cameraDev->internalStreamInfo[streamId].pos.y;
        info->u.pos.w = cameraDev->internalStreamInfo[streamId].pos.w;
        info->u.pos.h = cameraDev->internalStreamInfo[streamId].pos.h;
    } else {
        DHLOGI("not support now\n");
    }
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraDeviceSetInfo(const char *camera, const DeviceInfo *info)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->isDevOpened == false, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(info == nullptr, HAL_FAILURE);
    DHLOGI("not support now\n");
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraDeviceGetInfo(const char *camera, DeviceInfo *info)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(info == nullptr, HAL_FAILURE);
    DHLOGI("not support now\n");
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraGetAbility(const char *camera, AbilityInfo *ability)
{
    HAL_ENTER();
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(ability == nullptr, HAL_FAILURE);
    HAL_LOG_DOFUNC(memset_s(ability, sizeof(AbilityInfo), 0, sizeof(AbilityInfo)));
    HAL_LOG_DOFUNC(strncpy_s(ability->cameraDesc, DESC_MAX_LEN - 1, CAMERA_RGB, sizeof(CAMERA_RGB)));
    ability->type = CAM_TYPE_WIDE_ANGLE;
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraSetResultCb(const char *camera, CameraResultCb cb)
{
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cb == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(cameraDev->resultCb != nullptr, HAL_FAILURE);
    cameraDev->resultCb = cb;
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraUpdateSettings(const char *camera, const char *value, uint32_t len)
{
    CameraDevice *cameraDev = GetCameraDevice(camera);
    LOG_CHK_RETURN_ERR(value == nullptr, HAL_FAILURE);
    std::string encodeString = Base64Encode(reinterpret_cast<const unsigned char *>(value), len);
    DCameraSettings dcSetting;
    dcSetting.type_ = DCSettingsType::UPDATE_METADATA;
    dcSetting.value_ = encodeString;

    std::vector<DCameraSettings> dcSettings;
    dcSettings.push_back(dcSetting);
    if (cameraDev->provider == nullptr) {
        DHLOGE("Distributed camera provider instance is null.");
        return HAL_FAILURE;
    }
    int32_t ret = cameraDev->provider->UpdateSettings(cameraDev->dhBase_, dcSettings);
    return ret;
}

int32_t DistributedHalCameraAddCameraDevice(const char *deviceId, const char *dhId, const char *abilityInfo,
    void *callbackObj)
{
    HAL_ENTER();
    DHLOGI("deviceId :%s dhId:%s abilityInfo:%s\n", deviceId, dhId, abilityInfo);
    std::string cameraId = std::string(deviceId) + "_" + std::string(dhId);
    CameraDevice *cameraDev = GetCameraDevice(cameraId.c_str());
    LOG_CHK_RETURN_ERR(cameraDev != nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(abilityInfo == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(callbackObj == nullptr, HAL_FAILURE);
    CameraDevice *addCameraDev = new CameraDevice();
    LOG_CHK_RETURN_ERR(addCameraDev == nullptr, HAL_FAILURE);
    addCameraDev->providerCallback = (IDCameraProviderCallback *)callbackObj;
    addCameraDev->abilityInfo = string(abilityInfo);
    if (g_distributedHalMgr.cameraDetect) {
        DHLOGI("cameraDetect :%s\n", cameraId.c_str());
        g_distributedHalMgr.cameraDetect(cameraId.c_str(), CAMERA_STATUS_PRESENT);
    }
    addCameraDev->provider = DCameraProvider::GetInstance();
    addCameraDev->dhBase_.deviceId_ = string(deviceId);
    addCameraDev->dhBase_.dhId_ = string(dhId);
    g_distributedHalMgr.deviceInfo[cameraId] = addCameraDev;
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraRemoveCameraDevice(const char *deviceId, const char *dhId)
{
    HAL_ENTER();
    DHLOGI("deviceId :%s dhId:%s\n", deviceId, dhId);
    LOG_CHK_RETURN_ERR(deviceId == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(dhId == nullptr, HAL_FAILURE);
    std::string cameraId = std::string(deviceId) + "_" + std::string(dhId);
    CameraDevice *cameraDev = GetCameraDevice(cameraId.c_str());
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    if (g_distributedHalMgr.cameraDetect) {
        DHLOGI("RemoveCameraDevice :%s\n", cameraId.c_str());
        g_distributedHalMgr.cameraDetect(cameraId.c_str(), CAMERA_STATUS_NOT_PRESENT);
    }
    g_distributedHalMgr.destoryCamera.push_back(cameraId);
    cameraDev->providerCallback = nullptr;
    HAL_EXIT();
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraAcquireBuffer(const char *deviceId, const char *dhId, int32_t streamId, HalBuffer *buffer)
{
    LOG_CHK_RETURN_ERR(deviceId == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(dhId == nullptr, HAL_FAILURE);
    std::string cameraId = std::string(deviceId) + "_" + std::string(dhId);
    CameraDevice *cameraDev = GetCameraDevice(cameraId.c_str());
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(buffer == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM || streamId < 0, HAL_FAILURE);
    std::shared_ptr<HalBuffer> halbuf = cameraDev->bufferManager.AcquireBuffer();
    if (halbuf == nullptr) {
        DHLOGE("AcquireBuffer failed");
        return HAL_FAILURE;
    }
    buffer->virAddr = halbuf->virAddr;
    buffer->size = halbuf->size;
    buffer->flag = halbuf->flag;
    return HAL_SUCCESS;
}

static void ConvertToInputBuffer(const HalBuffer *buffer, InputInfo &inputBuffer)
{
    inputBuffer.buffers->type = BUFFER_TYPE_VIRTUAL;
    inputBuffer.buffers->addr = (uint8_t *)buffer->virAddr;
    inputBuffer.buffers->offset = 0; /**< Buffer offset */
    inputBuffer.buffers->length = buffer->size; /**< Length of valid data */
    inputBuffer.buffers->size = buffer->size;
}

int32_t DistributedHalCameraShutterBuffer(const char *deviceId, const char *dhId,
    int32_t streamId, const HalBuffer *buffer)
{
    LOG_CHK_RETURN_ERR(deviceId == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(dhId == nullptr, HAL_FAILURE);
    std::string cameraId = std::string(deviceId) + "_" + std::string(dhId);
    CameraDevice *cameraDev = GetCameraDevice(cameraId.c_str());
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(buffer == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(streamId >= CAEMRA_MAX_STREAM_NUM || streamId < 0, HAL_FAILURE);
    InputInfo inputBuffer = {};
    CodecBufferInfo buffers = {};
    inputBuffer.bufferCnt = 1;
    inputBuffer.buffers = &buffers;
    ConvertToInputBuffer(buffer, inputBuffer);
#ifdef ENABLE_DFX
    MediaPrintSyncTimeDiffUs();
#endif
    int32_t ret = cameraDev->decoder.QueueInputBuffer(inputBuffer, 0);
    if (ret != HAL_SUCCESS) {
        DHLOGE("QueueInputBuffer failed = %d.", ret);
    }
    std::shared_ptr<HalBuffer> halbuf = std::make_shared<HalBuffer>();
    if (halbuf == nullptr) {
        DHLOGE(" make_shared Buffer failed");
        return HAL_FAILURE;
    }
    halbuf->virAddr = buffer->virAddr;
    halbuf->size = buffer->size;
    halbuf->flag = buffer->flag;
    RetCode retCode = cameraDev->bufferManager.ShutterBuffer(halbuf);
    if (retCode == Ret::RC_ERROR) {
        DHLOGE("ShutterBuffer failed ret:%u", retCode);
    }
    return HAL_SUCCESS;
}

int32_t DistributedHalCameraOnSettingsResult(const char *deviceId, const char *dhId, const void *result)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(deviceId == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(dhId == nullptr, HAL_FAILURE);
    std::string cameraId = std::string(deviceId) + "_" + std::string(dhId);
    CameraDevice *cameraDev = GetCameraDevice(cameraId.c_str());
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(result == nullptr, HAL_FAILURE);
    HAL_EXIT();
    return HAL_SUCCESS;
}

static void IsOpenSessFailedState(CameraDevice *cameraDev, bool state)
{
    {
        std::unique_lock<std::mutex> lock(cameraDev->isOpenSessFailedlock_);
        cameraDev->isOpenSessFailed_ = state;
    }
    cameraDev->openSessCV_.notify_one();
}

static void NotifyStartCaptureError(CameraDevice *cameraDev)
{
    std::vector<int32_t> streamIds = GetStreamIds(cameraDev);
    if (!streamIds.empty()) {
        cameraDev->provider->StopCapture(cameraDev->dhBase_, streamIds);
    }
}

static void NotifyCameraError(CameraDevice *cameraDev, const DCameraEventResult eventResult)
{
}

static int32_t CheckNotifyEvent(CameraDevice **cameraDev, const char *deviceId, const char *dhId, const void *event)
{
    LOG_CHK_RETURN_ERR(deviceId == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(dhId == nullptr, HAL_FAILURE);
    std::string cameraId = std::string(deviceId) + "_" + std::string(dhId);
    *cameraDev = GetCameraDevice(cameraId.c_str());
    LOG_CHK_RETURN_ERR(*cameraDev == nullptr, HAL_FAILURE);
    LOG_CHK_RETURN_ERR(event == nullptr, HAL_FAILURE);
    return HAL_SUCCESS;
}

static int32_t CheckNotifyType(const DCameraHDFEvent *dCameraEvent)
{
    DHLOGI("Notify for event type = %d, result = %d, content = %s.",
        dCameraEvent->type_, dCameraEvent->result_, dCameraEvent->content_.c_str());
    if ((dCameraEvent->type_ != DCameraEventType::DCAMERA_MESSAGE) &&
        (dCameraEvent->type_ != DCameraEventType::DCAMERA_OPERATION)) {
        DHLOGE("Invalid distributed camera event type = %d.", dCameraEvent->type_);
        return HAL_FAILURE;
    }
    return HAL_SUCCESS;
}

static void DispatchCameraNotify(CameraDevice *cameraDev, const DCameraHDFEvent *dCameraEvent)
{
    switch (dCameraEvent->result_) {
        case DCameraEventResult::DCAMERA_EVENT_CHANNEL_CONNECTED:
            IsOpenSessFailedState(cameraDev, false);
            break;
        case DCameraEventResult::DCAMERA_EVENT_OPEN_CHANNEL_ERROR:
            IsOpenSessFailedState(cameraDev, true);
            break;
        case DCameraEventResult::DCAMERA_EVENT_CHANNEL_DISCONNECTED:
            NotifyCameraError(cameraDev, DCameraEventResult::DCAMERA_EVENT_CHANNEL_DISCONNECTED);
            break;
        case DCameraEventResult::DCAMERA_EVENT_CONFIG_STREAMS_ERROR:
        case DCameraEventResult::DCAMERA_EVENT_START_CAPTURE_ERROR:
            NotifyStartCaptureError(cameraDev);
            break;
        case DCameraEventResult::DCAMERA_EVENT_DEVICE_ERROR:
            NotifyCameraError(cameraDev, DCameraEventResult::DCAMERA_EVENT_DEVICE_ERROR);
            break;
        case DCameraEventResult::DCAMERA_EVENT_DEVICE_PREEMPT:
            NotifyCameraError(cameraDev, DCameraEventResult::DCAMERA_EVENT_DEVICE_PREEMPT);
            break;
        case DCameraEventResult::DCAMERA_EVENT_DEVICE_IN_USE:
            NotifyCameraError(cameraDev, DCameraEventResult::DCAMERA_EVENT_DEVICE_IN_USE);
            break;
        case DCameraEventResult::DCAMERA_EVENT_NO_PERMISSION:
            NotifyCameraError(cameraDev, DCameraEventResult::DCAMERA_EVENT_NO_PERMISSION);
            break;
        default:
            break;
    }
}

int32_t DistributedHalCameraNotify(const char *deviceId, const char *dhId, const void *event)
{
    HAL_ENTER();
    CameraDevice *cameraDev = nullptr;
    if (CheckNotifyEvent(&cameraDev, deviceId, dhId, event) != HAL_SUCCESS) {
        return HAL_FAILURE;
    }
    DCameraHDFEvent *dCameraEvent = (DCameraHDFEvent *)event;
    if (CheckNotifyType(dCameraEvent) != HAL_SUCCESS) {
        return HAL_FAILURE;
    }
    DispatchCameraNotify(cameraDev, dCameraEvent);
    HAL_EXIT();
    return HAL_SUCCESS;
}

void *DistributedHalCameraGetProviderCallback(const char *deviceId, const char *dhId)
{
    HAL_ENTER();
    LOG_CHK_RETURN_ERR(deviceId == nullptr, nullptr);
    LOG_CHK_RETURN_ERR(dhId == nullptr, nullptr);
    std::string cameraId = std::string(deviceId) + "_" + std::string(dhId);
    CameraDevice *cameraDev = GetCameraDevice(cameraId.c_str());
    LOG_CHK_RETURN_ERR(cameraDev == nullptr, nullptr);
    HAL_EXIT();
    return cameraDev->providerCallback;
}

static HalCameraManager g_distributedHalCameraManager = {
    .HalCameraInit = DistributedHalCameraInit,
    .HalCameraDeinit = DistributedHalCameraDeinit,
    .HalCameraGetModeNum = DistributedHalCameraGetModeNum,
    .HalCameraSetMode = DistributedHalCameraSetMode,
    .HalCameraSetDeviceDetectCb = DistributedHalCameraSetDeviceDetectCb,
    .HalCameraGetDeviceNum = DistributedHalCameraGetDeviceNum,
    .HalCameraGetDeviceList = DistributedHalCameraGetDeviceList,
    .HalCameraGetStreamCapNum = DistributedHalCameraGetStreamCapNum,
    .HalCameraGetStreamCap = DistributedHalCameraGetStreamCap,
    .HalCameraDeviceOpen = DistributedHalCameraDeviceOpen,
    .HalCameraDeviceClose = DistributedHalCameraDeviceClose,
    .HalCameraStreamCreate = DistributedHalCameraStreamCreate,
    .HalCameraStreamDestroy = DistributedHalCameraStreamDestroy,
    .HalCameraGetDeviceId = DistributedHalCameraGetDeviceId,
    .HalCameraStreamOn = DistributedHalCameraStreamOn,
    .HalCameraStreamOff = DistributedHalCameraStreamOff,
    .HalCameraDequeueBuf = DistributedHalCameraDequeueBuf,
    .HalCameraQueueBuf = DistributedHalCameraQueueBuf,
    .HalCameraSetBufferCallback = DistributedHalCameraSetBufferCallback,
    .HalCameraStreamSetInfo = DistributedHalCameraStreamSetInfo,
    .HalCameraStreamGetInfo = DistributedHalCameraStreamGetInfo,
    .HalCameraDeviceSetInfo = DistributedHalCameraDeviceSetInfo,
    .HalCameraDeviceGetInfo = DistributedHalCameraDeviceGetInfo,
    .HalCameraGetAbility = DistributedHalCameraGetAbility,
    .HalCameraSetResultCb = DistributedHalCameraSetResultCb,
    .HalCameraUpdateSettings = DistributedHalCameraUpdateSettings,
    .HalCameraAddCameraDevice = DistributedHalCameraAddCameraDevice,
    .HalCameraRemoveCameraDevice = DistributedHalCameraRemoveCameraDevice,
    .HalCameraAcquireBuffer = DistributedHalCameraAcquireBuffer,
    .HalCameraShutterBuffer = DistributedHalCameraShutterBuffer,
    .HalCameraOnSettingsResult = DistributedHalCameraOnSettingsResult,
    .HalCameraNotify = DistributedHalCameraNotify,
    .HalCameraGetProviderCallback = DistributedHalCameraGetProviderCallback,
};

HalCameraManager *GetHalDistributedCameraFuncs(void)
{
    return &g_distributedHalCameraManager;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
