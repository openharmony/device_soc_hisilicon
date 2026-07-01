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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include "securec.h"
#include "ot_common.h"
#include "dev_ext.h"
#include "drv_cipher_debug.h"
#include "sys_cipher.h"

#define CRYPTO_MUTEX                pthread_mutex_t
#define crypto_mutex_init(x)        (td_void)pthread_mutex_init(x, NULL)
#define crypto_mutex_lock(x)        (td_void)pthread_mutex_lock(x)
#define crypto_mutex_unlock(x)      (td_void)pthread_mutex_unlock(x)
#define crypto_mutex_destroy(x)     pthread_mutex_destroy(x)

#define cipher_open(name, flags, mode)     open(name, flags, mode)
#define cipher_close(fd)                   close(fd)
#define cipher_ioctl(fd, request, var)     ioctl(fd, request, var)

typedef struct {
    CRYPTO_MUTEX lock;
    td_s32 ref_count;
    td_s32 cipher_fd;
} sys_cipher_mgmt;

static sys_cipher_mgmt g_sys_cipher_mgmt = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .ref_count = -1, /* < 0: close, 0: open > 0: multiple initialization */
    .cipher_fd = -1, /* handle of cipher device */
};

#define CIPHER_INIT_MAX_NUM     0x7FFFFFFF

#define _sys_cipher_not_init_return(ref_count)                      \
    do {                                                            \
        if ((ref_count) < 0) {                                        \
            ot_cipher_error("cipher init counter %d\n", ref_count);    \
            print_errno(OT_ERR_CIPHER_NOT_INIT);          \
            return OT_ERR_CIPHER_NOT_INIT;                          \
        }                                                           \
    } while (0)

/* ****************************** API Code **************************** */
static sys_cipher_mgmt *_get_sys_cipher_mgmt(td_void)
{
    return &g_sys_cipher_mgmt;
}

td_s32 sys_symc_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    crypto_mutex_lock(&mgmt->lock);

    if (mgmt->ref_count >= 0) {
        if (mgmt->ref_count < CIPHER_INIT_MAX_NUM) {
            mgmt->ref_count++;
        }
        crypto_mutex_unlock(&mgmt->lock);
        return TD_SUCCESS;
    }

    mgmt->cipher_fd = cipher_open("/dev/"UMAP_DEVNAME_CIPHER_BASE, O_RDWR, 0);
    if (mgmt->cipher_fd < 0) {
        crypto_mutex_unlock(&mgmt->lock);
        ot_cipher_error("cipher_open /dev/%s failed. ret %x\n", UMAP_DEVNAME_CIPHER_BASE, OT_ERR_CIPHER_FAILED_INIT);
        return OT_ERR_CIPHER_FAILED_INIT;
    }
    mgmt->ref_count = 0;

    crypto_mutex_unlock(&mgmt->lock);

    func_exit();

    return ret;
}

td_s32 sys_symc_deinit(td_void)
{
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    crypto_mutex_lock(&mgmt->lock);

    if (mgmt->ref_count != 0) {
        if (mgmt->ref_count > 0) {
            mgmt->ref_count--;
        }

        crypto_mutex_unlock(&mgmt->lock);
        return TD_SUCCESS;
    }

    if (cipher_close(mgmt->cipher_fd) < 0) {
        mgmt->cipher_fd = -1;
        mgmt->ref_count = -1;
        crypto_mutex_unlock(&mgmt->lock);
        ot_cipher_error("cipher_close /dev/%s failed.\n", UMAP_DEVNAME_CIPHER_BASE);
        return OT_ERR_CIPHER_FAILED_INIT;
    }
    mgmt->cipher_fd = -1;
    mgmt->ref_count = -1;

    crypto_mutex_unlock(&mgmt->lock);

    func_exit();

    return TD_SUCCESS;
}

td_s32 sys_symc_create(td_u32 *id, ot_cipher_type type)
{
    td_s32 ret;
    symc_create_t create;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&create, sizeof(create), 0, sizeof(create));

    create.type = type;
    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SYMC_CREATE, &create);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    *id = create.id;

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_symc_destroy(td_u32 id)
{
    td_s32 ret;
    symc_destroy_t destroy;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&destroy, sizeof(destroy), 0, sizeof(destroy));

    destroy.id = id;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SYMC_DESTROY, &destroy);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

static td_s32 sys_symc_set_cfg(symc_cfg_t *cfg, td_u32 id, const ot_cipher_ctrl *ctrl)
{
    const td_u32 *iv = TD_NULL;

    (td_void)memset_s(cfg, sizeof(symc_cfg_t), 0, sizeof(symc_cfg_t));
    cfg->id = id;
    cfg->alg = ctrl->alg;
    cfg->mode = ctrl->work_mode;
    cfg->width = OT_CIPHER_BIT_WIDTH_128BIT;
    cfg->ivlen = AES_IV_SIZE;

    if (ctrl->alg == OT_CIPHER_ALG_AES) {
        if (ctrl->work_mode == OT_CIPHER_WORK_MODE_CCM ||
            ctrl->work_mode == OT_CIPHER_WORK_MODE_GCM) {
            const ot_cipher_ctrl_aes_ccm_gcm *aes_ccm_gcm = &ctrl->aes_ccm_gcm_ctrl;
            cfg->klen = aes_ccm_gcm->key_len;
            cfg->ivlen = aes_ccm_gcm->iv_len;
            cfg->iv_usage = OT_CIPHER_IV_CHG_ONE_PACK;
            cfg->tlen = aes_ccm_gcm->tag_len;
            cfg->alen = aes_ccm_gcm->aad_len;
            addr_u64(cfg->aad) = aes_ccm_gcm->aad_phys_addr;
            iv = aes_ccm_gcm->iv;
        } else {
            const ot_cipher_ctrl_aes *aes = &ctrl->aes_ctrl;
            cfg->klen = aes->key_len;
            cfg->iv_usage = aes->chg_flags;
            cfg->width = aes->bit_width;
            iv = aes->iv;
        }
    } else if (ctrl->alg == OT_CIPHER_ALG_SM4) {
        const ot_cipher_ctrl_sm4 *sm4 = &ctrl->sm4_ctrl;
        cfg->iv_usage = sm4->chg_flags;
        iv = sm4->iv;
    } else if (ctrl->alg != OT_CIPHER_ALG_DMA) {
        ot_cipher_error("Unsupported alg %d\n", ctrl->alg);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    if (iv != TD_NULL) {
        if (memcpy_s(cfg->iv, sizeof(cfg->iv), iv, cfg->ivlen) != EOK) {
            return OT_ERR_CIPHER_INVALID_PARAM;
        }
    }

    return TD_SUCCESS;
}

td_s32 sys_symc_config(td_u32 id, const ot_cipher_ctrl *ctrl)
{
    td_s32 ret;
    symc_cfg_t cfg;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&cfg, sizeof(cfg), 0, sizeof(cfg));

    ret = sys_symc_set_cfg(&cfg, id, ctrl);
    if (ret != TD_SUCCESS) {
        print_func_errno(sys_symc_set_cfg, ret);
        return ret;
    }

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SYMC_SET_CFG, &cfg);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        (td_void)memset_s(&cfg, sizeof(cfg), 0, sizeof(cfg));
        return ret;
    }

    (td_void)memset_s(&cfg, sizeof(cfg), 0, sizeof(cfg));
    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_symc_get_config(td_u32 id, ot_cipher_ctrl *ctrl)
{
    td_s32 ret;
    symc_get_cfg_t get_cfg;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&get_cfg, sizeof(get_cfg), 0, sizeof(get_cfg));

    get_cfg.id = id;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SYMC_GET_CFG, &get_cfg);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }
    (td_void)memcpy_s(ctrl, sizeof(ot_cipher_ctrl), &get_cfg.ctrl, sizeof(ot_cipher_ctrl));

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_symc_crypto(td_u32 id, const compat_addr in, const compat_addr out, td_u32 len, td_u32 operation)
{
    td_s32 ret;
    symc_encrypt_t crypt;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    chk_param_err_return(len > 4 * 1024 * 1024); // 4 * 1024 * 1024 is the max size of malloci

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&crypt, sizeof(crypt), 0, sizeof(crypt));

    crypt.id = id;
    crypt.input = in;
    crypt.output = out;
    crypt.length = len;
    crypt.operation = operation;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SYMC_ENCRYPT, &crypt);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_aead_get_tag(td_u32 id, td_u8 *tag, td_u32 taglen)
{
    td_s32 ret;
    aead_tag_t aead_tag;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();
    chk_param_err_return(taglen > 16); /* 16 is MAX TAG LEN */
    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&aead_tag, sizeof(aead_tag), 0, sizeof(aead_tag));

    aead_tag.id = id;
    aead_tag.taglen = taglen;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SYMC_GET_TAG, &aead_tag);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    ret = memcpy_s(tag, taglen, aead_tag.tag, aead_tag.taglen);
    if (ret != EOK) {
        print_func_errno(memcpy_s, ret);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_hash_start(td_u32 *id, ot_cipher_hash_type type, const td_u8 *key, td_u32 keylen)
{
    td_s32 ret;
    hash_start_t start;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&start, sizeof(start), 0, sizeof(start));

    start.id = 0;
    start.type = type;
    start.keylen = keylen;
    addr_via_const(start.key) = key;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_HASH_START, &start);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    *id = start.id;

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_hash_update(td_u32 id, const td_u8 *input, td_u32 length, hash_chunk_src src)
{
    td_s32 ret;
    hash_update_t update;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&update, sizeof(update), 0, sizeof(update));

    update.id = id;
    addr_via_const(update.input) = input;
    update.length = length;
    update.src = src;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_HASH_UPDATE, &update);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_hash_finish(td_u32 id, td_u8 *hash, td_u32 hashlen)
{
    td_s32 ret;
    hash_finish_t finish;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&finish, sizeof(finish), 0, sizeof(finish));

    finish.id = id;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_HASH_FINISH, &finish);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    if (hashlen < finish.hashlen) {
        ot_cipher_error("hash out length is too short! expect %u, real %u\n", finish.hashlen, hashlen);
        return OT_ERR_CIPHER_INVALID_PARAM;
    }

    ret = memcpy_s(hash, hashlen, finish.hash, finish.hashlen);
    if (ret != EOK) {
        print_func_errno(memcpy_s, ret);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_rsa_encrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, sys_rsa_crypto_data *rsa_data)
{
    td_s32 ret;
    rsa_info_t rsa_info;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&rsa_info, sizeof(rsa_info_t), 0, sizeof(rsa_info_t));

    addr_via_const(rsa_info.n) = key->n;
    addr_via_const(rsa_info.d) = key->d;
    addr_via_const(rsa_info.p) = key->p;
    addr_via_const(rsa_info.q) = key->q;
    addr_via_const(rsa_info.dp) = key->dp;
    addr_via_const(rsa_info.dq) = key->dq;
    addr_via_const(rsa_info.qp) = key->qp;

    rsa_info.e = key->e;
    rsa_info.public = key->public;
    rsa_info.klen = key->klen;
    rsa_info.scheme = scheme;
    addr_via_const(rsa_info.in) = rsa_data->in;
    rsa_info.inlen = rsa_data->in_len;
    addr_via(rsa_info.out) = rsa_data->out;
    rsa_info.outlen = rsa_data->out_buf_len;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_RSA_ENCRYPT, &rsa_info);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    *rsa_data->out_len = rsa_info.outlen;

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_rsa_decrypt(ot_cipher_rsa_encrypt_scheme scheme,
    const cryp_rsa_key *key, sys_rsa_crypto_data *rsa_data)
{
    td_s32 ret;
    rsa_info_t rsa_info;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&rsa_info, sizeof(rsa_info), 0, sizeof(rsa_info));

    addr_via_const(rsa_info.n) = key->n;
    addr_via_const(rsa_info.d) = key->d;
    addr_via_const(rsa_info.p) = key->p;
    addr_via_const(rsa_info.q) = key->q;
    addr_via_const(rsa_info.dp) = key->dp;
    addr_via_const(rsa_info.dq) = key->dq;
    addr_via_const(rsa_info.qp) = key->qp;

    rsa_info.e = key->e;
    rsa_info.public = key->public;
    rsa_info.klen = key->klen;
    rsa_info.scheme = scheme;
    addr_via_const(rsa_info.in) = rsa_data->in;
    rsa_info.inlen = rsa_data->in_len;
    addr_via(rsa_info.out) = rsa_data->out;
    rsa_info.outlen = rsa_data->out_buf_len;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_RSA_DECRYPT, &rsa_info);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    *rsa_data->out_len = rsa_info.outlen;

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_rsa_sign_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *rsa_key, sys_rsa_sign_data *sign_data)
{
    td_s32 ret;
    rsa_info_t rsa_info;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&rsa_info, sizeof(rsa_info), 0, sizeof(rsa_info));

    addr_via_const(rsa_info.n)  = rsa_key->n;
    addr_via_const(rsa_info.d)  = rsa_key->d;
    addr_via_const(rsa_info.p)  = rsa_key->p;
    addr_via_const(rsa_info.q)  = rsa_key->q;
    addr_via_const(rsa_info.dp) = rsa_key->dp;
    addr_via_const(rsa_info.dq) = rsa_key->dq;
    addr_via_const(rsa_info.qp) = rsa_key->qp;

    rsa_info.e = rsa_key->e;
    rsa_info.public = rsa_key->public;
    rsa_info.klen = rsa_key->klen;
    rsa_info.scheme = scheme;
    addr_via_const(rsa_info.in) = sign_data->hash;
    rsa_info.inlen = sign_data->hash_len;
    addr_via(rsa_info.out) = sign_data->sign;
    rsa_info.outlen = sign_data->sign_len;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_RSA_SIGN, &rsa_info);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_rsa_verify_hash(ot_cipher_rsa_sign_scheme scheme,
    const cryp_rsa_key *rsa_key, const sys_rsa_sign_data *sign_data)
{
    td_s32 ret;
    rsa_info_t rsa_info;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&rsa_info, sizeof(rsa_info_t), 0, sizeof(rsa_info_t));

    addr_via_const(rsa_info.n) = rsa_key->n;

    rsa_info.e = rsa_key->e;
    rsa_info.public = rsa_key->public;
    rsa_info.klen = rsa_key->klen;
    rsa_info.scheme = scheme;
    addr_via_const(rsa_info.in) = sign_data->sign;
    rsa_info.inlen = sign_data->sign_len;
    addr_via_const(rsa_info.out) = sign_data->hash;
    rsa_info.outlen = sign_data->hash_len;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_RSA_VERIFY, &rsa_info);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_trng_get_random(td_u32 *randnum, td_u32 timeout)
{
    td_s32 ret;
    trng_t trng;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&trng, sizeof(trng), 0, sizeof(trng));

    trng.timeout = timeout;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_TRNG, &trng);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    *randnum = trng.randnum;
    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_sm2_sign(const ot_cipher_sm2_sign *sm2_sign,
    const ot_cipher_common_data *msg, ot_cipher_sm2_sign_data *sign_data)
{
    td_s32 ret;
    sm2_sign_t sign;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&sign, sizeof(sign), 0, sizeof(sign));
    (td_void)memcpy_s(sign.d, SM2_LEN_IN_BYTE, sm2_sign->d, SM2_LEN_IN_BYTE);
    (td_void)memcpy_s(sign.px, SM2_LEN_IN_BYTE, sm2_sign->px, SM2_LEN_IN_BYTE);
    (td_void)memcpy_s(sign.py, SM2_LEN_IN_BYTE, sm2_sign->py, SM2_LEN_IN_BYTE);
    sign.id = sm2_sign->id;
    sign.id_len = sm2_sign->id_len;
    if (msg != TD_NULL) {
        sign.msg = msg->data;
        sign.msglen = msg->data_len;
    }

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SM2_SIGN, &sign);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }
    (td_void)memcpy_s(sign_data->r, SM2_LEN_IN_BYTE, sign.r, SM2_LEN_IN_BYTE);
    (td_void)memcpy_s(sign_data->s, SM2_LEN_IN_BYTE, sign.s, SM2_LEN_IN_BYTE);

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_sm2_verify(const ot_cipher_sm2_verify *sm2_verify,
    const ot_cipher_common_data *msg, const ot_cipher_sm2_sign_data *sign_data)
{
    td_s32 ret;
    sm2_verify_t verify;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&verify, sizeof(verify), 0, sizeof(verify));
    (td_void)memcpy_s(verify.px, SM2_LEN_IN_BYTE, sm2_verify->px, SM2_LEN_IN_BYTE);
    (td_void)memcpy_s(verify.py, SM2_LEN_IN_BYTE, sm2_verify->py, SM2_LEN_IN_BYTE);
    verify.id = sm2_verify->id;
    verify.id_len = sm2_verify->id_len;
    if (msg != TD_NULL) {
        verify.msg = msg->data;
        verify.msglen = msg->data_len;
    }

    (td_void)memcpy_s(verify.r, SM2_LEN_IN_BYTE, sign_data->r, SM2_LEN_IN_BYTE);
    (td_void)memcpy_s(verify.s, SM2_LEN_IN_BYTE, sign_data->s, SM2_LEN_IN_BYTE);

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SM2_VERIFY, &verify);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_sm2_encrypt(const ot_cipher_sm2_public_key *sm2_key,
    const ot_cipher_common_data *plain_txt, ot_cipher_common_data *cipher_txt)
{
    td_s32 ret;
    sm2_encrypt_t encrypt;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&encrypt, sizeof(encrypt), 0, sizeof(encrypt));
    (td_void)memcpy_s(&encrypt.pub_key, sizeof(ot_cipher_sm2_public_key), sm2_key, sizeof(ot_cipher_sm2_public_key));
    encrypt.plain_data = plain_txt->data;
    encrypt.plain_len = plain_txt->data_len;
    encrypt.cipher_data = cipher_txt->data;
    encrypt.cipher_len = cipher_txt->data_len;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SM2_ENCRYPT, &encrypt);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    if (cipher_txt->data_len < encrypt.cipher_len) {
        ot_cipher_error("Need len %u, data len %u\n", encrypt.cipher_len, cipher_txt->data_len);
    } else {
        cipher_txt->data_len = encrypt.cipher_len;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_sm2_decrypt(const ot_cipher_sm2_private_key *sm2_key,
    const ot_cipher_common_data *cipher_txt, ot_cipher_common_data *plain_txt)
{
    td_s32 ret;
    sm2_decrypt_t decrypt;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&decrypt, sizeof(decrypt), 0, sizeof(decrypt));
    (td_void)memcpy_s(&decrypt.pri_key, sizeof(ot_cipher_sm2_private_key), sm2_key, sizeof(ot_cipher_sm2_private_key));
    decrypt.cipher_data = cipher_txt->data;
    decrypt.cipher_len = cipher_txt->data_len;
    decrypt.plain_data = plain_txt->data;
    decrypt.plain_len = plain_txt->data_len;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SM2_DECRYPT, &decrypt);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    if (plain_txt->data_len < decrypt.plain_len) {
        ot_cipher_error("Need len %u, data len %u\n", decrypt.plain_len, plain_txt->data_len);
    } else {
        plain_txt->data_len = decrypt.plain_len;
    }

    func_exit();
    return TD_SUCCESS;
}

td_s32 sys_keyslot_create(const ot_keyslot_attr *attr, td_handle *keyslot)
{
    td_s32 ret;
    keyslot_create_t create;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&create, sizeof(keyslot_create_t), 0, sizeof(keyslot_create_t));
    ret = memcpy_s(&create.attr, sizeof(ot_keyslot_attr), attr, sizeof(ot_keyslot_attr));
    if (ret != EOK) {
        print_func_errno(memset_s, ret);
        return OT_ERR_CIPHER_FAILED_SEC_FUNC;
    }

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_KEYSLOT_CREATE, &create);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }
    *keyslot = create.keyslot;

    ot_cipher_dbg("create keyslot %d\n", *keyslot);

    func_enter();

    func_exit();

    return ret;
}

td_s32 sys_keyslot_destroy(td_handle keyslot)
{
    td_s32 ret;
    keyslot_destroy_t destroy;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&destroy, sizeof(keyslot_destroy_t), 0, sizeof(keyslot_destroy_t));
    destroy.keyslot = keyslot;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_KEYSLOT_DESTROY, &destroy);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    func_exit();

    return ret;
}

td_s32 sys_cipher_attach(td_handle cipher, td_handle keyslot)
{
    td_s32 ret;
    symc_keyslot_t symc;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&symc, sizeof(symc_keyslot_t), 0, sizeof(symc_keyslot_t));
    symc.cipher = cipher;
    symc.keyslot = keyslot;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SYMC_ATTACH, &symc);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    func_exit();

    return ret;
}

td_s32 sys_cipher_detach(td_handle cipher, td_handle keyslot)
{
    td_s32 ret;
    symc_keyslot_t symc;
    sys_cipher_mgmt *mgmt = _get_sys_cipher_mgmt();

    func_enter();

    _sys_cipher_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&symc, sizeof(symc_keyslot_t), 0, sizeof(symc_keyslot_t));
    symc.cipher = cipher;
    symc.keyslot = keyslot;

    ret = cipher_ioctl(mgmt->cipher_fd, CRYPTO_CMD_SYMC_DETACH, &symc);
    if (ret != TD_SUCCESS) {
        print_func_errno(cipher_ioctl, ret);
        return ret;
    }

    func_exit();

    return ret;
}

/** @} */ /** <!-- ==== API Code end ==== */
