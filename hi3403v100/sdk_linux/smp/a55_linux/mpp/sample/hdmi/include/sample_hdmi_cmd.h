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

#ifndef SAMPLE_HDMI_CMD_H
#define SAMPLE_HDMI_CMD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "ot_type.h"
#include "sample_comm.h"
#include "sample_hdmi.h"
#include "sample_hdmi_video_path.h"

#define STR_LEN 20

typedef struct {
    td_u32 index;
    td_u8 index_string[STR_LEN];
} hdmi_input_param;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

td_s32 hdmi_test_cmd(td_char *string, td_u32 len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif

