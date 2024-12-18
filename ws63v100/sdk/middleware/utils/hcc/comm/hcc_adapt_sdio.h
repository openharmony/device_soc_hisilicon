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
 * Description: hcc adapt SDIO completion.
 * Author:
 * Create: 2021-07-26
 */

#ifndef HCC_ADAPT_SDIO_HEADER
#define HCC_ADAPT_SDIO_HEADER

#ifdef CONFIG_HCC_SUPPORT_SDIO
#include "hcc_bus_types.h"
#include "hcc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

hcc_bus *hcc_adapt_sdio_load(hcc_handler *hcc);
td_void hcc_adapt_sdio_unload(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* CONFIG_HCC_SUPPORT_SDIO */
#endif /* HCC_ADAPT_SDIO_HEADER */
