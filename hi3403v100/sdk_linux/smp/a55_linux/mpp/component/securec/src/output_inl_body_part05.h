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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART05_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART05_H_

    formatAttr->textLen = 0;
    formatAttr->text.str = NULL;
    formatAttr->prefixLen = 0;
    formatAttr->prefix[0] = SECUREC_CHAR('\0');
    formatAttr->prefix[1] = SECUREC_CHAR('\0');
}

SECUREC_INLINE int SecOutputProcessState(SecOutputStateCtx *ctx, va_list argList)
{
    int noOutput = 0;

    switch (*(ctx->state)) {
        case STAT_NORMAL:
            SecWriteChar(ctx->stream, ctx->ch, ctx->charsOut);
            return 1;
        case STAT_PERCENT:
            SecOutputResetFormatAttr(ctx->formatAttr, &noOutput);
            break;
        case STAT_FLAG:
            SecDecodeFlags(ctx->ch, ctx->formatAttr);
            break;
        case STAT_WIDTH:
            if (SecOutputHandleWidth(ctx->ch, ctx->formatAttr, ctx->lastState, argList) != 0) {
                return -1;
            }
            break;
        case STAT_DOT:
            ctx->formatAttr->precision = 0;
            break;
        case STAT_PRECIS:
            if (SecOutputHandlePrecision(ctx->ch, ctx->formatAttr, argList) != 0) {
                return -1;
            }
            break;
        case STAT_SIZE:
            return SecOutputHandleSize(ctx) != 0 ? 1 : 0;
        case STAT_TYPE:
            return SecOutputHandleStateType(ctx, argList);
        case STAT_INVALID: /* fall-through */ /* FALLTHRU */
        default:
            return -1;
    }
    return 0;
}

SECUREC_INLINE int SecOutput(SecPrintfStream *stream, const SecChar *cFormat, va_list argList)
{
    const SecChar *format = cFormat;
    int charsOut = 0;
    SecFmtState state = STAT_NORMAL;
    SecFormatAttr formatAttr;
    SecOutputStateCtx stateCtx;

    SecOutputInitFormatAttr(&formatAttr);
    stateCtx.stream = stream;
    stateCtx.formatAttr = &formatAttr;
    stateCtx.format = &format;
    stateCtx.state = &state;
    stateCtx.charsOut = &charsOut;

    /* Loop each format character */
    while (*format != SECUREC_CHAR('\0') && charsOut >= 0) {
        SecFmtState lastState = state;
        SecChar ch = *format; /* Currently read character */
        ++format;
        state = SecDecodeState(ch, lastState);
        stateCtx.lastState = lastState;
        stateCtx.ch = ch;
        {
            int stateRet = SecOutputProcessState(&stateCtx, argList);
            if (stateRet < 0) {
                return -1;
            }
            if (stateRet > 0) {
                continue;
            }
        }
    }

    if (state != STAT_NORMAL && state != STAT_TYPE) {
        return -1;
    }

    return charsOut;            /* The number of characters written */
}

/*
 * Output one zero character zero into the SecPrintfStream structure
 * If there is not enough space, make sure f->count is less than 0
 */
SECUREC_INLINE int SecPutZeroChar(SecPrintfStream *stream)
{
    --stream->count;
    if (stream->count >= 0) {
        *(stream->cur) = SECUREC_CHAR('\0');
        ++stream->cur;
        return 0;
    }
    return -1;
}

/*
 * Multi character formatted output implementation
 */
#ifdef SECUREC_FOR_WCHAR
int SecVswprintfImpl(wchar_t *string, size_t count, const wchar_t *format, va_list argList)
#else
int SecVsnprintfImpl(char *string, size_t count, const char *format, va_list argList)
#endif
{
    SecPrintfStream stream;
    int retVal;

    stream.count = (int)count; /* The count include \0 character, must be greater than zero */
    stream.cur = string;

    retVal = SecOutput(&stream, format, argList);
    if (retVal >= 0) {
        if (SecPutZeroChar(&stream) == 0) {
            return retVal;
        }
    }
    if (stream.count < 0) {
        /* The buffer was too small, then truncate */
        string[count - 1] = SECUREC_CHAR('\0');
        return SECUREC_PRINTF_TRUNCATE;
    }
    string[0] = SECUREC_CHAR('\0'); /* Empty the dest string */
    return -1;
}


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART05_H_ */
