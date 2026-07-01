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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART04_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART04_H_

            if ((attr->flags & SECUREC_FLAG_ALTERNATE) != 0) {
                /* Alternate form means force a leading 0 */
                attr->flags |= SECUREC_FLAG_FORCE_OCTAL;
            }
            break;
        default:
            /* Do nothing */
            break;
    }
}

#if SECUREC_ENABLE_SPRINTF_FLOAT
SECUREC_INLINE void SecFreeFloatBuffer(SecFloatAdapt *floatAdapt)
{
    if (floatAdapt->floatBuffer != NULL) {
        SECUREC_FREE(floatAdapt->floatBuffer);
    }
    if (floatAdapt->allocatedFmtStr != NULL) {
        SECUREC_FREE(floatAdapt->allocatedFmtStr);
    }
    floatAdapt->floatBuffer = NULL;
    floatAdapt->allocatedFmtStr = NULL;
    floatAdapt->fmtStr = NULL;
    floatAdapt->bufferSize = 0;
}

SECUREC_INLINE void SecSeekToFrontPercent(const SecChar **format)
{
    const SecChar *fmt = *format;
    while (*fmt != SECUREC_CHAR('%')) { /* Must meet '%' */
        --fmt;
    }
    *format = fmt;
}

/* Init float format, return 0 is OK */
SECUREC_INLINE int SecInitFloatFmt(SecFloatAdapt *floatFmt, const SecChar *format)
{
    const SecChar *fmt = format - 2;  /* Sub 2 to the position before 'f' or 'g' */
    int fmtStrLen;
    int i;

    SecSeekToFrontPercent(&fmt);
    /* Now fmt point to '%' */
    fmtStrLen = (int)(size_t)(format - fmt) + 1;   /* With ending terminator */
    if (fmtStrLen > (int)sizeof(floatFmt->buffer)) {
        /* When buffer is NOT enough, alloc a new buffer */
        floatFmt->allocatedFmtStr = (char *)SECUREC_MALLOC((size_t)((unsigned int)fmtStrLen));
        if (floatFmt->allocatedFmtStr == NULL) {
            return -1;
        }
        floatFmt->fmtStr = floatFmt->allocatedFmtStr;
    } else {
        floatFmt->fmtStr = floatFmt->buffer;
        floatFmt->allocatedFmtStr = NULL; /* Must set to NULL, later code free memory based on this identity */
    }

    for (i = 0; i < fmtStrLen - 1; ++i) {
        /* Convert wchar to char */
        floatFmt->fmtStr[i] = (char)(fmt[i]);  /* Copy the format string */
    }
    floatFmt->fmtStr[fmtStrLen - 1] = '\0';

    return 0;
}

/* Init float buffer and format, return 0 is OK */
SECUREC_INLINE int SecInitFloatBuffer(SecFloatAdapt *floatAdapt, const SecChar *format, SecFormatAttr *attr)
{
    floatAdapt->allocatedFmtStr = NULL;
    floatAdapt->fmtStr = NULL;
    floatAdapt->floatBuffer = NULL;
    /* Compute the precision value */
    if (attr->precision < 0) {
        attr->precision = SECUREC_FLOAT_DEFAULT_PRECISION;
    }
    /*
     * Calc buffer size to store double value
     * The maximum length of SECUREC_MAX_WIDTH_LEN is enough
     */
    if ((attr->flags & SECUREC_FLAG_LONG_DOUBLE) != 0) {
        if (attr->precision > (SECUREC_MAX_WIDTH_LEN - SECUREC_FLOAT_BUFSIZE_LB)) {
            return -1;
        }
        /* Long double needs to meet the basic print length */
        floatAdapt->bufferSize = SECUREC_FLOAT_BUFSIZE_LB + attr->precision + SECUREC_FLOAT_BUF_EXT;
    } else {
        if (attr->precision > (SECUREC_MAX_WIDTH_LEN - SECUREC_FLOAT_BUFSIZE)) {
            return -1;
        }
        /* Double needs to meet the basic print length */
        floatAdapt->bufferSize = SECUREC_FLOAT_BUFSIZE + attr->precision + SECUREC_FLOAT_BUF_EXT;
    }
    if (attr->fldWidth > floatAdapt->bufferSize) {
        floatAdapt->bufferSize = attr->fldWidth + SECUREC_FLOAT_BUF_EXT;
    }

    if (floatAdapt->bufferSize > SECUREC_BUFFER_SIZE) {
        /* The current value of SECUREC_BUFFER_SIZE could not store the formatted float string */
        floatAdapt->floatBuffer = (char *)SECUREC_MALLOC(((size_t)(unsigned int)floatAdapt->bufferSize));
        if (floatAdapt->floatBuffer == NULL) {
            return -1;
        }
        attr->text.str = floatAdapt->floatBuffer;
    } else {
        attr->text.str = attr->buffer.str; /* Output buffer for float string with default size */
    }

    if (SecInitFloatFmt(floatAdapt, format) != 0) {
        if (floatAdapt->floatBuffer != NULL) {
            SECUREC_FREE(floatAdapt->floatBuffer);
            floatAdapt->floatBuffer = NULL;
        }
        return -1;
    }
    return 0;
}
#endif

SECUREC_INLINE SecInt64 SecUpdateNegativeChar(SecFormatAttr *attr, char ch)
{
    SecInt64 num64 = ch; /* Sign extend */
    if (num64 >= 128) { /* 128 on some platform, char is always unsigned */
        unsigned char tmp = (unsigned char)(~((unsigned char)ch));
        num64 = tmp + 1;
        attr->flags |= SECUREC_FLAG_NEGATIVE;
    }
    return num64;
}

/*
 * If the precision is not satisfied, zero is added before the string
 */
SECUREC_INLINE void SecNumberSatisfyPrecision(SecFormatAttr *attr)
{
    int precision;
    if (attr->precision < 0) {
        precision = 1; /* Default precision 1 */
    } else {
#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
        attr->flags &= ~SECUREC_FLAG_LEADZERO;
#else
        if ((attr->flags & SECUREC_FLAG_POINTER) == 0) {
            attr->flags &= ~SECUREC_FLAG_LEADZERO;
        }
#endif
        if (attr->precision > SECUREC_MAX_PRECISION) {
            attr->precision = SECUREC_MAX_PRECISION;
        }
        precision = attr->precision;
    }
    while (attr->textLen < precision) {
        --attr->text.str;
        *(attr->text.str) = '0';
        ++attr->textLen;
    }
}

/*
 * Add leading zero for %#o
 */
SECUREC_INLINE void SecNumberForceOctal(SecFormatAttr *attr)
{
    /* Force a leading zero if FORCEOCTAL flag set */
    if ((attr->flags & SECUREC_FLAG_FORCE_OCTAL) != 0 &&
        (attr->textLen == 0 || attr->text.str[0] != '0')) {
        --attr->text.str;
        *(attr->text.str) = '0';
        ++attr->textLen;
    }
}

SECUREC_INLINE void SecUpdateSignedNumberPrefix(SecFormatAttr *attr)
{
    if ((attr->flags & SECUREC_FLAG_SIGNED) == 0) {
        return;
    }
    if ((attr->flags & SECUREC_FLAG_NEGATIVE) != 0) {
        /* Prefix is '-' */
        attr->prefix[0] = SECUREC_CHAR('-');
        attr->prefixLen = 1;
        return;
    }
    if ((attr->flags & SECUREC_FLAG_SIGN) != 0) {
        /* Prefix is '+' */
        attr->prefix[0] = SECUREC_CHAR('+');
        attr->prefixLen = 1;
        return;
    }
    if ((attr->flags & SECUREC_FLAG_SIGN_SPACE) != 0) {
        /* Prefix is ' ' */
        attr->prefix[0] = SECUREC_CHAR(' ');
        attr->prefixLen = 1;
        return;
    }
    return;
}

SECUREC_INLINE void SecNumberCompatZero(SecFormatAttr *attr)
{
#if SECUREC_IN_KERNEL
    if ((attr->flags & SECUREC_FLAG_POINTER) != 0) {
        static char strNullPointer[SECUREC_NULL_STRING_SIZE] = "(null)";
        attr->text.str = strNullPointer;
        attr->textLen = 6; /* Length of (null) is 6 */
        attr->flags &= ~SECUREC_FLAG_LEADZERO;
        attr->prefixLen = 0;
        if (attr->precision >= 0 && attr->precision < attr->textLen) {
            attr->textLen = attr->precision;
        }
    }
    if ((attr->flags & SECUREC_FLAG_POINTER) == 0 && attr->radix == SECUREC_RADIX_HEX &&
        (attr->flags & SECUREC_FLAG_ALTERNATE) != 0) {
        /* Add 0x prefix for %x or %X, the prefix string has been set before */
        attr->prefixLen = SECUREC_PREFIX_LEN;
    }
#elif defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && (!defined(SECUREC_ON_UNIX))
    if ((attr->flags & SECUREC_FLAG_POINTER) != 0) {
        static char strNullPointer[SECUREC_NULL_STRING_SIZE] = "(nil)";
        attr->text.str = strNullPointer;
        attr->textLen = 5; /* Length of (nil) is 5 */
        attr->flags &= ~SECUREC_FLAG_LEADZERO;
    }
#elif defined(SECUREC_VXWORKS_PLATFORM) || defined(__hpux)
    if ((attr->flags & SECUREC_FLAG_POINTER) != 0 && (attr->flags & SECUREC_FLAG_ALTERNATE) != 0) {
        /* Add 0x prefix for %p, the prefix string has been set before */
        attr->prefixLen = SECUREC_PREFIX_LEN;
    }
#endif
    (void)attr; /* To clear e438 last value assigned not used , the compiler will optimize this code */
}

/*
 * Formatting output core function
 */
SECUREC_INLINE SecInt64 SecOutputReadNumber(SecFormatAttr *formatAttr, va_list argList)
{
    if ((formatAttr->flags & SECUREC_FLAG_I64) != 0 ||
        (formatAttr->flags & SECUREC_FLAG_LONGLONG) != 0) {
        return (SecInt64)va_arg(argList, SecInt64); /* Maximum Bit Width sign bit unchanged */
    }
    if ((formatAttr->flags & SECUREC_FLAG_LONG) != 0) {
        return SECUREC_GET_LONG_FROM_ARG(*formatAttr);
    }
    if ((formatAttr->flags & SECUREC_FLAG_CHAR) != 0) {
        return SECUREC_GET_CHAR_FROM_ARG(*formatAttr);
    }
    if ((formatAttr->flags & SECUREC_FLAG_SHORT) != 0) {
        return SECUREC_GET_SHORT_FROM_ARG(*formatAttr);
    }
#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
    if ((formatAttr->flags & SECUREC_FLAG_PTRDIFF) != 0) {
        return (ptrdiff_t)va_arg(argList, ptrdiff_t);  /* Sign extend */
    }
    if ((formatAttr->flags & SECUREC_FLAG_SIZE) != 0) {
        return SECUREC_GET_SIZE_FROM_ARG(*formatAttr);
    }
    if ((formatAttr->flags & SECUREC_FLAG_INTMAX) != 0) {
        return (SecInt64)va_arg(argList, SecInt64);
    }
#endif
    return SECUREC_GET_INT_FROM_ARG(*formatAttr);
}

SECUREC_INLINE int SecOutputHandleFloat(SecPrintfStream *stream, SecFormatAttr *formatAttr,
                                        const SecChar *format, va_list argList, int *charsOut)
{
#if SECUREC_ENABLE_SPRINTF_FLOAT
    SecFloatAdapt floatAdapt;

    if (SecInitFloatBuffer(&floatAdapt, format, formatAttr) != 0) {
        return 1;
    }
    if ((formatAttr->flags & SECUREC_FLAG_LONG_DOUBLE) != 0) {
#if defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && SECUREC_ENABLE_SPRINTF_LONG_DOUBLE
        long double tmp = (long double)va_arg(argList, long double);
        SecFormatLongDouble(formatAttr, &floatAdapt, tmp);
#else
        double tmp = (double)va_arg(argList, double);
        SecFormatDouble(formatAttr, &floatAdapt, tmp);
#endif
    } else {
        double tmp = (double)va_arg(argList, double);
        SecFormatDouble(formatAttr, &floatAdapt, tmp);
    }
    SecWriteFloatText(stream, formatAttr, charsOut);
    SecFreeFloatBuffer(&floatAdapt);
    return 1;
#else
    (void)stream;
    (void)formatAttr;
    (void)format;
    (void)argList;
    (void)charsOut;
    return -1;
#endif
}

typedef struct {
    SecPrintfStream *stream;
    SecFormatAttr *formatAttr;
    const SecChar **format;
    SecFmtState *state;
    SecFmtState lastState;
    SecChar ch;
    int *charsOut;
} SecOutputStateCtx;

SECUREC_INLINE int SecOutputHandleType(SecOutputStateCtx *ctx, va_list argList)
{
    SecInt64 num64;

    switch (ctx->ch) {
        case SECUREC_CHAR('C'): /* Wide char */
            SecUpdateWcharFlags(ctx->formatAttr);
            /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('c'):
            SecDecodeTypeC(ctx->formatAttr, (unsigned int)va_arg(argList, int));
            return 0;
        case SECUREC_CHAR('S'):    /* Wide char string */
            SecUpdateWstringFlags(ctx->formatAttr);
            /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('s'):
            SecDecodeTypeS(ctx->formatAttr, (char *)va_arg(argList, char *));
            return 0;
        case SECUREC_CHAR('G'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('g'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('E'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('F'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('e'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('f'):
            return SecOutputHandleFloat(ctx->stream, ctx->formatAttr, *(ctx->format), argList, ctx->charsOut);
        case SECUREC_CHAR('X'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('p'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('x'):
            SecUpdateXpxFlags(ctx->formatAttr, ctx->ch);
            /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('i'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('d'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('u'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('o'):
            SecUpdateOudiFlags(ctx->formatAttr, ctx->ch);
            num64 = SecOutputReadNumber(ctx->formatAttr, argList);
            SecNumberToBuffer(ctx->formatAttr, num64);
            SecNumberSatisfyPrecision(ctx->formatAttr);
            SecNumberForceOctal(ctx->formatAttr);
            SecUpdateSignedNumberPrefix(ctx->formatAttr);
            if (num64 == 0) {
                SecNumberCompatZero(ctx->formatAttr);
            }
            return 0;
        default:
            return 0;
    }
}

SECUREC_INLINE void SecOutputResetFormatAttr(SecFormatAttr *formatAttr, int *noOutput)
{
    *noOutput = 0;
    formatAttr->prefixLen = 0;
    formatAttr->textLen = 0;
    formatAttr->flags = 0;
    formatAttr->fldWidth = 0;
    formatAttr->precision = -1;
    formatAttr->textIsWide = 0;
    formatAttr->dynWidth = 0;
    formatAttr->dynPrecision = 0;
}

SECUREC_INLINE void SecOutputEmitFormattedField(SecPrintfStream *stream, SecFormatAttr *formatAttr, int *charsOut)
{
    formatAttr->padding = (formatAttr->fldWidth - formatAttr->textLen) - formatAttr->prefixLen;
    SecWriteLeftPadding(stream, formatAttr, charsOut);
    SecWritePrefix(stream, formatAttr, charsOut);
    SecWriteLeadingZero(stream, formatAttr, charsOut);
    SecWriteText(stream, formatAttr, charsOut);
    SecWriteRightPadding(stream, formatAttr, charsOut);
}

SECUREC_INLINE int SecOutputHandleWidth(SecChar ch, SecFormatAttr *formatAttr, SecFmtState lastState, va_list argList)
{
    if (ch == SECUREC_CHAR('*')) {
        formatAttr->fldWidth = (int)va_arg(argList, int);
        formatAttr->dynWidth = 1;
    }
    return SecDecodeWidth(ch, formatAttr, lastState);
}

SECUREC_INLINE int SecOutputHandlePrecision(SecChar ch, SecFormatAttr *formatAttr, va_list argList)
{
    if (ch == SECUREC_CHAR('*')) {
        formatAttr->precision = (int)va_arg(argList, int);
        formatAttr->dynPrecision = 1;
    }
    return SecDecodePrecision(ch, formatAttr);
}

SECUREC_INLINE int SecOutputHandleSize(SecOutputStateCtx *ctx)
{
    if (SecDecodeSize(ctx->ch, ctx->formatAttr, ctx->format) != 0) {
        SecWriteChar(ctx->stream, ctx->ch, ctx->charsOut);
        *(ctx->state) = STAT_NORMAL;
        return 1;
    }
    return 0;
}

SECUREC_INLINE int SecOutputHandleStateType(SecOutputStateCtx *ctx, va_list argList)
{
    int noOutput = SecOutputHandleType(ctx, argList);
    if (noOutput < 0) {
        return -1;
    }
    if (noOutput == 0) {
        SecOutputEmitFormattedField(ctx->stream, ctx->formatAttr, ctx->charsOut);
    }
    return 0;
}

SECUREC_INLINE void SecOutputInitFormatAttr(SecFormatAttr *formatAttr)
{
    formatAttr->flags = 0;
    formatAttr->textIsWide = 0;
    formatAttr->fldWidth = 0;
    formatAttr->precision = 0;
    formatAttr->dynWidth = 0;
    formatAttr->dynPrecision = 0;
    formatAttr->digits = g_itoaUpperDigits;
    formatAttr->radix = SECUREC_RADIX_DECIMAL;
    formatAttr->padding = 0;


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART04_H_ */
