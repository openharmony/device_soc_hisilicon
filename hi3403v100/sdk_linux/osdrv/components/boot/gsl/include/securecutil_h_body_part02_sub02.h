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

#ifndef HI3403V100_SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_INCLUDE_SECURECUTIL_H_BODY_PART02_SUB02_H_
#define HI3403V100_SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_INCLUDE_SECURECUTIL_H_BODY_PART02_SUB02_H_

} SecStrBuf24;
typedef struct {
    unsigned char buf[25]; /* Performance optimization code structure assignment length 25 bytes */
} SecStrBuf25;
typedef struct {
    unsigned char buf[26]; /* Performance optimization code structure assignment length 26 bytes */
} SecStrBuf26;
typedef struct {
    unsigned char buf[27]; /* Performance optimization code structure assignment length 27 bytes */
} SecStrBuf27;
typedef struct {
    unsigned char buf[28]; /* Performance optimization code structure assignment length 28 bytes */
} SecStrBuf28;
typedef struct {
    unsigned char buf[29]; /* Performance optimization code structure assignment length 29 bytes */
} SecStrBuf29;
typedef struct {
    unsigned char buf[30]; /* Performance optimization code structure assignment length 30 bytes */
} SecStrBuf30;
typedef struct {
    unsigned char buf[31]; /* Performance optimization code structure assignment length 31 bytes */
} SecStrBuf31;
typedef struct {
    unsigned char buf[32]; /* Performance optimization code structure assignment length 32 bytes */
} SecStrBuf32;
typedef struct {
    unsigned char buf[33]; /* Performance optimization code structure assignment length 33 bytes */
} SecStrBuf33;
typedef struct {
    unsigned char buf[34]; /* Performance optimization code structure assignment length 34 bytes */
} SecStrBuf34;
typedef struct {
    unsigned char buf[35]; /* Performance optimization code structure assignment length 35 bytes */
} SecStrBuf35;
typedef struct {
    unsigned char buf[36]; /* Performance optimization code structure assignment length 36 bytes */
} SecStrBuf36;
typedef struct {
    unsigned char buf[37]; /* Performance optimization code structure assignment length 37 bytes */
} SecStrBuf37;
typedef struct {
    unsigned char buf[38]; /* Performance optimization code structure assignment length 38 bytes */
} SecStrBuf38;
typedef struct {
    unsigned char buf[39]; /* Performance optimization code structure assignment length 39 bytes */
} SecStrBuf39;
typedef struct {
    unsigned char buf[40]; /* Performance optimization code structure assignment length 40 bytes */
} SecStrBuf40;
typedef struct {
    unsigned char buf[41]; /* Performance optimization code structure assignment length 41 bytes */
} SecStrBuf41;
typedef struct {
    unsigned char buf[42]; /* Performance optimization code structure assignment length 42 bytes */
} SecStrBuf42;
typedef struct {
    unsigned char buf[43]; /* Performance optimization code structure assignment length 43 bytes */
} SecStrBuf43;
typedef struct {
    unsigned char buf[44]; /* Performance optimization code structure assignment length 44 bytes */
} SecStrBuf44;
typedef struct {
    unsigned char buf[45]; /* Performance optimization code structure assignment length 45 bytes */
} SecStrBuf45;
typedef struct {
    unsigned char buf[46]; /* Performance optimization code structure assignment length 46 bytes */
} SecStrBuf46;
typedef struct {
    unsigned char buf[47]; /* Performance optimization code structure assignment length 47 bytes */
} SecStrBuf47;
typedef struct {
    unsigned char buf[48]; /* Performance optimization code structure assignment length 48 bytes */
} SecStrBuf48;
typedef struct {
    unsigned char buf[49]; /* Performance optimization code structure assignment length 49 bytes */
} SecStrBuf49;
typedef struct {
    unsigned char buf[50]; /* Performance optimization code structure assignment length 50 bytes */
} SecStrBuf50;
typedef struct {
    unsigned char buf[51]; /* Performance optimization code structure assignment length 51 bytes */
} SecStrBuf51;
typedef struct {
    unsigned char buf[52]; /* Performance optimization code structure assignment length 52 bytes */
} SecStrBuf52;
typedef struct {
    unsigned char buf[53]; /* Performance optimization code structure assignment length 53 bytes */
} SecStrBuf53;
typedef struct {
    unsigned char buf[54]; /* Performance optimization code structure assignment length 54 bytes */
} SecStrBuf54;
typedef struct {
    unsigned char buf[55]; /* Performance optimization code structure assignment length 55 bytes */
} SecStrBuf55;
typedef struct {
    unsigned char buf[56]; /* Performance optimization code structure assignment length 56 bytes */
} SecStrBuf56;
typedef struct {
    unsigned char buf[57]; /* Performance optimization code structure assignment length 57 bytes */
} SecStrBuf57;
typedef struct {
    unsigned char buf[58]; /* Performance optimization code structure assignment length 58 bytes */
} SecStrBuf58;
typedef struct {
    unsigned char buf[59]; /* Performance optimization code structure assignment length 59 bytes */
} SecStrBuf59;
typedef struct {
    unsigned char buf[60]; /* Performance optimization code structure assignment length 60 bytes */
} SecStrBuf60;
typedef struct {
    unsigned char buf[61]; /* Performance optimization code structure assignment length 61 bytes */
} SecStrBuf61;
typedef struct {
    unsigned char buf[62]; /* Performance optimization code structure assignment length 62 bytes */
} SecStrBuf62;
typedef struct {
    unsigned char buf[63]; /* Performance optimization code structure assignment length 63 bytes */
} SecStrBuf63;
typedef struct {
    unsigned char buf[64]; /* Performance optimization code structure assignment length 64 bytes */
} SecStrBuf64;

/*
 * User can change the error handler by modify the following definition,
 * such as logging the detail error in file.
 */
#if defined(_DEBUG) || defined(DEBUG)
#if defined(SECUREC_ERROR_HANDLER_BY_ASSERT)
#define SECUREC_ERROR_INVALID_PARAMTER(msg) assert(msg "invalid argument" == NULL)
#define SECUREC_ERROR_INVALID_RANGE(msg)    assert(msg "invalid dest buffer size" == NULL)
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)   assert(msg "buffer overlap" == NULL)
#elif defined(SECUREC_ERROR_HANDLER_BY_PRINTF)
#if SECUREC_IN_KERNEL
#define SECUREC_ERROR_INVALID_PARAMTER(msg) printk("%s invalid argument\n", msg)
#define SECUREC_ERROR_INVALID_RANGE(msg)    printk("%s invalid dest buffer size\n", msg)
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)   printk("%s buffer overlap\n", msg)
#else
#define SECUREC_ERROR_INVALID_PARAMTER(msg) printf("%s invalid argument\n", msg)
#define SECUREC_ERROR_INVALID_RANGE(msg)    printf("%s invalid dest buffer size\n", msg)
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)   printf("%s buffer overlap\n", msg)
#endif
#elif defined(SECUREC_ERROR_HANDLER_BY_FILE_LOG)
#define SECUREC_ERROR_INVALID_PARAMTER(msg) LogSecureCRuntimeError(msg " EINVAL\n")
#define SECUREC_ERROR_INVALID_RANGE(msg)    LogSecureCRuntimeError(msg " ERANGE\n")
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)   LogSecureCRuntimeError(msg " EOVERLAP\n")
#endif
#endif

/* Default handler is none */
#ifndef SECUREC_ERROR_INVALID_PARAMTER
#define SECUREC_ERROR_INVALID_PARAMTER(msg)
#endif
#ifndef SECUREC_ERROR_INVALID_RANGE
#define SECUREC_ERROR_INVALID_RANGE(msg)
#endif
#ifndef SECUREC_ERROR_BUFFER_OVERLAP
#define SECUREC_ERROR_BUFFER_OVERLAP(msg)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/* Assembly language memory copy and memory set for X86 or MIPS ... */
#ifdef SECUREC_USE_ASM
void *memcpy_opt(void *dest, const void *src, size_t n);
void *memset_opt(void *s, int c, size_t n);
#endif

#if defined(SECUREC_ERROR_HANDLER_BY_FILE_LOG)
void LogSecureCRuntimeError(const char *errDetail);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* HI3403V100_SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_INCLUDE_SECURECUTIL_H_BODY_PART02_SUB02_H_ */
