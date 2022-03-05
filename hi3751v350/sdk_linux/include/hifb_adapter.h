/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022. All rights reserved.
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
