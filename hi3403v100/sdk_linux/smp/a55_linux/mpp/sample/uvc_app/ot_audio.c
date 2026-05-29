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

#include "ot_audio.h"

static struct ot_audio g_ot_audio = {
    .mpi_ac_ops = NULL,
    .audioing = 0,
};

ot_audio *get_ot_audio()
{
    return &g_ot_audio;
}

int ot_audio_init(void)
{
    if (get_ot_audio()->mpi_ac_ops && get_ot_audio()->mpi_ac_ops->init) {
        return get_ot_audio()->mpi_ac_ops->init();
    }

    return 0;
}

int ot_audio_startup(void)
{
    int ret = 0;

    if (get_ot_audio()->mpi_ac_ops && get_ot_audio()->mpi_ac_ops->startup) {
        ret = get_ot_audio()->mpi_ac_ops->startup();
    }

    if (ret == 0) {
        get_ot_audio()->audioing = 1;
    }
    return ret;
}

int ot_audio_shutdown(void)
{
    int ret = 0;

    if (get_ot_audio()->mpi_ac_ops && get_ot_audio()->mpi_ac_ops->shutdown) {
        ret = get_ot_audio()->mpi_ac_ops->shutdown();
    }

    if (ret == 0) {
        get_ot_audio()->audioing = 0;
    }
    return ret;
}

int ot_audio_register_mpi_ops(struct audio_control_ops *ac_ops)
{
    if (ac_ops != NULL) {
        get_ot_audio()->mpi_ac_ops = ac_ops;
    }

    return 0;
}
