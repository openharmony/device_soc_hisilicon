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

#ifndef __OT_OSAL_USER__
#define __OT_OSAL_USER__

#include "autoconf.h"
#ifdef CONFIG_USER_SPACE
#include <sys/types.h>
#else
#include <linux/types.h>
#endif
#include "ot_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef CONFIG_USER_SPACE

typedef struct {
    fd_set fds;
    int dev;
} osal_dev_fd_set;

int osal_opendev(const char *path, int flag, ...);
int osal_closedev(int fd);
int osal_readdev(int fd, void *buf, unsigned long count);
int osal_writedev(int fd, const void *buf, unsigned long count);
int osal_ioctldev(int fd, unsigned int cmd, ...);
void *osal_mmapdev(void *addr, unsigned long length, int prot, int flags, int fd, unsigned long offset);
int osal_init(void);
void osal_exit(void);

#define OSAL_OPEN     osal_opendev
#define OSAL_READ     osal_readdev
#define OSAL_WRITE    osal_writedev
#define OSAL_IOCTL    osal_ioctldev
#define OSAL_CLOSE    osal_closedev
#define OSAL_MMAP     osal_mmapdev
#define OSAL_SELECT   osal_select
#define osal_fd_set   osal_dev_fd_set

int osal_select(int nfds, osal_fd_set *readfds, osal_fd_set *writefds, osal_fd_set *exceptfds, struct timeval *timeout);
void osal_FD_ZERO(osal_fd_set *set);
void osal_FD_SET(int fd, osal_fd_set *set);
void osal_FD_CLR(int fd, osal_fd_set *set);
int osal_FD_ISSET(int fd, osal_fd_set *set);

#define OSAL_FD_ZERO  osal_FD_ZERO
#define OSAL_FD_SET   osal_FD_SET
#define OSAL_FD_CLR   osal_FD_CLR
#define OSAL_FD_ISSET osal_FD_ISSET

#else // kernel space
#define OSAL_OPEN     open
#define OSAL_READ     read
#define OSAL_WRITE    write
#define OSAL_IOCTL    ioctl
#define OSAL_CLOSE    close
#define OSAL_MMAP     mmap
#define OSAL_SELECT   select
#define osal_fd_set   fd_set
#define OSAL_FD_ZERO  FD_ZERO
#define OSAL_FD_SET   FD_SET
#define OSAL_FD_ISSET FD_ISSET
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
