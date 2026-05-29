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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART02_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART02_H_

#if SECUREC_ENABLE_SCANF_FLOAT

/*
 * Convert a floating point string to a floating point number
 */
SECUREC_INLINE int SecAssignNarrowFloat(const char *floatStr, const SecScanSpec *spec)
{
    char *endPtr = NULL;
    double d;
#if SECUREC_SUPPORT_STRTOLD
    if (spec->numberWidth == SECUREC_NUM_WIDTH_LONG_LONG) {
        long double d2;
        errno = 0;
        d2 = strtold(floatStr, &endPtr);
        if (endPtr == floatStr || errno == ERANGE) {
            return -1;
        }
        *(long double UNALIGNED *)(spec->argPtr) = d2;
        return 0;
    }
#endif
    d = strtod(floatStr, &endPtr);
    /* cannot detect if endPtr points to the end of floatStr,because strtod handles only two characters for 1.E */
    if (endPtr == floatStr) {
        return -1;
    }
    if (spec->numberWidth > SECUREC_NUM_WIDTH_INT) {
        *(double UNALIGNED *)(spec->argPtr) = (double)d;
    } else {
        *(float UNALIGNED *)(spec->argPtr) = (float)d;
    }
    return 0;
}

#ifdef SECUREC_FOR_WCHAR
/*
 * Convert a floating point wchar string to a floating point number
 * Success  ret 0
 */
SECUREC_INLINE int SecAssignWideFloat(const SecFloatSpec *floatSpec, const SecScanSpec *spec)
{
    int retVal;
    /* Convert float string */
    size_t mbsLen;
    size_t tempFloatStrLen = (size_t)(floatSpec->floatStrUsedLen + 1) * sizeof(wchar_t);
    char *tempFloatStr = (char *)SECUREC_MALLOC(tempFloatStrLen);
    if (tempFloatStr == NULL) {
        return -1;
    }
    tempFloatStr[0] = '\0';
    SECUREC_MASK_MSVC_CRT_WARNING
    mbsLen = wcstombs(tempFloatStr, floatSpec->floatStr, tempFloatStrLen - 1);
    SECUREC_END_MASK_MSVC_CRT_WARNING
    /* This condition must satisfy mbsLen is not -1 */
    if (mbsLen >= tempFloatStrLen) {
        SECUREC_FREE(tempFloatStr);
        return -1;
    }
    tempFloatStr[mbsLen] = '\0';
    retVal = SecAssignNarrowFloat(tempFloatStr, spec);
    SECUREC_FREE(tempFloatStr);
    return retVal;
}
#endif

SECUREC_INLINE int SecAssignFloat(const SecFloatSpec *floatSpec, const SecScanSpec *spec)
{
#ifdef SECUREC_FOR_WCHAR
    return SecAssignWideFloat(floatSpec, spec);
#else
    return SecAssignNarrowFloat(floatSpec->floatStr, spec);
#endif
}

/*
 * Init SecFloatSpec before parse format
 */
SECUREC_INLINE void SecInitFloatSpec(SecFloatSpec *floatSpec)
{
    floatSpec->floatStr = floatSpec->buffer;
    floatSpec->allocatedFloatStr = NULL;
    floatSpec->floatStrTotalLen = sizeof(floatSpec->buffer) / sizeof(floatSpec->buffer[0]);
    floatSpec->floatStrUsedLen = 0;
}

SECUREC_INLINE void SecFreeFloatSpec(SecFloatSpec *floatSpec, int *doneCount)
{
    /* 2014.3.6 add, clear the stack data */
    if (memset_s(floatSpec->buffer, sizeof(floatSpec->buffer), 0, sizeof(floatSpec->buffer)) != EOK) {
        *doneCount = 0; /* This code just to meet the coding requirements */
    }
    /* The pFloatStr can be alloced in SecExtendFloatLen function, clear and free it */
    if (floatSpec->allocatedFloatStr != NULL) {
        size_t bufferSize = floatSpec->floatStrTotalLen * sizeof(SecChar);
        if (memset_s(floatSpec->allocatedFloatStr, bufferSize, 0, bufferSize) != EOK) {
            *doneCount = 0; /* This code just to meet the coding requirements */
        }
        SECUREC_FREE(floatSpec->allocatedFloatStr);
        floatSpec->allocatedFloatStr = NULL;
        floatSpec->floatStr = NULL;
    }
}

/*
 * Splice floating point string
 * Return 0 OK
 */
SECUREC_INLINE int SecExtendFloatLen(SecFloatSpec *floatSpec)
{
    if (floatSpec->floatStrUsedLen >= floatSpec->floatStrTotalLen) {
        /* Buffer size is len x sizeof(SecChar) */
        size_t oriSize = floatSpec->floatStrTotalLen * sizeof(SecChar);
        /* Add one character to clear tool warning */
        size_t nextSize = (oriSize * 2) + sizeof(SecChar); /* Multiply 2 to extend buffer size */

        /* Prevents integer overflow, the maximum length of SECUREC_MAX_WIDTH_LEN is enough */
        if (nextSize <= (size_t)SECUREC_MAX_WIDTH_LEN) {
            void *nextBuffer = (void *)SECUREC_MALLOC(nextSize);
            if (nextBuffer == NULL) {
                return -1;
            }
            if (memcpy_s(nextBuffer, nextSize, floatSpec->floatStr, oriSize) != EOK) {
                SECUREC_FREE(nextBuffer);   /* This is a dead code, just to meet the coding requirements */
                return -1;
            }
            /* Clear old buffer memory */
            if (memset_s(floatSpec->floatStr, oriSize, 0, oriSize) != EOK) {
                SECUREC_FREE(nextBuffer);   /* This is a dead code, just to meet the coding requirements */
                return -1;
            }
            /* Free old allocated buffer */
            if (floatSpec->allocatedFloatStr != NULL) {
                SECUREC_FREE(floatSpec->allocatedFloatStr);
            }
            floatSpec->allocatedFloatStr = (SecChar *)(nextBuffer);    /* Use to clear free on stack warning */
            floatSpec->floatStr = (SecChar *)(nextBuffer);
            floatSpec->floatStrTotalLen = nextSize / sizeof(SecChar); /* Get buffer total len in character */
            return 0;
        }
        return -1; /* Next size is beyond max */
    }
    return 0;
}

/* Do not use localeconv()->decimal_pointif  onlay support  '.' */
SECUREC_INLINE int SecIsFloatDecimal(SecChar ch)
{
    return (int)(ch == SECUREC_CHAR('.'));
}

SECUREC_INLINE int SecInputFloatSign(SecFileStream *stream, SecScanSpec *spec, SecFloatSpec *floatSpec)
{
    if (!SECUREC_FILED_WIDTH_ENOUGH(spec)) {
        return 0;
    }
    spec->ch = SecGetChar(stream, &(spec->charCount));
    if (spec->ch == SECUREC_CHAR('+') || spec->ch == SECUREC_CHAR('-')) {
        SECUREC_FILED_WIDTH_DEC(spec); /* Make sure the count after un get char is correct */
        if (spec->ch == SECUREC_CHAR('-')) {
            floatSpec->floatStr[floatSpec->floatStrUsedLen] = SECUREC_CHAR('-');
            ++floatSpec->floatStrUsedLen;
            if (SecExtendFloatLen(floatSpec) != 0) {
                return -1;
            }
        }
    } else {
        SecUnGetChar(spec->ch, stream, &(spec->charCount));
    }
    return 0;
}

SECUREC_INLINE int SecInputFloatDigit(SecFileStream *stream, SecScanSpec *spec, SecFloatSpec *floatSpec)
{
    /* Now get integral part */
    while (SECUREC_FILED_WIDTH_ENOUGH(spec)) {
        spec->ch = SecGetChar(stream, &(spec->charCount));
        if (SecIsDigit(spec->ch) == 0) {
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
            return 0;
        }
        SECUREC_FILED_WIDTH_DEC(spec); /* Must be behind un get char, otherwise the logic is incorrect */
        spec->numberState = SECUREC_NUMBER_STATE_STARTED;
        floatSpec->floatStr[floatSpec->floatStrUsedLen] = (SecChar)spec->ch;
        ++floatSpec->floatStrUsedLen;
        if (SecExtendFloatLen(floatSpec) != 0) {
            return -1;
        }
    }
    return 0;
}

/*
 * Scan value of exponent.
 * Return 0 OK
 */
SECUREC_INLINE int SecInputFloatE(SecFileStream *stream, SecScanSpec *spec, SecFloatSpec *floatSpec)
{
    if (SecInputFloatSign(stream, spec, floatSpec) == -1) {
        return -1;
    }
    if (SecInputFloatDigit(stream, spec, floatSpec) != 0) {
        return -1;
    }
    return 0;
}

SECUREC_INLINE int SecInputFloatFractional(SecFileStream *stream, SecScanSpec *spec, SecFloatSpec *floatSpec)
{
    if (SECUREC_FILED_WIDTH_ENOUGH(spec)) {
        spec->ch = SecGetChar(stream, &(spec->charCount));
        if (SecIsFloatDecimal((SecChar)spec->ch) == 0) {
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
            return 0;
        }
        SECUREC_FILED_WIDTH_DEC(spec); /* Must be behind un get char, otherwise the logic is incorrect */
        /* Now check for decimal */
        floatSpec->floatStr[floatSpec->floatStrUsedLen] = (SecChar)spec->ch;
        ++floatSpec->floatStrUsedLen;
        if (SecExtendFloatLen(floatSpec) != 0) {
            return -1;
        }
        if (SecInputFloatDigit(stream, spec, floatSpec) != 0) {
            return -1;
        }
    }
    return 0;
}

SECUREC_INLINE int SecInputFloatExponent(SecFileStream *stream, SecScanSpec *spec, SecFloatSpec *floatSpec)
{
    /* Now get exponent part */
    if (spec->numberState == SECUREC_NUMBER_STATE_STARTED && SECUREC_FILED_WIDTH_ENOUGH(spec)) {
        spec->ch = SecGetChar(stream, &(spec->charCount));
        if (spec->ch != SECUREC_CHAR('e') && spec->ch != SECUREC_CHAR('E')) {
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
            return 0;
        }
        SECUREC_FILED_WIDTH_DEC(spec); /* Must be behind un get char, otherwise the logic is incorrect */
        floatSpec->floatStr[floatSpec->floatStrUsedLen] = SECUREC_CHAR('e');
        ++floatSpec->floatStrUsedLen;
        if (SecExtendFloatLen(floatSpec) != 0) {
            return -1;
        }
        if (SecInputFloatE(stream, spec, floatSpec) != 0) {
            return -1;
        }
    }
    return 0;
}

/*
 * Scan %f.
 * Return 0 OK
 */
SECUREC_INLINE int SecInputFloat(SecFileStream *stream, SecScanSpec *spec, SecFloatSpec *floatSpec)
{
    floatSpec->floatStrUsedLen = 0;

    /* The following code sequence is strict */
    if (SecInputFloatSign(stream, spec, floatSpec) != 0) {
        return -1;
    }
    if (SecInputFloatDigit(stream, spec, floatSpec) != 0) {
        return -1;
    }
    if (SecInputFloatFractional(stream, spec, floatSpec) != 0) {
        return -1;
    }
    if (SecInputFloatExponent(stream, spec, floatSpec) != 0) {
        return -1;
    }

    /* Make sure  have a string terminator, buffer is large enough */
    floatSpec->floatStr[floatSpec->floatStrUsedLen] = SECUREC_CHAR('\0');
    if (spec->numberState == SECUREC_NUMBER_STATE_STARTED) {
        return 0;
    }
    return -1;
}
#endif

#if (!defined(SECUREC_FOR_WCHAR) && SECUREC_HAVE_WCHART && SECUREC_HAVE_MBTOWC) || \
    (!defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_VERSION))
/* only multi-bytes string need isleadbyte() function */
SECUREC_INLINE int SecIsLeadByte(SecInt ch)
{
    unsigned int c = (unsigned int)ch;
#if !(defined(_MSC_VER) || defined(_INC_WCTYPE))
    return (int)(c & 0x80U); /* Use bitwise operation to check if the most significant bit is 1 */
#else
    return (int)isleadbyte((int)(c & 0xffU)); /* Use bitwise operations to limit character values to valid ranges */
#endif
}
#endif

/*
 * Parsing whether it is a wide character
 */
SECUREC_INLINE void SecUpdateWcharFlagByType(SecUnsignedChar ch, SecScanSpec *spec)
{
    if (spec->isWCharOrLong != 0) {
        /* Wide character identifiers have been explicitly set by l or h flag */
        return;
    }

    /* Set default flag */
#if defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_WIN_FORMAT)
    spec->isWCharOrLong = 1;  /* On windows wide char version %c %s %[ is wide char */
#else
    spec->isWCharOrLong = -1; /* On linux all version %c %s %[ is multi char */
#endif

    if (ch == SECUREC_CHAR('C') || ch == SECUREC_CHAR('S')) {
#if defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_WIN_FORMAT)
        spec->isWCharOrLong = -1; /* On windows wide char version %C %S is multi char */
#else
        spec->isWCharOrLong = 1;  /* On linux all version %C %S is wide char */
#endif
    }

    return;
}
/*
 * Decode  %l %ll
 */
SECUREC_INLINE void SecDecodeScanQualifierL(const SecUnsignedChar **format, SecScanSpec *spec)
{
    const SecUnsignedChar *fmt = *format;
    if (*(fmt + 1) == SECUREC_CHAR('l')) {
        spec->numberArgType = 1;
        spec->numberWidth = SECUREC_NUM_WIDTH_LONG_LONG;
        ++fmt;
    } else {
        spec->numberWidth = SECUREC_NUM_WIDTH_LONG;
#if defined(SECUREC_ON_64BITS) && !(defined(SECUREC_COMPATIBLE_WIN_FORMAT))
        /* On window 64 system sizeof long is 32bit */
        spec->numberArgType = 1;
#endif
        spec->isWCharOrLong = 1;
    }
    *format = fmt;
}

/*
 * Decode  %I %I43 %I64 %Id %Ii %Io ...
 * Set finishFlag to  1  finish Flag
 */
SECUREC_INLINE void SecDecodeScanQualifierI(const SecUnsignedChar **format, SecScanSpec *spec, int *finishFlag)
{
    const SecUnsignedChar *fmt = *format;
    if ((*(fmt + 1) == SECUREC_CHAR('6')) &&
        (*(fmt + 2) == SECUREC_CHAR('4'))) { /* Offset 2 for I64 */
        spec->numberArgType = 1;
        *format = *format + 2; /* Add 2 to skip I64 point to '4' next loop will inc */
    } else if ((*(fmt + 1) == SECUREC_CHAR('3')) &&
                (*(fmt + 2) == SECUREC_CHAR('2'))) { /* Offset 2 for I32 */
        *format = *format + 2; /* Add 2 to skip I32 point to '2' next loop will inc */
    } else if ((*(fmt + 1) == SECUREC_CHAR('d')) ||
                (*(fmt + 1) == SECUREC_CHAR('i')) ||
                (*(fmt + 1) == SECUREC_CHAR('o')) ||
                (*(fmt + 1) == SECUREC_CHAR('x')) ||
                (*(fmt + 1) == SECUREC_CHAR('X'))) {
        spec->numberArgType = SecNumberArgType(sizeof(void *));
    } else {
        /* For %I */
        spec->numberArgType = SecNumberArgType(sizeof(void *));
        *finishFlag = 1;
    }
}

SECUREC_INLINE int SecDecodeScanWidth(const SecUnsignedChar **format, SecScanSpec *spec)
{
    const SecUnsignedChar *fmt = *format;
    while (SecIsDigit((SecInt)(int)(*fmt)) != 0) {
        spec->widthSet = 1;
        if (SECUREC_MUL_TEN_ADD_BEYOND_MAX(spec->width)) {
            return -1;
        }
        spec->width = (int)SECUREC_MUL_TEN((unsigned int)spec->width) + (unsigned char)(*fmt - SECUREC_CHAR('0'));
        ++fmt;
    }
    *format = fmt;
    return 0;
}

/*
 * Init default flags for each format. do not init ch this variable is context-dependent
 */
SECUREC_INLINE void SecSetDefaultScanSpec(SecScanSpec *spec)
{
    /* The ch and charCount member variables cannot be initialized here */
    spec->argPtr = NULL;
    spec->arrayWidth = 0;
    spec->number64 = 0;
    spec->number = 0;
    spec->numberWidth = SECUREC_NUM_WIDTH_INT;    /* 0 = SHORT, 1 = int, > 1  long or L_DOUBLE */
    spec->numberArgType = 0;                         /* 1 for 64-bit integer, 0 otherwise */
    spec->width = 0;
    spec->widthSet = 0;
    spec->convChr = 0;
    spec->oriConvChr = 0;
    spec->isWCharOrLong = 0;
    spec->suppress = 0;
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    spec->beyondMax = 0;
#endif
    spec->negative = 0;
    spec->numberState = SECUREC_NUMBER_STATE_DEFAULT;
}

/*
 * Decode qualifier %I %L %h ...
 * Set finishFlag to  1  finish Flag
 */
SECUREC_INLINE void  SecDecodeScanQualifier(const SecUnsignedChar **format, SecScanSpec *spec, int *finishFlag)
{
    switch (**format) {
        case SECUREC_CHAR('F'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('N'):
            break;
        case SECUREC_CHAR('h'):
            --spec->numberWidth; /* The h for SHORT , hh for CHAR */
            spec->isWCharOrLong = -1;
            break;
#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
        case SECUREC_CHAR('j'):
            spec->numberWidth = SECUREC_NUM_WIDTH_LONG_LONG;  /* For intmax_t or uintmax_t */
            spec->numberArgType = 1;
            break;
        case SECUREC_CHAR('t'): /* fall-through */ /* FALLTHRU */
#endif


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART02_H_ */
