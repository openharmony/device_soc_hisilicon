/**
 * @file cstl_public.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
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

 * @brief cstl_public 公共定义实现源码
 * @details cstl模块公共定义源码
 * @date 2021-05-14
 * @version v0.1.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author     <th>Description
 * </table>
 * *******************************************************************************************
 */

#include "cstl_public_inner.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup cstl
 */
__attribute__((visibility("default"))) bool IsMultiOverflow(size_t x, size_t y)
{
    bool ret = false;

    if ((x > 0) && (y > 0)) {
        ret = ((SIZE_MAX / x) < y) ? true : false;
    }

    return ret;
}

/* 加法溢出判断 */
/**
 * @ingroup cstl
 */
__attribute__((visibility("default"))) bool IsAddOverflow(size_t x, size_t y)
{
    return ((x + y) < x);
}

/**
 * @ingroup cstl
 */
__attribute__((visibility("default"))) int32_t CstlIntCmpFunc(uintptr_t data1, uintptr_t data2)
{
    int32_t ret;
    uintptr_t key1Value = data1;
    uintptr_t key2Value = data2;

    if (key1Value > key2Value) {
        ret = 1;
    } else if (key1Value < key2Value) {
        ret = -1;
    } else {
        ret = 0;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif
