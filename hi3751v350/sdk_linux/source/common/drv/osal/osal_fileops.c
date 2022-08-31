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

#include <linux/fs.h>
#include <asm/uaccess.h>
#include "hi_osal.h"
#include "securec.h"

char *g_klib_store_path = NULL;

struct file *klib_fopen(const char *filename, int flags, int mode)
{
    struct file *filp = filp_open(filename, flags, mode);
    return (IS_ERR(filp)) ? NULL : filp;
}

void klib_fclose(struct file *filp)
{
    if (filp != NULL) {
        filp_close(filp, NULL);
    }
    return;
}

int klib_fwrite(const char *buf, unsigned long size, struct file *filp)
{
    int writelen;

    if (filp == NULL) {
        return -ENOENT;
    }

    writelen = __kernel_write(filp, buf, size, &filp->f_pos);
    return writelen;
}

int klib_fread(char *buf, unsigned long size, struct file *filp)
{
    mm_segment_t old_fs;
    int readlen;

    if (filp == NULL) {
        return -ENOENT;
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    /* The cast to a user pointer is valid due to the set_fs() */
    readlen = vfs_read(filp, (void __user *)buf, size, &filp->f_pos);
    set_fs(old_fs);
    return readlen;
}

void *osal_klib_fopen(const char *filename, int flags, int mode)
{
    if (filename == NULL) {
        osal_printk("%s - filename NULL\n", __FUNCTION__);
        return NULL;
    }

    return (void *)klib_fopen(filename, flags, mode);
}
EXPORT_SYMBOL(osal_klib_fopen);

void osal_klib_fclose(void *filp)
{
    if (filp == NULL) {
        osal_printk("%s - filp NULL\n", __FUNCTION__);
        return;
    }

    klib_fclose((struct file *)filp);
}
EXPORT_SYMBOL(osal_klib_fclose);

int osal_klib_fwrite(const char *buf, unsigned long size, void *filp)
{
    if ((buf == NULL) || (filp == NULL)) {
        osal_printk("%s - buf&filp NULL\n", __FUNCTION__);
        return -1;
    }

    return klib_fwrite(buf, size, (struct file *)filp);
}
EXPORT_SYMBOL(osal_klib_fwrite);

int osal_klib_fread(char *buf, unsigned long size, void *filp)
{
    if ((buf == NULL) || (filp == NULL)) {
        osal_printk("%s - buf&filp NULL\n", __FUNCTION__);
        return -1;
    }

    return klib_fread(buf, size, (struct file *)filp);
}
EXPORT_SYMBOL(osal_klib_fread);

int osal_klib_user_fread(char *buf, unsigned long size, void *filp)
{
    int len;
    mm_segment_t st_old_fs = {0};

    if (filp == NULL || buf == NULL) {
        osal_printk("%s - buf&filp NULL\n", __FUNCTION__);
        return -1; /* no such file or directory */
    }

    if (((((struct file *)filp)->f_flags & O_ACCMODE) & (O_RDONLY | O_RDWR)) != 0) {
        return -1; /* permission denied */
    }

    /* saved the original file space */
    st_old_fs = get_fs();

    /* extend to the kernel data space */
    set_fs(USER_DS);

    len = vfs_read((struct file *)filp, buf, size, &(((struct file *)filp)->f_pos));

    /* restore the original file space */
    set_fs(st_old_fs);

    return len;
}
EXPORT_SYMBOL(osal_klib_user_fread);

int osal_klib_user_fwrite(const char *buf, unsigned long size, void *filp)
{
    int len;
    mm_segment_t st_old_fs = {0};

    if (filp == NULL || buf == NULL) {
        osal_printk("%s - buf&filp NULL\n", __FUNCTION__);
        return -1; /* no such file or directory */
    }

    if (((((struct file *)filp)->f_flags & O_ACCMODE) & (O_WRONLY | O_RDWR)) == 0) {
        return -1; /* permission denied */
    }

    st_old_fs = get_fs();
    set_fs(USER_DS);

    len = vfs_write((struct file *)filp, buf, size, &(((struct file *)filp)->f_pos));

    set_fs(st_old_fs);

    return len;
}
EXPORT_SYMBOL(osal_klib_user_fwrite);

int osal_klib_fseek(long long offset, int whence, void *filp)
{
    int ret;
    loff_t res;

    if (filp == NULL) {
        osal_printk("%s - filp NULL\n", __FUNCTION__);
        return -1;
    }

    res = vfs_llseek(filp, offset, whence);
    ret = res;
    if (res != (loff_t)ret) {
        ret = -EOVERFLOW;
    }

    return ret;
}
EXPORT_SYMBOL(osal_klib_fseek);

void osal_klib_fsync(void *filp)
{
    if (filp == NULL) {
        osal_printk("%s - filp NULL\n", __FUNCTION__);
        return;
    }

    vfs_fsync(filp, 0);
}
EXPORT_SYMBOL(osal_klib_fsync);

void osal_klib_set_store_path(char *path)
{
    g_klib_store_path = path;
}
EXPORT_SYMBOL(osal_klib_set_store_path);

int osal_klib_get_store_path(char *path, unsigned int path_size)
{
    int len;

#if defined (CONFIG_CLOSE_UART0)
    /* not support log when uart close or user version */
    if (osal_get_buildvariant() != OSAL_BUILDVARIANT_ENG) {
        return 0;
    }
#endif

    if ((path == NULL) || (g_klib_store_path == NULL)) {
        osal_printk("%s - path Or g_klib_store_path NULL\n", __FUNCTION__);
        return -1;
    }

    len = strlen(g_klib_store_path) + 1;
    if (len > path_size || path_size <= 1) {
        osal_printk("%s - path_size(%u) unvaild\n", __FUNCTION__, path_size);
        return -1;
    }

    return memcpy_s(path, path_size, g_klib_store_path, len);
}
EXPORT_SYMBOL(osal_klib_get_store_path);
