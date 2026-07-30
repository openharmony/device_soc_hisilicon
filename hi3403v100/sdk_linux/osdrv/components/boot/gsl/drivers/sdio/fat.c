/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
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

#include "fat.h"
#include "securecutil.h"

/* Constants for reported magic-number values. */
#define GSL_FAT_FACTOR_2                       2
#define GSL_FAT_VALUE_3                        3

static block_dev_desc_t *cur_dev;
static disk_partition_t cur_part_info;
static unsigned long sdio_pos;

#define FILE_NAME "u-boot.bin"
#define BIT_CNT_8 8
#define DOS_PARTITION_NUM 4
#define BLOCK_SIZE 512

#define DOS_PART_MAGIC1 0x55
#define DOS_PART_MAGIC2 0xaa

#define le32_to_int(le32) ((((le32)[3]) << 24) + (((le32)[2]) << 16) + (((le32)[1]) << 8) + (le32)[0])

static int disk_read(uint32_t block, uint32_t nr_blocks, void *buf)
{
    if (!cur_dev || !cur_dev->block_read) {
        return -1;
    }

    return cur_dev->block_read(cur_dev->dev, cur_part_info.start + block,
        nr_blocks, buf);
}

/* Extract zero terminated short name from a directory entry */

static void get_name(const dir_entry *dirent, char *s_name, int len)
{
    char *ptr = NULL;
    errno_t err;

    if ((len == 0) || (len > FILE_NAME_MAX_LEN)) {
        return;
    }

    err = memcpy_s(s_name, FILE_NAME_MAX_LEN, dirent->name, FILE_NAME_MAX_LEN);
    if (err != EOK) {
        return;
    }

    s_name[FILE_NAME_MAX_LEN - 1] = '\0';
    ptr = s_name;
    while (*ptr && *ptr != ' ') {
        ptr++;
    }
    if (dirent->ext[0] && dirent->ext[0] != ' ') {
        *ptr = '.';
        ptr++;
        err = memcpy_s(ptr, FILE_NAME_MAX_LEN, dirent->ext, DIR_EXT_LEN);
        if (err != EOK) {
            return;
        }

        ptr[DIR_EXT_LEN] = '\0';
        while (*ptr && *ptr != ' ') {
            ptr++;
        }
    }
    *ptr = '\0';
    if (*s_name == DELETED_FLAG) {
        *s_name = '\0';
    } else if (*s_name == ARING) {
        *s_name = DELETED_FLAG;
    }

    while (*s_name != '\0') {
        tolower(*s_name);
        s_name++;
    }
}

/* Get the entry at index 'entry' in a FAT (12/16/32) table.
 * On failure 0x00 is returned.
 */
static uint32_t get_fatent(fsdata *mydata, uint32_t entry)
{
    uint32_t bufnum;
    uint32_t  offset;
    uint32_t ret = 0x00;

    bufnum = entry / FAT32BUFSIZE;
    offset = entry - bufnum * FAT32BUFSIZE;
    /* Read a new block of FAT entries into the cache. */
    if (bufnum != mydata->fatbufnum) {
        uint32_t getsize = FATBUFBLOCKS;
        uint8_t *bufptr = mydata->fatbuf;
        uint32_t fatlength = mydata->fatlength;
        uint32_t startblock = bufnum * FATBUFBLOCKS;

        if (getsize > fatlength) {
            getsize = fatlength;
        }
        fatlength *= mydata->sect_size;    /* We want it in bytes now */
        startblock += mydata->fat_sect;    /* Offset from start of disk */

        if (disk_read(startblock, getsize, bufptr) < 0) {
            return ret;
        }
        mydata->fatbufnum = bufnum;
    }

    /* Get the actual entry from the table */
    ret = ((uint32_t *) mydata->fatbuf)[offset];

    return ret;
}

/*
 * Read at most 'size' bytes from the specified cluster into 'buffer'.
 * Return 0 on success, -1 otherwise.
 */

static int get_cluster(const fsdata *mydata, uint32_t clustnum, uint8_t *buffer,
    unsigned long size)
{
    uint32_t idx;
    uint32_t startsect;
    uint32_t nr_sect;
    int ret;
    errno_t err;

    if (clustnum > 0) {
        startsect = mydata->data_begin + clustnum * mydata->clust_size;
    } else {
        startsect = mydata->rootdir_sect;
    }

    nr_sect = size / mydata->sect_size;
    ret = disk_read(startsect, nr_sect, buffer);
    if (ret != nr_sect) {
        return -1;
    }

    if (size > (mydata->sect_size * nr_sect)) {
        uint8_t tmpbuf[mydata->sect_size];
        idx = size / mydata->sect_size;
        ret = disk_read(startsect + idx, 1, tmpbuf);
        if (ret != 1) {
            return -1;
        }

        buffer += idx * mydata->sect_size;
        err = memcpy_s(buffer, size - idx * mydata->sect_size,
            tmpbuf, size - idx * mydata->sect_size);
        if (err != EOK) {
            return -1;
        }

        return 0;
    }

    return 0;
}

static int do_curclust_contents(fsdata *mydata, const dir_entry *dentptr, uint8_t *buffer,
    unsigned long maxsize, file_contents_parms_t *contents_parms)
{
    uint8_t *tmp_buffer = NULL;

    if (sdio_pos >= contents_parms->filesize)
    {
        return -1;
    }
    if ((maxsize > 0) && (contents_parms->filesize > (sdio_pos + maxsize))) {
        contents_parms->filesize = sdio_pos + maxsize;
    }
    contents_parms->actsize = contents_parms->bytesperclust;
    while (contents_parms->actsize <= sdio_pos) {
        contents_parms->curclust = get_fatent(mydata, contents_parms->curclust);
        if (check_clust(contents_parms->curclust)) {
            return -1;
        }
        contents_parms->actsize += contents_parms->bytesperclust;
    }

    contents_parms->actsize -= contents_parms->bytesperclust;
    contents_parms->filesize -= contents_parms->actsize;
    sdio_pos -= contents_parms->actsize;

    if (sdio_pos) {
        contents_parms->actsize = min(contents_parms->filesize,
            (unsigned long)contents_parms->bytesperclust);
        tmp_buffer = gsl_malloc(contents_parms->actsize);
        if (!tmp_buffer) {
        return -1;
        }
    if (get_cluster(mydata, contents_parms->curclust, tmp_buffer, contents_parms->actsize) != 0) {
        gsl_free(tmp_buffer);
        return -1;
    }

    contents_parms->filesize -= contents_parms->actsize;
    contents_parms->actsize -= sdio_pos;
    if (memcpy_s(*buffer, contents_parms->actsize, tmp_buffer + sdio_pos, contents_parms->actsize) != EOK) {
            gsl_free(tmp_buffer);
        return -1;
    }

        gsl_free(tmp_buffer);
    contents_parms->gotsize += contents_parms->actsize;
    if (!contents_parms->filesize) {
        return 0;
    }
    buffer += contents_parms->actsize;
    contents_parms->curclust = get_fatent(mydata, contents_parms->curclust);
        if (check_clust(contents_parms->curclust)) {
        return -1;
        }
    }

    contents_parms->actsize = contents_parms->bytesperclust;

    return 1;
}

static int do_consecutive_clusters(fsdata *mydata, const dir_entry *dentptr, uint8_t *buffer,
                   unsigned long maxsize, file_contents_parms_t *contents_parms)
{
    uint32_t endclust = contents_parms->curclust;
    uint32_t newclust;

    do {
        /* search for consecutive clusters */
        while (contents_parms->actsize < contents_parms->filesize) {
            newclust = get_fatent(mydata, endclust);
            if ((newclust - 1) != endclust) {
                goto getit;
            }

            if (check_clust(newclust)) {
                return contents_parms->gotsize;
            }

            endclust = newclust;
            contents_parms->actsize += contents_parms->bytesperclust;
        }

        /* get remaining bytes */
        contents_parms->actsize = contents_parms->filesize;
        if (get_cluster(mydata, contents_parms->curclust, buffer, (int)contents_parms->actsize) != 0) {
            return -1;
        }

        contents_parms->gotsize += contents_parms->actsize;
        return contents_parms->gotsize;
getit:
        if (get_cluster(mydata, contents_parms->curclust, buffer, (int)contents_parms->actsize) != 0) {
            return -1;
        }

        contents_parms->gotsize += (int)contents_parms->actsize;
        contents_parms->filesize -= contents_parms->actsize;
        buffer += contents_parms->actsize;

        contents_parms->curclust = get_fatent(mydata, endclust);
        if (check_clust(contents_parms->curclust)) {
            return contents_parms->gotsize;
        }
        contents_parms->actsize = contents_parms->bytesperclust;
        endclust = contents_parms->curclust;
    } while (1);
}

/*
 * Read at most 'maxsize' bytes from 'sdio_pos' in the file associated with 'dentptr'
 * into 'buffer'.
 * Return the number of bytes read or -1 on fatal errors.
 */
static long get_contents(fsdata *mydata, const dir_entry *dentptr,
             uint8_t *buffer, unsigned long maxsize)

{
    int ret;

    file_contents_parms_t contents_parms = {0};
    contents_parms.filesize = dentptr->size;
    contents_parms.bytesperclust = mydata->clust_size * mydata->sect_size;
    contents_parms.curclust = start(dentptr);

    ret = do_curclust_contents(mydata, dentptr, buffer, maxsize, &contents_parms);
    if (ret == 0 || ret == -1) {
        return ret;
    }

    ret = do_consecutive_clusters(mydata, dentptr, buffer, maxsize, &contents_parms);

    return ret;
}

void set_sdio_pos(unsigned long pos)
{
    sdio_pos = pos;
}

int do_data_block(fsdata *mydata, uint8_t *do_fat_read_at_block,
          uint32_t *root_cluster, uint32_t *cursect)
{
    boot_sector *bs = NULL;

    if ((disk_read(0, 1, do_fat_read_at_block) < 0) || do_fat_read_at_block == NULL) {
        return -1;
    }

    bs = (boot_sector *)do_fat_read_at_block;
    if (bs == NULL) {
        return -1;
    }

    *root_cluster = bs->root_cluster;
    mydata->fatlength = bs->fat32_length;
    mydata->fat_sect = bs->reserved;
    *cursect = mydata->rootdir_sect = mydata->fat_sect +
        mydata->fatlength * bs->fats;

    mydata->sect_size = (bs->sector_size[1] << BIT_CNT_8) + bs->sector_size[0];
    mydata->clust_size = bs->cluster_size;
    if (mydata->sect_size != cur_part_info.blksz) {
        return -1;
    }

    mydata->data_begin = mydata->rootdir_sect - (mydata->clust_size * GSL_FAT_FACTOR_2); /* get data begin */
    mydata->fatbufnum = -1;
    mydata->fatbuf = gsl_malloc(FATBUFSIZE);
    if (mydata->fatbuf == NULL) {
        return -1;
    }

    return 0;
}

dir_entry *do_data_cluster(const char *filename, uint32_t cursect,
    uint8_t *do_fat_read_at_block, fsdata *mydata, uint32_t root_cluster)
{
    int i;
    int j = 0;
    int fat32_end = 0;
    int nxtsect;
    int nxt_clust;
    dir_entry *dentptr = NULL;
    char s_name[FILE_NAME_MAX_LEN];

    if (mydata == NULL || filename == NULL) {
        return NULL;
    }
    while (1) {
        if (disk_read(cursect, 1, do_fat_read_at_block) < 0) {
            return NULL;
        }
        dentptr = (dir_entry *) do_fat_read_at_block;

        for (i = 0; i < DIRENTSPERBLOCK; i++) {
            if (dentptr->name[0] == DELETED_FLAG) {
                dentptr++;
                continue;
            }
            if ((dentptr->attr & ATTR_VOLUME)) {
                /* Volume label or VFAT entry */
                dentptr++;
                continue;
            } else if (dentptr->name[0] == 0) {
                return NULL;
            }

            get_name(dentptr, s_name, FILE_NAME_MAX_LEN);
            if (strcmp(filename, s_name)) {
                dentptr++;
                continue;
            }

            /* We got a match */
            return dentptr;
        }

        /*
         * On FAT32 we must fetch the FAT entries for the next
         * root directory clusters when a cluster has been
         * completely processed.
         */
        j++;
        if (j == mydata->clust_size) {
            nxt_clust = get_fatent(mydata, root_cluster);
            fat32_end = check_clust(nxt_clust);

            nxtsect = mydata->data_begin +
                (nxt_clust * mydata->clust_size);

            root_cluster = nxt_clust;
            cursect = nxtsect;
            j = 0;
        } else {
            cursect++;
        }
        if (fat32_end) {
            return NULL;
        }
    }

    return dentptr;
}

long do_fat_read_at(const char *filename, void *buffer,
    unsigned long maxsize)
{
    uint8_t *do_fat_read_at_block = NULL;
    fsdata datablock;
    fsdata *mydata = &datablock;
    dir_entry *dentptr = NULL;
    uint32_t root_cluster = 0;
    uint32_t cursect;
    long ret;

    do_fat_read_at_block = gsl_malloc(cur_part_info.blksz);
    if (!do_fat_read_at_block) {
        return -1;
    }

    ret = do_data_block(mydata, do_fat_read_at_block, &root_cluster, &cursect);
    if (ret != 0) {
        gsl_free(mydata->fatbuf);
        gsl_free(do_fat_read_at_block);
        return ret;
    }

    dentptr = do_data_cluster(filename, cursect, do_fat_read_at_block, mydata, root_cluster);
    if (dentptr == NULL) {
        gsl_free(mydata->fatbuf);
        gsl_free(do_fat_read_at_block);
        return ret;
    }

    ret = get_contents(mydata, dentptr, buffer, maxsize);
    gsl_free(mydata->fatbuf);
    gsl_free(do_fat_read_at_block);

    return ret;
}

static int step_primary_logical_partitions(block_dev_desc_t *dev_desc, unsigned char *buffer)
{
    dos_partition_t pt[DOS_PARTITION_NUM];
    unsigned char *temp_buffer = NULL;
    int size;
    int i;
    char *magic = "\x55\xAA";
    char *fs_type = "FAT32";

    if (memcpy_s((char *)pt, sizeof(pt), (char *)(buffer + DOS_PART_TBL_OFFSET),
        sizeof(dos_partition_t) * DOS_PARTITION_NUM) != EOK) {
        return -1;
    }

    for (i = 0; i < DOS_PARTITION_NUM; i++) {
        /* check partition type */
        if (((pt[i].boot_ind & ~0x80) != 0) ||
            (pt[i].sys_ind == 0) ||
            (pt[i].sys_ind == 0x5) || /*is_extended*/
            (pt[i].sys_ind == 0xf) ||
            (pt[i].sys_ind == 0x85)) {
            continue;
        }

        cur_part_info.blksz = BLOCK_SIZE;
        cur_part_info.start = le32_to_int(pt[i].start4);
        cur_part_info.size  = le32_to_int(pt[i].size4);
        cur_dev = dev_desc;

        if ((disk_read(0, 1, buffer) == 1) &&
            (memcmp(buffer + DOS_PART_MAGIC_OFFSET, magic, strlen(magic)) == 0) &&
            (memcmp(buffer + DOS_FS32_TYPE_OFFSET, fs_type, strlen(fs_type)) == 0)) {
            temp_buffer = gsl_malloc(512); /* max size 512 bytes */
            if (temp_buffer == NULL) {
                return -1;
            }
            size = do_fat_read_at(FILE_NAME, temp_buffer, 512);  /* max size 512 bytes */
            gsl_free(temp_buffer);
            temp_buffer = NULL;

            if (size > 0) {
                return 0;
            }
        }
        cur_dev = NULL;
    }

    return -1;
}

int whole_disk_fat_filesystem(block_dev_desc_t *dev_desc, unsigned char *buffer)
{
    unsigned char *temp_buffer = NULL;
    int size;
    char *magic = "\x55\xAA";
    char *fs_type = "FAT32";

    if (dev_desc == NULL || buffer == NULL) {
        return -1;
    }

    if (!cur_dev) {
        cur_dev = dev_desc;
        cur_part_info.start = 0;
        cur_part_info.size = dev_desc->lba;
        cur_part_info.blksz = dev_desc->blksz;
    }

    if ((disk_read(0, 1, buffer) == 1) &&
        (memcmp(buffer + DOS_PART_MAGIC_OFFSET, magic, strlen(magic)) == 0) &&
        (memcmp(buffer + DOS_FS32_TYPE_OFFSET, fs_type, strlen(fs_type)) == 0)) {
        temp_buffer = gsl_malloc(512); /* max size 512 bytes */
        if (temp_buffer == NULL) {
            return -1;
        }
        size = do_fat_read_at(FILE_NAME, temp_buffer, 512); /* max size 512 bytes */
        gsl_free(temp_buffer);
        if (size <= 0) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

int fat_register_device(block_dev_desc_t *dev_desc)
{
    unsigned char *buffer;
    int ret;

    if (dev_desc->blksz != BLOCK_SIZE) {
        debug_info("Not support this udisk (blksz>512)\n", 0);
        return -1;
    }

    buffer = gsl_malloc(dev_desc->blksz);
    if (!buffer) {
        return -1;
    }

    if (memset_s(buffer, dev_desc->blksz, 0, dev_desc->blksz) !=  EOK) {
        gsl_free(buffer);
        return -1;
    }

    /* step1 : check dos partition flag */
    cur_dev = NULL;
    if (dev_desc->block_read(dev_desc->dev, 0, 1, (uint32_t *)buffer) != 1) {
        gsl_free(buffer);
        return -1;
    }

    if (buffer[DOS_PART_MAGIC_OFFSET] != DOS_PART_MAGIC1 ||
        buffer[DOS_PART_MAGIC_OFFSET + 1] != DOS_PART_MAGIC2) {
        debug_info("No partition table\n", 0);
        gsl_free(buffer);
        return 0;
    }

    /* step2 :step all primary/logical partitions */
    ret = step_primary_logical_partitions(dev_desc, buffer);
    if (ret == 0) {
        gsl_free(buffer);
        return GSL_FAT_VALUE_3; /* readbootok */
    }

    ret = whole_disk_fat_filesystem(dev_desc, buffer);
    if (ret == 0) {
        gsl_free(buffer);
        return GSL_FAT_VALUE_3; /* readbootok */
    }

    gsl_free(buffer);
    return 0; /* nofastboot */
}
