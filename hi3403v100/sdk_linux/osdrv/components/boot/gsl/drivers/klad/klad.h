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

#ifndef __KLAD_H__
#define __KLAD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

int32_t klad_com_process(const rkp_deob_kdf *deob_kdf, common_hkl *com_klad,
			 const uint32_t check_wd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

