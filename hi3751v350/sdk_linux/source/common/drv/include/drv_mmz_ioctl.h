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

#ifndef __DRV_MMZ_IOCTL_H__
#define __DRV_MMZ_IOCTL_H__

#define HIL_MAX_NAME_LEN 16

/* remove pclint warning anonymous struct or union */
struct mmb_info {
    HI_U32 phys_addr; /* phys-memory address */
    HI_U32 smmu_addr;
    HI_U32 align;  /* phys-memory's special align size */
    HI_U32 size;   /* length of memory, in bytes */
    HI_U8 *mapped; /* userspace mapped ptr */

    struct {
        HI_U32 prot : 8;      /* PROT_READ or PROT_WRITE */
        HI_U32 flags : 12;    /* MAP_SHARED or MAP_PRIVATE */
        HI_U32 reserved : 12; /* reserved, do not use */
    };
    char mmb_name[HIL_MAX_NAME_LEN];
    char mmz_name[HIL_MAX_NAME_LEN];
    HI_U32 gfp; /* reserved, do set to 0 */

    HI_U32 ref; /* reference count of current memory block */
    HI_U8 source; /* query result of smmu addr */
    HI_S32 fd; /* exported fd of dma buffer */

    HI_U32 pt_addr; /* pg_table addr */
    HI_U32 err_rd_addr;
    HI_U32 err_wr_addr;
};

struct dirty_area {
    HI_U32 dirty_phys_start; /* dirty physical address */
    HI_U8 *dirty_virt_start; /* dirty virtual  address,
    must be coherent with dirty_phys_addr */
    HI_U32 dirty_size;
};

struct sec_info {
    char mmb_name[HIL_MAX_NAME_LEN];
    char mmz_name[HIL_MAX_NAME_LEN];
    HI_U32 sec_smmu;
    HI_U32 nosec_smmu;
    HI_U32 phys_addr;
    HI_U32 size;
    HI_U32 align;
};

#define IOC_MMB_ALLOC _IOWR('m', 10, struct mmb_info)
#define IOC_MMB_FREE _IOW('m', 12, struct mmb_info)
#define IOC_MMB_GET _IOW('m', 13, struct mmb_info)
#define IOC_MMB_PUT _IOW('m', 15, struct mmb_info)

#define IOC_MMB_USER_REMAP        _IOWR('m', 20, struct mmb_info)
#define IOC_MMB_USER_REMAP_CACHED _IOWR('m', 21, struct mmb_info)
#define IOC_MMB_USER_UNMAP        _IOWR('m', 22, struct mmb_info)
#define IOC_MMB_USER_GETPHYADDR   _IOWR('m', 23, struct mmb_info)

#define IOC_MMB_USER_CMA_MAPTO_SMMU   _IOWR('m', 25, struct mmb_info)
#define IOC_MMB_USER_CMA_UNMAPTO_SMMU _IOWR('m', 26, struct mmb_info)
#define IOC_MMB_USER_QUERY_REF    _IOWR('m', 27, struct mmb_info)
#define IOC_MMB_USER_QUERY_SRC    _IOWR('m', 28, struct mmb_info)

#define IOC_MMB_SECSMMU_ALLOC         _IOWR('s', 12, struct sec_info)
#define IOC_MMB_SECSMMU_FREE          _IOWR('s', 13, struct sec_info)
#define IOC_MMB_SECSMMU_MAPTOSECSMMU  _IOWR('s', 10, struct sec_info)
#define IOC_MMB_SECSMMU_UNMAPFROMSMMU _IOWR('s', 11, struct sec_info)

#define IOC_MMB_FLUSH_DCACHE _IOWR('c', 40, struct mmb_info)

#define IOC_MMB_TEST_CACHE _IOW('t', 11, struct mmb_info)

/* notice that these two CMDs are not managed by mmz */
#define IOC_DMA_BUF_EXPORT_FD _IOWR('m', 29, struct mmb_info)
#define IOC_SMMU_GET_PGTABLE_ADDR _IOWR('m', 30, struct mmb_info)

#endif /* __DRV_MMZ_IOCTL_H__ */

