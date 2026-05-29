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

#ifndef DRV_OSAL_LIB_H
#define DRV_OSAL_LIB_H

#include "drv_osal_init.h"

/*! \check uuid, but misamtch with file system verify  */
#define crypto_chk_owner_err_return(local)                      \
    do {                                                        \
        CRYPTO_OWNER owner;                                     \
        crypto_get_owner(&owner);                               \
        if (memcmp(&owner, local, sizeof(owner)) != 0) {        \
            print_errno(OT_ERR_CIPHER_ILLEGAL_UUID);  \
            return OT_ERR_CIPHER_ILLEGAL_UUID;                  \
        }                                                       \
    } while (0)

/* max length module name */
#define CRYPTO_MODULE_NAME_LEN             16

/* the max cipher hard channel count. */
#define CRYPTO_HARD_CHANNEL_MAX          0x10

/* the max cipher hard channel count. */
#define KEYSLOT_CHANNEL_MAX              0x10

/* 512M */
#define MAX_COPY_FROM_USER_SIZE    0x20000000

/* serure mmz or not, not used */
#define MEM_TYPE_MMZ                     0x00
#define MEM_TYPE_SMMU                    0x01

/* Cipher cmd param buffer size for ioctrl. */
#define CRYPTO_CMD_PARAM_SIZE            256

/* big coding transform to little coding. */
#define crypto_cpu_to_be16(v) (((v) << 8) | ((v) >> 8))

#define crypto_cpu_to_be32(v)                                       \
        ((((td_u32)(v)) >> 24) | ((((td_u32)(v)) >> 8) & 0xff00) |  \
        ((((td_u32)(v)) << 8) & 0xff0000) | (((td_u32)(v)) << 24))

#define crypto_cpu_to_be64(x) ((td_u64)(                    \
        (((td_u64)(x) & (td_u64)0x00000000000000ffULL) << 56) | \
        (((td_u64)(x) & (td_u64)0x000000000000ff00ULL) << 40) | \
        (((td_u64)(x) & (td_u64)0x0000000000ff0000ULL) << 24) | \
        (((td_u64)(x) & (td_u64)0x00000000ff000000ULL) << 8) |  \
        (((td_u64)(x) & (td_u64)0x000000ff00000000ULL) >> 8) |  \
        (((td_u64)(x) & (td_u64)0x0000ff0000000000ULL) >> 24) | \
        (((td_u64)(x) & (td_u64)0x00ff000000000000ULL) >> 40) | \
        (((td_u64)(x) & (td_u64)0xff00000000000000ULL) >> 56)))

/* defined the base error code. */
#define OT_BASE_ERR_BASE                0x400
#define OT_BASE_ERR_BASE_SYMC           (OT_BASE_ERR_BASE + 0x100)
#define OT_BASE_ERR_BASE_HASH           (OT_BASE_ERR_BASE + 0x200)
#define OT_BASE_ERR_BASE_RSA            (OT_BASE_ERR_BASE + 0x300)
#define OT_BASE_ERR_BASE_TRNG           (OT_BASE_ERR_BASE + 0x400)
#define OT_BASE_ERR_BASE_SM2            (OT_BASE_ERR_BASE + 0x500)

/* enumeration module_id. */
typedef enum {
    CRYPTO_MODULE_ID_SYMC,        /* Symmetric Cipher */
    CRYPTO_MODULE_ID_SYMC_KEY,    /* Symmetric Cipher key */
    CRYPTO_MODULE_ID_HASH,        /* Message Digest */
    CRYPTO_MODULE_ID_IFEP_RSA,    /* Asymmetric developed by IFEP */
    CRYPTO_MODULE_ID_SIC_RSA,     /* Asymmetric developed by SIC */
    CRYPTO_MODULE_ID_TRNG,        /* Random Data Generation */
    CRYPTO_MODULE_ID_PKE,         /* Public Key Cryptographic Algorithm Based on Elliptic Curves */
    CRYPTO_MODULE_ID_SM4,         /* SM4 */
    CRYPTO_MODULE_ID_SMMU,        /* SMMU */
    CRYPTO_MODULE_ID_CNT,         /* Count of module id */
} module_id;

/* struct channel
 * the context of hardware channel.
 */
typedef struct {
    /* the state of instance, open or closed. */
    td_u32 open;

    /* the context of channel, which is defined by specific module. */
    td_void *ctx;
} channel_context;

/* struct of crypto_mem. */
typedef struct {
    compat_addr dma_addr;      /* dam addr, may be mmz or smmu */
    compat_addr mmz_addr;      /* mmz addr, sometimes the smmu must mapped from mmz */
    td_void *dma_virt;         /* cpu virtual addr mapped from dam addr */
    td_u32 dma_size;           /* dma memory size */
    td_void *user_buf;         /* buffer of user */
} crypto_mem;

typedef enum {
    MODULE_INFO_RESET_VALID,
} module_info_option;

/*! \****************************** API Declaration *****************************/

/* cipher drv mode init. */
td_s32 cipher_drv_mod_init(td_void);

/* cipher drv mode deinit. */
td_void cipher_drv_mod_exit(td_void);

/* cipher check addr. */
td_s32 cipher_check_mmz_phy_addr(td_phys_addr_t phy_addr, td_u32 length);

/* cipher crypto ioctl. */
td_s32 crypto_ioctl(td_u32 cmd, td_void *argp);

/* cipher crypto entry for module init. */
td_s32 crypto_entry(void);

/* cipher crypto entry for module exit. */
td_s32 crypto_exit(void);

/* cipher crypto release. */
td_s32 crypto_release(void);

/*
 * \brief  init dma memory.
 */
td_void crypto_mem_init(td_void);

/*
 * \brief  deinit dma memory.
 */
td_void crypto_mem_deinit(td_void);

/*
 * \brief  allocate and map a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \param[in] size The size of mem.
 * \param[in] name The name of mem.
 * \return         TD_SUCCESS if successful, or OT_BASE_ERR_MALLOC_FAILED.
 */
td_s32 crypto_mem_create(crypto_mem *mem, td_u32 type, const char *name, td_u32 size);

/*
 * \brief  destroy and unmap a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \return         0 if successful, or OT_BASE_ERR_UNMAP_FAILED.
 */
td_s32 crypto_mem_destroy(crypto_mem *mem);

/*
 * \brief  map a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \param[in] dma_ddr The address of dma mem.
 * \param[in] dma_size The size of dma mem.
 * \return         TD_SUCCESS if successful, or OT_BASE_ERR_MAP_FAILED.
 */
td_s32 crypto_mem_open(crypto_mem *mem, compat_addr dma_ddr, td_u32 dma_size);

/*
 * \brief  unmap a dma memory.
 * \param[in] mem  The struct of crypto_mem.
 * \param[in] dma_ddr The address of dma mem.
 * \return         TD_SUCCESS if successful, or OT_BASE_ERR_UNMAP_FAILED.
 */
td_s32 crypto_mem_close(crypto_mem *mem);

/*
 * \brief  get dma memory virtual address
 * \param[in] mem The struct of crypto_mem.
 * \return         dma_addr if successful, or zero.
 */
td_void *crypto_mem_virt(const crypto_mem *mem);

/*
 * \brief  check whether cpu is secure or not.
 * \retval secure cpu, true is returned otherwise false is returned.
 */
td_u32 crypto_is_sec_cpu(td_void);

extern int check_otp_cmd_mode(void);

td_s32 crypto_get_cpu_secure_sta(td_void);

/*
 * \brief  map the physics addr to cpu within the base table, contains the base addr and crg addr.
 * \retval    on success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 module_addr_map(td_void);

/*
 * \brief  unmap the physics addr to cpu within the base table, contains the base addr and crg addr.
 * \retval    on success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 module_addr_unmap(td_void);

/*
 * \brief  get secure cpu type.
 */
td_u32 module_get_secure(td_void);

/*
 * \brief  enable a module, open clock  and remove reset signal.
 * \param[in]  id The module id.
 * \retval    NA
 */
td_void module_enable(module_id id);

/*
 * \brief  disable a module, close clock and set reset signal.
 * \param[in] id The module id.
 * \retval    NA
 */
td_void module_disable(module_id id);

/*
 * brief  Wait queue callback func.
 * retval if condition match, return TD_TRUE.
 */
td_s32 crypto_queue_callback_func(const td_void *param);

/*
 * \brief  set irq number.
 * \param[in]  id The module id.
 * \param[in]  irq irq number.
 * \retval    NA.
 */
td_void module_set_irq(module_id id, td_u32 irq);

td_void module_set_nsec_irq(module_id id, td_u32 irq);

/*
 * \brief  get attribute of module.
 * \param[in]  id The module id.
 * \param[out] int_valid enable interrupt or not.
 * \param[out] int_num interrupt number of module.
 * \param[out] name name of module.
 * \retval    on success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_void module_get_attr(module_id id, td_u32 *int_valid, td_u32 *int_num, const char **name);

/*
 * \brief  get info of module.
 * \param[in]  id The module id.
 * \param[out] info info of module.
 * \param[in]  option .
 * \retval    NA.
 */
td_void module_get_info(module_id id, td_u32 *info, module_info_option option);

/*
 * \brief  read a register.
 * \param[in]  id The module id.
 * \param[in]  offset The module id.
 * \retval    the value of register
 */
td_u32 module_reg_read(module_id id, td_u32 offset);

/*
 * \brief  hex to string.
 * \param[in]  buf The string buffer.
 * \param[in]  val The value of hex.
 * \retval    NA
 */
td_void hex2str(char *buf, td_u8 val);

/*
 * \brief  write a register.
 * \param[in]  id The module id.
 * \retval    NA
 */
td_void module_reg_write(module_id id, td_u32 offset, td_u32 val);

/* cipher module read and write a register */
#define symc_read(offset)         module_reg_read(CRYPTO_MODULE_ID_SYMC, offset)
#define symc_write(offset, val)   module_reg_write(CRYPTO_MODULE_ID_SYMC, offset, val)

/* hash module read and write a register */
#define hash_read(offset)         module_reg_read(CRYPTO_MODULE_ID_HASH, offset)
#define hash_write(offset, val)   module_reg_write(CRYPTO_MODULE_ID_HASH, offset, val)

/* rsa module read and write a register */
#define ifep_rsa_read(offset)       module_reg_read(CRYPTO_MODULE_ID_IFEP_RSA, offset)
#define ifep_rsa_write(offset, val) module_reg_write(CRYPTO_MODULE_ID_IFEP_RSA, offset, val)

/* trng module read and write a register */
#define trng_read(offset)         module_reg_read(CRYPTO_MODULE_ID_TRNG, offset)
#define trng_write(offset, val)   module_reg_write(CRYPTO_MODULE_ID_TRNG, offset, val)

/* sm2 module read and write a register */
#define pke_read(offset)         module_reg_read(CRYPTO_MODULE_ID_PKE, offset)
#define pke_write(offset, val)   module_reg_write(CRYPTO_MODULE_ID_PKE, offset, val)

/* smmu module read and write a register */
#define smmu_read(offset)         module_reg_read(CRYPTO_MODULE_ID_SMMU, offset)
#define smmu_write(offset, val)   module_reg_write(CRYPTO_MODULE_ID_SMMU, offset, val)

/*
 * \brief  Initialize the channel list.
 * \param[in]  ctx The context of channel.
 * \param[in]  num The channel numbers, max is 32.
 * \param[in]  ctx_size The size of context.
 * \retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 crypto_channel_init(channel_context *ctx, td_u32 num, td_u32 ctx_size);

/*
 * \brief  Deinitialize the channel list.
 * \param[in]  ctx The context of channel.
 * \param[in]  num The channel numbers, max is 32.
 * \retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 crypto_channel_deinit(channel_context *ctx, td_u32 num);

/*
 * \brief  allocate a channel.
 * \param[in]  ctx The context of channel.
 * \param[in]  num The channel numbers, max is 32.
 * \param[in]  mask Mask which channel allowed be alloc, max is 32.
 * \param[out] id The id of channel.
 * \retval     On success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_s32 crypto_channel_alloc(channel_context *ctx, td_u32 num, td_u32 mask, td_u32 *id);

/*
 * \brief  free a channel.
 * \param[in]  ctx The context of channel.
 * \param[in]  num The channel numbers, max is 32.
 * \param[in] id The id of channel.
 * \retval    on success, TD_SUCCESS is returned.  On error, TD_FAILURE is returned.
 */
td_void crypto_channel_free(channel_context *ctx, td_u32 num, td_u32 id);

/*
 * \brief  get the private data of hard channel.
 * \param[in] ctx The context of channel.
 * \param[in] num The channel numbers, max is 32.
 * \param[in] id The id of channel.
 * \retval    on success, the address of context is returned.  On error, NULL is returned..
 */
td_void *crypto_channel_get_context(const channel_context *ctx, td_u32 num, td_u32 id);

/*
 * \brief  get the rang.
 * \retval    random number.
 */
td_u32 get_rand(td_void);

td_void smmu_get_table_addr(td_phys_addr_t *rdaddr, td_phys_addr_t *wraddr, td_u64 *table);

td_s32 crypto_copy_from_user(td_void *to, unsigned long to_len,
    const td_void *from, unsigned long from_len);

td_s32 crypto_copy_to_user(td_void *to, unsigned long to_len,
    const td_void *from, unsigned long from_len);

#endif /* DRV_OSAL_LIB_H */
