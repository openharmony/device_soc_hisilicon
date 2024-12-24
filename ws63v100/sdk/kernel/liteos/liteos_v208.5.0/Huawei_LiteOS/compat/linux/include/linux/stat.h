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
 */
#ifndef _LINUX_STAT_H
#define _LINUX_STAT_H

#include "sys/stat.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !defined(S_IRUGO)
#define COMPAT_S_IRUGO (S_IRUSR | S_IRGRP | S_IROTH)
#define S_IRUGO COMPAT_S_IRUGO
#endif
#if !defined(S_IWUGO)
#define COMPAT_S_IWUGO (S_IWUSR | S_IWGRP | S_IWOTH)
#define S_IWUGO COMPAT_S_IWUGO
#endif
#if !defined(S_IXUGO)
#define COMPAT_S_IXUGO (S_IXUSR | S_IXGRP | S_IXOTH)
#define S_IXUGO COMPAT_S_IXUGO
#endif
#if !defined(S_IRWXUGO)
#define COMPAT_S_IRWXUGO (S_IRWXU | S_IRWXG | S_IRWXO)
#define S_IRWXUGO COMPAT_S_IRWXUGO
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINUX_STAT_H */
