/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 *set fileencoding
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __HI_LOG_H__
#define __HI_LOG_H__
#include <linux/version.h>
#include "hi_type.h"
#include "hi_module.h"

#define __HI_DEBUG_H__

#if defined(HI_LOG_SUPPORT) && (0 == HI_LOG_SUPPORT)
#undef HI_DEBUG

#define HI_PANIC(fmt...) \
    do {                 \
    } while (0)

#define HI_PRINT(fmt...) \
    do {                 \
    } while (0)
#else
#define HI_DEBUG

#ifdef __OS_LINUX__
#ifdef __KERNEL__
#include <linux/printk.h>
#define HI_PRINT printk
#else
#include <stdio.h>
#define HI_PRINT printf
#endif /* end of __KERNEL__ */
#else  /* __OS_ECOS__ */
#define HI_PRINT printf
#endif /* end of __OS_LINUX__ */

#ifdef __KERNEL__
#define HI_PANIC printk
#else
#define HI_PANIC printf
#endif

#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/* \addtogroup     HI_DEBUG */
/* @{ */ /* <!-- [HI_DEBUG] */

/* Default level of the output debugging information */
/* CNcomment: Ĭ?ϵĵ?????Ϣ???����? */
#define HI_LOG_LEVEL_DEFAULT HI_LOG_LEVEL_NOTICE

/* Level of the output debugging information */
/* CNcomment: ??????Ϣ???����? */
typedef enum HI_LOG_LEVEL_E {
    /* <fatal error. it indicates that a critical problem occurs in the system. */
    /* therefore, you must pay attention to it. */
    HI_LOG_LEVEL_FATAL = 0,
    /* <CNcomment: ????????, ??????????Ҫ?ر???ע??һ?????ִ???????????ϵͳ???????ش????? */
    /* <Major error. It indicates that a major problem occurs in the system and the system cannot run. */
    HI_LOG_LEVEL_ERROR = 1,
    /* <CNcomment: һ??????, һ?????ִ???????????ϵͳ?????˱Ƚϴ??????⣬?????????????? */
    /* <Warning. It indicates that a minor problem occurs in the system, but the system still can run properly. */
    HI_LOG_LEVEL_WARNING = 2,
    /* <CNcomment: ?澯??Ϣ, һ?????ִ?????Ϣ????ϵͳ???ܳ??????⣬???ǻ??ܼ??????? */
    /* <Notice. It indicates that a normal but significant condition in the system, */
    /* but the system still can run properly. */
    HI_LOG_LEVEL_NOTICE = 3,
    /* <CNcomment: ????????Ҫ????Ϣ, һ????????ϵͳ?Ĺؼ?·???????? */
    /* <Message. It is used to prompt users. Users can open the message when locating problems. */
    /* It is recommended to disable this message in general. */
    HI_LOG_LEVEL_INFO = 4,
    /* <CNcomment: ??ʾ??Ϣ, һ????Ϊ?????û??????????ڶ?λ??????ʱ?????Դ򿪣?һ???????½????ر? */
    /* <Debug. It is used to prompt developers. Developers can open the message when locating problems. */
    /* It is recommended to disable this message in general. */
    HI_LOG_LEVEL_DBG = 5,
    /* <CNcomment: ??ʾ??Ϣ, һ????Ϊ??????Ա???????????趨?Ĵ?ӡ??????һ???????½????ر? */
    /* <Verbose. It is used to prompt developers. Developers can open the message when locating problems. */
    /* It is recommended to disable this message in general. */
    HI_LOG_LEVEL_VERBOSE = 6,

    HI_LOG_LEVEL_BUTT
} HI_LOG_LEVEL_E;

/* Just only for fatal level print.   */ /* CNcomment: Ϊ?˴?ӡ??????Ϣ???ƶ??ĺ???ӡ???? */
#define HI_TRACE_LEVEL_FATAL (0)
/* Just only for error level print.   */ /* CNcomment: Ϊ?˴?ӡ??????Ϣ???ƶ??ĺ???ӡ???? */
#define HI_TRACE_LEVEL_ERROR (1)
/* Just only for warning level print. */ /* CNcomment: Ϊ?˴?ӡ??????Ϣ???ƶ??ĺ???ӡ???? */
#define HI_TRACE_LEVEL_WARN (2)
/* Just only for notice level print.  */ /* CNcomment: Ϊ?˴?ӡע????Ϣ???ƶ??ĺ???ӡ???? */
#define HI_TRACE_LEVEL_NOTICE (3)
/* Just only for info level print.    */ /* CNcomment: Ϊ?˴?ӡ??Ϣ???????ƶ??ĺ???ӡ???? */
#define HI_TRACE_LEVEL_INFO (4)
/* Just only for debug level print.   */ /* CNcomment: Ϊ?˴?ӡ??????Ϣ???ƶ??ĺ???ӡ???? */
#define HI_TRACE_LEVEL_DBG (5)
/* Just only for verbose level print. */ /* CNcomment: Ϊ?˴?ӡ?߳??ĵ?????Ϣ???ƶ??ĺ???ӡ???? */
#define HI_TRACE_LEVEL_VERBOSE (6)

/* Just only debug output,MUST BE NOT calling it. */
/* CNcomment: ??????????Ϣ?ӿڣ????Ƽ?ֱ?ӵ??ô˽ӿ? */
extern hi_void hi_log_out(hi_u32 level, HI_MOD_ID_E enModId,
                          hi_u8 *p_func_name, hi_u32 line_num, const char *format, ...);
extern hi_void hi_log_out(hi_u32 level, HI_MOD_ID_E enModId,
		          hi_u8 *p_func_name, hi_u32 line_num, const char *format, ...);
extern hi_u32 log_get_time_ms(void);

#ifdef HI_DEBUG

#ifndef LOG_D_MODULE_ID
#define LOG_D_MODULE_ID HI_ID_SYS
#endif

#ifndef HI_LOG_LEVEL
#define HI_LOG_LEVEL (HI_TRACE_LEVEL_DBG)
#endif

#ifdef __KERNEL__
#if defined(LINUX_VERSION_CODE) && (LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0))
#define HI_TRACE(level, module_id, fmt...)                                 \
    do {                                                                   \
        hi_log_out(level, module_id, (hi_u8 *)__FUNCTION__, __LINE__, fmt); \
    } while (0)
#else
#define HI_TRACE(level, module_id, fmt...)
#endif
#else
#define HI_TRACE(level, module_id, fmt...)                                 \
    do {                                                                   \
        hi_log_out(level, module_id, (hi_u8 *)__FUNCTION__, __LINE__, fmt); \
    } while (0)
#endif
#define HI_ASSERT(expr)                                                                                               \
    do {                                                                                                              \
        if (!(expr)) {                                                                                                \
            HI_PANIC("\nASSERT failed at:\n  >File name: %s\n  >Function : %s\n  >Line No. : %d\n  >Condition: %s\n", \
                __FILE__, __FUNCTION__, __LINE__, #expr);                                                             \
        }                                                                                                             \
    } while (0)

#define HI_ASSERT_RET(expr)                                    \
    do {                                                       \
        if (!(expr)) {                                         \
            HI_PRINT("\n<%s %d>: ASSERT Failure{" #expr "}\n", \
                __FUNCTION__, __LINE__);                       \
            return HI_FAILURE;                                 \
        }                                                      \
    } while (0)

/* Supported for debug output to serial/network/u-disk. */
/* CNcomment: ????ģ????Ҫ???????º???????????????Ϣ???????����??ڡ????ڡ?U?̴洢?? */
/* Just only reserve the fatal level output. */
/* CNcomment: ?????????????ĵ?????Ϣ */
#if (HI_LOG_LEVEL == HI_TRACE_LEVEL_FATAL)
#define HI_LOG_FATAL(fmt...)  HI_TRACE(HI_TRACE_LEVEL_FATAL, LOG_D_MODULE_ID, fmt)
#define HI_LOG_ERR(fmt...)
#define HI_LOG_WARN(fmt...)
#define HI_LOG_NOTICE(fmt...)
#define HI_LOG_INFO(fmt...)
#define HI_LOG_DBG(fmt...)
#define HI_LOG_VERBOSE(fmt...)
/* Just only reserve the fatal/error level output. */
/* CNcomment: ?????????????ĺʹ??󼶱??ĵ?????Ϣ */
#elif (HI_LOG_LEVEL == HI_TRACE_LEVEL_ERROR)
#define HI_LOG_FATAL(fmt...)  HI_TRACE(HI_TRACE_LEVEL_FATAL, LOG_D_MODULE_ID, fmt)
#define HI_LOG_ERR(fmt...)   HI_TRACE(HI_TRACE_LEVEL_ERROR, LOG_D_MODULE_ID, fmt)
#define HI_LOG_WARN(fmt...)
#define HI_LOG_NOTICE(fmt...)
#define HI_LOG_INFO(fmt...)
#define HI_LOG_DBG(fmt...)
#define HI_LOG_VERBOSE(fmt...)
/* Just only reserve the fatal/error/warning level output. */
/* CNcomment: ?????????????ġ??????ġ????漶???ĵ?????Ϣ */
#elif (HI_LOG_LEVEL == HI_TRACE_LEVEL_WARN)
#define HI_LOG_FATAL(fmt...)  HI_TRACE(HI_TRACE_LEVEL_FATAL, LOG_D_MODULE_ID, fmt)
#define HI_LOG_ERR(fmt...)   HI_TRACE(HI_TRACE_LEVEL_ERROR, LOG_D_MODULE_ID, fmt)
#define HI_LOG_WARN(fmt...)  HI_TRACE(HI_TRACE_LEVEL_WARN, LOG_D_MODULE_ID, fmt)
#define HI_LOG_NOTICE(fmt...)
#define HI_LOG_INFO(fmt...)
#define HI_LOG_DBG(fmt...)
#define HI_LOG_VERBOSE(fmt...)
/* Just only reserve the fatal/error/warning/note level output. */
/* CNcomment: ?????????????ġ??????ġ???????ע?⼶???ĵ?????Ϣ */
#elif (HI_LOG_LEVEL == HI_TRACE_LEVEL_NOTICE)
#define HI_LOG_FATAL(fmt...)  HI_TRACE(HI_TRACE_LEVEL_FATAL, LOG_D_MODULE_ID, fmt)
#define HI_LOG_ERR(fmt...)   HI_TRACE(HI_TRACE_LEVEL_ERROR, LOG_D_MODULE_ID, fmt)
#define HI_LOG_WARN(fmt...)  HI_TRACE(HI_TRACE_LEVEL_WARN, LOG_D_MODULE_ID, fmt)
#define HI_LOG_NOTICE(fmt...)  HI_TRACE(HI_TRACE_LEVEL_NOTICE, LOG_D_MODULE_ID, fmt)
#define HI_LOG_INFO(fmt...)
#define HI_LOG_DBG(fmt...)
#define HI_LOG_VERBOSE(fmt...)
/* Just only reserve the fatal/error/warning/info level output. */
/* CNcomment: ?????????????ġ??????ġ?????????Ϣ?????ĵ?????Ϣ */
#elif (HI_LOG_LEVEL == HI_TRACE_LEVEL_INFO)
#define HI_LOG_FATAL(fmt...)  HI_TRACE(HI_TRACE_LEVEL_FATAL, LOG_D_MODULE_ID, fmt)
#define HI_LOG_ERR(fmt...)   HI_TRACE(HI_TRACE_LEVEL_ERROR, LOG_D_MODULE_ID, fmt)
#define HI_LOG_WARN(fmt...)  HI_TRACE(HI_TRACE_LEVEL_WARN, LOG_D_MODULE_ID, fmt)
#define HI_LOG_NOTICE(fmt...)  HI_TRACE(HI_TRACE_LEVEL_NOTICE, LOG_D_MODULE_ID, fmt)
#define HI_LOG_INFO(fmt...)   HI_TRACE(HI_TRACE_LEVEL_INFO, LOG_D_MODULE_ID, fmt)
#define HI_LOG_DBG(fmt...)
#define HI_LOG_VERBOSE(fmt...)
#elif (HI_LOG_LEVEL == HI_TRACE_LEVEL_DBG)
/* Reserve all the levels output. */
/* CNcomment: ???????м?????????Ϣ */
#define HI_LOG_FATAL(fmt...)  HI_TRACE(HI_TRACE_LEVEL_FATAL, LOG_D_MODULE_ID, fmt)
#define HI_LOG_ERR(fmt...)   HI_TRACE(HI_TRACE_LEVEL_ERROR, LOG_D_MODULE_ID, fmt)
#define HI_LOG_WARN(fmt...)  HI_TRACE(HI_TRACE_LEVEL_WARN, LOG_D_MODULE_ID, fmt)
#define HI_LOG_NOTICE(fmt...)  HI_TRACE(HI_TRACE_LEVEL_NOTICE, LOG_D_MODULE_ID, fmt)
#define HI_LOG_INFO(fmt...)   HI_TRACE(HI_TRACE_LEVEL_INFO, LOG_D_MODULE_ID, fmt)
#define HI_LOG_DBG(fmt...)    HI_TRACE(HI_TRACE_LEVEL_DBG, LOG_D_MODULE_ID, fmt)
#define HI_LOG_VERBOSE(fmt...)
#elif (HI_LOG_LEVEL == HI_TRACE_LEVEL_VERBOSE)
/* Reserve all the levels output. */
/* CNcomment: ???????м?????????Ϣ */
#define HI_LOG_FATAL(fmt...)  HI_TRACE(HI_TRACE_LEVEL_FATAL, LOG_D_MODULE_ID, fmt)
#define HI_LOG_ERR(fmt...)   HI_TRACE(HI_TRACE_LEVEL_ERROR, LOG_D_MODULE_ID, fmt)
#define HI_LOG_WARN(fmt...)  HI_TRACE(HI_TRACE_LEVEL_WARN, LOG_D_MODULE_ID, fmt)
#define HI_LOG_NOTICE(fmt...)  HI_TRACE(HI_TRACE_LEVEL_NOTICE, LOG_D_MODULE_ID, fmt)
#define HI_LOG_INFO(fmt...)   HI_TRACE(HI_TRACE_LEVEL_INFO, LOG_D_MODULE_ID, fmt)
#define HI_LOG_DBG(fmt...)    HI_TRACE(HI_TRACE_LEVEL_DBG, LOG_D_MODULE_ID, fmt)
#define HI_LOG_VERBOSE(fmt...) HI_TRACE(HI_TRACE_LEVEL_VERBOSE, LOG_D_MODULE_ID, fmt)

#endif

/* Only for debug, forbidden to modify and upload */
/* CNcomment: ?����???ʹ?ã???ֹ?????ϴ? */
#ifdef HI_KEY_PRINT_OUT
#define HI_KEY_PRINT(module_id, fmt...)
#define HI_KEY_PRINT_HEX(module_id, str, addr, len) \
#else
#define HI_KEY_PRINT(module_id, fmt...)
#define HI_KEY_PRINT_HEX(module_id, str, addr, len)
#endif

#else
#define HI_LOG_FATAL(fmt...)
#define HI_LOG_ERR(fmt...)
#define HI_LOG_WARN(fmt...)
#define HI_LOG_NOTICE(fmt...)
#define HI_LOG_INFO(fmt...)
#define HI_LOG_DBG(fmt...)
#define HI_LOG_VERBOSE(fmt...)

#define HI_KEY_PRINT(module_id, fmt...)
#define HI_KEY_PRINT_HEX(module_id, str, addr, len)

#define HI_ASSERT(expr)
#define HI_ASSERT_RET(expr)
#endif /* endif HI_DEBUG */

#define HI_CHECK_SEC_FUNC(ret)              \
        do {                                                      \
            errno_t err_ret = (ret);                       \
            if (err_ret != EOK) {                          \
                HI_LOG_ERR("secure func call error\n"); \
            }                                                        \
        } while (0)

/* @} */ /* <!-- ==== Structure Definition End ==== */
#define hi_notice_func_enter() HI_LOG_NOTICE(" ===>[Enter]\n")
#define hi_notice_func_exit()  HI_LOG_NOTICE(" <===[Exit]\n")
#define hi_notice_func_trace() HI_LOG_NOTICE(" =TRACE=\n")

#define hi_info_func_enter() HI_LOG_INFO(" ===>[Enter]\n")
#define hi_info_func_exit()  HI_LOG_INFO(" <===[Exit]\n")
#define hi_info_func_trace() HI_LOG_INFO(" =TRACE=\n")

#define hi_dbg_func_enter() HI_LOG_DBG(" ===>[Enter]\n")
#define hi_dbg_func_exit()  HI_LOG_DBG(" <===[Exit]\n")
#define hi_dbg_func_trace() HI_LOG_DBG(" =TRACE=\n")

/* Function trace log for user input low frequency counts */
/* CNcomment: ???????뽵Ƶ??ӡ?Ĵ??? */
#define hi_dbg_func_trace_low_freq_cnt(freq)                                    \
    do {                                                                    \
        static unsigned int freqtimes = 0;                                  \
        if (0 == (freqtimes % (unsigned int)freq)) {                        \
            HI_LOG_DBG(" =TRACE Freq=%d,freqtimes=%d=\n", freq, freqtimes); \
        }                                                                   \
        freqtimes++;                                                        \
    } while (0)

/* function trace log for user add extra print log as need */
/* CNcomment: ?û????????????Ӷ????Ĵ?ӡ */
/* need use hi_func_trace_low_freq_cnt_end at the same time, to repesent the addtional prints is the end */
/* CNcomment: ??Ҫͬʱʹ??hi_func_trace_low_freq_cnt_end?????????????ӵĴ?ӡ??ֹ */
#define hi_func_trace_low_freq_cnt_begin(freq)            \
    do {                                             \
        static unsigned int freqtimes = 0;           \
        if (0 == (freqtimes % (unsigned int)freq)) { \
            do {                                     \
        } while (0)

#define hi_func_trace_low_freq_cnt_end() \
    }                               \
    freqtimes++;                    \
    }                               \
    while (0)

/* Function trace log for user input low frequency time */
/* CNcomment: ???????뽵Ƶ??ӡ??ʱ?? */
#define hi_dbg_func_trace_low_freq_time(time)                                     \
    do {                                                                      \
        static hi_u32 last_time = 0;                                        \
        hi_u32 curr_time;                                                   \
        curr_time = log_get_time_ms();                                         \
        if (time <= (curr_time - last_time)) {                            \
            HI_LOG_DBG(" =TRACE freq=%d,freqtimes=%d=\n", time, curr_time); \
            last_time = curr_time;                                        \
        }                                                                     \
    } while (0)

/* function trace log for user add extra print log as need */
/* CNcomment: ?û????????????Ӷ????Ĵ?ӡ */
/* need use hi_func_trace_low_freq_time_end at the same time, to repesent the addtional prints is the end */
/* CNcomment: ??Ҫͬʱʹ??hi_func_trace_low_freq_time_end?????????????ӵĴ?ӡ??ֹ */
#define hi_func_trace_low_freq_time_begin(time)         \
    do {                                           \
        static hi_u32 last_time = 0;             \
        hi_u32 curr_time;                        \
        curr_time = log_get_time_ms();              \
        if (time <= (curr_time - last_time)) { \
            do {                                   \
        } while (0)

#define hi_func_trace_low_freq_time_end() \
    last_time = curr_time;       \
    }                                \
    }                                \
    while (0)

/* function trace log, strictly prohibited to expand */
#define hi_err_print_err_code(err_code) HI_LOG_ERR("Error Code: [0x%08X]\n", err_code)

/* function trace log, print the called function name when function is error */
#define hi_err_print_call_fun_err(func, err_code) HI_LOG_ERR("Call %s Failed, Error Code: [0x%08X]\n", #func, err_code)

/* Function trace log, print the pointer name when pointer is null */
#define hi_err_print_null_pointer(val) HI_LOG_ERR("%s = %p,  Null Pointer!\n", #val, val)

/* used for displaying more detailed error information */
#define hi_err_print_s32(val)         HI_LOG_ERR("%s = %d\n", #val, val)
#define hi_err_print_u32(val)         HI_LOG_ERR("%s = %u\n", #val, val)
#define hi_err_print_s64(val)         HI_LOG_ERR("%s = %lld\n", #val, val)
#define hi_err_print_u64(val)         HI_LOG_ERR("%s = %llu\n", #val, val)
#define hi_err_print_h32(val)         HI_LOG_ERR("%s = 0x%08X\n", #val, val)
#define hi_err_print_h64(val)         HI_LOG_ERR("%s = 0x%016llX\n", #val, val)
#define hi_err_print_str(val)         HI_LOG_ERR("%s = %s\n", #val, val)
#define hi_err_print_void(val)        HI_LOG_ERR("%s = %p\n", #val, val)
#define hi_err_print_float(val)       HI_LOG_ERR("%s = %f\n", #val, val)
#define hi_err_print_info(val)        HI_LOG_ERR("<%s>\n", val)
#define hi_err_print_bool(val)        HI_LOG_ERR("%s = %s\n", #val, val ? "True" : "False")
#define hi_err_printstr(val, length) \
    do {                                                                                          \
        if (strlen(val) > length) {                                                               \
            HI_LOG_ERR("The length of the string is greater than the size of the buff itself\n"); \
        } else {                                                                                  \
            hi_err_print_str(val);                                                                 \
        }                                                                                         \
    } while (0)

/* used for displaying more detailed warning information */
#define hi_warn_print_s32(val)         HI_LOG_WARN("%s = %d\n", #val, val)
#define hi_warn_print_u32(val)         HI_LOG_WARN("%s = %u\n", #val, val)
#define hi_warn_print_s64(val)         HI_LOG_WARN("%s = %lld\n", #val, val)
#define hi_warn_print_u64(val)         HI_LOG_WARN("%s = %llu\n", #val, val)
#define hi_warn_print_h32(val)         HI_LOG_WARN("%s = 0x%08X\n", #val, val)
#define hi_warn_print_h64(val)         HI_LOG_WARN("%s = 0x%016llX\n", #val, val)
#define hi_warn_print_str(val)         HI_LOG_WARN("%s = %s\n", #val, val)
#define hi_warn_print_void(val)        HI_LOG_WARN("%s = %p\n", #val, val)
#define hi_warn_print_float(val)       HI_LOG_WARN("%s = %f\n", #val, val)
#define hi_warn_print_info(val)        HI_LOG_WARN("<%s>\n", val)
#define hi_warn_print_bool(val)        HI_LOG_WARN("%s = %s\n", #val, val ? "True" : "False")
#define hi_warn_printstr(val, length) \
    do {                                                                                          \
        if (strlen(val) > length) {                                                               \
            HI_LOG_ERR("The length of the string is greater than the size of the buff itself\n"); \
        } else {                                                                                  \
            hi_warn_print_str(val);                                                                \
        }                                                                                         \
    } while (0)

/* used for displaying normal but significant information */
#define hi_notice_print_s32(val)         HI_LOG_NOTICE("%s = %d\n", #val, val)
#define hi_notice_print_u32(val)         HI_LOG_NOTICE("%s = %u\n", #val, val)
#define hi_notice_print_s64(val)         HI_LOG_NOTICE("%s = %lld\n", #val, val)
#define hi_notice_print_u64(val)         HI_LOG_NOTICE("%s = %llu\n", #val, val)
#define hi_notice_print_h32(val)         HI_LOG_NOTICE("%s = 0x%08X\n", #val, val)
#define hi_notice_print_h64(val)         HI_LOG_NOTICE("%s = 0x%016llX\n", #val, val)
#define hi_notice_print_str(val)         HI_LOG_NOTICE("%s = %s\n", #val, val)
#define hi_notice_print_void(val)        HI_LOG_NOTICE("%s = %p\n", #val, val)
#define hi_notice_print_float(val)       HI_LOG_NOTICE("%s = %f\n", #val, val)
#define hi_notice_print_info(val)        HI_LOG_NOTICE("<%s>\n", val)
#define hi_notice_print_bool(val)        HI_LOG_NOTICE("%s = %s\n", #val, val ? "True" : "False")
#define hi_notice_printstr(val, length) \
    do {                                                                                          \
        if (strlen(val) > length) {                                                               \
            HI_LOG_ERR("The length of the string is greater than the size of the buff itself\n"); \
        } else {                                                                                  \
            hi_notice_print_str(val);                                                              \
        }                                                                                         \
    } while (0)

/* only used for key info, can be expanded as needed */
#define hi_info_print_s32(val)         HI_LOG_INFO("%s = %d\n", #val, val)
#define hi_info_print_u32(val)         HI_LOG_INFO("%s = %u\n", #val, val)
#define hi_info_print_s64(val)         HI_LOG_INFO("%s = %lld\n", #val, val)
#define hi_info_print_u64(val)         HI_LOG_INFO("%s = %llu\n", #val, val)
#define hi_info_print_h32(val)         HI_LOG_INFO("%s = 0x%08X\n", #val, val)
#define hi_info_print_h64(val)         HI_LOG_INFO("%s = 0x%016llX\n", #val, val)
#define hi_info_print_str(val)         HI_LOG_INFO("%s = %s\n", #val, val)
#define hi_info_print_void(val)        HI_LOG_INFO("%s = %p\n", #val, val)
#define hi_info_print_float(val)       HI_LOG_INFO("%s = %f\n", #val, val)
#define hi_info_print_info(val)        HI_LOG_INFO("<%s>\n", val)
#define hi_info_print_bool(val)        HI_LOG_INFO("%s = %s\n", #val, val ? "True" : "False")
#define hi_info_printstr(val, length) \
    do {                                                                                          \
        if (strlen(val) > length) {                                                               \
            HI_LOG_ERR("The length of the string is greater than the size of the buff itself\n"); \
        } else {                                                                                  \
            hi_info_print_str(val);                                                                \
        }                                                                                         \
    } while (0)

/** only used for self debug, can be expanded as needed */
#define hi_dbg_print_s32(val)         HI_LOG_DBG("%s = %d\n", #val, val)
#define hi_dbg_print_u32(val)         HI_LOG_DBG("%s = %u\n", #val, val)
#define hi_dbg_print_s64(val)         HI_LOG_DBG("%s = %lld\n", #val, val)
#define hi_dbg_print_u64(val)         HI_LOG_DBG("%s = %llu\n", #val, val)
#define hi_dbg_print_h32(val)         HI_LOG_DBG("%s = 0x%08X\n", #val, val)
#define hi_dbg_print_h64(val)         HI_LOG_DBG("%s = 0x%016llX\n", #val, val)
#define hi_dbg_print_str(val)         HI_LOG_DBG("%s = %s\n", #val, val)
#define hi_dbg_print_void(val)        HI_LOG_DBG("%s = %p\n", #val, val)
#define hi_dbg_print_float(val)       HI_LOG_DBG("%s = %f\n", #val, val)
#define hi_dbg_print_info(val)        HI_LOG_DBG("<%s>\n", val)
#define hi_dbg_print_bool(val)        HI_LOG_DBG("%s = %s\n", #val, val ? "True" : "False")
#define hi_dbg_printstr(val, length) \
    do {                                                                                          \
        if (strlen(val) > length) {                                                               \
            HI_LOG_ERR("The length of the string is greater than the size of the buff itself\n"); \
        } else {                                                                                  \
            hi_dbg_print_str(val);                                                                 \
        }                                                                                         \
    } while (0)

/** only used for self debug, can be expanded as needed */
#define hi_verbose_print_s32(val)         HI_LOG_VERBOSE("%s = %d\n", #val, val)
#define hi_verbose_print_u32(val)         HI_LOG_VERBOSE("%s = %u\n", #val, val)
#define hi_verbose_print_s64(val)         HI_LOG_VERBOSE("%s = %lld\n", #val, val)
#define hi_verbose_print_u64(val)         HI_LOG_VERBOSE("%s = %llu\n", #val, val)
#define hi_verbose_print_h32(val)         HI_LOG_VERBOSE("%s = 0x%08X\n", #val, val)
#define hi_verbose_print_h64(val)         HI_LOG_VERBOSE("%s = 0x%016llX\n", #val, val)
#define hi_verbose_print_str(val)         HI_LOG_VERBOSE("%s = %s\n", #val, val)
#define hi_verbose_print_void(val)        HI_LOG_VERBOSE("%s = %p\n", #val, val)
#define hi_verbose_print_float(val)       HI_LOG_VERBOSE("%s = %f\n", #val, val)
#define hi_verbose_print_info(val)        HI_LOG_VERBOSE("<%s>\n", val)
#define hi_verbose_print_bool(val)        HI_LOG_VERBOSE("%s = %s\n", #val, val ? "True" : "False")
#define hi_verbose_printstr(val, length) \
    do {                                                                                          \
        if (strlen(val) > length) {                                                               \
            HI_LOG_ERR("The length of the string is greater than the size of the buff itself\n"); \
        } else {                                                                                  \
            hi_verbose_print_str(val);                                                             \
        }                                                                                         \
    } while (0)

#define hi_log_print_block(pu8_datablock, length) \
    do { \
        hi_u32 u32ii = 0; \
        hi_char aci_out_str[64] = {0}; \
        hi_char *pci_buffer = (hi_char *)(pu8_datablock); \
        errno_t _sec_errno; \
        HI_PRINT(" %s\n", #pu8_datablock); \
        for (u32ii = 0; u32ii < (length); u32ii++) { \
            if ((u32ii % 16 == 0) && (u32ii != 0)) { \
                HI_PRINT("%s\n", aci_out_str); \
                _sec_errno = memset_s(aci_out_str, sizeof(aci_out_str), 0x00, sizeof(aci_out_str)); \
                if (_sec_errno != EOK) { \
                    HI_LOG_ERR("hi_log_print_block memset_s failed!\n"); \
                } \
            } \
            _sec_errno = snprintf_s(&aci_out_str[(u32ii % 16) * 3], (64 - (u32ii % 16) * 3), \
                                    4, " %02X", pci_buffer[u32ii]); \
            if (_sec_errno < 0) { \
                HI_LOG_ERR("hi_log_print_block snprintf_s failed!\n"); \
            } \
        } \
        if ((u32ii % 16 != 0) && (u32ii != 0)) { \
            HI_PRINT("%s\n", aci_out_str); \
            _sec_errno = memset_s(aci_out_str, sizeof(aci_out_str), 0x00, sizeof(aci_out_str)); \
            if (_sec_errno != EOK) { \
                HI_LOG_ERR("hi_log_print_block memset_s failed!\n"); \
            } \
        } \
    } while (0)

#define MKSTR(exp)        # exp
#define MKMARCOTOSTR(exp) MKSTR(exp)
#define VERSION_STRING    ("SDK_VERSION:[" MKMARCOTOSTR(SDK_VERSION) "] Build Time:[" __DATE__ ", " __TIME__ "]")

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_LOG_H__ */

