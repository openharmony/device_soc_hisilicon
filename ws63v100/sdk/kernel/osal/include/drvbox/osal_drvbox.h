/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: OS Abstract Layer.
 */

/**
 * @defgroup osal_drvbox osal_drvbox
 */
#ifndef __OSAL_DRVBOX_H__
#define __OSAL_DRVBOX_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @ingroup osal_drvbox
 * @brief Switch region from usr box to driver box.
 *
 * @par Description:
 * This API is used to switch executing region from usr box to driver box, and change pmp/mpu configuration respectivly.
 *
 * @attention
 * This api can only be invoked in common usr lib.
 *
 * @param    drv_id    [in] used to check the invoking permission of the driver.
 *
 * @retval   DRV_BOX_INVALID_SWITCH  No need to do the switching process due to the limitation.
 * @retval   DRV_BOX_SWITCH_FAIL     Permission rejected.
 * @retval   LOS_OK                  Box region switching success.
 *
 * @par Support System:
 * seliteos.
 */
unsigned int osal_drvmgr_switch_to_drvbox(unsigned int drv_id);

/**
 * @ingroup osal_drvbox
 * @brief Switch region from driver box to usr box.
 *
 * @par Description:
 * This API is used to switch box running region from driver box to usr box,
 * and change pmp/mpu configuration respectivly.
 *
 * @attention
 * This api can only be invoked in common usr lib space.
 *
 * @par Support System:
 * seliteos.
 */
void osal_drvmgr_switch_to_usrbox(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* __OSAL_DRVBOX_H__ */