/*
* Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#ifndef HIFB_ADAPTER_H
#define HIFB_ADAPTER_H

#include "hi_osal_user.h"
#include "hifb_public.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USING_USERSPACE_DRV
#ifndef USING_OSAL_DEV
#ifdef OSAL_IOCTL
#undef OSAL_IOCTL
#endif
#define OSAL_IOCTL fb_ioctldev
#endif
int fb_ioctldev(int fd, unsigned int cmd, ...);
#endif

void FbIpcServerRegister(void);

#ifdef __cplusplus
}
#endif

#endif // HIFB_ADAPTER_H
