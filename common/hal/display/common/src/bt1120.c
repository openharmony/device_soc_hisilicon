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

#include "display_layer_internal.h"
#ifdef __BT1120_SUPPORT__
#include "ss_mpi_vb.h"
#include "ss_mpi_audio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */
int32_t EnableVOWithBT1120(ot_vo_dev voDev)
{
    ss_mpi_vo_disable(voDev);

    ot_vo_pub_attr pubAttr = { 0 };
    int32_t ret = ss_mpi_vo_get_pub_attr(voDev, &pubAttr);

    pubAttr.intf_type = OT_VO_INTF_BT1120;
    pubAttr.intf_sync = OT_VO_OUT_1080P60;
    pubAttr.bg_color = COLOR_RGB_BLUE;
    ret = ss_mpi_vo_set_pub_attr(voDev, &pubAttr);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("set vo atter failed with %#x!\n", ret);
        return DISPLAY_FAILURE;
    }

    ret = ss_mpi_vo_enable(voDev);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("vo enable failed with %#x!\n", ret);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

int32_t DisableVOWithBT1120(ot_vo_dev voDev)
{
    ss_mpi_vo_disable(voDev);
    return DISPLAY_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif  // __BT1120_SUPPORT__
