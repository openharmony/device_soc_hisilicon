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

#include <linux/types.h>
#include <linux/smp.h>

#include "hi_type.h"

#include "drv_mmz.h"
#include "drv_media_mem.h"
#include "hi_drv_mmz.h"
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
#include "drv_tzsmmu.h"
#endif

#define HI_ZERO 0

#define HIL_MAX_NAME_LEN 16

HI_S32 hi_drv_mmz_alloc_and_map(const char *bufname, char *zone_name, HI_U32 size, int align, mmz_buffer *mmz_buf)
{
    mmb_addr_t phyaddr;
    const char *name = "--";

    if (mmz_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }
    if (bufname != NULL) {
        name = bufname;
    }

    phyaddr = new_mmb(name, size, align, NULL);
    if (phyaddr == MMB_ADDR_INVALID) {
        hi_mmz_error("Alloc %s failed\n", name);
        goto err;
    }

    mmz_buf->pu8_start_vir_addr = remap_mmb(phyaddr, 0);
    if (mmz_buf->pu8_start_vir_addr == HI_NULL) {
        delete_mmb(phyaddr, 0);
        hi_mmz_error("Remap %s failed\n", name);
        goto err;
    }

    mmz_buf->start_phy_addr = (HI_U32)phyaddr;
    mmz_buf->size = size;

    return HI_SUCCESS;

err:
    mmz_buf->pu8_start_vir_addr = HI_NULL;
    mmz_buf->start_phy_addr = (HI_U32)MMB_ADDR_INVALID;
    mmz_buf->size = 0;

    return HI_FAILURE;
}

HI_VOID hi_drv_mmz_unmap_and_release(mmz_buffer *mmz_buf)
{
    mmb_addr_t phyaddr;

    if (mmz_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return;
    }
    phyaddr = mmz_buf->start_phy_addr;

    if (phyaddr != MMB_ADDR_INVALID) {
        if (mmz_buf->pu8_start_vir_addr != NULL) {
            unmap_mmb((HI_VOID *)mmz_buf->pu8_start_vir_addr);
        }
        delete_mmb(phyaddr, 0);
    }
}

HI_S32 hi_drv_mmz_alloc(const char *bufname, char *zone_name, HI_U32 size, int align, mmz_buffer *mmz_buf)
{
    mmb_addr_t phyaddr;
    char *mmz_name = zone_name;
    const char *name = "--";

    if (mmz_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }
    if (bufname != NULL) {
        name = bufname;
    }

    phyaddr = new_mmb(name, size, align, mmz_name);
    if (phyaddr == MMB_ADDR_INVALID) {
        phyaddr = new_mmb(name, size, align, NULL);
        if (phyaddr == MMB_ADDR_INVALID) {
            hi_mmz_error("Alloc %s failed\n", bufname);
            return HI_FAILURE;
        }
    }

    mmz_buf->pu8_start_vir_addr = HI_NULL;
    mmz_buf->start_phy_addr = (HI_U32)phyaddr;
    mmz_buf->size = size;

    return HI_SUCCESS;
}

HI_S32 hi_drv_mmz_map_cache(mmz_buffer *mmz_buf)
{
    mmb_addr_t phyaddr;

    if (mmz_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }
    phyaddr = mmz_buf->start_phy_addr;

    mmz_buf->pu8_start_vir_addr = remap_mmb_cached(phyaddr, 0);
    if (mmz_buf->pu8_start_vir_addr == HI_NULL) {
        mmz_buf->pu8_start_vir_addr = (HI_U8 *)NULL;
        hi_mmz_error("Remap buf failed\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 hi_drv_mmz_flush(mmz_buffer *mmz_buf)
{
    mmb_addr_t phyaddr;
    HI_U8 *viraddr = NULL;
    mmb_addr_t len;

    if (mmz_buf == NULL || mmz_buf->pu8_start_vir_addr == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }

    viraddr = mmz_buf->pu8_start_vir_addr;
    len = mmz_buf->size;

    if (mmz_buf->start_phy_addr != MMB_ADDR_INVALID) {
        phyaddr = mmz_buf->start_phy_addr;
    } else {
        hi_mmz_error("invalid MMZ param\n");
        return HI_FAILURE;
    }

    flush_cache_kern(phyaddr, (void *)viraddr, len, 0);
    return HI_SUCCESS;
}

HI_S32 hi_drv_mmz_map(mmz_buffer *mmz_buf)
{
    mmb_addr_t phyaddr;

    if (mmz_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }
    phyaddr = mmz_buf->start_phy_addr;
    mmz_buf->pu8_start_vir_addr = remap_mmb(phyaddr, 0);
    if (mmz_buf->pu8_start_vir_addr == HI_NULL) {
        mmz_buf->pu8_start_vir_addr = HI_NULL;
        hi_mmz_error("Remap buf failed\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID hi_drv_mmz_unmap(mmz_buffer *mmz_buf)
{
    if (mmz_buf == NULL || mmz_buf->pu8_start_vir_addr == NULL) {
        return;
    }
    unmap_mmb((HI_VOID *)mmz_buf->pu8_start_vir_addr);
}

HI_VOID hi_drv_mmz_release(mmz_buffer *mmz_buf)
{
    mmb_addr_t phyaddr;

    if (mmz_buf == NULL) {
        hi_mmz_error("err args and free failed!\n");
        return;
    }

    phyaddr = mmz_buf->start_phy_addr;
    if (phyaddr != MMB_ADDR_INVALID) {
        delete_mmb(phyaddr, 0);
    }
}

HI_S32 hi_drv_mmz_get_phy_by_virt(mmz_buffer *mmz_buf)
{
    if (mmz_buf == NULL || mmz_buf->pu8_start_vir_addr == NULL) {
        hi_mmz_error("err arg\n");
        return HI_FAILURE;
    }

    mmz_buf->start_phy_addr = (HI_U32)get_phyaddr_byvirt((HI_VOID *)mmz_buf->pu8_start_vir_addr, 0);
    if (mmz_buf->start_phy_addr != MMB_ADDR_INVALID) {
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 hi_drv_mmz_buffer_get(mmz_buffer *mmz_buf)
{
    if (mmz_buf == NULL) {
        hi_mmz_error("err args and free failed!\n");
        return HI_FAILURE;
    }

    return mmb_buf_get(mmz_buf->start_phy_addr, 0);
}

HI_S32 hi_drv_mmz_buffer_put(mmz_buffer *mmz_buf)
{
    if (mmz_buf == NULL) {
        hi_mmz_error("err args and free failed!\n");
        return HI_FAILURE;
    }

    return mmb_buf_put(mmz_buf->start_phy_addr, 0);
}

HI_S32 hi_drv_mmz_query_buffer_ref(mmz_buffer *mmz_buf, HI_U32 *ref)
{
    HI_U32 mem_ref = 0;
    HI_S32 ret;

    if (mmz_buf == NULL || ref == NULL) {
        hi_mmz_error("err args and Query failed!\n");
        return HI_FAILURE;
    }

    ret = mmb_buf_ref_query(mmz_buf->start_phy_addr, 0, &mem_ref);
    if (ret) {
        hi_mmz_error("mmb_buf_ref_query failed, phys:0x%x \n", mmz_buf->start_phy_addr);
        return HI_FAILURE;
    }
    *ref = mem_ref;

    return HI_SUCCESS;
}

#ifdef HI_SMMU_SUPPORT
HI_S32 hi_drv_smmu_alloc_and_map(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, smmu_buffer *smmu_buf)
{
    mmb_addr_t smmuaddr;

    if (smmu_buf == NULL || bufname == NULL || (size == 0)) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }

    smmuaddr = new_mmb(bufname, size, align, MMZ_SMMU);
    if (smmuaddr == MMB_ADDR_INVALID) {
        hi_mmz_error("Alloc %s failed\n", bufname);
        goto err;
    }

    smmu_buf->pu8_start_vir_addr = remap_mmb(smmuaddr, 1);
    if (smmu_buf->pu8_start_vir_addr == HI_NULL) {
        delete_mmb(smmuaddr, 1);
        hi_mmz_error("Remap %s failed\n", bufname);
        goto err;
    }

    smmu_buf->start_smmu_addr = (HI_U32)smmuaddr;
    smmu_buf->size = size;

    return HI_SUCCESS;

err:
    smmu_buf->size = 0;
    smmu_buf->pu8_start_vir_addr = HI_NULL;
    smmu_buf->start_smmu_addr = (HI_U32)MMB_ADDR_INVALID;

    return HI_FAILURE;
}

HI_VOID hi_drv_smmu_unmap_and_release(smmu_buffer *smmu_buf)
{
    mmb_addr_t smmuaddr;

    if (smmu_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return;
    }

    smmuaddr = (mmb_addr_t)smmu_buf->start_smmu_addr;
    if (smmuaddr != MMB_ADDR_INVALID) {
        if (smmu_buf->pu8_start_vir_addr != NULL) {
            unmap_mmb(smmu_buf->pu8_start_vir_addr);
        }
        delete_mmb(smmuaddr, 1);
    }
}

HI_S32 hi_drv_smmu_alloc(const HI_CHAR *bufname, HI_U32 size, int align, smmu_buffer *smmu_buf)
{
    mmb_addr_t smmuaddr;

    if (smmu_buf == NULL || bufname == NULL || (size == 0)) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }

    smmu_buf->size = 0;
    smmu_buf->pu8_start_vir_addr = HI_NULL;
    smmu_buf->start_smmu_addr = (HI_U32)MMB_ADDR_INVALID;

    smmuaddr = new_mmb(bufname, size, align, MMZ_SMMU);
    if (smmuaddr == MMB_ADDR_INVALID) {
        hi_mmz_error("Alloc %s failed\n", bufname);
        return HI_FAILURE;
    }

    smmu_buf->pu8_start_vir_addr = HI_NULL;
    smmu_buf->start_smmu_addr = (HI_U32)smmuaddr;
    smmu_buf->size = size;

    return HI_SUCCESS;
}

HI_S32 hi_drv_smmu_map_cache(smmu_buffer *smmu_buf)
{
    mmb_addr_t smmuaddr;

    if (smmu_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }

    smmuaddr = smmu_buf->start_smmu_addr;
    if (smmuaddr == MMB_ADDR_INVALID) {
        hi_mmz_error("invalid smmu addr\n");
        return HI_FAILURE;
    }

    smmu_buf->pu8_start_vir_addr = remap_mmb_cached(smmuaddr, 1);
    if (smmu_buf->pu8_start_vir_addr == HI_NULL) {
        smmu_buf->pu8_start_vir_addr = HI_NULL;
        hi_mmz_error("Remap buf failed\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 hi_drv_smmu_flush(smmu_buffer *smmu_buf)
{
    mmb_addr_t smmuaddr;
    HI_U8 *viraddr = NULL;
    mmb_addr_t len;

    if (smmu_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }

    viraddr = smmu_buf->pu8_start_vir_addr;
    len = smmu_buf->size;

    if (smmu_buf->start_smmu_addr != MMB_ADDR_INVALID) {
        smmuaddr = smmu_buf->start_smmu_addr;
    } else {
        hi_mmz_error("invalid MMZ param\n");
        return HI_FAILURE;
    }

    flush_cache_kern(smmuaddr, (void *)viraddr, len, 1);
    return HI_SUCCESS;
}

HI_S32 hi_drv_smmu_map(smmu_buffer *smmu_buf)
{
    mmb_addr_t smmuaddr;

    if (smmu_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }

    smmuaddr = smmu_buf->start_smmu_addr;
    if (smmuaddr == MMB_ADDR_INVALID) {
        hi_mmz_error("invalid smmu addr\n");
        return HI_FAILURE;
    }

    smmu_buf->pu8_start_vir_addr = remap_mmb(smmuaddr, 1);
    if (smmu_buf->pu8_start_vir_addr == HI_NULL) {
        smmu_buf->pu8_start_vir_addr = HI_NULL;
        hi_mmz_error("Remap buf failed\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID hi_drv_smmu_unmap(smmu_buffer *smmu_buf)
{
    if (smmu_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return;
    }
    if (smmu_buf->pu8_start_vir_addr != NULL) {
        unmap_mmb(smmu_buf->pu8_start_vir_addr);
    }
}

HI_VOID hi_drv_smmu_release(smmu_buffer *smmu_buf)
{
    mmb_addr_t smmuaddr;

    if (smmu_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return;
    }

    smmuaddr = (mmb_addr_t)smmu_buf->start_smmu_addr;
    if (smmuaddr != MMB_ADDR_INVALID) {
        delete_mmb(smmuaddr, 1);
    }
}

HI_U32 hi_drv_mmz_map_to_smmu(mmz_buffer *mmz_buf)
{
    mmb_addr_t iommu_addr;
    mmb_addr_t phyaddr;

    if (mmz_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return (HI_U32)MMB_ADDR_INVALID;
    }
    phyaddr = mmz_buf->start_phy_addr;
    iommu_addr = cma_mapto_smmu(phyaddr, 0);
    if (iommu_addr == MMB_ADDR_INVALID) {
        hi_mmz_error("phy:0x%x cma map to smmu failed! \n", (unsigned int)phyaddr);
        return (HI_U32)MMB_ADDR_INVALID;
    }

    return iommu_addr;
}

HI_S32 hi_drv_mmz_unmap_from_smmu(smmu_buffer *smmu_buf)
{
    int ret;
    mmb_addr_t iommu_addr;

    if (smmu_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }
    iommu_addr = smmu_buf->start_smmu_addr;
    ret = cma_unmapfrom_smmu(iommu_addr, 1);
    if (ret == HI_FAILURE) {
        hi_mmz_error("smmu:0x%x cma unmap to smmu failed!\n", (unsigned int)iommu_addr);
        smmu_buf->start_smmu_addr = (HI_U32)MMB_ADDR_INVALID;
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 hi_drv_smmu_get_smmu_addr_by_virt(smmu_buffer *smmu_buf)
{
    if (smmu_buf == NULL || smmu_buf->pu8_start_vir_addr == NULL) {
        hi_mmz_error("err arg\n");
        return HI_FAILURE;
    }

    smmu_buf->start_smmu_addr = (HI_U32)get_phyaddr_byvirt((HI_VOID *)smmu_buf->pu8_start_vir_addr, 1);
    if (smmu_buf->start_smmu_addr != MMB_ADDR_INVALID) {
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

extern void hisi_get_iommu_ptable_addr(unsigned long *pt_addr, unsigned long *err_rdaddr, unsigned long *err_wraddr);

HI_VOID hi_drv_smmu_get_page_table_addr(HI_U32 *pt_addr, HI_U32 *err_rd_addr, HI_U32 *err_wr_addr)
{
    unsigned long pt_addr_long, err_rd_addr_long, err_wr_addr_long;

    if (pt_addr == NULL || err_rd_addr == NULL || err_wr_addr == NULL) {
        hi_mmz_error("invalid param\n");
        return;
    }

    hisi_get_iommu_ptable_addr(&pt_addr_long, &err_rd_addr_long, &err_wr_addr_long);
    *pt_addr = (unsigned int)pt_addr_long;
    *err_rd_addr = (unsigned int)err_rd_addr_long;
    *err_wr_addr = (unsigned int)err_wr_addr_long;
}

HI_S32 hi_drv_smmu_buffer_get(smmu_buffer *smmu_buf)
{
    if (smmu_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }

    return mmb_buf_get(smmu_buf->start_smmu_addr, 1);
}

HI_S32 hi_drv_smmu_buffer_put(smmu_buffer *smmu_buf)
{
    if (smmu_buf == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }

    return mmb_buf_put(smmu_buf->start_smmu_addr, 1);
}

HI_S32 hi_drv_smmu_query_buffer_ref(smmu_buffer *smmu_buf, HI_U32 *ref)
{
    HI_U32 mem_ref = 0;
    HI_S32 ret;

    if (smmu_buf == NULL || ref == NULL) {
        hi_mmz_error("invalid param\n");
        return HI_FAILURE;
    }

    ret = mmb_buf_ref_query(smmu_buf->start_smmu_addr, 1, &mem_ref);
    if (ret) {
        hi_mmz_error("mmb_buf_ref_query failed, smmu:0x%x \n", smmu_buf->start_smmu_addr);
        return HI_FAILURE;
    }
    *ref = mem_ref;

    return HI_SUCCESS;
}

HI_S32 hi_drv_smmu_query_buffer_source(smmu_buffer *smmu_buf, HI_S32 *source)
{
    HI_S32 ret;
    HI_S32 mem_source = 0;

    if (smmu_buf == NULL || source == NULL) {
        hi_mmz_error("err args and Query Source failed!\n");
        return HI_FAILURE;
    }

    ret = query_buffer_source(smmu_buf->start_smmu_addr, &mem_source);
    if (ret) {
        hi_mmz_error("query_buffer_source failed, smmu:0x%x \n", smmu_buf->start_smmu_addr);
        return HI_FAILURE;
    }

    *source = mem_source;

    return HI_SUCCESS;
}

HI_S32 hi_drv_sec_smmu_query_buffer_source(unsigned int sec_smmu, HI_S32 *source)
{
    HI_S32 ret;
    HI_S32 mem_source = 0;

    if (sec_smmu == 0 || source == NULL) {
        hi_mmz_error("err args and Query failed!\n");
        return HI_FAILURE;
    }

    ret = query_secure_buffer_source(sec_smmu, &mem_source);
    if (ret != HI_SUCCESS) {
        hi_mmz_error("query_buffer_source failed, smmu:0x%x \n", sec_smmu);
        return HI_FAILURE;
    }

    *source = mem_source;

    return HI_SUCCESS;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
HI_S32 hi_drv_secmmz_alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, mmz_buffer *secmmz_buf)
{
    mmb_addr_t phyaddr;
    u32 addr;

    if (bufname == HI_NULL) {
        hi_mmz_error("bufname is null\n");
        return HI_FAILURE;
    }

    if (size == 0) {
        hi_mmz_error("bufname=%s, size is 0\n", bufname);
        return HI_FAILURE;
    }
    if (secmmz_buf == NULL) {
        hi_mmz_error("bufname=%s, pSecMBuf is null\n", bufname);
        return HI_FAILURE;
    }

    secmmz_buf->size = 0;
    secmmz_buf->pu8_start_vir_addr = HI_NULL;
    secmmz_buf->start_phy_addr = (HI_U32)MMB_ADDR_INVALID;

    phyaddr = new_mmb(bufname, size, align, NULL);
    if (phyaddr == MMB_ADDR_INVALID) {
        hi_mmz_error("Alloc %s failed\n", bufname);
        return HI_FAILURE;
    }

    addr = secmem_alloc(phyaddr, 0);
    if (addr == MMB_ADDR_INVALID) {
        delete_mmb(phyaddr, 0);
        return HI_FAILURE;
    }

    secmmz_buf->start_phy_addr = (HI_U32)addr;
    secmmz_buf->size = size;

    return HI_SUCCESS;
}

HI_S32 hi_drv_secmmz_release(const mmz_buffer *secmmz_buf)
{
    u32 phys_addr;

    if (secmmz_buf == NULL) {
        hi_mmz_error("invalidate params!\n");
        return HI_FAILURE;
    }

    phys_addr = secmem_free(secmmz_buf->start_phy_addr, 0);
    if (phys_addr == MMB_ADDR_INVALID) {
        hi_mmz_debug("secmem cannot to be free! phys:0x%x \n", secmmz_buf->start_phy_addr);
        return HI_FAILURE;
    }

    delete_mmb(phys_addr, 0);

    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, smmu_buffer *sec_smmu_buf)
{
    mmb_addr_t smmuaddr;
    HI_U32 sec_smmu;

    if (bufname == NULL) {
        hi_mmz_error("bufname is null\n");
        return HI_FAILURE;
    }

    if (size == 0) {
        hi_mmz_error("bufname=%s, size is 0\n", bufname);
        return HI_FAILURE;
    }

    if (sec_smmu_buf == NULL) {
        hi_mmz_error("bufname=%s, pSecSmmuBuf is null\n", bufname);
        return HI_FAILURE;
    }

    sec_smmu_buf->size = 0;
    sec_smmu_buf->pu8_start_vir_addr = HI_NULL;
    sec_smmu_buf->start_smmu_addr = (HI_U32)MMB_ADDR_INVALID;

    smmuaddr = new_mmb(bufname, size, align, MMZ_SMMU);
    if (smmuaddr == MMB_ADDR_INVALID) {
        hi_mmz_error("Alloc %s failed\n", bufname);
        return HI_FAILURE;
    }

    sec_smmu = secmem_alloc(smmuaddr, 1);
    if (sec_smmu == MMB_ADDR_INVALID) {
        delete_mmb(smmuaddr, 1);
        return HI_FAILURE;
    }

    sec_smmu_buf->start_smmu_addr = (HI_U32)sec_smmu;
    sec_smmu_buf->size = size;

    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_release(const smmu_buffer *sec_smmu_buf)
{
    u32 smmuaddr;

    if (sec_smmu_buf == NULL) {
        hi_mmz_error("invalidate params!\n");
        return HI_FAILURE;
    }

    smmuaddr = secmem_free(sec_smmu_buf->start_smmu_addr, 1);
    if (smmuaddr == MMB_ADDR_INVALID) {
        hi_mmz_debug("secmem cannot to be freed! secsmmu:0x%x \n", sec_smmu_buf->start_smmu_addr);
        return HI_FAILURE;
    }

    delete_mmb(smmuaddr, 1);
    return HI_SUCCESS;
}

HI_S32 hi_drv_smmu_map_to_sec_smmu(HI_U32 non_sec_smmu, smmu_buffer *sec_smmu_buf)
{
    HI_U32 sec_smmu;

    if (sec_smmu_buf == NULL) {
        hi_mmz_error("invalidate param!\n");
        return HI_FAILURE;
    }

    sec_smmu = secmem_map_to_secsmmu(non_sec_smmu, 1);
    if (sec_smmu == MMB_ADDR_INVALID) {
        hi_mmz_error("map to sec smmu failed! non_sec_smmu:0x%x \n", non_sec_smmu);
        return HI_FAILURE;
    }

    sec_smmu_buf->start_smmu_addr = sec_smmu;
    sec_smmu_buf->pu8_start_vir_addr = HI_NULL;

    return HI_SUCCESS;
}

HI_S32 hi_drv_smmu_unmap_from_sec_smmu(const smmu_buffer *sec_smmu_buf)
{
    int ret;

    if (sec_smmu_buf == NULL) {
        hi_mmz_error("invalidate param!\n");
        return HI_FAILURE;
    }
    ret = secmem_unmap_from_secsmmu(sec_smmu_buf->start_smmu_addr, 1);
    if (ret) {
        hi_mmz_error("secmem_unmap_from_secsmmu failed!sec-smmu:0x%x \n", sec_smmu_buf->start_smmu_addr);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 hi_drv_mmz_map_to_sec_smmu(const mmz_buffer *mmz_buf, smmu_buffer *sec_smmu_buf)
{
    HI_U32 sec_smmu;

    if (mmz_buf == NULL || sec_smmu_buf == NULL) {
        hi_mmz_error("invalidate param!\n");
        return HI_FAILURE;
    }

    sec_smmu = secmem_map_to_secsmmu(mmz_buf->start_phy_addr, 0);
    if (sec_smmu == MMB_ADDR_INVALID) {
        hi_mmz_error("map to sec smmu failed! phys_addr:0x%x \n", mmz_buf->start_phy_addr);
        return HI_FAILURE;
    }

    sec_smmu_buf->start_smmu_addr = sec_smmu;
    sec_smmu_buf->pu8_start_vir_addr = HI_NULL;

    return HI_SUCCESS;
}

HI_S32 hi_drv_mmz_unmap_from_sec_smmu(const smmu_buffer *sec_smmu_buf)
{
    int ret;

    if (sec_smmu_buf == NULL) {
        hi_mmz_error("invalidate param!\n");
        return HI_FAILURE;
    }
    ret = secmem_unmap_from_secsmmu(sec_smmu_buf->start_smmu_addr, 0);
    if (ret) {
        hi_mmz_error("secmem_unmap_from_secsmmu failed!sec-smmu:0x%x \n", sec_smmu_buf->start_smmu_addr);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 hi_drv_secmmz_buffer_get(mmz_buffer *secmmz_buf)
{
    if (secmmz_buf == NULL) {
        hi_mmz_error("invalidate param!\n");
        return HI_FAILURE;
    }

    return sec_mem_get(secmmz_buf->start_phy_addr, 0);
}

HI_S32 hi_drv_secmmz_buffer_put(mmz_buffer *secmmz_buf)
{
    if (secmmz_buf == NULL) {
        hi_mmz_error("invalidate param!\n");
        return HI_FAILURE;
    }

    return sec_mem_put(secmmz_buf->start_phy_addr, 0);
}

HI_S32 hi_drv_secsmmu_buffer_get(smmu_buffer *sec_smmu_buf)
{
    if (sec_smmu_buf == NULL) {
        hi_mmz_error("invalidate param!\n");
        return HI_FAILURE;
    }

    return sec_mem_get(sec_smmu_buf->start_smmu_addr, 1);
}

HI_S32 hi_drv_secsmmu_buffer_put(smmu_buffer *sec_smmu_buf)
{
    if (sec_smmu_buf == NULL) {
        hi_mmz_error("invalidate param!\n");
        return HI_FAILURE;
    }

    return sec_mem_put(sec_smmu_buf->start_smmu_addr, 1);
}

HI_S32 hi_drv_secmmz_query_buffer_ref(mmz_buffer *secmmz_buf, HI_U32 *ref)
{
    HI_U32 mem_ref = 0;
    HI_S32 ret;

    if (secmmz_buf == NULL || ref == NULL) {
        hi_mmz_error("err args and Query failed!\n");
        return HI_FAILURE;
    }

    ret = sec_mem_buf_query_ref(secmmz_buf->start_phy_addr, 0, &mem_ref);
    if (ret) {
        hi_mmz_error("sec_mem_buf_query_ref failed, sec phys:0x%x \n", secmmz_buf->start_phy_addr);
        return HI_FAILURE;
    }
    *ref = mem_ref;

    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_query_buffer_ref(smmu_buffer *sec_smmu_buf, HI_U32 *ref)
{
    HI_U32 mem_ref = 0;
    HI_S32 ret;

    if (sec_smmu_buf == NULL || ref == NULL) {
        hi_mmz_error("err args and Query failed!\n");
        return HI_FAILURE;
    }

    ret = sec_mem_buf_query_ref(sec_smmu_buf->start_smmu_addr, 1, &mem_ref);
    if (ret) {
        hi_mmz_error("sec_mem_buf_query_ref failed, sec smmu:0x%x \n", sec_smmu_buf->start_smmu_addr);
        return HI_FAILURE;
    }
    *ref = mem_ref;

    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_get_smmu(const smmu_buffer *sec_smmu_buf, smmu_buffer *smmu_buf)
{
    HI_U32 addr;

    if (sec_smmu_buf == NULL || smmu_buf == NULL) {
        hi_mmz_error("invalidate params!\n");
        return HI_FAILURE;
    }

    addr = get_nonsecsmmu_by_secsmmu(sec_smmu_buf->start_smmu_addr);
    if (addr == MMB_ADDR_INVALID) {
        hi_mmz_error("get_nonsecsmmu_by_secsmmu failed!\n");
        return HI_FAILURE;
    }
    smmu_buf->start_smmu_addr = addr;

    return HI_SUCCESS;
}
#else
HI_S32 hi_drv_secmmz_alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, mmz_buffer *secmmz_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secmmz_release(const mmz_buffer *secmmz_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, smmu_buffer *sec_smmu_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_release(const smmu_buffer *sec_smmu_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_smmu_map_to_sec_smmu(HI_U32 non_sec_smmu, smmu_buffer *sec_smmu_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_smmu_unmap_from_sec_smmu(const smmu_buffer *sec_smmu_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_mmz_map_to_sec_smmu(const mmz_buffer *mmz_buf, smmu_buffer *sec_smmu_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_mmz_unmap_from_sec_smmu(const smmu_buffer *sec_smmu_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secmmz_buffer_get(mmz_buffer *secmmz_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secmmz_buffer_put(mmz_buffer *secmmz_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_buffer_get(smmu_buffer *sec_smmu_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_buffer_put(smmu_buffer *sec_smmu_buf)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secmmz_query_buffer_ref(mmz_buffer *secmmz_buf, HI_U32 *ref)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_query_buffer_ref(smmu_buffer *sec_smmu_buf, HI_U32 *ref)
{
    return HI_SUCCESS;
}

HI_S32 hi_drv_secsmmu_get_smmu(const smmu_buffer *sec_smmu_buf, smmu_buffer *smmu_buf)
{
    return HI_SUCCESS;
}

#endif

EXPORT_SYMBOL(hi_drv_mmz_alloc_and_map);
EXPORT_SYMBOL(hi_drv_mmz_unmap_and_release);
EXPORT_SYMBOL(hi_drv_mmz_alloc);
EXPORT_SYMBOL(hi_drv_mmz_map_cache);
EXPORT_SYMBOL(hi_drv_mmz_flush);
EXPORT_SYMBOL(hi_drv_mmz_map);
EXPORT_SYMBOL(hi_drv_mmz_unmap);
EXPORT_SYMBOL(hi_drv_mmz_release);
EXPORT_SYMBOL(hi_drv_mmz_get_phy_by_virt);
EXPORT_SYMBOL(hi_drv_mmz_buffer_get);
EXPORT_SYMBOL(hi_drv_mmz_buffer_put);
EXPORT_SYMBOL(hi_drv_mmz_query_buffer_ref);

#ifdef HI_SMMU_SUPPORT
EXPORT_SYMBOL(hi_drv_smmu_alloc_and_map);
EXPORT_SYMBOL(hi_drv_smmu_unmap_and_release);
EXPORT_SYMBOL(hi_drv_smmu_alloc);
EXPORT_SYMBOL(hi_drv_smmu_release);
EXPORT_SYMBOL(hi_drv_smmu_map);
EXPORT_SYMBOL(hi_drv_smmu_map_cache);
EXPORT_SYMBOL(hi_drv_smmu_unmap);
EXPORT_SYMBOL(hi_drv_smmu_flush);
EXPORT_SYMBOL(hi_drv_mmz_map_to_smmu);
EXPORT_SYMBOL(hi_drv_mmz_unmap_from_smmu);
EXPORT_SYMBOL(hi_drv_smmu_get_smmu_addr_by_virt);
EXPORT_SYMBOL(hi_drv_smmu_get_page_table_addr);
EXPORT_SYMBOL(hi_drv_smmu_buffer_get);
EXPORT_SYMBOL(hi_drv_smmu_buffer_put);
EXPORT_SYMBOL(hi_drv_smmu_query_buffer_ref);
EXPORT_SYMBOL(hi_drv_smmu_query_buffer_source);
EXPORT_SYMBOL(hi_drv_sec_smmu_query_buffer_source);
#endif

EXPORT_SYMBOL(hi_drv_secmmz_alloc);
EXPORT_SYMBOL(hi_drv_secmmz_release);
EXPORT_SYMBOL(hi_drv_secsmmu_alloc);
EXPORT_SYMBOL(hi_drv_secsmmu_release);
EXPORT_SYMBOL(hi_drv_smmu_map_to_sec_smmu);
EXPORT_SYMBOL(hi_drv_smmu_unmap_from_sec_smmu);
EXPORT_SYMBOL(hi_drv_mmz_map_to_sec_smmu);
EXPORT_SYMBOL(hi_drv_mmz_unmap_from_sec_smmu);
EXPORT_SYMBOL(hi_drv_secmmz_buffer_get);
EXPORT_SYMBOL(hi_drv_secmmz_buffer_put);
EXPORT_SYMBOL(hi_drv_secsmmu_buffer_get);
EXPORT_SYMBOL(hi_drv_secsmmu_buffer_put);
EXPORT_SYMBOL(hi_drv_secmmz_query_buffer_ref);
EXPORT_SYMBOL(hi_drv_secsmmu_query_buffer_ref);
EXPORT_SYMBOL(hi_drv_secsmmu_get_smmu);

