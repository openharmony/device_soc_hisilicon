/*
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
 * Description: define hmac version
 * Create: 2021-9-28
 */

#ifndef __HMAC_VERSION_H__
#define __HMAC_VERSION_H__

#include "osal_types.h"

/*
 * WSxx [  a  ].[  c  ].T[  c  ].[  d  ]
 * WSxx [1-255].[1-255].T[1-255].[1-255]
 */
typedef union {
    osal_u32 ver;
    struct {
        osal_u32 d : 8; /* [ 7,  0] */
        osal_u32 c : 8; /* [15,  8] */
        osal_u32 b : 8; /* [23, 16] */
        osal_u32 a : 8; /* [31, 24] */
    } bits;
} version_u;

typedef union {
    osal_u32 ver;
    struct {
        osal_u32 c : 8; /* [7..0] */
        osal_u32 r : 8; /* [15..8] */
        osal_u32 v : 16; /* [31..16] */
    } bits;
} rom_version_u;

typedef struct {
    version_u host;
    version_u device;
    rom_version_u device_rom;
} version_t;

osal_u32 hmac_get_version(osal_void);
osal_u32 hmac_get_device_version(osal_void);
osal_u32 hmac_get_device_rom_version(osal_void);

#endif

