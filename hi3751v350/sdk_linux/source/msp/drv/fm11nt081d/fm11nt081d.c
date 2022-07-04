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

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/i2c.h>

#include "hi_drv_i2c.h"
#include "hi_osal.h"
#include "hi_module.h"
#include "hi_drv_gpio.h"
#include "fm11nt081d.h"

#define NFC_EEPROM_BASE_ADDR (0x0010)
#define NFC_EEPROM_FINALY_ADDR (0x0384)
#define NFC_CMD_LEN (2)
#define NFC_WECHAT_NDEF_LEN (34)
#define READ_NFC_WECHAT_NDEF_LEN (50)
#define NFC_TOUTIAO_NDEF_LEN (47)
#define READ_NFC_TOUTIAO_NDEF_LEN (63)
#define NFC_EERROM_READ_BUFF_LEN_MAX (888)
#define NFC_CSN_GPIO_NUM (44) // GPIO5_4 5*8+4 = 44

#define C081_NFC_ADDR (0xAE)
#define I2C_WR (0x00)
#define I2C_RD (0x01)
#define C081_NFC_WRITE_ADDR (C081_NFC_ADDR|I2C_WR)
#define C081_NFC_READ_ADDR (C081_NFC_ADDR|I2C_RD)
#define FM11_E2_USER_ADDR (0x0010)
#define FM11_E2_MAUNF_ADDR (0x03FF)
#define FM11_E2_BLOCK_SIZE (16)
#define DEFAULT_MD_LEN (128)
#define RIGHR_MOVE_8_BIT (8)
#define NFC_NDEF_MAX_LEN (888)

#define LOW_LEVEL  0
#define HIGH_LEVEL 1
#define I2C1_SDA 0x03
#define I2C1_SCL 0x03
#define GPIO5_4 0xF8A2149C
#define CSN_DOWN 0x0
#define GPIO0_4 0xF8A21000
#define GPIO0_5 0xF8A21004

static struct miscdevice nfc_miscdevice = { 0, };
hi_u8 wbuf[5] = {0x05, 0x78, 0xF7, 0x90, 0x02};

i2c_ext_func *i2c_func = HI_NULL;
gpio_ext_func *g_gpio_func = HI_NULL;

typedef enum {
    I2C_CHANNEL_0 = 0,
    I2C_CHANNEL_1 = 1
} i2c_channel_def;

static hi_u32 C081NfcI2cWrite(hi_u16 cmd, hi_u8 *data_buff, hi_u8 len)
{
    hi_u32 status = 0;
    hi_u32 ret = 0;
    hi_u32 id = I2C_CHANNEL_1;

    if (i2c_func == HI_NULL) {
        NFC_ERR("HI_ID_ I2C handle get failed \r\n");
        return HI_FAILURE;
    }

    if (data_buff == HI_NULL) {
        NFC_ERR("C081NfcI2cWrite data is null\r\n");
    }

    status = i2c_func->pfn_i2c_write(id, C081_NFC_ADDR & 0xFE, cmd, NFC_CMD_LEN, data_buff, len);
    if (status != 0) {
        return HI_FAILURE;
    }
    
    return 0;
}

static hi_void PullSetCsn(hi_s32 level)
{
    g_gpio_func->pfn_gpio_write_bit(NFC_CSN_GPIO_NUM, level);
}

static hi_void EepWritePage(hi_u8 *pBuffer, hi_u16 writeAddr, hi_u8 dataLen)
{
    hi_u32 status;
    if (pBuffer == NULL) {
        NFC_ERR("eepWritePage buffer is null\r\n");
    }
    PullSetCsn(LOW_LEVEL);
    status = C081NfcI2cWrite(writeAddr, pBuffer, dataLen);
    msleep(10); // The delay time must be 10ms
    PullSetCsn(HIGH_LEVEL);
}

static hi_void Fm11nt081dWriteEeprom(hi_u16 baseAddr, hi_u32 len, hi_u8 *wbuf)
{
    hi_u8 offset = 0;
    hi_u8 *writeBuff = wbuf;
    hi_u32 writeLen = len;
    hi_u16 addr = baseAddr;

    if (writeBuff == NULL) {
        NFC_ERR("write ndef is null\r\n");
        return;
    }

    if (addr < FM11_E2_USER_ADDR || addr >= FM11_E2_MAUNF_ADDR) {
        offset = FM11_E2_BLOCK_SIZE - (addr % FM11_E2_BLOCK_SIZE);
        NFC_ERR("offset = %d, writeLen = %d\r\n", offset, writeLen);
        if (writeLen > offset) {
            EepWritePage(writeBuff, addr, offset);
            addr += offset;
            writeBuff += offset;
            writeLen -= offset;
        } else {
            EepWritePage(writeBuff, addr, writeLen);
            writeLen = 0;
        }
    }
    while (writeLen) {
        if (writeLen >= FM11_E2_BLOCK_SIZE) {
            EepWritePage(writeBuff, addr, FM11_E2_BLOCK_SIZE);
            addr += FM11_E2_BLOCK_SIZE;
            writeBuff += FM11_E2_BLOCK_SIZE;
            writeLen -= FM11_E2_BLOCK_SIZE;
        } else {
            EepWritePage(writeBuff, addr, writeLen);
            writeLen = 0;
        }
    }
}

static hi_u32 WriteRead(hi_u16 cmd, hi_u8 send_len, hi_u8 read_len)
{
    hi_u32 i = 0;
    hi_u32 ret = 0;
    hi_u32 status = 0;
    hi_u32 id = I2C_CHANNEL_1;
    hi_u8 recvData[888] = {0};

    if (i2c_func == HI_NULL) {
        NFC_ERR("HI_ID_I2C handle get failed \r\n");
        return HI_FAILURE;
    }

    status = i2c_func->pfn_i2c_read(id, C081_NFC_ADDR | I2C_RD, cmd, send_len, recvData, read_len);
    if (status != 0) {
        return HI_FAILURE;
    }

    NFC_INFO("<Fm11nt081d>: read ndef data\r\n");
    
    for (i = 0; i < read_len; i++) {
        NFC_ERR("0x%x ", recvData[i]);
    }

    NFC_INFO("\r\n");
    return 0;
}

static hi_u32 Fm11nt081ReadEep(hi_u16 readAddr, hi_u16 len)
{
    hi_u32 status;

    status = WriteRead(readAddr, NFC_CMD_LEN, len);
    if (status != 0) {
        return HI_FAILURE;
    }
    return  0;
}

static const struct i2c_device_id nfc_id[] = {
    {"ft", 0},
    {}
};

MODULE_DEVICE_TABLE(i2c, nfc_id);

static hi_s32 Fm11nt081dSetReg(hi_u32 addr, hi_u32 value)
{
    void* pmem = ioremap(addr, DEFAULT_MD_LEN);

    if (pmem == HI_NULL) {
        return HI_FAILURE;
    }

    *(hi_u32*)pmem = value;
    iounmap(pmem);
    return 0;
}

hi_u8 ndefFile[4][NFC_NDEF_MAX_LEN] = {
    // wechat
    {
        0x03, 0x20,
        0xd4, 0x0f, 0x0e, 0x61, 0x6e, 0x64, 0x72, 0x6f,
        0x69, 0x64, 0x2e, 0x63, 0x6f, 0x6d, 0x3a, 0x70,
        0x6b, 0x67, 0x63, 0x6f, 0x6d, 0x2e, 0x74, 0x65,
        0x6e, 0x63, 0x65, 0x6e, 0x74, 0x2e, 0x6d, 0x6d,
    },
    // today headline
    {
        0x03, 0x2d,
        0xd4, 0x0f, 0x1b, 0x61, 0x6e, 0x64, 0x72, 0x6f,
        0x69, 0x64, 0x2e, 0x63, 0x6f, 0x6d, 0x3a, 0x70,
        0x6b, 0x67, 0x63, 0x6f, 0x6d, 0x2e, 0x73, 0x63,
        0x2e, 0x61, 0x6e, 0x64, 0x72, 0x6f, 0x69, 0x64,
        0x2e, 0x61, 0x72, 0x74, 0x69, 0x63, 0x6c, 0x65,
    },
    // taobao
    {
        0x03, 0x23,
        0xd4, 0x0f, 0x11, 0x61, 0x6e, 0x64, 0x72, 0x6f,
        0x69, 0x64, 0x2e, 0x63, 0x6f, 0x6d, 0x3a, 0x70,
        0x6b, 0x67, 0x63, 0x6f, 0x6d, 0x2e, 0x74, 0x61,
        0x6f, 0x62, 0x61, 0x6f, 0x2e, 0x74, 0x61, 0x6f,
        0x62, 0x61, 0x6f,
    },
    // smart life
    {
        0x03, 0x26,
        0xd4, 0x0f, 0x14, 0x61, 0x6e, 0x64, 0x72, 0x6f,
        0x69, 0x64, 0x2e, 0x63, 0x6f, 0x6d, 0x3a, 0x70,
        0x6b, 0x67, 0x63, 0x6f, 0x6d, 0x2e, 0x68, 0x75,
        0x61, 0x77, 0x65, 0x69, 0x2e, 0x73, 0x6d, 0x61,
        0x72, 0x74, 0x68, 0x6f, 0x6d, 0x65,
    }
};

static hi_u32 nfc_read_ep(hi_u8 *recvData)
{
    hi_u32 id = I2C_CHANNEL_1;
    hi_u32 status = 0;
    hi_s32 i;
    hi_u32 ret = 0;

    if (i2c_func == HI_NULL) {
        NFC_ERR("HI_ID_I2C handle get failed \r\n");
        return HI_FAILURE;
    }

    status = i2c_func->pfn_i2c_read(id, C081_NFC_ADDR | I2C_RD, NFC_EEPROM_BASE_ADDR, NFC_CMD_LEN, recvData,
        NFC_TOUTIAO_NDEF_LEN);
    if (status != 0) {
        return HI_FAILURE;
    }

    NFC_INFO("<Fm11nt081d>: read ndef data\r\n");
    
    for (i = 0; i < strlen(recvData); i++) {
        NFC_ERR("0x%x ", recvData[i]);
    }

    NFC_INFO("\r\n");
    return 0;
}


hi_void nfc_write_ep(hi_u8 *buf)
{
    Fm11nt081dWriteEeprom(0x3B1, 1, &wbuf[1]);
    Fm11nt081dWriteEeprom(0x3B5, 1, &wbuf[3]); // 3 for index

    /* write NFC EEPROM */
    Fm11nt081dWriteEeprom(NFC_EEPROM_BASE_ADDR, NFC_TOUTIAO_NDEF_LEN, buf);
    // csn pull down
    PullSetCsn(LOW_LEVEL);
}

static hi_s32 nfc_open(struct inode *inode, struct file *filp)
{
    NFC_INFO("Enter nfc_open");
    return HI_SUCCESS;
}

static hi_s32 nfc_release(struct inode *inode, struct file *filp)
{
    NFC_INFO("Enter nfc_release");
    return HI_SUCCESS;
}

#ifdef HAVE_UNLOCKED_IOCTL
static long nfc_ioctl(struct file *filp, hi_u32 cmd, unsigned long arg)
#else
static hi_s32 nfc_ioctl(struct inode *inode, struct file *filp, hi_u32 cmd, unsigned long arg)
#endif
{
    hi_s32 ret = 0;
    hi_char data_buf[120];

    if (_IOC_TYPE(cmd) != HI_ID_NFC)
        return -EINVAL;
    
    if (_IOC_NR(cmd) > NFC_MAX_CMD)
        return -EINVAL;

    switch (cmd) {
        case NFC_READ:
            nfc_read_ep(data_buf);
            ret = copy_to_user((hi_char __user *)arg, data_buf, sizeof(data_buf));
            if (ret) {
                ret = -EFAULT;
            }
            NFC_INFO("io ctl read_led: %d", strlen(data_buf));
            break;
        case NFC_WRITE:
            ret = copy_from_user(data_buf, (hi_char __user *)arg, sizeof(data_buf));
            if (ret) {
                ret = -EFAULT;
            }
            nfc_write_ep(data_buf);
            break;
        default:
            ret = -EFAULT;
    }
    return ret;
}

static ssize_t nfc_read(struct file *filp, hi_char __user *buf, size_t size, loff_t *offset)
{
    hi_s32 ret;
    hi_char recv_msg[20];
    ret = copy_to_user(buf, recv_msg, size);
    NFC_INFO("nfc_read: %s, %d", buf, size);

    if (ret < 0) {
        return -EFAULT;
    }
    return ret;
}

static ssize_t nfc_write(struct file *filp, const hi_char *buf, size_t size, loff_t *offset)
{
    hi_s32 ret;
    hi_char send_msg[20];
    ret = copy_from_user(send_msg, buf, size);
    if (ret < 0) {
        return -EFAULT;
    }
    return ret;
}

struct file_operations nfc_fops = {
    .owner = THIS_MODULE,
    .open = nfc_open,
    .release = nfc_release,
#ifdef HAVE_UNLOCKED_IOCTL
    .unlocked_ioctl = nfc_ioctl,
#else
    .ioctl = nfc_ioctl,
#endif
    .read = nfc_read,
    .write = nfc_write,
};

static hi_s32 __init fm11nt081d_init(hi_void)
{
    hi_s32 ret = 0;
    NFC_INFO("-----------------fm11nt081d_init-----------\n");

    // fm11nt081d NFC GPIO configure
    Fm11nt081dSetReg(GPIO5_4, CSN_DOWN);
    Fm11nt081dSetReg(GPIO0_4, I2C1_SDA);
    Fm11nt081dSetReg(GPIO5_4, I2C1_SCL);

    gpio_drv_get_gpio_ext_func(&g_gpio_func);
    if (g_gpio_func == HI_NULL) {
        NFC_ERR("get gpio export function failed!\n");
        return -EFAULT;
    }

    /* set GPIO to be output and low level */
    g_gpio_func->pfn_gpio_dir_set_bit(NFC_CSN_GPIO_NUM, LOW_LEVEL);
    g_gpio_func->pfn_gpio_write_bit(NFC_CSN_GPIO_NUM, LOW_LEVEL);

    /* i2c_1 init */
    ret = hi_drv_i2c_init();
    if (ret) {
        NFC_ERR("I2C init failed : %d\n", ret);
        return -EFAULT;
    }
    i2c_drv_get_i2c_ext_func(&i2c_func);
    if (i2c_func == HI_NULL) {
        NFC_ERR("HI_ID_I2C handle get failed \r\n");
        return -EFAULT;
    }

    nfc_miscdevice.minor = MISC_DYNAMIC_MINOR;
    nfc_miscdevice.name = HINFC_DEVICE_NAME;
    nfc_miscdevice.fops = &nfc_fops;

    ret = misc_register(&nfc_miscdevice);
    if (ret != 0) {
        NFC_ERR("[%s, line: %d]Error: can't register\n", "NFC", __LINE__);
        return -EFAULT;
    }
    return 0;
}

static hi_void __exit fm11nt081d_exit(hi_void)
{
    NFC_INFO("-----------------fm11nt081d_exit-----------\n");
    hi_drv_i2c_de_init();

    misc_deregister(&nfc_miscdevice);
}
module_init(fm11nt081d_init);
module_exit(fm11nt081d_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");
