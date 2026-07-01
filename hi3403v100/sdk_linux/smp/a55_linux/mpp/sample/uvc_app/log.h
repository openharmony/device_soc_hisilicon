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

#ifndef __UVC_LOG_H__
#define __UVC_LOG_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#define log(...) fprintf(stderr, __VA_ARGS__)
#define rlog(format, ...) fprintf(stderr, "\033[;31m" format "\033[0m\n", ##__VA_ARGS__)

#define info(...) log_info("info", __func__, __LINE__, __VA_ARGS__)
#define err(...) log_info("Error", __func__, __LINE__, __VA_ARGS__)
#define err_on(cond, ...) ((cond) ? err(__VA_ARGS__) : 0)

#define crit(...)                                              \
    do {                                                       \
        log_info("Critical", __func__, __LINE__, __VA_ARGS__); \
        exit(0);                                               \
    } while (0)

#define crit_on(cond, ...)     \
    do {                       \
        if (cond)              \
            crit(__VA_ARGS__); \
    } while (0)

void log_info(const char *prefix, const char *file, int line, const char *fmt, ...);

#endif // __UVC_LOG_H__
