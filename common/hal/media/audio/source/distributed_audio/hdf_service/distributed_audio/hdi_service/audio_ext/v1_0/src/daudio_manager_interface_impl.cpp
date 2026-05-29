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

#include "daudio_manager_interface_impl.h"

#include <hdf_base.h>

#include "daudio_errcode.h"
#include "daudio_log.h"
#include "daudio_utils.h"

#undef DH_LOG_TAG
#define DH_LOG_TAG "DAudioManagerInterfaceImpl"

using namespace OHOS::DistributedHardware;
using namespace OHOS::HDI::DistributedAudio::Audio::V1_0;

namespace OHOS {
namespace HDI {
namespace DistributedAudio {
namespace Audioext {
namespace V1_0 {
DAudioManagerInterfaceImpl *DAudioManagerInterfaceImpl::dAudioMgr_ = nullptr;
std::mutex DAudioManagerInterfaceImpl::mgrMtx_;
extern "C" IDAudioManager *DAudioManagerImplGetInstance(void)
{
    return DAudioManagerInterfaceImpl::GetDAudioManager();
}

DAudioManagerInterfaceImpl::DAudioManagerInterfaceImpl()
{
    DHLOGD("Distributed audio ext manager constructed.");
    audioMgr_ = AudioManagerInterfaceImpl::GetAudioManager();
}

DAudioManagerInterfaceImpl::~DAudioManagerInterfaceImpl()
{
    DHLOGD("Distributed audio ext manager destructed.");
}

int32_t DAudioManagerInterfaceImpl::RegisterAudioDevice(const std::string &adpName, int32_t devId,
    const std::string &capability, const std::shared_ptr<IDAudioCallback> &callbackObj)
{
    DHLOGI("Register audio device, name: %s, device: %d.", GetAnonyString(adpName).c_str(), devId);
    if (audioMgr_ == nullptr) {
        DHLOGE("Audio manager is null.");
        return HDF_FAILURE;
    }

    int32_t ret = audioMgr_->AddAudioDevice(adpName, devId, capability, callbackObj);
    if (ret != DH_SUCCESS) {
        DHLOGE("Register audio device failed, ret = %d", ret);
        return HDF_FAILURE;
    }

    DHLOGI("Register audio device success.");
    return HDF_SUCCESS;
}

int32_t DAudioManagerInterfaceImpl::UnRegisterAudioDevice(const std::string &adpName, int32_t devId)
{
    DHLOGI("UnRegister audio device, name: %s, device: %d.", GetAnonyString(adpName).c_str(), devId);
    if (audioMgr_ == nullptr) {
        DHLOGE("Audio manager is null.");
        return HDF_FAILURE;
    }

    int32_t ret = audioMgr_->RemoveAudioDevice(adpName, devId);
    if (ret != DH_SUCCESS) {
        DHLOGE("UnRegister audio devcie failed. ret = %d", ret);
        return HDF_FAILURE;
    }

    DHLOGI("UnRegister audio device success.");
    return HDF_SUCCESS;
}

int32_t DAudioManagerInterfaceImpl::NotifyEvent(const std::string &adpName, int32_t devId, const DAudioEvent &event)
{
    if (audioMgr_ == nullptr) {
        DHLOGE("Audio manager is null.");
        return HDF_FAILURE;
    }
    DHLOGI("Notify event. event type = %d", event.type);
    int32_t ret = audioMgr_->Notify(adpName, devId, event);
    if (ret != DH_SUCCESS) {
        DHLOGE("Notify audio event failed. ret = %d", ret);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}
} // V1_0
} // AudioExt
} // Daudio
} // HDI
} // OHOS
