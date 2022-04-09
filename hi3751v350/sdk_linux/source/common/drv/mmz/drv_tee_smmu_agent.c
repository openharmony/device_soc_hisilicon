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

#include <linux/fs.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kthread.h>
#include <linux/hisilicon/tee_smmu.h>
#include <linux/platform_device.h>
#include "teek_ns_client.h"
#include "agent.h"
#include "mem.h"

#include "drv_media_mem.h"
#include "drv_mmz.h"
#include "drv_tzsmmu.h"

#define SMMU_AGENT_ID    0x1234
#define HIL_MAX_NAME_LEN        16
#define CLOSE_AGENT 0x12
enum smmu_cmd_t {
    HISI_MEM_ALLOC = 0x0,
    HISI_MEM_FREE,
    HISI_MEM_GET_MEMINFO,
    HISI_MEM_PUT_MEMINFO,
    HISI_AGENT_CLOSE,
};

struct smmu_ctrl_t {
    enum smmu_cmd_t cmd;
    int memtype; // 0:cma mem; 1: system mem
    char name[HIL_MAX_NAME_LEN];
    unsigned int size;
    unsigned long long phys_addr;
    unsigned long long normal_smmu;
    unsigned long long sec_smmu;
    unsigned long long tz_mblock_phys;
    unsigned long long private_data_phys;
    unsigned long long private_size;
    unsigned long long handle_id;
    unsigned long long reserved;
};

static int smmu_agent_mem_alloc(struct smmu_ctrl_t *smmu_ctrl)
{
    size_t size;
    struct tee_mem_info mem_info = {0};
    struct tee_mem_addr mem_addr = {0};

    if (smmu_ctrl->memtype) {
        mem_addr.addr = new_mmb(smmu_ctrl->name, smmu_ctrl->size, 0x1000, "iommu");
        smmu_ctrl->normal_smmu = mem_addr.addr;
        mem_addr.iommu = 1;
        mem_addr.sec_addr = smmu_ctrl->sec_smmu;
    } else {
        mem_addr.addr = new_mmb(smmu_ctrl->name, smmu_ctrl->size, 0x1000, NULL);
        smmu_ctrl->phys_addr = mem_addr.addr;
    }
    mem_addr.iommu = smmu_ctrl->memtype;

    if (mem_addr.addr == MMB_ADDR_INVALID) {
        printk("%s(%d) alloc mem failed! \n", __FUNCTION__, __LINE__);
        return -1;
    }

    mem_info.handle_id = smmu_ctrl->handle_id;
    mem_info.table = get_meminfo(mem_addr.addr, mem_addr.iommu, (unsigned int *)&size, (unsigned int *)&mem_addr.addr);
    if (mem_info.table == NULL) {
        printk("%s(%d) Cannot get meminfo and check parameter! \n", __FUNCTION__, __LINE__);
        return -1;
    }
    mem_info.size = PAGE_ALIGN(size);

    smmu_ctrl->tz_mblock_phys = hisi_tee_smmumem_into_ta(&mem_info, mem_addr);
    if (smmu_ctrl->tz_mblock_phys == 0) {
        hisi_tee_smmumem_outfrom_ta(smmu_ctrl->tz_mblock_phys);
        printk("%s(%d) hisi_tee_smmumem_into_ta failed \n", __FUNCTION__, __LINE__);
        delete_mmb(mem_addr.addr, mem_addr.iommu);
        return -1;
    }

    set_sec_mmb_flag(mem_addr.addr, mem_addr.iommu);
    sec_mmb_get(mem_addr.addr, mem_addr.iommu, 0x0);

    return 0;
}

static int smmu_agent_mem_free(struct smmu_ctrl_t *smmu_ctrl)
{
    mmb_addr_t addr = {0};
    int iommu;

    if (smmu_ctrl->memtype) {
        iommu = 1;
        addr = (mmb_addr_t)(smmu_ctrl->normal_smmu);
    } else {
        iommu = 0;
        addr = (mmb_addr_t)(smmu_ctrl->phys_addr);
    }
    clr_sec_mmb_flag(addr, iommu);
    if (!sec_mmb_put(addr, iommu)) {
        hisi_tee_smmumem_outfrom_ta(smmu_ctrl->tz_mblock_phys);
        delete_mmb(addr, iommu);
    }
    return 0;
}

static int smmu_agent_get_meminfo(struct smmu_ctrl_t *smmu_ctrl)
{
    size_t size;
    struct tee_mem_info mem_info = {0};
    struct tee_mem_addr mem_addr = {0};

    if (smmu_ctrl->memtype) {
        mem_addr.addr = smmu_ctrl->normal_smmu;
    } else {
        mem_addr.addr = smmu_ctrl->phys_addr;
    }
    mem_addr.iommu = smmu_ctrl->memtype;

    mem_info.handle_id = smmu_ctrl->handle_id;
    mem_info.table = get_meminfo(mem_addr.addr, mem_addr.iommu, (unsigned int *)&size, (unsigned int *)&mem_addr.addr);
    if (mem_info.table == NULL) {
        printk("%s(%d) Cannot get meminfo and check parameter! \n", __FUNCTION__, __LINE__);
        return -1;
    }
    mem_info.size = PAGE_ALIGN(size);
    smmu_ctrl->tz_mblock_phys = hisi_tee_smmumem_into_ta(&mem_info, mem_addr);
    if (smmu_ctrl->tz_mblock_phys == 0) {
        hisi_tee_smmumem_outfrom_ta(smmu_ctrl->tz_mblock_phys);
        printk("%s(%d) hisi_tee_smmumem_into_ta failed \n", __FUNCTION__, __LINE__);
        smmu_ctrl->tz_mblock_phys = 0;
    }

    sec_mmb_get(mem_addr.addr, mem_addr.iommu, 0x0);
    return 0;
}

static int smmu_agent_put_meminfo(struct smmu_ctrl_t *smmu_ctrl)
{
    mmb_addr_t addr = {0};
    int iommu;

    if (smmu_ctrl->memtype) {
        iommu = 1;
        addr = (mmb_addr_t)(smmu_ctrl->normal_smmu);
    } else {
        iommu = 0;
        addr = (mmb_addr_t)(smmu_ctrl->phys_addr);
    }

    if (!sec_mmb_put(addr, iommu)) {
        hisi_tee_smmumem_outfrom_ta(smmu_ctrl->tz_mblock_phys);
    }

    return 0;
}

static int smmu_agent_work(struct tee_agent_kernel_ops* agent_instance)
{
    struct smmu_ctrl_t *smmu_ctrl;
    int ret = -1;
#ifdef CFG_HI_TEE_ITRUSTEE_SUPPORT
    smmu_ctrl = (struct smmu_ctrl_t *)((tc_ns_shared_mem *)(agent_instance->agent_buff))->kernel_addr;
#else
    smmu_ctrl = (struct smmu_ctrl_t *)agent_instance->agent_buffer->kernel_addr;
#endif
    if (smmu_ctrl == NULL) {
        printk("%s :system err!\n", __func__);
        return -1;
    }

    switch (smmu_ctrl->cmd) {
        case HISI_MEM_ALLOC: {
            ret = smmu_agent_mem_alloc(smmu_ctrl);
            break;
        }
        case HISI_MEM_FREE: {
            ret = smmu_agent_mem_free(smmu_ctrl);
            break;
        }
        case HISI_MEM_GET_MEMINFO: {
            ret = smmu_agent_get_meminfo(smmu_ctrl);
            break;
        }
        case HISI_MEM_PUT_MEMINFO: {
            ret = smmu_agent_put_meminfo(smmu_ctrl);
            break;
        }
        case HISI_AGENT_CLOSE: {
            /* kernel thread should not be stopped    */
            ret = CLOSE_AGENT;
            break;
        }
    }

    return ret;
}

static int smmu_tee_agent_work(void *data)
{
    int ret = 0;
    int agent_station = 0;
    struct tee_agent_kernel_ops *agent_instance = (struct tee_agent_kernel_ops *)data;

    while (!kthread_should_stop()) {
        ret = tc_ns_wait_event(agent_instance->agent_id);
        if (ret) {
            printk("%s(%d) %s wait event fail \n", __FUNCTION__, __LINE__, agent_instance->agent_name);
            break;
        }

        if (agent_instance->tee_agent_work)
            agent_station = agent_instance->tee_agent_work(agent_instance);

        ret = tc_ns_send_event_response(agent_instance->agent_id);
        if (ret) {
            printk("%s(%d) %s send event response fail \n", __FUNCTION__, __LINE__, agent_instance->agent_name);
            break;
        }

        if (agent_station == CLOSE_AGENT) {
            /* agent will be closed. */
            break;
        }
    }

    return ret;
}

static int smmu_agent_init(struct tee_agent_kernel_ops *agent_instance)
{
#ifdef CFG_HI_TEE_ITRUSTEE_SUPPORT
    kthread_run(hisi_teesmmu_agent_init, "resume", "hisi_teesmmu_agent_init");
#endif
    return 0;
}

#define AGENT_BUFF_SIZE          (4*1024)
static int smmu_tee_agent_run(struct tee_agent_kernel_ops *agent_instance)
{
    tc_ns_shared_mem *shared_mem = NULL;
    tc_ns_dev_file dev = {0};
    int ret = 0;

    /* 1. Allocate agent buffer */
#ifdef CFG_HI_TEE_ITRUSTEE_SUPPORT
    shared_mem = tc_mem_allocate(AGENT_BUFF_SIZE);
#else
    shared_mem = tc_mem_allocate(&dev, AGENT_BUFF_SIZE, true);
#endif
    if (IS_ERR(shared_mem)) {
        printk("%s(%d) allocate agent buffer fail \n", __FUNCTION__, __LINE__);
        ret = PTR_ERR(shared_mem);
        goto out;
    }
    /* 2. Register agent buffer to TEE */
#ifdef CFG_HI_TEE_ITRUSTEE_SUPPORT
    agent_instance->agent_buff = shared_mem;
    ret = tc_ns_register_agent(&dev, agent_instance->agent_id, shared_mem->len,
                               (void **)(&(shared_mem->kernel_addr)), false);
#else
    agent_instance->agent_buffer = shared_mem;
    ret = tc_ns_register_agent(&dev, agent_instance->agent_id, shared_mem);
#endif
    if (ret) {
        printk("%s(%d) register agent buffer fail \n", __FUNCTION__, __LINE__);
        ret = -1;
        goto out;
    }

    /* 3. Create thread to run agent */
    agent_instance->agent_thread =
        kthread_run(smmu_tee_agent_work, (void *)agent_instance, "agent_%s", agent_instance->agent_name);
    if (IS_ERR(agent_instance->agent_thread)) {
        printk("%s(%d) kthread create fail \n", __FUNCTION__, __LINE__);
        ret = PTR_ERR(agent_instance->agent_thread);
        agent_instance->agent_thread = NULL;
        goto out;
    }
    return 0;

out:
    if (!IS_ERR_OR_NULL(shared_mem)) {
        tc_mem_free(shared_mem);
#ifdef CFG_HI_TEE_ITRUSTEE_SUPPORT
        agent_instance->agent_buff = NULL;
#else
        agent_instance->agent_buffer = NULL;
#endif
        shared_mem = NULL;
    }
    if (!IS_ERR_OR_NULL(agent_instance->agent_thread)) {
        kthread_stop(agent_instance->agent_thread);
        agent_instance->agent_thread = NULL;
    }

    return ret;
}

static int smmu_agent_exit(struct tee_agent_kernel_ops* agent_instance)
{
        return 0;
}

static int smmu_agent_stop(struct tee_agent_kernel_ops *agent_instance)
{
    int ret;

    /*
     * make agent in secure os to send message to agent in
     * no secure world to stop wait event, and stop the thread
     */
    ret = hi_tee_agent_end();
    if (ret) {
        printk("%s(%d) close agend failed!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    tc_ns_unregister_agent(agent_instance->agent_id);
    if (!IS_ERR_OR_NULL(agent_instance->agent_thread)) {
        kthread_stop(agent_instance->agent_thread);
    }
    return 0;
}

static struct tee_agent_kernel_ops g_smmu_agent_ops = {
    .agent_name = "smmu",
    .agent_id = SMMU_AGENT_ID,
    .tee_agent_init = smmu_agent_init,
    .tee_agent_run = smmu_tee_agent_run,
    .tee_agent_work = smmu_agent_work,
    .tee_agent_exit = smmu_agent_exit,
    .tee_agent_stop = smmu_agent_stop,
    .tee_agent_crash_work = NULL,
    .list = LIST_HEAD_INIT(g_smmu_agent_ops.list)
};

static int smmu_tee_agent_init(struct tee_agent_kernel_ops* agent_instance)
{
    int ret;

    if ((agent_instance == NULL) || (agent_instance->agent_id == 0) ||
        (agent_instance->agent_name == NULL) || (agent_instance->tee_agent_work == NULL)) {
        printk("%s(%d) agent is invalid \n", __FUNCTION__, __LINE__);
        return -1;
    }

    /* Initialize the agent */
    if (agent_instance->tee_agent_init)
        ret = agent_instance->tee_agent_init(agent_instance);
    else {
        printk("%s(%d) tee_agent_init %s should not be null \n", __FUNCTION__, __LINE__, agent_instance->agent_name);
        return -1;
    }

    if (ret) {
        printk("%s(%d) tee_agent_init %s failed \n", __FUNCTION__, __LINE__, agent_instance->agent_name);
        return -1;
    }
    /* Run the agent */
    if (agent_instance->tee_agent_run)
        ret = agent_instance->tee_agent_run(agent_instance);
    else {
        printk("%s(%d) tee_agent_run %s should not be null \n", __FUNCTION__, __LINE__, agent_instance->agent_name);
        return -1;
    }

    if (ret) {
        printk("%s(%d) tee_agent_run %s failed \n", __FUNCTION__, __LINE__, agent_instance->agent_name);
        if (agent_instance->tee_agent_exit)
            agent_instance->tee_agent_exit(agent_instance);
        return -1;
    }

    return 0;
}

static int hi_smmuagent_suspend(struct platform_device *pdev, pm_message_t state)
{
#ifdef CFG_HI_TEE_ITRUSTEE_SUPPORT
    kthread_run(hisi_teesmmu_agent_init, "suspend", "hisi_teesmmu_agent_init");
#endif

    return 0;
}
static int hi_smmuagent_resume(struct platform_device *pdev)
{
#ifdef CFG_HI_TEE_ITRUSTEE_SUPPORT
    kthread_run(hisi_teesmmu_agent_init, "resume", "hisi_teesmmu_agent_init");
#endif

    return 0;
}
static const struct of_device_id hi_smmuagent_match[] = {
    { .compatible = "hisilicon,hi_smmuagent" },
    { }
};
static struct platform_driver hi_smmuagent = {
    .suspend = hi_smmuagent_suspend,
    .resume  = hi_smmuagent_resume,
    .driver  = {
        .name   = "hi_smmuagent",
        .of_match_table = hi_smmuagent_match,
    },
};

static struct platform_device hi_smmuagent_dev = {
    .name = "hi_smmuagent"
};

int tee_mem_agent_register(void)
{
    int ret;
    ret = hisi_teesmmu_init();
    if (ret) {
        printk("%s(%d) hisi_teesmmu_init failed!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    platform_device_register(&hi_smmuagent_dev);
    platform_driver_register(&hi_smmuagent);

    ret = tee_agent_kernel_register(&g_smmu_agent_ops);
    if (ret) {
        printk("%s(%d) register smmu agent failed!\n", __FUNCTION__, __LINE__);
        return -1;
    }
    ret = smmu_tee_agent_init(&g_smmu_agent_ops);
    if (ret) {
        printk("%s(%d) smmu agent init failed!\n", __FUNCTION__, __LINE__);
        /* unregister  */
        list_del(&(g_smmu_agent_ops.list));
        return -1;
    }
    return 0;
}

int tee_mem_agent_unregister(void)
{
    int ret = 0;
    struct tee_agent_kernel_ops *agent_instance = &g_smmu_agent_ops;

    if (agent_instance->tee_agent_exit) {
        ret = agent_instance->tee_agent_exit(agent_instance);
    } else {
        printk("%s(%d) tee_agent_init %s should not be null \n", __FUNCTION__, __LINE__, agent_instance->agent_name);
        return -1;
    }

    if (agent_instance->tee_agent_stop) {
        agent_instance->tee_agent_stop(agent_instance);
    }

    platform_driver_unregister(&hi_smmuagent);
    platform_device_unregister(&hi_smmuagent_dev);

    hisi_teesmmu_exit();

    return 0;
}
