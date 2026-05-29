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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART01_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART01_H_


#ifndef SECUREC_ENABLE_SPRINTF_LONG_DOUBLE
/* Some compilers do not support long double */
#define SECUREC_ENABLE_SPRINTF_LONG_DOUBLE 1
#endif

#define SECUREC_NULL_STRING_SIZE            8
#define SECUREC_STATE_TABLE_SIZE            337

#if defined(SECUREC_VXWORKS_VERSION_5_4) && !defined(SECUREC_ON_64BITS)
#define SECUREC_DIV_QUOTIENT_OCTAL(val64)     ((val64) >> 3ULL)
#define SECUREC_DIV_RESIDUE_OCTAL(val64)      ((val64) & 7ULL)

#define SECUREC_DIV_QUOTIENT_HEX(val64)       ((val64) >> 4ULL)
#define SECUREC_DIV_RESIDUE_HEX(val64)        ((val64) & 0xfULL)
#endif

#define SECUREC_RADIX_OCTAL                 8U
#define SECUREC_RADIX_DECIMAL               10U
#define SECUREC_RADIX_HEX                   16U
#define SECUREC_PREFIX_LEN                  2
/* Size include '+' and '\0' */
#define SECUREC_FLOAT_BUF_EXT               2

/* Sign extend or Zero-extend */
#define SECUREC_GET_LONG_FROM_ARG(attr) ((((attr).flags & SECUREC_FLAG_SIGNED) != 0) ? \
    (SecInt64)(long)va_arg(argList, long) : \
    (SecInt64)(unsigned long)va_arg(argList, long))

/* Sign extend or Zero-extend */
#define SECUREC_GET_CHAR_FROM_ARG(attr) ((((attr).flags & SECUREC_FLAG_SIGNED) != 0) ? \
    SecUpdateNegativeChar(&(attr), ((char)va_arg(argList, int))) : \
    (SecInt64)(unsigned char)va_arg(argList, int))

/* Sign extend or Zero-extend */
#define SECUREC_GET_SHORT_FROM_ARG(attr) ((((attr).flags & SECUREC_FLAG_SIGNED) != 0) ? \
    (SecInt64)(short)va_arg(argList, int) : \
    (SecInt64)(unsigned short)va_arg(argList, int))

/* Sign extend or Zero-extend */
#define SECUREC_GET_INT_FROM_ARG(attr) ((((attr).flags & SECUREC_FLAG_SIGNED) != 0) ? \
    (SecInt64)(int)va_arg(argList, int) : \
    (SecInt64)(unsigned int)va_arg(argList, int))

#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
/* Sign extend or Zero-extend. No suitable macros were found to handle the branch */
#define SECUREC_GET_SIZE_FROM_ARG(attr) ((((attr).flags & SECUREC_FLAG_SIGNED) != 0) ? \
    ((SecIsSameSize(sizeof(size_t), sizeof(long)) != 0) ? (SecInt64)(long)va_arg(argList, long) : \
    ((SecIsSameSize(sizeof(size_t), sizeof(long long)) != 0) ? (SecInt64)(long long)va_arg(argList, long long) : \
    (SecInt64)(int)va_arg(argList, int))) : \
    (SecInt64)(size_t)va_arg(argList, size_t))
#endif

/* Format output buffer pointer and available size */
typedef struct {
    int count;
    SecChar *cur;
} SecPrintfStream;

typedef union {
    /* Integer formatting refers to the end of the buffer, plus 1 to prevent tool alarms */
    char str[SECUREC_BUFFER_SIZE + 1];
#if SECUREC_HAVE_WCHART
    wchar_t wStr[SECUREC_WCHAR_BUFFER_SIZE]; /* Just for %lc */
#endif
} SecBuffer;

typedef union {
    char *str;                  /* Not a null terminated  string */
#if SECUREC_HAVE_WCHART
    wchar_t *wStr;
#endif
} SecFormatBuf;

typedef struct {
    const char *digits;                 /* Point to the hexadecimal subset */
    SecFormatBuf text;                  /* Point to formatted string */
    int textLen;                        /* Length of the text */
    int textIsWide;                     /* Flag for text is wide chars ; 0 is not wide char */
    unsigned int radix;                 /* Use for output number , default set to 10 */
    unsigned int flags;
    int fldWidth;
    int precision;
    int dynWidth;                       /* %*   1 width from variable parameter ;0 not */
    int dynPrecision;                   /* %.*  1 precision from variable parameter ;0 not */
    int padding;                        /* Padding len */
    int prefixLen;                      /* Length of prefix, 0 or 1 or 2 */
    SecChar prefix[SECUREC_PREFIX_LEN]; /* Prefix is  0 or 0x */
    SecBuffer buffer;
} SecFormatAttr;

#if SECUREC_ENABLE_SPRINTF_FLOAT
#ifdef SECUREC_STACK_SIZE_LESS_THAN_1K
#define SECUREC_FMT_STR_LEN                 8
#else
#define SECUREC_FMT_STR_LEN                 16
#endif
typedef struct {
    char buffer[SECUREC_FMT_STR_LEN];
    char *fmtStr;                     /* Initialization must point to buffer */
    char *allocatedFmtStr;            /* Initialization must be NULL  to store allocated point */
    char *floatBuffer;                /* Use heap memory if the SecFormatAttr.buffer is not enough */
    int bufferSize;                   /* The size of floatBuffer */
} SecFloatAdapt;
#endif

/* Use 20 to Align the data */
#define SECUREC_DIGITS_BUF_SIZE  20
/* The serial number of 'x' or 'X' is 16 */
#define SECUREC_NUMBER_OF_X  16
/* Some systems can not use pointers to point to string literals, but can use string arrays. */
/* For example, when handling code under uboot, there is a problem with the pointer */
static const char g_itoaUpperDigits[SECUREC_DIGITS_BUF_SIZE] = "0123456789ABCDEFX";
static const char g_itoaLowerDigits[SECUREC_DIGITS_BUF_SIZE] = "0123456789abcdefx";

#if SECUREC_ENABLE_SPRINTF_FLOAT
/* Call system sprintf to format float value */
SECUREC_INLINE int SecFormatFloat(char *strDest, size_t destMax, const char *format, ...)
{
    int ret;                    /* If initialization causes  e838 */
    va_list argList;

    va_start(argList, format);
    SECUREC_MASK_VSPRINTF_WARNING
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif
    ret = __builtin_vsnprintf(strDest, destMax, format, argList);
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    SECUREC_END_MASK_VSPRINTF_WARNING
    va_end(argList);
    (void)argList; /* To clear e438 last value assigned not used , the compiler will optimize this code */

    return ret;
}

#if defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && SECUREC_ENABLE_SPRINTF_LONG_DOUBLE
/* Out put long double value to dest */
SECUREC_INLINE void SecFormatLongDouble(SecFormatAttr *attr, const SecFloatAdapt *floatAdapt, long double ldValue)
{
    int fldWidth = (((attr->flags & SECUREC_FLAG_LEFT) != 0) ? (-attr->fldWidth) : attr->fldWidth);
    if (attr->dynWidth != 0 && attr->dynPrecision != 0) {
        attr->textLen = SecFormatFloat(attr->text.str, (size_t)floatAdapt->bufferSize,
            floatAdapt->fmtStr, fldWidth, attr->precision, ldValue);
    } else if (attr->dynWidth != 0) {
        attr->textLen = SecFormatFloat(attr->text.str, (size_t)floatAdapt->bufferSize,
            floatAdapt->fmtStr, fldWidth, ldValue);
    } else if (attr->dynPrecision != 0) {
        attr->textLen = SecFormatFloat(attr->text.str, (size_t)floatAdapt->bufferSize,
            floatAdapt->fmtStr, attr->precision, ldValue);
    } else {
        attr->textLen = SecFormatFloat(attr->text.str, (size_t)floatAdapt->bufferSize,
            floatAdapt->fmtStr, ldValue);
    }
    if (attr->textLen < 0 || attr->textLen >= floatAdapt->bufferSize) {
        attr->textLen = 0;
    }
}
#endif

/* Out put double value to dest */
SECUREC_INLINE void SecFormatDouble(SecFormatAttr *attr, const SecFloatAdapt *floatAdapt, double dValue)
{
    int fldWidth = (((attr->flags & SECUREC_FLAG_LEFT) != 0) ? (-attr->fldWidth) : attr->fldWidth);
    if (attr->dynWidth != 0 && attr->dynPrecision != 0) {
        attr->textLen = SecFormatFloat(attr->text.str, (size_t)floatAdapt->bufferSize,
            floatAdapt->fmtStr, fldWidth, attr->precision, dValue);
    } else if (attr->dynWidth != 0) {
        attr->textLen = SecFormatFloat(attr->text.str, (size_t)floatAdapt->bufferSize,
            floatAdapt->fmtStr, fldWidth, dValue);
    } else if (attr->dynPrecision != 0) {
        attr->textLen = SecFormatFloat(attr->text.str, (size_t)floatAdapt->bufferSize,
            floatAdapt->fmtStr, attr->precision, dValue);
    } else {
        attr->textLen = SecFormatFloat(attr->text.str, (size_t)floatAdapt->bufferSize,
            floatAdapt->fmtStr, dValue);
    }
    if (attr->textLen < 0 || attr->textLen >= floatAdapt->bufferSize) {
        attr->textLen = 0;
    }
}
#endif

#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
/* To clear e506 warning */
SECUREC_INLINE int SecIsSameSize(size_t sizeA, size_t sizeB)
{
    return (int)(sizeA == sizeB);
}
#endif

#ifndef SECUREC_ON_64BITS
/*
 * Compiler Optimized Division 8.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber32ToOctalString(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    SecUnsignedInt32 val32 = number;
    do {
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[val32 % SECUREC_RADIX_OCTAL];
        val32 /= SECUREC_RADIX_OCTAL;
    } while (val32 != 0);
}

#ifdef _AIX
/*
 * Compiler Optimized Division 10.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber32ToDecString(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    SecUnsignedInt32 val32 = number;
    do {
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[val32 % SECUREC_RADIX_DECIMAL];
        val32 /= SECUREC_RADIX_DECIMAL;
    } while (val32 != 0);
}
#endif
/*
 * Compiler Optimized Division 16.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber32ToHexString(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    SecUnsignedInt32 val32 = number;
    do {
        --attr->text.str;
        *(attr->text.str) = attr->digits[val32 % SECUREC_RADIX_HEX];
        val32 /= SECUREC_RADIX_HEX;
    } while (val32 != 0);
}

#ifndef _AIX
/* Use fast div 10 */
SECUREC_INLINE void SecNumber32ToDecStringFast(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    SecUnsignedInt32 val32 = number;
    do {
        SecUnsignedInt32 quotient;
        SecUnsignedInt32 remain;
        --attr->text.str;
        *(attr->text.str) = g_itoaLowerDigits[val32 % SECUREC_RADIX_DECIMAL];
        quotient = (val32 >> 1U) + (val32 >> 2U); /* Fast div  magic 2 */
        quotient = quotient + (quotient >> 4U); /* Fast div  magic 4 */
        quotient = quotient + (quotient >> 8U); /* Fast div  magic 8 */
        quotient = quotient + (quotient >> 16U); /* Fast div  magic 16 */
        quotient = quotient >> 3U; /* Fast div  magic 3 */
        remain = val32 - SECUREC_MUL_TEN(quotient);
        val32 = (remain > 9U) ? (quotient + 1U) : quotient; /* Fast div  magic 9 */
    } while (val32 != 0);
}
#endif

SECUREC_INLINE void SecNumber32ToString(SecUnsignedInt32 number, SecFormatAttr *attr)
{
    switch (attr->radix) {
        case SECUREC_RADIX_HEX:
            SecNumber32ToHexString(number, attr);
            break;
        case SECUREC_RADIX_OCTAL:
            SecNumber32ToOctalString(number, attr);
            break;
        case SECUREC_RADIX_DECIMAL:
#ifdef _AIX
            /* The compiler will optimize div 10 */
            SecNumber32ToDecString(number, attr);
#else
            SecNumber32ToDecStringFast(number, attr);
#endif
            break;
        default:
            /* Do nothing */
            break;
    }
}
#endif

#if defined(SECUREC_USE_SPECIAL_DIV64) || (defined(SECUREC_VXWORKS_VERSION_5_4) && !defined(SECUREC_ON_64BITS))
/*
 * This function just to clear warning, on sume vxworks compiler shift 32 bit make warnings
 */
SECUREC_INLINE SecUnsignedInt64 SecU64Shr32(SecUnsignedInt64 number)
{
    return (((number) >> 16U) >> 16U); /* Two shifts of 16 bits to realize shifts of 32 bits */
}
/*
 * Fast divide by 10 algorithm.
 * Calculation divisor multiply  0xcccccccccccccccdULL, resultHi64 >> 3 as quotient
 */
SECUREC_INLINE void SecU64Div10(SecUnsignedInt64 divisor, SecUnsignedInt64 *quotient, SecUnsignedInt32 *residue)
{
    SecUnsignedInt64 mask = 0xffffffffULL; /* Use 0xffffffffULL as 32 bit mask */
    SecUnsignedInt64 magicHi = 0xccccccccULL; /* Fast divide 10 magic numbers high 32bit 0xccccccccULL */
    SecUnsignedInt64 magicLow = 0xcccccccdULL; /* Fast divide 10 magic numbers low 32bit  0xcccccccdULL */
    SecUnsignedInt64 divisorHi = (SecUnsignedInt64)(SecU64Shr32(divisor)); /* High 32 bit use  */
    SecUnsignedInt64 divisorLow = (SecUnsignedInt64)(divisor & mask); /* Low 32 bit mask */
    SecUnsignedInt64 factorHi = divisorHi * magicHi;
    SecUnsignedInt64 factorLow1 = divisorHi * magicLow;
    SecUnsignedInt64 factorLow2 = divisorLow * magicHi;
    SecUnsignedInt64 factorLow3 = divisorLow * magicLow;
    SecUnsignedInt64 carry = (factorLow1 & mask) + (factorLow2 & mask) + SecU64Shr32(factorLow3);
    SecUnsignedInt64 resultHi64 = factorHi + SecU64Shr32(factorLow1) + SecU64Shr32(factorLow2) + SecU64Shr32(carry);

    *quotient = resultHi64 >> 3U; /* Fast divide 10 magic numbers 3 */
    *residue = (SecUnsignedInt32)(divisor - ((*quotient) * 10)); /* Quotient mul 10 */
    return;
}
#if defined(SECUREC_VXWORKS_VERSION_5_4) && !defined(SECUREC_ON_64BITS)
/*
 * Divide function for VXWORKS
 */
SECUREC_INLINE int SecU64Div32(SecUnsignedInt64 divisor, SecUnsignedInt32 radix,
    SecUnsignedInt64 *quotient, SecUnsignedInt32 *residue)
{
    switch (radix) {
        case SECUREC_RADIX_DECIMAL:
            SecU64Div10(divisor, quotient, residue);
            break;
        case SECUREC_RADIX_HEX:
            *quotient = SECUREC_DIV_QUOTIENT_HEX(divisor);
            *residue = (SecUnsignedInt32)SECUREC_DIV_RESIDUE_HEX(divisor);
            break;
        case SECUREC_RADIX_OCTAL:
            *quotient = SECUREC_DIV_QUOTIENT_OCTAL(divisor);
            *residue = (SecUnsignedInt32)SECUREC_DIV_RESIDUE_OCTAL(divisor);
            break;
        default:
            return -1; /* This does not happen in the current file */
    }
    return 0;
}
SECUREC_INLINE void SecNumber64ToStringSpecial(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        SecUnsignedInt32 digit = 0; /* Ascii value of digit */
        SecUnsignedInt64 quotient = 0;
        if (SecU64Div32(val64, (SecUnsignedInt32)attr->radix, &quotient, &digit) != 0) {
            /* Just break, when enter this function, no error is returned */
            break;
        }
        --attr->text.str;
        *(attr->text.str) = attr->digits[digit];
        val64 = quotient;
    } while (val64 != 0);
}
#endif
#endif


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART01_H_ */
