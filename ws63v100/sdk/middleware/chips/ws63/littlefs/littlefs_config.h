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
 * Description: little file system config header.
 */

#ifndef LITTLSFS_CONFIG_H
#define LITTLSFS_CONFIG_H

#if defined(CONFIG_LFS_ADAPT_DEBUG) || defined(CONFIG_LFS_ADAPT_WARN) || defined(CONFIG_LFS_ADAPT_ERROR)
#include "debug_print.h"
#endif

#ifdef CONFIG_LFS_ADAPT_DEBUG
#define lfs_debug_print_info(fmt, arg...) print_str("LFS [I]:" fmt, ##arg)
#else
#define lfs_debug_print_info(fmt, arg...)
#endif

#ifdef CONFIG_LFS_ADAPT_WARN
#define lfs_debug_print_warning(fmt, arg...) print_str("LFS [W]:" fmt, ##arg)
#else
#define lfs_debug_print_warning(fmt, arg...)
#endif

#ifdef CONFIG_LFS_ADAPT_ERROR
#define lfs_debug_print_error(fmt, arg...) print_str("LFS [E]:" fmt, ##arg)
#else
#define lfs_debug_print_error(fmt, arg...)
#endif

#endif