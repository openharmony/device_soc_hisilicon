/*
 * Copyright 2020 Huawei Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Avoid duplicate header files,not include securecutil.h */
#include "securecutil.h"

#if defined(ANDROID) && !defined(SECUREC_CLOSE_ANDROID_HANDLE) && (SECUREC_HAVE_WCTOMB || SECUREC_HAVE_MBTOWC)
#include <wchar.h>
#if SECUREC_HAVE_WCTOMB
/*
 * Convert wide characters to narrow multi-bytes
 */
int wctomb(char *s, wchar_t wc)
{
    return (int)wcrtomb(s, wc, NULL);
}
#endif

#if SECUREC_HAVE_MBTOWC
/*
 * Converting narrow multi-byte characters to wide characters
 * mbrtowc returns -1 or -2 upon failure, unlike mbtowc, which only returns -1
 * When the return value is less than zero, we treat it as a failure
 */
int mbtowc(wchar_t *pwc, const char *s, size_t n)
{
    return (int)mbrtowc(pwc, s, n, NULL);
}
#endif
#endif

/* The V100R001C01 version num is 0x5 (High 8 bits) */
#define SECUREC_C_VERSION     0x500U
#define SECUREC_SPC_VERSION   0xbU
#define SECUREC_VERSION_STR   "V100R001C01SPC011B003"

/*
 * Get version string and version number.
 * The rules for version number are as follows:
 * 1) SPC verNumber<->verStr like:
 * 0x201<->C01
 * 0x202<->C01SPC001   Redefine numbers after this version
 * 0x502<->C01SPC002
 * 0x503<->C01SPC003
 * ...
 * 0X50a<->SPC010
 * 0X50b<->SPC011
 * ...
 * 0x700<->C02
 * 0x701<->C01SPC001
 * 0x702<->C02SPC002
 * ...
 * 2) CP verNumber<->verStr like:
 * 0X601<->CP0001
 * 0X602<->CP0002
 * ...
 */
const char *GetHwSecureCVersion(unsigned short *verNumber)
{
    if (verNumber != NULL) {
        *verNumber = (unsigned short)(SECUREC_C_VERSION | SECUREC_SPC_VERSION);
    }
    return SECUREC_VERSION_STR;
}
#if SECUREC_EXPORT_KERNEL_SYMBOL
EXPORT_SYMBOL(GetHwSecureCVersion);
#endif

