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

#include "dcamera_provider.h"
#include "anonymous_string.h"
#include "constants.h"
#include "distributed_hardware_log.h"
#include "dcamera.h"

#define TAG "CameraProvider"

OHOS::HDI::DistributedCamera::V1_0::IDCameraProvider *CameraProviderHdiImplGetInstance(void)
{
    return static_cast<OHOS::HDI::DistributedCamera::V1_0::IDCameraProvider *>(
        OHOS::DistributedHardware::DCameraProvider::GetInstance());
}

namespace OHOS {
namespace DistributedHardware {

DCameraProvider* DCameraProvider::GetInstance()
{
    static DCameraProvider instance;
    return &instance;
}

DCameraProvider::DCameraProvider()
{
    distributedHalCameraDev_ = GetHalDistributedCameraFuncs();
    int32_t ret = distributedHalCameraDev_->HalCameraInit();
    DHLOGI("distributedHalCameraDev Init ret(%d)", ret);
}

int32_t DCameraProvider::EnableDCameraDevice(const DHBase& dhBase, const std::string& abilityInfo,
    const std::shared_ptr<IDCameraProviderCallback>& callbackObj)
{
    if (IsDhBaseInfoInvalid(dhBase)) {
        DHLOGE("EnableDCameraDevice, devId or dhId is invalid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    DHLOGI("EnableDCameraDevice for {devId: %s, dhId: %s, abilityInfo length: %d}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str(), abilityInfo.length());

    if (abilityInfo.empty() || abilityInfo.length() > ABILITYINFO_MAX_LENGTH) {
        DHLOGE("EnableDCameraDevice, dcamera ability is empty or over limit.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    if (callbackObj == nullptr) {
        DHLOGE("EnableDCameraDevice, dcamera provider callback is null.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    int32_t ret = DCamRetCode::SUCCESS;
    if (distributedHalCameraDev_ == nullptr) {
        DHLOGE("EnableDCameraDevice, distributedHalCameraDev is null.");
        return DCamRetCode::DEVICE_NOT_INIT;
    }
    ret = distributedHalCameraDev_->HalCameraAddCameraDevice(dhBase.deviceId_.c_str(), dhBase.dhId_.c_str(),
        abilityInfo.c_str(), callbackObj.get());
    if (ret != DCamRetCode::SUCCESS) {
        DHLOGE("EnableDCameraDevice failed, ret = %d.", ret);
    }
    DHLOGI("EnableDCameraDevice ret(%d)", ret);
    return ret;
}

int32_t DCameraProvider::DisableDCameraDevice(const DHBase& dhBase)
{
    if (IsDhBaseInfoInvalid(dhBase)) {
        DHLOGE("DisableDCameraDevice, devId or dhId is invalid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    DHLOGI("DisableDCameraDevice for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());
    int32_t ret = DCamRetCode::SUCCESS;
    if (distributedHalCameraDev_ == nullptr) {
        DHLOGE("DisableDCameraDevice, dcamera host is null.");
        return DCamRetCode::DEVICE_NOT_INIT;
    }
    ret = distributedHalCameraDev_->HalCameraRemoveCameraDevice(dhBase.deviceId_.c_str(), dhBase.dhId_.c_str());
    if (ret != DCamRetCode::SUCCESS) {
        DHLOGE("DisableDCameraDevice failed, ret = %d.", ret);
        return ret;
    }

    return ret;
}

static void ConvertToDCameraBuffer(const HalBuffer &halBuffer, DCameraBuffer &buffer)
{
     buffer.bufferHandle_.virAddr = halBuffer.virAddr;
     buffer.bufferHandle_.size = halBuffer.size;
     buffer.size_ = halBuffer.size;
     buffer.index_ = halBuffer.flag;
}

int32_t DCameraProvider::AcquireBuffer(const DHBase& dhBase, int32_t streamId, DCameraBuffer&buffer)
{
    if (IsDhBaseInfoInvalid(dhBase)) {
        DHLOGE("AcquireBuffer, devId or dhId is invalid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    if (streamId < 0) {
        DHLOGE("AcquireBuffer, input streamId is invalid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    if (distributedHalCameraDev_ == nullptr) {
        DHLOGE("AcquireBuffer failed, dcamera device not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    HalBuffer halBuffer = {};
    int32_t ret = distributedHalCameraDev_->HalCameraAcquireBuffer(dhBase.deviceId_.c_str(), dhBase.dhId_.c_str(),
        streamId, &halBuffer);
    if (ret != DCamRetCode::SUCCESS) {
        DHLOGE("AcquireBuffer failed, ret = %d.", ret);
        return ret;
    }
    ConvertToDCameraBuffer(halBuffer, buffer);
    return DCamRetCode::SUCCESS;
}

static void ConvertToHalBuffer(const DCameraBuffer &buffer, HalBuffer &halBuffer)
{
    halBuffer.virAddr = buffer.bufferHandle_.virAddr;
    halBuffer.size = buffer.size_;
}

int32_t DCameraProvider::ShutterBuffer(const DHBase& dhBase, int32_t streamId, const DCameraBuffer& buffer)
{
    if (IsDhBaseInfoInvalid(dhBase)) {
        DHLOGE("ShutterBuffer, devId or dhId is invalid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    if (buffer.index_ < 0 || buffer.size_ < 0) {
        DHLOGE("ShutterBuffer, input dcamera buffer is invalid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    if (streamId < 0) {
        DHLOGE("ShutterBuffer, input streamId is invalid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    if (distributedHalCameraDev_ == nullptr) {
        DHLOGE("ShutterBuffer failed, dcamera device not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    HalBuffer halBuffer = {};
    ConvertToHalBuffer(buffer, halBuffer);
    return distributedHalCameraDev_->HalCameraShutterBuffer(dhBase.deviceId_.c_str(), dhBase.dhId_.c_str(),
        streamId, &halBuffer);
}

int32_t DCameraProvider::OnSettingsResult(const DHBase& dhBase, const DCameraSettings& result)
{
    if (IsDhBaseInfoInvalid(dhBase)) {
        DHLOGE("OnSettingsResult, devId or dhId is invalid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    if (IsDCameraSettingsInvalid(result)) {
        DHLOGE("OnSettingsResult, input dcamera settings is valid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    DHLOGI("OnSettingsResult for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());

    if (distributedHalCameraDev_ == nullptr) {
        DHLOGE("OnSettingsResult failed, dcamera device not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    std::shared_ptr<DCameraSettings> dCameraResult = std::make_shared<DCameraSettings>();
    dCameraResult->type_ = result.type_;
    dCameraResult->value_ = result.value_;
    return distributedHalCameraDev_->HalCameraOnSettingsResult(dhBase.deviceId_.c_str(), dhBase.dhId_.c_str(),
        dCameraResult.get());
}

int32_t DCameraProvider::Notify(const DHBase& dhBase, const DCameraHDFEvent& event)
{
    if (IsDhBaseInfoInvalid(dhBase)) {
        DHLOGE("Notify, devId or dhId is invalid.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    if (IsDCameraHDFEventInvalid(event)) {
        DHLOGE("Notify, input dcamera hdf event is null.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    DHLOGI("Notify for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());

    if (distributedHalCameraDev_ == nullptr) {
        DHLOGE("Notify failed, dcamera device not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    std::shared_ptr<DCameraHDFEvent> dCameraEvent = std::make_shared<DCameraHDFEvent>();
    dCameraEvent->type_ = event.type_;
    dCameraEvent->result_ = event.result_;
    dCameraEvent->content_ = event.content_;
    return distributedHalCameraDev_->HalCameraNotify(dhBase.deviceId_.c_str(), dhBase.dhId_.c_str(),
        dCameraEvent.get());
}

int32_t DCameraProvider::OpenSession(const DHBase &dhBase)
{
    DHLOGI("OpenSession for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());
    IDCameraProviderCallback *callback = GetCallbackBydhBase(dhBase);

    if (callback == nullptr) {
        DHLOGE("OpenSession, dcamera provider callback not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    return callback->OpenSession(dhBase);
}

int32_t DCameraProvider::CloseSession(const DHBase &dhBase)
{
    DHLOGI("CloseSession for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());
    IDCameraProviderCallback *callback = GetCallbackBydhBase(dhBase);
    if (callback == nullptr) {
        DHLOGE("CloseSession, dcamera provider callback not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    return callback->CloseSession(dhBase);
}

int32_t DCameraProvider::ConfigureStreams(const DHBase &dhBase, const std::vector<DCStreamInfo> &streamInfos)
{
    DHLOGI("ConfigureStreams for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());
    IDCameraProviderCallback *callback = GetCallbackBydhBase(dhBase);
    if (callback == nullptr) {
        DHLOGE("DCameraProvider::ConfigStreams, dcamera provider callback not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    for (auto info = streamInfos.begin(); info != streamInfos.end(); info++) {
        DHLOGI("ConfigureStreams: id=%d, width=%d, height=%d, format=%d, " +
               "type=%d.", info->streamId_, info->width_, info->height_, info->format_, info->type_);
    }
    return callback->ConfigureStreams(dhBase, streamInfos);
}

int32_t DCameraProvider::ReleaseStreams(const DHBase &dhBase, const std::vector<int> &streamIds)
{
    DHLOGI("ReleaseStreams for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());
    IDCameraProviderCallback *callback = GetCallbackBydhBase(dhBase);
    if (callback == nullptr) {
        DHLOGE("ReleaseStreams, dcamera provider callback not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    std::string idString = "";
    for (int id : streamIds) {
        idString += (std::to_string(id) + ", ");
    }
    DHLOGI("ReleaseStreams: ids=[%s].", idString.c_str());
    return callback->ReleaseStreams(dhBase, streamIds);
}

int32_t DCameraProvider::StartCapture(const DHBase &dhBase, const std::vector<DCCaptureInfo> &captureInfos)
{
    DHLOGI("StartCapture for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());
    IDCameraProviderCallback *callback = GetCallbackBydhBase(dhBase);
    if (callback == nullptr) {
        DHLOGE("StartCapture, dcamera provider callback not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    for (auto info = captureInfos.begin(); info != captureInfos.end(); info++) {
        std::string idString = "";
        for (int id : info->streamIds_) {
            idString += (std::to_string(id) + ", ");
        }
        DHLOGI("StartCapture: ids=[%s], width=%d, height=%d, format=%d, type=%d, isCapture=%d.",
            (idString.empty() ? idString.c_str() : (idString.substr(0, idString.length() - INGNORE_STR_LEN)).c_str()),
            info->width_, info->height_, info->format_, info->type_, info->isCapture_);
    }
    return callback->StartCapture(dhBase, captureInfos);
}

int32_t DCameraProvider::StopCapture(const DHBase &dhBase, const std::vector<int> &streamIds)
{
    DHLOGI("StopCapture for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());
    IDCameraProviderCallback *callback = GetCallbackBydhBase(dhBase);
    if (callback == nullptr) {
        DHLOGE("StopCapture, dcamera provider callback not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    std::string idString = "";
    for (int id : streamIds) {
        idString += (std::to_string(id) + ", ");
    }
    DHLOGI("StopCapture: ids=[%s].",
        idString.empty() ? idString.c_str() : (idString.substr(0, idString.length() - INGNORE_STR_LEN)).c_str());
    return callback->StopCapture(dhBase, streamIds);
}

int32_t DCameraProvider::UpdateSettings(const DHBase &dhBase, const std::vector<DCameraSettings> &settings)
{
    DHLOGI("UpdateSettings for {devId: %s, dhId: %s}.",
        GetAnonyString(dhBase.deviceId_).c_str(), GetAnonyString(dhBase.dhId_).c_str());
    IDCameraProviderCallback *callback = GetCallbackBydhBase(dhBase);
    if (callback == nullptr) {
        DHLOGE("UpdateSettings, dcamera provider callback not found.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    return callback->UpdateSettings(dhBase, settings);
}

bool DCameraProvider::IsDCameraSettingsInvalid(const DCameraSettings& result)
{
    return result.value_.empty() || result.value_.length() > SETTING_VALUE_MAX_LENGTH;
}

bool DCameraProvider::IsDCameraHDFEventInvalid(const DCameraHDFEvent& event)
{
    return event.content_.length() > HDF_EVENT_CONTENT_MAX_LENGTH;
}

IDCameraProviderCallback *DCameraProvider::GetCallbackBydhBase(const DHBase &dhBase)
{
    void *callback = distributedHalCameraDev_->HalCameraGetProviderCallback(
        dhBase.deviceId_.c_str(), dhBase.dhId_.c_str());
    if (callback == nullptr) {
        DHLOGE("GetCallbackBydhBase failed, dcamera device not found.");
        return nullptr;
    }
    return (IDCameraProviderCallback *)callback;
}

} // namespace DistributedHardware
} // namespace OHOS
