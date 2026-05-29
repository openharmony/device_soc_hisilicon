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

#ifndef OHOS_HDI_IAUDIO_TYPES_H
#define OHOS_HDI_IAUDIO_TYPES_H

#include <string>
#include <memory>
#include <vector>
namespace OHOS {
    namespace HDI {
        namespace DistributedAudio {
            namespace Audio {
                namespace V1_0 {
                    enum AudioPortDirection {
                        PORT_OUT = 1, /**< Output port */
                        PORT_IN = 2, /**< Input port */
                        PORT_OUT_IN = 3, /**< Input/output port, supporting both audio input and output */
                    };
                    enum AudioPortPin {
                        PIN_NONE = 0,                 /**< Invalid pin */
                        PIN_OUT_SPEAKER = 1 << 0,            /**< Speaker output pin */
                        PIN_OUT_HEADSET = 1 << 1,            /**< Wired headset pin for output */
                        PIN_OUT_LINEOUT = 1 << 2,            /**< Line-out pin */
                        PIN_OUT_HDMI = 1 << 3,            /**< HDMI output pin */
                        PIN_OUT_USB = 1 << 4,            /**< USB output pin */
                        PIN_OUT_USB_EXT = 1 << 5,            /**< Extended USB output pin*/
                        PIN_OUT_EARPIECE = 1 << 5 | 1 << 4,   /**< Earpiece output pin */
                        PIN_OUT_BLUETOOTH_SCO = 1 << 6,            /**< Bluetooth SCO output pin */
                        PIN_OUT_DAUDIO_DEFAULT = 1 << 7,
                        PIN_IN_MIC = 1 << 27 | 1 << 0,  /**< Microphone input pin */
                        PIN_IN_HS_MIC = 1 << 27 | 1 << 1,  /**< Wired headset microphone pin for input */
                        PIN_IN_LINEIN = 1 << 27 | 1 << 2,  /**< Line-in pin */
                        PIN_IN_USB_EXT = 1 << 27 | 1 << 3,  /**< Extended USB input pin*/
                        PIN_IN_BLUETOOTH_SCO_HEADSET = 1 << 27 | 1 << 4,  /**< Bluetooth SCO headset input pin */
                        PIN_IN_DAUDIO_DEFAULT = 1 << 27 | 1 << 5,
                    };
                    enum AudioCategory {
                        AUDIO_IN_MEDIA = 0,
                        AUDIO_IN_COMMUNICATION = 1,
                        AUDIO_IN_RINGTONE = 2,
                        AUDIO_IN_CALL = 3,
                        AUDIO_MMAP_NOIRQ = 4,
                    };
                    enum AudioFormat {
                        AUDIO_FORMAT_TYPE_PCM_8_BIT = 1 << 0,                      /**< 8-bit PCM */
                        AUDIO_FORMAT_TYPE_PCM_16_BIT = 1 << 1,                      /**< 16-bit PCM */
                        AUDIO_FORMAT_TYPE_PCM_24_BIT = 1 << 1 | 1 << 0,             /**< 24-bit PCM */
                        AUDIO_FORMAT_TYPE_PCM_32_BIT = 1 << 2,                      /**< 32-bit PCM */
                        AUDIO_FORMAT_TYPE_AAC_MAIN = 1 << 24 | 1 << 0,            /**< AAC main */
                        AUDIO_FORMAT_TYPE_AAC_LC = 1 << 24 | 1 << 1,            /**< AAC LC */
                        AUDIO_FORMAT_TYPE_AAC_LD = 1 << 24 | 1 << 1 | 1 << 0,   /**< AAC LD */
                        AUDIO_FORMAT_TYPE_AAC_ELD = 1 << 24 | 1 << 2,            /**< AAC ELD */
                        AUDIO_FORMAT_TYPE_AAC_HE_V1 = 1 << 24 | 1 << 2 | 1 << 0,   /**< AAC HE_V1 */
                        AUDIO_FORMAT_TYPE_AAC_HE_V2 = 1 << 24 | 1 << 2 | 1 << 1,   /**< AAC HE_V2 */
                        AUDIO_FORMAT_TYPE_G711A = 1 << 25 | 1 << 0,            /**< G711A */
                        AUDIO_FORMAT_TYPE_G711U = 1 << 25 | 1 << 1,            /**< G711u */
                        AUDIO_FORMAT_TYPE_G726 = 1 << 25 | 1 << 1 | 1 << 0,   /**< G726 */
                    };
                    enum AudioChannelMask {
                        AUDIO_CHANNEL_FRONT_LEFT = 1,  /**< Front left channel */
                        AUDIO_CHANNEL_FRONT_RIGHT = 2,  /**< Front right channel */
                        AUDIO_CHANNEL_MONO = 1,  /**< Mono channel */
                        /**< Stereo channel, consisting of front left and front right channels */
                        AUDIO_CHANNEL_STEREO = 3,
                    };
                    enum AudioSampleRatesMask {
                        AUDIO_SAMPLE_RATE_MASK_8000 = 1 << 0,        /**< 8 kHz */
                        AUDIO_SAMPLE_RATE_MASK_12000 = 1 << 1,        /**< 12 kHz */
                        AUDIO_SAMPLE_RATE_MASK_11025 = 1 << 2,        /**< 11.025 kHz */
                        AUDIO_SAMPLE_RATE_MASK_16000 = 1 << 3,        /**< 16 kHz */
                        AUDIO_SAMPLE_RATE_MASK_22050 = 1 << 4,        /**< 22.050 kHz */
                        AUDIO_SAMPLE_RATE_MASK_24000 = 1 << 5,        /**< 24 kHz */
                        AUDIO_SAMPLE_RATE_MASK_32000 = 1 << 6,        /**< 32 kHz */
                        AUDIO_SAMPLE_RATE_MASK_44100 = 1 << 7,        /**< 44.1 kHz */
                        AUDIO_SAMPLE_RATE_MASK_48000 = 1 << 8,        /**< 48 kHz */
                        AUDIO_SAMPLE_RATE_MASK_64000 = 1 << 9,        /**< 64 kHz */
                        AUDIO_SAMPLE_RATE_MASK_96000 = 1 << 10,       /**< 96 kHz */
                        AUDIO_SAMPLE_RATE_MASK_INVALID = 4294967295,    /**< Invalid sampling rate */
                    };
                    enum AudioPortPassthroughMode {
                        PORT_PASSTHROUGH_LPCM = 1,
                        PORT_PASSTHROUGH_RAW = 2,
                        PORT_PASSTHROUGH_HBR2LBR = 4,
                        PORT_PASSTHROUGH_AUTO = 8,
                    };
                    enum AudioSampleFormat {
                        AUDIO_SAMPLE_FORMAT_S8 = 0,
                        AUDIO_SAMPLE_FORMAT_S8P = 1,
                        AUDIO_SAMPLE_FORMAT_U8 = 2,
                        AUDIO_SAMPLE_FORMAT_U8P = 3,
                        AUDIO_SAMPLE_FORMAT_S16 = 4,
                        AUDIO_SAMPLE_FORMAT_S16P = 5,
                        AUDIO_SAMPLE_FORMAT_U16 = 6,
                        AUDIO_SAMPLE_FORMAT_U16P = 7,
                        AUDIO_SAMPLE_FORMAT_S24 = 8,
                        AUDIO_SAMPLE_FORMAT_S24P = 9,
                        AUDIO_SAMPLE_FORMAT_U24 = 10,
                        AUDIO_SAMPLE_FORMAT_U24P = 11,
                        AUDIO_SAMPLE_FORMAT_S32 = 12,
                        AUDIO_SAMPLE_FORMAT_S32P = 13,
                        AUDIO_SAMPLE_FORMAT_U32 = 14,
                        AUDIO_SAMPLE_FORMAT_U32P = 15,
                        AUDIO_SAMPLE_FORMAT_S64 = 16,
                        AUDIO_SAMPLE_FORMAT_S64P = 17,
                        AUDIO_SAMPLE_FORMAT_U64 = 18,
                        AUDIO_SAMPLE_FORMAT_U64P = 19,
                        AUDIO_SAMPLE_FORMAT_F32 = 20,
                        AUDIO_SAMPLE_FORMAT_F32P = 21,
                        AUDIO_SAMPLE_FORMAT_F64 = 22,
                        AUDIO_SAMPLE_FORMAT_F64P = 23,
                    };
                    enum AudioChannelMode {
                        AUDIO_CHANNEL_NORMAL = 0,
                        AUDIO_CHANNEL_BOTH_LEFT = 1,
                        AUDIO_CHANNEL_BOTH_RIGHT = 2,
                        AUDIO_CHANNEL_EXCHANGE = 3,
                        AUDIO_CHANNEL_MIX = 4,
                        AUDIO_CHANNEL_LEFT_MUTE = 5,
                        AUDIO_CHANNEL_RIGHT_MUTE = 6,
                        AUDIO_CHANNEL_BOTH_MUTE = 7,
                    };
                    enum AudioDrainNotifyType {
                        AUDIO_DRAIN_NORMAL_MODE = 0,
                        AUDIO_DRAIN_EARLY_MODE = 1,
                    };
                    enum AudioCallbackType {
                        AUDIO_NONBLOCK_WRITE_COMPELETED = 0,
                        AUDIO_DRAIN_COMPELETED = 1,
                        AUDIO_FLUSH_COMPLETED = 2,
                        AUDIO_RENDER_FULL = 3,
                        AUDIO_ERROR_OCCUR = 4,
                    };
                    enum AudioPortRole {
                        AUDIO_PORT_UNASSIGNED_ROLE = 0,
                        AUDIO_PORT_SOURCE_ROLE = 1,
                        AUDIO_PORT_SINK_ROLE = 2,
                    };
                    enum AudioPortType {
                        AUDIO_PORT_UNASSIGNED_TYPE = 0,
                        AUDIO_PORT_DEVICE_TYPE = 1,
                        AUDIO_PORT_MIX_TYPE = 2,
                        AUDIO_PORT_SESSION_TYPE = 3,
                    };
                    enum AudioSessionType {
                        AUDIO_OUTPUT_STAGE_SESSION = 0,
                        AUDIO_OUTPUT_MIX_SESSION = 1,
                        AUDIO_ALLOCATE_SESSION = 2,
                        AUDIO_INVALID_SESSION = 3,
                    };
                    enum AudioDeviceType {
                        AUDIO_LINEOUT = 1 << 0,
                        AUDIO_HEADPHONE = 1 << 1,
                        AUDIO_HEADSET = 1 << 2,
                        AUDIO_USB_HEADSET = 1 << 3,
                        AUDIO_USB_HEADPHONE = 1 << 4,
                        AUDIO_USBA_HEADSET = 1 << 5,
                        AUDIO_USBA_HEADPHONE = 1 << 6,
                        AUDIO_PRIMARY_DEVICE = 1 << 7,
                        AUDIO_USB_DEVICE = 1 << 8,
                        AUDIO_A2DP_DEVICE = 1 << 9,
                        AUDIO_DEVICE_UNKNOWN,
                    };
                    enum AudioEventType {
                        AUDIO_DEVICE_ADD = 1,
                        AUDIO_DEVICE_REMOVE = 2,
                        AUDIO_LOAD_SUCCESS = 3,
                        AUDIO_LOAD_FAILURE = 4,
                        AUDIO_UNLOAD = 5,
                        AUDIO_SERVICE_VALID = 7,
                        AUDIO_SERVICE_INVALID = 8,
                        AUDIO_CAPTURE_THRESHOLD = 9,
                        AUDIO_EVENT_UNKNOWN = 10,
                    };
                    enum AudioExtParamKey {
                        AUDIO_EXT_PARAM_KEY_NONE = 0,     /**< Distributed audio extra param key none */
                        AUDIO_EXT_PARAM_KEY_VOLUME = 1,   /**< Distributed audio extra param key volume event */
                        AUDIO_EXT_PARAM_KEY_FOCUS = 2,    /**< Distributed audio extra param key focus event */
                        AUDIO_EXT_PARAM_KEY_BUTTON = 3,   /**< Distributed audio extra param key media button event */
                        AUDIO_EXT_PARAM_KEY_EFFECT = 4,   /**< Distributed audio extra param key audio effect event */
                        AUDIO_EXT_PARAM_KEY_STATUS = 5,   /**< Distributed audio extra param key device status event */
                        AUDIO_EXT_PARAM_KEY_LOWPOWER = 1000, /**< Low power event type */
                    };
                    struct AudioDeviceStatus {
                        uint32_t pnpStatus;
                    };
                    union SceneDesc {
                        uint32_t id;
                    };
                    struct AudioPort {
                        enum AudioPortDirection dir;
                        uint32_t portId;
                        std::string portName;
                    };
                    struct AudioAdapterDescriptor {
                        std::string adapterName;
                        std::vector<AudioPort> ports;
                    };
                    struct AudioDeviceDescriptor {
                        uint32_t portId;
                        enum AudioPortPin pins;
                        std::string desc;
                    };
                    struct AudioSceneDescriptor {
                        union SceneDesc scene;
                        struct AudioDeviceDescriptor desc;
                    };
                    struct AudioSampleAttributes {
                        enum AudioCategory type;
                        bool interleaved;
                        enum AudioFormat format;
                        uint32_t sampleRate;
                        uint32_t channelCount;
                        uint32_t period;
                        uint32_t frameSize;
                        bool isBigEndian;
                        bool isSignedData;
                        uint32_t startThreshold;
                        uint32_t stopThreshold;
                        uint32_t silenceThreshold;
                        int32_t streamId;
                    };
                    struct AudioTimeStamp {
                        int64_t tvSec;
                        int64_t tvNSec;
                    };
                    struct AudioSubPortCapability {
                        uint32_t portId;
                        std::string desc;
                        enum AudioPortPassthroughMode mask;
                    };
                    struct AudioPortCapability {
                        uint32_t deviceType;
                        uint32_t deviceId;
                        bool hardwareMode;
                        uint32_t formatNum;
                        std::vector <enum AudioFormat> formats;
                        uint32_t sampleRateMasks;
                        enum AudioChannelMask channelMasks;
                        uint32_t channelCount;
                        std::vector<struct AudioSubPortCapability> subPorts;
                        std::vector<enum AudioSampleFormat> supportSampleFormats;
                    };
                    struct AudioMmapBufferDescriptor {
                        std::vector<int8_t> memoryAddress;
                        int32_t memoryFd;
                        int32_t totalBufferFrames;
                        int32_t transferFrameSize;
                        int32_t isShareable;
                        uint32_t offset;
                        std::string filePath;
                    };
                    struct AudioDevExtInfo {
                        int32_t moduleId;
                        enum AudioPortPin type;
                        std::string desc;
                    };
                    struct AudioMixExtInfo {
                        int32_t moduleId;
                        int32_t streamId;
                    };
                    struct AudioSessionExtInfo {
                        enum AudioSessionType sessionType;
                    };
                    struct AudioInfo {
                        struct AudioDevExtInfo device;
                        struct AudioMixExtInfo mix;
                        struct AudioSessionExtInfo session;
                    };
                    struct AudioRouteNode {
                        int32_t portId;
                        enum AudioPortRole role;
                        enum AudioPortType type;
                        struct AudioInfo ext;
                    };
                    struct AudioRoute {
                        std::vector<struct AudioRouteNode> sources;
                        std::vector<struct AudioRouteNode> sinks;
                    };
                    struct AudioEvent {
                        uint32_t eventType; /**< @link enum AudioEventType */
                        uint32_t deviceType; /**< @link enum AudioDeviceType */
                    };
                }
            }
        }
    }
}
#endif