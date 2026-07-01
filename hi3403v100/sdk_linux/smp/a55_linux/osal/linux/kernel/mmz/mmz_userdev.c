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

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <linux/dma-mapping.h>
#include <linux/uaccess.h>

#ifdef CONFIG_OT_SYS_SMMU_SUPPORT
#include <linux/iommu.h>
#include "linux/vendor/sva_ext.h"
#endif

#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/cacheflush.h>
#include "securec.h"
#include "osal_mmz.h"
#include "ot_osal.h"
#include "mmz_comm.h"
#include "mm_ext.h"
#include "media_mem.h"
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
#include <linux/libnvdimm.h>
#include "osal_ioctl.h"
#endif

struct mmz_userdev_info {
    pid_t pid;
    pid_t mmap_pid;
    struct semaphore sem;
    struct osal_list_head list;
};

static void mmz_flush_dcache_mmb_dirty(struct dirty_area *p_area)
{
    if (p_area == NULL) {
        return;
    }

#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    __flush_dcache_area(p_area->dirty_virt_start, p_area->dirty_size);
#else
    dcache_clean_inval_poc((unsigned long)p_area->dirty_virt_start,
        (unsigned long)p_area->dirty_virt_start + p_area->dirty_size);
#endif
#else
    /* flush l1 cache, use vir addr */
    __cpuc_flush_dcache_area(p_area->dirty_virt_start, p_area->dirty_size);

#ifdef CONFIG_CACHE_L2X0
    /* flush l2 cache, use paddr */
    outer_flush_range(p_area->dirty_phys_start,
                      p_area->dirty_phys_start + p_area->dirty_size);
#endif
#endif
}

static int mmz_flush_dcache_mmb(const struct mmb_info *pmi)
{
    ot_mmb_t *mmb = NULL;

    if (pmi == NULL) {
        return -EINVAL;
    }

    mmb = pmi->mmb;
    if ((mmb == NULL) || (pmi->map_cached == 0)) {
        osal_trace("%s->%d,error!\n", __func__, __LINE__);
        return -EINVAL;
    }

#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    __flush_dcache_area(pmi->mapped, (size_t)pmi->size);
#else
    dcache_clean_inval_poc((unsigned long)pmi->mapped, (unsigned long)pmi->mapped + pmi->size);
#endif
#else
    /* flush l1 cache, use vir addr */
    __cpuc_flush_dcache_area(pmi->mapped, (size_t)pmi->size);

#ifdef CONFIG_CACHE_L2X0
    /* flush l2 cache, use paddr */
    outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
#endif
#endif
    return 0;
}
#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
void flush_cache_all(void)
{
}
#endif
#endif

/*
 * this function should never be called with local irq disabled,
 * because on_each_cpu marco will raise ipi interrupt.
 */
int mmz_flush_dcache_all(void)
{
#ifdef CONFIG_64BIT
    on_each_cpu((smp_call_func_t)flush_cache_all, NULL, 1);
#else
    on_each_cpu((smp_call_func_t)__cpuc_flush_kern_all, NULL, 1);
    outer_flush_all();
#endif /* CONFIG_64BIT */
    return 0;
}

static int mmz_userdev_open(struct inode *inode, struct file *file)
{
    struct mmz_userdev_info *pmu = NULL;

    osal_unused(inode);

    pmu = kmalloc(sizeof(*pmu), GFP_KERNEL);
    if (pmu == NULL) {
        error_mmz("alloc mmz_userdev_info failed!\n");
        return -ENOMEM;
    }
    (void)memset_s(pmu, sizeof(*pmu), 0, sizeof(*pmu));
    pmu->pid = current->pid;
    pmu->mmap_pid = 0;
    sema_init(&pmu->sem, 1);
    OSAL_INIT_LIST_HEAD(&pmu->list);

    /* This file could be opened just for once */
    file->private_data = (void *)pmu;

    return 0;
}

static int ioctl_mmb_alloc(const struct file *file,
                           unsigned int iocmd,
                           struct mmb_info *pmi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *new_mmbinfo = NULL;
    ot_mmb_t *mmb = NULL;
    ot_mmz_alloc_para_in para_in = {0};

    para_in.mmz_name = pmi->mmz_name;
    para_in.buf_name = pmi->mmb_name;
    para_in.size = pmi->size;
    para_in.align = pmi->align;
    para_in.gfp = pmi->gfp;
    para_in.kernel_only = 0;

    mmb = ot_mmb_alloc(&para_in);
    if (mmb == NULL) {
        error_mmz("ot_mmb_alloc(%s, %llu, 0x%llx, %lu, %s) failed!\n",
            pmi->mmb_name, (unsigned long long)pmi->size, (unsigned long long)pmi->align,
            pmi->gfp, pmi->mmz_name);
        return -ENOMEM;
    }

    new_mmbinfo = kmalloc(sizeof(*new_mmbinfo), GFP_KERNEL);
    if (new_mmbinfo == NULL) {
        ot_mmb_free(mmb);
        error_mmz("alloc mmb_info failed!\n");
        return -ENOMEM;
    }

    if (memcpy_s(new_mmbinfo, sizeof(*new_mmbinfo), pmi, sizeof(*new_mmbinfo)) != EOK) {
        ot_mmb_free(mmb);
        kfree(new_mmbinfo);
        return -EFAULT;
    }
    new_mmbinfo->phys_addr = ot_mmb_phys(mmb);
    new_mmbinfo->mmb = mmb;
    new_mmbinfo->prot = PROT_READ;
    new_mmbinfo->flags = MAP_SHARED;
    osal_list_add_tail(&new_mmbinfo->list, &pmu->list);

    pmi->phys_addr = new_mmbinfo->phys_addr;

    ot_mmb_get(mmb);

    return 0;
}

static int ioctl_mmb_alloc_v2(const struct file *file,
                              unsigned int iocmd, struct mmb_info *pmi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *new_mmbinfo = NULL;
    ot_mmb_t *mmb = NULL;
    ot_mmz_alloc_para_in para_in = {0};

    para_in.mmz_name = pmi->mmz_name;
    para_in.buf_name = pmi->mmb_name;
    para_in.size = pmi->size;
    para_in.align = pmi->align;
    para_in.gfp = pmi->gfp;
    para_in.order = pmi->order;
    para_in.kernel_only = 0;

    mmb = ot_mmb_alloc_v2(&para_in);
    if (mmb == NULL) {
        error_mmz("ot_mmb_alloc_v2(%s, %llu, 0x%llx, %lu, %s) failed!\n",
            pmi->mmb_name, (unsigned long long)pmi->size, (unsigned long long)pmi->align,
            pmi->gfp, pmi->mmz_name);
        return -ENOMEM;
    }

    new_mmbinfo = kmalloc(sizeof(*new_mmbinfo), GFP_KERNEL);
    if (new_mmbinfo == NULL) {
        ot_mmb_free(mmb);
        error_mmz("alloc mmb_info failed!\n");
        return -ENOMEM;
    }

    if (memcpy_s(new_mmbinfo, sizeof(*new_mmbinfo), pmi, sizeof(*pmi)) != EOK) {
        ot_mmb_free(mmb);
        kfree(new_mmbinfo);
        return -EFAULT;
    }
    new_mmbinfo->phys_addr = ot_mmb_phys(mmb);
    new_mmbinfo->mmb = mmb;
    new_mmbinfo->prot = PROT_READ;
    new_mmbinfo->flags = MAP_SHARED;
    osal_list_add_tail(&new_mmbinfo->list, &pmu->list);

    pmi->phys_addr = new_mmbinfo->phys_addr;

    ot_mmb_get(mmb);

    return 0;
}

static int ioctl_mmb_alloc_v3(const struct file *file, unsigned int iocmd, struct mmb_info *pmi)
{
    ot_mmb_t *mmb = NULL;
    ot_mmz_alloc_para_in  para_in = {0};
    ot_mmz_alloc_para_out para_out = {0};
    struct mmb_info *new_mmbinfo = NULL;
    struct mmz_userdev_info *pmu = file->private_data;
    para_in.mmz_name = NULL;
    para_in.buf_name = pmi->mmb_name;
    para_in.start    = pmi->phys_addr;
    para_in.size     = pmi->size;
    para_in.gfp      = pmi->gfp;
    mmb = ot_mmb_alloc_v3(&para_in, &para_out);
    if (mmb == NULL) {
#ifdef CONFIG_PHYS_ADDR_BIT_WIDTH_64
        error_mmz("ot_mmb_alloc_v3(%s, %llu, 0x%llx, %lu) failed!\n", pmi->mmb_name, pmi->size, pmi->align, pmi->gfp);
#else
        error_mmz("ot_mmb_alloc_v3(%s, %lu, 0x%lx, %lu) failed!\n", pmi->mmb_name, pmi->size, pmi->align, pmi->gfp);
#endif
        return -ENOMEM;
    }
    new_mmbinfo = kmalloc(sizeof(*new_mmbinfo), GFP_KERNEL);
    if (new_mmbinfo == NULL) {
        ot_mmb_free(mmb);
        error_mmz("alloc mmb_info failed!\n");
        return -ENOMEM;
    }
    if (memcpy_s(new_mmbinfo, sizeof(*new_mmbinfo), pmi, sizeof(*pmi)) != EOK) {
        ot_mmb_free(mmb);
        kfree(new_mmbinfo);
        return -EFAULT;
    }
    new_mmbinfo->phys_addr = ot_mmb_phys(mmb);
    new_mmbinfo->mmb = mmb;
    new_mmbinfo->prot = PROT_READ;
    new_mmbinfo->flags = MAP_SHARED;
    osal_list_add_tail(&new_mmbinfo->list, &pmu->list);
    pmi->phys_addr = new_mmbinfo->phys_addr;
    ot_mmb_get(mmb);
    return 0;
}
static struct mmb_info *get_mmbinfo(unsigned long addr, const struct mmz_userdev_info *pmu)
{
    struct mmb_info *p = NULL;

    osal_list_for_each_entry(p, &pmu->list, list) {
        if ((addr >= p->phys_addr) && (addr < (p->phys_addr + p->size))) {
            break;
        }
    }
    if (&p->list == &pmu->list) {
        return NULL;
    }

    return p;
}

static struct mmb_info *get_mmbinfo_safe(unsigned long addr, const struct mmz_userdev_info *pmu)
{
    struct mmb_info *p = NULL;

    p = get_mmbinfo(addr, pmu);
    if (p == NULL) {
        error_mmz("mmb(0x%08lX) not found!\n", addr);
        return NULL;
    }

    return p;
}

static int ioctl_mmb_user_unmap(const struct file *file, unsigned int iocmd, struct mmb_info *pmi);

static int _usrdev_mmb_free(struct mmb_info *p)
{
    int ret;

    osal_list_del(&p->list);
    ot_mmb_put(p->mmb);
    ret = ot_mmb_free(p->mmb);
    kfree(p);

    return ret;
}

static int ioctl_mmb_free(const struct file *file,
                          unsigned int iocmd, const struct mmb_info *pmi)
{
    int ret;
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *p = get_mmbinfo_safe(pmi->phys_addr, pmu);

    if ((p == NULL) || (p->mmb == NULL)) {
        return -EPERM;
    }

    if (p->delayed_free) {
        warning("mmb<%s> is delayed_free, can not free again!\n", p->mmb->name);
        return -EBUSY;
    }

    if ((p->map_ref > 0) || (p->mmb_ref > 0)) {
        warning("mmb<%s> is still in use!\n", p->mmb->name);
        p->delayed_free = 1;
        return -EBUSY;
    }

    ret = _usrdev_mmb_free(p);

    return ret;
}

static int ioctl_mmb_attr(const struct file *file,
                          unsigned int iocmd, struct mmb_info *pmi)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *p = NULL;

    if ((p = get_mmbinfo_safe(pmi->phys_addr, pmu)) == NULL) {
        return -EPERM;
    }

    if (memcpy_s(pmi, sizeof(*pmi), p, sizeof(*pmi)) != EOK) {
        return -EFAULT;
    }
    return 0;
}

static int ioctl_mmb_remap_existing(struct mmb_info *p, struct mmb_info *pmi, int cached)
{
    if (cached != p->map_cached) {
        error_mmz("mmb<%s> already mapped as %s, cannot remap as %s.\n",
            p->mmb->name, p->map_cached ? "cached" : "non-cached", cached ? "cached" : "non-cached");
        return -EINVAL;
    }

    p->map_ref++;
    p->mmb_ref++;
    ot_mmb_get(p->mmb);

    /*
     * pmi->phys may not always start at p->phys,
     * and may start with offset from p->phys.
     */
    pmi->mapped = p->mapped + (pmi->phys_addr - p->phys_addr);
    return 0;
}

static int ioctl_mmb_do_user_remap(struct file *file, struct mmz_userdev_info *pmu,
    struct mmb_info *p, struct mmb_info *pmi, int cached)
{
    unsigned long addr = 0;
    unsigned long len = PAGE_ALIGN(p->size);
    unsigned long prot = (pmi->prot == 0) ? p->prot : pmi->prot;
    unsigned long flags = (pmi->flags == 0) ? p->flags : pmi->flags;
    unsigned long pgoff = p->phys_addr;

    pmu->mmap_pid = current->pid;
    p->map_cached = cached;
    addr = vm_mmap(file, addr, len, prot, flags, pgoff);
    pmu->mmap_pid = 0;

    if (IS_ERR_VALUE((uintptr_t)addr)) {
        error_mmz("vm_mmap(file, 0, %lu, 0x%08lX, 0x%08lX, 0x%08lX) return 0x%08lX\n",
            len, prot, flags, pgoff, addr);
        return addr;
    }

    p->mapped = (void *)(uintptr_t)addr;
    p->prot = prot;
    p->flags = flags;
    p->map_ref++;
    p->mmb_ref++;
    ot_mmb_get(p->mmb);
    pmi->mapped = p->mapped + (pmi->phys_addr - p->phys_addr);
    return 0;
}

static int ioctl_mmb_user_remap(struct file *file, unsigned int iocmd, struct mmb_info *pmi, int cached)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *p = NULL;

    if ((p = get_mmbinfo_safe(pmi->phys_addr, pmu)) == NULL) {
        return -EPERM;
    }
    /*
     * mmb could be remapped for more than once, but should not
     * be remapped with confusing cache type.
     */
    if ((p->mapped != NULL) && (p->mmb != NULL) && (p->map_ref > 0)) {
        return ioctl_mmb_remap_existing(p, pmi, cached);
    }

    if (p->phys_addr & ~PAGE_MASK) {
        return -EINVAL;
    }

    return ioctl_mmb_do_user_remap(file, pmu, p, pmi, cached);
}

static int ioctl_mmb_user_unmap_with_cache(const struct mmb_info *p, unsigned long addr, unsigned long len)
{
    struct mm_struct *mm = current->mm;

    mmz_map_down(mm);
    if (ot_vma_check(addr, addr + len)) {
        error_mmz("mmb<%s> vma is invalid.\n", p->mmb->name);
        mmz_map_up(mm);
        return -EPERM;
    }
#ifdef CONFIG_64BIT
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    __flush_dcache_area((void *)(uintptr_t)addr, (size_t)len);
#else
    dcache_clean_inval_poc(addr, addr + len);
#endif
#else
    __cpuc_flush_dcache_area((void *)(uintptr_t)addr, (size_t)len);
#ifdef CONFIG_CACHE_L2X0
    outer_flush_range(p->phys_addr, p->phys_addr + len);
#endif
#endif /* CONFIG_64BIT */
    mmz_map_up(mm);

    return 0;
}

static int ioctl_mmb_user_unmap(const struct file *file, unsigned int iocmd, struct mmb_info *pmi)
{
    int ret;
    unsigned long addr;
    unsigned long len;
    struct mmb_info *p = NULL;
    struct mmz_userdev_info *pmu = file->private_data;

    p = get_mmbinfo_safe(pmi->phys_addr, pmu);
    if (p == NULL) {
        return -EPERM;
    }

    if (p->mapped == NULL) {
#ifdef CONFIG_PHYS_ADDR_BIT_WIDTH_64
        warning("mmb(0x%llx) isn't user-mapped!\n", p->phys_addr);
#else
        warning("mmb(0x%lx) isn't user-mapped!\n", p->phys_addr);
#endif
        pmi->mapped = NULL;
        return -EIO;
    }

    if (!((p->map_ref > 0) && (p->mmb_ref > 0))) {
        error_mmz("mmb<%s> has invalid refer: map_ref=%d, mmb_ref=%d.\n",
            p->mmb->name, p->map_ref, p->mmb_ref);
        return -EIO;
    }

    p->map_ref--;
    p->mmb_ref--;
    ot_mmb_put(p->mmb);

    if (p->map_ref > 0) {
        return 0;
    }

    addr = (unsigned long)(uintptr_t)p->mapped;
    len = PAGE_ALIGN(p->size);

    /* todo,before unmap,refresh cache manually */
    if (p->map_cached) {
        ret = ioctl_mmb_user_unmap_with_cache(p, addr, len);
        if (ret != 0) {
            return ret;
        }
    }

    ret = vm_munmap(addr, len);
    if (!IS_ERR_VALUE((uintptr_t)ret)) {
        p->mapped = NULL;
        pmi->mapped = NULL;
    }

    if (p->delayed_free && (p->map_ref == 0) && (p->mmb_ref == 0)) {
        _usrdev_mmb_free(p);
    }

    return ret;
}

static int ioctl_mmb_virt2phys(const struct file *file,
                               unsigned int iocmd, struct mmb_info *pmi)
{
    int ret = 0;
    unsigned long virt;
    unsigned long phys;
    unsigned long offset = 0;

    osal_unused(file);
    virt = (unsigned long)(uintptr_t)pmi->mapped;
    phys = usr_virt_to_phys(virt);
    if (!phys) {
        ret = -ENOMEM;
    }

    if (ot_mmb_getby_phys_2(phys, &offset) == NULL) {
        error_mmz("Not mmz alloc memory[0x%lx 0x%lx]! 0x%lx\n", virt, phys, offset);
        return -EINVAL;
    }

    pmi->phys_addr = phys;

    return ret;
}

static int ioctl_mmb_mem_share(const struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        error_mmz("invalid mem handle!\n");
        return -EINVAL;
    }

    if (mmb->kernel_only == 1) {
        media_mem_put_mmb(mmb);
        error_mmz("share failed, mmb used only in kernel!\n");
        return -EPERM;
    }

    if (!media_mem_is_check_pid()) {
        media_mem_put_mmb(mmb);
        return 0;
    }

    if (mmb->check_pid != 1) {
        media_mem_put_mmb(mmb);
        error_mmz("buffer is already share all!\n");
        return -EPERM;
    }
    /* pid[0] can share */
    if (osal_get_current_tgid() != mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        error_mmz("only process that alloc buffer can share!\n");
        return -EPERM;
    }
    if (share_info->shared_pid == mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        error_mmz("cannot share to process that alloc buffer!\n");
        return -EPERM;
    }
    if (mmb->pid_num >= OT_MMB_MAX_PID_NUM) {
        media_mem_put_mmb(mmb);
        error_mmz("share failed, max pid_num is %d!\n", OT_MMB_MAX_PID_NUM);
        return -EPERM;
    }
    if (find_pid_in_mmb(mmb, share_info->shared_pid) < 0) {
        /* share */
        mmb->pid[mmb->pid_num++] = share_info->shared_pid;
    }
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_mem_unshare(const struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;
    int cur_pid;
    int pos;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        error_mmz("invalid mem handle!\n");
        return -EINVAL;
    }

    if (mmb->kernel_only == 1) {
        media_mem_put_mmb(mmb);
        error_mmz("unshare failed, mmb used only in kernel!\n");
        return -EPERM;
    }

    if (!media_mem_is_check_pid()) {
        media_mem_put_mmb(mmb);
        return 0;
    }

    if (mmb->check_pid != 1) {
        media_mem_put_mmb(mmb);
        error_mmz("buffer is already share all!\n");
        return -EPERM;
    }
    /* pid[0] and pid[n] can unshare with pid[n] (except pid[0]) */
    pos = find_pid_in_mmb(mmb, share_info->shared_pid);
    if (pos < 0) {
        media_mem_put_mmb(mmb);
        error_mmz("mmb is not shared with pid(%d)!\n", share_info->shared_pid);
        return -EPERM;
    }
    if (share_info->shared_pid == mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        error_mmz("unshare failed, pid(%d) is the process that alloc buffer!\n", share_info->shared_pid);
        return -EPERM;
    }
    cur_pid = osal_get_current_tgid();
    if (cur_pid != mmb->pid[0] && share_info->shared_pid != cur_pid) {
        media_mem_put_mmb(mmb);
        error_mmz("process cannot unshare with pid(%d)!\n", share_info->shared_pid);
        return -EPERM;
    }
    /* unshare */
    clear_pid_in_mmb(mmb, pos);
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_mem_share_all(const struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        error_mmz("invalid mem handle!\n");
        return -EINVAL;
    }

    if (mmb->kernel_only == 1) {
        media_mem_put_mmb(mmb);
        error_mmz("share all failed, mmb used only in kernel!\n");
        return -EPERM;
    }

    if ((!media_mem_is_check_pid()) || (mmb->check_pid != 1)) {
        media_mem_put_mmb(mmb);
        return 0;
    }
    /* pid[0] can share all */
    if (osal_get_current_tgid() != mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        error_mmz("only process that alloc buffer can share all!\n");
        return -EPERM;
    }
    /* share all */
    mmb->check_pid = 0;
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_mem_unshare_all(const struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        error_mmz("invalid mem handle!\n");
        return -EINVAL;
    }

    if (mmb->kernel_only == 1) {
        media_mem_put_mmb(mmb);
        error_mmz("unshare all failed, mmb used only in kernel!\n");
        return -EPERM;
    }

    if ((!media_mem_is_check_pid()) || (mmb->check_pid == 1)) {
        media_mem_put_mmb(mmb);
        return 0;
    }
    /* pid[0] can unshare all */
    if (osal_get_current_tgid() != mmb->pid[0]) {
        media_mem_put_mmb(mmb);
        error_mmz("only process that alloc buffer can unshare all!\n");
        return -EPERM;
    }
    /* unshare all */
    mmb->check_pid = 1;
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_get_sys_mem_by_virt(struct mmb_share_info *share_info)
{
    unsigned long virt_addr;
    unsigned long phys_addr;
    ot_mmb_t *mmb = NULL;

    virt_addr = (unsigned long)(uintptr_t)share_info->virt_addr;
    phys_addr = usr_virt_to_phys(virt_addr);
    if (phys_addr == 0) {
        error_mmz("invalid virt_addr!\n");
        return -EINVAL;
    }

    mmb = media_mem_get_mmb_by_phys(phys_addr);
    if (mmb == NULL) {
        error_mmz("not mmz alloc memory[0x%lx 0x%lx]!\n", virt_addr, phys_addr);
        return -EINVAL;
    }

    share_info->phys_addr = mmb->phys_addr;
    share_info->offset = phys_addr - mmb->phys_addr;
    share_info->mem_handle = mmb;
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_get_sys_mem_by_phys(struct mmb_share_info *share_info)
{
    unsigned long phys_addr;
    ot_mmb_t *mmb = NULL;

    phys_addr = (unsigned long)share_info->phys_addr;
    mmb = media_mem_get_mmb_by_phys(phys_addr);
    if (mmb == NULL) {
        error_mmz("not mmz alloc memory[0x%lx]!\n", phys_addr);
        return -EINVAL;
    }

    if (ot_mmb_check_mem_share(mmb) < 0) {
        media_mem_put_mmb(mmb);
        error_mmz("mem share check failed!\n");
        return -EPERM;
    }

    share_info->phys_addr = mmb->phys_addr;
    share_info->offset = phys_addr - mmb->phys_addr;
    share_info->mem_handle = mmb;
    media_mem_put_mmb(mmb);
    return 0;
}

static int ioctl_mmb_get_sys_mem_by_handle(struct mmb_share_info *share_info)
{
    ot_mmb_t *mmb = NULL;

    mmb = media_mem_get_mmb_by_handle(share_info->mem_handle);
    if (mmb == NULL) {
        error_mmz("invalid mem handle!\n");
        return -EINVAL;
    }

    if (ot_mmb_check_mem_share(mmb) < 0) {
        media_mem_put_mmb(mmb);
        error_mmz("mem share check failed!\n");
        return -EPERM;
    }

    share_info->phys_addr = mmb->phys_addr;
    share_info->offset = 0;
    share_info->mem_handle = mmb;
    media_mem_put_mmb(mmb);
    return 0;
}

int ioctl_mmb_sys_flush_cache(OSAL_PHYS_ADDR_TYPE phy_addr, void *vir_addr, unsigned int size)
{
    unsigned long end_vir_addr;
    unsigned long end_phy_addr;
    unsigned long trans_phy;
    trans_phy = usr_virt_to_phys((unsigned long)(uintptr_t)vir_addr);
    if (trans_phy == 0) {
        error_mmz("start virtual address %p is err.\n", vir_addr);
        return -1;
    }
    if ((trans_phy & 0xFFFFFFFFFFFFFFFEULL) != phy_addr) {
#ifdef CONFIG_PHYS_ADDR_BIT_WIDTH_64
        error_mmz("trans_phy 0x%lx and phy_addr 0x%llx are not equal!\n", trans_phy, phy_addr);
#else
        error_mmz("trans_phy 0x%lx and phy_addr 0x%lx are not equal!\n", trans_phy, phy_addr);
#endif
        return -1;
    }
    end_vir_addr = (unsigned long)(uintptr_t)vir_addr + size - CACHE_LINE_SIZE;
    trans_phy = usr_virt_to_phys(end_vir_addr);
    if (trans_phy == 0) {
        error_mmz("end virtual address 0x%lx is err.\n", end_vir_addr);
        return -1;
    }
    end_phy_addr = phy_addr + size - CACHE_LINE_SIZE;
    if ((trans_phy & 0xFFFFFFFFFFFFFFFEULL) != end_phy_addr) {
        error_mmz("trans_phy 0x%lx and end_phy_addr 0x%lx are not equal!\n", trans_phy, end_phy_addr);
        return -1;
    }
    return ot_mmb_flush_dcache_byaddr_safe(vir_addr, phy_addr, size);
}
int ioctl_mmb_sys_mflush_cache(OSAL_PHYS_ADDR_TYPE phy_addr, void *vir_addr, unsigned int size)
{
    unsigned long end_vir_addr;
    unsigned long end_phy_addr;
    unsigned long trans_phy;
    int ret;
    trans_phy = usr_virt_to_phys((td_uintptr_t)vir_addr);
    if (trans_phy == 0) {
        error_mmz("start virtual address %p is err.\n", vir_addr);
        return -1;
    }
    if ((trans_phy & 0xFFFFFFFFFFFFFFFEULL) != phy_addr) {
        error_mmz("trans_phy 0x%lx and phy_addr 0x%lx are not equal!\n", trans_phy, (unsigned long)phy_addr);
        return -1;
    }
    end_vir_addr = (td_uintptr_t)vir_addr + size - CACHE_LINE_SIZE;
    trans_phy = usr_virt_to_phys(end_vir_addr);
    if (trans_phy == 0) {
        error_mmz("end virtual address 0x%lx is err.\n", end_vir_addr);
        return -1;
    }
    end_phy_addr = (unsigned int)phy_addr + size - CACHE_LINE_SIZE;
    if ((trans_phy & 0xFFFFFFFFFFFFFFFEULL) != end_phy_addr) {
        error_mmz("trans_phy 0x%lx and end_phy_addr 0x%lx are not equal!\n", trans_phy, end_phy_addr);
        return -1;
    }
    ret = ot_mmb_flush_dcache_byaddr_safe(vir_addr, (unsigned int)phy_addr, size);
    if (ret != TD_SUCCESS) {
        return -1;
    }
    return TD_SUCCESS;
}
int ioctl_mmb_check_mmz_phy_addr(OSAL_PHYS_ADDR_TYPE phy_addr, unsigned int len)
{
    if (ot_is_phys_in_mmz((unsigned long)phy_addr, len)) {
        if (ot_map_mmz_check_phys((unsigned long)phy_addr, len)) {
            return -1;
        }
    }
    return 0;
}
int ioctl_mmb_invalid_cache_byaddr(void *kvirt, unsigned long phys_addr, unsigned long len)
{
    return ot_mmb_invalid_cache_byaddr(kvirt, phys_addr, len);
}
int ioctl_mmb_check_phy_in_priv(struct file const *file, struct mmb_info const *pmi)
{
    struct mmb_info *p = NULL;
    struct mmz_userdev_info *pmu = file->private_data;
    p = get_mmbinfo_safe(pmi->phys_addr, pmu);
    if (p == NULL) {
        return -1;
    }
    return 0;
}
int ioctl_mmb_user_mmf_map(struct file const *file, struct mmb_info *pmi)
{
    osal_unused(file);
    pmi->mapped = cmpi_remap_nocache(pmi->phys_addr, pmi->size);
    if (pmi->mapped == NULL) {
        return -1;
    }
    return 0;
}
int ioctl_mmb_user_mmf_map_cache(struct file const *file, struct mmb_info *pmi)
{
    osal_unused(file);
    pmi->mapped = cmpi_remap_cached(pmi->phys_addr, pmi->size);
    if (pmi->mapped == NULL) {
        return -1;
    }
    return 0;
}
int ioctl_mmb_user_mmf_unmap(void *virt_addr)
{
    ot_mmf_unmap(virt_addr);
    return 0;
}

int ioctl_mmb_check_mem_share(mmb_user_info *pmi)
{
    return ot_mmb_check_mem_share((const ot_mmb_t *)pmi->mmb_handle);
}

int ioctl_mmb_check_mem_share_with_pid(mmb_user_info *pmi)
{
    return ot_mmb_check_mem_share_with_pid((const ot_mmb_t *)pmi->mmb_handle, pmi->pid);
}

int ioctl_mmz_check_phys_addr(mmb_user_info *pmi)
{
    return ot_mmz_check_phys_addr(pmi->phys_addr, pmi->size);
}

int ioctl_mmz_get_mem_process_isolation(void)
{
    return ot_mmz_get_mem_process_isolation();
}

int ioctl_cmpi_mmz_phys_to_handle(mmb_user_info *pmi)
{
    pmi->mmb_handle = (void *)cmpi_mmz_phys_to_handle(pmi->phys_addr);
    return 0;
}

int ioctl_mmz_get_phys_addr(struct mmb_info *pmi)
{
    unsigned long phys_addr = ot_mmz_get_phys(pmi->mmz_name);
    pmi->phys_addr = phys_addr;
    return 0;
}

static int mmz_userdev_ioctl_m(struct file *file, unsigned int cmd, struct mmb_info *pmi)
{
    switch (OSAL_IOC_NR(cmd)) {
        case OSAL_IOC_NR(IOC_MMB_ALLOC):
            return ioctl_mmb_alloc(file, cmd, pmi);
        case OSAL_IOC_NR(IOC_MMB_ALLOC_V2):
            return ioctl_mmb_alloc_v2(file, cmd, pmi);
        case OSAL_IOC_NR(IOC_MMB_ALLOC_V3):
            return ioctl_mmb_alloc_v3(file, cmd, pmi);
        case OSAL_IOC_NR(IOC_MMB_ATTR):
            return ioctl_mmb_attr(file, cmd, pmi);
        case OSAL_IOC_NR(IOC_MMB_FREE):
            return ioctl_mmb_free(file, cmd, pmi);
        case OSAL_IOC_NR(IOC_MMB_USER_REMAP):
            return ioctl_mmb_user_remap(file, cmd, pmi, 0);
        case OSAL_IOC_NR(IOC_MMB_USER_REMAP_CACHED):
            return ioctl_mmb_user_remap(file, cmd, pmi, 1);
        case OSAL_IOC_NR(IOC_MMB_USER_UNMAP):
            return ioctl_mmb_user_unmap(file, cmd, pmi);
        case OSAL_IOC_NR(IOC_MMB_VIRT_GET_PHYS):
            return ioctl_mmb_virt2phys(file, cmd, pmi);
        case OSAL_IOC_NR(IOC_MMB_SYS_FLUSH_CACHE):
            return ioctl_mmb_sys_flush_cache(pmi->phys_addr, pmi->mapped, pmi->size);
        case OSAL_IOC_NR(IOC_MMB_SYS_MFLUSH_CACHE):
            return ioctl_mmb_sys_mflush_cache(pmi->phys_addr, pmi->mapped, pmi->size);
        case OSAL_IOC_NR(IOC_MMB_BASE_CHECK_ADDR):
            return ioctl_mmb_check_mmz_phy_addr(pmi->phys_addr, pmi->size);
        case OSAL_IOC_NR(IOC_MMB_INVALID_CACHE):
            return ioctl_mmb_invalid_cache_byaddr(pmi->mapped, pmi->phys_addr, pmi->size);
        case OSAL_IOC_NR(IOC_MMB_CHECK_PHY_ALLOC):
            return ioctl_mmb_check_phy_in_priv(file, pmi);
        case OSAL_IOC_NR(IOC_MMB_MMF_REMAP):
            return ioctl_mmb_user_mmf_map(file, pmi);
        case OSAL_IOC_NR(IOC_MMB_MMF_REMAP_CACHED):
            return ioctl_mmb_user_mmf_map_cache(file, pmi);
        case OSAL_IOC_NR(IOC_MMB_MMF_UNMAP):
            return ioctl_mmb_user_mmf_unmap(pmi->mapped);
        case OSAL_IOC_NR(IOC_MMZ_GET_PHYS_ADDR):
            return ioctl_mmz_get_phys_addr(pmi);
        default:
            error_mmz("invalid ioctl cmd = %08X\n", cmd);
            return -EINVAL;
    }
}

static int mmz_userdev_ioctl_s(struct file *file, unsigned int cmd, struct mmb_share_info *share_info)
{
    switch (OSAL_IOC_NR(cmd)) {
        case OSAL_IOC_NR(IOC_MMB_MEM_SHARE):
            return ioctl_mmb_mem_share(share_info);
        case OSAL_IOC_NR(IOC_MMB_MEM_UNSHARE):
            return ioctl_mmb_mem_unshare(share_info);
        case OSAL_IOC_NR(IOC_MMB_MEM_SHARE_ALL):
            return ioctl_mmb_mem_share_all(share_info);
        case OSAL_IOC_NR(IOC_MMB_MEM_UNSHARE_ALL):
            return ioctl_mmb_mem_unshare_all(share_info);
        case OSAL_IOC_NR(IOC_MMB_VIRT_GET_SYS_MEM):
            return ioctl_mmb_get_sys_mem_by_virt(share_info);
        case OSAL_IOC_NR(IOC_MMB_PHYS_GET_SYS_MEM):
            return ioctl_mmb_get_sys_mem_by_phys(share_info);
        case OSAL_IOC_NR(IOC_MMB_HANDLE_GET_SYS_MEM):
            return ioctl_mmb_get_sys_mem_by_handle(share_info);
        default:
            error_mmz("invalid ioctl cmd = %08X\n", cmd);
            return -EINVAL;
    }
}

static int mmz_userdev_ioctl_r(unsigned int cmd, struct mmb_info *pmi)
{
    switch (OSAL_IOC_NR(cmd)) {
        case OSAL_IOC_NR(IOC_MMB_ADD_REF):
            pmi->mmb_ref++;
            ot_mmb_get(pmi->mmb);
            break;
        case OSAL_IOC_NR(IOC_MMB_DEC_REF):
            if (pmi->mmb_ref <= 0) {
                error_mmz("mmb<%s> mmb_ref is %d!\n", pmi->mmb->name, pmi->mmb_ref);
                return -EPERM;
            }
            pmi->mmb_ref--;
            ot_mmb_put(pmi->mmb);
            if (pmi->delayed_free && (pmi->mmb_ref == 0) && (pmi->map_ref == 0)) {
                _usrdev_mmb_free(pmi);
            }
            break;
        default:
            return -EINVAL;
            break;
    }

    return 0;
}

/* just for test */
static int mmz_userdev_ioctl_t(const struct file *file, unsigned int cmd, const struct mmb_info *pmi);

static int mmz_userdev_ioctl_of_m(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct mmb_info mi = { 0 };
    int ret;

    if ((OSAL_IOC_SIZE(cmd) > sizeof(mi)) || (arg == 0)) {
        error_mmz("OSAL_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", OSAL_IOC_SIZE(cmd), arg);
        return -EINVAL;
    }
    (void)memset_s(&mi, sizeof(mi), 0, sizeof(mi));
    if (copy_from_user(&mi, (void *)(uintptr_t)arg, OSAL_IOC_SIZE(cmd))) {
        osal_trace("\nmmz_userdev_ioctl: copy_from_user error.\n");
        return -EFAULT;
    }
    mi.mmz_name[OT_MMZ_NAME_LEN - 1] = '\0';
    mi.mmb_name[OT_MMB_NAME_LEN - 1] = '\0';
    ret = mmz_userdev_ioctl_m(file, cmd, &mi);
    if (!ret && (cmd & IOC_OUT)) {
        if (copy_to_user((void *)(uintptr_t)arg, &mi, OSAL_IOC_SIZE(cmd))) {
            osal_trace("\nmmz_userdev_ioctl: copy_to_user error.\n");
            return -EFAULT;
        }
    }
    return ret;
}

static int mmz_userdev_ioctl_of_s(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct mmb_share_info share_info = {0};
    int ret;

    if ((OSAL_IOC_SIZE(cmd) > sizeof(share_info)) || (arg == 0)) {
        error_mmz("OSAL_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", OSAL_IOC_SIZE(cmd), arg);
        return -EINVAL;
    }

    if (copy_from_user(&share_info, (void *)(uintptr_t)arg, OSAL_IOC_SIZE(cmd))) {
        osal_trace("\nmmz_userdev_ioctl: copy_from_user error.\n");
        return -EFAULT;
    }

    ret = mmz_userdev_ioctl_s(file, cmd, &share_info);
    if (!ret && (cmd & IOC_OUT)) {
        if (copy_to_user((void *)(uintptr_t)arg, &share_info, OSAL_IOC_SIZE(cmd))) {
            osal_trace("\nmmz_userdev_ioctl: copy_to_user error.\n");
            return -EFAULT;
        }
    }
    return ret;
}

static int mmz_userdev_ioctl_of_r(struct file *file, unsigned int cmd, unsigned long arg,
    struct mmz_userdev_info *pmu)
{
    struct mmb_info *pmi = NULL;

    pmi = get_mmbinfo_safe(arg, pmu);
    if (pmi == NULL) {
        return -EPERM;
    }

    return mmz_userdev_ioctl_r(cmd, pmi);
}

static int mmz_userdev_ioctl_of_c(unsigned int cmd, unsigned long arg, const struct mmz_userdev_info *pmu)
{
    struct mmb_info *pmi = NULL;
    int ret = 0;

    if (arg == 0) {
        mmz_flush_dcache_all();
        return ret;
    }

    pmi = get_mmbinfo_safe(arg, pmu);
    if (pmi == NULL) {
        return -EPERM;
    }

    switch (OSAL_IOC_NR(cmd)) {
        case OSAL_IOC_NR(IOC_MMB_FLUSH_DCACHE):
            mmz_flush_dcache_mmb(pmi);
            break;
        default:
            ret = -EINVAL;
            break;
    }
    return ret;
}

static int mmz_userdev_check_dirty_area_range(const struct dirty_area *area,
    const struct mmz_userdev_info *pmu)
{
    ot_mmb_t *mmb = NULL;
    struct mmb_info *pmi = NULL;
    unsigned long offset;

    mmb = ot_mmb_getby_phys_2(area->dirty_phys_start, &offset);
    if (mmb == NULL) {
#ifdef CONFIG_PHYS_ADDR_BIT_WIDTH_64
        error_mmz("dirty_phys_addr=0x%llx\n", area->dirty_phys_start);
#else
        error_mmz("dirty_phys_addr=0x%lx\n", area->dirty_phys_start);
#endif
        return -EFAULT;
    }

    pmi = get_mmbinfo_safe(mmb->phys_addr, pmu);
    if (pmi == NULL) {
        return -EPERM;
    }
    if ((uintptr_t)area->dirty_virt_start != (uintptr_t)pmi->mapped + offset) {
        osal_trace(KERN_WARNING "dirty_virt_start addr was not consistent with dirty_phys_start addr!\n");
        return -EFAULT;
    }
    if (area->dirty_phys_start + area->dirty_size > mmb->phys_addr + mmb->length) {
        osal_trace(KERN_WARNING "\ndirty area overflow!\n");
        return -EFAULT;
    }

    return 0;
}

static int mmz_userdev_ioctl_of_d(unsigned int cmd, unsigned long arg, const struct mmz_userdev_info *pmu)
{
    struct dirty_area area;
    OSAL_PHYS_ADDR_TYPE orig_addr;
    unsigned long virt_addr;
    struct mm_struct *mm = current->mm;
    int ret;

    if ((OSAL_IOC_SIZE(cmd) != sizeof(area)) || (arg == 0)) {
        error_mmz("OSAL_IOC_SIZE(cmd)=%d, arg==0x%08lx\n", OSAL_IOC_SIZE(cmd), arg);
        return -EINVAL;
    }
    (void)memset_s(&area, sizeof(area), 0, sizeof(area));
    if (copy_from_user(&area, (void *)(uintptr_t)arg, OSAL_IOC_SIZE(cmd))) {
        osal_trace(KERN_WARNING "\nmmz_userdev_ioctl: copy_from_user error.\n");
        return -EFAULT;
    }

    ret = mmz_userdev_check_dirty_area_range(&area, pmu);
    if (ret != 0) {
        return ret;
    }

    mmz_map_down(mm);

    if (ot_vma_check((uintptr_t)area.dirty_virt_start, (uintptr_t)area.dirty_virt_start + area.dirty_size)) {
        osal_trace(KERN_WARNING "\ndirty area[0x%lx,0x%lx] overflow!\n",
               (unsigned long)(uintptr_t)area.dirty_virt_start,
               (unsigned long)((unsigned long)(uintptr_t)area.dirty_virt_start + area.dirty_size));
        mmz_map_up(mm);
        return -EFAULT;
    }

    /* cache line aligned */
    orig_addr = area.dirty_phys_start;
    area.dirty_phys_start &= ~(CACHE_LINE_SIZE - 1);
    virt_addr = (unsigned long)(uintptr_t)area.dirty_virt_start;
    virt_addr &= ~(CACHE_LINE_SIZE - 1);
    area.dirty_virt_start = (void *)(uintptr_t)virt_addr;
    area.dirty_size = (area.dirty_size + (orig_addr - area.dirty_phys_start) +
                      (CACHE_LINE_SIZE - 1)) & ~(CACHE_LINE_SIZE - 1);

    mmz_flush_dcache_mmb_dirty(&area);
    mmz_map_up(mm);
    return 0;
}

static int mmz_userdev_ioctl_of_t(const struct file *file, unsigned int cmd, unsigned long arg,
    const struct mmz_userdev_info *pmu)
{
    struct mmb_info mi;

    if ((OSAL_IOC_SIZE(cmd) != sizeof(mi)) || (arg == 0)) {
        error_mmz("OSAL_IOC_SIZE(cmd)=%d, arg==0x%08lx\n", OSAL_IOC_SIZE(cmd), arg);
        return -EINVAL;
    }

    (void)memset_s(&mi, sizeof(mi), 0, sizeof(mi));
    if (copy_from_user(&mi, (void *)(uintptr_t)arg, sizeof(mi))) {
        osal_trace("\nmmz_userdev_ioctl: copy_from_user error.\n");
        return -EFAULT;
    }

    if ((get_mmbinfo_safe(mi.phys_addr, pmu)) == NULL) {
        return -EPERM;
    }
    return mmz_userdev_ioctl_t(file, cmd, &mi);
}

static int mmz_userdev_ioctl_p(struct file *file, unsigned int cmd, mmb_user_info *pmi)
{
    switch (OSAL_IOC_NR(cmd)) {
        case OSAL_IOC_NR(IOC_MMB_CHECK_MEM_SHARE):
            return ioctl_mmb_check_mem_share(pmi);
        case OSAL_IOC_NR(IOC_MMB_CHECK_MEM_SHARE_WITH_PID):
            return ioctl_mmb_check_mem_share_with_pid(pmi);
        case OSAL_IOC_NR(IOC_MMZ_CHECK_PHYS_ADDR):
            return ioctl_mmz_check_phys_addr(pmi);
        case OSAL_IOC_NR(IOC_MMZ_GET_MEM_PROCESS_ISOLATION):
            return ioctl_mmz_get_mem_process_isolation();
        case OSAL_IOC_NR(IOC_MMZ_PHYS_TO_HANDLE):
            return ioctl_cmpi_mmz_phys_to_handle(pmi);
        default:
            error_mmz("invalid ioctl cmd = %08X\n", cmd);
            return -EINVAL;
    }
}

static int mmz_userdev_ioctl_of_p(struct file *file, unsigned int cmd, unsigned long arg)
{
    mmb_user_info mi = { 0 };
    int ret;

    if ((OSAL_IOC_SIZE(cmd) > sizeof(mi)) || (arg == 0)) {
        error_mmz("OSAL_IOC_SIZE(cmd)=%d, arg==0x%08lX\n", OSAL_IOC_SIZE(cmd), arg);
        return -EINVAL;
    }
    (void)memset_s(&mi, sizeof(mi), 0, sizeof(mi));
    if (copy_from_user(&mi, (void *)(uintptr_t)arg, OSAL_IOC_SIZE(cmd))) {
        osal_trace("\nmmz_userdev_ioctl: copy_from_user error.\n");
        return -EFAULT;
    }

    ret = mmz_userdev_ioctl_p(file, cmd, &mi);
    if (!ret && (cmd & IOC_OUT)) {
        if (copy_to_user((void *)(uintptr_t)arg, &mi, OSAL_IOC_SIZE(cmd))) {
            osal_trace("\nmmz_userdev_ioctl: copy_to_user error.\n");
            return -EFAULT;
        }
    }
    return ret;
}

static long mmz_userdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret;
    struct mmz_userdev_info *pmu = file->private_data;

    down(&pmu->sem);

    if (OSAL_IOC_TYPE(cmd) == 'm') {
        ret = mmz_userdev_ioctl_of_m(file, cmd, arg);
    } else if (OSAL_IOC_TYPE(cmd) == 's') {
        ret = mmz_userdev_ioctl_of_s(file, cmd, arg);
    } else if (OSAL_IOC_TYPE(cmd) == 'r') {
        ret = mmz_userdev_ioctl_of_r(file, cmd, arg, pmu);
    } else if (OSAL_IOC_TYPE(cmd) == 'c') {
        ret = mmz_userdev_ioctl_of_c(cmd, arg, pmu);
    } else if (OSAL_IOC_TYPE(cmd) == 'd') {
        ret = mmz_userdev_ioctl_of_d(cmd, arg, pmu);
    } else if (OSAL_IOC_TYPE(cmd) == 't') {
        ret = mmz_userdev_ioctl_of_t(file, cmd, arg, pmu);
    } else if (OSAL_IOC_TYPE(cmd) == 'p') {
        ret = mmz_userdev_ioctl_of_p(file, cmd, arg);
    } else {
        ret = -EINVAL;
    }

    up(&pmu->sem);
    return ret;
}

#ifdef CONFIG_COMPAT
static long compat_mmz_userdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    return mmz_userdev_ioctl(file, cmd, (unsigned long)(uintptr_t)compat_ptr(arg));
}
#endif

static void mmz_userdev_mmap_with_file_flag(const struct file *file, struct vm_area_struct *vma,
    int mmb_cached)
{
    if (file->f_flags & O_SYNC) {
#ifdef CONFIG_64BIT
        vma->vm_page_prot = __pgprot((pgprot_val(vma->vm_page_prot) & ~PTE_RDONLY)
                                     | PTE_WRITE | PTE_DIRTY);
#endif
        vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
    } else {
#ifdef CONFIG_64BIT
        vma->vm_page_prot = __pgprot((pgprot_val(vma->vm_page_prot) & ~PTE_RDONLY)
                                     | PTE_WRITE | PTE_DIRTY);
#else
        vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot)
                                     | L_PTE_PRESENT | L_PTE_YOUNG
                                     | L_PTE_DIRTY | L_PTE_MT_DEV_CACHED);
#endif
        if (mmb_cached == 0) {
            /*
             * pagetable property changes from <normal nocache> to
             * <strong order>, for enhance ddr access performance,
             */
            /*  <normal nocache> */
            vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
        }
    }
}

static void mmz_userdev_mmap_page_valid(struct vm_area_struct *vma)
{
    unsigned long start = vma->vm_start;
    unsigned long pfn = vma->vm_pgoff;
    size_t size = vma->vm_end - vma->vm_start;

    while (size) {
        if (pfn_valid(pfn)) {
            if (vm_insert_page(vma, start, pfn_to_page(pfn))) {
                error_mmz("insert page failed.\n");
                break;
            }
        } else {
#ifdef CONFIG_64BIT
            error_mmz("vm map failed for phy address(0x%llx)\n", __pfn_to_phys(pfn));
#else
            error_mmz("vm map failed for phy address(0x%x)\n", __pfn_to_phys(pfn));
#endif
        }

        start += PAGE_SIZE;
        size = (size < PAGE_SIZE) ? 0 : (size - PAGE_SIZE);
        pfn++;
    }
}

static int mmz_userdev_mmap_page_invalid(struct vm_area_struct *vma)
{
    size_t size = vma->vm_end - vma->vm_start;
    if (size == 0) {
        return -EPERM;
    }
    /*
     * Remap-pfn-range will mark the range
     * as VM_IO and VM_RESERVED
     */
    if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, vma->vm_end - vma->vm_start,
                        vma->vm_page_prot)) {
        return -EAGAIN;
    }
    return 0;
}

static int mmz_userdev_get_mmap_cache(struct mmz_userdev_info *pmu, unsigned long offset,
    size_t size, int *mmb_cached)
{
    ot_mmb_t *mmb = NULL;
    struct mmb_info *p = get_mmbinfo(offset, pmu);

    if (p != NULL) {
        if ((p->mapped != NULL) && p->map_cached) {
            error_mmz("mmb(0x%08lX) have been mapped already and cache_type is %u?!\n", offset, p->map_cached);
            return -EIO;
        }
        *mmb_cached = p->map_cached;
        return 0;
    }

    mmb = media_mem_get_mmb_by_phys_and_size(offset, size);
    if (mmb == NULL) {
        /* Allow mmap MMZ allocated by other core. */
        return ot_map_mmz_check_phys(offset, size) ? -EPERM : 0;
    }

    if (ot_mmb_check_mem_share(mmb) < 0) {
        media_mem_put_mmb(mmb);
        return -EPERM;
    }
    *mmb_cached = mmb->flags & OT_MMB_MAP2KERN_CACHED;
    media_mem_put_mmb(mmb);
    return 0;
}

static int mmz_userdev_mmap_pages(struct vm_area_struct *vma)
{
    int ret;

    if (pfn_valid(vma->vm_pgoff)) {
        mmz_userdev_mmap_page_valid(vma);
        return 0;
    }

    ret = mmz_userdev_mmap_page_invalid(vma);
    return ret;
}

int mmz_userdev_mmap(struct file *file, struct vm_area_struct *vma)
{
    struct mmz_userdev_info *pmu = NULL;
    unsigned long offset;
    int mmb_cached = 0;
    int ret;
    size_t size;

    if ((file == NULL) || (file->private_data == NULL) || (vma == NULL) || (vma->vm_mm == NULL)) {
        error_mmz("%s - parameter invalid!\n", __FUNCTION__);
        return -1;
    }

    pmu = file->private_data;
    offset = vma->vm_pgoff << PAGE_SHIFT;
    size = vma->vm_end - vma->vm_start;

    ret = mmz_userdev_get_mmap_cache(pmu, offset, size, &mmb_cached);
    if (ret != 0) {
        return ret;
    }

    mmz_userdev_mmap_with_file_flag(file, vma, mmb_cached);

    ret = mmz_userdev_mmap_pages(vma);
    if (ret != 0) {
        return ret;
    }

#ifdef CONFIG_OT_SYS_SMMU_SUPPORT
    /* flush page table cache to share the data with CBBs that have sMMU */
    svm_flush_cache(vma->vm_mm, vma->vm_start, vma->vm_end - vma->vm_start);
#endif

    return 0;
}

static int mmz_userdev_release(struct inode *inode, struct file *file)
{
    struct mmz_userdev_info *pmu = file->private_data;
    struct mmb_info *p = NULL;
    struct mmb_info *n = NULL;
    int pid = pmu->pid;

    osal_unused(inode);
    list_for_each_entry_safe(p, n, &pmu->list, list) {
        error_mmz("MMB LEAK(pid=%d): 0x%lX, %lu bytes, '%s'\n",
            pmu->pid, ot_mmb_phys(p->mmb), ot_mmb_length(p->mmb), ot_mmb_name(p->mmb));

        /*
         * we do not need to release mapped-area here,
         * system will do it for us
         */
        if (p->mapped != NULL) {
            warning("mmb<0x%llx> mapped to userspace 0x%pK will be unmaped!\n",
                (unsigned long long)p->phys_addr, p->mapped);
        }
        for (; p->mmb_ref > 0; p->mmb_ref--) {
            ot_mmb_put(p->mmb);
        }
        _usrdev_mmb_free(p);
    }

    file->private_data = NULL;
    kfree(pmu);
    pmu = NULL;

    if (media_mem_is_check_pid()) {
        media_mem_clear_pid_in_all_mmb(pid);
    }
    return 0;
}

static struct file_operations g_mmz_userdev_fops = {
    .owner = THIS_MODULE,
    .open = mmz_userdev_open,
    .release = mmz_userdev_release,
#ifdef CONFIG_COMPAT
    .compat_ioctl = compat_mmz_userdev_ioctl,
#endif
    .unlocked_ioctl = mmz_userdev_ioctl,
    .mmap = mmz_userdev_mmap,
};

static struct miscdevice g_mmz_userdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &g_mmz_userdev_fops,
    .name = "mmz_userdev"
};

int __init mmz_userdev_init(void)
{
    int ret;
    ret = misc_register(&g_mmz_userdev);
    if (ret) {
        osal_trace("register mmz dev failure!\n");
        return -1;
    }

    return 0;
}

void __exit mmz_userdev_exit(void)
{
    misc_deregister(&g_mmz_userdev);
}

/* Test func */
static int mmz_userdev_ioctl_t(const struct file *file, unsigned int cmd, const struct mmb_info *pmi)
{
    osal_unused(file);
    osal_unused(pmi);
    osal_unused(cmd);
    return 0;
}

