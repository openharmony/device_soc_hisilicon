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
 * Description: oal_spinlock.h 的头文件
 * Create: 2022-4-27
 */

#ifndef __OAL_LINUX_SPINLOCK_H__
#define __OAL_LINUX_SPINLOCK_H__

#include <linux/sched.h>
#include <linux/spinlock.h>
#include "oal_util.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 函数指针，用来指向需要自旋锁保护的的函数 */
typedef osal_u32(*oal_irqlocked_func)(osal_void *);

#define OAL_SPIN_LOCK_MAGIC_TAG (0xdead4ead)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of oal_spinlock.h */
