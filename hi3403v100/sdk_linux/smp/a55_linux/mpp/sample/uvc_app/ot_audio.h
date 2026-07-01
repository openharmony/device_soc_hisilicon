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

#ifndef __OT_AUDIO_H__
#define __OT_AUDIO_H__

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

typedef struct audio_control_ops {
    int (*init)(void);
    int (*startup)(void);
    int (*shutdown)(void);
} audio_control_ops_st;

typedef struct ot_audio {
    struct audio_control_ops *mpi_ac_ops;
    int audioing;
} ot_audio;

/* audio control functions */
extern int ot_audio_register_mpi_ops(struct audio_control_ops *ac_ops);

extern int ot_audio_init(void);
extern int ot_audio_startup(void);
extern int ot_audio_shutdown(void);

#endif // __OT_AUDIO_H__
