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
#include <linux/module.h>
#include <linux/seq_file.h>
#include "gfbg_init.h"
#include "ot_type.h"
#include <linux/of_platform.h>

char *g_layer_mmz_names[GFBG_MAX_LAYER_NUM] = { [0 ... GFBG_MAX_LAYER_NUM - 1] = TD_NULL };

char g_tmp_video[128] = "gfbg:vram0_size:8100"; /* 128 The length of the array */
module_param_string(video, g_tmp_video, 128, 0);

module_param_array(g_layer_mmz_names, charp, TD_NULL, S_IRUGO);
MODULE_PARM_DESC(g_layer_mmz_names, "The mmz names for the graphics layers.");
char *gfbg_get_layer_mmz_names(td_u32 layer_id)
{
    if (layer_id >= GFBG_MAX_LAYER_NUM) {
        return TD_NULL;
    }
    return g_layer_mmz_names[layer_id];
}

/****************************** dmabuf start **************************/
#ifdef CONFIG_DMABUF_SUPPORT
#define ot_unused(x) ((x) = (x))

typedef struct {
    phys_addr_t mem_base;
} gfbg_mem_block_pdata;

static struct sg_table *gfbg_memblock_map(struct dma_buf_attachment *buf_attach,
    enum dma_data_direction data_direction)
{
    td_s32 i;
    td_s32 ret;
    td_ulong page_pfn;
    gfbg_mem_block_pdata *mem_block_data = TD_NULL;
    struct page *fb_page = TD_NULL;
    struct sg_table *fb_table = TD_NULL;
    struct scatterlist *sg_list = TD_NULL;

    ot_unused(data_direction);

    if ((buf_attach == TD_NULL) || (buf_attach->dmabuf == TD_NULL) || (buf_attach->dmabuf->priv == TD_NULL)) {
        return TD_NULL;
    }

    mem_block_data = buf_attach->dmabuf->priv;
    if (mem_block_data == TD_NULL) {
        return TD_NULL;
    }

    page_pfn = PFN_DOWN(mem_block_data->mem_base);
    fb_page = pfn_to_page(page_pfn);
    if (fb_page == TD_NULL) {
        return TD_NULL;
    }

    fb_table = kzalloc(sizeof(*fb_table), GFP_KERNEL);
    if (fb_table == TD_NULL) {
        return ERR_PTR(-ENOMEM);
    }

    ret = sg_alloc_table(fb_table, 1, GFP_KERNEL);
    if (ret < 0) {
        kfree(fb_table);
        return ERR_PTR(ret);
    }

    sg_set_page(fb_table->sgl, fb_page, buf_attach->dmabuf->size, 0);
    for_each_sg(fb_table->sgl, sg_list, fb_table->nents, i) {
        if (sg_list != TD_NULL) {
        sg_dma_address(sg_list) = sg_phys(sg_list);
    }
    }
    return fb_table;
}

static td_void gfbg_memblock_unmap(struct dma_buf_attachment *buf_attach, struct sg_table *fb_table,
                                   enum dma_data_direction data_direction)
{
    ot_unused(buf_attach);
    ot_unused(data_direction);
    if (fb_table != TD_NULL) {
        sg_free_table(fb_table);
    }
}

static td_void __init_memblock gfbg_memblock_release(struct dma_buf *fb_dma_buf)
{
    ot_unused(fb_dma_buf);
}

static td_void *gfbg_memblock_do_kmap(struct dma_buf *fb_dma_buf, td_ulong pgoffset, bool atomic)
{
    td_ulong page_pfn;
    gfbg_mem_block_pdata *mem_block_data = TD_NULL;
    struct page *fb_page = TD_NULL;

    if ((fb_dma_buf == TD_NULL) || (fb_dma_buf->priv == TD_NULL)) {
        return TD_NULL;
    }

    mem_block_data = fb_dma_buf->priv;
    if (mem_block_data == TD_NULL) {
        return TD_NULL;
    }

    page_pfn = PFN_DOWN(mem_block_data->mem_base) + pgoffset;
    fb_page = pfn_to_page(page_pfn);
    if (fb_page == TD_NULL) {
        return TD_NULL;
    }

    if (atomic) {
        return kmap_atomic(fb_page);
    } else {
        return kmap(fb_page);
    }
}

static td_void *gfbg_memblock_kmap_atomic(struct dma_buf *fb_dma_buf, td_ulong pgoffset)
{
    return gfbg_memblock_do_kmap(fb_dma_buf, pgoffset, true);
}

static td_void gfbg_memblock_kunmap_atomic(struct dma_buf *fb_dma_buf, td_ulong pgoffset, td_void *vir_addr)
{
    ot_unused(fb_dma_buf);
    ot_unused(pgoffset);
    if (vir_addr != TD_NULL) {
        kunmap_atomic(vir_addr);
    }
}

static td_void *gfbg_memblock_kmap(struct dma_buf *fb_dma_buf, td_ulong pgoffset)
{
    return gfbg_memblock_do_kmap(fb_dma_buf, pgoffset, false);
}

static td_void gfbg_memblock_kunmap(struct dma_buf *fb_dma_buf, td_ulong pgoffset, td_void *vir_addr)
{
    ot_unused(fb_dma_buf);
    ot_unused(pgoffset);
    if (vir_addr != TD_NULL) {
        kunmap(vir_addr);
    }
}

static inline int gfbg_valid_mmap_phys_addr_range(unsigned long pfn, size_t size)
{
    return (pfn + (size >> PAGE_SHIFT)) <= (1 + (PHYS_MASK >> PAGE_SHIFT));
}

static td_s32 gfbg_memblock_mmap(struct dma_buf *fb_dma_buf, struct vm_area_struct *vma_area)
{
    td_ulong size;
    gfbg_mem_block_pdata *mem_block_data = TD_NULL;

    if ((fb_dma_buf == TD_NULL) || (vma_area == TD_NULL)) {
        return -1;
    }

    mem_block_data = fb_dma_buf->priv;
    if (mem_block_data == TD_NULL) {
        return -1;
    }

    if (mem_block_data->mem_base == 0) {
        return -1;
    }

    size =  vma_area->vm_end - vma_area->vm_start;
    if (size == 0) {
        return -1;
    }

    if (!gfbg_valid_mmap_phys_addr_range(vma_area->vm_pgoff, size)) {
        return -1;
    }
    vma_area->vm_page_prot =  pgprot_writecombine(vma_area->vm_page_prot);

    return remap_pfn_range(vma_area, vma_area->vm_start, PFN_DOWN(mem_block_data->mem_base), size,
                           vma_area->vm_page_prot);
}

struct dma_buf_ops g_gfbg_mem_block_ops = {
    .map_dma_buf   = gfbg_memblock_map,
    .unmap_dma_buf = gfbg_memblock_unmap,
    .release       = gfbg_memblock_release,
    .kmap_atomic   = gfbg_memblock_kmap_atomic,
    .kunmap_atomic = gfbg_memblock_kunmap_atomic,
    .kmap          = gfbg_memblock_kmap,
    .kunmap        = gfbg_memblock_kunmap,
    .mmap          = gfbg_memblock_mmap,
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
struct dma_buf *gfbg_memblock_export(phys_addr_t mem_base, size_t mem_size, td_s32 flags)
{
    struct gfbg_mem_block_pdata *mem_block_data = TD_NULL;
    struct dma_buf *fb_dma_buf = TD_NULL;

    if ((mem_base != PAGE_ALIGN(mem_base)) || (mem_size != PAGE_ALIGN(mem_size))) {
        return ERR_PTR(-EINVAL);
    }
    mem_block_data = kzalloc(sizeof(*mem_block_data), GFP_KERNEL);
    if (mem_block_data == TD_NULL) {
        return ERR_PTR(-ENOMEM);
    }

    mem_block_data->mem_base = mem_base;
    fb_dma_buf = dma_buf_export(mem_block_data, &g_gfbg_mem_block_ops, mem_size, flags, TD_NULL);
    if (IS_ERR(fb_dma_buf)) {
        kfree(mem_block_data);
    }
    return fb_dma_buf;
}
#else
struct dma_buf *gfbg_memblock_export(phys_addr_t mem_base, size_t mem_size, td_s32 flags)
{
    DEFINE_DMA_BUF_EXPORT_INFO(export_info);
    struct dma_buf *fb_dma_buf = TD_NULL;

    if ((mem_base != PAGE_ALIGN(mem_base)) || (mem_size != PAGE_ALIGN(mem_size))) {
        return ERR_PTR(-EINVAL);
    }

    export_info.ops      = &g_gfbg_mem_block_ops;
    export_info.size     = mem_size;
    export_info.flags    = flags;
    export_info.resv     = TD_NULL;
    export_info.priv     = (td_void *)mem_base;

    fb_dma_buf = dma_buf_export(&export_info);

    return fb_dma_buf;
}
#endif
#endif
/****************************** dmabuf end **************************/
/****************************** vsync start **************************/
#ifdef CONFIG_VSYNC_SUPPORT
#define VSYNC_LEN 64
static unsigned int g_vsync_major = 288;
static unsigned int g_vsync_minor = 0;
static dev_t g_vsync_devno;
static struct class *g_vsync_cls;
static struct device *g_vsync_device;

static unsigned int g_pts_major = 289;
static unsigned int g_pts_minor = 0;
static dev_t g_pts_devno;
static struct class *g_pts_cls;
static struct device *g_pts_device;

static char g_vdata[VSYNC_LEN] = {0};
static char g_nowdata[VSYNC_LEN] = {0};

static spinlock_t g_vsync_lock;
static wait_queue_head_t g_wait;

static unsigned long long int g_timestamp;
static bool g_bnewdata = 0;

static int vsync_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int vsync_release(struct inode *inode, struct file *file)
{
    return 0;
}

static int vsync_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    unsigned int len;
    unsigned long syncflags = 0;

    spin_lock_irqsave(&g_vsync_lock, syncflags);
    (td_void)memset_s(g_vdata, VSYNC_LEN, 0, VSYNC_LEN);
    if (snprintf_s(g_vdata, VSYNC_LEN, VSYNC_LEN - 1, "%llu\n", g_timestamp) < 0) {
        printk("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        spin_unlock_irqrestore(&g_vsync_lock, syncflags);
        return TD_FAILURE;
    }
    spin_unlock_irqrestore(&g_vsync_lock, syncflags);

    len = strnlen(g_vdata, (VSYNC_LEN - 1)) + 1;
    if ((buf == TD_NULL) || (count < len)) {
        return 0;
    }

    if (copy_to_user(buf, g_vdata, len)) {
        printk("copy to user err\n");
        len = 0;
    }

    spin_lock_irqsave(&g_vsync_lock, syncflags);
    g_bnewdata = 0;
    spin_unlock_irqrestore(&g_vsync_lock, syncflags);

    return len;
}

static unsigned int vsync_poll(struct file *file, struct poll_table_struct *table)
{
    unsigned int mask = 0;
    unsigned long syncflags = 0;

    poll_wait(file, &g_wait, table);

    spin_lock_irqsave(&g_vsync_lock, syncflags);
    if (g_bnewdata) {
        mask |= POLLIN | POLLRDNORM;
    }
    spin_unlock_irqrestore(&g_vsync_lock, syncflags);

    return mask;
}

static struct file_operations g_vsync_ops = {
    .owner = THIS_MODULE,
    .open = vsync_open,
    .read = (td_void *)vsync_read,
    .release = vsync_release,
    .poll = vsync_poll,
};

static unsigned long long int inline get_cur_pts(void)
{
    unsigned long long int time_now;

    time_now = osal_sched_clock();
    do_div(time_now, 1000); /* 1000 alg data */

    return time_now;
}

int gfbg_vsync_notify(void)
{
    unsigned long syncflags = 0;

    spin_lock_irqsave(&g_vsync_lock, syncflags);
    g_timestamp = get_cur_pts();
    g_bnewdata = 1;
    spin_unlock_irqrestore(&g_vsync_lock, syncflags);

    wake_up(&g_wait);

    return 0;
}

int gfbg_vsync_init(void)
{
    int ret;

    g_vsync_devno = MKDEV(g_vsync_major, g_vsync_minor);
    ret = register_chrdev(g_vsync_major, "vsync", &g_vsync_ops);
    if (ret < 0) {
        printk("Unable to register characterdevice!\n");
        return ret;
    }
    g_vsync_cls = class_create(THIS_MODULE, "vsync");
    if (IS_ERR(g_vsync_cls)) {
        unregister_chrdev(g_vsync_major, "vsync");
        return -EBUSY;
    }

    g_vsync_device = device_create(g_vsync_cls, TD_NULL, g_vsync_devno, TD_NULL, "vsync"); /* mknod /dev/vsync */
    if (IS_ERR(g_vsync_device)) {
        class_destroy(g_vsync_cls);
        unregister_chrdev(g_vsync_major, "vsync");
        return -EBUSY;
    }

    init_waitqueue_head(&g_wait);
    spin_lock_init(&g_vsync_lock);

    return 0;
}

void gfbg_vsync_exit(void)
{
    device_destroy(g_vsync_cls, g_vsync_devno);
    class_destroy(g_vsync_cls);
    unregister_chrdev(g_vsync_major, "vsync");
}

static int pts_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    unsigned long long int time_now;
    unsigned int len;
    time_now = get_cur_pts();

    (td_void)memset_s(g_nowdata, sizeof(g_nowdata), 0, VSYNC_LEN);
    if (snprintf_s(g_nowdata, sizeof(g_nowdata), VSYNC_LEN - 1, "%llu\n", time_now) < 0) {
        printk("%s:%d:snprintf_s failure\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    len = strnlen(g_nowdata, (VSYNC_LEN - 1)) + 1;
    if ((buf == TD_NULL) || (count < len)) {
        return 0;
    }

    if (copy_to_user(buf, g_nowdata, len)) {
        printk("copy to user err\n");
        len = 0;
    }

    return len;
}

static struct file_operations g_pts_ops = {
    .owner = THIS_MODULE,
    .read = (td_void *)pts_read,
};

int gfbg_pts_init(void)
{
    int ret;

    g_pts_devno = MKDEV(g_pts_major, g_pts_minor);
    ret = register_chrdev(g_pts_major, "vsync_pts", &g_pts_ops);
    if (ret < 0) {
        printk("Unable to register characterdevice!\n");
        return ret;
    }

    g_pts_cls = class_create(THIS_MODULE, "vsync_pts");
    if (IS_ERR(g_pts_cls)) {
        unregister_chrdev(g_pts_major, "vsync_pts");
        return -EBUSY;
    }

    g_pts_device = device_create(g_pts_cls, TD_NULL, g_pts_devno, TD_NULL, "vsync_pts"); /* mknod /dev/vsync_pts */
    if (IS_ERR(g_pts_device)) {
        class_destroy(g_pts_cls);
        unregister_chrdev(g_pts_major, "vsync_pts");
        return -EBUSY;
    }

    return 0;
}

void gfbg_pts_exit(void)
{
    device_destroy(g_pts_cls, g_pts_devno);
    class_destroy(g_pts_cls);
    unregister_chrdev(g_pts_major, "vsync_pts");
}
#endif
/****************************** vsync end **************************/
#ifdef CONFIG_OT_VO_FB_SEPARATE
#define GFBG_INT_NAME_LENGTH 10

static int ot_gfbg_probe(struct platform_device *pdev)
{
    td_char gfbg_int_name[GFBG_INT_NAME_LENGTH] = "gfbg";
    int *fifb_irq = (int *)(vou_get_gfbg_irq());
    set_video_name(g_tmp_video);
    *fifb_irq = osal_platform_get_irq_byname(pdev, gfbg_int_name);
    if (*fifb_irq <= 0) {
        dev_err(&pdev->dev, "cannot find gfbg IRQ\n");
        return TD_FAILURE;
    }
    if (gfbg_init() != TD_SUCCESS) {
        osal_printk("gfbg_init FAILURE!\n");
    }
    return 0;
}

static int ot_gfbg_remove(struct platform_device *pdev)
{
    ot_unused(pdev);
    gfbg_cleanup();
    return 0;
}

static const struct of_device_id ot_gfbg_match[] = {
    { .compatible = "vendor,gfbg" },
    {},
};
MODULE_DEVICE_TABLE(of, ot_gfbg_match);

static struct platform_driver ot_gfbg_driver = {
    .probe = ot_gfbg_probe,
    .remove = ot_gfbg_remove,
    .driver = {
        .name   = "ot_gfbg",
        .of_match_table = ot_gfbg_match,
    },
};

osal_module_platform_driver(ot_gfbg_driver);
MODULE_LICENSE("GPL");

#else
td_void set_global_name(void)
{
    set_video_name(g_tmp_video);
}
#endif

