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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART05_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART05_H_

#ifndef SECUREC_FOR_WCHAR
#if SECUREC_HAVE_WCHART
SECUREC_INLINE wchar_t SecConvertInputCharToWchar(SecScanSpec *spec, SecFileStream *stream)
{
    wchar_t tempWChar = L'?';   /* Set default char is ? */
#if SECUREC_HAVE_MBTOWC
    char temp[SECUREC_MULTI_BYTE_MAX_LEN + 1];
    temp[0] = (char)spec->ch;
    temp[1] = '\0';
#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
    if (SecIsLeadByte(spec->ch) != 0) {
        spec->ch = SecGetChar(stream, &(spec->charCount));
        temp[1] = (char)spec->ch;
        temp[2] = '\0'; /* 2 of string terminator position */
    }
    if (mbtowc(&tempWChar, temp, sizeof(temp)) <= 0) {
        /* No string termination error for tool */
        tempWChar = L'?';
    }
#else
    if (SecIsLeadByte(spec->ch) != 0) {
        int convRes = 0;
        int di = 1;
        /* On Linux like system, the string is encoded in UTF-8 */
        while (convRes <= 0 && di < (int)MB_CUR_MAX && di < SECUREC_MULTI_BYTE_MAX_LEN) {
            spec->ch = SecGetChar(stream, &(spec->charCount));
            temp[di] = (char)spec->ch;
            ++di;
            temp[di] = '\0';
            convRes = mbtowc(&tempWChar, temp, sizeof(temp));
        }
        if (convRes <= 0) {
            tempWChar = L'?';
        }
    } else {
        if (mbtowc(&tempWChar, temp, sizeof(temp)) <= 0) {
            tempWChar = L'?';
        }
    }
#endif
#else
    (void)spec;      /* To clear e438 last value assigned not used , the compiler will optimize this code */
    (void)stream;    /* To clear e438 last value assigned not used , the compiler will optimize this code */
#endif /* SECUREC_HAVE_MBTOWC */

    return tempWChar;
}
#endif /* SECUREC_HAVE_WCHART */

SECUREC_INLINE int SecInputForChar(SecScanSpec *spec, SecFileStream *stream)
{
    void *endPtr = spec->argPtr;
    if (spec->isWCharOrLong > 0) {
#if SECUREC_HAVE_WCHART
        *(wchar_t UNALIGNED *)endPtr = SecConvertInputCharToWchar(spec, stream);
        endPtr = (wchar_t *)endPtr + 1;
        --spec->arrayWidth;
#else
    (void)stream;    /* To clear e438 last value assigned not used , the compiler will optimize this code */
    return -1;
#endif
    } else {
        *(char *)endPtr = (char)spec->ch;
        endPtr = (char *)endPtr + 1;
        --spec->arrayWidth;
    }
    spec->argPtr = endPtr;
    return 0;
}
#endif

/*
 * Scan digital part of %d %i %o %u %x %p.
 * Return 0 OK
 */
SECUREC_INLINE int SecInputNumberDigital(SecFileStream *stream, SecScanSpec *spec)
{
    static void (* const secFinishNumber[SECUREC_DECODE_NUMBER_FUNC_NUM])(SecScanSpec *spec) = {
        SecFinishNumber, SecFinishNumber64
    };
    while (SECUREC_FILED_WIDTH_ENOUGH(spec)) {
        spec->ch = SecGetChar(stream, &(spec->charCount));
        /* Decode ch to number */
        if (SecDecodeNumber(spec) != 0) {
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
            break;
        }
        SECUREC_FILED_WIDTH_DEC(spec); /* Must be behind un get char, otherwise the logic is incorrect */
        spec->numberState = SECUREC_NUMBER_STATE_STARTED;
    }
    /* Handling integer negative numbers and beyond max */
    (*secFinishNumber[spec->numberArgType])(spec);
    if (spec->numberState == SECUREC_NUMBER_STATE_STARTED) {
        return 0;
    }
    return -1;
}

/*
 * Scan %d %i %o %u %x %p.
 * Return 0 OK
 */
SECUREC_INLINE int SecInputNumber(SecFileStream *stream, SecScanSpec *spec)
{
    /* Character already read */
    if (spec->ch == SECUREC_CHAR('+') || spec->ch == SECUREC_CHAR('-')) {
        if (spec->ch == SECUREC_CHAR('-')) {
            spec->negative = 1;
#if SECUREC_IN_KERNEL
            /* In kernel Refuse to enter negative number */
            if (SECUREC_CONVERT_IS_UNSIGNED(spec->oriConvChr)) {
                return -1;
            }
#endif
        }
        SECUREC_FILED_WIDTH_DEC(spec); /* Do not need to check width here, must be greater than 0 */
        spec->ch = SecGetChar(stream, &(spec->charCount)); /* Eat + or - */
        spec->ch = SecGetChar(stream, &(spec->charCount)); /* Get next character, used for the '0' judgments */
        SecUnGetChar(spec->ch, stream, &(spec->charCount)); /* Not sure if it was actually read, so push back */
    }

    if (spec->oriConvChr == 'i') {
        spec->convChr = 'd'; /* The i could be d, o, or x, use d as default */
    }

    if (spec->ch == SECUREC_CHAR('0') && (spec->oriConvChr == 'x' || spec->oriConvChr == 'i') &&
        SECUREC_FILED_WIDTH_ENOUGH(spec)) {
        /* Input string begin with 0, may be 0x123  0X123  0123  0x  01  0yy  09  0  0ab 00 */
        SECUREC_FILED_WIDTH_DEC(spec);
        spec->ch = SecGetChar(stream, &(spec->charCount)); /* ch is '0' */

        /* Read only '0' due to width limitation */
        if (!SECUREC_FILED_WIDTH_ENOUGH(spec)) {
            /* The number or number64 in spec has been set 0 */
            return 0;
        }

        spec->ch = SecGetChar(stream, &(spec->charCount)); /* Get next char to check x or X, do not dec width */
        if ((SecChar)spec->ch == SECUREC_CHAR('x') || (SecChar)spec->ch == SECUREC_CHAR('X')) {
            spec->convChr = 'x';
            SECUREC_FILED_WIDTH_DEC(spec); /* Make incorrect width for x or X */
        } else {
            if (spec->oriConvChr == 'i') {
                spec->convChr = 'o';
            }
            /* For "0y" "08" "01" "0a" ... ,push the 'y' '8' '1' 'a'  back */
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
            /* Since 0 has been read, it indicates that a valid character has been read */
            spec->numberState = SECUREC_NUMBER_STATE_STARTED;
        }
    }
    return SecInputNumberDigital(stream, spec);
}

/*
 * Scan %c %s %[
 * Return 0 OK
 */
SECUREC_INLINE int SecInputString(SecFileStream *stream, SecScanSpec *spec,
    const SecBracketTable *bracketTable, int *doneCount)
{
    void *startPtr = spec->argPtr;
    int suppressed = 0;
    int errNoMem = 0;

    while (SECUREC_FILED_WIDTH_ENOUGH(spec)) {
        SECUREC_FILED_WIDTH_DEC(spec);
        spec->ch = SecGetChar(stream, &(spec->charCount));
        /*
         * The char condition or string condition and bracket condition.
         * Only supports wide characters with a maximum length of two bytes
         */
        if (spec->ch != SECUREC_EOF && (SecCanInputCharacter(spec->convChr) != 0 ||
            SecCanInputString(spec->convChr, spec->ch) != 0 ||
            SecCanInputForBracket(spec->convChr, spec->ch, bracketTable) != 0)) {
            if (spec->suppress != 0) {
                /* Used to identify processed data for %*, use argPtr to identify will cause 613, so use suppressed */
                suppressed = 1;
                continue;
            }
            /* Now suppress is not set */
            if (spec->arrayWidth == 0) {
                errNoMem = 1; /* We have exhausted the user's buffer */
                break;
            }
#ifdef SECUREC_FOR_WCHAR
            errNoMem = SecInputForWchar(spec);
#else
            errNoMem = SecInputForChar(spec, stream);
#endif
            if (errNoMem != 0) {
                break;
            }
        } else {
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
            break;
        }
    }

    if (errNoMem != 0) {
        /* In case of error, blank out the input buffer */
        SecAddEndingZero(startPtr, spec);
        return -1;
    }
    if ((spec->suppress != 0 && suppressed == 0) ||
        (spec->suppress == 0 && startPtr == spec->argPtr)) {
        /* No input was scanned */
        return -1;
    }
    if (spec->convChr != 'c') {
        /* Add null-terminate for strings */
        SecAddEndingZero(spec->argPtr, spec);
    }
    if (spec->suppress == 0) {
        *doneCount = *doneCount + 1;
    }
    return 0;
}

#ifdef SECUREC_FOR_WCHAR
/*
 * Alloce buffer for wchar version of %[.
 * Return 0 OK
 */
SECUREC_INLINE int SecAllocBracketTable(SecBracketTable *bracketTable)
{
    if (bracketTable->table == NULL) {
        /* Table should be freed after use */
        bracketTable->table = (unsigned char *)SECUREC_MALLOC(SECUREC_BRACKET_TABLE_SIZE);
        if (bracketTable->table == NULL) {
            return -1;
        }
    }
    return 0;
}

/*
 * Free buffer for wchar version of %[
 */
SECUREC_INLINE void SecFreeBracketTable(SecBracketTable *bracketTable)
{
    if (bracketTable->table != NULL) {
        SECUREC_FREE(bracketTable->table);
        bracketTable->table = NULL;
    }
}
#endif

typedef struct {
    SecFileStream *stream;
    SecScanSpec *spec;
    SecBracketTable *bracketTable;
    const SecUnsignedChar **format;
    int *doneCount;
    int *paraIsNull;
    int *formatError;
    int *errRet;
    int *match;
#if SECUREC_ENABLE_SCANF_FLOAT
    SecFloatSpec *floatSpec;
#endif
} SecInputConvCtx;

SECUREC_INLINE int SecInputHandleStringConv(SecInputConvCtx *ctx, va_list argList)
{
    SecUnGetChar(ctx->spec->ch, ctx->stream, &(ctx->spec->charCount));
    if (ctx->spec->suppress == 0) {
        ctx->spec->argPtr = (void *)va_arg(argList, void *);
        if (ctx->spec->argPtr == NULL) {
            *(ctx->paraIsNull) = 1;
            ++(*(ctx->errRet));
            return 1;
        }
        ctx->spec->arrayWidth = SECUREC_GET_ARRAYWIDTH(argList);
        if (SECUREC_ARRAY_WIDTH_IS_WRONG((*(ctx->spec)))) {
            ++(*(ctx->errRet));
            return 1;
        }
        if (ctx->spec->convChr != 'c') {
            --(ctx->spec->arrayWidth);
        }
    } else {
        ctx->spec->argPtr = NULL;
    }

    if (ctx->spec->convChr == SECUREC_BRACE) {
#ifdef SECUREC_FOR_WCHAR
        if (SecAllocBracketTable(ctx->bracketTable) != 0) {
            ++(*(ctx->errRet));
            return 1;
        }
#endif
        (void)SECUREC_MEMSET_FUNC_OPT(ctx->bracketTable->table, 0, (size_t)SECUREC_BRACKET_TABLE_SIZE);
        if (SecSetupBracketTable(ctx->format, ctx->bracketTable) != 0) {
            ++(*(ctx->errRet));
            return 1;
        }
        if (**(ctx->format) == SECUREC_CHAR('\0')) {
            SecAddEndingZero(ctx->spec->argPtr, ctx->spec);
            ++(*(ctx->errRet));
            return 1;
        }
    }

    if (SecInputString(ctx->stream, ctx->spec, ctx->bracketTable, ctx->doneCount) != 0) {
        ++(*(ctx->errRet));
        return 1;
    }
    return 0;
}

SECUREC_INLINE int SecInputHandleNumberConv(SecInputConvCtx *ctx, va_list argList)
{
    SecUnGetChar(ctx->spec->ch, ctx->stream, &(ctx->spec->charCount));
    if (SecInputNumber(ctx->stream, ctx->spec) != 0) {
        ++(*(ctx->errRet));
        return 1;
    }
    if (ctx->spec->suppress == 0) {
        ctx->spec->argPtr = (void *)va_arg(argList, void *);
        if (ctx->spec->argPtr == NULL) {
            *(ctx->paraIsNull) = 1;
            ++(*(ctx->errRet));
            return 1;
        }
        SecAssignNumber(ctx->spec);
        ++(*(ctx->doneCount));
    }
    return 0;
}

SECUREC_INLINE int SecInputHandleCountConv(SecScanSpec *spec, va_list argList, int *paraIsNull, int *errRet)
{
    if (spec->suppress == 0) {
        spec->argPtr = (void *)va_arg(argList, void *);
        if (spec->argPtr == NULL) {
            *paraIsNull = 1;
            ++(*errRet);
            return 1;
        }
        spec->number = (unsigned long)(unsigned int)(spec->charCount);
        spec->numberArgType = 0;
        SecAssignNumber(spec);
    }
    return 0;
}

SECUREC_INLINE int SecInputHandleFloatConv(SecInputConvCtx *ctx, va_list argList)
{
    SecUnGetChar(ctx->spec->ch, ctx->stream, &(ctx->spec->charCount));
#if SECUREC_ENABLE_SCANF_FLOAT
    if (SecInputFloat(ctx->stream, ctx->spec, ctx->floatSpec) != 0) {
        ++(*(ctx->errRet));
        return 1;
    }
    if (ctx->spec->suppress == 0) {
        ctx->spec->argPtr = (void *)va_arg(argList, void *);
        if (ctx->spec->argPtr == NULL) {
            ++(*(ctx->errRet));
            *(ctx->paraIsNull) = 1;
            return 1;
        }
        if (SecAssignFloat(ctx->floatSpec, ctx->spec) != 0) {
            ++(*(ctx->errRet));
            return 1;
        }
        ++(*(ctx->doneCount));
    }
#else
    (void)argList;
    ++(*(ctx->errRet));
#endif
    return 0;
}

SECUREC_INLINE int SecInputHandleDefaultConv(SecInputConvCtx *ctx)
{
    if ((int)(**(ctx->format)) != (int)ctx->spec->ch) {
        SecUnGetChar(ctx->spec->ch, ctx->stream, &(ctx->spec->charCount));
        *(ctx->formatError) = 1;
        ++(*(ctx->errRet));
        return 1;
    }
    --(*(ctx->match));
    return 0;
}

SECUREC_INLINE int SecInputHandleConversion(SecInputConvCtx *ctx, va_list argList)
{
    switch (ctx->spec->oriConvChr) {
        case 'c':
            if (ctx->spec->widthSet == 0) {
                ctx->spec->widthSet = 1;
                ctx->spec->width = 1;
            }
            /* fall-through */ /* FALLTHRU */
        case 's': /* fall-through */ /* FALLTHRU */
        case SECUREC_BRACE:
            return SecInputHandleStringConv(ctx, argList);
        case 'p':
            ctx->spec->numberWidth = SECUREC_NUM_WIDTH_INT;
#ifdef SECUREC_ON_64BITS
            ctx->spec->numberArgType = 1;
#endif
            /* fall-through */ /* FALLTHRU */
        case 'o': /* fall-through */ /* FALLTHRU */
        case 'u': /* fall-through */ /* FALLTHRU */
        case 'd': /* fall-through */ /* FALLTHRU */
        case 'i': /* fall-through */ /* FALLTHRU */
        case 'x':
            return SecInputHandleNumberConv(ctx, argList);
        case 'n':
            return SecInputHandleCountConv(ctx->spec, argList, ctx->paraIsNull, ctx->errRet);
        case 'e': /* fall-through */ /* FALLTHRU */
        case 'f': /* fall-through */ /* FALLTHRU */
        case 'g':
            return SecInputHandleFloatConv(ctx, argList);
        default:
            return SecInputHandleDefaultConv(ctx);
    }
}

#define SECUREC_INPUT_NEXT    0
#define SECUREC_INPUT_BREAK   1
#define SECUREC_INPUT_PERCENT 2

static int SecInputHandleLiteralFormat(SecFileStream *stream, SecScanSpec *spec, const SecUnsignedChar **format)
{
    if (SecIsSpace((SecInt)(int)(**format)) != 0) {
        spec->ch = SecSkipSpaceChar(stream, &(spec->charCount));


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART05_H_ */
