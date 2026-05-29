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

#ifndef SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART03_H_
#define SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART03_H_

        0x08, 0x08, 0x00, 0x00, 0x00, 0x02, 0x02, 0x03, 0x05, 0x05, 0x08, 0x00, 0x00, 0x00, 0x03, 0x03,
        0x03, 0x05, 0x05, 0x08, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00,
        0x00
    };

#ifdef SECUREC_FOR_WCHAR
    /* Convert to unsigned char to clear gcc 4.3.4 warning */
    unsigned char fmtType = (unsigned char)((((unsigned int)(int)(ch)) <= (unsigned int)(int)(L'~')) ? \
        (stateTable[(unsigned char)(ch)]) : 0);
    return (SecFmtState)(stateTable[fmtType * ((unsigned char)STAT_INVALID + 1) +
        (unsigned char)(lastState) + SECUREC_FMT_STATE_OFFSET]);
#else
    unsigned char fmtType = stateTable[(unsigned char)(ch)];
    return (SecFmtState)(stateTable[fmtType * ((unsigned char)STAT_INVALID + 1) +
        (unsigned char)(lastState) + SECUREC_FMT_STATE_OFFSET]);
#endif
}

SECUREC_INLINE void SecDecodeFlags(SecChar ch, SecFormatAttr *attr)
{
    switch (ch) {
        case SECUREC_CHAR(' '):
            attr->flags |= SECUREC_FLAG_SIGN_SPACE;
            break;
        case SECUREC_CHAR('+'):
            attr->flags |= SECUREC_FLAG_SIGN;
            break;
        case SECUREC_CHAR('-'):
            attr->flags |= SECUREC_FLAG_LEFT;
            break;
        case SECUREC_CHAR('0'):
            attr->flags |= SECUREC_FLAG_LEADZERO;   /* Add zero th the front */
            break;
        case SECUREC_CHAR('#'):
            attr->flags |= SECUREC_FLAG_ALTERNATE;  /* Output %x with 0x */
            break;
        default:
            /* Do nothing */
            break;
    }
    return;
}

/*
 * Decoded size identifier in format string to Reduce the number of lines of function code
 */
SECUREC_INLINE int SecDecodeSizeI(SecFormatAttr *attr, const SecChar **format)
{
#ifdef SECUREC_ON_64BITS
    attr->flags |= SECUREC_FLAG_I64;    /* %I  to  INT64 */
#endif
    if ((**format == SECUREC_CHAR('6')) && (*((*format) + 1) == SECUREC_CHAR('4'))) {
        (*format) += 2; /* Add 2 to skip I64 */
        attr->flags |= SECUREC_FLAG_I64;    /* %I64  to  INT64 */
    } else if ((**format == SECUREC_CHAR('3')) && (*((*format) + 1) == SECUREC_CHAR('2'))) {
        (*format) += 2; /* Add 2 to skip I32 */
        attr->flags &= ~SECUREC_FLAG_I64;   /* %I64  to  INT32 */
    } else if ((**format == SECUREC_CHAR('d')) || (**format == SECUREC_CHAR('i')) ||
        (**format == SECUREC_CHAR('o')) || (**format == SECUREC_CHAR('u')) ||
        (**format == SECUREC_CHAR('x')) || (**format == SECUREC_CHAR('X'))) {
        /* Do nothing */
    } else {
        /* Compatibility  code for "%I" just print I */
        return -1;
    }
    return 0;
}

/*
 * Decoded size identifier in format string, and skip format to next charater
 */
SECUREC_INLINE int SecDecodeSize(SecChar ch, SecFormatAttr *attr, const SecChar **format)
{
    switch (ch) {
        case SECUREC_CHAR('l'):
            if (**format == SECUREC_CHAR('l')) {
                *format = *format + 1;
                attr->flags |= SECUREC_FLAG_LONGLONG; /* For long long */
            } else {
                attr->flags |= SECUREC_FLAG_LONG;     /* For long int or wchar_t */
            }
            break;
#ifdef SECUREC_COMPATIBLE_LINUX_FORMAT
        case SECUREC_CHAR('z'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('Z'):
            attr->flags |= SECUREC_FLAG_SIZE;
            break;
        case SECUREC_CHAR('j'):
            attr->flags |= SECUREC_FLAG_INTMAX;
            break;
#endif
        case SECUREC_CHAR('t'):
            attr->flags |= SECUREC_FLAG_PTRDIFF;
            break;
        case SECUREC_CHAR('q'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('L'):
            attr->flags |= (SECUREC_FLAG_LONGLONG | SECUREC_FLAG_LONG_DOUBLE);
            break;
        case SECUREC_CHAR('I'):
            if (SecDecodeSizeI(attr, format) != 0) {
                /* Compatibility  code for "%I" just print I */
                return -1;
            }
            break;
        case SECUREC_CHAR('h'):
            if (**format == SECUREC_CHAR('h')) {
                *format = *format + 1;
                attr->flags |= SECUREC_FLAG_CHAR;   /* For char */
            } else {
                attr->flags |= SECUREC_FLAG_SHORT;  /* For short int */
            }
            break;
        case SECUREC_CHAR('w'):
            attr->flags |= SECUREC_FLAG_WIDECHAR;   /* For wide char */
            break;
        default:
            /* Do nothing */
            break;
    }
    return 0;
}

/*
 * Decoded char type identifier
 */
SECUREC_INLINE void SecDecodeTypeC(SecFormatAttr *attr, unsigned int c)
{
    attr->textLen = 1; /* Only 1 wide character */

#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT)) && !(defined(__hpux)) && !(defined(SECUREC_ON_SOLARIS))
    attr->flags &= ~SECUREC_FLAG_LEADZERO;
#endif

#ifdef SECUREC_FOR_WCHAR
    if ((attr->flags & SECUREC_FLAG_SHORT) != 0) {
        /* Get  multibyte character from argument */
        attr->buffer.str[0] = (char)c;
        attr->text.str = attr->buffer.str;
        attr->textIsWide = 0;
    } else {
        attr->buffer.wStr[0] = (wchar_t)c;
        attr->text.wStr = attr->buffer.wStr;
        attr->textIsWide = 1;
    }
#else /* Not SECUREC_FOR_WCHAR */
    if ((attr->flags & (SECUREC_FLAG_LONG | SECUREC_FLAG_WIDECHAR)) != 0) {
#if SECUREC_HAVE_WCHART
        attr->buffer.wStr[0] = (wchar_t)c;
        attr->text.wStr = attr->buffer.wStr;
        attr->textIsWide = 1;
#else
        attr->textLen = 0; /* Ignore unsupported characters */
        attr->fldWidth = 0; /* No paddings  */
#endif
    } else {
        /* Get  multibyte character from argument */
        attr->buffer.str[0] = (char)c;
        attr->text.str = attr->buffer.str;
        attr->textIsWide = 0;
    }
#endif
}

#ifdef SECUREC_FOR_WCHAR
#define SECUREC_IS_NARROW_STRING(attr) (((attr)->flags & SECUREC_FLAG_SHORT) != 0)
#else
#define SECUREC_IS_NARROW_STRING(attr) (((attr)->flags & (SECUREC_FLAG_LONG | SECUREC_FLAG_WIDECHAR)) == 0)
#endif

SECUREC_INLINE void SecDecodeTypeSchar(SecFormatAttr *attr)
{
    size_t textLen;
    if (attr->text.str == NULL) {
        /*
         * Literal string to print null ptr, define it as array rather than const text area
         * To avoid gcc warning with pointing const text with variable
         */
        static char strNullString[SECUREC_NULL_STRING_SIZE] = "(null)";
        attr->text.str = strNullString;
    }
    if (attr->precision == -1) {
        /* Precision NOT assigned */
        /* The strlen performance is high when the string length is greater than 32 */
        textLen = strlen(attr->text.str);
        if (textLen > SECUREC_STRING_MAX_LEN) {
            textLen = 0;
        }
    } else {
        /* Precision assigned */
        SECUREC_CALC_STR_LEN(attr->text.str, (size_t)(unsigned int)attr->precision, &textLen);
    }
    attr->textLen = (int)textLen;
}

SECUREC_INLINE void SecDecodeTypeSwchar(SecFormatAttr *attr)
{
#if SECUREC_HAVE_WCHART
    size_t textLen;
    attr->textIsWide = 1;
    if (attr->text.wStr == NULL) {
        /*
         * Literal string to print null ptr, define it as array rather than const text area
         * To avoid gcc warning with pointing const text with variable
         */
        static wchar_t wStrNullString[SECUREC_NULL_STRING_SIZE] = { L'(', L'n', L'u', L'l', L'l', L')', L'\0', L'\0' };
        attr->text.wStr = wStrNullString;
    }
    /* The textLen in wchar_t,when precision is -1, it is unlimited  */
    SECUREC_CALC_WSTR_LEN(attr->text.wStr, (size_t)(unsigned int)attr->precision, &textLen);
    if (textLen > SECUREC_WCHAR_STRING_MAX_LEN) {
        textLen = 0;
    }
    attr->textLen = (int)textLen;
#else
    attr->textLen = 0;
#endif
}

/*
 * Decoded string identifier
 */
SECUREC_INLINE void SecDecodeTypeS(SecFormatAttr *attr, char *argPtr)
{
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT))
#if (!defined(SECUREC_ON_UNIX))
    attr->flags &= ~SECUREC_FLAG_LEADZERO;
#endif
#if (defined(SECUREC_FOR_WCHAR))
    if ((attr->flags & SECUREC_FLAG_LONG) == 0) {
        attr->flags |= SECUREC_FLAG_SHORT;
    }
#endif
#endif
    attr->text.str = argPtr;
    if (SECUREC_IS_NARROW_STRING(attr)) {
        /* The textLen now contains length in multibyte chars */
        SecDecodeTypeSchar(attr);
    } else {
        /* The textLen now contains length in wide chars */
        SecDecodeTypeSwchar(attr);
    }
}

/*
 * Check precision in format
 */
SECUREC_INLINE int SecDecodePrecision(SecChar ch, SecFormatAttr *attr)
{
    if (attr->dynPrecision == 0) {
        /* Add digit to current precision */
        if (SECUREC_MUL_TEN_ADD_BEYOND_MAX(attr->precision)) {
            return -1;
        }
        attr->precision = (int)SECUREC_MUL_TEN((unsigned int)attr->precision) +
            (unsigned char)(ch - SECUREC_CHAR('0'));
    } else {
        if (attr->precision < 0) {
            attr->precision = -1;
        }
        if (attr->precision > SECUREC_MAX_WIDTH_LEN) {
            return -1;
        }
    }
    return 0;
}

/*
 * Check width in format
 */
SECUREC_INLINE int SecDecodeWidth(SecChar ch, SecFormatAttr *attr, SecFmtState lastState)
{
    if (attr->dynWidth == 0) {
        if (lastState != STAT_WIDTH) {
            attr->fldWidth = 0;
        }
        if (SECUREC_MUL_TEN_ADD_BEYOND_MAX(attr->fldWidth)) {
            return -1;
        }
        attr->fldWidth = (int)SECUREC_MUL_TEN((unsigned int)attr->fldWidth) +
            (unsigned char)(ch - SECUREC_CHAR('0'));
    } else {
        if (attr->fldWidth < 0) {
            attr->flags |= SECUREC_FLAG_LEFT;
            attr->fldWidth = (-attr->fldWidth);
        }
        if (attr->fldWidth > SECUREC_MAX_WIDTH_LEN) {
            return -1;
        }
    }
    return 0;
}

/*
 * The sprintf_s function processes the wide character as a parameter for %C
 * The swprintf_s function processes the multiple character as a parameter for %C
 */
SECUREC_INLINE void SecUpdateWcharFlags(SecFormatAttr *attr)
{
    if ((attr->flags & (SECUREC_FLAG_SHORT | SECUREC_FLAG_LONG | SECUREC_FLAG_WIDECHAR)) == 0) {
#ifdef SECUREC_FOR_WCHAR
        attr->flags |= SECUREC_FLAG_SHORT;
#else
        attr->flags |= SECUREC_FLAG_WIDECHAR;
#endif
    }
}
/*
 * When encountering %S, current just same as %C
 */
SECUREC_INLINE void SecUpdateWstringFlags(SecFormatAttr *attr)
{
    SecUpdateWcharFlags(attr);
}

#if SECUREC_IN_KERNEL
SECUREC_INLINE void SecUpdatePointFlagsForKernel(SecFormatAttr *attr)
{
    /* Width is not set */
    if (attr->fldWidth <= 0) {
        attr->flags |= SECUREC_FLAG_LEADZERO;
        attr->fldWidth = 2 * sizeof(void *);  /* 2 x byte number is the length of hex */
    }
    if ((attr->flags & SECUREC_FLAG_ALTERNATE) != 0) {
        /* Alternate form means '0x' prefix */
        attr->prefix[0] = SECUREC_CHAR('0');
        attr->prefix[1] = SECUREC_CHAR('x');
        attr->prefixLen = SECUREC_PREFIX_LEN;
    }
    attr->flags |= SECUREC_FLAG_LONG;  /* Converting a long */
}
#endif

SECUREC_INLINE void SecUpdatePointFlags(SecFormatAttr *attr)
{
    attr->flags |= SECUREC_FLAG_POINTER;
#if SECUREC_IN_KERNEL
    SecUpdatePointFlagsForKernel(attr);
#else
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) || defined(SECUREC_VXWORKS_PLATFORM)) && (!defined(SECUREC_ON_UNIX))
#if defined(SECUREC_VXWORKS_PLATFORM)
    attr->precision = 1;
#else
    attr->precision = 0;
#endif
    attr->flags |= SECUREC_FLAG_ALTERNATE; /* "0x" is not default prefix in UNIX */
    attr->digits = g_itoaLowerDigits;
#else /* On unix or win */
#if defined(_AIX) || defined(SECUREC_ON_SOLARIS)
    attr->precision = 1;
#else
    attr->precision = 2 * sizeof(void *);  /* 2 x byte number is the length of hex */
#endif
#if defined(SECUREC_ON_UNIX)
    attr->digits = g_itoaLowerDigits;
#else
    attr->digits = g_itoaUpperDigits;
#endif
#endif

#if defined(SECUREC_COMPATIBLE_WIN_FORMAT)
    attr->flags &= ~SECUREC_FLAG_LEADZERO;
#endif

#ifdef SECUREC_ON_64BITS
    attr->flags |= SECUREC_FLAG_I64;   /* Converting an int64 */
#else
    attr->flags |= SECUREC_FLAG_LONG;  /* Converting a long */
#endif
    /* Set up for %#p on different system */
    if ((attr->flags & SECUREC_FLAG_ALTERNATE) != 0) {
        /* Alternate form means '0x' prefix */
        attr->prefix[0] = SECUREC_CHAR('0');
#if (defined(SECUREC_COMPATIBLE_LINUX_FORMAT) || defined(SECUREC_VXWORKS_PLATFORM))
        attr->prefix[1] = SECUREC_CHAR('x');
#else
        attr->prefix[1] = (SecChar)(attr->digits[SECUREC_NUMBER_OF_X]);
#endif
#if defined(_AIX) || defined(SECUREC_ON_SOLARIS)
        attr->prefixLen = 0;
#else
        attr->prefixLen = SECUREC_PREFIX_LEN;
#endif
    }
#endif
}

SECUREC_INLINE void SecUpdateXpxFlags(SecFormatAttr *attr, SecChar ch)
{
    /* Use unsigned lower hex output for 'x' */
    attr->digits = g_itoaLowerDigits;
    attr->radix = SECUREC_RADIX_HEX;
    switch (ch) {
        case SECUREC_CHAR('p'):
            /* Print a pointer */
            SecUpdatePointFlags(attr);
            break;
        case SECUREC_CHAR('X'): /* fall-through */ /* FALLTHRU */
            /* Unsigned upper hex output */
            attr->digits = g_itoaUpperDigits;
            /* fall-through */ /* FALLTHRU */
        default:
            /* For %#x or %#X */
            if ((attr->flags & SECUREC_FLAG_ALTERNATE) != 0) {
                /* Alternate form means '0x' prefix */
                attr->prefix[0] = SECUREC_CHAR('0');
                attr->prefix[1] = (SecChar)(attr->digits[SECUREC_NUMBER_OF_X]);
                attr->prefixLen = SECUREC_PREFIX_LEN;
            }
            break;
    }
}

SECUREC_INLINE void SecUpdateOudiFlags(SecFormatAttr *attr, SecChar ch)
{
    /* Do not set digits here */
    switch (ch) {
        case SECUREC_CHAR('i'): /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('d'): /* fall-through */ /* FALLTHRU */
            /* For signed decimal output */
            attr->flags |= SECUREC_FLAG_SIGNED;
            /* fall-through */ /* FALLTHRU */
        case SECUREC_CHAR('u'):
            attr->radix = SECUREC_RADIX_DECIMAL;
            attr->digits = g_itoaLowerDigits;
            break;
        case SECUREC_CHAR('o'):
            /* For unsigned octal output */
            attr->radix = SECUREC_RADIX_OCTAL;
            attr->digits = g_itoaLowerDigits;


#endif /* SDK_LINUX_SMP_A55_LINUX_MPP_COMPONENT_SECUREC_SRC_OUTPUT_INL_BODY_PART03_H_ */
