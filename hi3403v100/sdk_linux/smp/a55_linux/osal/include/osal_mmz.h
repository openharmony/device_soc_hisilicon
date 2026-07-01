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

#ifndef _OSAL_MMZ_H
#define _OSAL_MMZ_H

#include "ot_osal.h"
#include "osal_ioctl.h"

#ifdef __KERNEL__
#include <linux/string.h>
#else
#include <string.h>
#endif

#ifndef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE            0x40
#endif

#define OT_MMZ_NAME_LEN           32
#define OT_MMB_NAME_LEN           32

#define OT_MMB_MAX_PID_NUM     5

#define MMZ_REGION_FREE        0
#define MMZ_REGION_BUSY       (-1)
#define MMZ_REGION_NOT_ENOUGH (-2)
#define MMZ_REGION_OUT_RANGE  (-3)


struct ot_media_memory_zone {
    char name[OT_MMZ_NAME_LEN];

    unsigned long gfp;

    unsigned long phys_start;
    unsigned long nbytes;

    struct osal_list_head list;
    union {
        struct device *cma_dev;
        unsigned char *bitmap;
    };
    struct osal_list_head mmb_list;

    unsigned int alloc_type;
    unsigned long block_align;

    void (*destructor)(const void *);
};
typedef struct ot_media_memory_zone ot_mmz_t;

#define OT_MMZ_FMT_S              "PHYS(0x%08lX, 0x%08lX), GFP=%lu, nBYTES=%luKB,    NAME=\"%s\""
#define ot_mmz_fmt_arg(p) (p)->phys_start, (p)->phys_start + (p)->nbytes - 1, (p)->gfp, (p)->nbytes / SZ_1K, (p)->name

struct ot_media_memory_block {
#ifndef MMZ_V2_SUPPORT
    unsigned int id;
#endif
    char name[OT_MMB_NAME_LEN];
    struct ot_media_memory_zone *zone;
    struct osal_list_head list;

    unsigned long phys_addr;
    void *kvirt;
    unsigned long length;

    unsigned long flags;

    unsigned int order;

    int phy_ref;
    int map_ref;

    int kernel_only;
    int check_pid;
    int pid_num;
    int pid[OT_MMB_MAX_PID_NUM];
};
typedef struct ot_media_memory_block ot_mmb_t;

#define ot_mmb_kvirt(p) ({ot_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->kvirt; })
#define ot_mmb_phys(p) ({ot_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->phys_addr; })
#define ot_mmb_length(p) ({ot_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->length; })
#define ot_mmb_name(p) ({ot_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->name; })
#define ot_mmb_zone(p) ({ot_mmb_t *__mmb=(p); OSAL_BUG_ON(__mmb==NULL); __mmb->zone; })

#define OT_MMB_MAP2KERN           (1 << 0)
#define OT_MMB_MAP2KERN_CACHED    (1 << 1)
#define OT_MMB_RELEASED           (1 << 2)

#ifdef CONFIG_64BIT
#define OT_MMB_FMT_S \
    "phys(0x%08lX, 0x%08lX), kvirt=0x%016lX, flags=0x%lX, kernel_only=%d, share_all=%d pid[%d,%d,%d,%d,%d], " \
    "length=%luKB,    name=\"%s\""
#else
#define OT_MMB_FMT_S \
    "phys(0x%08lX, 0x%08lX), kvirt=0x%08lX, flags=0x%lX, kernel_only=%d, share_all=%d pid[%d,%d,%d,%d,%d], " \
    "length=%luKB,    name=\"%s\""
#endif
#define ot_mmb_fmt_arg(p)   (p)->phys_addr, mmz_grain_align((p)->phys_addr + (p)->length) - 1, \
    (unsigned long)(uintptr_t)((p)->kvirt), (p)->flags, (p)->kernel_only, !((p)->check_pid), \
    (p)->pid[0], (p)->pid[1], (p)->pid[2], (p)->pid[3], (p)->pid[4], (p)->length / SZ_1K, (p)->name

#define DEFAULT_ALLOC              0
#define SLAB_ALLOC                 1
#define EQ_BLOCK_ALLOC             2

#define LOW_TO_HIGH                0
#define HIGH_TO_LOW                1

#define MMZ_DBG_LEVEL              0x0
#define MMZ_INFO_LEVEL             0x1

#define mmz_trace(level, s, params...)                                             \
    do {                                                                           \
        if (level & MMZ_DBG_LEVEL)                                                 \
            printk(KERN_INFO "[%s, %d]: " s "\n", __FUNCTION__, __LINE__, params); \
    } while (0)
#define mmz_trace_func() mmz_trace(0x02, "%s", __FILE__)

#define MMZ_GRAIN                  PAGE_SIZE
#define mmz_bitmap_size(p) (mmz_align2(mmz_length2grain((p)->nbytes), 8) / 8)

#define mmz_get_bit(p, n) (((p)->bitmap[(n) / 8] >> ((n) & 0x7)) & 0x1)
#define mmz_set_bit(p, n) ((p)->bitmap[(n) / 8] |= 1 << ((n) & 0x7))
#define mmz_clr_bit(p, n) ((p)->bitmap[(n) / 8] &= ~(1 << ((n) & 0x7)))

#define mmz_pos2phy_addr(p, n) ((p)->phys_start + (n) * MMZ_GRAIN)
#define mmz_phy_addr2pos(p, a) (((a) - (p)->phys_start) / MMZ_GRAIN)

#define mmz_align2low(x, g) (((g) == 0) ? (0) : (((x) / (g)) * (g)))
#define mmz_align2(x, g) (((g) == 0) ? (0) : ((((x) + (g) - 1) / (g)) * (g)))
#define mmz_grain_align(x) mmz_align2(x, MMZ_GRAIN)
#define mmz_length2grain(len) (mmz_grain_align(len) / MMZ_GRAIN)

int ot_mmz_get_anony(void);

static inline int osal_mmz_match_region(const ot_mmz_t *p, unsigned long gfp, const char *mmz_name)
{
    if (((gfp) != 0) && (p->gfp != gfp)) {
        return 0;
    }
    if (((mmz_name) == NULL) || (*mmz_name == '\0')) {
        return (ot_mmz_get_anony() == 1) && (strcmp("anonymous", p->name) == 0);
    }
    return strcmp(mmz_name, p->name) == 0;
}

#define begin_list_for_each_mmz(p, gfp, mmz_name)       \
    list_for_each_entry(p, ot_mmz_get_mmz_list(), list) \
    {                                                   \
        if (!osal_mmz_match_region((p), (gfp), (mmz_name))) { continue; } \
        if (1) { mmz_trace(1, OT_MMZ_FMT_S, ot_mmz_fmt_arg(p)); }
#define end_list_for_each_mmz() }

#ifdef CONFIG_PHYS_ADDR_BIT_WIDTH_64
#define OSAL_PHYS_ADDR_TYPE         unsigned long long
#define OSAL_PHYS_LEN_TYPE          unsigned long long
#define OSAL_PHYS_ADDR_ALIGN        __attribute__((aligned(8)))
#else
#define OSAL_PHYS_ADDR_TYPE         unsigned long
#define OSAL_PHYS_LEN_TYPE          unsigned long
#define OSAL_PHYS_ADDR_ALIGN        __attribute__((aligned(sizeof(long))))
#endif

struct mmb_info {
    OSAL_PHYS_ADDR_TYPE phys_addr; /* phys-memory address */
    OSAL_PHYS_ADDR_TYPE OSAL_PHYS_ADDR_ALIGN align; /* if you need your phys-memory have special align size */
    OSAL_PHYS_LEN_TYPE OSAL_PHYS_ADDR_ALIGN size; /* length of memory you need, in bytes */
    unsigned int OSAL_PHYS_ADDR_ALIGN order;

    void *OSAL_PHYS_ADDR_ALIGN mapped; /* userspace mapped ptr */

    union {
        struct {
            unsigned long prot : 8; /* PROT_READ or PROT_WRITE */
            unsigned long flags : 12; /* MAP_SHARED or MAP_PRIVATE */

#ifdef __KERNEL__
            unsigned long reserved : 8; /* reserved, do not use */
            unsigned long delayed_free : 1;
            unsigned long map_cached : 1;
#endif
        };
        unsigned long w32_stuf;
    } OSAL_PHYS_ADDR_ALIGN;

    char mmb_name[OT_MMB_NAME_LEN];
    char mmz_name[OT_MMZ_NAME_LEN];
    unsigned long OSAL_PHYS_ADDR_ALIGN gfp; /* reserved, do set to 0 */

#ifdef __KERNEL__
    int map_ref;
    int mmb_ref;

    struct osal_list_head list;
    ot_mmb_t *mmb;
#endif
} __attribute__((aligned(8)));

struct mmb_share_info {
    unsigned long phys_addr;
    void *virt_addr;
    void *mem_handle;
    unsigned long offset;
    int shared_pid;
};

struct dirty_area {
    OSAL_PHYS_ADDR_TYPE dirty_phys_start; /* dirty physical address */
    void *OSAL_PHYS_ADDR_ALIGN dirty_virt_start; /* dirty virtual  address,
                       must be coherent with dirty_phys_addr */
    OSAL_PHYS_LEN_TYPE OSAL_PHYS_ADDR_ALIGN dirty_size;
} OSAL_PHYS_ADDR_ALIGN;

typedef struct {
    unsigned long phys_addr;
    unsigned long size;
    int pid;
    ot_mmb_t mmb;
    void *mmb_handle;
} mmb_user_info;

typedef enum {
    IOC_MMB_MEM_SHARE_NR = 0,
    IOC_MMB_MEM_UNSHARE_NR,
    IOC_MMB_MEM_SHARE_ALL_NR,
    IOC_MMB_MEM_UNSHARE_ALL_NR,
    IOC_MMB_VIRT_GET_SYS_MEM_NR,
    IOC_MMB_PHYS_GET_SYS_MEM_NR,
    IOC_MMB_HANDLE_GET_SYS_MEM_NR,
    IOC_MMB_ALLOC_NR = 10,
    IOC_MMB_ATTR_NR,
    IOC_MMB_FREE_NR,
    IOC_MMB_ALLOC_V2_NR,
    IOC_MMB_ALLOC_V3_NR,
    IOC_MMB_USER_REMAP_NR,
    IOC_MMB_USER_REMAP_CACHED_NR,
    IOC_MMB_USER_UNMAP_NR,
    IOC_MMB_VIRT_GET_PHYS_NR,
    IOC_MMB_ADD_REF_NR,
    IOC_MMB_DEC_REF_NR,
    IOC_MMB_FLUSH_DCACHE_NR,
    IOC_MMB_FLUSH_DCACHE_DIRTY_NR,
    IOC_MMB_TEST_CACHE_NR,
    IOC_MMB_SYS_FLUSH_CACHE_NR,
    IOC_MMB_BASE_CHECK_ADDR_NR,
    IOC_MMB_INVALID_CACHE_NR,
    IOC_MMB_CHECK_PHY_ALLOC_NR,
    IOC_MMB_MMF_REMAP_NR,
    IOC_MMB_MMF_REMAP_CACHED_NR,
    IOC_MMB_MMF_UNMAP_NR,
    IOC_MMB_SYS_MFLUSH_CACHE_NR,
    IOC_MMB_CHECK_MEM_SHARE_NR,
    IOC_MMB_CHECK_MEM_SHARE_WITH_PID_NR,
    IOC_MMZ_CHECK_PHYS_ADDR_NR,
    IOC_MMZ_GET_MEM_PROCESS_ISOLATION_NR,
    IOC_MMZ_PHYS_TO_HANDLE_NR,
    IOC_MMZ_GET_PHYS_ADDR_NR,
    IOC_NR_MMB_BUTT
}ioc_nr_mmz;

#define IOC_MMB_ALLOC              OSAL_IOWR('m', IOC_MMB_ALLOC_NR, struct mmb_info)
#define IOC_MMB_ATTR               OSAL_IOR('m', IOC_MMB_ATTR_NR, struct mmb_info)
#define IOC_MMB_FREE               OSAL_IOW('m', IOC_MMB_FREE_NR, struct mmb_info)
#define IOC_MMB_ALLOC_V2           OSAL_IOWR('m', IOC_MMB_ALLOC_V2_NR, struct mmb_info)
#define IOC_MMB_ALLOC_V3           OSAL_IOWR('m', IOC_MMB_ALLOC_V3_NR, struct mmb_info)

#define IOC_MMB_USER_REMAP         OSAL_IOWR('m', IOC_MMB_USER_REMAP_NR, struct mmb_info)
#define IOC_MMB_USER_REMAP_CACHED  OSAL_IOWR('m', IOC_MMB_USER_REMAP_CACHED_NR, struct mmb_info)
#define IOC_MMB_USER_UNMAP         OSAL_IOWR('m', IOC_MMB_USER_UNMAP_NR, struct mmb_info)
#define IOC_MMB_VIRT_GET_PHYS      OSAL_IOWR('m', IOC_MMB_VIRT_GET_PHYS_NR, struct mmb_info)
#define IOC_MMB_SYS_FLUSH_CACHE    OSAL_IOW('m', IOC_MMB_SYS_FLUSH_CACHE_NR, struct mmb_info)
#define IOC_MMB_SYS_MFLUSH_CACHE    OSAL_IOW('m', IOC_MMB_SYS_MFLUSH_CACHE_NR, struct mmb_info)
#define IOC_MMB_BASE_CHECK_ADDR    OSAL_IOW('m', IOC_MMB_BASE_CHECK_ADDR_NR, struct mmb_info)
#define IOC_MMB_INVALID_CACHE      OSAL_IOW('m', IOC_MMB_INVALID_CACHE_NR, struct mmb_info)
#define IOC_MMB_CHECK_PHY_ALLOC    OSAL_IOWR('m', IOC_MMB_CHECK_PHY_ALLOC_NR, struct mmb_info)
#define IOC_MMB_MMF_REMAP          OSAL_IOWR('m', IOC_MMB_MMF_REMAP_NR, struct mmb_info)
#define IOC_MMB_MMF_REMAP_CACHED   OSAL_IOWR('m', IOC_MMB_MMF_REMAP_CACHED_NR, struct mmb_info)
#define IOC_MMB_MMF_UNMAP          OSAL_IOWR('m', IOC_MMB_MMF_UNMAP_NR, struct mmb_info)
#define IOC_MMZ_GET_PHYS_ADDR      OSAL_IOWR('m', IOC_MMZ_GET_PHYS_ADDR_NR, struct mmb_info)

#define IOC_MMB_CHECK_MEM_SHARE           OSAL_IOWR('p', IOC_MMB_CHECK_MEM_SHARE_NR, mmb_user_info)
#define IOC_MMB_CHECK_MEM_SHARE_WITH_PID  OSAL_IOWR('p', IOC_MMB_CHECK_MEM_SHARE_WITH_PID_NR, mmb_user_info)
#define IOC_MMZ_CHECK_PHYS_ADDR           OSAL_IOWR('p', IOC_MMZ_CHECK_PHYS_ADDR_NR, mmb_user_info)
#define IOC_MMZ_GET_MEM_PROCESS_ISOLATION OSAL_IO('p', IOC_MMZ_GET_MEM_PROCESS_ISOLATION_NR)
#define IOC_MMZ_PHYS_TO_HANDLE            OSAL_IOWR('p', IOC_MMZ_PHYS_TO_HANDLE_NR, mmb_user_info)

#define IOC_MMB_MEM_SHARE          OSAL_IOW('s', IOC_MMB_MEM_SHARE_NR, struct mmb_share_info)
#define IOC_MMB_MEM_UNSHARE        OSAL_IOW('s', IOC_MMB_MEM_UNSHARE_NR, struct mmb_share_info)
#define IOC_MMB_MEM_SHARE_ALL      OSAL_IOW('s', IOC_MMB_MEM_SHARE_ALL_NR, struct mmb_share_info)
#define IOC_MMB_MEM_UNSHARE_ALL    OSAL_IOW('s', IOC_MMB_MEM_UNSHARE_ALL_NR, struct mmb_share_info)
#define IOC_MMB_VIRT_GET_SYS_MEM   OSAL_IOWR('s', IOC_MMB_VIRT_GET_SYS_MEM_NR, struct mmb_share_info)
#define IOC_MMB_PHYS_GET_SYS_MEM   OSAL_IOWR('s', IOC_MMB_PHYS_GET_SYS_MEM_NR, struct mmb_share_info)
#define IOC_MMB_HANDLE_GET_SYS_MEM OSAL_IOWR('s', IOC_MMB_HANDLE_GET_SYS_MEM_NR, struct mmb_share_info)

#define IOC_MMB_ADD_REF            OSAL_IO('r', IOC_MMB_ADD_REF_NR) /* ioctl(file, cmd, arg), arg is mmb_addr */
#define IOC_MMB_DEC_REF            OSAL_IO('r', IOC_MMB_DEC_REF_NR) /* ioctl(file, cmd, arg), arg is mmb_addr */

#define IOC_MMB_FLUSH_DCACHE       OSAL_IO('c', IOC_MMB_FLUSH_DCACHE_NR)

#define IOC_MMB_FLUSH_DCACHE_DIRTY OSAL_IOW('d', IOC_MMB_FLUSH_DCACHE_DIRTY_NR, struct dirty_area)
#define IOC_MMB_TEST_CACHE         OSAL_IOW('t', IOC_MMB_TEST_CACHE_NR, struct mmb_info)

#define MMZ_SETUP_CMDLINE_LEN      256
typedef struct {
    char mmz[MMZ_SETUP_CMDLINE_LEN];
    char map_mmz[MMZ_SETUP_CMDLINE_LEN];
    int anony;
} mmz_module_params_s;

typedef struct {
    unsigned long gfp;
    unsigned long phys_start;
    unsigned long nbytes;
    unsigned long block_align;
    unsigned int alloc_type;
} ot_mmz_create_attr;

typedef struct {
    const char *mmz_name;
    const char *buf_name;
    unsigned long start;        /* alloc_v3 used */
    unsigned long size;
    unsigned long align;
    unsigned long gfp;
    unsigned int order;         /* alloc_v2/alloc_in_v2 used */
    const ot_mmz_t *user_mmz;   /* alloc_in/alloc_in_v2 used */
    int kernel_only;
} ot_mmz_alloc_para_in;

typedef struct {
    int erro_num;
} ot_mmz_alloc_para_out;


/*
 * APIs
 */
ot_mmz_t *ot_mmz_create(const char *name, unsigned long gfp, unsigned long phys_start, unsigned long nbytes);
ot_mmz_t *ot_mmz_create_v2(const char *name, const ot_mmz_create_attr *mmz_create_attr);

int ot_mmz_destroy(const ot_mmz_t *zone);

int ot_mmz_register(ot_mmz_t *zone);
int ot_mmz_unregister(ot_mmz_t *zone);
ot_mmz_t *ot_mmz_find(unsigned long gfp, const char *mmz_name);

ot_mmb_t *ot_mmb_alloc(const ot_mmz_alloc_para_in *para_in);
ot_mmb_t *ot_mmb_alloc_v2(const ot_mmz_alloc_para_in *para_in);
ot_mmb_t *ot_mmb_alloc_v3(const ot_mmz_alloc_para_in *para_in, ot_mmz_alloc_para_out *para_out);
int ot_mmb_free(ot_mmb_t *mmb);
ot_mmb_t *ot_mmb_getby_phys(unsigned long addr);
ot_mmb_t *ot_mmb_getby_phys_2(unsigned long addr, unsigned long *out_offset);
ot_mmb_t *ot_mmb_getby_kvirt(const void *virt);
unsigned long usr_virt_to_phys(unsigned long virt);

int ot_map_mmz_check_phys(unsigned long addr_start, unsigned long addr_len);

int ot_is_phys_in_mmz(unsigned long addr_start, unsigned long addr_len);

int ot_mmb_check_mem_share(const ot_mmb_t *mmb);
int ot_mmb_check_mem_share_with_pid(const ot_mmb_t *mmb, int pid);
int ot_mmz_check_phys_addr(unsigned long phys_addr, unsigned long size);
int ot_mmz_get_mem_process_isolation(void);

int ot_vma_check(unsigned long vm_start, unsigned long vm_end);
int ot_mmb_flush_dcache_byaddr_safe(void *kvirt, unsigned long phys_addr, unsigned long length);

unsigned long ot_mmz_get_phys(const char *zone_name);

ot_mmb_t *ot_mmb_alloc_in(const ot_mmz_alloc_para_in *para_in);
ot_mmb_t *ot_mmb_alloc_in_v2(const ot_mmz_alloc_para_in *para_in);

#define ot_mmb_freeby_phys(phys_addr) ot_mmb_free(ot_mmb_getby_phys(phys_addr))
#define ot_mmb_freeby_kvirt(kvirt) ot_mmb_free(ot_mmb_getby_kvirt(kvirt))

void *ot_mmb_map2kern(ot_mmb_t *mmb);
void *ot_mmb_map2kern_cached(ot_mmb_t *mmb);

int ot_mmb_flush_dcache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length);
int ot_mmb_invalid_cache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long length);

int ot_mmb_unmap(ot_mmb_t *mmb);
int ot_mmb_get(ot_mmb_t *mmb);
int ot_mmb_put(ot_mmb_t *mmb);

void *ot_mmf_map2kern_nocache(unsigned long phys, int len);
void *ot_mmf_map2kern_cache(unsigned long phys, int len);
void ot_mmf_unmap(void *virt);
int ot_mmz_get_anony(void);
struct osal_list_head *ot_mmz_get_mmz_list(void);

/* for mmz userdev */
int mmz_userdev_init(void);
void mmz_userdev_exit(void);
int mmz_flush_dcache_all(void);

#endif
