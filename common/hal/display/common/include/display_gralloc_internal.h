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

#ifndef HI_DISP_GRALLOC_H
#define HI_DISP_GRALLOC_H
#include "display_type.h"
#include "disp_common.h"
#include "buffer_handle.h"
#ifndef _WIN32
#include "display_adapt_fb.h"
#include "display_adapt_sys.h"
#endif

/* max allowed malloc memory:256M */
#define MAX_MALLOC_SIZE 0x10000000L

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#define ALIGN_UP(x, a) ((((x) + ((a) - 1)) / (a)) * (a))
#define HEIGHT_ALIGN 2U
#define WIDTH_ALIGN 1U
#define MAX_PLANES 3

#define DEFAULT_READ_WRITE_PERMISSIONS 0666
#define SHM_MAX_KEY 10000
#define SHM_START_KEY 1
#define INVALID_SHMID (-1)
#define BITS_PER_BYTE 8

#ifndef DISPLAY_CHK_GOTO
#define DISPLAY_CHK_GOTO(val, ret, ...) \
    do {                                \
        if (val) {                      \
            __VA_ARGS__;                \
            goto ret;                   \
        }                               \
    } while (0)
#endif

typedef struct {
    BufferHandle hdl;
    int32_t shmid;
} PriBufferHandle;

int32_t ShareMemory(uint64_t phyAddr, BufferHandle *buffer);
int32_t UnShareMemory(BufferHandle *buffer);

#endif /* HI_DISP_GRALLOC_H */
