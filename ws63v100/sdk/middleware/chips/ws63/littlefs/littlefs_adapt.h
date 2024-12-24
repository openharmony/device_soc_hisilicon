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
 * Description: little file system adapt layer.
 */

#ifndef LITTLSFS_ADAPT_H
#define LITTLSFS_ADAPT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
void fs_adapt_mount(void);

void fs_adapt_unmount(void);

int fs_adapt_open(const char* path, int oflag);

int fs_adapt_close(int fd);

int fs_adapt_read(int fd, char *buf, unsigned int len);

int fs_adapt_write(int fd, const char *buf, unsigned int len);

int fs_adapt_delete(const char *path);

int fs_adapt_stat(const char *path, unsigned int *file_size);

int fs_adapt_seek(int fd, int offset, unsigned int whence);

int fs_adapt_sync(int fd);

int fs_adapt_mkdir(const char *path);

void lfs_test(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
 
#endif
