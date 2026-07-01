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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART03_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART03_H_

#if SECUREC_IN_KERNEL
        case SECUREC_CHAR('Z'): /* fall-through */ /* FALLTHRU */
#endif
        case SECUREC_CHAR('z'):
#ifdef SECUREC_ON_64BITS
            spec->numberWidth = SECUREC_NUM_WIDTH_LONG_LONG;
            spec->numberArgType = 1;
#else
            spec->numberWidth = SECUREC_NUM_WIDTH_LONG;
#endif
            break;
        case SECUREC_CHAR('L'):    /* For long double */ /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('q'):
            spec->numberWidth = SECUREC_NUM_WIDTH_LONG_LONG;
            spec->numberArgType = 1;
            break;
        case SECUREC_CHAR('l'):
            SecDecodeScanQualifierL(format, spec);
            break;
        case SECUREC_CHAR('w'):
            spec->isWCharOrLong = 1;
            break;
        case SECUREC_CHAR('*'):
            spec->suppress = 1;
            break;
        case SECUREC_CHAR('I'):
            SecDecodeScanQualifierI(format, spec, finishFlag);
            break;
        default:
            *finishFlag = 1;
            break;
    }
}
/*
 * Decode width and qualifier in format
 */
SECUREC_INLINE int SecDecodeScanFlag(const SecUnsignedChar **format, SecScanSpec *spec)
{
    const SecUnsignedChar *fmt = *format;
    int finishFlag = 0;

    do {
        ++fmt; /*  First skip % , next  seek fmt */
        /* May %*6d , so put it inside the loop */
        if (SecDecodeScanWidth(&fmt, spec) != 0) {
            return -1;
        }
        SecDecodeScanQualifier(&fmt, spec, &finishFlag);
    } while (finishFlag == 0);
    *format = fmt;
    return 0;
}

/*
 * Judging whether a zeroing buffer is needed according to different formats
 */
SECUREC_INLINE int SecDecodeClearFormat(const SecUnsignedChar *format, int *convChr)
{
    const SecUnsignedChar *fmt = format;
    /* To lowercase */
    int ch = SECUREC_TO_LOWERCASE(*fmt);
    if (!(ch == 'c' || ch == 's' || ch == SECUREC_BRACE)) {
        return -1;     /* First argument is not a string type */
    }
    if (ch == SECUREC_BRACE) {
#if !(defined(SECUREC_COMPATIBLE_WIN_FORMAT))
        if (*fmt == SECUREC_CHAR('{')) {
            return -1;
        }
#endif
        ++fmt;
        if (*fmt == SECUREC_CHAR('^')) {
            ++fmt;
        }
        if (*fmt == SECUREC_CHAR(']')) {
            ++fmt;
        }
        while (*fmt != SECUREC_CHAR('\0') && *fmt != SECUREC_CHAR(']')) {
            ++fmt;
        }
        if (*fmt == SECUREC_CHAR('\0')) {
            return -1; /* Trunc'd format string */
        }
    }
    *convChr = ch;
    return 0;
}

/*
 * Add L'\0' for wchar string , add '\0' for char string
 */
SECUREC_INLINE void SecAddEndingZero(void *ptr, const SecScanSpec *spec)
{
    if (spec->suppress == 0) {
        *(char *)ptr = '\0';
#if SECUREC_HAVE_WCHART
        if (spec->isWCharOrLong > 0) {
            *(wchar_t UNALIGNED *)ptr = L'\0';
        }
#endif
    }
}

SECUREC_INLINE void SecDecodeClearArg(SecScanSpec *spec, va_list argList)
{
    va_list argListSave; /* Backup for argList value, this variable don't need initialized */
    (void)SECUREC_MEMSET_FUNC_OPT(&argListSave, 0, sizeof(va_list)); /* To clear e530 argListSave not initialized */
#if defined(va_copy)
    va_copy(argListSave, argList);
#elif defined(SECUREC_VA_COPY)        /* For vxworks */
    SECUREC_VA_COPY(argListSave, argList);
#else
    argListSave = argList;
#endif
    spec->argPtr = (void *)va_arg(argListSave, void *);
    /* Get the next argument, size of the array in characters */
    /* Use 0xffffffffUL mask to Support  pass integer as array length */
    spec->arrayWidth = ((size_t)(va_arg(argListSave, size_t))) & 0xffffffffUL;
    va_end(argListSave);
    /* To clear e438 last value assigned not used , the compiler will optimize this code */
    (void)argListSave;
}

#ifdef SECUREC_FOR_WCHAR
/*
 *  Clean up the first %s %c buffer to zero for wchar version
 */
void SecClearDestBufW(const wchar_t *buffer, const wchar_t *format, va_list argList)
#else
/*
 *  Clean up the first %s %c buffer to zero for char version
 */
void SecClearDestBuf(const char *buffer, const char *format, va_list argList)
#endif
{
    SecScanSpec spec;
    int convChr = 0;
    const SecUnsignedChar *fmt = (const SecUnsignedChar *)format;

    /* Find first % */
    while (*fmt != SECUREC_CHAR('\0') && *fmt != SECUREC_CHAR('%')) {
        ++fmt;
    }
    if (*fmt == SECUREC_CHAR('\0')) {
        return;
    }

    SecSetDefaultScanSpec(&spec);
    if (SecDecodeScanFlag(&fmt, &spec) != 0) {
        return;
    }

    /* Update wchar flag for %S %C */
    SecUpdateWcharFlagByType(*fmt, &spec);
    if (spec.suppress != 0) {
        return;
    }

    if (SecDecodeClearFormat(fmt, &convChr) != 0) {
        return;
    }

    if (*buffer != SECUREC_CHAR('\0') && convChr != 's') {
        /*
         * When buffer not empty just clear %s.
         * Example call sscanf by  argment of (" \n", "%s", s, sizeof(s))
         */
        return;
    }

    SecDecodeClearArg(&spec, argList);
    /* There is no need to judge the upper limit */
    if (spec.arrayWidth == 0 || spec.argPtr == NULL) {
        return;
    }
    /* Clear one char */
    SecAddEndingZero(spec.argPtr, &spec);
    return;
}

/*
 *  Assign number  to output buffer
 */
SECUREC_INLINE void SecAssignNumber(const SecScanSpec *spec)
{
    void *argPtr = spec->argPtr;
    if (spec->numberArgType != 0) {
#if defined(SECUREC_VXWORKS_PLATFORM)
#if defined(SECUREC_VXWORKS_PLATFORM_COMP)
        *(SecInt64 UNALIGNED *)argPtr = (SecInt64)(spec->number64);
#else
        /* Take number64 as unsigned number unsigned to int clear Compile warning */
        *(SecInt64 UNALIGNED *)argPtr = *(SecUnsignedInt64 *)(&(spec->number64));
#endif
#else
        /* Take number64 as unsigned number */
        *(SecInt64 UNALIGNED *)argPtr = (SecInt64)(spec->number64);
#endif
        return;
    }
    if (spec->numberWidth > SECUREC_NUM_WIDTH_INT) {
        /* Take number as unsigned number */
        *(long UNALIGNED *)argPtr = (long)(spec->number);
    } else if (spec->numberWidth == SECUREC_NUM_WIDTH_INT) {
        *(int UNALIGNED *)argPtr = (int)(spec->number);
    } else if (spec->numberWidth == SECUREC_NUM_WIDTH_SHORT) {
        /* Take number as unsigned number */
        *(short UNALIGNED *)argPtr = (short)(spec->number);
    } else {  /* < 0 for hh format modifier */
        /* Take number as unsigned number */
        *(char UNALIGNED *)argPtr = (char)(spec->number);
    }
}

#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
/*
 *  Judge the long bit width
 */
SECUREC_INLINE int SecIsLongBitEqual(int bitNum)
{
    return (int)((unsigned int)bitNum == SECUREC_LONG_BIT_NUM);
}
#endif

/*
 * Convert hexadecimal characters to decimal value
 */
SECUREC_INLINE int SecHexValueOfChar(SecInt ch)
{
    /* Use isdigt Causing tool false alarms */
    return (int)((ch >= '0' && ch <= '9') ? ((unsigned char)ch - '0') :
            ((((unsigned char)ch | (unsigned char)('a' - 'A')) - ('a')) + 10)); /* Adding 10 is to hex value */
}

/*
 * Parse decimal character to integer for 32bit .
 */
static void SecDecodeNumberDecimal(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    unsigned long decimalEdge = SECUREC_MAX_32BITS_VALUE_DIV_TEN;
#ifdef SECUREC_ON_64BITS
    if (SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH) != 0) {
        decimalEdge = (unsigned long)SECUREC_MAX_64BITS_VALUE_DIV_TEN;
    }
#endif
    if (spec->number > decimalEdge) {
        spec->beyondMax = 1;
    }
#endif
    spec->number = SECUREC_MUL_TEN(spec->number);
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->number == SECUREC_MUL_TEN(decimalEdge)) {
        /* This code is specially converted to unsigned long type for compatibility */
        SecUnsignedInt64 number64As = (unsigned long)SECUREC_MAX_64BITS_VALUE - spec->number;
        if (number64As < (SecUnsignedInt64)(SecUnsignedInt)spec->ch - (SecUnsignedInt)SECUREC_CHAR('0')) {
            spec->beyondMax = 1;
        }
    }
#endif
    spec->number += ((unsigned long)(SecUnsignedInt)spec->ch - (SecUnsignedInt)SECUREC_CHAR('0'));
}

/*
 * Parse Hex character to integer for 32bit .
 */
static void SecDecodeNumberHex(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (SECUREC_LONG_HEX_BEYOND_MAX(spec->number)) {
        spec->beyondMax = 1;
    }
#endif
    spec->number = SECUREC_MUL_SIXTEEN(spec->number);
    spec->number += (unsigned long)(unsigned int)SecHexValueOfChar(spec->ch);
}

/*
 * Parse Octal character to integer for 32bit .
 */
static void SecDecodeNumberOctal(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (SECUREC_LONG_OCTAL_BEYOND_MAX(spec->number)) {
        spec->beyondMax = 1;
    }
#endif
    spec->number = SECUREC_MUL_EIGHT(spec->number);
    spec->number += ((unsigned long)(SecUnsignedInt)spec->ch - (SecUnsignedInt)SECUREC_CHAR('0'));
}

#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
/* Compatible with integer negative values other than int */
SECUREC_INLINE void SecFinishNumberNegativeOther(SecScanSpec *spec)
{
    if (SECUREC_CONVERT_IS_SIGNED(spec->oriConvChr)) {
        if (spec->number > SECUREC_MIN_LONG_NEG_VALUE) {
            spec->number = SECUREC_MIN_LONG_NEG_VALUE;
        } else {
            spec->number = (unsigned long)(0U - spec->number); /* Wrap with unsigned long numbers */
        }
        if (spec->beyondMax != 0) {
            if (spec->numberWidth < SECUREC_NUM_WIDTH_INT) {
                spec->number = 0;
            }
            if (spec->numberWidth == SECUREC_NUM_WIDTH_LONG) {
                spec->number = SECUREC_MIN_LONG_NEG_VALUE;
            }
        }
    } else { /* For o, u, x, X, p */
        spec->number = (unsigned long)(0U - spec->number); /* Wrap with unsigned long numbers */
        if (spec->beyondMax != 0) {
            spec->number = (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
    }
}
/* Compatible processing of integer negative numbers */
SECUREC_INLINE void SecFinishNumberNegativeInt(SecScanSpec *spec)
{
    if (SECUREC_CONVERT_IS_SIGNED(spec->oriConvChr)) {
#ifdef SECUREC_ON_64BITS
        if (SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH) != 0) {
            if ((spec->number > SECUREC_MIN_64BITS_NEG_VALUE)) {
                spec->number = 0;
            } else {
                spec->number = (unsigned int)(0U - (unsigned int)spec->number); /* Wrap with unsigned int numbers */
            }
        }
#else
        if (SecIsLongBitEqual(SECUREC_LP32_BIT_WIDTH) != 0) {
            if ((spec->number > SECUREC_MIN_32BITS_NEG_VALUE)) {
                spec->number = SECUREC_MIN_32BITS_NEG_VALUE;
            } else {
                spec->number = (unsigned int)(0U - (unsigned int)spec->number); /* Wrap with unsigned int numbers */
            }
        }
#endif
        if (spec->beyondMax != 0) {
#ifdef SECUREC_ON_64BITS
            if (SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH) != 0) {
                spec->number = 0;
            }
#else
            if (SecIsLongBitEqual(SECUREC_LP32_BIT_WIDTH) != 0) {
                spec->number = SECUREC_MIN_32BITS_NEG_VALUE;
            }
#endif
        }
    } else {            /* For o, u, x, X ,p */
#ifdef SECUREC_ON_64BITS
        if (spec->number > SECUREC_MAX_32BITS_VALUE_INC) {
            spec->number = SECUREC_MAX_32BITS_VALUE;
        } else {
            spec->number = (unsigned int)(0U - (unsigned int)spec->number); /* Wrap with unsigned int numbers */
        }
#else
        spec->number = (unsigned int)(0U - (unsigned int)spec->number); /* Wrap with unsigned int numbers */
#endif
        if (spec->beyondMax != 0) {
            spec->number = (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
    }
}

/* Compatible with integer positive values other than int */
SECUREC_INLINE void SecFinishNumberPositiveOther(SecScanSpec *spec)
{
    if (SECUREC_CONVERT_IS_SIGNED(spec->oriConvChr)) {
        if (spec->number > SECUREC_MAX_LONG_POS_VALUE) {
            spec->number = SECUREC_MAX_LONG_POS_VALUE;
        }
        if ((spec->beyondMax != 0 && spec->numberWidth < SECUREC_NUM_WIDTH_INT)) {
            spec->number = (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
        if (spec->beyondMax != 0 && spec->numberWidth == SECUREC_NUM_WIDTH_LONG) {
            spec->number = SECUREC_MAX_LONG_POS_VALUE;
        }
    } else {
        if (spec->beyondMax != 0) {
            spec->number = (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
    }
}

/* Compatible processing of integer positive numbers */
SECUREC_INLINE void SecFinishNumberPositiveInt(SecScanSpec *spec)
{
    if (SECUREC_CONVERT_IS_SIGNED(spec->oriConvChr)) {
#ifdef SECUREC_ON_64BITS
        if (SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH) != 0) {
            if (spec->number > SECUREC_MAX_64BITS_POS_VALUE) {
                spec->number = (unsigned long)SECUREC_MAX_64BITS_VALUE;
            }
        }
        if (spec->beyondMax != 0 && SecIsLongBitEqual(SECUREC_LP64_BIT_WIDTH) != 0) {
            spec->number = (unsigned long)SECUREC_MAX_64BITS_VALUE;
        }
#else
        if (SecIsLongBitEqual(SECUREC_LP32_BIT_WIDTH) != 0) {
            if (spec->number > SECUREC_MAX_32BITS_POS_VALUE) {
                spec->number = SECUREC_MAX_32BITS_POS_VALUE;
            }
        }
        if (spec->beyondMax != 0 && SecIsLongBitEqual(SECUREC_LP32_BIT_WIDTH) != 0) {
            spec->number = SECUREC_MAX_32BITS_POS_VALUE;
        }
#endif
    } else {            /* For o,u,x,X,p */
        if (spec->beyondMax != 0) {
            spec->number = SECUREC_MAX_32BITS_VALUE;
        }
    }
}

#endif

/*
 * Parse decimal character to integer for 64bit .
 */
static void SecDecodeNumber64Decimal(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->number64 > SECUREC_MAX_64BITS_VALUE_DIV_TEN) {
        spec->beyondMax = 1;
    }
#endif
    spec->number64 = SECUREC_MUL_TEN(spec->number64);


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART03_H_ */
