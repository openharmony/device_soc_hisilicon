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

#ifndef OT_AIO_MOD_INIT_H
#define OT_AIO_MOD_INIT_H

#include "ot_type.h"
#include "ot_osal.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

/* variable init */
td_void aiao_set_aio_base(td_void *aio_base);
td_void aiao_set_acodec_base(td_void *acodec_base);
td_void aiao_set_reg_acodec_base(td_void *reg_acodec_base);
td_void aiao_set_aiao_irq(td_u32 aiao_irq);

/* export symbol */
td_void *aiao_get_acodec_base(td_void);

int aiao_module_init(void);
void aiao_module_exit(void);

int ot_aiao_mod_init(void);
void ot_aiao_mod_exit(void);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef OT_AIO_MOD_INIT_H */
