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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART04_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART04_H_

#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->number64 == SECUREC_MAX_64BITS_VALUE_CUT_LAST_DIGIT) {
        SecUnsignedInt64 number64As = (SecUnsignedInt64)SECUREC_MAX_64BITS_VALUE - spec->number64;
        if (number64As < (SecUnsignedInt64)(SecUnsignedInt)spec->ch - (SecUnsignedInt)SECUREC_CHAR('0')) {
            spec->beyondMax = 1;
        }
    }
#endif
    spec->number64 += ((SecUnsignedInt64)(SecUnsignedInt)spec->ch - (SecUnsignedInt)SECUREC_CHAR('0'));
}

/*
 * Parse Hex character to integer for 64bit .
 */
static void SecDecodeNumber64Hex(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (SECUREC_QWORD_HEX_BEYOND_MAX(spec->number64)) {
        spec->beyondMax = 1;
    }
#endif
    spec->number64 = SECUREC_MUL_SIXTEEN(spec->number64);
    spec->number64 += (SecUnsignedInt64)(unsigned int)SecHexValueOfChar(spec->ch);
}

/*
 * Parse Octal character to integer for 64bit .
 */
static void SecDecodeNumber64Octal(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (SECUREC_QWORD_OCTAL_BEYOND_MAX(spec->number64)) {
        spec->beyondMax = 1;
    }
#endif
    spec->number64 = SECUREC_MUL_EIGHT(spec->number64);
    spec->number64 += ((SecUnsignedInt64)(SecUnsignedInt)spec->ch - (SecUnsignedInt)SECUREC_CHAR('0'));
}

#define SECUREC_DECODE_NUMBER_FUNC_NUM 2

/*
 * Parse 64-bit integer formatted input, return 0 when ch is a number.
 */
SECUREC_INLINE int SecDecodeNumber(SecScanSpec *spec)
{
    /* Function name cannot add address symbol, causing 546 alarm */
    static void (* const secDecodeNumberHex[SECUREC_DECODE_NUMBER_FUNC_NUM])(SecScanSpec *spec) = {
        SecDecodeNumberHex, SecDecodeNumber64Hex
    };
    static void (* const secDecodeNumberOctal[SECUREC_DECODE_NUMBER_FUNC_NUM])(SecScanSpec *spec) = {
        SecDecodeNumberOctal, SecDecodeNumber64Octal
    };
    static void (* const secDecodeNumberDecimal[SECUREC_DECODE_NUMBER_FUNC_NUM])(SecScanSpec *spec) = {
        SecDecodeNumberDecimal, SecDecodeNumber64Decimal
    };
    if (spec->convChr == 'x' || spec->convChr == 'p') {
        if (SecIsXdigit(spec->ch) != 0) {
            (*secDecodeNumberHex[spec->numberArgType])(spec);
        } else {
            return -1;
        }
        return 0;
    }
    if (SecIsDigit(spec->ch) == 0) {
        return -1;
    }
    if (spec->convChr == 'o') {
        if (spec->ch < SECUREC_CHAR('8')) { /* Octal maximum limit '8' */
            (*secDecodeNumberOctal[spec->numberArgType])(spec);
        } else {
            return -1;
        }
    } else { /* The convChr is 'd' */
        (*secDecodeNumberDecimal[spec->numberArgType])(spec);
    }
    return 0;
}

/*
 * Complete the final 32-bit integer formatted input
 */
static void SecFinishNumber(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->negative != 0) {
        if (spec->numberWidth == SECUREC_NUM_WIDTH_INT) {
            SecFinishNumberNegativeInt(spec);
        } else {
            SecFinishNumberNegativeOther(spec);
        }
    } else {
        if (spec->numberWidth == SECUREC_NUM_WIDTH_INT) {
            SecFinishNumberPositiveInt(spec);
        } else {
            SecFinishNumberPositiveOther(spec);
        }
    }
#else
    if (spec->negative != 0) {
#if defined(__hpux)
        if (spec->oriConvChr != 'p') {
            spec->number = (unsigned long)(0U - spec->number); /* Wrap with unsigned long numbers */
        }
#else
        spec->number = (unsigned long)(0U - spec->number); /* Wrap with unsigned long numbers */
#endif
    }
#endif
    return;
}

/*
 * Complete the final 64-bit integer formatted input
 */
static void SecFinishNumber64(SecScanSpec *spec)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) && !(defined(SECUREC_ON_UNIX)))
    if (spec->negative != 0) {
        if (SECUREC_CONVERT_IS_SIGNED(spec->oriConvChr)) {
            if (spec->number64 > SECUREC_MIN_64BITS_NEG_VALUE) {
                spec->number64 = SECUREC_MIN_64BITS_NEG_VALUE;
            } else {
                spec->number64 = (SecUnsignedInt64)(0U - spec->number64); /* Wrap with unsigned int64 numbers */
            }
            if (spec->beyondMax != 0) {
                spec->number64 = SECUREC_MIN_64BITS_NEG_VALUE;
            }
        } else {                /* For o, u, x, X, p */
            spec->number64 = (SecUnsignedInt64)(0U - spec->number64); /* Wrap with unsigned int64 numbers */
            if (spec->beyondMax != 0) {
                spec->number64 = SECUREC_MAX_64BITS_VALUE;
            }
        }
    } else {
        if (SECUREC_CONVERT_IS_SIGNED(spec->oriConvChr)) {
            if (spec->number64 > SECUREC_MAX_64BITS_POS_VALUE) {
                spec->number64 = SECUREC_MAX_64BITS_POS_VALUE;
            }
            if (spec->beyondMax != 0) {
                spec->number64 = SECUREC_MAX_64BITS_POS_VALUE;
            }
        } else {
            if (spec->beyondMax != 0) {
                spec->number64 = SECUREC_MAX_64BITS_VALUE;
            }
        }
    }
#else
    if (spec->negative != 0) {
#if defined(__hpux)
        if (spec->oriConvChr != 'p') {
            spec->number64 = (SecUnsignedInt64)(0U - spec->number64); /* Wrap with unsigned int64 numbers */
        }
#else
        spec->number64 = (SecUnsignedInt64)(0U - spec->number64); /* Wrap with unsigned int64 numbers */
#endif
    }
#endif
    return;
}

#if SECUREC_ENABLE_SCANF_FILE

/*
 *  Adjust the pointer position of the file stream
 */
SECUREC_INLINE void SecSeekStream(SecFileStream *stream)
{
    if (stream->count == 0) {
        if (feof(stream->pf) != 0) {
            /* File pointer at the end of file, don't need to seek back */
            stream->base[0] = '\0';
            return;
        }
    }
    /* Seek to original position, for file read, but nothing to input */
    if (fseek(stream->pf, stream->oriFilePos, SEEK_SET) != 0) {
        /* Seek failed, ignore it */
        stream->oriFilePos = 0;
        return;
    }

    if (stream->fileRealRead > 0) { /* Do not seek without input data */
#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
        size_t residue = stream->fileRealRead % SECUREC_BUFFERED_BLOK_SIZE;
        size_t loops;
        for (loops = 0; loops < (stream->fileRealRead / SECUREC_BUFFERED_BLOK_SIZE); ++loops) {
            if (fread(stream->base, (size_t)SECUREC_BUFFERED_BLOK_SIZE, (size_t)1, stream->pf) != (size_t)1) {
                break;
            }
        }
        if (residue != 0) {
            long curFilePos;
            if (fread(stream->base, residue, (size_t)1, stream->pf) != (size_t)1) {
                return;
            }
            curFilePos = ftell(stream->pf);
            if (curFilePos < stream->oriFilePos ||
                (size_t)(unsigned long)(curFilePos - stream->oriFilePos) < stream->fileRealRead) {
                /* Try to remedy the problem */
                size_t readOffset = (size_t)(unsigned long)(curFilePos - stream->oriFilePos);
                long adjustNum = (long)(stream->fileRealRead - readOffset);
                (void)fseek(stream->pf, adjustNum, SEEK_CUR);
            }
        }
#else
        /* Seek from oriFilePos. Regardless of the integer sign problem, call scanf will not read very large data */
        if (fseek(stream->pf, (long)stream->fileRealRead, SEEK_CUR) != 0) {
            /* Seek failed, ignore it */
            stream->oriFilePos = 0;
            return;
        }
#endif
    }
    return;
}

/*
 *  Adjust the pointer position of the file stream and free memory
 */
SECUREC_INLINE void SecAdjustStream(SecFileStream *stream)
{
    if ((stream->flag & SECUREC_FILE_STREAM_FLAG) != 0 && stream->base != NULL) {
        SecSeekStream(stream);
        SECUREC_FREE(stream->base);
        stream->base = NULL;
    }
    return;
}
#endif

SECUREC_INLINE void SecSkipSpaceFormat(const SecUnsignedChar **format)
{
    const SecUnsignedChar *fmt = *format;
    while (SecIsSpace((SecInt)(int)(*fmt)) != 0) {
        ++fmt;
    }
    *format = fmt;
}

#if !defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_VERSION)
/*
 * Handling multi-character characters
 */
SECUREC_INLINE int SecDecodeLeadByte(SecScanSpec *spec, const SecUnsignedChar **format, SecFileStream *stream)
{
#if SECUREC_HAVE_MBTOWC
    const SecUnsignedChar *fmt = *format;
    int ch1 = (int)spec->ch;
    int ch2 = SecGetChar(stream, &(spec->charCount));
    spec->ch = (SecInt)ch2;
    if (*fmt == SECUREC_CHAR('\0') || (int)(*fmt) != ch2) {
        /* in console mode, ungetc twice may cause problem */
        SecUnGetChar(ch2, stream, &(spec->charCount));
        SecUnGetChar(ch1, stream, &(spec->charCount));
        return -1;
    }
    ++fmt;
    if ((unsigned int)MB_CUR_MAX >= SECUREC_UTF8_BOM_HEADER_SIZE &&
        (((unsigned char)ch1 & SECUREC_UTF8_LEAD_1ST) == SECUREC_UTF8_LEAD_1ST) &&
        (((unsigned char)ch2 & SECUREC_UTF8_LEAD_2ND) == SECUREC_UTF8_LEAD_2ND)) {
        /* This char is very likely to be a UTF-8 char */
        wchar_t tempWChar;
        char temp[SECUREC_MULTI_BYTE_MAX_LEN];
        int ch3 = (int)SecGetChar(stream, &(spec->charCount));
        spec->ch = (SecInt)ch3;
        if (*fmt == SECUREC_CHAR('\0') || (int)(*fmt) != ch3) {
            SecUnGetChar(ch3, stream, &(spec->charCount));
            return -1;
        }
        temp[0] = (char)ch1;
        temp[1] = (char)ch2; /* 1 index of second character */
        temp[2] = (char)ch3; /* 2 index of third character */
        temp[3] = '\0';      /* 3 of string terminator position */
        if (mbtowc(&tempWChar, temp, sizeof(temp)) > 0) {
            /* Succeed */
            ++fmt;
            --spec->charCount;
        } else {
            SecUnGetChar(ch3, stream, &(spec->charCount));
        }
    }
    --spec->charCount;    /* Only count as one character read */
    *format = fmt;
    return 0;
#else
    SecUnGetChar(spec->ch, stream, &(spec->charCount));
    (void)format; /* To clear e438 last value assigned not used , the compiler will optimize this code */
    return -1;
#endif
}

SECUREC_INLINE int SecFilterWcharInFormat(SecScanSpec *spec, const SecUnsignedChar **format, SecFileStream *stream)
{
    if (SecIsLeadByte(spec->ch) != 0) {
        if (SecDecodeLeadByte(spec, format, stream) != 0) {
            return -1;
        }
    }
    return 0;
}
#endif

/*
 *  Resolving sequence of characters from %[ format, format wile point to ']'
 */
SECUREC_INLINE int SecSetupBracketTable(const SecUnsignedChar **format, SecBracketTable *bracketTable)
{
    const SecUnsignedChar *fmt = *format;
    SecUnsignedChar prevChar = 0;
#if !(defined(SECUREC_COMPATIBLE_WIN_FORMAT))
    if (*fmt == SECUREC_CHAR('{')) {
        return -1;
    }
#endif
    /* For building "table" data */
    ++fmt; /* Skip [ */
    bracketTable->mask = 0; /* Set all bits to 0 */
    if (*fmt == SECUREC_CHAR('^')) {
        ++fmt;
        bracketTable->mask = (unsigned char)0xffU; /* Use 0xffU to set all bits to 1 */
    }
    if (*fmt == SECUREC_CHAR(']')) {
        prevChar = SECUREC_CHAR(']');
        ++fmt;
        SecBracketSetBit(bracketTable->table, SECUREC_CHAR(']'));
    }
    while (*fmt != SECUREC_CHAR('\0') && *fmt != SECUREC_CHAR(']')) {
        SecUnsignedChar expCh = *fmt;
        ++fmt;
        if (expCh != SECUREC_CHAR('-') || prevChar == 0 || *fmt == SECUREC_CHAR(']')) {
            /* Normal character */
            prevChar = expCh;
            SecBracketSetBit(bracketTable->table, expCh);
        } else {
            /* For %[a-z] */
            expCh = *fmt;   /* Get end of range */
            ++fmt;
            if (prevChar <= expCh) {  /* %[a-z] %[a-a] */
                SecBracketSetBitRange(bracketTable->table, prevChar, expCh);
            } else {
                /* For %[z-a] */
#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
                /* Swap start and end characters */
                SecBracketSetBitRange(bracketTable->table, expCh, prevChar);
#else
                SecBracketSetBit(bracketTable->table, SECUREC_CHAR('-'));
                SecBracketSetBit(bracketTable->table, expCh);
#endif
            }
            prevChar = 0;
        }
    }
    *format = fmt;
    return 0;
}

#ifdef SECUREC_FOR_WCHAR
SECUREC_INLINE int SecInputForWchar(SecScanSpec *spec)
{
    void *endPtr = spec->argPtr;
    if (spec->isWCharOrLong > 0) {
        *(wchar_t UNALIGNED *)endPtr = (wchar_t)spec->ch;
        endPtr = (wchar_t *)endPtr + 1;
        --spec->arrayWidth;
    } else {
#if SECUREC_HAVE_WCTOMB
        int temp;
        char tmpBuf[SECUREC_MB_LEN + 1];
        SECUREC_MASK_MSVC_CRT_WARNING temp = wctomb(tmpBuf, (wchar_t)spec->ch);
        SECUREC_END_MASK_MSVC_CRT_WARNING
        if (temp <= 0 || (size_t)(unsigned int)temp > sizeof(tmpBuf)) {
            /* If wctomb  error, then ignore character */
            return 0;
        }
        if (((size_t)(unsigned int)temp) > spec->arrayWidth) {
            return -1;
        }
        if (memcpy_s(endPtr, spec->arrayWidth, tmpBuf, (size_t)(unsigned int)temp) != EOK) {
            return -1;
        }
        endPtr = (char *)endPtr + temp;
        spec->arrayWidth -= (size_t)(unsigned int)temp;
#else
        return -1;
#endif
    }
    spec->argPtr = endPtr;
    return 0;
}
#endif


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART04_H_ */
