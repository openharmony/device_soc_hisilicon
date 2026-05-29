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

#include "ot_osal.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/version.h>

int osal_strcmp(const char *cs, const char *ct)
{
    return strcmp(cs, ct);
}
EXPORT_SYMBOL(osal_strcmp);
int osal_strncmp(const char *cs, const char *ct, int count)
{
    return strncmp(cs, ct, count);
}
EXPORT_SYMBOL(osal_strncmp);
char *osal_strstr(const char *s1, const char *s2)
{
    return strstr(s1, s2);
}
EXPORT_SYMBOL(osal_strstr);
int osal_strlen(const char *s)
{
    return strlen(s);
}
EXPORT_SYMBOL(osal_strlen);
int osal_strnlen(const char *s, int count)
{
    return strnlen(s, count);
}
EXPORT_SYMBOL(osal_strnlen);
char *osal_strsep(char **s, const char *ct)
{
    return strsep(s, ct);
}
EXPORT_SYMBOL(osal_strsep);
int osal_memcmp(const void *cs, const void *ct, int count)
{
    return memcmp(cs, ct, count);
}
EXPORT_SYMBOL(osal_memcmp);
long osal_strtol(const char *cp, char **endp, unsigned int base)
{
    return simple_strtol(cp, endp, base);
}
EXPORT_SYMBOL(osal_strtol);
