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

#include <linux/mman.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/vmalloc.h>
#include <linux/list.h>

#include "securec.h"
#include "drv_media_mem.h"
#include "drv_mmz_ioctl.h"
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
#include "drv_tzsmmu.h"
#endif
#include "drv_tee_smmu_agent.h"
#include "himedia.h"

#ifdef CONFIG_COMPAT
#include "drv_mmz_compat.h"
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 0)
#include <linux/dma-buf.h>
#include <linux/dma-direction.h>
#endif
#include <linux/hisilicon/hisi_iommu.h>

#define HI_ZERO 0

DEFINE_SEMAPHORE(process_lock);
LIST_HEAD(release_list);
struct release_info {
    hil_mmb_t *mmb;
    struct list_head list;
};

int mmz_flush_dcache_mmb(const struct mmb_info *mi)
{
    hil_mmb_t *mmb = NULL;
    struct mmb_udata *udata = NULL;
    struct mmb_udata *p = NULL;
    unsigned int phyaddr;
    unsigned int iommu;

    if (mi == NULL) {
        hi_mmz_warn("err args!\n");
        return -EINVAL;
    }
    /*
     * If pmi->phys_addr is not  MMB_ADDR_INVALID, pmi->phys_addr
     * is userd first.Because pmi->smmu_addr may be 0 and it is legal at
     * the same time, but pmi->phys_addr hasn't such feature.So phys_addr
     * is stricter.
     */
    if (mi->phys_addr != MMB_ADDR_INVALID) {
        phyaddr = mi->phys_addr;
        iommu = 0;
    } else {
        phyaddr = mi->smmu_addr;
        iommu = 1;
    }

    mmb = hil_mmb_getby_phys(phyaddr, iommu);
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    if (mmb == NULL) {
        hi_mmz_warn("err args!\n");
        return -EINVAL;
    }
#else
    if (mmb == NULL || mmb->handle == NULL) {
        hi_mmz_warn("err args!\n");
        return -EINVAL;
    }
#endif

    spin_lock(&mmb->u_lock);
    if (list_empty(&mmb->ulist)) {
        if (iommu) {
            hi_mmz_warn("mmb smmu:0x%x have not mapped yet!\n", (unsigned int)phyaddr);
        } else {
            hi_mmz_warn("mmb phy:0x%x have not mapped yet!\n", (unsigned int)phyaddr);
        }
        spin_unlock(&mmb->u_lock);
        return -EINVAL;
    }

    list_for_each_entry(p, &mmb->ulist, list) {
        if (p->tgid == current->tgid) {
            udata = p;
            break;
        }
    }
    spin_unlock(&mmb->u_lock);

    if (udata == NULL || udata->map_cached == 0 || udata->uvirt == NULL) {
        hi_mmz_debug("error!\n");
        return -EINVAL;
    }

#ifndef DMABUF_FLUSH_CACHE
    flush_inner_cache((void *)udata->uvirt, mmb->length);
    flush_outer_cache_range(phyaddr, mmb->length, iommu);
#else
    dma_buf_end_cpu_access(mmb->handle, DMA_TO_DEVICE); // clean cache
    dma_buf_begin_cpu_access(mmb->handle, DMA_FROM_DEVICE); // invalid cache
#endif

    return 0;
}

int mmz_flush_all_dcache(void)
{
#ifdef CONFIG_SMP
    on_each_cpu((smp_call_func_t)mmz_flush_dcache_all, NULL, 1);
#else
    mmz_flush_dcache_all();
#endif

#ifndef CONFIG_64BIT
    /* just for A9 core */
    outer_flush_all();
#endif
    return 0;
}

static int mmz_userdev_open(struct inode *inode, struct file *file)
{
    struct mmz_userdev_info *pmu = NULL;

    if (file == NULL) {
        hi_mmz_error("err args!\n");
        return -ENOMEM;
    }

    pmu = kmalloc(sizeof(*pmu), GFP_KERNEL);
    if (pmu == NULL) {
        hi_mmz_error("alloc mmz_userdev_info failed!\n");
        return -ENOMEM;
    }
    pmu->tpid = current->tgid;
    sema_init(&pmu->sem, 1);
    pmu->mmap_tpid = 0;
    file->private_data = (void *)pmu;

    return HI_SUCCESS;
}

static int ioctl_mmb_alloc(const struct file *file, unsigned int iocmd, struct mmb_info *mi)
{
    hil_mmb_t *mmb = NULL;

    mmb = hil_mmb_alloc(mi->mmb_name, mi->size, mi->align, mi->mmz_name, HI_USER_ALLOC);
    if (mmb == NULL) {
        return -ENOMEM;
    }

    if (mmb->iommu) {
        mi->smmu_addr = mmb->iommu_addr;
    } else {
        mi->phys_addr = mmb->phys_addr;
    }

    return HI_SUCCESS;
}

static hil_mmb_t *get_mmb_info(unsigned int addr, unsigned int iommu)
{
    hil_mmb_t *p = NULL;

    p = hil_mmb_getby_phys(addr, iommu);
    if (p == NULL) {
        hi_mmz_warn("mmb(0x%08X) not found!\n", addr);
        return NULL;
    }

    return p;
}

static int ioctl_mmb_user_unmap(const struct file *file, unsigned int iocmd, struct mmb_info *mi);

static int ioctl_mmb_free(const struct file *file, unsigned int iocmd, const struct mmb_info *mi)
{
    int ret;
    unsigned int iommu;
    unsigned int phyaddr;
    hil_mmb_t *mmb = NULL;

    if (mi->phys_addr != MMB_ADDR_INVALID) {
        phyaddr = mi->phys_addr;
        iommu = 0;
    } else {
        phyaddr = mi->smmu_addr;
        iommu = 1;
    }

    mmb = hil_mmb_getby_phys(phyaddr, iommu);
    if (mmb == NULL) {
        hi_mmz_warn("mmb free failed!\n");
        return -EPERM;
    }

    down(&g_mmz_lock);
    ret = hil_mmb_free(mmb);
    up(&g_mmz_lock);

    return ret;
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 0)
static int ioctl_mmb_get(const struct file *file, unsigned int iocmd, const struct mmb_info *mi)
{
    int ret;
    unsigned int iommu;
    unsigned int phyaddr;

    if (mi->phys_addr != MMB_ADDR_INVALID) {
        phyaddr = mi->phys_addr;
        iommu = 0;
    } else {
        phyaddr = mi->smmu_addr;
        iommu = 1;
    }

    ret = mmb_get(phyaddr, iommu);

    return ret;
}

static int ioctl_mmb_put(const struct file *file, unsigned int iocmd, const struct mmb_info *mi)
{
    int ret;
    unsigned int iommu;
    unsigned int phyaddr;

    if (mi->phys_addr != MMB_ADDR_INVALID) {
        phyaddr = mi->phys_addr;
        iommu = 0;
    } else {
        phyaddr = mi->smmu_addr;
        iommu = 1;
    }

    ret = mmb_put(phyaddr, iommu);

    return ret;
}

static int ioctl_mmb_query_ref(const struct file *file, unsigned int iocmd, struct mmb_info *mi)
{
    int ret;
    unsigned int iommu;
    unsigned int phyaddr;
    unsigned int ref;

    if (mi->phys_addr != MMB_ADDR_INVALID) {
        phyaddr = mi->phys_addr;
        iommu = 0;
    } else {
        phyaddr = mi->smmu_addr;
        iommu = 1;
    }

    ret = mmb_ref_query(phyaddr, iommu, &ref);
    if (ret == HI_SUCCESS) {
        mi->ref = ref;
    }

    return ret;
}

static int ioctl_mmb_query_source(const struct file *file, unsigned int iocmd, struct mmb_info *mi)
{
    int ret;
    int source;

    if (mi->smmu_addr == MMB_ADDR_INVALID) {
        return HI_FAILURE;
    }

    ret = mem_source_query(mi->smmu_addr, &source);
    if (ret == HI_SUCCESS) {
        mi->source = source;
    }

    return ret;
}

int dma_buf_export_fd(unsigned int phyaddr, unsigned int iommu)
{
    hil_mmb_t *mmb = NULL;
    mmb = hil_mmb_getby_phys(phyaddr, iommu);
    if (mmb == NULL || mmb->handle == NULL) {
        hi_mmz_warn("export dma buf fd failed!\n");
        return -EPERM;
    }

    return hi_dma_buf_fd(mmb->handle, O_RDWR | O_CLOEXEC);
}

static int ioctl_dma_buf_export_fd(const struct file *file, unsigned int iocmd, struct mmb_info *mi)
{
    int fd;
    unsigned int iommu;
    unsigned int phyaddr;

    if (mi->phys_addr != MMB_ADDR_INVALID) {
        phyaddr = mi->phys_addr;
        iommu = 0;
    } else {
        phyaddr = mi->smmu_addr;
        iommu = 1;
    }

    fd = dma_buf_export_fd(phyaddr, iommu);
    if (fd < 0) {
        return HI_FAILURE;
    }
    mi->fd = fd;

    return HI_SUCCESS;
}
#endif

static int ioctl_mmb_get_pgtable_addr(const struct file *file, unsigned int iocmd, struct mmb_info *mi)
{
    unsigned long pt_addr_long = (unsigned long)MMB_ADDR_INVALID;
    unsigned long err_rd_addr_long = (unsigned long)MMB_ADDR_INVALID;
    unsigned long err_wr_addr_long = (unsigned long)MMB_ADDR_INVALID;

    hisi_get_iommu_ptable_addr(&pt_addr_long, &err_rd_addr_long, &err_wr_addr_long);

    if (pt_addr_long == (unsigned long)MMB_ADDR_INVALID
            || err_rd_addr_long == (unsigned long)MMB_ADDR_INVALID
            || err_wr_addr_long == (unsigned long)MMB_ADDR_INVALID) {
        return HI_FAILURE;
    }

    mi->pt_addr = (unsigned int)pt_addr_long;
    mi->err_rd_addr = (unsigned int)err_rd_addr_long;
    mi->err_wr_addr = (unsigned int)err_wr_addr_long;

    return HI_SUCCESS;
}

static int ioctl_mmb_user_map_cace_attr(hil_mmb_t *mmb, struct mmb_info *mi, int cached, unsigned long *offset)
{
    struct mmb_udata *p = NULL;
    unsigned int phyaddr;
    unsigned int iommu;
    unsigned long offset_tmp;
    int ret = 1; /* 1, not map cache attr */

    if (mi->phys_addr != MMB_ADDR_INVALID) {
        phyaddr = mi->phys_addr;
        iommu = 0;
    } else {
        phyaddr = mi->smmu_addr;
        iommu = 1;
    }

    if (iommu) {
        offset_tmp = phyaddr - mmb->iommu_addr;
    } else {
        offset_tmp = phyaddr - mmb->phys_addr;
    }
    *offset = offset_tmp;

    spin_lock(&mmb->u_lock);
    if ((mmb->map_ref > 0) && (!list_empty(&mmb->ulist))) {
        list_for_each_entry(p, &mmb->ulist, list) {
            if (p->tgid == current->tgid) {
                if (p->map_cached == cached) {
                    mmb->phy_ref++;
                    mmb->map_ref++;
                    mi->mapped = (void *)((uintptr_t)p->uvirt + offset_tmp);
                    p->map_ref++;
                    ret = 0;  /* 0, map cache attr */
                    break;
                } else {
                    hi_mmz_warn("mmb<%s> already mapped one cache attr, can not be remap to other attr\n", mmb->name);
                    ret = -EINVAL;
                    break;
                }
            }
        }
    }
    spin_unlock(&mmb->u_lock);

    return ret;
}

static unsigned long ioctl_mmb_user_remap_get_virt_addr(struct file *file, struct mmb_udata *udata, hil_mmb_t *mmb,
                                                        int cached, const struct mmb_info *mi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    unsigned long prot, flags, len, addr;
#if !(LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0))
    unsigned long round_up_len;
#endif

    /*
     * ion_map framework is used here,so rule must fit to ion ramwork.
     * vma->vm_pgoff is set to 0. pgoff will be passed to vma->vm_pgoff.
     */
    const unsigned long pgoff = 0;
    addr = 0;
    len = PAGE_ALIGN(mmb->length);

    prot = mi->prot;
    if (!prot || !(prot & PROT_READ) || !(prot & PROT_WRITE)) {
        prot = prot | PROT_READ | PROT_WRITE;
    }
    flags = mi->flags;
    if (flags == 0) {
        flags = MAP_SHARED;
    }
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    down_write(&current->mm->mmap_sem);
#else
    down_write(&current->mm->mmap_lock);
#endif
    udata->tgid = current->tgid;
    udata->map_cached = cached;

    /* The following 3 structures are needed in mmap function. */
    pmu->mmap_tpid = current->tgid;
    pmu->private_data = udata;
    pmu->tmp = mmb;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
    addr = do_mmap_pgoff(file, addr, len, prot, flags, pgoff);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 0)
    addr = do_mmap(file, addr, len, prot, flags, pgoff, &round_up_len, NULL);
#elif LINUX_VERSION_CODE >  KERNEL_VERSION(4, 14, 0)
    addr = do_mmap_pgoff(file, addr, len, prot, flags, pgoff, &round_up_len, NULL);
#else
    addr = do_mmap_pgoff(file, addr, len, prot, flags, pgoff, &round_up_len);
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
    up_write(&current->mm->mmap_sem);
#else
    up_write(&current->mm->mmap_lock);
#endif
    if (IS_ERR_VALUE((uintptr_t)addr)) {
        hi_mmz_error("do_mmap_pgoff(file, 0, %lu, 0x%08lX, 0x%08lX, 0x%08lX) return 0x%08lX\n",
                     len, prot, flags, pgoff, addr);
        pmu->mmap_tpid = 0;
        pmu->tmp = NULL;
        pmu->private_data = NULL;
        return 0;
    }

    /* Following 3 parameters need to clear for next remap operation. */
    pmu->mmap_tpid = 0;
    pmu->tmp = NULL;
    pmu->private_data = NULL;

    return addr;
}

static int ioctl_mmb_user_remap(struct file *file, unsigned int iocmd, struct mmb_info *mi, int cached)
{
    hil_mmb_t *mmb = NULL;
    struct mmb_udata *udata = NULL;
    unsigned long addr;
    unsigned int iommu;
    unsigned int phyaddr;
    unsigned long offset = 0;
    int ret;

    /*
     * If pmi->phys_addr is not MMB_ADDR_INVALID , pmi->phys_addr is userd first.
     * Because pmi->smmu_addr may be 0 and it is legal atthe same time,
     * but pmi->phys_addr hasn't such feature.So phys_addr is stricter.
     */
    if (mi->phys_addr != MMB_ADDR_INVALID) {
        phyaddr = mi->phys_addr;
        iommu = 0;
    } else {
        phyaddr = mi->smmu_addr;
        iommu = 1;
    }

    mmb = get_mmb_info(phyaddr, iommu);
    if (mmb == NULL) {
        return -EPERM;
    }

    down(&g_mmz_lock);
    ret = ioctl_mmb_user_map_cace_attr(mmb, mi, cached, &offset);
    if (ret != 1) {
        up(&g_mmz_lock);
        return ret;
    }

    udata = kzalloc(sizeof(struct mmb_udata), GFP_KERNEL);
    if (udata == NULL) {
        hi_mmz_warn("nomem!\n");
        up(&g_mmz_lock);
        return HI_FAILURE;
    }

    addr = ioctl_mmb_user_remap_get_virt_addr(file, udata, mmb, cached, mi);
    if (addr == 0) {
        kfree(udata);
        up(&g_mmz_lock);
        return HI_FAILURE;
    }

    udata->uvirt = (void *)(uintptr_t)addr;
    udata->map_ref++;

    spin_lock(&mmb->u_lock);
    list_add_tail(&udata->list, &mmb->ulist);
    spin_unlock(&mmb->u_lock);

    mmb->phy_ref++;
    mmb->map_ref++;
    mi->mapped = (void *)((uintptr_t)udata->uvirt + offset);
    up(&g_mmz_lock);
    return 0;
}

static int ioctl_mmb_user_get_udata(hil_mmb_t *mmb, struct mmb_udata **udata)
{
    struct mmb_udata *p = NULL;

    spin_lock(&mmb->u_lock);
    if (list_empty(&mmb->ulist)) {
        hi_mmz_warn("[%s] addr(0x%08X) haven't been user-mapped yet!\n",
                    mmb->name, mmb->iommu ? mmb->iommu_addr : mmb->phys_addr);
        spin_unlock(&mmb->u_lock);
        return -EIO;
    }

    list_for_each_entry(p, &mmb->ulist, list) {
        if (current->tgid == p->tgid) {
            *udata = p;
            break;
        }
    }
    spin_unlock(&mmb->u_lock);

    return 0;  /* 0, success */
}

static int ioctl_mmb_user_reference_decrease(hil_mmb_t *mmb, struct mmb_udata *udata)
{
    uintptr_t addr;
    unsigned int len;
    int ret = 0;
    int ref;

    addr = (uintptr_t)udata->uvirt;
    len = PAGE_ALIGN(mmb->length);

    ref = udata->map_ref - 1;
    if (!ref) {
        /*
         * we register struct vm_operations_struct when mmap called
         * so ref count is decreased in vm_operations_struct->close,
         */
#if LINUX_VERSION_CODE <  KERNEL_VERSION(5, 10, 0)
        down_write(&current->mm->mmap_sem);
#else
        down_write(&current->mm->mmap_lock);
#endif
#if LINUX_VERSION_CODE <  KERNEL_VERSION(4, 14, 0)
        ret = do_munmap(current->mm, addr, len);
#else
        ret = do_munmap(current->mm, addr, len, NULL);
#endif
#if LINUX_VERSION_CODE <  KERNEL_VERSION(5, 10, 0)
        up_write(&current->mm->mmap_sem);
#else
        up_write(&current->mm->mmap_lock);
#endif

        if (!ret) {
            /* the memory may be used by system later so clean the L2 cache(L1 cache is guaranteed by do_munmap) */
            udata->uvirt = NULL;
        }
        spin_lock(&mmb->u_lock);
        udata->map_ref--;
        mmb->map_ref--;
        mmb->phy_ref--;
        list_del(&udata->list);
        spin_unlock(&mmb->u_lock);
        kfree(udata);
        udata = NULL;
    } else {
        spin_lock(&mmb->u_lock);
        udata->map_ref--;
        mmb->map_ref--;
        mmb->phy_ref--;
        spin_unlock(&mmb->u_lock);
    }

    return ret;
}

static int ioctl_mmb_user_unmap(const struct file *file, unsigned int iocmd, struct mmb_info *mi)
{
    hil_mmb_t *mmb = NULL;
    struct mmb_udata *udata = NULL;
    unsigned int iommu;
    unsigned int phyaddr;
    int ret;

    /*
     * If pmi->phys_addr is not MMB_ADDR_INVALID, pmi->phys_addr is userd first.
     * Because pmi->smmu_addr may be 0 and it is legal atthe same time,
     * but pmi->phys_addr hasn't such feature. So phys_addr is stricter.
     */
    phyaddr = (mi->phys_addr != MMB_ADDR_INVALID) ? mi->phys_addr : mi->smmu_addr;
    iommu = (mi->phys_addr != MMB_ADDR_INVALID) ? 0 : 1;

    mmb = get_mmb_info(phyaddr, iommu);
    if (mmb == NULL) {
        return -EPERM;
    }

    down(&g_mmz_lock);
    if (!mmb->map_ref) {
        hi_mmz_warn("[%s] addr(0x%08X) haven't been mapped yet!\n",
                    mmb->name, mmb->iommu ? mmb->iommu_addr : mmb->phys_addr);
        mi->mapped = NULL;
        up(&g_mmz_lock);
        return -EIO;
    }

    ret = ioctl_mmb_user_get_udata(mmb, &udata);
    if (ret) {
        mi->mapped = NULL;
        up(&g_mmz_lock);
        return ret;
    }

    if (udata == NULL) {
        /* do not call mmap yourself or unmap is called when process is killed */
        hi_mmz_warn("do not call mmap() yourself!\n");
        up(&g_mmz_lock);
        return 0;
    }

    if (udata->map_cached) {
        up(&g_mmz_lock);
#ifndef DMABUF_FLUSH_CACHE
        mmz_flush_dcache_area((void *)udata->uvirt, (size_t)mmb->length);
        flush_outer_cache_range(phyaddr, mmb->length, iommu);
#else
        dma_buf_end_cpu_access(mmb->handle, DMA_TO_DEVICE); // clean cache
        dma_buf_begin_cpu_access(mmb->handle, DMA_FROM_DEVICE); // invalid cache
#endif
        down(&g_mmz_lock);
    }

    udata->unmap_flag = (udata->unmap_flag | NORMAL_FLAG);

    ret = ioctl_mmb_user_reference_decrease(mmb, udata);
    if ((mmb->phy_ref == 0) && (mmb->map_ref == 0) && (mmb->cma_smmu_ref == 0)) {
        hil_mmb_free(mmb);
    }

    mi->mapped = NULL;
    up(&g_mmz_lock);
    return ret;
}

/* find mmbinfo by use addr */
static hil_mmb_t *get_mmbinfo_byusraddr(unsigned long addr, struct mmb_udata *udata)
{
    hil_mmb_t *mmb = NULL;
    struct mmb_udata *p = NULL;
    hil_mmz_t *zone = NULL;
    int ret;

    if (addr == (unsigned long)NULL) {
        return NULL;
    }

    down(&g_mmz_lock);
    list_for_each_entry(zone, &g_mmz_list, list) {
        struct rb_node *n;
        for (n = rb_first(&zone->root); n; n = rb_next(n)) {
            hil_mmb_t *m = NULL;
            if (zone->iommu) {
                m = rb_entry(n, hil_mmb_t, s_node);
            } else {
                m = rb_entry(n, hil_mmb_t, node);
            }

            if ((!m->map_ref) && list_empty(&m->ulist)) {
                continue;
            }
            spin_lock(&m->u_lock);
            list_for_each_entry(p, &m->ulist, list) {
                if ((p->tgid == current->tgid) && ((uintptr_t)p->uvirt <= addr) &&
                    ((uintptr_t)p->uvirt + m->length > addr)) {
                    mmb = m;
                    ret = memcpy_s(udata, sizeof(struct mmb_udata), p, sizeof(struct mmb_udata));
                    if (ret != EOK) {
                        hi_mmz_warn("memcpy failed\n");
                        mmb = NULL;
                    }
                    spin_unlock(&m->u_lock);
                    goto end;
                }
            }
            spin_unlock(&m->u_lock);
        }
    }
end:
    up(&g_mmz_lock);
    return mmb;
}

static int ioctl_mmb_user_getphyaddr(const struct file *file, unsigned int iocmd, struct mmb_info *mi)
{
    hil_mmb_t *p = NULL;
    struct mmb_udata udata;
    int ret;

    ret = memset_s(&udata, sizeof(udata), 0, sizeof(udata));
    if (ret != EOK) {
        hi_mmz_warn("memset failed\n");
        return -EPERM;
    }

    p = get_mmbinfo_byusraddr((uintptr_t)mi->mapped, &udata);
    if (p == NULL) {
        return -EPERM;
    }
    if (p->iommu_addr != MMB_ADDR_INVALID) {
        mi->smmu_addr = p->iommu_addr + ((uintptr_t)mi->mapped - (uintptr_t)udata.uvirt);
    }

    if (p->phys_addr != MMB_ADDR_INVALID) {
        mi->phys_addr = p->phys_addr + ((uintptr_t)mi->mapped - (uintptr_t)udata.uvirt);
    }

    mi->size = p->length - ((uintptr_t)mi->mapped - (uintptr_t)udata.uvirt);
    return 0;
}

static int ioctl_mmb_user_cma_mapto_iommu(const struct file *file, unsigned int iocmd, struct mmb_info *mi)
{
    mmb_addr_t addr;

    addr = hil_mmb_cma_mapto_iommu(mi->phys_addr, 0);
    if (addr == MMB_ADDR_INVALID) {
        hi_mmz_warn("Phys:0x%x  cma mapto smmu failed!\n", (unsigned int)mi->phys_addr);
        return -EPERM;
    }
    mi->smmu_addr = addr;
    return 0;
}

static int ioctl_mmb_user_cma_unmapfrom_iommu(const struct file *file, unsigned int iocmd, const struct mmb_info *mi)
{
    int ret;

    ret = hil_mmb_cma_unmapfrom_iommu(mi->smmu_addr, 1);
    if (ret == HI_FAILURE) {
        hi_mmz_warn("smmu:0x%x  cma unmapfrom smmu failed!\n", (unsigned int)mi->smmu_addr);
        return -EPERM;
    }
    return HI_SUCCESS;
}

int mmz_userdev_ioctl_m(struct inode *inode, struct file *file, unsigned int cmd, struct mmb_info *mi)
{
    int ret = 0;
    switch (_IOC_NR(cmd)) {
        case _IOC_NR(IOC_MMB_ALLOC):
            ret = ioctl_mmb_alloc(file, cmd, mi);
            break;
        case _IOC_NR(IOC_MMB_FREE):
            ret = ioctl_mmb_free(file, cmd, mi);
            break;
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 0)
        case _IOC_NR(IOC_MMB_GET):
            ret = ioctl_mmb_get(file, cmd, mi);
            break;
        case _IOC_NR(IOC_MMB_PUT):
            ret = ioctl_mmb_put(file, cmd, mi);
            break;
#endif
        case _IOC_NR(IOC_MMB_USER_REMAP):
            ret = ioctl_mmb_user_remap(file, cmd, mi, 0);
            break;
        case _IOC_NR(IOC_MMB_USER_REMAP_CACHED):
            ret = ioctl_mmb_user_remap(file, cmd, mi, 1);
            break;
        case _IOC_NR(IOC_MMB_USER_UNMAP):
            ret = ioctl_mmb_user_unmap(file, cmd, mi);
            break;
        case _IOC_NR(IOC_MMB_USER_GETPHYADDR):
            ret = ioctl_mmb_user_getphyaddr(file, cmd, mi);
            break;
        case _IOC_NR(IOC_MMB_USER_CMA_MAPTO_SMMU):
            ret = ioctl_mmb_user_cma_mapto_iommu(file, cmd, mi);
            break;
        case _IOC_NR(IOC_MMB_USER_CMA_UNMAPTO_SMMU):
            ret = ioctl_mmb_user_cma_unmapfrom_iommu(file, cmd, mi);
            break;
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 10, 0)
        case _IOC_NR(IOC_MMB_USER_QUERY_REF):
            ret = ioctl_mmb_query_ref(file, cmd, mi);
            break;
        case _IOC_NR(IOC_MMB_USER_QUERY_SRC):
            ret = ioctl_mmb_query_source(file, cmd, mi);
            break;
        case _IOC_NR(IOC_DMA_BUF_EXPORT_FD):
            ret = ioctl_dma_buf_export_fd(file, cmd, mi);
            break;
#endif
        case _IOC_NR(IOC_SMMU_GET_PGTABLE_ADDR):
            ret = ioctl_mmb_get_pgtable_addr(file, cmd, mi);
            break;
        default:
            hi_mmz_error("invalid ioctl cmd = %08X\n", cmd);
            ret = -EINVAL;
            break;
    }

    return ret;
}

static long mmz_userdev_ioctl_get_data(unsigned int cmd, unsigned long arg, void *data, size_t size)
{
    int ret;

    if (size != _IOC_SIZE(cmd) || arg == 0) {
        hi_mmz_error("_IOC_SIZE(cmd) = %d, arg = 0x%08lX\n", _IOC_SIZE(cmd), arg);
        return -EINVAL;
    }

    ret = memset_s(data, size, 0, size);
    if (ret != EOK) {
        hi_mmz_warn("memset failed\n");
        return -EINVAL;
    }

    if (copy_from_user(data, (void *)(uintptr_t)arg, _IOC_SIZE(cmd))) {
        hi_mmz_error("copy_from_user error.\n");
        return -EINVAL;
    }

    return 0;
}

static int mmz_userdev_ioctl_c(unsigned int cmd, unsigned long arg)
{
    struct mmb_info mmi;
    int ret;

    if (arg == 0) {
        mmz_flush_all_dcache();
        ret = 0;
        goto __error_exit;
    }

    ret = mmz_userdev_ioctl_get_data(cmd, arg, (void *)&mmi, sizeof(mmi));
    if (ret) {
        hi_mmz_error("get data error.\n");
        goto __error_exit;
    }

    switch (_IOC_NR(cmd)) {
        case _IOC_NR(IOC_MMB_FLUSH_DCACHE): {
            mmz_flush_dcache_mmb(&mmi);
            ret = 0;
            break;
        }
        default:
            ret = -EINVAL;
            break;
    }

__error_exit:
    return ret;
}

static long mmz_userdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long ret = 0;
    struct mmz_userdev_info *pmu = NULL;

    if (file == NULL || file->private_data == NULL) {
        hi_mmz_error("invaled params!\n");
        return -EINVAL;
    }

    pmu = file->private_data;
    down(&pmu->sem);

    if (_IOC_TYPE(cmd) == 'm') {
        struct mmb_info mi = {0};

        ret = mmz_userdev_ioctl_get_data(cmd, arg, (void *)&mi, sizeof(mi));
        if (ret) {
            hi_mmz_error("get data error.\n");
            goto __error_exit;
        }

        ret = mmz_userdev_ioctl_m(file->f_path.dentry->d_inode, file, cmd, &mi);
        if (!ret && (cmd & IOC_OUT)) {
            if (copy_to_user((void *)(uintptr_t)arg, &mi, _IOC_SIZE(cmd))) {
                hi_mmz_error("mmz_userdev_ioctl: copy_to_user error.\n");
                ret = -EFAULT;
                goto __error_exit;
            }
        }
    } else if (_IOC_TYPE(cmd) == 'c') {
        ret = mmz_userdev_ioctl_c(cmd, arg);
        if (ret) {
            hi_mmz_error("mmz_userdev_ioctl_c error.\n");
            goto __error_exit;
        }
    } else {
        ret = -EINVAL;
    }

__error_exit:
    up(&pmu->sem);
    return ret;
}

static void mmz_vm_open(struct vm_area_struct *vma)
{
    return;
}

static void mmz_vm_close(struct vm_area_struct *vma)
{
    return;
}

static struct vm_operations_struct g_mmz_vma_ops = {
    .open = mmz_vm_open,
    .close = mmz_vm_close,
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
static int mmz_userdev_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct mmz_userdev_info *pmu = NULL;
    hil_mmb_t *mmb = NULL;
    struct mmb_udata *udata = NULL;
    int map_fd;
    struct file *map_file = NULL;
    unsigned int cached;
    int ret;

    if (vma == NULL || file == NULL || file->private_data == NULL) {
        hi_mmz_error("sys err\n");
        return -EPERM;
    }

    pmu = file->private_data;
    mmb = pmu->tmp;
    udata = pmu->private_data;

    if (mmb == NULL || udata == NULL) {
        return -EPERM;
    }

    if (current->tgid != pmu->mmap_tpid) {
        hi_mmz_error("do not call mmap() yourself!\n");
        return -EPERM;
    }

    /*
     * only ION_FLAG_CACHED is meanings mapped cached and build map when page
     * is used in Missing page exception.
     * ION_FLAG_CACHED | ION_FLAG_CACHED_NEEDS_SYNC means cached mapped and
     * build map as soon as the func is called.
     */
    if (udata->map_cached) {
        cached = ION_FLAG_CACHED | ION_FLAG_CACHED_NEEDS_SYNC;
    } else {
        cached = 0;
    }

    if (mmb->client == NULL || mmb->handle == NULL) {
        return -EBADF;
    }
    set_buffer_cached(mmb->client, mmb->handle, cached);
    map_fd = ion_share_dma_buf_fd(mmb->client, mmb->handle);
    map_file = fget(map_fd);
    if (map_file == NULL || map_file->f_op == NULL) {
        goto err;
    }

    if (!map_file->f_op->mmap) {
        goto err;
    }

    /*
     * Ref count is decrease in vma->vm_ops->closed. And this satisfy:
     * 1 we can decrease count of ref before release func is called
     * 2 no effect in normal operation process
     */
    vma->vm_private_data = mmb;
    vma->vm_ops = &g_mmz_vma_ops;
    mmz_vm_open(vma);

    ret = map_file->f_op->mmap(map_file, vma);
    fput(map_file);
    sys_close(map_fd);
    return ret;
err:
    return -EBADF;
}
#else
int mmz_userdev_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct mmz_userdev_info *pmu = NULL;
    struct mmb_udata *udata = NULL;
    hil_mmb_t *mmb = NULL;
    int ret;

    if (vma == NULL || file == NULL || file->private_data == NULL) {
        hi_mmz_error("sys err\n");
        return -EPERM;
    }

    pmu = file->private_data;
    mmb = pmu->tmp;
    udata = pmu->private_data;

    if (mmb == NULL || mmb->handle == NULL
                    || udata == NULL) {
        return -EPERM;
    }
    if (current->tgid != pmu->mmap_tpid) {
        hi_mmz_error("do not call mmap() yourself!\n");
        return -EPERM;
    }

    /*
     * Ref count is decrease in vma->vm_ops->closed. And this satisfy:
     * 1 we can decrease count of ref before release func is called
     * 2 no effect in normal operation process
     */
    vma->vm_private_data = mmb;
    vma->vm_ops = &g_mmz_vma_ops;
    if (!udata->map_cached) {
        vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
    }

    mmz_vm_open(vma);

    ret = dma_buf_mmap(mmb->handle, vma, vma->vm_pgoff);
    if (ret) {
        return -EBADF;
    }

    return ret;
}
#endif

#if LINUX_VERSION_CODE <  KERNEL_VERSION(5, 10, 0)
/* check mmb before use this func */
static int force_secmem_free(const hil_mmb_t *mmb)
{
    int ret;
    unsigned int base_addr;
    while (mmb->sec_smmu_ref) {
        if (mmb->sec_flag) {
            if (mmb->iommu) {
                base_addr = secmem_free(mmb->sec_smmu, 1);
            } else {
                base_addr = secmem_free(mmb->phys_addr, 0);
            }
            if (base_addr == 0) {
                hi_mmz_warn("secmem_free failed, mmb->iommu = %d! \n", mmb->iommu);
            }
        } else {
            ret = secmem_unmap_from_secsmmu(mmb->sec_smmu, mmb->iommu);
            if (ret != 0) {
                hi_mmz_warn("secmem_unmap_from_secsmmu failed! \n");
                return -1;
            }
        }
    }

    return 0;
}
#endif

static void force_mmb_free(hil_mmb_t *mmb)
{
    struct mmb_kdata *p = NULL;
    int ref;
    int ret;

    if (mmb == NULL) {
        hi_mmz_warn("err args ,free mmb failed\n");
        return;
    }

#if LINUX_VERSION_CODE <  KERNEL_VERSION(5, 10, 0)
    ret = force_secmem_free(mmb);
    if (ret) {
        hi_mmz_warn("force_secmem_free failed\n");
        return;
    }
#endif

    if (!mmb->iommu) {
        while (mmb->cma_smmu_ref) {
            ret = hil_mmb_cma_unmapfrom_iommu(mmb->iommu_addr, !mmb->iommu);
            if (ret) {
                hi_mmz_warn("unmap iommu failed!\n");
                return;
            }
        }
    }

    p = mmb->kdata;
    if (p != NULL) {
        ref = p->kmap_ref;
        while (ref) {
            p->kmap_ref--;
            mmb->map_ref--;
            mmb->phy_ref--;
            ref = p->kmap_ref;
            if (!p->kmap_ref) {
                /* need to check as user space */
                vunmap(p->kvirt);
                kfree(p);
                p = NULL;
                mmb->kdata = NULL;
            }
        }
    }

    down(&g_mmz_lock);
    hil_mmb_free(mmb);
    up(&g_mmz_lock);
}

static void force_mem_free(void)
{
    struct release_info *p = NULL, *q = NULL;
    int ret;

    ret = list_empty(&release_list);
    if (ret) {
        /* no need to release */
        return;
    }

    list_for_each_entry_safe(p, q, &release_list, list) {
        force_mmb_free(p->mmb);
        list_del(&p->list);
        kfree(p);
    }
}

static int add_release_list(hil_mmb_t *m)
{
    int ret = 0;
    struct release_info *info = NULL;

    if (m == NULL) {
        hi_mmz_warn("sys err\n");
        return -1;
    }

    info = kmalloc(sizeof(struct release_info), GFP_KERNEL);
    if (info == NULL) {
        hi_mmz_warn("no mem, release failed \n");
        return -1;
    }

    ret = memset_s(info, sizeof(struct release_info), 0, sizeof(struct release_info));
    if (ret != 0) {
        return -1;
    }

    info->mmb = m;
    list_add_tail(&info->list, &release_list);

    return 0;
}

static int mmz_userdev_add_mmb_to_release_list(hil_mmb_t *m, pid_t tpid)
{
    struct mmb_udata *q = NULL;
    struct mmb_udata *p = NULL;
    int ret;

    if ((m->flag != HI_KERNEL_ALLOC) && (m->owner_id == tpid)) {
        if (add_release_list(m)) {
            return -1;
        }
    }

    spin_lock(&m->u_lock);
    ret = list_empty(&m->ulist);
    if (!ret) {
        list_for_each_entry_safe(q, p, &m->ulist, list) {
            if ((m->flag == HI_KERNEL_ALLOC) && (q->tgid == tpid)) {
                while (q->map_ref) {
                    q->map_ref--;
                    m->map_ref--;
                    m->phy_ref--;
                }
                list_del(&q->list);
                kfree(q);
                q = NULL;
                if (m->phy_ref == 0) {
                    m->phy_ref = 1;
                    spin_unlock(&m->u_lock);
                    add_release_list(m);
                    spin_lock(&m->u_lock);
                }
                continue;
            }
            /* user-mod alloc, then user-mod use */
            if ((((m->flag != HI_KERNEL_ALLOC)) && (m->owner_id == tpid)) || ((q->tgid == tpid))) {
                while (q->map_ref) {
                    q->map_ref--;
                    m->map_ref--;
                    m->phy_ref--;
                }
                list_del(&q->list);
                kfree(q);
                q = NULL;
            }
        }
    }
    spin_unlock(&m->u_lock);

    return 0;
}

static int mmz_userdev_release(struct inode *inode, struct file *file)
{
    struct mmz_userdev_info *pmu = NULL;
    hil_mmz_t *zone = NULL;
    hil_mmz_t *z = NULL;
    int ret;

    if (file == NULL || file->private_data == NULL) {
        hi_mmz_error("err args!\n");
        return -EPERM;
    }

    pmu = file->private_data;

    down(&process_lock);
    down(&g_mmz_lock);
    list_for_each_entry_safe(zone, z, &g_mmz_list, list) {
        struct rb_node *n;
        for (n = rb_first(&zone->root); n; n = rb_next(n)) {
            hil_mmb_t *m = NULL;
            if (zone->iommu) {
                m = rb_entry(n, hil_mmb_t, s_node);
            } else {
                m = rb_entry(n, hil_mmb_t, node);
            }

            ret = mmz_userdev_add_mmb_to_release_list(m, pmu->tpid);
            if (ret) {
                goto out;
            }
        }
    }
out:
    up(&g_mmz_lock);
    force_mem_free();
    up(&process_lock);
    file->private_data = NULL;
    kfree(pmu);

    return 0;
}

static struct file_operations g_mmz_userdev_fops = {
    .owner = THIS_MODULE,
    .open = mmz_userdev_open,
    .release = mmz_userdev_release,
    .unlocked_ioctl = mmz_userdev_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = compat_mmz_userdev_ioctl,
#endif
    .mmap = mmz_userdev_mmap,
};

#if !(LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0))
#if (HI_PROC_SUPPORT == 1)
static int proc_mmz_read(struct inode *inode, struct file *file)
{
    return single_open(file, mmz_read_proc, PDE_DATA(inode));
}

#define    MAX_BUFFER_LENTH 10

ssize_t proc_mmz_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos)
{
    char   *order_info = NULL;
    size_t len         = MAX_BUFFER_LENTH;
    int    flag        = -1;

    if ((ppos == NULL) || (*ppos >= MAX_BUFFER_LENTH) || (count >= MAX_BUFFER_LENTH)) {
        return -EFBIG;
    }


    order_info = kmalloc(MAX_BUFFER_LENTH + 1, GFP_KERNEL);
    if (order_info == NULL) {
        return -EFAULT;
    }

    len = min(len, count);

    if (copy_from_user(order_info, buffer, len)) {
        kfree(order_info);
        order_info = NULL;
        return -EFAULT;
    }
    order_info[MAX_BUFFER_LENTH] = '\0';

    if (strstr(order_info, "help")) {
        hi_mmz_fatal("Usage:\n");
        hi_mmz_fatal("      echo help >/proc/media-mem : get the help about echo XXX >/proce/media-mem\n");
        hi_mmz_fatal("      echo show >/proc/media-mem : show present print level value\n");
        hi_mmz_fatal("      echo n    >/proc/media-mem : set the print level and 0 <n< 6\n");
        goto end;
    }

    if (strstr(order_info, "show")) {
        hi_mmz_fatal("The mmz print level now is %d\n", g_mmz_print_level);
        goto end;
    }

    flag = (order_info[0]);
    if (len >2 || flag < '1' || flag > '5') {
        hi_mmz_fatal("Input Error, input 'echo help >/proc/media-mem' to get help \n");
        goto end;
    }
    g_mmz_print_level = flag - '0';
    hi_mmz_fatal("Set succeeded, the mmz print level now is %d\n", g_mmz_print_level);

end:
    kfree(order_info);
    order_info = NULL;
    *ppos      = len ;
    return len;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
static struct file_operations g_proc_mmz_fops = {
    .owner = THIS_MODULE,
    .open = proc_mmz_read,
    .read = seq_read,
    .llseek = seq_lseek,
    .write = proc_mmz_write,
    .release = single_release,
};
#else
static struct proc_ops g_proc_mmz_fops = {
    .proc_open = proc_mmz_read,
    .proc_read = seq_read,
    .proc_write = proc_mmz_write,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};
#endif // KERNEL 5.10

#endif // HI_PROC_SUPPORT
#endif

/* ********************** proc ************************* */
#define MEDIA_MEM_NAME "media-mem"
static int media_mem_proc_init(void)
{
#if !(HI_PROC_SUPPORT == 0)
    struct proc_dir_entry *p;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
    p = create_proc_entry(MEDIA_MEM_NAME, 0444, NULL); /* 0444: access permission */
    if (p == NULL) {
        return -1;
    }
    p->read_proc = mmz_read_proc;
    p->write_proc = NULL;
#else
    p = proc_create(MEDIA_MEM_NAME, 0444, NULL, &g_proc_mmz_fops); /* 0444: access permission */
    if (p == NULL) {
        return -1;
    }
#endif

#endif
    return 0;
}

static void media_mem_proc_exit(void)
{
#if !(HI_PROC_SUPPORT == 0)
    remove_proc_entry(MEDIA_MEM_NAME, NULL);
#endif
}

/* ******************* init ************************* */
static pm_device g_mmz_userdev = {
    .minor = HIMEDIA_DYNAMIC_MINOR,
    .name = "mmz_userdev",
    .owner = THIS_MODULE,
    .app_ops = &g_mmz_userdev_fops
};

int drv_mmz_mod_init(void)
{
    HI_S32 ret;

    hi_drv_mmz_init();
    media_mem_proc_init();
    ret = hi_drv_pm_register(&g_mmz_userdev);
    if (ret != HI_SUCCESS) {
        media_mem_proc_exit();
        hi_drv_mmz_exit();
        return HI_FAILURE;
    }
#ifdef MODULE
    hi_mmz_fatal("Load hi_mmz.ko success.\t\t\n");
#endif
    tee_mem_agent_register();

    return 0;
}

void drv_mmz_mod_exit(void)
{
    hi_drv_pm_un_register(&g_mmz_userdev);
    media_mem_proc_exit();
    hi_drv_mmz_exit();
    tee_mem_agent_unregister();

#ifdef MODULE
    hi_mmz_fatal("remove hi_mmz.ko success.\n");
#endif
}

#if defined(MODULE) || defined(CFG_HI_USER_DRV)
module_init(drv_mmz_mod_init);
module_exit(drv_mmz_mod_exit);
#endif

EXPORT_SYMBOL(drv_mmz_mod_init);
EXPORT_SYMBOL(drv_mmz_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hisilicon");

