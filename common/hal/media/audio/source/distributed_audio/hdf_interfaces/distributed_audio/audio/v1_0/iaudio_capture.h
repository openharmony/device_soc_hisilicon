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

#ifndef OHOS_HDI_IAUDIO_CAPTURE_H
#define OHOS_HDI_IAUDIO_CAPTURE_H

#include <vector>
#include "audio_types.h"

namespace OHOS {
    namespace HDI {
        namespace DistributedAudio {
            namespace Audio {
                namespace V1_0 {
                    class IAudioCapture {
                        public:
                        virtual ~IAudioCapture() {}
                        virtual int32_t CaptureFrame(std::vector<int8_t> &frame, uint64_t requestBytes) = 0;
                        virtual int32_t GetCapturePosition(uint64_t &frames, AudioTimeStamp &time) = 0;
                        virtual int32_t CheckSceneCapability(const AudioSceneDescriptor &scene, bool &supported) = 0;
                        virtual int32_t SelectScene(const AudioSceneDescriptor &scene) = 0;
                        virtual int32_t SetMute(bool mute) = 0;
                        virtual int32_t GetMute(bool &mute) = 0;
                        virtual int32_t SetVolume(float volume) = 0;
                        virtual int32_t GetVolume(float &volume) = 0;
                        virtual int32_t GetGainThreshold(float &min, float &max) = 0;
                        virtual int32_t GetGain(float &gain) = 0;
                        virtual int32_t SetGain(float gain) = 0;
                        virtual int32_t GetFrameSize(uint64_t &size) = 0;
                        virtual int32_t GetFrameCount(uint64_t &count) = 0;
                        virtual int32_t SetSampleAttributes(const AudioSampleAttributes &attrs) = 0;
                        virtual int32_t GetSampleAttributes(AudioSampleAttributes &attrs) = 0;
                        virtual int32_t GetCurrentChannelId(uint32_t &channelId) = 0;
                        virtual int32_t SetExtraParams(const std::string &keyValueList) = 0;
                        virtual int32_t GetExtraParams(std::string &keyValueList) = 0;
                        virtual int32_t ReqMmapBuffer(int32_t reqSize, AudioMmapBufferDescriptor &desc) = 0;
                        virtual int32_t GetMmapPosition(uint64_t &frames, AudioTimeStamp &time) = 0;
                        virtual int32_t AddAudioEffect(uint64_t effectid) = 0;
                        virtual int32_t RemoveAudioEffect(uint64_t effectid) = 0;
                        virtual int32_t GetFrameBufferSize(uint64_t &bufferSize) = 0;
                        virtual int32_t Start() = 0;
                        virtual int32_t Stop() = 0;
                        virtual int32_t Pause() = 0;
                        virtual int32_t Resume() = 0;
                        virtual int32_t Flush() = 0;
                        virtual int32_t TurnStandbyMode() = 0;
                        virtual int32_t AudioDevDump(int32_t range, int32_t fd) = 0;
                        virtual int32_t IsSupportsPauseAndResume(bool &supportPause, bool &supportResume) = 0;
                    };
                }
            }
        }
    }
}
#endif