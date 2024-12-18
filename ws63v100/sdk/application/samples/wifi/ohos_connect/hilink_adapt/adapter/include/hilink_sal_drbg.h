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
 * Description: 安全用途伪随机数适配层接口（此文件为DEMO，需集成方适配修改）
 */
#ifndef HILINK_SAL_DRBG_H
#define HILINK_SAL_DRBG_H

#include "hilink_sal_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* HiLinkDrbgContext;

/**
 * @brief 安全用途伪随机数发生器初始化
 *
 * @param custom [IN] 自定义业务字符串
 * @return 随机数发生器上下文
 */
HiLinkDrbgContext HILINK_SAL_DrbgInit(const char *custom);

/**
 * @brief 安全用途伪随机数获取
 *
 * @param ctx [IN] 随机数发生器上下文
 * @param out [OUT] 随机数缓冲区，可写长度至少为outLen
 * @param outLen [IN] 随机数缓冲区长度
 * @return 0成功，其他失败
 */
int HILINK_SAL_DrbgRandom(HiLinkDrbgContext ctx, unsigned char *out, unsigned int outLen);

/**
 * @brief 安全用途伪随机数去初始化
 *
 * @param ctx [IN] 随机数发生器上下文
 */
void HILINK_SAL_DrbgDeinit(HiLinkDrbgContext ctx);

#ifdef __cplusplus
}
#endif

#endif /* HILINK_RANDOM_ADAPTER_H */