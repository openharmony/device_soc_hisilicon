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

#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/semaphore.h>
#include <linux/semaphore.h>
#include <asm/cacheflush.h>
#include <linux/securec.h>
#include "drv_mmz.h"
#include "drv_tzsmmu.h"
#include "drv_media_mem.h"

#define HI_ZERO 0

#define MAX_MEMBLOCK_NUM (1024 * 1024)

/* func: set secure flag and create sec smmu map
 * input no-secure mem info(smmu addr or phys addr,indicated by iommu),
 * and return secure address( sec smmu addr or phys addr)
 */
u32 secmem_alloc(mmb_addr_t phyaddr, unsigned int iommu)
{
    u32 size;
    int ret;
    struct tee_mem_info teebuf = {0};
    struct tee_mem_addr mem_addr = {0};
    u32 base_addr = 0;

    teebuf.table = get_meminfo(phyaddr, iommu, &size, &base_addr);
    if (teebuf.table == NULL) {
        hi_mmz_warn("Cannot get meminfo and check parameter!\n");
        goto out;
    }
    teebuf.size = PAGE_ALIGN(size);

    mem_addr.iommu = iommu;
    mem_addr.addr = base_addr;
    ret = hisi_secmem_alloc(&teebuf, &mem_addr, NULL, 0); /* 0 do not used */
    if (ret || !mem_addr.sec_addr) {
        goto out;
    }

    /* set mmb flag to indicate that the mem is secure */
    set_sec_mmb_flag(phyaddr, iommu);
    sec_mmb_get(phyaddr, iommu, mem_addr.sec_addr);

    return iommu ? mem_addr.sec_addr : phyaddr;

out:
    return MMB_ADDR_INVALID;
}

/* func: clear secure flag and unmap the sec smmu
 * input sec mem info (sec smmu addr or phys addr, indicated by iommu),
 * and output non-sec info (smmu addr or phys addr)
 */
u32 secmem_free(mmb_addr_t sec_addr, unsigned int iommu)
{
    int ret;
    u32 addr;
    int sec_smmu_ref;
    struct tee_mem_info teebuf = {0};
    u32 size = 0;
    u32 base_addr = 0;
    u32 offset;
    u32 sec_base_addr;

    if (iommu) {
        addr = (u32)get_nonsecsmmu_by_secsmmu(sec_addr);
    } else {
        addr = (u32)sec_addr;
    }

    teebuf.table = get_meminfo(addr, iommu, &size, &base_addr);
    if (teebuf.table == NULL) {
        hi_mmz_warn("Cannot get meminfo and check parameter!\n");
        goto exit;
    }
    offset = addr - base_addr;
    sec_base_addr = sec_addr - offset;
    sec_smmu_ref = sec_mmb_put(base_addr, iommu);
    if (sec_smmu_ref) {
        /*
         * free later. warning: map sec-smmu many times
         */
        goto exit;
    }

    ret = hisi_secmem_free(sec_base_addr, iommu);
    if (ret) {
        hi_mmz_warn("hisi_secmem_free failed!\n");
        goto out;
    }

    clr_sec_mmb_flag(base_addr, iommu);

    return base_addr;

out:
    sec_mmb_get(addr, iommu, sec_base_addr);
exit:
    return MMB_ADDR_INVALID;
}

/* func: map to sec smmu
 * addr:input, smmu or phy address in normal world
 * iommu: address type
 * sec smmu address is returned when exec success
 * MMB_ADDR_INVALID is returned when exec failed
 */
u32 secmem_map_to_secsmmu(HI_U32 phyaddr, int iommu)
{
    struct tee_mem_info teebuf = {0};
    struct tee_mem_addr mem_addr = {0};
    u32 base_addr = 0;
    u32 size = 0;
    int ret;
    u32 offset;

    if (iommu) {
        mem_addr.sec_addr = get_sec_smmu_by_nosmmu(phyaddr);
    } else {
        mem_addr.sec_addr = get_sec_smmu_by_phys(phyaddr);
    }
    if (mem_addr.sec_addr != MMB_ADDR_INVALID) {
        /* map to sec smmu before,and give the previous value */
        offset = 0;
        goto out;
    }

    teebuf.table = get_meminfo(phyaddr, iommu, &size, &base_addr);
    if (teebuf.table == NULL) {
        hi_mmz_warn("Cannot get meminfo and check parameter!\n");
        goto exit;
    }
    teebuf.size = PAGE_ALIGN(size);

    mem_addr.iommu = iommu;
    mem_addr.addr = base_addr;
    ret = hisi_secmem_mapto_secsmmu(&teebuf, &mem_addr, NULL, 0); /* 0 not used */
    if (ret) {
        hi_mmz_warn("hisi_secmem_mapto_secsmmu failed!\n");
        goto exit;
    }
    offset = phyaddr - mem_addr.addr;
out:
    sec_mmb_get(phyaddr, iommu, mem_addr.sec_addr);

    return (mem_addr.sec_addr + offset);

exit:
    return MMB_ADDR_INVALID;
}

/* func: unmap from sec smmu
 * secsmmu:input, smmu address in sec world
 * iommu: memtype indicate the mem attr(mmz or system)
 * 0 is returned when exec success
 * non-zero is returned when exec failed
 */
int secmem_unmap_from_secsmmu(HI_U32 sec_addr, int iommu)
{
    int ret;
    int sec_smmu_ref;
    unsigned int addr;
    struct tee_mem_info teebuf = {0};
    u32 size = 0;
    u32 base_addr = 0;
    u32 sec_addr_base;

    if (iommu) {
        /* the mem is system mem */
        addr = get_nonsecsmmu_by_secsmmu(sec_addr);
    } else {
        /* the mem is system mem */
        addr = get_phys_by_secsmmu(sec_addr);
    }
    teebuf.table = get_meminfo(addr, iommu, &size, &base_addr);
    if (teebuf.table == NULL) {
        hi_mmz_warn("Cannot get meminfo and check parameter!\n");
        goto err;
    }

    sec_addr_base = sec_addr - (addr - base_addr);
    sec_smmu_ref = sec_mmb_put(base_addr, iommu);
    if (sec_smmu_ref) {
        /* free later. warning: map sec-smmu many times */
        goto out;
    }

    if (is_sec_mem(base_addr, iommu)) {
        /* mem is secure mem, and should be clear sec flag and unmap sec smmu. */
        ret = hisi_secmem_free(sec_addr_base, 1);
        if (ret != 0) {
            hi_mmz_warn("hisi_secmem_free failed!\n");
            goto exit;
        }
        clr_sec_mmb_flag(base_addr, iommu);
    } else {
        /*
         * mem is normal mem, and just should be unmap sec smmu.
         */
        ret = hisi_secmem_unmap_from_secsmmu(sec_addr_base);
        if (ret) {
            hi_mmz_warn("hisi_secmem_unmap_from_secsmmu failed!\n");
            goto exit;
        }
    }

    /*
     * the mem may not be free here, but it is just a repair the mistake for the exceptional calling order like:
     * 1 secmem_alloc --- secmem_map_to_secsmmu --- secmem_free --- secmem_unmap_from_secsmmu
     * 2 nomal alloc --- secmem_map_to_secsmmu --- normal free --- secmem_unmap_from_secsmmu
     */
    sec_delay_release_for_mem(base_addr, iommu);
out:
    return 0;
exit:
    sec_mmb_get(base_addr, iommu, sec_addr_base);
err:
    return -1;
}

int hi_tee_agent_end(void)
{
    return hisi_secmem_agent_end();
}

int sec_mem_get(HI_U32 sec_addr, int iommu)
{
    HI_U32 addr = MMB_ADDR_INVALID;

    if (iommu) {
        addr = get_nonsecsmmu_by_secsmmu(sec_addr);
    } else {
        addr = sec_addr;
    }

    if (addr == MMB_ADDR_INVALID) {
        hi_mmz_warn("err args:addr:0x%x iommu:%d!\n", sec_addr, iommu);
        return HI_FAILURE;
    }

    if (!is_sec_mem(addr, iommu)) {
        hi_mmz_warn("The mem should be sec!\n");
        return HI_FAILURE;
    }

    /* mem sec_smmu_ref is not 0, ignore sec_addr */
    return sec_mmb_get(addr, iommu, sec_addr);
}

int sec_mem_put(HI_U32 sec_addr, int iommu)
{
    HI_U32 addr = MMB_ADDR_INVALID;
    int ret;

    if (iommu) {
        addr = get_nonsecsmmu_by_secsmmu(sec_addr);
    } else {
        addr = sec_addr;
    }

    if (addr == MMB_ADDR_INVALID) {
        hi_mmz_warn("err args: addr:0x%x iommu:%d!\n", sec_addr, iommu);
        return HI_FAILURE;
    }

    if (!is_sec_mem(addr, iommu)) {
        hi_mmz_warn("The mem should be sec!\n");
        return HI_FAILURE;
    }

    ret = sec_mmb_put(addr, iommu);
    if (ret < 0) {
        hi_mmz_warn("call wrong times, the sec ref is %d.\n", ret);
        return HI_FAILURE;
    } else if (ret > 0) {
        /* just decrement the ref */
        return HI_SUCCESS;
    } else {
        ret = hisi_secmem_free(sec_addr, iommu);
        if (ret) {
            hi_mmz_warn("hisi_secmem_free failed!\n");
            return HI_FAILURE;
        }

        clr_sec_mmb_flag(addr, iommu);
        delete_mmb(addr, iommu);
    }

    return HI_SUCCESS;
}

int sec_mem_buf_query_ref(HI_U32 sec_addr, int iommu, HI_U32 *ref)
{
    if (ref == NULL) {
        hi_mmz_warn("ref should not be null!\n");
        return HI_FAILURE;
    }

    return sec_mmb_query_ref(sec_addr, iommu, ref);
}

EXPORT_SYMBOL(secmem_alloc);
EXPORT_SYMBOL(secmem_free);
EXPORT_SYMBOL(secmem_map_to_secsmmu);
EXPORT_SYMBOL(secmem_unmap_from_secsmmu);
EXPORT_SYMBOL(hi_tee_agent_end);
EXPORT_SYMBOL(sec_mem_get);
EXPORT_SYMBOL(sec_mem_put);
EXPORT_SYMBOL(sec_mem_buf_query_ref);

