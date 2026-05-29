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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART06_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART06_H_

        SecUnGetChar(spec->ch, stream, &(spec->charCount));
        SecSkipSpaceFormat(format);
        return SECUREC_INPUT_NEXT;
    }

    if (**format != SECUREC_CHAR('%')) {
        spec->ch = SecGetChar(stream, &(spec->charCount));
        if ((int)(**format) != (int)(spec->ch)) {
            SecUnGetChar(spec->ch, stream, &(spec->charCount));
            return SECUREC_INPUT_BREAK;
        }
        ++(*format);
#if !defined(SECUREC_FOR_WCHAR) && defined(SECUREC_COMPATIBLE_VERSION)
        if (SecFilterWcharInFormat(spec, format, stream) != 0) {
            return SECUREC_INPUT_BREAK;
        }
#endif
        return SECUREC_INPUT_NEXT;
    }

    return SECUREC_INPUT_PERCENT;
}

static int SecInputPrepareConversion(SecFileStream *stream, SecScanSpec *spec, const SecUnsignedChar **format,
    int *formatError, int *errRet)
{
    SecSetDefaultScanSpec(spec);
    if (SecDecodeScanFlag(format, spec) != 0) {
        *formatError = 1;
        ++(*errRet);
        return SECUREC_INPUT_NEXT;
    }
    if (!SECUREC_FILED_WIDTH_ENOUGH(spec)) {
        ++(*errRet);
        return SECUREC_INPUT_NEXT;
    }
    SecUpdateWcharFlagByType(**format, spec);
    spec->convChr = SECUREC_TO_LOWERCASE(**format);
    spec->oriConvChr = spec->convChr;
    if (spec->convChr != 'n') {
        if (spec->convChr != 'c' && spec->convChr != SECUREC_BRACE) {
            spec->ch = SecSkipSpaceChar(stream, &(spec->charCount));
        } else {
            spec->ch = SecGetChar(stream, &(spec->charCount));
        }
        if (spec->ch == SECUREC_EOF) {
            ++(*errRet);
            return SECUREC_INPUT_NEXT;
        }
    }

    return SECUREC_INPUT_PERCENT;
}

static int SecInputSCoreDone(SecInputConvCtx *ctx)
{
#ifdef SECUREC_FOR_WCHAR
    SecFreeBracketTable(ctx->bracketTable);
#endif
#if SECUREC_ENABLE_SCANF_FLOAT
    SecFreeFloatSpec(ctx->floatSpec, ctx->doneCount);
#endif
#if SECUREC_ENABLE_SCANF_FILE
    SecAdjustStream(ctx->stream);
#endif
    if (ctx->spec->ch == SECUREC_EOF) {
        return ((*(ctx->doneCount) != 0 || *(ctx->match) != 0) ? *(ctx->doneCount) : SECUREC_SCANF_EINVAL);
    }
    if (*(ctx->formatError) != 0 || *(ctx->paraIsNull) != 0) {
        return SECUREC_SCANF_ERROR_PARA;
    }
    return *(ctx->doneCount);
}

static int SecInputProcessPercent(SecInputConvCtx *convCtx, va_list argList)
{
    if (SecInputPrepareConversion(convCtx->stream, convCtx->spec, convCtx->format,
        convCtx->formatError, convCtx->errRet) != SECUREC_INPUT_PERCENT) {
        return SECUREC_INPUT_NEXT;
    }
    if (SecInputHandleConversion(convCtx, argList) != 0) {
        return SECUREC_INPUT_NEXT;
    }
    ++(*(convCtx->match));
    ++(*(convCtx->format));
    return SECUREC_INPUT_NEXT;
}

#ifdef SECUREC_FOR_WCHAR
/*
 *  Formatting input core functions for wchar version.Called by a function such as vswscanf_s
 */
static int SecInputSWCore(SecFileStream *stream, const wchar_t *cFormat, va_list argList)
#else
/*
 * Formatting input core functions for char version.Called by a function such as vsscanf_s
 */
static int SecInputSCore(SecFileStream *stream, const char *cFormat, va_list argList)
#endif
{
    const SecUnsignedChar *format = (const SecUnsignedChar *)cFormat;
    SecBracketTable bracketTable = SECUREC_INIT_BRACKET_TABLE;
    SecScanSpec spec;
    int doneCount = 0;
    int formatError = 0;
    int paraIsNull = 0;
    int match = 0; /* When % is found , inc this value */
    int errRet = 0;
    int action;
#if SECUREC_ENABLE_SCANF_FLOAT
    SecFloatSpec floatSpec;
    SecInputConvCtx convCtx = {stream, &spec, &bracketTable, &format, &doneCount, &paraIsNull, &formatError,
        &errRet, &match, &floatSpec};
    SecInitFloatSpec(&floatSpec);
#else
    SecInputConvCtx convCtx = {stream, &spec, &bracketTable, &format, &doneCount, &paraIsNull, &formatError,
        &errRet, &match};
#endif
    spec.ch = 0; /* Need to initialize to 0 */
    spec.charCount = 0; /* Need to initialize to 0 */

    /* Format must not NULL, use err < 1 to claer 845 */
    while (errRet < 1 && *format != SECUREC_CHAR('\0')) {
        action = SecInputHandleLiteralFormat(stream, &spec, &format);
        if (action == SECUREC_INPUT_BREAK) {
            break;
        }
        if (action == SECUREC_INPUT_NEXT) {
            continue;
        }
        if (SecInputProcessPercent(&convCtx, argList) == SECUREC_INPUT_NEXT) {
            continue;
        }
    }

#if SECUREC_ENABLE_SCANF_FLOAT
    return SecInputSCoreDone(&convCtx);
#else
    return SecInputSCoreDone(&convCtx);
#endif
}

#ifdef SECUREC_FOR_WCHAR
int SecInputSW(SecFileStream *stream, const wchar_t *cFormat, va_list argList)
{
    return SecInputSWCore(stream, cFormat, argList);
}
#else
int SecInputS(SecFileStream *stream, const char *cFormat, va_list argList)
{
    return SecInputSCore(stream, cFormat, argList);
}
#endif

#if SECUREC_ENABLE_SCANF_FILE
/*
 *  Get char from stream use std function
 */
SECUREC_INLINE SecInt SecGetCharFromStream(const SecFileStream *stream)
{
    SecInt ch;
    ch = SECUREC_GETC(stream->pf);
    return ch;
}

/*
 * Try to read the BOM header, when meet a BOM head, discard it, then data is Aligned to base
 */
SECUREC_INLINE void SecReadAndSkipBomHeader(SecFileStream *stream)
{
    /* Use size_t type conversion  to clean e747 */
    stream->count = fread(stream->base, (size_t)1, (size_t)SECUREC_BOM_HEADER_SIZE, stream->pf);
    if (stream->count > SECUREC_BOM_HEADER_SIZE) {
        stream->count = 0;
    }
    if (SECUREC_BEGIN_WITH_BOM(stream->base, stream->count)) {
        /* It's BOM header, discard it */
        stream->count = 0;
    }
}

/*
 *  Get char  from file stream or buffer
 */
SECUREC_INLINE SecInt SecGetCharFromFile(SecFileStream *stream)
{
    SecInt ch;
    if (stream->count < sizeof(SecChar)) {
        /* Load file to buffer */
        size_t len;
        if (stream->base != NULL) {
            /* Put the last unread data in the buffer head */
            for (len = 0; len < stream->count; ++len) {
                stream->base[len] = stream->cur[len];
            }
        } else {
            stream->oriFilePos = ftell(stream->pf);   /* Save original file read position */
            if (stream->oriFilePos == -1) {
                /* It may be a pipe stream */
                stream->flag = SECUREC_PIPE_STREAM_FLAG;
                return SecGetCharFromStream(stream);
            }
            /* Reserve the length of BOM head */
            stream->base = (char *)SECUREC_MALLOC(SECUREC_BUFFERED_BLOK_SIZE +
                SECUREC_BOM_HEADER_SIZE + sizeof(SecChar)); /* To store '\0' and aligned to wide char */
            if (stream->base == NULL) {
                return SECUREC_EOF;
            }
            /* First read file */
            if (stream->oriFilePos == 0) {
                /* Make sure the data is aligned to base */
                SecReadAndSkipBomHeader(stream);
            }
        }

        /* Skip existing data and read data */
        len = fread(stream->base + stream->count, (size_t)1, (size_t)SECUREC_BUFFERED_BLOK_SIZE, stream->pf);
        if (len > SECUREC_BUFFERED_BLOK_SIZE) { /* It won't happen, */
            len = 0;
        }
        stream->count += len;
        stream->cur = stream->base;
        stream->flag |= SECUREC_LOAD_FILE_TO_MEM_FLAG;
        stream->base[stream->count] = '\0';   /* For tool Warning string null */
    }

    SECUREC_GET_CHAR(stream, &ch);
    if (ch != SECUREC_EOF) {
        stream->fileRealRead += sizeof(SecChar);
    }
    return ch;
}
#endif

/*
 *  Get char  for wchar version
 */
SECUREC_INLINE SecInt SecGetChar(SecFileStream *stream, int *counter)
{
    *counter = *counter + 1; /* Always plus 1 */
    /* The main scenario is scanf str */
    if ((stream->flag & SECUREC_MEM_STR_FLAG) != 0) {
        SecInt ch;
        SECUREC_GET_CHAR(stream, &ch);
        return ch;
    }
#if SECUREC_ENABLE_SCANF_FILE
    if ((stream->flag & SECUREC_FILE_STREAM_FLAG) != 0) {
        return SecGetCharFromFile(stream);
    }
    if ((stream->flag & SECUREC_PIPE_STREAM_FLAG) != 0) {
        return SecGetCharFromStream(stream);
    }
#endif
    return SECUREC_EOF;
}

/*
 *  Unget Public realizatio char for wchar and char version
 */
SECUREC_INLINE void SecUnGetCharImpl(SecInt ch, SecFileStream *stream)
{
    if ((stream->flag & SECUREC_MEM_STR_FLAG) != 0) {
        SECUREC_UN_GET_CHAR(stream);
        return;
    }
#if SECUREC_ENABLE_SCANF_FILE
    if ((stream->flag & SECUREC_LOAD_FILE_TO_MEM_FLAG) != 0) {
        SECUREC_UN_GET_CHAR(stream);
        if (stream->fileRealRead > 0) {
            stream->fileRealRead -= sizeof(SecChar);
        }
        return;
    }
    if ((stream->flag & SECUREC_PIPE_STREAM_FLAG) != 0) {
        (void)SECUREC_UN_GETC(ch, stream->pf);
        return;
    }
#else
    (void)ch; /* To clear e438 last value assigned not used , the compiler will optimize this code */
#endif
}

/*
 *  Unget char  for char version
 */
SECUREC_INLINE void SecUnGetChar(SecInt ch, SecFileStream *stream, int *counter)
{
    *counter = *counter - 1; /* Always mius 1 */
    if (ch != SECUREC_EOF) {
        SecUnGetCharImpl(ch, stream);
    }
}

/*
 *  Skip space char by isspace
 */
SECUREC_INLINE SecInt SecSkipSpaceChar(SecFileStream *stream, int *counter)
{
    SecInt ch;
    do {
        ch = SecGetChar(stream, counter);
        if (ch == SECUREC_EOF) {
            break;
        }
    } while (SecIsSpace(ch) != 0);
    return ch;
}


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_INPUT_INL_BODY_PART06_H_ */
