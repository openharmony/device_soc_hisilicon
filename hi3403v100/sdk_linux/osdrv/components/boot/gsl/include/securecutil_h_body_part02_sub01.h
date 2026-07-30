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

#ifndef HI3403V100_SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_INCLUDE_SECURECUTIL_H_BODY_PART02_SUB01_H_
#define HI3403V100_SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_INCLUDE_SECURECUTIL_H_BODY_PART02_SUB01_H_


#ifndef ERANGE_AND_RESET
/* Once the error is detected, the dest buffer must be reseted! */
#define ERANGE_AND_RESET  (34 | 128)
#endif

/* Success */
#ifndef EOK
#define EOK 0
#endif

/* Define the max length of the string */
#ifndef SECUREC_STRING_MAX_LEN
#define SECUREC_STRING_MAX_LEN 0x7fffffffUL
#endif

#ifndef EINVAL_AND_RESET
/* Once the error is detected, the dest buffer must be reseted! */
#define EINVAL_AND_RESET (22 | 128)
#endif

#ifndef ERANGE
/* The destination buffer is not long enough and destination buffer can not be reset */
#define ERANGE 34
#endif

#ifndef EINVAL
/* The src buffer is not correct and destination buffer cant not be reset */
#define EINVAL 22
#endif

#ifndef EOVERLAP_AND_RESET
/* Once the buffer overlap is detected, the dest buffer must be reseted! */
#define EOVERLAP_AND_RESET (54 | 128)
#endif

#ifndef SECUREC_MEM_MAX_LEN
#define SECUREC_MEM_MAX_LEN 0x7fffffffUL
#endif

/* Add SECUREC_MEM_MAX_LEN for memcpy and memmove */

/* SECUREC_PCLINT for tool do not recognize __builtin_expect, just for pclint */
#if defined(__GNUC__) && \
    ((__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3))) && \
!defined(SECUREC_PCLINT)
/*
 * This is a built-in function that can be used without a declaration, if you encounter an undeclared compilation alarm,
 * you can add -DSECUREC_NEED_BUILTIN_EXPECT_DECLARE to complier options
 */
#define SECUREC_LIKELY(x) __builtin_expect(!!(x), 1)
#define SECUREC_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define SECUREC_LIKELY(x) (x)
#define SECUREC_UNLIKELY(x) (x)
#endif

#if (defined(_MSC_VER)) && (_MSC_VER >= 1400)
/* Shield compilation alerts using discarded functions and Constant  expression to maximize code compatibility */
#define SECUREC_MASK_MSVC_CRT_WARNING __pragma(warning(push)) \
    __pragma(warning(disable : 4996 4127))
#define SECUREC_END_MASK_MSVC_CRT_WARNING  __pragma(warning(pop))
#else
#define SECUREC_MASK_MSVC_CRT_WARNING
#define SECUREC_END_MASK_MSVC_CRT_WARNING
#endif
#define SECUREC_WHILE_ZERO SECUREC_MASK_MSVC_CRT_WARNING while (0) SECUREC_END_MASK_MSVC_CRT_WARNING

/* Automatically identify the platform that supports strnlen function, and use this function to improve performance */
#ifndef SECUREC_HAVE_STRNLEN
#if (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)
#if SECUREC_IN_KERNEL
#define SECUREC_HAVE_STRNLEN 0
#else
#if defined(__GLIBC__) && __GLIBC__ >= 2 && defined(__GLIBC_MINOR__) && __GLIBC_MINOR__ >= 10
#define SECUREC_HAVE_STRNLEN 1
#else
#define SECUREC_HAVE_STRNLEN 0
#endif
#endif
#else
#define SECUREC_HAVE_STRNLEN 0
#endif
#endif

#if SECUREC_IN_KERNEL
/* In kernel disbale functions */
#ifndef SECUREC_ENABLE_SCANF_FILE
#define SECUREC_ENABLE_SCANF_FILE 0
#endif
#ifndef SECUREC_ENABLE_SCANF_FLOAT
#define SECUREC_ENABLE_SCANF_FLOAT 0
#endif
#ifndef SECUREC_ENABLE_SPRINTF_FLOAT
#define SECUREC_ENABLE_SPRINTF_FLOAT 0
#endif
#ifndef SECUREC_HAVE_MBTOWC
#define SECUREC_HAVE_MBTOWC 0
#endif
#ifndef SECUREC_HAVE_WCTOMB
#define SECUREC_HAVE_WCTOMB 0
#endif
#ifndef SECUREC_HAVE_WCHART
#define SECUREC_HAVE_WCHART 0
#endif
#else /* Not in kernel */
/* Systems that do not support file, can define this macro to 0. */
#ifndef SECUREC_ENABLE_SCANF_FILE
#define SECUREC_ENABLE_SCANF_FILE 1
#endif
#ifndef SECUREC_ENABLE_SCANF_FLOAT
#define SECUREC_ENABLE_SCANF_FLOAT 1
#endif
/* Systems that do not support float, can define this macro to 0. */
#ifndef SECUREC_ENABLE_SPRINTF_FLOAT
#define SECUREC_ENABLE_SPRINTF_FLOAT 1
#endif
#ifndef SECUREC_HAVE_MBTOWC
#define SECUREC_HAVE_MBTOWC 1
#endif
#ifndef SECUREC_HAVE_WCTOMB
#define SECUREC_HAVE_WCTOMB 1
#endif
#ifndef SECUREC_HAVE_WCHART
#define SECUREC_HAVE_WCHART 1
#endif
#endif

#ifndef SECUREC_ENABLE_INLINE
#define SECUREC_ENABLE_INLINE 0
#endif

#ifndef SECUREC_NOT_CALL_LIBC_CORE_API
#define SECUREC_NOT_CALL_LIBC_CORE_API 1
#endif

#ifndef SECUREC_INLINE
#if SECUREC_ENABLE_INLINE
#define SECUREC_INLINE static inline
#else
#define SECUREC_INLINE static
#endif
#endif

#ifndef SECUREC_WARP_OUTPUT
#if SECUREC_IN_KERNEL
#define SECUREC_WARP_OUTPUT 1
#else
#define SECUREC_WARP_OUTPUT 0
#endif
#endif

#ifndef SECUREC_STREAM_STDIN
#define SECUREC_STREAM_STDIN stdin
#endif

#define SECUREC_MUL_SIXTEEN(x)              ((x) << 4U)
#define SECUREC_MUL_EIGHT(x)                ((x) << 3U)
#define SECUREC_MUL_TEN(x)                  ((((x) << 2U) + (x)) << 1U)
/* Limited format input and output width */
#define SECUREC_MAX_WIDTH_LEN_DIV_TEN       21474836
#define SECUREC_MAX_WIDTH_LEN               (SECUREC_MAX_WIDTH_LEN_DIV_TEN * 10)
/* Is the x multiplied by 10 greater than */
#define SECUREC_MUL_TEN_ADD_BEYOND_MAX(x)   (((x) > SECUREC_MAX_WIDTH_LEN_DIV_TEN))

#define SECUREC_FLOAT_BUFSIZE               (309 + 40)  /* Max length of double value */
#define SECUREC_FLOAT_BUFSIZE_LB            (4932 + 40) /* Max length of long double value */
#define SECUREC_FLOAT_DEFAULT_PRECISION     6

/* This macro does not handle pointer equality or integer overflow */
#define SECUREC_MEMORY_NO_OVERLAP(dest, src, count) \
    (((src) < (dest) && ((const char *)(src) + (count)) <= (char *)(dest)) || \
     ((dest) < (src) && ((char *)(dest) + (count)) <= (const char *)(src)))

#define SECUREC_MEMORY_IS_OVERLAP(dest, src, count) \
    (((src) < (dest) && ((const char *)(src) + (count)) > (char *)(dest)) || \
     ((dest) < (src) && ((char *)(dest) + (count)) > (const char *)(src)))

/*
 * Check whether the strings overlap, len is the length of the string not include terminator
 * Length is related to data type char or wchar , do not force conversion of types
 */
#define SECUREC_STRING_NO_OVERLAP(dest, src, len) \
    (((src) < (dest) && ((src) + (len)) < (dest)) || \
     ((dest) < (src) && ((dest) + (len)) < (src)))

/*
 * Check whether the strings overlap for strcpy wcscpy function, dest len and src Len are not include terminator
 * Length is related to data type char or wchar , do not force conversion of types
 */
#define SECUREC_STRING_IS_OVERLAP(dest, src, len) \
    (((src) < (dest) && ((src) + (len)) >= (dest)) || \
     ((dest) < (src) && ((dest) + (len)) >= (src)))

/*
 * Check whether the strings overlap for strcat wcscat function, dest len and src Len are not include terminator
 * Length is related to data type char or wchar , do not force conversion of types
 */
#define SECUREC_CAT_STRING_IS_OVERLAP(dest, destLen, src, srcLen) \
    (((dest) < (src) && ((dest) + (destLen) + (srcLen)) >= (src)) || \
     ((src) < (dest) && ((src) + (srcLen)) >= (dest)))

#if SECUREC_HAVE_STRNLEN
#define SECUREC_CALC_STR_LEN(str, maxLen, outLen) do { \
    *(outLen) = strnlen((str), (maxLen)); \
} SECUREC_WHILE_ZERO
SECUREC_INLINE size_t SecCalcStrLenOpt(const char *str, size_t maxLen)
{
    size_t len;
    SECUREC_CALC_STR_LEN(str, maxLen, &len);
    return len;
}
#define SECUREC_CALC_STR_LEN_OPT(str, maxLen, outLen) do { \
    *(outLen) = SecCalcStrLenOpt((str), (maxLen)); \
} SECUREC_WHILE_ZERO
#else
#define SECUREC_CALC_STR_LEN(str, maxLen, outLen) do { \
    const char *strEnd_ = (const char *)(str); \
    size_t availableSize_ = (size_t)(maxLen); \
    while (availableSize_ > 0 && *strEnd_ != '\0') { \
        --availableSize_; \
        ++strEnd_; \
    } \
    *(outLen) = (size_t)(strEnd_ - (str)); \
} SECUREC_WHILE_ZERO
#define SECUREC_CALC_STR_LEN_OPT SECUREC_CALC_STR_LEN
#endif

#define SECUREC_CALC_WSTR_LEN(str, maxLen, outLen) do { \
    const wchar_t *strEnd_ = (const wchar_t *)(str); \
    size_t len_ = 0; \
    while (len_ < (maxLen) && *strEnd_ != L'\0') { \
        ++len_; \
        ++strEnd_; \
    } \
    *(outLen) = len_; \
} SECUREC_WHILE_ZERO

/* Performance optimization, product may disable inline function */
SECUREC_INLINE void *SecPrimitiveMemSet(void *dest, int c, size_t count)
{
    unsigned char *p = (unsigned char *)dest;
    while (count > 0) {
        *p++ = (unsigned char)c;
        --count;
    }
    return dest;
}

SECUREC_INLINE void *SecPrimitiveMemCopy(void *dest, const void *src, size_t count)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    while (count > 0) {
        *d++ = *s++;
        --count;
    }
    return dest;
}

#ifdef SECUREC_USE_ASM
#define SECUREC_MEMSET_FUNC_OPT  memset_opt
#define SECUREC_MEMCPY_FUNC_OPT  memcpy_opt
#else
#define SECUREC_MEMSET_FUNC_OPT  SecPrimitiveMemSet
#define SECUREC_MEMCPY_FUNC_OPT  SecPrimitiveMemCopy
#endif
#define SECUREC_MEMCPY_WARP_OPT(dest, src, count)    (void)SECUREC_MEMCPY_FUNC_OPT((dest), (src), (count))

#ifndef SECUREC_MEMSET_BARRIER
#if defined(__GNUC__)
#define SECUREC_MEMSET_BARRIER 1
#else
#define SECUREC_MEMSET_BARRIER 0
#endif
#endif

#ifndef SECUREC_MEMSET_INDIRECT_USE
#define SECUREC_MEMSET_INDIRECT_USE 1
#endif
#if SECUREC_MEMSET_BARRIER
#define SECUREC_MEMORY_BARRIER(dest) __asm__ __volatile__("": : "r"(dest) : "memory")
#else
#define SECUREC_MEMORY_BARRIER(dest)
#endif
#if SECUREC_MEMSET_BARRIER
#define SECUREC_MEMSET_PREVENT_DSE(dest, value, count)  do { \
        (void)SECUREC_MEMSET_FUNC_OPT(dest, value, count); \
        SECUREC_MEMORY_BARRIER(dest); \
} SECUREC_WHILE_ZERO
#elif SECUREC_MEMSET_INDIRECT_USE
#define SECUREC_MEMSET_PREVENT_DSE(dest, value, count)  do { \
    void *(* const volatile fn_)(void *s_, int c_, size_t n_) = SECUREC_MEMSET_FUNC_OPT; \
    (void)(*fn_)((dest), (value), (count)); \
} SECUREC_WHILE_ZERO
#else
#define SECUREC_MEMSET_PREVENT_DSE(dest, value, count)  (void)SECUREC_MEMSET_FUNC_OPT((dest), (value), (count))
#endif

#ifdef SECUREC_FORMAT_OUTPUT_INPUT
#if defined(SECUREC_COMPATIBLE_WIN_FORMAT) || defined(__ARMCC_VERSION)
typedef __int64 SecInt64;
typedef unsigned __int64 SecUnsignedInt64;
#if defined(__ARMCC_VERSION)
typedef unsigned int SecUnsignedInt32;
#else
typedef unsigned __int32 SecUnsignedInt32;
#endif
#else
typedef unsigned int SecUnsignedInt32;
typedef long long SecInt64;
typedef unsigned long long SecUnsignedInt64;
#endif

#ifdef SECUREC_FOR_WCHAR
#if defined(SECUREC_VXWORKS_PLATFORM) && !defined(__WINT_TYPE__)
typedef wchar_t wint_t;
#endif
#ifndef WEOF
#define WEOF ((wchar_t)(-1))
#endif
#define SECUREC_CHAR(x) L ## x
typedef wchar_t SecChar;
typedef wchar_t SecUnsignedChar;
typedef wint_t SecInt;
typedef wint_t SecUnsignedInt;
#else /*  no SECUREC_FOR_WCHAR */
#define SECUREC_CHAR(x) (x)
typedef char SecChar;
typedef unsigned char SecUnsignedChar;
typedef int SecInt;
typedef unsigned int SecUnsignedInt;
#endif
#endif

/*
 * Determine whether the address is 8-byte aligned
 * Some systems do not have uintptr_t type, so  use NULL to clear tool alarm 507
 */
#define SECUREC_ADDR_ALIGNED_8(addr) ((((size_t)(addr)) & 7U) == 0) /* Use 7 to check aligned 8 */

/*
 * If you define the memory allocation function, you need to define the function prototype.
 * You can define this macro as a header file.
 */
#if defined(SECUREC_MALLOC_PROTOTYPE)
SECUREC_MALLOC_PROTOTYPE
#endif

#ifndef SECUREC_MALLOC
#define SECUREC_MALLOC(x) gsl_malloc((size_t)(x))
#endif

#ifndef SECUREC_FREE
#define SECUREC_FREE(x)   gsl_free((void *)(x))
#endif

/* Struct for performance */
#define SECUREC_COPY_VALUE_BY_STRUCT(dest, src, n) do { \
    *(SecStrBuf##n *)(void *)(dest) = *(const SecStrBuf##n *)(const void *)(src); \
} SECUREC_WHILE_ZERO

typedef struct {
    unsigned char buf[2]; /* Performance optimization code structure assignment length 2 bytes */
} SecStrBuf2;
typedef struct {
    unsigned char buf[3]; /* Performance optimization code structure assignment length 3 bytes */
} SecStrBuf3;
typedef struct {
    unsigned char buf[4]; /* Performance optimization code structure assignment length 4 bytes */
} SecStrBuf4;
typedef struct {
    unsigned char buf[5]; /* Performance optimization code structure assignment length 5 bytes */
} SecStrBuf5;
typedef struct {
    unsigned char buf[6]; /* Performance optimization code structure assignment length 6 bytes */
} SecStrBuf6;
typedef struct {
    unsigned char buf[7]; /* Performance optimization code structure assignment length 7 bytes */
} SecStrBuf7;
typedef struct {
    unsigned char buf[8]; /* Performance optimization code structure assignment length 8 bytes */
} SecStrBuf8;
typedef struct {
    unsigned char buf[9]; /* Performance optimization code structure assignment length 9 bytes */
} SecStrBuf9;
typedef struct {
    unsigned char buf[10]; /* Performance optimization code structure assignment length 10 bytes */
} SecStrBuf10;
typedef struct {
    unsigned char buf[11]; /* Performance optimization code structure assignment length 11 bytes */
} SecStrBuf11;
typedef struct {
    unsigned char buf[12]; /* Performance optimization code structure assignment length 12 bytes */
} SecStrBuf12;
typedef struct {
    unsigned char buf[13]; /* Performance optimization code structure assignment length 13 bytes */
} SecStrBuf13;
typedef struct {
    unsigned char buf[14]; /* Performance optimization code structure assignment length 14 bytes */
} SecStrBuf14;
typedef struct {
    unsigned char buf[15]; /* Performance optimization code structure assignment length 15 bytes */
} SecStrBuf15;
typedef struct {
    unsigned char buf[16]; /* Performance optimization code structure assignment length 16 bytes */
} SecStrBuf16;
typedef struct {
    unsigned char buf[17]; /* Performance optimization code structure assignment length 17 bytes */
} SecStrBuf17;
typedef struct {
    unsigned char buf[18]; /* Performance optimization code structure assignment length 18 bytes */
} SecStrBuf18;
typedef struct {
    unsigned char buf[19]; /* Performance optimization code structure assignment length 19 bytes */
} SecStrBuf19;
typedef struct {
    unsigned char buf[20]; /* Performance optimization code structure assignment length 20 bytes */
} SecStrBuf20;
typedef struct {
    unsigned char buf[21]; /* Performance optimization code structure assignment length 21 bytes */
} SecStrBuf21;
typedef struct {
    unsigned char buf[22]; /* Performance optimization code structure assignment length 22 bytes */
} SecStrBuf22;
typedef struct {
    unsigned char buf[23]; /* Performance optimization code structure assignment length 23 bytes */
} SecStrBuf23;
typedef struct {
    unsigned char buf[24]; /* Performance optimization code structure assignment length 24 bytes */


#endif /* HI3403V100_SDK_LINUX_OSDRV_COMPONENTS_BOOT_GSL_INCLUDE_SECURECUTIL_H_BODY_PART02_SUB01_H_ */
