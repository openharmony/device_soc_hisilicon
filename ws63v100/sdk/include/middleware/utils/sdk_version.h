/**
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 *
 * Description: Provides sdk version message \n
 *
 * History: \n
 * 2022-05-12, Create file. \n
 */

#ifndef SDK_VERSION_H
#define SDK_VERSION_H

#include "stdint.h"

/**
 * @defgroup middleware_utils_version Version
 * @ingroup  middleware_utils
 * @{
 */

/**
 * @if Eng
 * @brief  obtain sdk id string
 * @retval sdk id string
 * @endif
 */
const char *uapi_sdk_read_id(void);

/**
 * @}
 */

#endif /* SDK_VERSION_H */
