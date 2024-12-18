/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: common function of shell cmds
 * Author: none
 * Create: 2020
 */

#include "lwip/nettool/utility.h"

#if LWIP_ENABLE_BASIC_SHELL_CMD
 
int convert_string_to_hex(const char *src, unsigned char *dest)
{
  *dest = 0;
  while (*src) {
    *dest = (unsigned char)((*dest << 4) & 0xFF);
    if ((*src >= '0') && (*src <= '9')) {   /* between 0 to 9 */
      *dest |= (unsigned char)(*src - '0');
    } else if ((*src >= 'A' && *src <= 'F')) { /* between A to F */
      *dest |= (unsigned char)((*src - 'A') + 10);
    } else if ((*src >= 'a' && *src <= 'f')) { /* between a to f */
      *dest |= (unsigned char)((*src - 'a') + 10);
    } else {
      return -1;
    }
    ++src;
  }
 
  return 0;
}
 
#endif /* LWIP_ENABLE_BASIC_SHELL_CMD */

s32_t lwip_isdigitstr(const s8_t* str)
{
  while(*str) {
    if((*str >= '0') && (*str <= '9')) {
      ++str;
      continue;
    }

    return -1;
  }

  return 0;
}
