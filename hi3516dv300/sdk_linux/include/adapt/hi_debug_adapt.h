/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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

#ifndef __HI_DEBUG_ADAPT_H__
#define __HI_DEBUG_ADAPT_H__

#ifndef __KERNEL__
#include <stdio.h>
#include <stdarg.h>
#endif

#ifdef __HuaweiLite__
#include<assert.h>
#else
#ifndef __KERNEL__
#include<assert.h>
#endif
#endif

#include "hi_type.h"
#include "hi_common_adapt.h"
#include "hi_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct {
    hi_mod_id  mod_id;
    hi_s32  level;
    hi_char mod_name[16];
} hi_log_level_conf;

#define MPI_STATIC static inline

#ifdef __HuaweiLite__

#ifdef CONFIG_HI_LOG_TRACE_SUPPORT

#define MPI_ASSERT(expr)              \
    do {                                   \
        if (!(expr)) {                    \
            (hi_void)printf("\nASSERT at:\n"       \
                   "  >Function : %s\n"   \
                   "  >Line No. : %d\n"   \
                   "  >Condition: %s\n",  __FUNCTION__, __LINE__, #expr); \
            assert(0 == 1); \
        } \
    } while (0)

#else

#define MPI_ASSERT(expr)

#endif

#else

#ifndef __KERNEL__

#ifdef CONFIG_HI_LOG_TRACE_SUPPORT

#define MPI_ASSERT(expr)              \
    do {                                   \
        if (!(expr)) {                    \
            (hi_void)printf("\nASSERT at:\n"       \
                   "  >Function : %s\n"   \
                   "  >Line No. : %d\n"   \
                   "  >Condition: %s\n",  __FUNCTION__, __LINE__, #expr); \
            assert(0 == 1); \
        } \
    } while (0)

#else

#define MPI_ASSERT(expr)

#endif

#endif

#endif

#ifdef __KERNEL__

int hi_log(hi_s32 level, hi_mod_id mod_id, const char *fmt, ...) __attribute__((format(printf, 3, 4)));

#endif /* end of __KERNEL__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DEBUG_ADAPT_H__ */

