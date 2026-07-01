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

#ifndef OHOS_AUDIO_RENDER_INTERFACE_IMPL_H
#define OHOS_AUDIO_RENDER_INTERFACE_IMPL_H

#include <mutex>
#include <string>
#include <cmath>

#include "audio_render_interface_impl_base.h"

#include "audio_types.h"
#include "iaudio_render.h"
#include "id_audio_manager.h"
#include "id_audio_callback.h"

namespace OHOS {
namespace HDI {
namespace DistributedAudio {
namespace Audio {
namespace V1_0 {
using OHOS::HDI::DistributedAudio::Audioext::V1_0::AudioData;
using OHOS::HDI::DistributedAudio::Audioext::V1_0::AudioParameter;
using OHOS::HDI::DistributedAudio::Audioext::V1_0::DAudioEvent;
using OHOS::HDI::DistributedAudio::Audioext::V1_0::IDAudioCallback;

constexpr uint32_t DURATION_FRAMES = 100;
constexpr uint32_t CUR_FRAME_INIT_VALUE = 0;

class AudioRenderInterfaceImpl : public AudioRenderInterfaceImplBase {
public:
    AudioRenderInterfaceImpl(const std::string &adpName, const AudioDeviceDescriptor &desc,
        const AudioSampleAttributes &attrs, const std::shared_ptr<IDAudioCallback> &callback);
    ~AudioRenderInterfaceImpl() override;

    int32_t GetLatency(uint32_t &ms) override;
    int32_t RenderFrame(const std::vector<int8_t> &frame, uint64_t &replyBytes) override;
    int32_t GetRenderPosition(uint64_t &frames, AudioTimeStamp &time) override;
    int32_t SetRenderSpeed(float speed) override;
    int32_t GetRenderSpeed(float &speed) override;
    int32_t SetChannelMode(AudioChannelMode mode) override;
    int32_t GetChannelMode(AudioChannelMode &mode) override;
    int32_t RegCallback(const std::shared_ptr<IAudioCallback> &audioCallback, int8_t cookie) override;
    int32_t DrainBuffer(AudioDrainNotifyType &type) override;
    int32_t IsSupportsDrain(bool &support) override;
    int32_t CheckSceneCapability(const AudioSceneDescriptor &scene, bool &supported) override;
    int32_t SelectScene(const AudioSceneDescriptor &scene) override;
    int32_t SetMute(bool mute) override;
    int32_t GetMute(bool &mute) override;
    int32_t SetVolume(float volume) override;
    int32_t GetVolume(float &volume) override;
    int32_t GetGainThreshold(float &min, float &max) override;
    int32_t SetGain(float gain) override;
    int32_t GetGain(float &gain) override;
    int32_t GetFrameSize(uint64_t &size) override;
    int32_t GetFrameCount(uint64_t &count) override;
    int32_t SetSampleAttributes(const AudioSampleAttributes &attrs) override;
    int32_t GetSampleAttributes(AudioSampleAttributes &attrs) override;
    int32_t GetCurrentChannelId(uint32_t &channelId) override;
    int32_t SetExtraParams(const std::string &keyValueList) override;
    int32_t GetExtraParams(std::string &keyValueList) override;
    int32_t ReqMmapBuffer(int32_t reqSize, AudioMmapBufferDescriptor &desc) override;
    int32_t GetMmapPosition(uint64_t &frames, AudioTimeStamp &time) override;
    int32_t AddAudioEffect(uint64_t effectid) override;
    int32_t RemoveAudioEffect(uint64_t effectid) override;
    int32_t GetFrameBufferSize(uint64_t &bufferSize) override;
    int32_t Start() override;
    int32_t Stop() override;
    int32_t Pause() override;
    int32_t Resume() override;
    int32_t Flush() override;
    int32_t TurnStandbyMode() override;
    int32_t AudioDevDump(int32_t range, int32_t fd) override;
    int32_t IsSupportsPauseAndResume(bool &supportPause, bool &supportResume) override;

    const AudioDeviceDescriptor &GetRenderDesc() override;
    void SetVolumeInner(const uint32_t vol) override;
    void SetVolumeRangeInner(const uint32_t volMax, const uint32_t volMin) override;
    uint32_t GetVolumeInner() override;
    uint32_t GetMaxVolumeInner() override;
    uint32_t GetMinVolumeInner() override;
    void SetAttrs(const std::string &adpName, const AudioDeviceDescriptor &desc,
        const AudioSampleAttributes &attrs, const std::shared_ptr<IDAudioCallback> &callback) override;

private:
    float GetFadeRate(uint32_t currentIndex, const uint32_t durationIndex);
    int32_t FadeInProcess(const uint32_t durationFrame, int8_t* frameData, const size_t frameLength);

private:
    static constexpr int64_t AUDIO_OFFSET_FRAME_NUM = 10;

    std::string adapterName_;
    AudioDeviceDescriptor devDesc_;
    AudioSampleAttributes devAttrs_;

    std::mutex renderMtx_;
    std::mutex volMtx_;
    bool firstOpenFlag_ = true;
    uint32_t timeInterval_ = 5;
    uint32_t currentFrame_ = 0;
    uint32_t vol_ = 0;
    uint32_t volMax_ = 0;
    uint32_t volMin_ = 0;
    int64_t frameIndex_ = 0;
    float renderSpeed_ = 0;
    AudioChannelMode channelMode_ = AUDIO_CHANNEL_NORMAL;
    AudioRenderStatus renderStatus_ = RENDER_STATUS_CLOSE;
    std::shared_ptr<IDAudioCallback> audioExtCallback_ = nullptr;
    std::shared_ptr<IAudioCallback> renderCallback_ = nullptr;
};
} // V1_0
} // Audio
} // Distributedaudio
} // HDI
} // OHOS
#endif // OHOS_AUDIO_RENDER_INTERFACE_IMPL_H