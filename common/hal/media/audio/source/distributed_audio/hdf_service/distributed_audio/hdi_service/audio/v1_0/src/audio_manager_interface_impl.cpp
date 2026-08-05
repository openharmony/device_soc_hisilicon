/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include "audio_manager_interface_impl.h"

#include <cstring>
#include <hdf_base.h>
#include <sstream>
#include <algorithm>
#include "daudio_constants.h"
#include "daudio_errcode.h"
#include "daudio_events.h"
#include "daudio_log.h"
#include "daudio_utils.h"
#include "iaudio_device_callback.h"
#include "audio_device_manager.h"

#undef DH_LOG_TAG
#define DH_LOG_TAG "AudioManagerInterfaceImpl"


namespace OHOS {
namespace HDI {
namespace DistributedAudio {
namespace Audio {
namespace V1_0 {
using namespace OHOS::DistributedHardware;

AudioManagerInterfaceImpl *AudioManagerInterfaceImpl::GetAudioManager()
{
    static AudioManagerInterfaceImpl instance;
    return &instance;
}

AudioManagerInterfaceImpl::AudioManagerInterfaceImpl()
{
    DHLOGD("Distributed audio manager constructed.");
}

AudioManagerInterfaceImpl::~AudioManagerInterfaceImpl()
{
    DHLOGD("Distributed audio manager destructed.");
}

int32_t AudioManagerInterfaceImpl::GetAllAdapters(std::vector<AudioAdapterDescriptor> &descs)
{
    DHLOGI("Get all distributed audio adapters.");
    std::lock_guard<std::mutex> adpLck(adapterMapMtx_);

    std::transform(mapAudioAdapter_.begin(), mapAudioAdapter_.end(), std::back_inserter(descs),
        [](auto& adp) { return adp.second->GetAdapterDesc(); });

    DHLOGI("Get adapters success, total is (%zu). ", mapAudioAdapter_.size());
    return HDF_SUCCESS;
}

int32_t AudioManagerInterfaceImpl::LoadAdapter(const AudioAdapterDescriptor &desc,
    std::shared_ptr<IAudioAdapter> &adapter)
{
    DHLOGI("Load distributed audio adapter: %s.", GetAnonyString(desc.adapterName).c_str());
    std::lock_guard<std::mutex> adpLck(adapterMapMtx_);
    auto adp = mapAudioAdapter_.find(desc.adapterName);
    if (adp == mapAudioAdapter_.end()) {
        DHLOGE("Load audio adapter failed, can not find adapter.");
        adapter = nullptr;
        return HDF_FAILURE;
    }

    int32_t ret = adp->second->AdapterLoad();
    if (ret != DH_SUCCESS) {
        DHLOGE("Load audio adapter failed, adapter return: %d.", ret);
        adapter = nullptr;
        return HDF_FAILURE;
    }

    adapter = adp->second;
    DHLOGI("Load adapter success.");
    return HDF_SUCCESS;
}

int32_t AudioManagerInterfaceImpl::UnloadAdapter(const std::string &adapterName)
{
    DHLOGI("Unload distributed audio adapter: %s.", GetAnonyString(adapterName).c_str());
    std::lock_guard<std::mutex> adpLck(adapterMapMtx_);
    auto adp = mapAudioAdapter_.find(adapterName);
    if (adp == mapAudioAdapter_.end()) {
        DHLOGE("Unload audio adapter failed, can not find adapter.");
        return HDF_SUCCESS;
    }

    int32_t ret = adp->second->AdapterUnload();
    if (ret != DH_SUCCESS) {
        DHLOGE("Unload audio adapter failed, adapter return: %d.", ret);
        return ret;
    }
    DHLOGI("Unload adapter success.");
    return HDF_SUCCESS;
}

int32_t AudioManagerInterfaceImpl::ReleaseAudioManagerObject()
{
    DHLOGD("Release distributed audio manager object.");
    return HDF_SUCCESS;
}

int32_t AudioManagerInterfaceImpl::AddAudioDevice(const std::string &adpName, const uint32_t dhId,
    const std::string &caps, const std::shared_ptr<IDAudioCallback> &callback)
{
    DHLOGI("Add audio device name: %s, device: %d.", (adpName).c_str(), dhId);
    std::lock_guard<std::mutex> adpLck(adapterMapMtx_);
    auto adp = mapAudioAdapter_.find(adpName);
    if (adp == mapAudioAdapter_.end()) {
        int32_t ret = CreateAdapter(adpName, dhId, callback);
        if (ret != DH_SUCCESS) {
            DHLOGE("Create audio adapter failed.");
            return ERR_DH_AUDIO_HDF_FAIL;
        }
    }
    adp = mapAudioAdapter_.find(adpName);
    if (adp == mapAudioAdapter_.end() || adp->second == nullptr) {
        DHLOGE("Audio device has not been created  or is null ptr.");
        return ERR_DH_AUDIO_HDF_INVALID_OPERATION;
    }
    int32_t deviceType = GetDevTypeByDHId(dhId);
    AudioDeviceInfo info = {};
    switch (deviceType) {
        case AUDIO_DEVICE_TYPE_SPEAKER:
            adp->second->SetSpeakerCallback(callback);
            info.deviceType = AUDIO_DEVICE_SPEAKER_VIRTUAL;
            (void)strncpy(info.deviceName, "virtual_speaker", MAX_DEVICE_NAME_LEN - 1);
            info.deviceName[MAX_DEVICE_NAME_LEN - 1] = '\0';
            break;
        case AUDIO_DEVICE_TYPE_MIC:
            adp->second->SetMicCallback(callback);
            info.deviceType = AUDIO_DEVICE_MIC_VIRTUAL;
            (void)strncpy(info.deviceName, "virtual_mic", MAX_DEVICE_NAME_LEN - 1);
            info.deviceName[MAX_DEVICE_NAME_LEN - 1] = '\0';
            break;
        case AUDIO_DEVICE_TYPE_UNKNOWN:
        default:
            DHLOGE("DhId is illegal, devType is unknow.");
            return ERR_DH_AUDIO_HDF_FAIL;
    }
    int32_t ret = adp->second->AddAudioDevice(dhId, caps);
    if (ret != DH_SUCCESS) {
        DHLOGE("Add audio device failed, adapter return: %d.", ret);
        return ERR_DH_AUDIO_HDF_FAIL;
    }
    info.dhId = dhId;
    info.connectStatus = CONNECT;
    if (callback_ != nullptr) {
        callback_->OnDeviceChange(info);
        DHLOGI("Add audio device OnDeviceChange success.");
    }
    DHLOGI("Add audio device success.");
    return DH_SUCCESS;
}

int32_t AudioManagerInterfaceImpl::RemoveAudioDevice(const std::string &adpName, const uint32_t dhId)
{
    DHLOGI("Remove audio device name: %s, device: %d.", GetAnonyString(adpName).c_str(), dhId);
    std::lock_guard<std::mutex> adpLck(adapterMapMtx_);
    auto adp = mapAudioAdapter_.find(adpName);
    if (adp == mapAudioAdapter_.end() || adp->second == nullptr) {
        DHLOGE("Audio device has not been created  or is null ptr.");
        return ERR_DH_AUDIO_HDF_INVALID_OPERATION;
    }

    int32_t ret = adp->second->RemoveAudioDevice(dhId);
    if (ret != DH_SUCCESS) {
        DHLOGE("Remove audio device failed, adapter return: %d.", ret);
        return ret;
    }
    AudioDeviceInfo info = {};
    int32_t deviceType = GetDevTypeByDHId(dhId);
    if (deviceType == AUDIO_DEVICE_TYPE_SPEAKER) {
        info.deviceType = AUDIO_DEVICE_SPEAKER_VIRTUAL;
        (void)strncpy(info.deviceName, "virtual_speaker", MAX_DEVICE_NAME_LEN - 1);
        info.deviceName[MAX_DEVICE_NAME_LEN - 1] = '\0';
    } else if (deviceType == AUDIO_DEVICE_TYPE_MIC) {
        info.deviceType = AUDIO_DEVICE_MIC_VIRTUAL;
        (void)strncpy(info.deviceName, "virtual_mic", MAX_DEVICE_NAME_LEN - 1);
        info.deviceName[MAX_DEVICE_NAME_LEN - 1] = '\0';
    }
    info.dhId = dhId;
    info.connectStatus = DISCONNECT;
    if (callback_ != nullptr) {
        callback_->OnDeviceChange(info);
    }
    if (adp->second->IsPortsNoReg()) {
        mapAudioAdapter_.erase(adpName);
    }
    DHLOGI("Remove audio device success, mapAudioAdapter size() is : %d .", mapAudioAdapter_.size());
    return DH_SUCCESS;
}

int32_t AudioManagerInterfaceImpl::Notify(const std::string &adpName, const uint32_t devId, const DAudioEvent &event)
{
    DHLOGI("Notify event, adapter name: %s. event type: %d", GetAnonyString(adpName).c_str(),
        event.type);
    auto adp = mapAudioAdapter_.find(adpName);
    if (adp == mapAudioAdapter_.end()) {
        DHLOGE("Notify failed, can not find adapter.");
        return ERR_DH_AUDIO_HDF_INVALID_OPERATION;
    }

    int32_t ret = adp->second->Notify(devId, event);
    if (ret != DH_SUCCESS) {
        DHLOGE("Notify failed, adapter return: %d.", ret);
        return ERR_DH_AUDIO_HDF_FAIL;
    }
    return DH_SUCCESS;
}

int32_t AudioManagerInterfaceImpl::NotifyFwk(const DAudioDevEvent &event)
{
    DHLOGD("Notify audio fwk event(type:%d, adapter:%s, pin:%d, deviceType = %d).", event.eventType,
        (event.adapterName).c_str(), event.dhId, event.deviceType);
    std::stringstream ss;
    ss << "EVENT_TYPE=" << event.eventType << ";NID=" << event.adapterName << ";PIN=" << event.dhId << ";VID=" <<
        event.volGroupId << ";IID=" << event.iptGroupId;
    std::string eventInfo = ss.str();
    DHLOGI("Notify audio fwk success.");
    return DH_SUCCESS;
}

void AudioManagerInterfaceImpl::SetDeviceChangeCallback(
    const std::shared_ptr<IAudioManagerDeviceChangeCallback> &callback)
{
    callback_ = callback;
    DHLOGE("AudioManagerInterfaceImpl SetDeviceChangeCallback success!.");
}

int32_t AudioManagerInterfaceImpl::CreateAdapter(const std::string &adpName, const uint32_t devId,
    const std::shared_ptr<IDAudioCallback> &callback)
{
    if (callback == nullptr) {
        DHLOGE("Adapter callback is null.");
        return ERR_DH_AUDIO_HDF_NULLPTR;
    }
    AudioAdapterDescriptor desc = { adpName };
    std::shared_ptr<AudioAdapterInterfaceImpl> adapter = std::make_shared<AudioAdapterInterfaceImpl>(desc);
    if (adapter == nullptr) {
        DHLOGE("Create new audio adapter failed.");
        return ERR_DH_AUDIO_HDF_NULLPTR;
    }
    mapAudioAdapter_.insert(std::make_pair(adpName, adapter));
    return DH_SUCCESS;
}


} // V1_0
} // Audio
} // Distributedaudio
} // HDI
} // OHOSf
