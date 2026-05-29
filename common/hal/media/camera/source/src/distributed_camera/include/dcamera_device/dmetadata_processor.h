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

#ifndef DISTRIBUTED_CAMERA_METADATA_PROCESSOR_H
#define DISTRIBUTED_CAMERA_METADATA_PROCESSOR_H

#include <set>
#include <map>
#include <mutex>
#include <vector>
#include "constants.h"
#include "dcamera.h"
#include "json/json.h"
#include "v1_0/dcamera_types.h"
#include "v1_0/types.h"

namespace OHOS {
namespace DistributedHardware {
using namespace OHOS::HDI::Camera::V1_0;
using namespace OHOS::HDI::DistributedCamera::V1_0;
class DMetadataProcessor {
public:
    DMetadataProcessor() = default;
    ~DMetadataProcessor() = default;
    DMetadataProcessor(const DMetadataProcessor &other) = delete;
    DMetadataProcessor(DMetadataProcessor &&other) = delete;
    DMetadataProcessor& operator=(const DMetadataProcessor &other) = delete;
    DMetadataProcessor& operator=(DMetadataProcessor &&other) = delete;

public:
    DCamRetCode InitDCameraAbility(const std::string &abilityInfo);
    DCamRetCode GetDCameraAbility(std::shared_ptr<CameraAbility> &ability);
    DCamRetCode SetMetadataResultMode(const ResultCallbackMode &mode);
    DCamRetCode GetEnabledMetadataResults(std::vector<MetaType> &results);
    DCamRetCode EnableMetadataResult(const std::vector<MetaType> &results);
    DCamRetCode DisableMetadataResult(const std::vector<MetaType> &results);
    DCamRetCode ResetEnableResults();
    DCamRetCode SaveResultMetadata(std::string resultStr);
    void UpdateResultMetadata(const uint64_t &resultTimestamp);
    void SetResultCallback(std::function<void(uint64_t, std::shared_ptr<OHOS::Camera::CameraMetadata>)> &resultCbk);
    void PrintDCameraMetadata(const common_metadata_header_t *metadata);

private:
    DCamRetCode InitDCameraDefaultAbilityKeys(const std::string &abilityInfo);
    DCamRetCode InitDCameraOutputAbilityKeys(const std::string &abilityInfo);
    DCamRetCode AddAbilityEntry(uint32_t tag, const void *data, size_t size);
    DCamRetCode UpdateAbilityEntry(uint32_t tag, const void *data, size_t size);
    void ConvertToCameraMetadata(common_metadata_header_t *&input,
        std::shared_ptr<OHOS::Camera::CameraMetadata> &output);
    void ResizeMetadataHeader(common_metadata_header_t *&header, uint32_t itemCapacity, uint32_t dataCapacity);
    void UpdateAllResult(const uint64_t &resultTimestamp);
    void UpdateOnChanged(const uint64_t &resultTimestamp);
    bool IsMetadataItemChanged(const camera_metadata_item_t &item, const camera_metadata_item_t &anoItem);
    uint32_t GetDataSize(uint32_t type);
    void* GetMetadataItemData(const camera_metadata_item_t &item);
    std::map<int, std::vector<DCResolution>> GetDCameraSupportedFormats(const std::string &abilityInfo);
    void ParsePhotoFormats(Json::Value& rootValue, std::map<int, std::vector<DCResolution>>& supportedFormats);
    void ParsePreviewFormats(Json::Value& rootValue, std::map<int, std::vector<DCResolution>>& supportedFormats);
    void ParseVideoFormats(Json::Value& rootValue, std::map<int, std::vector<DCResolution>>& supportedFormats);
    void InitDcameraBaseAbility();
    void ParseDefaultAbilityInfo(const std::string &abilityInfo);
    void AddDefaultPositionAbility();
    void AddDefaultControlAbility();
    void AddDefaultSensorAbility();
    void GetEachNodeSupportedResolution(std::vector<int>& formats, const std::string rootNode,
        std::map<int, std::vector<DCResolution>>& supportedFormats, Json::Value& rootValue);
    void GetNodeSupportedResolution(int format, std::string formatStr, const std::string rootNode,
        std::map<int, std::vector<DCResolution>>& supportedFormats, Json::Value& rootValue);
    void SetFpsRanges();

private:
    constexpr static uint32_t JSON_ARRAY_MAX_SIZE = 1000;
    std::function<void(uint64_t, std::shared_ptr<OHOS::Camera::CameraMetadata>)> resultCallback_;
    std::shared_ptr<CameraAbility> dCameraAbility_;
    std::string protocolVersion_;
    std::string dCameraPosition_;
    DCResolution maxPreviewResolution_;
    DCResolution maxPhotoResolution_;
    ResultCallbackMode metaResultMode_;
    std::set<MetaType> allResultSet_;
    std::set<MetaType> enabledResultSet_;
    std::mutex producerMutex_;

    // The latest result metadata that received from the sink device.
    std::shared_ptr<OHOS::Camera::CameraMetadata> latestProducerMetadataResult_;

    // The latest result metadata that replied to the camera service.
    std::shared_ptr<OHOS::Camera::CameraMetadata> latestConsumerMetadataResult_;
};
} // namespace DistributedHardware
} // namespace OHOS
#endif // DISTRIBUTED_CAMERA_METADATA_PROCESSOR_H