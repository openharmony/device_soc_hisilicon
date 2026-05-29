/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include "audio_adapter_interface_impl.h"

#include <dlfcn.h>
#include <hdf_base.h>
#include <sstream>

#include "daudio_constants.h"
#include "daudio_errcode.h"
#include "daudio_events.h"
#include "daudio_log.h"
#include "daudio_utils.h"

#undef DH_LOG_TAG
#define DH_LOG_TAG "AudioAdapterInterfaceImpl"

using namespace OHOS::DistributedHardware;
namespace OHOS {
namespace HDI {
namespace DistributedAudio {
namespace Audio {
namespace V1_0 {
AudioAdapterInterfaceImpl::AudioAdapterInterfaceImpl(const AudioAdapterDescriptor &desc)
    : adpDescriptor_(desc)
{
    renderParam_ = { 0, 0, 0, 0, 0, 0 };
    captureParam_ = { 0, 0, 0, 0, 0, 0 };
    DHLOGD("Distributed audio adapter constructed, name(%s).", GetAnonyString(desc.adapterName).c_str());
}

AudioAdapterInterfaceImpl::~AudioAdapterInterfaceImpl()
{
    DHLOGD("Distributed audio adapter destructed, name(%s).", GetAnonyString(adpDescriptor_.adapterName).c_str());
}


void AudioAdapterInterfaceImpl::SetSpeakerCallback(const std::shared_ptr<IDAudioCallback> &spkCallback)
{
    if (spkCallback == nullptr) {
        DHLOGE("Callback is nullptr.");
        return;
    }
    extSpkCallback_ = spkCallback;
}

void AudioAdapterInterfaceImpl::SetMicCallback(const std::shared_ptr<IDAudioCallback> &micCallback)
{
    if (micCallback == nullptr) {
        DHLOGE("Callback is nullptr.");
        return;
    }
    extMicCallback_ = micCallback;
}

int32_t AudioAdapterInterfaceImpl::InitAllPorts()
{
    DHLOGI("Init (%zu) distributed audio ports.", mapAudioDevice_.size());
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::CreateRender(const AudioDeviceDescriptor &desc,
    const AudioSampleAttributes &attrs, std::shared_ptr<IAudioRender> &render)
{
    DHLOGI("Create distributed audio render, {pin: %zu, sampleRate: %zu, channel: %zu, formats: %zu, type: %d}.",
        desc.pins, attrs.sampleRate, attrs.channelCount, attrs.format, static_cast<int32_t>(attrs.type));
    render = nullptr;
    {
        std::lock_guard<std::mutex> devLck(devMapMtx_);
        if (mapAudioDevice_.find(desc.pins) == mapAudioDevice_.end()) {
            DHLOGE("Can not find device, create render failed.");
            return HDF_FAILURE;
        }
    }
#ifdef DAUDIO_SUPPORT_EXTENSION
    if (attrs.type == AUDIO_MMAP_NOIRQ) {
        DHLOGI("Try to mmap mode.");
        renderFlags_ = Audioext::V1_0::MMAP_MODE;
        audioRender_ = std::make_shared<AudioRenderExtImpl>();
        audioRender_->SetAttrs(adpDescriptor_.adapterName, desc, attrs, extSpkCallback_);
    } else {
        DHLOGI("Try to normal mode.");
        renderFlags_ = Audioext::V1_0::NORMAL_MODE;
        audioRender_ = std::make_shared<AudioRenderInterfaceImpl>(
            adpDescriptor_.adapterName, desc, attrs, extSpkCallback_);
    }
#else
    renderFlags_ = Audioext::V1_0::NORMAL_MODE;
    audioRender_ = std::make_shared<AudioRenderInterfaceImpl>(
        adpDescriptor_.adapterName, desc, attrs, extSpkCallback_);
#endif
    if (audioRender_ == nullptr) {
        DHLOGE("Create render failed.");
        return HDF_FAILURE;
    }

    int32_t ret = OpenRenderDevice(desc, attrs);
    if (ret != DH_SUCCESS) {
        DHLOGE("Open render device failed.");
        audioRender_ = nullptr;
        return ret == ERR_DH_AUDIO_HDF_WAIT_TIMEOUT ? HDF_ERR_TIMEOUT : HDF_FAILURE;
    }
    render = audioRender_;
    DHLOGI("Create render success.");
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::DestroyRender(const AudioDeviceDescriptor &desc)
{
    DHLOGI("Destroy distributed audio render, {pin: %zu}.", desc.pins);
    if (audioRender_ == nullptr) {
        DHLOGD("Render has not been created, do not need destroy.");
        return HDF_SUCCESS;
    }
    if (desc.pins != audioRender_->GetRenderDesc().pins) {
        DHLOGE("Render pin is wrong, destroy render failed.");
        return HDF_FAILURE;
    }

    int32_t ret = CloseRenderDevice(desc);
    if (ret != DH_SUCCESS) {
        DHLOGE("Close render device failed.");
        return HDF_FAILURE;
    }
    audioRender_ = nullptr;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::CreateCapture(const AudioDeviceDescriptor &desc,
    const AudioSampleAttributes &attrs, std::shared_ptr<IAudioCapture> &capture)
{
    DHLOGI("Create distributed audio capture, {pin: %zu, sampleRate: %zu, channel: %zu, formats: %zu}.",
        desc.pins, attrs.sampleRate, attrs.channelCount, attrs.format);
    capture = nullptr;
    {
        std::lock_guard<std::mutex> devLck(devMapMtx_);
        if (mapAudioDevice_.find(desc.pins) == mapAudioDevice_.end()) {
            DHLOGE("Can not find device, create capture failed.");
            return HDF_FAILURE;
        }
    }
#ifdef DAUDIO_SUPPORT_EXTENSION
    if (attrs.type == AUDIO_MMAP_NOIRQ) {
        DHLOGI("Try to mmap mode.");
        capturerFlags_ = Audioext::V1_0::MMAP_MODE;
        audioCapture_ = std::make_shared<AudioCaptureExtImpl>();
        audioCapture_->SetAttrs(adpDescriptor_.adapterName, desc, attrs, extMicCallback_);
    } else {
        DHLOGI("Try to normal mode.");
        capturerFlags_ = Audioext::V1_0::NORMAL_MODE;
        audioCapture_ = std::make_shared<AudioCaptureInterfaceImpl>(
            adpDescriptor_.adapterName, desc, attrs, extMicCallback_);
    }
#else
    capturerFlags_ = Audioext::V1_0::NORMAL_MODE;
    audioCapture_ = std::make_shared<AudioCaptureInterfaceImpl>(
        adpDescriptor_.adapterName, desc, attrs, extMicCallback_);
#endif
    if (audioCapture_ == nullptr) {
        DHLOGE("Create capture failed.");
        return HDF_FAILURE;
    }

    int32_t ret = OpenCaptureDevice(desc, attrs);
    if (ret != DH_SUCCESS) {
        DHLOGE("Open capture device failed.");
        audioCapture_ = nullptr;
        return ret == ERR_DH_AUDIO_HDF_WAIT_TIMEOUT ? HDF_ERR_TIMEOUT : HDF_FAILURE;
    }
    capture = audioCapture_;
    DHLOGI("Create capture success.");
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::DestroyCapture(const AudioDeviceDescriptor &desc)
{
    DHLOGI("Destroy distributed audio capture, {pin: %zu}.", desc.pins);
    if (audioCapture_ == nullptr) {
        DHLOGD("Capture has not been created, do not need destroy.");
        return HDF_SUCCESS;
    }
    if (desc.pins != audioCapture_->GetCaptureDesc().pins) {
        DHLOGE("Capture pin is wrong, destroy capture failed.");
        return HDF_FAILURE;
    }

    int32_t ret = CloseCaptureDevice(desc);
    if (ret != DH_SUCCESS) {
        DHLOGE("Close capture device failed.");
        return HDF_FAILURE;
    }
    audioCapture_ = nullptr;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::GetPortCapability(const AudioPort &port, AudioPortCapability &capability)
{
    DHLOGD("Get audio port capability.");
    (void)port;
    capability.sampleRateMasks = AUDIO_SAMPLE_RATE_DEFAULT;
    capability.channelCount = AUDIO_CHANNEL_COUNT_DEFAULT;

    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::SetPassthroughMode(const AudioPort &port, AudioPortPassthroughMode mode)
{
    (void)port;
    (void)mode;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::GetPassthroughMode(const AudioPort &port, AudioPortPassthroughMode &mode)
{
    (void)port;
    (void)mode;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::GetDeviceStatus(AudioDeviceStatus& status)
{
    (void) status;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::UpdateAudioRoute(const AudioRoute &route, int32_t &routeHandle)
{
    (void) route;
    (void) routeHandle;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::ReleaseAudioRoute(int32_t routeHandle)
{
    (void) routeHandle;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::SetMicMute(bool mute)
{
    (void) mute;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::GetMicMute(bool& mute)
{
    (void) mute;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::SetVoiceVolume(float volume)
{
    (void) volume;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::SetExtraParams(AudioExtParamKey key, const std::string &condition,
    const std::string &value)
{
    DHLOGD("Set audio parameters, key = %d, condition: %s value: %s.", key, condition.c_str(), value.c_str());
    int32_t ret = ERR_DH_AUDIO_HDF_FAIL;
    switch (key) {
        case AudioExtParamKey::AUDIO_EXT_PARAM_KEY_VOLUME:
            ret = SetAudioVolume(condition, value);
            if (ret != DH_SUCCESS) {
                DHLOGE("Set audio parameters failed.");
                return HDF_FAILURE;
            }
            break;
        case AudioExtParamKey::AUDIO_EXT_PARAM_KEY_NONE:
            DHLOGE("Parameter is unknown.");
            break;
        default:
            DHLOGE("Parameter is invalid.");
            return HDF_ERR_INVALID_PARAM;
    }
    DHLOGI("Set audio parameters success.");
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::GetExtraParams(AudioExtParamKey key, const std::string &condition,
    std::string &value)
{
    DHLOGD("Get audio parameters, key: %d, condition: %s.", key, condition.c_str());
    int32_t ret = ERR_DH_AUDIO_HDF_FAIL;
    switch (key) {
        case AudioExtParamKey::AUDIO_EXT_PARAM_KEY_VOLUME:
            ret = GetAudioVolume(condition, value);
            if (ret != DH_SUCCESS) {
                DHLOGE("Get audio parameters failed.");
                return HDF_FAILURE;
            }
            break;
        case AudioExtParamKey::AUDIO_EXT_PARAM_KEY_NONE:
            DHLOGE("Parameter is unknown.");
            break;
        default:
            DHLOGE("Parameter is invalid.");
            return HDF_ERR_INVALID_PARAM;
    }
    DHLOGI("Get audio parameters success.");
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::RegExtraParamObserver(const std::shared_ptr<IAudioCallback> &audioCallback,
    int8_t cookie)
{
    DHLOGD("Register audio param observer.");
    paramCallback_ = audioCallback;
    (void) cookie;
    return HDF_SUCCESS;
}

AudioAdapterDescriptor AudioAdapterInterfaceImpl::GetAdapterDesc()
{
    adpDescriptor_.ports.clear();
    std::lock_guard<std::mutex> devLck(devMapMtx_);
    for (auto pin = mapAudioDevice_.begin(); pin != mapAudioDevice_.end(); pin++) {
        AudioPort port = {PORT_OUT_IN, pin->first, ""};
        adpDescriptor_.ports.emplace_back(port);
    }
    return adpDescriptor_;
}

std::string AudioAdapterInterfaceImpl::GetDeviceCapabilitys(const uint32_t devId)
{
    std::lock_guard<std::mutex> devLck(devMapMtx_);
    auto dev = mapAudioDevice_.find(devId);
    if (dev == mapAudioDevice_.end()) {
        DHLOGE("Device not found.");
        return "";
    }
    return dev->second;
}

int32_t AudioAdapterInterfaceImpl::AdapterLoad()
{
    status_ = AudioAdapterStatus::STATUS_LOAD;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::AdapterUnload()
{
    if (audioRender_ != nullptr || audioCapture_ != nullptr) {
        DHLOGE("Adapter is busy, audio render or capture is not null.");
        return HDF_ERR_DEVICE_BUSY;
    }
    status_ = AudioAdapterStatus::STATUS_UNLOAD;
    return HDF_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::Notify(const uint32_t devId, const DAudioEvent &event)
{
    switch (static_cast<AudioExtParamEvent>(event.type)) {
        case HDF_AUDIO_EVENT_VOLUME_CHANGE:
            DHLOGI("Notify event: VOLUME_CHANGE, event content: %s.", event.content.c_str());
            return HandleVolumeChangeEvent(event);
        case HDF_AUDIO_EVENT_FOCUS_CHANGE:
            DHLOGI("Notify event: FOCUS_CHANGE, event content: %s.", event.content.c_str());
            return HandleFocusChangeEvent(event);
        case HDF_AUDIO_EVENT_RENDER_STATE_CHANGE:
            DHLOGI("Notify event: RENDER_STATE_CHANGE, event content: %s.", event.content.c_str());
            return HandleRenderStateChangeEvent(event);
        case HDF_AUDIO_EVENT_OPEN_SPK_RESULT:
        case HDF_AUDIO_EVENT_CLOSE_SPK_RESULT:
        case HDF_AUDIO_EVENT_OPEN_MIC_RESULT:
        case HDF_AUDIO_EVENT_CLOSE_MIC_RESULT:
            return HandleSANotifyEvent(event);
        case HDF_AUDIO_EVENT_SPK_CLOSED:
        case HDF_AUDIO_EVENT_MIC_CLOSED:
            return HandleDeviceClosed(event);
        default:
            DHLOGE("Audio event: %d is undefined.", event.type);
            return ERR_DH_AUDIO_HDF_INVALID_OPERATION;
    }
}

int32_t AudioAdapterInterfaceImpl::AddAudioDevice(const uint32_t devId, const std::string &caps)
{
    DHLOGI("Add distributed audio device %d.", devId);
    std::lock_guard<std::mutex> devLck(devMapMtx_);
    auto dev = mapAudioDevice_.find(devId);
    if (dev != mapAudioDevice_.end()) {
        DHLOGI("Device has been add, do not repeat add.");
        return DH_SUCCESS;
    }
    mapAudioDevice_.insert(std::make_pair(devId, caps));

    DHLOGI("Add audio device success.");
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::RemoveAudioDevice(const uint32_t devId)
{
    DHLOGI("Remove distributed audio device %d.", devId);
    {
        std::lock_guard<std::mutex> devLck(devMapMtx_);
        if (mapAudioDevice_.find(devId) == mapAudioDevice_.end()) {
            DHLOGE("Device has not been add, remove device failed.");
            return ERR_DH_AUDIO_HDF_INVALID_OPERATION;
        }
        mapAudioDevice_.erase(devId);
    }
    AudioDeviceDescriptor dec;
    if (devId == spkPinInUse_) {
        dec.pins = static_cast<AudioPortPin>(spkPinInUse_);
        DestroyRender(dec);
    }
    if (devId == micPinInUse_) {
        dec.pins = static_cast<AudioPortPin>(micPinInUse_);
        DestroyCapture(dec);
    }

    DHLOGI("Remove audio device success.");
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::OpenRenderDevice(const AudioDeviceDescriptor &desc,
    const AudioSampleAttributes &attrs)
{
    DHLOGI("Open render device, pin: %d.", desc.pins);
    if (isSpkOpened_) {
        DHLOGI("Render already opened.");
        return DH_SUCCESS;
    }
    std::lock_guard<std::mutex> devLck(renderOptMtx_);
    spkPinInUse_ = desc.pins;
    renderParam_.format = attrs.format;
    renderParam_.channelCount = attrs.channelCount;
    renderParam_.sampleRate = attrs.sampleRate;
    renderParam_.streamUsage = attrs.type;
    renderParam_.frameSize = CalculateFrameSize(attrs.sampleRate, attrs.channelCount, attrs.format,
        timeInterval_, renderFlags_ == Audioext::V1_0::MMAP_MODE);
    renderParam_.renderFlags = renderFlags_;

    int32_t ret = extSpkCallback_->SetParameters(adpDescriptor_.adapterName, desc.pins, renderParam_);
    if (ret != HDF_SUCCESS) {
        DHLOGE("Set render parameters failed.");
        return ERR_DH_AUDIO_HDF_SET_PARAM_FAIL;
    }
    ret = extSpkCallback_->OpenDevice(adpDescriptor_.adapterName, desc.pins);
    if (ret != HDF_SUCCESS) {
        DHLOGE("Open render device failed.");
        return ERR_DH_AUDIO_HDF_OPEN_DEVICE_FAIL;
    }

    ret = WaitForSANotify(EVENT_OPEN_SPK);
    if (ret != DH_SUCCESS) {
        DHLOGE("Wait SA notify failed.");
        return ret;
    }
    DHLOGI("Open render device success.");
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::CloseRenderDevice(const AudioDeviceDescriptor &desc)
{
    DHLOGI("Close render device, pin: %d.", desc.pins);
    std::lock_guard<std::mutex> devLck(renderOptMtx_);
    if (spkPinInUse_ == 0) {
        DHLOGI("No need close render device.");
        return DH_SUCCESS;
    }
    renderParam_ = {};
    int32_t ret = extSpkCallback_->CloseDevice(adpDescriptor_.adapterName, desc.pins);
    if (ret != HDF_SUCCESS) {
        DHLOGE("Close audio device failed.");
        return ERR_DH_AUDIO_HDF_CLOSE_DEVICE_FAIL;
    }

    ret = WaitForSANotify(EVENT_CLOSE_SPK);
    if (ret != DH_SUCCESS) {
        DHLOGE("Wait SA notify failed.");
        return ret;
    }
    spkPinInUse_ = 0;
    DHLOGI("Close render device success.");
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::OpenCaptureDevice(const AudioDeviceDescriptor &desc,
    const AudioSampleAttributes &attrs)
{
    DHLOGI("Open capture device, pin: %d.", desc.pins);
    if (isMicOpened_) {
        DHLOGI("Capture already opened.");
        return DH_SUCCESS;
    }
    std::lock_guard<std::mutex> devLck(captureOptMtx_);
    micPinInUse_ = desc.pins;
    captureParam_.format = attrs.format;
    captureParam_.channelCount = attrs.channelCount;
    captureParam_.sampleRate = attrs.sampleRate;
    captureParam_.streamUsage = attrs.type;
    captureParam_.frameSize = CalculateFrameSize(attrs.sampleRate, attrs.channelCount,
        attrs.format, timeInterval_, capturerFlags_ == Audioext::V1_0::MMAP_MODE);
    captureParam_.capturerFlags = capturerFlags_;

    int32_t ret = extMicCallback_->SetParameters(adpDescriptor_.adapterName, desc.pins, captureParam_);
    if (ret != HDF_SUCCESS) {
        DHLOGE("Set audio parameters failed.");
        return ERR_DH_AUDIO_HDF_SET_PARAM_FAIL;
    }
    ret = extMicCallback_->OpenDevice(adpDescriptor_.adapterName, desc.pins);
    if (ret != HDF_SUCCESS) {
        DHLOGE("Open audio device failed.");
        return ERR_DH_AUDIO_HDF_OPEN_DEVICE_FAIL;
    }

    ret = WaitForSANotify(EVENT_OPEN_MIC);
    if (ret != DH_SUCCESS) {
        DHLOGE("Wait SA notify failed.");
        return ret;
    }
    DHLOGI("Open capture device success.");
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::CloseCaptureDevice(const AudioDeviceDescriptor &desc)
{
    DHLOGI("Close capture device, pin: %d.", desc.pins);
    std::lock_guard<std::mutex> devLck(captureOptMtx_);
    if (micPinInUse_ == 0) {
        DHLOGI("No need close capture device.");
        return DH_SUCCESS;
    }
    captureParam_ = {};
    int32_t ret = extMicCallback_->CloseDevice(adpDescriptor_.adapterName, desc.pins);
    if (ret != HDF_SUCCESS) {
        DHLOGE("Close audio device failed.");
        return ERR_DH_AUDIO_HDF_CLOSE_DEVICE_FAIL;
    }

    ret = WaitForSANotify(EVENT_CLOSE_MIC);
    if (ret != DH_SUCCESS) {
        DHLOGE("Wait SA notify failed.");
        return ret;
    }
    micPinInUse_ = 0;
    DHLOGI("Close capture device success.");
    return DH_SUCCESS;
}

uint32_t AudioAdapterInterfaceImpl::GetVolumeGroup(const uint32_t devId)
{
    uint32_t volGroup = VOLUME_GROUP_ID_DEFAULT;
    std::lock_guard<std::mutex> devLck(devMapMtx_);
    auto caps = mapAudioDevice_.find(devId);
    if (caps == mapAudioDevice_.end()) {
        DHLOGE("Can not find caps of dev:%u.", devId);
        return volGroup;
    }

    int32_t ret = GetAudioParamUInt(caps->second, VOLUME_GROUP_ID, volGroup);
    if (ret != DH_SUCCESS) {
        DHLOGE("Get volume group param failed, use default value, ret = %d.", ret);
    }
    return volGroup;
}

uint32_t AudioAdapterInterfaceImpl::GetInterruptGroup(const uint32_t devId)
{
    uint32_t iptGroup = INTERRUPT_GROUP_ID_DEFAULT;
    std::lock_guard<std::mutex> devLck(devMapMtx_);
    auto caps = mapAudioDevice_.find(devId);
    if (caps == mapAudioDevice_.end()) {
        DHLOGE("Can not find caps of dev:%u.", devId);
        return iptGroup;
    }

    int32_t ret = GetAudioParamUInt(caps->second, INTERRUPT_GROUP_ID, iptGroup);
    if (ret != DH_SUCCESS) {
        DHLOGE("Get interrupt group param failed, use default value, ret = %d.", ret);
    }
    return iptGroup;
}

int32_t AudioAdapterInterfaceImpl::SetAudioVolume(const std::string& condition, const std::string &param)
{
    if (extSpkCallback_ == nullptr) {
        DHLOGE("Callback is nullptr.");
        return ERR_DH_AUDIO_HDF_NULLPTR;
    }
    if (audioRender_ == nullptr) {
        DHLOGE("Render has not been created.");
        return ERR_DH_AUDIO_HDF_NULLPTR;
    }
    std::string content = condition;
    int32_t type = getEventTypeFromCondition(content);
    EXT_PARAM_EVENT eventType;

    if (type == VolumeEventType::EVENT_IS_STREAM_MUTE) {
        if (param == IS_MUTE_STATUS) {
            streamMuteStatus_ = 1;
        } else if (param == NOT_MUTE_STATUS) {
            streamMuteStatus_ = 0;
        } else {
            DHLOGE("Mute param is error.");
            return ERR_DH_AUDIO_HDF_FAIL;
        }
        eventType = HDF_AUDIO_EVNET_MUTE_SET;
        SetAudioParamStr(content, STREAM_MUTE_STATUS, param);
    } else {
        eventType = HDF_AUDIO_EVENT_VOLUME_SET;
        streamMuteStatus_ = 0;
        SetAudioParamStr(content, VOLUME_LEVEL, param);
    }
    DAudioEvent event = { eventType, content };
    int32_t ret =
        extSpkCallback_->NotifyEvent(adpDescriptor_.adapterName, audioRender_->GetRenderDesc().pins, event);
    if (ret != HDF_SUCCESS) {
        DHLOGE("NotifyEvent failed.");
        return ERR_DH_AUDIO_HDF_FAIL;
    }
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::GetAudioVolume(const std::string& condition, std::string &param)
{
    if (audioRender_ == nullptr) {
        DHLOGE("Render has not been created.");
        return ERR_DH_AUDIO_HDF_NULLPTR;
    }
    int32_t type = getEventTypeFromCondition(condition);
    uint32_t vol;
    switch (type) {
        case VolumeEventType::EVENT_GET_VOLUME:
            vol = audioRender_->GetVolumeInner();
            break;
        case VolumeEventType::EVENT_GET_MAX_VOLUME:
            vol = audioRender_->GetMaxVolumeInner();
            break;
        case VolumeEventType::EVENT_GET_MIN_VOLUME:
            vol = audioRender_->GetMinVolumeInner();
            break;
        case VolumeEventType::EVENT_IS_STREAM_MUTE:
            vol = streamMuteStatus_;
            break;
        default:
            vol = 0;
            DHLOGE("Get volume failed.");
    }
    param = std::to_string(vol);
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::getEventTypeFromCondition(const std::string &condition)
{
    std::string::size_type position = condition.find_first_of(";");
    int32_t type = std::stoi(condition.substr(TYPE_CONDITION, position - TYPE_CONDITION));
    return static_cast<VolumeEventType>(type);
}

int32_t AudioAdapterInterfaceImpl::HandleVolumeChangeEvent(const DAudioEvent &event)
{
    DHLOGI("Vol change (%s).", event.content.c_str());
    if (audioRender_ == nullptr) {
        DHLOGE("Render has not been created.");
        return ERR_DH_AUDIO_HDF_NULLPTR;
    }
    int32_t vol = AUDIO_DEFAULT_MIN_VOLUME_LEVEL;
    int32_t ret = GetAudioParamInt(event.content, VOLUME_LEVEL, vol);
    if (ret != DH_SUCCESS) {
        DHLOGE("Get volume value failed.");
        return ERR_DH_AUDIO_HDF_FAIL;
    }

    if (event.content.rfind("FIRST_VOLUME_CHANAGE", 0) == 0) {
        int32_t maxVol = AUDIO_DEFAULT_MAX_VOLUME_LEVEL;
        ret = GetAudioParamInt(event.content, MAX_VOLUME_LEVEL, maxVol);
        if (ret != DH_SUCCESS) {
            DHLOGE("Get max volume value failed, use defult max volume.");
        }
        int32_t minVol = AUDIO_DEFAULT_MIN_VOLUME_LEVEL;
        ret = GetAudioParamInt(event.content, MIN_VOLUME_LEVEL, minVol);
        if (ret != DH_SUCCESS) {
            DHLOGE("Get min volume value failed, use defult min volume.");
        }
        audioRender_->SetVolumeInner(vol);
        audioRender_->SetVolumeRangeInner(maxVol, minVol);
        return DH_SUCCESS;
    }

    audioRender_->SetVolumeInner(vol);
    if (paramCallback_ == nullptr) {
        DHLOGE("Audio param observer is null.");
        return ERR_DH_AUDIO_HDF_NULLPTR;
    }
    int8_t reserved;
    int8_t cookie;
    ret = paramCallback_->ParamCallback(AUDIO_EXT_PARAM_KEY_VOLUME, event.content, std::to_string(vol),
        reserved, cookie);
    if (ret != DH_SUCCESS) {
        DHLOGE("Notify vol failed.");
        return ERR_DH_AUDIO_HDF_FAIL;
    }
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::HandleFocusChangeEvent(const DAudioEvent &event)
{
    DHLOGI("Focus change (%s).", event.content.c_str());
    if (paramCallback_ == nullptr) {
        DHLOGE("Audio param observer is null.");
        return ERR_DH_AUDIO_HDF_NULLPTR;
    }
    int8_t reserved;
    int8_t cookie;
    int32_t ret = paramCallback_->ParamCallback(AUDIO_EXT_PARAM_KEY_FOCUS, event.content, "", reserved, cookie);
    if (ret != DH_SUCCESS) {
        DHLOGE("Notify Focus failed.");
        return ERR_DH_AUDIO_HDF_FAIL;
    }
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::HandleRenderStateChangeEvent(const DAudioEvent &event)
{
    DHLOGI("Render state change (%s).", event.content.c_str());
    if (paramCallback_ == nullptr) {
        DHLOGE("Audio param observer is null.");
        return ERR_DH_AUDIO_HDF_NULLPTR;
    }
    int8_t reserved;
    int8_t cookie;
    int32_t ret = paramCallback_->ParamCallback(AUDIO_EXT_PARAM_KEY_STATUS, event.content, "", reserved, cookie);
    if (ret != DH_SUCCESS) {
        DHLOGE("Notify render state failed.");
        return ERR_DH_AUDIO_HDF_FAIL;
    }
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::HandleSANotifyEvent(const DAudioEvent &event)
{
    DHLOGD("Notify event type %d, event content: %s.", event.type, event.content.c_str());
    switch (event.type) {
        case HDF_AUDIO_EVENT_OPEN_SPK_RESULT:
            if (event.content == HDF_EVENT_RESULT_SUCCESS) {
                isSpkOpened_ = true;
            }
            spkNotifyFlag_ = true;
            spkWaitCond_.notify_all();
            break;
        case HDF_AUDIO_EVENT_CLOSE_SPK_RESULT:
            if (event.content == HDF_EVENT_RESULT_SUCCESS) {
                isSpkOpened_ = false;
            }
            spkNotifyFlag_ = true;
            spkWaitCond_.notify_all();
            break;
        case HDF_AUDIO_EVENT_OPEN_MIC_RESULT:
            if (event.content == HDF_EVENT_RESULT_SUCCESS) {
                isMicOpened_ = true;
            }
            micNotifyFlag_ = true;
            micWaitCond_.notify_all();
            break;
        case HDF_AUDIO_EVENT_CLOSE_MIC_RESULT:
            if (event.content == HDF_EVENT_RESULT_SUCCESS) {
                isMicOpened_ = false;
            }
            micNotifyFlag_ = true;
            micWaitCond_.notify_all();
            break;
        default:
            DHLOGE("Notify not support event type %d, event content: %s.", event.type, event.content.c_str());
            return ERR_DH_AUDIO_HDF_FAIL;
    }
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::WaitForSANotify(const AudioDeviceEvent &event)
{
    if (event == EVENT_OPEN_SPK || event == EVENT_CLOSE_SPK) {
        spkNotifyFlag_ = false;
        std::unique_lock<std::mutex> lck(spkWaitMutex_);
        auto status = spkWaitCond_.wait_for(lck, std::chrono::seconds(WAIT_SECONDS), [this, event]() {
            return spkNotifyFlag_ ||
                (event == EVENT_OPEN_SPK && isSpkOpened_) || (event == EVENT_CLOSE_SPK && !isSpkOpened_);
        });
        if (!status) {
            DHLOGE("Wait spk event: %d timeout(%d)s.", event, WAIT_SECONDS);
            return ERR_DH_AUDIO_HDF_WAIT_TIMEOUT;
        }
        if (event == EVENT_OPEN_SPK && !isSpkOpened_) {
            DHLOGE("Wait open render device failed.");
            return ERR_DH_AUDIO_HDF_OPEN_DEVICE_FAIL;
        } else if (event == EVENT_CLOSE_SPK && isSpkOpened_) {
            DHLOGE("Wait close render device failed.");
            return ERR_DH_AUDIO_HDF_CLOSE_DEVICE_FAIL;
        }
        return DH_SUCCESS;
    }

    if (event == EVENT_OPEN_MIC || event == EVENT_CLOSE_MIC) {
        micNotifyFlag_ = false;
        std::unique_lock<std::mutex> lck(micWaitMutex_);
        auto status = micWaitCond_.wait_for(lck, std::chrono::seconds(WAIT_SECONDS), [this, event]() {
            return micNotifyFlag_ ||
                (event == EVENT_OPEN_MIC && isMicOpened_) || (event == EVENT_CLOSE_MIC && !isMicOpened_);
        });
        if (!status) {
            DHLOGE("Wait mic event: %d timeout(%d)s.", event, WAIT_SECONDS);
            return ERR_DH_AUDIO_HDF_WAIT_TIMEOUT;
        }
        if (event == EVENT_OPEN_MIC && !isMicOpened_) {
            DHLOGE("Wait open capture device failed.");
            return ERR_DH_AUDIO_HDF_OPEN_DEVICE_FAIL;
        } else if (event == EVENT_CLOSE_MIC && isMicOpened_) {
            DHLOGE("Wait close capture device failed.");
            return ERR_DH_AUDIO_HDF_CLOSE_DEVICE_FAIL;
        }
        return DH_SUCCESS;
    }
    return DH_SUCCESS;
}

int32_t AudioAdapterInterfaceImpl::HandleDeviceClosed(const DAudioEvent &event)
{
    DHLOGI("Handle device closed, event type: %d.", event.type);
    if (paramCallback_ != nullptr) {
        std::stringstream ss;
        ss << "ERR_EVENT;DEVICE_TYPE=" <<
            (event.type == HDF_AUDIO_EVENT_SPK_CLOSED ? AUDIO_DEVICE_TYPE_SPEAKER : AUDIO_DEVICE_TYPE_MIC) << ";";
        int8_t reserved;
        int8_t cookie;
        int32_t ret = paramCallback_->ParamCallback(AUDIO_EXT_PARAM_KEY_STATUS, ss.str(),
            std::to_string(EVENT_DEV_CLOSED), reserved, cookie);
        if (ret != DH_SUCCESS) {
            DHLOGE("Notify fwk failed.");
        }
    }

    AudioDeviceDescriptor dec;
    if (isSpkOpened_ && event.type == HDF_AUDIO_EVENT_SPK_CLOSED) {
        DHLOGE("Render device status error, close render.");
        dec.pins = static_cast<AudioPortPin>(spkPinInUse_);
        return DestroyRender(dec);
    } else if (isMicOpened_ && event.type == HDF_AUDIO_EVENT_MIC_CLOSED) {
        DHLOGE("Capture device status error, close capture.");
        dec.pins = static_cast<AudioPortPin>(micPinInUse_);
        return DestroyCapture(dec);
    }
    DHLOGI("Handle device closed success.");
    return DH_SUCCESS;
}

bool AudioAdapterInterfaceImpl::IsPortsNoReg()
{
    std::lock_guard<std::mutex> devLck(devMapMtx_);
    return mapAudioDevice_.empty();
}
} // V1_0
} // Audio
} // Distributedaudio
} // HDI
} // OHOS