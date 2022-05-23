/*
 * Copyright (C) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "hi_osal.h"
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/random.h>

int osal_memncmp(const void *buf1, unsigned long size1,
    const void *buf2, unsigned long size2)
{
    if (size1 != size2) {
        return -1;
    }

    return memcmp(buf1, buf2, size1);
}
EXPORT_SYMBOL(osal_memncmp);

int osal_strncmp(const char *str1, unsigned long size1,
    const char *str2, unsigned long size2)
{
    if (size1 != size2) {
        return -1;
    }

    return strncmp(str1, str2, size1);
}
EXPORT_SYMBOL(osal_strncmp);

int osal_strncasecmp(const char *str1, unsigned long size1,
    const char *str2, unsigned long size2)
{
    if (size1 != size2) {
        return -1;
    }

    return strncasecmp(str1, str2, size1);
}
EXPORT_SYMBOL(osal_strncasecmp);

long osal_strtol(const char *str, char **end, unsigned int base)
{
    return simple_strtol(str, end, base);
}
EXPORT_SYMBOL(osal_strtol);

unsigned long  osal_strtoul(const char *str, char **end, unsigned int base)
{
    return simple_strtoul(str, end, base);
}
EXPORT_SYMBOL(osal_strtoul);

void osal_get_random_bytes(void *buf, int nbytes)
{
    get_random_bytes(buf, nbytes);
}
EXPORT_SYMBOL(osal_get_random_bytes);
