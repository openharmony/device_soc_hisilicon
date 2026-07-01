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

#ifndef _FAT_H_
#define _FAT_H_

#include "types.h"

#define DIRENTSPERBLOCK    (mydata->sect_size / sizeof(dir_entry))
#define DIRENTSPERCLUST    ((mydata->clust_size * mydata->sect_size) / \
             sizeof(dir_entry))

#define FATBUFBLOCKS    4
#define FATBUFSIZE    (mydata->sect_size * FATBUFBLOCKS)
#define FAT32BUFSIZE    (FATBUFSIZE / 4)

/* support max usb partition num ,start with 1 */
#define USB_MAX_PART_NUM  5

/* File attributes */
#define ATTR_RO    1
#define ATTR_HIDDEN    2
#define ATTR_SYS    4
#define ATTR_VOLUME    8
#define ATTR_DIR    16
#define ATTR_ARCH    32

#define ATTR_VFAT    (ATTR_RO | ATTR_HIDDEN | ATTR_SYS | ATTR_VOLUME)

#define DELETED_FLAG    ((char)0xe5) /* Marks deleted files when in name[0] */
#define ARING        0x05         /* Used as special character in name[0] */
#define FILE_NAME_MAX_LEN 14
#define DIR_EXT_LEN 3
/*
 * Indicates that the entry is the last long entry in a set of long
 * dir entries
 */
#define LAST_LONG_ENTRY_MASK    0x40
#define start(dent)    ((dent)->start + ((dent)->starthi << 16))
#define tolower(c)                \
    do {                    \
        if ((c) >= 'A' && (c) <= 'Z')    \
            (c) += ('a' - 'A');    \
    } while (0)
#define check_clust(x) ((x) <= 1 ||(x) >= 0xffffff0)

typedef struct boot_sector {
    uint8_t    ignored[3];    /* Bootstrap code */
    char    system_id[8];    /* Name of fs */
    uint8_t    sector_size[2];    /* Bytes/sector */
    uint8_t    cluster_size;    /* Sectors/cluster */
    uint16_t    reserved;    /* Number of reserved sectors */
    uint8_t    fats;        /* Number of FATs */
    uint8_t    dir_entries[2];    /* Number of root directory entries */
    uint8_t    sectors[2];    /* Number of sectors */
    uint8_t    media;        /* Media code */
    uint16_t    fat_length;    /* Sectors/FAT */
    uint16_t    secs_track;    /* Sectors/track */
    uint16_t    heads;        /* Number of heads */
    uint32_t    hidden;        /* Number of hidden sectors */
    uint32_t    total_sect;    /* Number of sectors (if sectors == 0) */

    /* FAT32 only */
    uint32_t    fat32_length;    /* Sectors/FAT */
    uint16_t    flags;        /* Bit 8: fat mirroring, low 4: active fat */
    uint8_t    version[2];    /* Filesystem version */
    uint32_t    root_cluster;    /* First cluster in root directory */
    uint16_t    info_sector;    /* Filesystem info sector */
    uint16_t    backup_boot;    /* Backup boot sector */
    uint16_t    reserved2[6];    /* Unused */
} __attribute__((packed)) boot_sector;

typedef struct dir_entry {
    char    name[8], ext[DIR_EXT_LEN]; /* Name and extension */
    uint8_t    attr;            /* Attribute bits */
    uint8_t    lcase;            /* Case for base and extension */
    uint8_t    ctime_ms;        /* Creation time, milliseconds */
    uint16_t    ctime;        /* Creation time */
    uint16_t    cdate;        /* Creation date */
    uint16_t    adate;        /* Last access date */
    uint16_t    starthi;    /* High 16 bits of cluster in FAT32 */
    uint16_t    time, date, start; /* Time, date and first cluster */
    uint32_t    size;        /* File size in bytes */
} dir_entry;

/*
 * Private filesystem parameters
 *
 * Note: FAT buffer has to be 32 bit aligned
 * (see FAT32 accesses)
 */
typedef struct {
    uint8_t        *fatbuf;    /* Current FAT buffer */
    uint32_t    fatlength;    /* Length of FAT in sectors */
    uint16_t    fat_sect;    /* Starting sector of the FAT */
    uint32_t    rootdir_sect;    /* Start sector of root directory */
    uint16_t    sect_size;    /* Size of sectors in bytes */
    uint16_t    clust_size;    /* Size of clusters in sectors */
    int    data_begin;    /* The sector of the first cluster, can be negative */
    int    fatbufnum;    /* Used by get_fatent, init to -1 */
} fsdata;

#define DOS_FS32_TYPE_OFFSET    0x52
#define DEFAULT_SECTOR_SIZE    512
#define DOS_PART_TBL_OFFSET    0x1be
#define DOS_PART_MAGIC_OFFSET    0x1fe

typedef struct dos_partition {
    unsigned char boot_ind;        /* 0x80 - active            */
    unsigned char head;        /* starting head            */
    unsigned char sector;        /* starting sector            */
    unsigned char cyl;        /* starting cylinder            */
    unsigned char sys_ind;        /* What partition type            */
    unsigned char end_head;        /* end head                */
    unsigned char end_sector;    /* end sector                */
    unsigned char end_cyl;        /* end cylinder                */
    unsigned char start4[4];    /* starting sector counting from 0    */
    unsigned char size4[4];        /* nr of sectors in partition        */
} __attribute__((packed)) dos_partition_t;

typedef struct block_dev_desc {
    int        dev;        /* device number */
    unsigned char    target;        /* target SCSI ID */
    unsigned char    lun;        /* target LUN */
    unsigned char    removable;    /* removable device */
    unsigned long    lba;        /* number of blocks */
    unsigned long    blksz;        /* block size */
    unsigned long    (*block_read)(int dev,
                      unsigned long start,
                      unsigned long blkcnt,
                      void *buffer);
    unsigned long    (*block_write)(int dev,
                       unsigned long start,
                       unsigned long blkcnt,
                       const void *buffer);
    void        *priv;        /* driver private struct pointer */
} block_dev_desc_t;

typedef struct disk_partition {
    unsigned long    start;        /* # of first block in partition    */
    unsigned long    size;        /* number of blocks in partition    */
    unsigned long    blksz;        /* block size in bytes            */
} disk_partition_t;

typedef struct file_contents_parms {
    unsigned long filesize;
    unsigned long gotsize;
    unsigned long actsize;
    unsigned int bytesperclust;
    uint32_t curclust;
} file_contents_parms_t;

/* Currently this doesn't check if the dir exists or is valid... */
int fat_register_device(block_dev_desc_t *dev_desc);
long do_fat_read_at(const char *filename, void *buffer, unsigned long maxsize);
void set_sdio_pos(unsigned long pos);
#endif /* _FAT_H_ */
