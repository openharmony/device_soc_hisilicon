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

#include "drv_osal_lib.h"
#include "drv_symc.h"
#include "drv_hash.h"
#include "ot_common.h"
#include "dev_ext.h"

static osal_dev_t *g_cipher_device;

/* ****** proc function begin ******* */
#ifdef OT_PROC_SUPPORT
td_s32 symc_proc_read(struct osal_proc_dir_entry *p)
{
    symc_chn_status *status = TD_NULL;
    symc_module_status module;
    td_u32 i;
    td_u32 sta_size;

    if (p == TD_NULL) {
        ot_cipher_error("Invalid pointer!\n");
        return OT_ERR_CIPHER_INVALID_POINT;
    }

    CRYPTO_PROC_PRINT(p, "\n[CIPHER] Version: [%s], Build Time[%s, %s]\n\n", OT_MPP_VERSION, __DATE__, __TIME__);
    CRYPTO_PROC_PRINT(p, "\n---------------------------------------- cipher status ------------------------------------"
        "-----------------------------\n");
    CRYPTO_PROC_PRINT(p, "chn_id   status  decrypt   alg   mode  key_len     addr in/out    key_from   int_raw  "
        "int_en   int_status   iv_out\n");

    sta_size = CRYPTO_HARD_CHANNEL_MAX * sizeof(symc_chn_status);
    status = (symc_chn_status *)crypto_malloc(sta_size);
    if (status == TD_NULL) {
        ot_cipher_error("crypto malloc for status buff failed!\n");
        return OT_ERR_CIPHER_FAILED_MEM;
    }
    (td_void)memset_s(status, sta_size, 0, sta_size);

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        status[i].id = i;
    }

    (td_void)memset_s(&module, sizeof(symc_module_status), 0, sizeof(symc_module_status));
    drv_symc_proc_status(status, CRYPTO_HARD_CHANNEL_MAX * sizeof(symc_chn_status), &module);

    for (i = 0; i < CRYPTO_HARD_CHANNEL_MAX; i++) {
        if (((0x1 << i) & CIPHER_HARD_CHANNEL_MASK) == 0x00) {
            continue;
        }

        CRYPTO_PROC_PRINT(p, "  %02u     %s      %u      %s  %s    %03d    %08x/%08x    "
                          "%02u         %u         %u         %u       %s\n",
                          i,
                          status[i].open,      status[i].decrypt,           status[i].alg,
                          status[i].mode,      (td_u32)status[i].klen * 8,  status[i].inaddr, /* 8 byte bits */
                          status[i].outaddr,   status[i].ksrc,              status[i].int_raw,
                          status[i].int_en,    status[i].int_status,        status[i].iv);
    }
    crypto_free(status);

    return TD_SUCCESS;
}

static td_void symc_proc_init(td_void)
{
    osal_proc_entry_t *proc_entry = TD_NULL;

    proc_entry = osal_create_proc_entry(UMAP_DEVNAME_CIPHER_BASE, TD_NULL);
    if (proc_entry == TD_NULL) {
        ot_cipher_error("cipher: can't create proc.\n");
        return;
    }
    proc_entry->read = symc_proc_read;
}

static td_void symc_proc_deinit(td_void)
{
    osal_remove_proc_entry(UMAP_DEVNAME_CIPHER_BASE, NULL);
}
#endif /* ****** proc function end ******* */

static long ot_cipher_ioctl(td_u32 cmd,  unsigned long arg, td_void *private_data)
{
    crypto_unused(private_data);
    return crypto_ioctl(cmd, (td_void *)(uintptr_t)arg);
}

static td_s32 ot_cipher_release(void *private_data)
{
    crypto_unused(private_data);
    return crypto_release();
}

static osal_fileops_t g_dev_cipher_fops = {
    .open             = TD_NULL,
    .unlocked_ioctl   = ot_cipher_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl     = ot_cipher_ioctl,
#endif
    .release          = ot_cipher_release,
};

td_s32 cipher_drv_mod_init(td_void)
{
    td_s32 ret;

    g_cipher_device = osal_createdev(UMAP_DEVNAME_CIPHER_BASE);
    g_cipher_device->fops = &g_dev_cipher_fops;
    g_cipher_device->minor = UMAP_CIPHER_MINOR_BASE;

    ret = osal_registerdevice(g_cipher_device);
    if (ret != TD_SUCCESS) {
        ot_cipher_error("register CIPHER failed.\n");
        osal_destroydev(g_cipher_device);
        return ret;
    }

    ret = crypto_entry();
    if (ret != TD_SUCCESS) {
        osal_deregisterdevice(g_cipher_device);
        osal_destroydev(g_cipher_device);
        return ret;
    }

    /* ****** proc function begin ******* */
#ifdef OT_PROC_SUPPORT
    symc_proc_init();
#endif /* ****** proc function end ******* */

    OT_PRINT("load ot_cipher.ko ....OK!\n");

    return TD_SUCCESS;
}

void cipher_drv_mod_exit(void)
{
    /* ****** proc function begin ******* */
#ifdef OT_PROC_SUPPORT
    symc_proc_deinit();
#endif /* ****** proc function end ******* */

    (td_void)crypto_exit();
    osal_deregisterdevice(g_cipher_device);
    osal_destroydev(g_cipher_device);

    OT_PRINT("unload ot_cipher.ko ....OK!\n");
}

static td_s32 cipher_get_irq(struct platform_device *pdev)
{
#ifdef IRQ_DTS_SUPPORT
    td_s32 irq_num;

    if (crypto_is_sec_cpu()) {
        irq_num = platform_get_irq_byname(pdev, "sec_spacc");
        if (irq_num <= 0) {
            dev_err(&pdev->dev, "cannot find cipher IRQ number %d.\n", irq_num);
            return OT_ERR_CIPHER_FAILED_INIT;
        }
        module_set_irq(CRYPTO_MODULE_ID_SYMC, irq_num);

        irq_num = platform_get_irq_byname(pdev, "sec_spacc");
        if (irq_num <= 0) {
            dev_err(&pdev->dev, "cannot find trng IRQ number %d.\n", irq_num);
            return OT_ERR_CIPHER_FAILED_INIT;
        }
        module_set_irq(CRYPTO_MODULE_ID_HASH, irq_num);

        irq_num = platform_get_irq_byname(pdev, "sec_pke");
        if (irq_num <= 0) {
            dev_err(&pdev->dev, "cannot find rsa IRQ number %d.\n", irq_num);
            return OT_ERR_CIPHER_FAILED_INIT;
        }
        module_set_irq(CRYPTO_MODULE_ID_PKE, irq_num);
    } else {
        irq_num = platform_get_irq_byname(pdev, "nsec_spacc");
        if (irq_num <= 0) {
            dev_err(&pdev->dev, "cannot find cipher IRQ number %d.\n", irq_num);
            return OT_ERR_CIPHER_FAILED_INIT;
        }
        module_set_nsec_irq(CRYPTO_MODULE_ID_SYMC, irq_num);

        irq_num = platform_get_irq_byname(pdev, "nsec_spacc");
        if (irq_num <= 0) {
            dev_err(&pdev->dev, "cannot find trng IRQ number %d.\n", irq_num);
            return OT_ERR_CIPHER_FAILED_INIT;
        }
        module_set_nsec_irq(CRYPTO_MODULE_ID_HASH, irq_num);

        irq_num = platform_get_irq_byname(pdev, "nsec_pke");
        if (irq_num <= 0) {
            dev_err(&pdev->dev, "cannot find rsa IRQ number %d.\n", irq_num);
            return OT_ERR_CIPHER_FAILED_INIT;
        }
        module_set_nsec_irq(CRYPTO_MODULE_ID_PKE, irq_num);
    }
#endif

    return TD_SUCCESS;
}

static int ot_cipher_probe(struct platform_device *pdev)
{
    td_s32 ret;

    ret = crypto_get_cpu_secure_sta();
    if (ret != TD_SUCCESS) {
        print_func_errno(crypto_get_cpu_secure_sta, ret);
        return ret;
    }

    ret = cipher_get_irq(pdev);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_get_irq, ret);
        return ret;
    }

    ret = cipher_drv_mod_init();
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_drv_mod_init, ret);
        return ret;
    }

    return TD_SUCCESS;
}

static int ot_cipher_remove(struct platform_device *pdev)
{
    crypto_unused(pdev);
    cipher_drv_mod_exit();

    return 0;
}

static const struct of_device_id g_ot_cipher_match[] = {
    { .compatible = "vendor,cipher" },
    { },
};
MODULE_DEVICE_TABLE(of, g_ot_cipher_match);

static struct platform_driver g_ot_cipher_driver = {
    .probe          = ot_cipher_probe,
    .remove         = ot_cipher_remove,
    .driver         = {
        .name   = "ot_cipher",
        .of_match_table = g_ot_cipher_match,
    },
};

module_platform_driver(g_ot_cipher_driver);
MODULE_LICENSE("GPL");

