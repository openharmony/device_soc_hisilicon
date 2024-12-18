/**
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
 *
 * Description: Implementation of secure random number Adaptation Layer Interfaces. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "app_call.h"
#include "hilink_sal_drbg.h"

HiLinkDrbgContext HILINK_SAL_DrbgInit(const char *custom)
{
    return app_call1(APP_CALL_HILINK_SAL_DRBG_INIT, HiLinkDrbgContext, const char *, custom);
}

void HILINK_SAL_DrbgDeinit(HiLinkDrbgContext ctx)
{
    app_call1(APP_CALL_HILINK_SAL_DRBG_DEINIT, void, HiLinkDrbgContext, ctx);
}

int HILINK_SAL_DrbgRandom(HiLinkDrbgContext ctx, unsigned char *out, unsigned int outLen)
{
    return app_call3(APP_CALL_HILINK_SAL_DRBG_RANDOM, int,
        HiLinkDrbgContext, ctx, unsigned char *, out, unsigned int, outLen);
}