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
 * Description: thruput test header file.
 */


#ifndef __HMAC_THRUPUT_TEST_H__
#define __HMAC_THRUPUT_TEST_H__

#include "oal_ext_if.h"
#include "dmac_ext_if_hcm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THRUPUT_BYPASS_LEN  1300    /* WIFI侧hw bypass发送长度:仅bypass iperf跑流长包 */

osal_void hmac_set_thruput_test(osal_u8 type, osal_u8 value);
osal_void hmac_update_thruput(osal_u32 thruput);
osal_u8 hmac_is_thruput_test(osal_void);
osal_u8 hmac_is_thruput_enable(osal_u8 type);
osal_void hmac_delay(osal_u32 pos);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_thruput_test.h */
