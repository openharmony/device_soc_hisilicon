/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: OS Abstract Layer.
 */

/**
 * @defgroup osal_debug osal_debug
 */
#ifndef __OSAL_DEBUG_H__
#define __OSAL_DEBUG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef OSAL_DEBUG
#define OSAL_BUG() \
    do {           \
    } while (1)
#else
#define OSAL_BUG()
#endif
#define OSAL_ASSERT(expr)                       \
    do {                                        \
        if (!(expr)) {                          \
            osal_printk("\nASSERT failed at:\n" \
                "  >Condition: %s\n",           \
                #expr);                         \
            OSAL_BUG();                         \
        }                                       \
    } while (0)


#define OSAL_BUG_ON(expr)                                                  \
    do {                                                                   \
        if (expr) {                                                        \
            osal_printk("BUG: failure at %d/%s()!\n", __LINE__, __func__); \
            OSAL_BUG();                                                    \
        }                                                                  \
    } while (0)

/**
 * @ingroup osal_debug
 * @brief Log printing function.
 *
 * @par Support System:
 * linux liteos seliteos freertos.
 */
void osal_printk(const char *fmt, ...);

/**
 * @ingroup osal_debug
 * @brief Kernel panic function.
 *
 * @par Description:
 * Stack function that prints kernel panics.
 * After this function is called and stack information is printed, the system will fail to respond.
 *
 * @par Support System:
 * linux liteos.
 */
void osal_panic(const char *fmt, const char *fun, int line, const char *cond);

/**
 * @ingroup osal_debug
 * @brief Kernel backtrace function.
 *
 * @par Description:
 * Backtrace function that prints task call stack information traced from the running task.
 *
 * @par Support System:
 * linux liteos.
 */
void osal_dump_stack(void);

/**
 * @ingroup osal_debug
 * @brief Kernel panic function.
 *
 * @par Description:
 * If the condition is true, the system panics.
 *
 * @par Support System:
 * nonos.
 */
void osal_bug_on(unsigned char condition);

/**
 * @ingroup osal_debug
 * @brief Kernel dcache flush function.
 *
 * @par Description:
 * Flush cpu dcache
 *
 * @par Support System:
 * liteos.
 */
void osal_flush_cache(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* __OSAL_DEBUG_H__ */
