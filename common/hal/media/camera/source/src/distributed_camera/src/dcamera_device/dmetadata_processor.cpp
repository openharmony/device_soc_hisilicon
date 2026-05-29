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

#include "dmetadata_processor.h"

#include "dbuffer_manager.h"
#include "dcamera.h"
#include "distributed_hardware_log.h"
#include "json/json.h"
#include "metadata_utils.h"

namespace OHOS {
namespace DistributedHardware {
DCamRetCode DMetadataProcessor::InitDCameraAbility(const std::string &abilityInfo)
{
    JSONCPP_STRING errs;
    Json::CharReaderBuilder readerBuilder;
    Json::Value rootValue;

    std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
    if (jsonReader->parse(abilityInfo.c_str(), abilityInfo.c_str() + abilityInfo.length(), &rootValue, &errs) &&
        rootValue.isObject()) {
        if (rootValue.isMember("MetaData") && rootValue["MetaData"].isString()) {
            std::string metadataStr = rootValue["MetaData"].asString();
            if (!metadataStr.empty()) {
                std::hash<std::string> h;
                DHLOGI("Decode distributed camera metadata from base64, hash: %zu, length: %zu",
                    h(metadataStr), metadataStr.length());
                std::string decodeString = Base64Decode(metadataStr);
                DHLOGI("Decode distributed camera metadata from string, hash: %zu, length: %zu",
                    h(decodeString), decodeString.length());
                dCameraAbility_ = OHOS::Camera::MetadataUtils::DecodeFromString(decodeString);
                DHLOGI("Decode distributed camera metadata from string success.");
            }
        }
    }

    if (dCameraAbility_ == nullptr) {
        DHLOGE("Metadata is null in ability set or failed to decode metadata ability from string.");
        dCameraAbility_ = std::make_shared<CameraAbility>(DEFAULT_ENTRY_CAPACITY, DEFAULT_DATA_CAPACITY);
    }

    if (OHOS::Camera::GetCameraMetadataItemCount(dCameraAbility_->get()) <= 0) {
        DCamRetCode ret = InitDCameraDefaultAbilityKeys(abilityInfo);
        if (ret != SUCCESS) {
            DHLOGE("Init distributed camera defalult abilily keys failed.");
            dCameraAbility_ = nullptr;
            return ret;
        }
    }

    DCamRetCode ret = InitDCameraOutputAbilityKeys(abilityInfo);
    if (ret != SUCCESS) {
        DHLOGE("Init distributed camera output abilily keys failed.");
        dCameraAbility_ = nullptr;
        return ret;
    }

    camera_metadata_item_entry_t* itemEntry = OHOS::Camera::GetMetadataItems(dCameraAbility_->get());
    uint32_t count = dCameraAbility_->get()->item_count;
    for (uint32_t i = 0; i < count; i++, itemEntry++) {
        allResultSet_.insert((MetaType)(itemEntry->item));
    }
    return SUCCESS;
}

void DMetadataProcessor::InitDcameraBaseAbility()
{
    const uint8_t cameraType = OHOS_CAMERA_TYPE_LOGICAL;
    AddAbilityEntry(OHOS_ABILITY_CAMERA_TYPE, &cameraType, 1);

    const int64_t exposureTime = 0xFFFFFFFFFFFFFFFF;
    AddAbilityEntry(OHOS_SENSOR_EXPOSURE_TIME, &exposureTime, 1);

    const float correctionGain = 0.0;
    AddAbilityEntry(OHOS_SENSOR_COLOR_CORRECTION_GAINS, &correctionGain, 1);

    const uint8_t faceDetectMode = OHOS_CAMERA_FACE_DETECT_MODE_OFF;
    AddAbilityEntry(OHOS_STATISTICS_FACE_DETECT_MODE, &faceDetectMode, 1);

    const uint8_t histogramMode = OHOS_CAMERA_HISTOGRAM_MODE_OFF;
    AddAbilityEntry(OHOS_STATISTICS_HISTOGRAM_MODE, &histogramMode, 1);

    const uint8_t aeAntibandingMode = OHOS_CAMERA_AE_ANTIBANDING_MODE_OFF;
    AddAbilityEntry(OHOS_CONTROL_AE_ANTIBANDING_MODE, &aeAntibandingMode, 1);

    int32_t aeExposureCompensation = 0xFFFFFFFF;
    AddAbilityEntry(OHOS_CONTROL_AE_EXPOSURE_COMPENSATION, &aeExposureCompensation, 1);

    const uint8_t aeLock = OHOS_CAMERA_AE_LOCK_OFF;
    AddAbilityEntry(OHOS_CONTROL_AE_LOCK, &aeLock, 1);

    const uint8_t aeMode = OHOS_CAMERA_AE_MODE_OFF;
    AddAbilityEntry(OHOS_CONTROL_AE_MODE, &aeMode, 1);

    const uint8_t afMode = OHOS_CAMERA_AF_MODE_OFF;
    AddAbilityEntry(OHOS_CONTROL_AF_MODE, &afMode, 1);

    const uint8_t awbLock = OHOS_CAMERA_AWB_LOCK_OFF;
    AddAbilityEntry(OHOS_CONTROL_AWB_LOCK, &awbLock, 1);

    const uint8_t awbMode = OHOS_CAMERA_AWB_MODE_OFF;
    AddAbilityEntry(OHOS_CONTROL_AWB_MODE, &awbMode, 1);

    const uint8_t aeAntibandingModes = OHOS_CAMERA_AE_ANTIBANDING_MODE_AUTO;
    AddAbilityEntry(OHOS_CONTROL_AE_AVAILABLE_ANTIBANDING_MODES, &aeAntibandingModes, 1);

    const uint8_t aeAvailableModes = OHOS_CAMERA_AE_MODE_ON;
    AddAbilityEntry(OHOS_CONTROL_AE_AVAILABLE_MODES, &aeAvailableModes, 1);

    const int32_t compensationRange[] = { 0, 0 };
    AddAbilityEntry(OHOS_CONTROL_AE_COMPENSATION_RANGE, compensationRange,
        (sizeof(compensationRange) / sizeof(compensationRange[0])));

    const camera_rational_t compensationStep[] = { { 0, 1 } };
    AddAbilityEntry(OHOS_CONTROL_AE_COMPENSATION_STEP, compensationStep,
        (sizeof(compensationStep) / sizeof(compensationStep[0])));

    const uint8_t afAvailableModes[] = { OHOS_CAMERA_AF_MODE_AUTO, OHOS_CAMERA_AF_MODE_OFF };
    AddAbilityEntry(OHOS_CONTROL_AF_AVAILABLE_MODES, afAvailableModes,
        (sizeof(afAvailableModes) / sizeof(afAvailableModes[0])));

    const uint8_t awbAvailableModes = OHOS_CAMERA_AWB_MODE_AUTO;
    AddAbilityEntry(OHOS_CONTROL_AWB_AVAILABLE_MODES, &awbAvailableModes, 1);

    const uint8_t deviceExposureMode = OHOS_CAMERA_EXPOSURE_MODE_CONTINUOUS_AUTO;
    AddAbilityEntry(OHOS_ABILITY_DEVICE_AVAILABLE_EXPOSUREMODES, &deviceExposureMode, 1);

    const uint8_t controlExposureMode = OHOS_CAMERA_EXPOSURE_MODE_CONTINUOUS_AUTO;
    AddAbilityEntry(OHOS_CONTROL_EXPOSUREMODE, &controlExposureMode, 1);

    const uint8_t deviceFocusModes = OHOS_CAMERA_FOCUS_MODE_AUTO;
    AddAbilityEntry(OHOS_ABILITY_DEVICE_AVAILABLE_FOCUSMODES, &deviceFocusModes, 1);
    SetFpsRanges();
}

void DMetadataProcessor::SetFpsRanges()
{
    std::vector<int32_t> fpsRanges;
    fpsRanges.push_back(MIN_SUPPORT_DEFAULT_FPS);
    fpsRanges.push_back(MAX_SUPPORT_DEFAULT_FPS);
    AddAbilityEntry(OHOS_CONTROL_AE_TARGET_FPS_RANGE, fpsRanges.data(), fpsRanges.size());
    AddAbilityEntry(OHOS_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES, fpsRanges.data(), fpsRanges.size());
}

void DMetadataProcessor::ParseDefaultAbilityInfo(const std::string &abilityInfo)
{
    JSONCPP_STRING errs;
    Json::CharReaderBuilder readerBuilder;
    Json::Value rootValue;

    std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
    if (jsonReader->parse(abilityInfo.c_str(), abilityInfo.c_str() + abilityInfo.length(), &rootValue, &errs) &&
        rootValue.isObject()) {
        if (rootValue.isMember("ProtocolVer") && rootValue["ProtocolVer"].isString()) {
            protocolVersion_ = rootValue["ProtocolVer"].asString();
        }
        if (rootValue.isMember("Position") && rootValue["Position"].isString()) {
            dCameraPosition_ = rootValue["Position"].asString();
        }
    }
}

void DMetadataProcessor::AddDefaultPositionAbility()
{
    if (dCameraPosition_ == "BACK") {
        const uint8_t position = OHOS_CAMERA_POSITION_BACK;
        AddAbilityEntry(OHOS_ABILITY_CAMERA_POSITION, &position, 1);
    } else if (dCameraPosition_ == "FRONT") {
        const uint8_t position = OHOS_CAMERA_POSITION_FRONT;
        AddAbilityEntry(OHOS_ABILITY_CAMERA_POSITION, &position, 1);
    } else {
        const uint8_t position = OHOS_CAMERA_POSITION_OTHER;
        AddAbilityEntry(OHOS_ABILITY_CAMERA_POSITION, &position, 1);
    }
}

void DMetadataProcessor::AddDefaultControlAbility()
{
    const uint8_t controlFocusMode = OHOS_CAMERA_FOCUS_MODE_AUTO;
    AddAbilityEntry(OHOS_CONTROL_FOCUSMODE, &controlFocusMode, 1);

    const uint8_t deviceFlashModes = OHOS_CAMERA_FLASH_MODE_AUTO;
    AddAbilityEntry(OHOS_ABILITY_DEVICE_AVAILABLE_FLASHMODES, &deviceFlashModes, 1);

    const uint8_t controlFlashMode = OHOS_CAMERA_FLASH_MODE_CLOSE;
    AddAbilityEntry(OHOS_CONTROL_FLASHMODE, &controlFlashMode, 1);

    float zoomRatioRange[1] = {1.0};
    AddAbilityEntry(OHOS_ABILITY_ZOOM_RATIO_RANGE, zoomRatioRange,
        (sizeof(zoomRatioRange) / sizeof(zoomRatioRange[0])));

    const float zoomRatio = 1.0;
    AddAbilityEntry(OHOS_CONTROL_ZOOM_RATIO, &zoomRatio, 1);
}

void DMetadataProcessor::AddDefaultSensorAbility()
{
    int32_t activeArraySize[] = {
        0, 0, static_cast<int32_t>(maxPreviewResolution_.width_), static_cast<int32_t>(maxPreviewResolution_.height_)
    };
    AddAbilityEntry(OHOS_SENSOR_INFO_ACTIVE_ARRAY_SIZE, activeArraySize,
        (sizeof(activeArraySize) / sizeof(activeArraySize[0])));

    int32_t pixelArraySize[] = {
        static_cast<int32_t>(maxPreviewResolution_.width_), static_cast<int32_t>(maxPreviewResolution_.height_)
    };
    AddAbilityEntry(OHOS_SENSOR_INFO_PIXEL_ARRAY_SIZE, pixelArraySize,
        (sizeof(pixelArraySize) / sizeof(pixelArraySize[0])));

    const int32_t jpegThumbnailSizes[] = {0, 0, DEGREE_240, DEGREE_180};
    AddAbilityEntry(OHOS_JPEG_AVAILABLE_THUMBNAIL_SIZES, jpegThumbnailSizes,
        (sizeof(jpegThumbnailSizes) / sizeof(jpegThumbnailSizes[0])));
}

DCamRetCode DMetadataProcessor::InitDCameraDefaultAbilityKeys(const std::string &abilityInfo)
{
    ParseDefaultAbilityInfo(abilityInfo);
    AddDefaultPositionAbility();
    InitDcameraBaseAbility();
    AddDefaultControlAbility();
    AddDefaultSensorAbility();
    return SUCCESS;
}

DCamRetCode DMetadataProcessor::InitDCameraOutputAbilityKeys(const std::string &abilityInfo)
{
    std::map<int, std::vector<DCResolution>> supportedFormats = GetDCameraSupportedFormats(abilityInfo);

    std::vector<int32_t> streamConfigurations;
    std::map<int, std::vector<DCResolution>>::iterator iter;
    for (iter = supportedFormats.begin(); iter != supportedFormats.end(); ++iter) {
        std::vector<DCResolution> resolutionList = iter->second;
        for (auto resolution : resolutionList) {
            DHLOGI("DMetadataProcessor::supported formats: { format=%d, width=%d, height=%d }", iter->first,
                resolution.width_, resolution.height_);
            streamConfigurations.push_back(iter->first);
            streamConfigurations.push_back(resolution.width_);
            streamConfigurations.push_back(resolution.height_);
        }
    }
    UpdateAbilityEntry(OHOS_ABILITY_STREAM_AVAILABLE_BASIC_CONFIGURATIONS, streamConfigurations.data(),
        streamConfigurations.size());

    UpdateAbilityEntry(OHOS_SENSOR_INFO_MAX_FRAME_DURATION, &MAX_FRAME_DURATION, 1);

    const int32_t jpegMaxSize = maxPhotoResolution_.width_ * maxPhotoResolution_.height_;
    UpdateAbilityEntry(OHOS_JPEG_MAX_SIZE, &jpegMaxSize, 1);

    const uint8_t connectionType = OHOS_CAMERA_CONNECTION_TYPE_REMOTE;
    UpdateAbilityEntry(OHOS_ABILITY_CAMERA_CONNECTION_TYPE, &connectionType, 1);

    return SUCCESS;
}

DCamRetCode DMetadataProcessor::AddAbilityEntry(uint32_t tag, const void *data, size_t size)
{
    if (dCameraAbility_ == nullptr) {
        DHLOGE("Distributed camera abilily is null.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    camera_metadata_item_t item;
    int ret = OHOS::Camera::FindCameraMetadataItem(dCameraAbility_->get(), tag, &item);
    if (ret) {
        if (!dCameraAbility_->addEntry(tag, data, size)) {
            DHLOGE("Add tag %u failed.", tag);
            return FAILED;
        }
    }
    return SUCCESS;
}

DCamRetCode DMetadataProcessor::UpdateAbilityEntry(uint32_t tag, const void *data, size_t size)
{
    if (dCameraAbility_ == nullptr) {
        DHLOGE("Distributed camera abilily is null.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    camera_metadata_item_t item;
    int ret = OHOS::Camera::FindCameraMetadataItem(dCameraAbility_->get(), tag, &item);
    if (ret) {
        if (!dCameraAbility_->addEntry(tag, data, size)) {
            DHLOGE("Add tag %u failed.", tag);
            return FAILED;
        }
    } else {
        if (!dCameraAbility_->updateEntry(tag, data, size)) {
            DHLOGE("Update tag %u failed.", tag);
            return FAILED;
        }
    }
    return SUCCESS;
}

DCamRetCode DMetadataProcessor::GetDCameraAbility(std::shared_ptr<CameraAbility> &ability)
{
    ability = dCameraAbility_;
    return SUCCESS;
}

DCamRetCode DMetadataProcessor::SetMetadataResultMode(const ResultCallbackMode &mode)
{
    if (mode < ResultCallbackMode::PER_FRAME || mode > ResultCallbackMode::ON_CHANGED) {
        DHLOGE("Invalid result callback mode.");
        return DCamRetCode::INVALID_ARGUMENT;
    }
    metaResultMode_ = mode;
    return SUCCESS;
}

DCamRetCode DMetadataProcessor::GetEnabledMetadataResults(std::vector<MetaType> &results)
{
    auto iter = enabledResultSet_.begin();
    while (iter != enabledResultSet_.end()) {
        results.push_back(*iter);
        iter++;
    }
    return SUCCESS;
}

DCamRetCode DMetadataProcessor::EnableMetadataResult(const std::vector<MetaType> &results)
{
    if (results.size() == 0) {
        DHLOGE("Enable metadata result list is empty.");
        return SUCCESS;
    }

    for (size_t i = 0; i < results.size(); i++) {
        auto iter = allResultSet_.find(results[i]);
        if (iter != allResultSet_.end()) {
            auto anoIter = enabledResultSet_.find(results[i]);
            if (anoIter == enabledResultSet_.end()) {
                enabledResultSet_.insert(results[i]);
            }
        } else {
            DHLOGE("Cannot find match metatype.");
            return SUCCESS;
        }
    }
    return SUCCESS;
}

DCamRetCode DMetadataProcessor::DisableMetadataResult(const std::vector<MetaType> &results)
{
    if (results.size() == 0) {
        DHLOGE("Disable metadata result list is empty.");
        return SUCCESS;
    }

    for (size_t i = 0; i < results.size(); i++) {
        auto iter = allResultSet_.find(results[i]);
        if (iter != allResultSet_.end()) {
            auto anoIter = enabledResultSet_.find(results[i]);
            if (anoIter != enabledResultSet_.end()) {
                enabledResultSet_.erase(*iter);
            }
        } else {
            DHLOGE("Cannot find match metatype.");
            return SUCCESS;
        }
    }
    return SUCCESS;
}

DCamRetCode DMetadataProcessor::ResetEnableResults()
{
    if (enabledResultSet_.size() < allResultSet_.size()) {
        for (auto result : allResultSet_) {
            enabledResultSet_.insert(result);
        }
    }
    return SUCCESS;
}

void DMetadataProcessor::UpdateResultMetadata(const uint64_t &resultTimestamp)
{
    DHLOGD("DMetadataProcessor::UpdateResultMetadata result callback mode: %d", metaResultMode_);
    if (metaResultMode_ != ResultCallbackMode::PER_FRAME) {
        return;
    }

    std::lock_guard<std::mutex> autoLock(producerMutex_);
    if (latestProducerMetadataResult_ == nullptr) {
        DHLOGD("DMetadataProcessor::UpdateResultMetadata latest producer metadata result is null");
        return;
    }

    UpdateAllResult(resultTimestamp);
}

void DMetadataProcessor::SetResultCallback(
    std::function<void(uint64_t, std::shared_ptr<OHOS::Camera::CameraMetadata>)> &resultCbk)
{
    resultCallback_ = resultCbk;
}

void DMetadataProcessor::UpdateAllResult(const uint64_t &resultTimestamp)
{
    uint32_t itemCap = OHOS::Camera::GetCameraMetadataItemCapacity(latestProducerMetadataResult_->get());
    uint32_t dataSize = OHOS::Camera::GetCameraMetadataDataSize(latestProducerMetadataResult_->get());
    DHLOGD("DMetadataProcessor::UpdateAllResult itemCapacity: %u, dataSize: %u", itemCap, dataSize);
    std::shared_ptr<OHOS::Camera::CameraMetadata> result =
        std::make_shared<OHOS::Camera::CameraMetadata>(itemCap, dataSize);
    int32_t ret = OHOS::Camera::CopyCameraMetadataItems(result->get(), latestProducerMetadataResult_->get());
    if (ret != CAM_META_SUCCESS) {
        DHLOGE("DMetadataProcessor::UpdateAllResult copy metadata item failed, ret: %d", ret);
        return;
    }
    resultCallback_(resultTimestamp, result);
}

bool DMetadataProcessor::IsMetadataItemChanged(const camera_metadata_item_t &item,
    const camera_metadata_item_t &anoItem)
{
    uint32_t size;

    if ((item.count != anoItem.count) || (item.data_type != anoItem.data_type)) {
        return true;
    }
    size = GetDataSize(item.data_type);
    DHLOGD("DMetadataProcessor::UpdateOnChanged data size: %u", size);
    for (uint32_t i = 0; i < (size * static_cast<uint32_t>(item.count)); i++) {
        if (*(item.data.u8 + i) != *(anoItem.data.u8 + i)) {
            return true;
        }
    }
    return false;
}

void DMetadataProcessor::UpdateOnChanged(const uint64_t &resultTimestamp)
{
    bool needReturn = false;
    uint32_t itemCap = OHOS::Camera::GetCameraMetadataItemCapacity(latestProducerMetadataResult_->get());
    uint32_t dataSize = OHOS::Camera::GetCameraMetadataDataSize(latestProducerMetadataResult_->get());
    DHLOGD("DMetadataProcessor::UpdateOnChanged itemCapacity: %u, dataSize: %u", itemCap, dataSize);
    std::shared_ptr<OHOS::Camera::CameraMetadata> result =
        std::make_shared<OHOS::Camera::CameraMetadata>(itemCap, dataSize);
    DHLOGD("DMetadataProcessor::UpdateOnChanged enabledResultSet size: %d", enabledResultSet_.size());
    for (auto tag : enabledResultSet_) {
        DHLOGD("DMetadataProcessor::UpdateOnChanged cameta device metadata tag: %d", tag);
        camera_metadata_item_t item;
        camera_metadata_item_t anoItem;
        int ret1 = OHOS::Camera::FindCameraMetadataItem(latestProducerMetadataResult_->get(), tag, &item);
        int ret2 = OHOS::Camera::FindCameraMetadataItem(latestConsumerMetadataResult_->get(), tag, &anoItem);
        DHLOGD("DMetadataProcessor::UpdateOnChanged find metadata item ret: %d, %d", ret1, ret2);
        if (ret1 != CAM_META_SUCCESS) {
            continue;
        }

        if (ret2 != CAM_META_SUCCESS || IsMetadataItemChanged(item, anoItem)) {
            needReturn = true;
            result->addEntry(tag, GetMetadataItemData(item), item.count);
            continue;
        }
    }

    if (needReturn) {
        resultCallback_(resultTimestamp, result);
    }
}

DCamRetCode DMetadataProcessor::SaveResultMetadata(std::string resultStr)
{
    if (resultStr.empty()) {
        DHLOGE("Input result string is null.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    std::string metadataStr = Base64Decode(resultStr);
    std::lock_guard<std::mutex> autoLock(producerMutex_);
    latestConsumerMetadataResult_ = latestProducerMetadataResult_;
    latestProducerMetadataResult_ = OHOS::Camera::MetadataUtils::DecodeFromString(metadataStr);
    if (latestProducerMetadataResult_ == nullptr) {
        DHLOGE("Failed to decode metadata setting from string.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    if (!OHOS::Camera::GetCameraMetadataItemCount(latestProducerMetadataResult_->get())) {
        DHLOGE("Input result metadata item is empty.");
        return DCamRetCode::INVALID_ARGUMENT;
    }

    DHLOGD("DMetadataProcessor::SaveResultMetadata result callback mode: %d", metaResultMode_);
    if (metaResultMode_ != ResultCallbackMode::ON_CHANGED) {
        return SUCCESS;
    }

    uint64_t resultTimestamp = GetCurrentLocalTimeStamp();
    if (latestConsumerMetadataResult_ == nullptr) {
        UpdateAllResult(resultTimestamp);
        return SUCCESS;
    }

    camera_metadata_item_entry_t* itemEntry = OHOS::Camera::GetMetadataItems(latestProducerMetadataResult_->get());
    uint32_t count = latestProducerMetadataResult_->get()->item_count;
    for (uint32_t i = 0; i < count; i++, itemEntry++) {
        enabledResultSet_.insert((MetaType)(itemEntry->item));
    }
    UpdateOnChanged(resultTimestamp);
    return SUCCESS;
}

void DMetadataProcessor::ConvertToCameraMetadata(common_metadata_header_t *&input,
    std::shared_ptr<OHOS::Camera::CameraMetadata> &output)
{
    auto ret = OHOS::Camera::CopyCameraMetadataItems(output->get(), input);
    if (ret != CAM_META_SUCCESS) {
        DHLOGE("Failed to copy the old metadata to new metadata.");
        output = nullptr;
    }
}

void DMetadataProcessor::ResizeMetadataHeader(common_metadata_header_t *&header,
    uint32_t itemCapacity, uint32_t dataCapacity)
{
    if (header) {
        OHOS::Camera::FreeCameraMetadataBuffer(header);
    }
    header = OHOS::Camera::AllocateCameraMetadataBuffer(itemCapacity, dataCapacity);
}

uint32_t DMetadataProcessor::GetDataSize(uint32_t type)
{
    uint32_t size = 0;
    if (type == META_TYPE_BYTE) {
        size = sizeof(uint8_t);
    } else if (type == META_TYPE_INT32) {
        size = sizeof(int32_t);
    } else if (type == META_TYPE_UINT32) {
        size = sizeof(uint32_t);
    } else if (type == META_TYPE_FLOAT) {
        size = sizeof(float);
    } else if (type == META_TYPE_INT64) {
        size = sizeof(int64_t);
    } else if (type == META_TYPE_DOUBLE) {
        size = sizeof(double);
    } else if (type == META_TYPE_RATIONAL) {
        size = sizeof(camera_rational_t);
    } else {
        size = 0;
    }
    return size;
}

void* DMetadataProcessor::GetMetadataItemData(const camera_metadata_item_t &item)
{
    switch (item.data_type) {
        case META_TYPE_BYTE: {
            return item.data.u8;
        }
        case META_TYPE_INT32: {
            return item.data.i32;
        }
        case META_TYPE_UINT32: {
            return item.data.ui32;
        }
        case META_TYPE_FLOAT: {
            return item.data.f;
        }
        case META_TYPE_INT64: {
            return item.data.i64;
        }
        case META_TYPE_DOUBLE: {
            return item.data.d;
        }
        case META_TYPE_RATIONAL: {
            return item.data.r;
        }
        default: {
            DHLOGE("DMetadataProcessor::GetMetadataItemData invalid data type: %u", item.data_type);
            return nullptr;
        }
    }
}

void DMetadataProcessor::GetEachNodeSupportedResolution(std::vector<int>& formats, const std::string rootNode,
    std::map<int, std::vector<DCResolution>>& supportedFormats, Json::Value& rootValue)
{
    for (const auto &format : formats) {
        std::string formatStr = std::to_string(format);
        if (!rootValue[rootNode].isMember("Resolution") || !rootValue[rootNode]["Resolution"].isMember(formatStr) ||
            !rootValue[rootNode]["Resolution"][formatStr].isArray() ||
            rootValue[rootNode]["Resolution"][formatStr].size() == 0 ||
            rootValue[rootNode]["Resolution"][formatStr].size() > JSON_ARRAY_MAX_SIZE) {
            DHLOGE("Resolution or %s error.", formatStr.c_str());
            continue;
        }
        GetNodeSupportedResolution(format, formatStr, rootNode, supportedFormats, rootValue);
    }
}

void DMetadataProcessor::GetNodeSupportedResolution(int format, std::string formatStr, const std::string rootNode,
    std::map<int, std::vector<DCResolution>>& supportedFormats, Json::Value& rootValue)
{
    std::vector<DCResolution> resolutionVec;
    uint32_t size = rootValue[rootNode]["Resolution"][formatStr].size();
    for (uint32_t i = 0; i < size; i++) {
        if (!rootValue[rootNode]["Resolution"][formatStr][i].isString()) {
            DHLOGE("Resolution %s %d ,is not string.", formatStr.c_str(), i);
            continue;
        }
        std::string resoStr = rootValue[rootNode]["Resolution"][formatStr][i].asString();
        std::vector<std::string> reso;
        SplitString(resoStr, reso, STAR_SEPARATOR);
        if (reso.size() != SIZE_FMT_LEN) {
            continue;
        }
        uint32_t width = static_cast<uint32_t>(std::stoi(reso[0]));
        uint32_t height = static_cast<uint32_t>(std::stoi(reso[1]));
        if (height == 0 || width == 0 ||
            ((rootNode == "Photo") &&
                ((width * height) > (MAX_SUPPORT_PHOTO_WIDTH * MAX_SUPPORT_PHOTO_HEIGHT))) ||
            ((rootNode != "Photo") &&
                (width > MAX_SUPPORT_PREVIEW_WIDTH || height > MAX_SUPPORT_PREVIEW_HEIGHT))) {
            continue;
        }
        DCResolution resolution(width, height);
        resolutionVec.push_back(resolution);
    }
    if (!resolutionVec.empty()) {
        std::sort(resolutionVec.begin(), resolutionVec.end());
        supportedFormats[format] = resolutionVec;
        if ((rootNode != "Photo") && (maxPreviewResolution_ < resolutionVec[0])) {
            maxPreviewResolution_.width_ = resolutionVec[0].width_;
            maxPreviewResolution_.height_ = resolutionVec[0].height_;
        }
        if ((rootNode == "Photo") && (maxPhotoResolution_ < resolutionVec[0])) {
            maxPhotoResolution_.width_ = resolutionVec[0].width_;
            maxPhotoResolution_.height_ = resolutionVec[0].height_;
        }
    }
}

std::map<int, std::vector<DCResolution>> DMetadataProcessor::GetDCameraSupportedFormats(const std::string &abilityInfo)
{
    std::map<int, std::vector<DCResolution>> supportedFormats;
    JSONCPP_STRING errs;
    Json::CharReaderBuilder readerBuilder;
    Json::Value rootValue;

    std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
    if (!jsonReader->parse(abilityInfo.c_str(), abilityInfo.c_str() + abilityInfo.length(), &rootValue, &errs) ||
        !rootValue.isObject()) {
        return supportedFormats;
    }
    ParsePhotoFormats(rootValue, supportedFormats);
    ParsePreviewFormats(rootValue, supportedFormats);
    ParseVideoFormats(rootValue, supportedFormats);
    return supportedFormats;
}

void DMetadataProcessor::ParsePhotoFormats(Json::Value& rootValue,
    std::map<int, std::vector<DCResolution>>& supportedFormats)
{
    if (!rootValue.isMember("Photo") || !rootValue["Photo"].isMember("OutputFormat") ||
        !rootValue["Photo"]["OutputFormat"].isArray() || rootValue["Photo"]["OutputFormat"].size() == 0 ||
        rootValue["Photo"]["OutputFormat"].size() > JSON_ARRAY_MAX_SIZE) {
        DHLOGE("Photo or photo output format error.");
        return;
    }
    std::vector<int> photoFormats;
    uint32_t size = rootValue["Photo"]["OutputFormat"].size();
    for (uint32_t i = 0; i < size; i++) {
        if ((rootValue["Photo"]["OutputFormat"][i]).isInt()) {
            photoFormats.push_back((rootValue["Photo"]["OutputFormat"][i]).asInt());
        }
    }
    GetEachNodeSupportedResolution(photoFormats, "Photo", supportedFormats, rootValue);
}

void DMetadataProcessor::ParsePreviewFormats(Json::Value& rootValue,
    std::map<int, std::vector<DCResolution>>& supportedFormats)
{
    if (!rootValue.isMember("Preview") || !rootValue["Preview"].isMember("OutputFormat") ||
        !rootValue["Preview"]["OutputFormat"].isArray() || rootValue["Preview"]["OutputFormat"].size() == 0 ||
        rootValue["Preview"]["OutputFormat"].size() > JSON_ARRAY_MAX_SIZE) {
        DHLOGE("Preview or preview output format error.");
        return;
    }
    std::vector<int> previewFormats;
    uint32_t size = rootValue["Preview"]["OutputFormat"].size();
    for (uint32_t i = 0; i < size; i++) {
        if ((rootValue["Preview"]["OutputFormat"][i]).isInt()) {
            previewFormats.push_back((rootValue["Preview"]["OutputFormat"][i]).asInt());
        }
    }
    GetEachNodeSupportedResolution(previewFormats, "Preview", supportedFormats, rootValue);
}

void DMetadataProcessor::ParseVideoFormats(Json::Value& rootValue,
    std::map<int, std::vector<DCResolution>>& supportedFormats)
{
    if (!rootValue.isMember("Video") || !rootValue["Video"].isMember("OutputFormat") ||
        !rootValue["Video"]["OutputFormat"].isArray() || rootValue["Video"]["OutputFormat"].size() == 0 ||
        rootValue["Video"]["OutputFormat"].size() > JSON_ARRAY_MAX_SIZE) {
        DHLOGE("Video or video output format error.");
        return;
    }
    std::vector<int> videoFormats;
    uint32_t size = rootValue["Video"]["OutputFormat"].size();
    for (uint32_t i = 0; i < size; i++) {
        if ((rootValue["Video"]["OutputFormat"][i]).isInt()) {
            videoFormats.push_back((rootValue["Video"]["OutputFormat"][i]).asInt());
        }
    }
    GetEachNodeSupportedResolution(videoFormats, "Video", supportedFormats, rootValue);
}

void DMetadataProcessor::PrintDCameraMetadata(const common_metadata_header_t *metadata)
{
    if (metadata == nullptr) {
        DHLOGE("Failed to print metadata, input metadata is null.");
        return;
    }

    uint32_t tagCount = OHOS::Camera::GetCameraMetadataItemCount(metadata);
    DHLOGD("DMetadataProcessor::PrintDCameraMetadata, input metadata item count = %d.", tagCount);
    for (uint32_t i = 0; i < tagCount; i++) {
        camera_metadata_item_t item;
        int ret = OHOS::Camera::GetCameraMetadataItem(metadata, i, &item);
        if (ret != 0) {
            continue;
        }

        const char *name = OHOS::Camera::GetCameraMetadataItemName(item.item);
        if (item.data_type == META_TYPE_BYTE) {
            for (size_t k = 0; k < item.count; k++) {
                DHLOGI("tag index:%d, name:%s, value:%d", item.index, name, (uint8_t)(item.data.u8[k]));
            }
        } else if (item.data_type == META_TYPE_INT32) {
            for (size_t k = 0; k < item.count; k++) {
                DHLOGI("tag index:%d, name:%s, value:%d", item.index, name, (int32_t)(item.data.i32[k]));
            }
        } else if (item.data_type == META_TYPE_UINT32) {
            for (size_t k = 0; k < item.count; k++) {
                DHLOGI("tag index:%d, name:%s, value:%d", item.index, name, (uint32_t)(item.data.ui32[k]));
            }
        } else if (item.data_type == META_TYPE_FLOAT) {
            for (size_t k = 0; k < item.count; k++) {
                DHLOGI("tag index:%d, name:%s, value:%f", item.index, name, (float)(item.data.f[k]));
            }
        } else if (item.data_type == META_TYPE_INT64) {
            for (size_t k = 0; k < item.count; k++) {
                DHLOGI("tag index:%d, name:%s, value:%lld", item.index, name, (long long)(item.data.i64[k]));
            }
        } else if (item.data_type == META_TYPE_DOUBLE) {
            for (size_t k = 0; k < item.count; k++) {
                DHLOGI("tag index:%d, name:%s, value:%lf", item.index, name, (double)(item.data.d[k]));
            }
        } else {
            DHLOGI("tag index:%d, name:%s, value:%d", item.index, name, *(item.data.r));
        }
    }
}
} // namespace DistributedHardware
} // namespace OHOS
