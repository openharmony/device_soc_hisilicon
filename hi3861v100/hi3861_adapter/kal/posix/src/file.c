/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <securec.h>

#include "lwip/sockets.h"
#include "hks_client.h"
#include "hi_fs.h"

#define RANDOM_DEV_FD  (LWIP_SOCKET_OFFSET + LWIP_CONFIG_NUM_SOCKETS)
#define RANDOM_DEV_PATH  "/dev/random"

#define IS_SOCKET_FD(fd) ((fd) >= LWIP_SOCKET_OFFSET && (fd) < (LWIP_SOCKET_OFFSET + LWIP_CONFIG_NUM_SOCKETS))

#define HI_FS_MAX_OPEN_FILES (32+1)
#define HI_FS_FD_OFFSET RANDOM_DEV_FD

#define IS_HI_FS_FD(fd) ((fd) >= HI_FS_FD_OFFSET && (fd) < (HI_FS_FD_OFFSET + HI_FS_MAX_OPEN_FILES))
#define HI_FS_FD(fd) ((fd) - HI_FS_FD_OFFSET)

#define FREE_AND_SET_NULL(ptr) do { \
    free(ptr);                      \
    ptr = NULL;                     \
} while (0)

/**
 * @brief Get canonical form of a given path based on cwd(Current working directory).
 *
 * @param cwd Indicates the current working directory.
 * @param path Indicates the path to be canonicalization.
 * @param buf Indicates the pointer to the buffer where the result will be return.
 * @param bufSize Indicates the size of the buffer.
 * @return Returns the length of the canonical path.
 *
 * @attention if path is an absolute path, cwd is ignored. if cwd if not specified, it is assumed to be root('/').
 *            if the buffer is not big enough the result will be truncated, but the return value will always be the
 *            length of the canonical path.
 */
static size_t GetCanonicalPath(const char *cwd, const char *path, char *buf, size_t bufSize)
{
    if (!path) {
        path = "";
    }

    if (!cwd || path[0] == '/') {
        cwd = "";
    }

    size_t tmpLen = strlen(cwd) + strlen(path) + 4; // three '/' and one '\0'
    if (tmpLen <= 0) {
        return 0;
    }
    char *tmpBuf = (char *)malloc(tmpLen);
    if (tmpBuf == NULL) {
        return 0;
    }

    if (-1 == sprintf_s(tmpBuf, tmpLen, "/%s/%s/", cwd, path)) {
        free(tmpBuf);
        return 0;
    }

    char *p;
    /* replace /./ to / */
    while ((p = strstr(tmpBuf, "/./")) != NULL) {
        if (EOK != memmove_s(p, tmpLen - (p - tmpBuf), p + 2, tmpLen - (p - tmpBuf) - 2)) {
            free(tmpBuf);
            return 0;
        }
    }

    /* replace // to / */
    while ((p = strstr(tmpBuf, "//")) != NULL) {
        if (EOK != memmove_s(p, tmpLen - (p - tmpBuf), p + 1, tmpLen - (p - tmpBuf) - 1)) {
            free(tmpBuf);
            return 0;
        }
    }

    /* handle /../ (e.g., replace /aa/bb/../ to /aa/) */
    while ((p = strstr(tmpBuf, "/../")) != NULL) {
        char *start = p;
        while (start > tmpBuf && *--start != '/') {
        }
        if (EOK != memmove_s(start, tmpLen - (start - tmpBuf), p + 3, tmpLen - (p - tmpBuf) - 3)) {
            free(tmpBuf);
            return 0;
        }
    }

    size_t totalLen = strlen(tmpBuf);

    /* strip the last / */
    if (totalLen > 1 && tmpBuf[totalLen - 1] == '/') {
        tmpBuf[--totalLen] = 0;
    }

    if (!buf || bufSize == 0) {
        free(tmpBuf);
        return totalLen;
    }

    if (EOK != memcpy_s(buf, bufSize, tmpBuf, (totalLen + 1 > bufSize) ? bufSize : totalLen + 1)) {
        free(tmpBuf);
        return 0;
    }

    buf[bufSize - 1] = 0;
    free(tmpBuf);
    return totalLen;
}

int open(const char *file, int oflag, ...)
{
    int fd = -1;
    unsigned flags = O_RDONLY | O_WRONLY | O_RDWR | O_APPEND | O_CREAT | O_LARGEFILE | O_TRUNC | O_EXCL | O_DIRECTORY;
    if (((unsigned)oflag & ~flags) || (file == NULL)) {
        errno = EINVAL;
        return -1;
    }
    size_t pathLen = strlen(file) + 1;
    if (pathLen <= 0) {
        errno = EINVAL;
        return -1;
    }
    char *canonicalPath = (char *)malloc(pathLen);
    if (!canonicalPath) {
        errno = ENOMEM;
        return -1;
    }
    if (GetCanonicalPath(NULL, file, canonicalPath, pathLen) == 0) {
        FREE_AND_SET_NULL(canonicalPath);
        errno = ENOMEM;
        return -1;
    }
    if (strcmp(canonicalPath, RANDOM_DEV_PATH) == 0) {
        FREE_AND_SET_NULL(canonicalPath);
        if ((O_ACCMODE & (unsigned)oflag) != O_RDONLY) {
            errno = EPERM;
            return -1;
        }
        if ((unsigned)oflag & O_DIRECTORY) {
            errno = ENOTDIR;
            return -1;
        }
        return RANDOM_DEV_FD;
    }
    if (strcmp(canonicalPath, "/") == 0 || strcmp(canonicalPath, "/dev") == 0) {
        FREE_AND_SET_NULL(canonicalPath);
        if ((unsigned)oflag & O_DIRECTORY) {
            errno = EPERM;
            return -1;
        }
        errno = EISDIR;
        return -1;
    }
    FREE_AND_SET_NULL(canonicalPath);
    fd = hi_open(file, (uint32_t)oflag);
    if (fd < 0) {
	return -1;
    }
    return fd + HI_FS_FD_OFFSET;
}

int close(int fd)
{
    if (fd == RANDOM_DEV_FD) {
        return 0;
    }
    if (IS_SOCKET_FD(fd)) {
        return closesocket(fd);
    }
    if (IS_HI_FS_FD(fd)) {
        return hi_close(HI_FS_FD(fd));
    }
    errno = EBADF;
    return -1;
}

ssize_t read(int fd, void *buf, size_t nbytes)
{
    if (fd == RANDOM_DEV_FD) {
        if (nbytes == 0) {
            return 0;
        }
        if (buf == NULL) {
            errno = EINVAL;
            return -1;
        }
        if (nbytes > 1024) {
            nbytes = 1024; /* hks_generate_random: random_size must <= 1024 */
        }
        struct hks_blob key = {HKS_BLOB_TYPE_RAW, (uint8_t *)buf, nbytes};
        if (hks_generate_random(&key) != 0) {
            errno = EIO;
            return -1;
        }
        return (ssize_t)nbytes;
    }
    if (IS_SOCKET_FD(fd)) {
        return recv(fd, buf, nbytes, 0);
    }
    if (IS_HI_FS_FD(fd)) {
        return hi_read(HI_FS_FD(fd), buf, nbytes);
    }
    errno = EBADF;
    return -1;
}

ssize_t write(int fd, const void *buf, size_t nbytes)
{
    if (fd == RANDOM_DEV_FD) {
        errno = EBADF; /* "/dev/random" is readonly */
        return -1;
    }
    if (IS_SOCKET_FD(fd)) {
        return send(fd, buf, nbytes, 0);
    }
    if (IS_HI_FS_FD(fd)) {
        return hi_write(HI_FS_FD(fd), (const char*)buf, nbytes);
    }
    errno = EBADF;
    return -1;
}

off_t lseek(int fd, off_t offset, int whence)
{
    if (fd == RANDOM_DEV_FD) {
        errno = ENOTSUP;
        return (off_t)-1;
    }
    if (IS_SOCKET_FD(fd)) {
        errno = ESPIPE;
        return (off_t)-1;
    }
    if (IS_HI_FS_FD(fd)) {
        return hi_lseek(HI_FS_FD(fd), offset, whence);
    }
    errno = EBADF;
    return (off_t)-1;
}

int unlink(const char *path)
{
    return hi_unlink(path);
}

int fsync(int fd)
{
    (void)fd;
    return 0;
}
