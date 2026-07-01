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

#ifndef DISTRIBUTED_CAMERA_PROVIDER_H
#define DISTRIBUTED_CAMERA_PROVIDER_H
#include <memory>
#include "v1_0/id_camera_provider.h"
#include "v1_0/dcamera_types.h"
#include "hal_camera.h"

namespace OHOS {
namespace DistributedHardware {
using namespace OHOS::HDI::DistributedCamera::V1_0;

class DCameraProvider : public IDCameraProvider {
const uint32_t ABILITYINFO_MAX_LENGTH = 50 * 1024 * 1024;
const uint32_t HDF_EVENT_CONTENT_MAX_LENGTH = 50 * 1024 * 1024;
const uint32_t SETTING_VALUE_MAX_LENGTH = 50 * 1024 * 1024;
public:
    DCameraProvider();
    ~DCameraProvider() = default;
    DCameraProvider(const DCameraProvider &other) = delete;
    DCameraProvider(DCameraProvider &&other) = delete;
    DCameraProvider& operator=(const DCameraProvider &other) = delete;
    DCameraProvider& operator=(DCameraProvider &&other) = delete;

public:
    static DCameraProvider *GetInstance();
    int32_t EnableDCameraDevice(const DHBase& dhBase, const std::string& abilityInfo,
        const std::shared_ptr<IDCameraProviderCallback>& callbackObj) override;
    int32_t DisableDCameraDevice(const DHBase& dhBase) override;
    int32_t AcquireBuffer(const DHBase& dhBase, int32_t streamId, DCameraBuffer& buffer) override;
    int32_t ShutterBuffer(const DHBase& dhBase, int32_t streamId, const DCameraBuffer& buffer) override;
    int32_t OnSettingsResult(const DHBase& dhBase, const DCameraSettings& result) override;
    int32_t Notify(const DHBase& dhBase, const DCameraHDFEvent& event) override;

    int32_t OpenSession(const DHBase &dhBase);
    int32_t CloseSession(const DHBase &dhBase);
    int32_t ConfigureStreams(const DHBase &dhBase, const std::vector<DCStreamInfo> &streamInfos);
    int32_t ReleaseStreams(const DHBase &dhBase, const std::vector<int> &streamIds);
    int32_t StartCapture(const DHBase &dhBase, const std::vector<DCCaptureInfo> &captureInfos);
    int32_t StopCapture(const DHBase &dhBase, const std::vector<int> &streamIds);
    int32_t UpdateSettings(const DHBase &dhBase, const std::vector<DCameraSettings> &settings);

private:
    bool IsDCameraSettingsInvalid(const DCameraSettings& result);
    bool IsDCameraHDFEventInvalid(const DCameraHDFEvent& event);
    IDCameraProviderCallback *GetCallbackBydhBase(const DHBase &dhBase);

private:
    HalCameraManager *distributedHalCameraDev_ = nullptr;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // DISTRIBUTED_CAMERA_PROVIDER_H