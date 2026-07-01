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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART02_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART02_H_

#if defined(SECUREC_ON_64BITS) || !defined(SECUREC_VXWORKS_VERSION_5_4)
#if defined(SECUREC_USE_SPECIAL_DIV64)
/* The compiler does not provide 64 bit division problems */
SECUREC_INLINE void SecNumber64ToDecString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        SecUnsignedInt64 quotient = 0;
        SecUnsignedInt32 digit = 0;
        SecU64Div10(val64, &quotient, &digit);
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[digit];
        val64 = quotient;
    } while (val64 != 0);
}
#else
/*
 * Compiler Optimized Division 10.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber64ToDecString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[val64 % SECUREC_RADIX_DECIMAL];
        val64 /= SECUREC_RADIX_DECIMAL;
    } while (val64 != 0);
}
#endif

/*
 * Compiler Optimized Division 8.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber64ToOctalString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        --attr->text.str;
        /* Just use lowerDigits for 0 - 9 */
        *(attr->text.str) = g_itoaLowerDigits[val64 % SECUREC_RADIX_OCTAL];
        val64 /= SECUREC_RADIX_OCTAL;
    } while (val64 != 0);
}
/*
 * Compiler Optimized Division 16.
 * The text.str point to buffer end, must be Large enough
 */
SECUREC_INLINE void SecNumber64ToHexString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    SecUnsignedInt64 val64 = number;
    do {
        --attr->text.str;
        *(attr->text.str) = attr->digits[val64 % SECUREC_RADIX_HEX];
        val64 /= SECUREC_RADIX_HEX;
    } while (val64 != 0);
}

SECUREC_INLINE void SecNumber64ToString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
    switch (attr->radix) {
        /* The compiler will optimize div 10 */
        case SECUREC_RADIX_DECIMAL:
            SecNumber64ToDecString(number, attr);
            break;
        case SECUREC_RADIX_OCTAL:
            SecNumber64ToOctalString(number, attr);
            break;
        case SECUREC_RADIX_HEX:
            SecNumber64ToHexString(number, attr);
            break;
        default:
            /* Do nothing */
            break;
    }
}
#endif

/*
 * Converting integers to string
 */
SECUREC_INLINE void SecNumberToString(SecUnsignedInt64 number, SecFormatAttr *attr)
{
#ifdef SECUREC_ON_64BITS
    SecNumber64ToString(number, attr);
#else /* For 32 bits system */
    if (number <= 0xffffffffUL) { /* Use 0xffffffffUL to check if the value is in the 32-bit range */
        /* In most case, the value to be converted is small value */
        SecUnsignedInt32 n32Tmp = (SecUnsignedInt32)number;
        SecNumber32ToString(n32Tmp, attr);
    } else {
        /* The value to be converted is greater than 4G */
#if defined(SECUREC_VXWORKS_VERSION_5_4)
        SecNumber64ToStringSpecial(number, attr);
#else
        SecNumber64ToString(number, attr);
#endif
    }
#endif
}

SECUREC_INLINE int SecIsNumberNeedTo32Bit(const SecFormatAttr *attr)
{
    return (int)(((attr->flags & SECUREC_FLAG_I64) == 0) &&
#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
            ((attr->flags & SECUREC_FLAG_INTMAX) == 0) &&
#endif
#ifdef SECUREC_ON_64BITS
            ((attr->flags & SECUREC_FLAG_PTRDIFF) == 0) &&
            ((attr->flags & SECUREC_FLAG_SIZE) == 0) &&
#if !defined(SECUREC_COMPATIBLE_WIN_FORMAT)  /* on window 64 system sizeof long is 32bit */
            ((attr->flags & SECUREC_FLAG_LONG) == 0) &&
#endif
#endif
            ((attr->flags & SECUREC_FLAG_LONGLONG) == 0));
}

SECUREC_INLINE void SecNumberToBuffer(SecFormatAttr *attr, SecInt64 num64)
{
    SecUnsignedInt64 number;
    /* Check for negative; copy into number */
    if ((attr->flags & SECUREC_FLAG_SIGNED) != 0 && num64 < 0) {
        number = (SecUnsignedInt64)(0 - (SecUnsignedInt64)num64); /* Wrap with unsigned int64 numbers */
        attr->flags |= SECUREC_FLAG_NEGATIVE;
    } else {
        number = (SecUnsignedInt64)num64;
    }
    if (SecIsNumberNeedTo32Bit(attr) != 0) {
        number = (number & (SecUnsignedInt64)0xffffffffUL);  /* Use 0xffffffff as 32 bit mask */
    }

    /* The text.str must be point to buffer.str, this pointer is used outside the function */
    attr->text.str = &attr->buffer.str[SECUREC_BUFFER_SIZE];

    if (number == 0) {
        /* Turn off hex prefix default, and textLen is zero */
        attr->prefixLen = 0;
        attr->textLen = 0;
        return;
    }

    /* Convert integer to string. It must be invoked when number > 0, otherwise the following logic is incorrect */
    SecNumberToString(number, attr);
    /* Compute length of number,  text.str must be in buffer.str */
    attr->textLen = (int)(size_t)((char *)&attr->buffer.str[SECUREC_BUFFER_SIZE] - attr->text.str);
}

/*
 * Write one character to dest buffer
 */
SECUREC_INLINE void SecWriteChar(SecPrintfStream *stream, SecChar ch, int *charsOut)
{
    /* Count must be reduced first, In order to identify insufficient length */
    --stream->count;
    if (stream->count >= 0) {
        *(stream->cur) = ch;
        ++stream->cur;
        *charsOut = *charsOut + 1;
        return;
    }
    /* No enough length */
    *charsOut = -1;
}

/*
 * Write multiple identical characters.
 */
SECUREC_INLINE void SecWriteMultiChar(SecPrintfStream *stream, SecChar ch, int num, int *charsOut)
{
    int count;
    for (count = num; count > 0; --count) {
        --stream->count; /* count may be negative,indicating insufficient space */
        if (stream->count < 0) {
            *charsOut = -1;
            return;
        }
        *(stream->cur) = ch;
        ++stream->cur;
    }
    *charsOut = *charsOut + num;
}

/*
 * Write string function, where this function is called, make sure that len is greater than 0
 */
SECUREC_INLINE void SecWriteString(SecPrintfStream *stream, const SecChar *str, int len, int *charsOut)
{
    const SecChar *tmp = str;
    int count;
    for (count = len; count > 0; --count) {
        --stream->count; /* count may be negative,indicating insufficient space */
        if (stream->count < 0) {
            *charsOut = -1;
            return;
        }
        *(stream->cur) = *tmp;
        ++stream->cur;
        ++tmp;
    }
    *charsOut = *charsOut + len;
}

/* Use loop copy char or wchar_t string */
SECUREC_INLINE void SecWriteStringByLoop(SecPrintfStream *stream, const SecChar *str, int len)
{
    int i;
    const SecChar *tmp = str;
    for (i = 0; i < len; ++i) {
        *stream->cur = *tmp;
        ++stream->cur;
        ++tmp;
    }
    stream->count -= len;
}

SECUREC_INLINE void SecWriteStringOpt(SecPrintfStream *stream, const SecChar *str, int len)
{
    if (len < 12) { /* Performance optimization for mobile number length 12 */
        SecWriteStringByLoop(stream, str, len);
    } else {
        size_t count = (size_t)(unsigned int)len * sizeof(SecChar);
        SECUREC_MEMCPY_WARP_OPT(stream->cur, str, count);
        stream->cur += len;
        stream->count -= len;
    }
}

/*
 * Return if buffer length is enough
 * The count variable can be reduced to 0, and the external function complements the \0 terminator.
 */
SECUREC_INLINE int SecIsStreamBufEnough(const SecPrintfStream *stream, int needLen)
{
    return (int)(stream->count >= needLen);
}

/* Write text string */
SECUREC_INLINE void SecWriteTextOpt(SecPrintfStream *stream, const SecChar *str, int len, int *charsOut)
{
    if (SecIsStreamBufEnough(stream, len) != 0) {
        SecWriteStringOpt(stream, str, len);
        *charsOut += len;
    } else {
        SecWriteString(stream, str, len, charsOut);
    }
}

/* Write left padding */
SECUREC_INLINE void SecWriteLeftPadding(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    if ((attr->flags & (SECUREC_FLAG_LEFT | SECUREC_FLAG_LEADZERO)) == 0 && attr->padding > 0) {
        /* Pad on left with blanks */
        SecWriteMultiChar(stream, SECUREC_CHAR(' '), attr->padding, charsOut);
    }
}

/* Write prefix */
SECUREC_INLINE void SecWritePrefix(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    if (attr->prefixLen > 0) {
        SecWriteString(stream, attr->prefix, attr->prefixLen, charsOut);
    }
}

/* Write leading zeros */
SECUREC_INLINE void SecWriteLeadingZero(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    if ((attr->flags & SECUREC_FLAG_LEADZERO) != 0 && (attr->flags & SECUREC_FLAG_LEFT) == 0 &&
        attr->padding > 0) {
        SecWriteMultiChar(stream, SECUREC_CHAR('0'), attr->padding, charsOut);
    }
}

/* Write right padding */
SECUREC_INLINE void SecWriteRightPadding(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    if (*charsOut >= 0 && (attr->flags & SECUREC_FLAG_LEFT) != 0 && attr->padding > 0) {
        /* Pad on right with blanks */
        SecWriteMultiChar(stream, SECUREC_CHAR(' '), attr->padding, charsOut);
    }
}

#ifdef SECUREC_FOR_WCHAR
#define SECUREC_TEXT_CHAR_PTR(text)  ((text).wStr)
#define SECUREC_NEED_CONVERT_TEXT(attr) ((attr)->textIsWide == 0)
#if SECUREC_HAVE_MBTOWC
#define SECUREC_WRITE_TEXT_AFTER_CONVERT(stream, attr, charsOut) SecWriteTextAfterMbtowc((stream), (attr), (charsOut))
#else
#define SECUREC_WRITE_TEXT_AFTER_CONVERT(stream, attr, charsOut) (*(charsOut) = -1)
#endif
#else
#define SECUREC_TEXT_CHAR_PTR(text)  ((text).str)
#define SECUREC_NEED_CONVERT_TEXT(attr) ((attr)->textIsWide != 0)
#if SECUREC_HAVE_WCTOMB
#define SECUREC_WRITE_TEXT_AFTER_CONVERT(stream, attr, charsOut) SecWriteTextAfterWctomb((stream), (attr), (charsOut))
#else
#define SECUREC_WRITE_TEXT_AFTER_CONVERT(stream, attr, charsOut) (*(charsOut) = -1)
#endif
#endif

#ifdef SECUREC_FOR_WCHAR
#if SECUREC_HAVE_MBTOWC
SECUREC_INLINE void SecWriteTextAfterMbtowc(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    const char *p = attr->text.str;
    int count = attr->textLen;
    while (count > 0) {
        wchar_t wChar = L'\0';
        int retVal = mbtowc(&wChar, p, (size_t)MB_CUR_MAX);
        if (retVal <= 0) {
            *charsOut = -1;
            break;
        }
        SecWriteChar(stream, wChar, charsOut);
        if (*charsOut == -1) {
            break;
        }
        p += retVal;
        count -= retVal;
    }
}
#endif
#else  /* Not SECUREC_FOR_WCHAR */
#if SECUREC_HAVE_WCTOMB
SECUREC_INLINE void SecWriteTextAfterWctomb(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    const wchar_t *p = attr->text.wStr;
    int count = attr->textLen;
    while (count > 0) {
        char tmpBuf[SECUREC_MB_LEN + 1];
        SECUREC_MASK_MSVC_CRT_WARNING
        int retVal = wctomb(tmpBuf, *p);
        SECUREC_END_MASK_MSVC_CRT_WARNING
        if (retVal <= 0) {
            *charsOut = -1;
            break;
        }
        SecWriteString(stream, tmpBuf, retVal, charsOut);
        if (*charsOut == -1) {
            break;
        }
        --count;
        ++p;
    }
}
#endif
#endif

#if SECUREC_ENABLE_SPRINTF_FLOAT
/*
 * Write text of float
 * Using independent functions to optimize the expansion of inline functions by the compiler
 */
SECUREC_INLINE void SecWriteFloatText(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
#ifdef SECUREC_FOR_WCHAR
#if SECUREC_HAVE_MBTOWC
    SecWriteTextAfterMbtowc(stream, attr, charsOut);
#else
    *charsOut = -1;
    (void)stream; /* To clear e438 last value assigned not used , the compiler will optimize this code */
    (void)attr;   /* To clear e438 last value assigned not used , the compiler will optimize this code */
#endif
#else /* Not SECUREC_FOR_WCHAR */
    SecWriteString(stream, attr->text.str, attr->textLen, charsOut);
#endif
}
#endif

/* Write text of integer or string ... */
SECUREC_INLINE void SecWriteText(SecPrintfStream *stream, const SecFormatAttr *attr, int *charsOut)
{
    if (SECUREC_NEED_CONVERT_TEXT(attr)) {
        SECUREC_WRITE_TEXT_AFTER_CONVERT(stream, attr, charsOut);
    } else {
        SecWriteTextOpt(stream, SECUREC_TEXT_CHAR_PTR(attr->text), attr->textLen, charsOut);
    }
}

#define SECUREC_FMT_STATE_OFFSET  256

SECUREC_INLINE SecFmtState SecDecodeState(SecChar ch, SecFmtState lastState)
{
    static const unsigned char stateTable[SECUREC_STATE_TABLE_SIZE] = {
        /*
         * Type
         * 0:    nospecial meaning;
         * 1:    '%'
         * 2:    '.'
         * 3:    '*'
         * 4:    '0'
         * 5:    '1' ... '9'
         * 6:    ' ', '+', '-', '#'
         * 7:    'h', 'l', 'L', 'w' , 'N', 'z', 'q', 't', 'j'
         * 8:    'd', 'o', 'u', 'i', 'x', 'X', 'e', 'f', 'g', 'E', 'F', 'G', 's', 'c', '[', 'p'
         */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x06, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x06, 0x02, 0x00,
        0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x08, 0x08, 0x00, 0x07, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x07, 0x08, 0x07, 0x00, 0x07, 0x00, 0x00, 0x08,
        0x08, 0x07, 0x00, 0x08, 0x07, 0x08, 0x00, 0x07, 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
        /* Fill zero  for normal char 128 byte for 0x80 - 0xff */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        /*
         * State
         * 0: normal
         * 1: percent
         * 2: flag
         * 3: width
         * 4: dot
         * 5: precis
         * 6: size
         * 7: type
         * 8: invalid
         */
        0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x01, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08,
        0x01, 0x00, 0x00, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x03, 0x03, 0x08, 0x05,


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART02_H_ */
