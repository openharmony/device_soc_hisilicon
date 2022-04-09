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

#ifndef __HI_OSAL_USER_H__
#define __HI_OSAL_USER_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USING_USERSPACE_DRV // user space

#ifdef USING_OSAL_DEV
extern int osal_opendev(const char *path, int flag, ...);
extern int osal_closedev(int fd);
extern int osal_readdev(int fd, void *buf, unsigned long count);
extern int osal_writedev(int fd, const void *buf, unsigned long count);
extern int osal_ioctldev(int fd, unsigned int cmd, ...);
extern void *osal_mmapdev(void *addr, unsigned long length, int prot, int flags, int fd, unsigned long offset);
#define OSAL_OPEN     osal_opendev
#define OSAL_READ     osal_readdev
#define OSAL_WRITE    osal_writedev
#define OSAL_IOCTL    osal_ioctldev
#define OSAL_CLOSE    osal_closedev
#define OSAL_MMAP     osal_mmapdev
#else
extern int soc_opendev(const char *path, int flag, ...);
extern int soc_closedev(int fd);
extern int soc_readdev(int fd, void *buf, unsigned long count);
extern int soc_writedev(int fd, const void *buf, unsigned long count);
extern int soc_ioctldev(int fd, unsigned int cmd, ...);
extern void *soc_mmapdev(void *addr, unsigned long length, int prot, int flags, int fd, unsigned long offset);
#define OSAL_OPEN     soc_opendev
#define OSAL_READ     soc_readdev
#define OSAL_WRITE    soc_writedev
#define OSAL_IOCTL    soc_ioctldev
#define OSAL_CLOSE    soc_closedev
#define OSAL_MMAP     soc_mmapdev
#endif

#else // kernel space
#define OSAL_OPEN     open
#define OSAL_READ     read
#define OSAL_WRITE    write
#define OSAL_IOCTL    ioctl
#define OSAL_CLOSE    close
#define OSAL_MMAP     mmap
#endif

#ifdef __cplusplus
}
#endif
#endif
