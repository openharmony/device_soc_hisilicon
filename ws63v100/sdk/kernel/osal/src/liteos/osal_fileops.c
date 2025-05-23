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
 * Description: fileops
 *
 * Create: 2021-12-16
 */

#include <fcntl.h>
#ifdef LOSCFG_FS_VFS
#include <fs/fs.h>
#endif
#include <unistd.h>

#include "los_memory.h"
#include "soc_osal.h"
#include "osal_errno.h"
#include "osal_inner.h"

char *g_klib_store_path = NULL;
void *osal_klib_fopen(const char *filename, int flags, int mode)
{
    int *filp = NULL;
    if (filename == NULL) {
        return NULL;
    }
    int fd = open(filename, flags, mode);
    if (fd < 0) {
        osal_log("open failed! fd = %d, errno = %d\n", fd, get_errno());
        return NULL;
    } else {
        filp = (int *)LOS_MemAlloc((void*)m_aucSysMem0, sizeof(int));
        if (filp == NULL) {
            osal_log("LOS_MemAlloc failed!\n");
            close(fd);
            return NULL;
        }
        *filp = fd;
    }
    return (void *)filp;
}

void osal_klib_fclose(void *filp)
{
    if (filp == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    close(*(int *)filp);
    LOS_MemFree((void*)m_aucSysMem0, filp);
}

int osal_klib_fwrite(const char *buf, unsigned long size, void *filp)
{
    if (filp == NULL || buf == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    return write(*(int *)filp, buf, size);
}

int osal_klib_fread(char *buf, unsigned long size, void *filp)
{
    if (filp == NULL || buf == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    return read(*(int *)filp, buf, size);
}

int osal_klib_fseek(long long offset, int whence, void *filp)
{
    int ret;
    off_t res;
    if (filp == NULL || offset > INT32_MAX) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    res = lseek(*(int *)filp, (off_t)offset, whence);
    ret = (int)res;
    if (res != (off_t)ret) {
        ret = OSAL_EOVERFLOW;
    }
    return ret;
}

void osal_klib_fsync(void *filp)
{
    if (filp == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    fsync(*(int *)filp);
}
