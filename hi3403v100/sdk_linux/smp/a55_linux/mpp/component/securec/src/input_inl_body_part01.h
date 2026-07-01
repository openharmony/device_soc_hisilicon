/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART01_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART01_H_


#if SECUREC_IN_KERNEL
#if !defined(SECUREC_CTYPE_MACRO_ADAPT)
#include <linux/ctype.h>
#endif
#else
#if !defined(SECUREC_SYSAPI4VXWORKS) && !defined(SECUREC_CTYPE_MACRO_ADAPT)
#include <ctype.h>
#include <errno.h>
#ifdef SECUREC_FOR_WCHAR
#include <wctype.h>             /* For iswspace */
#endif
#endif
#endif

#ifndef EOF
#define EOF  (-1)
#endif

#define SECUREC_NUM_WIDTH_SHORT                 0
#define SECUREC_NUM_WIDTH_INT                   1
#define SECUREC_NUM_WIDTH_LONG                  2
#define SECUREC_NUM_WIDTH_LONG_LONG             3 /* Also long double */

#define SECUREC_BUFFERED_BLOK_SIZE              1024U

#if defined(SECUREC_VXWORKS_PLATFORM) && !defined(va_copy) && !defined(SECUREC_VA_COPY)
/* The name is the same as system macro. */
#define SECUREC_VA_COPY(dest, src) do { \
    size_t destSize_ = (size_t)sizeof(dest); \
    size_t srcSize_ = (size_t)sizeof(src); \
    if (destSize_ != srcSize_) { \
        SECUREC_MEMCPY_WARP_OPT((dest), (src), sizeof(va_list)); \
    } else { \
        SECUREC_MEMCPY_WARP_OPT(&(dest), &(src), sizeof(va_list)); \
    } \
} SECUREC_WHILE_ZERO
#endif

#define SECUREC_MULTI_BYTE_MAX_LEN              6

/* Compatibility macro name cannot be modifie */
#ifndef UNALIGNED
#if !(defined(_M_IA64)) && !(defined(_M_AMD64))
#define UNALIGNED
#else
#define UNALIGNED __unaligned
#endif
#endif

#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
/* Max 64bit value is 0xffffffffffffffff */
#define SECUREC_MAX_64BITS_VALUE                18446744073709551615ULL
#define SECUREC_MAX_64BITS_VALUE_DIV_TEN        1844674407370955161ULL
#define SECUREC_MAX_64BITS_VALUE_CUT_LAST_DIGIT 18446744073709551610ULL
#define SECUREC_MIN_64BITS_NEG_VALUE            9223372036854775808ULL
#define SECUREC_MAX_64BITS_POS_VALUE            9223372036854775807ULL
#define SECUREC_MIN_32BITS_NEG_VALUE            2147483648UL
#define SECUREC_MAX_32BITS_POS_VALUE            2147483647UL
#define SECUREC_MAX_32BITS_VALUE                4294967295UL
#define SECUREC_MAX_32BITS_VALUE_INC            4294967296UL
#define SECUREC_MAX_32BITS_VALUE_DIV_TEN        429496729UL
#define SECUREC_LONG_BIT_NUM                    ((unsigned int)(sizeof(long) << 3U))
/* Use ULL to clean up cl6x compilation alerts */
#define SECUREC_MAX_LONG_POS_VALUE              ((unsigned long)(1ULL << (SECUREC_LONG_BIT_NUM - 1)) - 1)
#define SECUREC_MIN_LONG_NEG_VALUE              ((unsigned long)(1ULL << (SECUREC_LONG_BIT_NUM - 1)))

/* Covert to long long to clean up cl6x compilation alerts */
#define SECUREC_LONG_HEX_BEYOND_MAX(number)     (((unsigned long long)(number) >> (SECUREC_LONG_BIT_NUM - 4U)) > 0)
#define SECUREC_LONG_OCTAL_BEYOND_MAX(number)   (((unsigned long long)(number) >> (SECUREC_LONG_BIT_NUM - 3U)) > 0)

#define SECUREC_QWORD_HEX_BEYOND_MAX(number)    (((number) >> (64U - 4U)) > 0)
#define SECUREC_QWORD_OCTAL_BEYOND_MAX(number)  (((number) >> (64U - 3U)) > 0)

#define SECUREC_LP64_BIT_WIDTH                  64
#define SECUREC_LP32_BIT_WIDTH                  32

#define SECUREC_CONVERT_IS_SIGNED(conv) ((conv) == 'd' || (conv) == 'i')
#endif

#define SECUREC_BRACE                           '{'     /* [ to { */
#define SECUREC_FILED_WIDTH_ENOUGH(spec) ((spec)->widthSet == 0 || (spec)->width > 0)
#define SECUREC_FILED_WIDTH_DEC(spec) do { \
    if ((spec)->widthSet != 0) { \
        --(spec)->width; \
    } \
} SECUREC_WHILE_ZERO

#ifdef SECUREC_FOR_WCHAR
/* Bits for all wchar, size is 65536/8, only supports wide characters with a maximum length of two bytes */
#define SECUREC_BRACKET_TABLE_SIZE    8192
#define SECUREC_EOF WEOF
#define SECUREC_MB_LEN 16       /* Max. # bytes in multibyte char  ,see MB_LEN_MAX */
#else
/* Bits for all char, size is 256/8 */
#define SECUREC_BRACKET_TABLE_SIZE    32
#define SECUREC_EOF EOF
#endif

#if SECUREC_HAVE_WCHART
#define SECUREC_ARRAY_WIDTH_IS_WRONG(spec) ((spec).arrayWidth == 0 || \
    ((spec).isWCharOrLong <= 0 && (spec).arrayWidth > SECUREC_STRING_MAX_LEN) || \
    ((spec).isWCharOrLong > 0 && (spec).arrayWidth > SECUREC_WCHAR_STRING_MAX_LEN))
#else
#define SECUREC_ARRAY_WIDTH_IS_WRONG(spec) ((spec).arrayWidth == 0 || (spec).arrayWidth > SECUREC_STRING_MAX_LEN)
#endif

#ifdef SECUREC_ON_64BITS
/* Use 0xffffffffUL mask to pass integer as array length */
#define SECUREC_GET_ARRAYWIDTH(argList) (((size_t)va_arg((argList), size_t)) & 0xffffffffUL)
#else /* !SECUREC_ON_64BITS */
#define SECUREC_GET_ARRAYWIDTH(argList) ((size_t)va_arg((argList), size_t))
#endif

typedef struct {
#ifdef SECUREC_FOR_WCHAR
    unsigned char *table; /* Default NULL */
#else
    unsigned char table[SECUREC_BRACKET_TABLE_SIZE]; /* Array length is large enough in application scenarios */
#endif
    unsigned char mask; /* Default 0 */
} SecBracketTable;

#ifdef SECUREC_FOR_WCHAR
#define SECUREC_INIT_BRACKET_TABLE { NULL, 0 }
#else
#define SECUREC_INIT_BRACKET_TABLE { {0}, 0 }
#endif

#if SECUREC_ENABLE_SCANF_FLOAT
typedef struct {
    size_t floatStrTotalLen;       /* Initialization must be length of buffer in charater */
    size_t floatStrUsedLen;        /* Store float string len */
    SecChar *floatStr;             /* Initialization must point to buffer */
    SecChar *allocatedFloatStr;    /* Initialization must be NULL  to store alloced point */
    SecChar buffer[SECUREC_FLOAT_BUFSIZE + 1];
} SecFloatSpec;
#endif

#define SECUREC_NUMBER_STATE_DEFAULT 0U
#define SECUREC_NUMBER_STATE_STARTED 1U

typedef struct {
    SecInt ch;            /* Char read from input */
    int charCount;        /* Number of characters processed */
    void *argPtr;         /* Variable parameter pointer, point to the end of the string */
    size_t arrayWidth;    /* Length of pointer Variable parameter, in charaters */
    SecUnsignedInt64 number64; /* Store input number64 value */
    unsigned long number; /* Store input number32 value */
    int numberWidth;      /* 0 = SHORT, 1 = int, > 1  long or L_DOUBLE */
    int numberArgType;       /* 1 for 64-bit integer, 0 otherwise. use it as decode function index */
    unsigned int negative;         /* 0 is positive */
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    unsigned int beyondMax;        /* Non-zero means beyond */
#endif
    unsigned int numberState; /* Identifies whether to start processing numbers, 1 is can input number */
    int width;            /* Width number in format */
    int widthSet;         /* 0 is not set width in format */
    int convChr;          /* Lowercase format conversion characters */
    int oriConvChr;       /* Store  original format conversion, convChr may change when parsing integers */
    signed char isWCharOrLong;  /* -1/0 not wchar or long, 1 for wchar or long */
    unsigned char suppress;     /* 0 is not have %* in format */
} SecScanSpec;

#ifdef SECUREC_FOR_WCHAR
#define SECUREC_GETC fgetwc
#define SECUREC_UN_GETC ungetwc
/* Only supports wide characters with a maximum length of two bytes in format string */
#define SECUREC_BRACKET_CHAR_MASK 0xffffU
#else
#define SECUREC_GETC fgetc
#define SECUREC_UN_GETC ungetc
#define SECUREC_BRACKET_CHAR_MASK 0xffU
#endif

#define SECUREC_CHAR_SIZE ((unsigned int)(sizeof(SecChar)))
/* To avoid 648, mask high bit: 0x00ffffff 0x0000ffff or 0x00000000 */
#define SECUREC_CHAR_MASK_HIGH (((((((((unsigned int)(-1) >> SECUREC_CHAR_SIZE) >> SECUREC_CHAR_SIZE) >> \
    SECUREC_CHAR_SIZE) >> SECUREC_CHAR_SIZE) >> \
    SECUREC_CHAR_SIZE) >> SECUREC_CHAR_SIZE) >> \
    SECUREC_CHAR_SIZE) >> SECUREC_CHAR_SIZE)

/* For char is 0xff, wcahr_t is 0xffff or 0xffffffff. */
#define SECUREC_CHAR_MASK (~((((((((((unsigned int)(-1) & SECUREC_CHAR_MASK_HIGH) << \
    SECUREC_CHAR_SIZE) << SECUREC_CHAR_SIZE) << \
    SECUREC_CHAR_SIZE) << SECUREC_CHAR_SIZE) << \
    SECUREC_CHAR_SIZE) << SECUREC_CHAR_SIZE) << \
    SECUREC_CHAR_SIZE) << SECUREC_CHAR_SIZE))

/* According wchar_t has multiple bytes, so use sizeof */
#define SECUREC_GET_CHAR(stream, outCh) do { \
    if ((stream)->count >= sizeof(SecChar)) { \
        *(outCh) = (SecInt)(SECUREC_CHAR_MASK & \
            (unsigned int)(int)(*((const SecChar *)(const void *)(stream)->cur))); \
        (stream)->cur += sizeof(SecChar); \
        (stream)->count -= sizeof(SecChar); \
    } else { \
        *(outCh) = SECUREC_EOF; \
    } \
} SECUREC_WHILE_ZERO

#define SECUREC_UN_GET_CHAR(stream) do { \
    if ((stream)->cur > (stream)->base) { \
        (stream)->cur -= sizeof(SecChar); \
        (stream)->count += sizeof(SecChar); \
    } \
} SECUREC_WHILE_ZERO

/* Convert wchar_t to int and then to unsigned int  to keep data clearing warning */
#define SECUREC_TO_LOWERCASE(chr) ((int)((unsigned int)(int)(chr) | (unsigned int)('a' - 'A')))

/* Record a flag for each bit */
#define SECUREC_BRACKET_INDEX(x)  ((unsigned int)(x) >> 3U)
#define SECUREC_BRACKET_VALUE(x)  ((unsigned char)(1U << ((unsigned int)(x) & 7U)))
#if SECUREC_IN_KERNEL
#define SECUREC_CONVERT_IS_UNSIGNED(conv) ((conv) == 'x' || (conv) == 'o' || (conv) == 'u')
#endif

/*
 * Set char in %[xxx] into table, only supports wide characters with a maximum length of two bytes
 */
SECUREC_INLINE void SecBracketSetBit(unsigned char *table, SecUnsignedChar ch)
{
    unsigned int tableIndex = SECUREC_BRACKET_INDEX(((unsigned int)(int)ch & SECUREC_BRACKET_CHAR_MASK));
    unsigned int tableValue = SECUREC_BRACKET_VALUE(((unsigned int)(int)ch & SECUREC_BRACKET_CHAR_MASK));
    /* Do not use   |=   optimize this code, it will cause compiling warning */
    table[tableIndex] = (unsigned char)(table[tableIndex] | tableValue);
}

SECUREC_INLINE void SecBracketSetBitRange(unsigned char *table, SecUnsignedChar startCh, SecUnsignedChar endCh)
{
    SecUnsignedChar expCh;
    /* %[a-z] %[a-a] Format %[a-\xff] end is 0xFF, condition (expCh <= endChar) cause dead loop */
    for (expCh = startCh; expCh < endCh; ++expCh) {
        SecBracketSetBit(table, expCh);
    }
    SecBracketSetBit(table, endCh);
}
/*
 * Determine whether the expression can be satisfied
 */
SECUREC_INLINE int SecCanInputForBracket(int convChr, SecInt ch, const SecBracketTable *bracketTable)
{
    unsigned int tableIndex = SECUREC_BRACKET_INDEX(((unsigned int)(int)ch & SECUREC_BRACKET_CHAR_MASK));
    unsigned int tableValue = SECUREC_BRACKET_VALUE(((unsigned int)(int)ch & SECUREC_BRACKET_CHAR_MASK));
#ifdef SECUREC_FOR_WCHAR
    if (((unsigned int)(int)ch & (~(SECUREC_BRACKET_CHAR_MASK))) != 0) {
        /* The value of the wide character exceeds the size of two bytes */
        return 0;
    }
    return (int)(convChr == SECUREC_BRACE &&
        (((unsigned int)bracketTable->table[tableIndex] ^ (unsigned int)bracketTable->mask) & tableValue) != 0);
#else
    return (int)(convChr == SECUREC_BRACE &&
        (((unsigned int)bracketTable->table[tableIndex] ^ (unsigned int)bracketTable->mask) & tableValue) != 0);
#endif
}

/*
 * String input ends when blank character is encountered
 */
SECUREC_INLINE int SecCanInputString(int convChr, SecInt ch)
{
    return (int)(convChr == 's' &&
        (!(ch >= SECUREC_CHAR('\t') && ch <= SECUREC_CHAR('\r')) && ch != SECUREC_CHAR(' ')));
}

/*
 * Can input a character when format is %c
 */
SECUREC_INLINE int SecCanInputCharacter(int convChr)
{
    return (int)(convChr == 'c');
}

/*
 * Determine if it is a 64-bit pointer  function
 * Return 0 is not ,1 is 64bit pointer
 */
SECUREC_INLINE int SecNumberArgType(size_t sizeOfVoidStar)
{
    /* Point size is 4 or 8 , Under the 64 bit system, the value not 0 */
    /* To clear e778 */
    if ((sizeOfVoidStar & sizeof(SecInt64)) != 0) {
        return 1;
    }
    return 0;
}
SECUREC_INLINE int SecIsDigit(SecInt ch);
SECUREC_INLINE int SecIsXdigit(SecInt ch);
SECUREC_INLINE int SecIsSpace(SecInt ch);
SECUREC_INLINE SecInt SecSkipSpaceChar(SecFileStream *stream, int *counter);
SECUREC_INLINE SecInt SecGetChar(SecFileStream *stream, int *counter);
SECUREC_INLINE void SecUnGetChar(SecInt ch, SecFileStream *stream, int *counter);


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART01_H_ */
