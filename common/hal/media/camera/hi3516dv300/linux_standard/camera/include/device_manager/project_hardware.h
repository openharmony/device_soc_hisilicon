/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef HOS_CAMERA_PROJET_HARDWARE_H
#define HOS_CAMERA_PROJET_HARDWARE_H

namespace OHOS::Camera {
std::vector<HardwareConfiguration> hardware = {
    {CAMERA_FIRST, DM_M_VI, DM_C_VI, (std::string) "vi"},
    {CAMERA_FIRST, DM_M_VO, DM_C_VO, (std::string) "vo"},
    {CAMERA_FIRST, DM_M_VI, DM_C_SENSOR, (std::string) "Imx335"},
    {CAMERA_FIRST, DM_M_VPSS, DM_C_VPSS, (std::string) "vpss"},
    {CAMERA_SECOND, DM_M_VI, DM_C_SENSOR, (std::string) "Imx600"},
    {CAMERA_SECOND, DM_M_VO, DM_C_VO, (std::string) "vo"},
    {CAMERA_SECOND, DM_M_VI, DM_C_VI, (std::string) "vi"}
};
} // namespace OHOS::Camera
#endif
