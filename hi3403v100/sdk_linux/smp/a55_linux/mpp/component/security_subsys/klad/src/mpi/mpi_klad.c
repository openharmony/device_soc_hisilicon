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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>
#include "ot_mpi_klad.h"
#include "ot_debug_klad.h"
#include "drv_ioctl_klad.h"
#include "securec.h"

#define KLAD_MUTEX_T                 pthread_mutex_t
#define klad_mutex_lock(x)           (td_void)pthread_mutex_lock(x)
#define klad_mutex_unlock(x)         (td_void)pthread_mutex_unlock(x)
#define klad_open(name, flags, mode) open(name, flags, mode)
#define klad_close(fd)               close(fd)
#define klad_ioctl(fd, request, var) ioctl(fd, request, var)

#define KLAD_INIT_MAX_NUM     0x7FFFFFFF

typedef struct {
    KLAD_MUTEX_T lock;
    td_s32 ref_count;
    td_s32 klad_fd;
} mpi_klad_mgmt;

static mpi_klad_mgmt g_mpi_klad_mgmt = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .ref_count = -1,
    .klad_fd = -1,
};

#define _mpi_klad_not_init_return(ref_count)                        \
    do {                                                            \
        if ((ref_count) < 0) {                                      \
            ot_klad_error("klad init counter %d\n", ref_count);     \
            return OT_ERR_KLAD_NOT_INIT;                            \
        }                                                           \
    } while (0)

static mpi_klad_mgmt *_mpi_klad_get_mgmt(td_void)
{
    return &g_mpi_klad_mgmt;
}

td_s32 ot_mpi_klad_init(td_void)
{
    td_s32 ret = TD_SUCCESS;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    klad_mutex_lock(&mgmt->lock);

    if (mgmt->ref_count >= 0) {
        if (mgmt->ref_count < KLAD_INIT_MAX_NUM) {
            mgmt->ref_count++;
        }
        klad_mutex_unlock(&mgmt->lock);
        return TD_SUCCESS;
    }

    mgmt->klad_fd = klad_open("/dev/"UMAP_DEVNAME_KLAD_BASE, O_RDWR, 0);
    if (mgmt->klad_fd < 0) {
        klad_mutex_unlock(&mgmt->lock);
        ot_klad_error("open /dev/%s failed. ret %x\n", UMAP_DEVNAME_KLAD_BASE, OT_ERR_KLAD_FAILED_INIT);
        return OT_ERR_KLAD_FAILED_INIT;
    }
    mgmt->ref_count = 0;

    klad_mutex_unlock(&mgmt->lock);

    ot_klad_exit();

    return ret;
}

td_s32 ot_mpi_klad_deinit(td_void)
{
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();
    ot_klad_enter();

    klad_mutex_lock(&mgmt->lock);

    if (mgmt->ref_count != 0) {
        if (mgmt->ref_count > 0) {
            mgmt->ref_count--;
        }
        klad_mutex_unlock(&mgmt->lock);
        return TD_SUCCESS;
    }

    if (klad_close(mgmt->klad_fd) < 0) {
        mgmt->klad_fd = -1;
        mgmt->ref_count = -1;
        klad_mutex_unlock(&mgmt->lock);
        ot_klad_error("close /dev/%s failed.\n", UMAP_DEVNAME_KLAD_BASE);
        return OT_ERR_KLAD_FAILED_INIT;
    }
    mgmt->klad_fd = -1;
    mgmt->ref_count = -1;

    klad_mutex_unlock(&mgmt->lock);

    ot_klad_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_klad_create(td_handle *klad)
{
    td_s32 ret;
    klad_ctl_handle handle;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    _mpi_klad_not_init_return(mgmt->ref_count);

    ot_klad_formula_fail_return(klad == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    (td_void)memset_s(&handle, sizeof(klad_ctl_handle), 0, sizeof(klad_ctl_handle));

    ret = klad_ioctl(mgmt->klad_fd, CMD_KLAD_CREATE_HANDLE, &handle);
    ot_klad_func_fail_return(klad_ioctl, ret != TD_SUCCESS, ret);

    *klad = handle.klad;

    ot_klad_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_klad_destroy(td_handle klad)
{
    td_s32 ret;
    klad_ctl_handle handle;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    _mpi_klad_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&handle, sizeof(klad_ctl_handle), 0, sizeof(klad_ctl_handle));
    handle.klad = klad;

    ret = klad_ioctl(mgmt->klad_fd, CMD_KLAD_DESTROY_HANDLE, &handle);
    ot_klad_func_fail_return(klad_ioctl, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_klad_attach(td_handle klad, td_handle target)
{
    td_s32 ret;
    klad_ctl_target ctl_target;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    _mpi_klad_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&ctl_target, sizeof(klad_ctl_target), 0, sizeof(klad_ctl_target));
    ctl_target.klad = klad;
    ctl_target.target = target;

    ret = klad_ioctl(mgmt->klad_fd, CMD_KLAD_ATTACH_TARGET, &ctl_target);
    ot_klad_func_fail_return(klad_ioctl, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_klad_detach(td_handle klad, td_handle target)
{
    td_s32 ret;
    klad_ctl_target ctl_target;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    _mpi_klad_not_init_return(mgmt->ref_count);

    (td_void)memset_s(&ctl_target, sizeof(klad_ctl_target), 0, sizeof(klad_ctl_target));
    ctl_target.klad = klad;
    ctl_target.target = target;

    ret = klad_ioctl(mgmt->klad_fd, CMD_KLAD_DETACH_TARGET, &ctl_target);
    ot_klad_func_fail_return(klad_ioctl, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_klad_set_attr(td_handle klad, const ot_klad_attr *attr)
{
    td_s32 ret;
    klad_ctl_attr ctl_attr;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    _mpi_klad_not_init_return(mgmt->ref_count);

    ot_klad_formula_fail_return(attr == TD_NULL, OT_ERR_KLAD_NULL_PTR);
    ot_klad_formula_fail_return(attr->klad_cfg.klad_type != OT_KLAD_TYPE_COMMON &&
        attr->klad_cfg.klad_type != OT_KLAD_TYPE_CLEARCW, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(attr->klad_cfg.rootkey_attr.key_sel < 0 ||
        attr->klad_cfg.rootkey_attr.key_sel > OT_KLAD_ROOTKEY_SEL_VENDOR, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(attr->klad_cfg.rootkey_attr.key_secure != OT_KLAD_ROOTKEY_SEC_REE &&
        attr->klad_cfg.rootkey_attr.key_secure != OT_KLAD_ROOTKEY_SEC_TEE, OT_ERR_KLAD_INVALID_PARAM);

    (td_void)memset_s(&ctl_attr, sizeof(klad_ctl_attr), 0, sizeof(klad_ctl_attr));
    ctl_attr.klad = klad;
    ret = memcpy_s(&(ctl_attr.attr), sizeof(ot_klad_attr), attr, sizeof(ot_klad_attr));
    ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);

    ret = klad_ioctl(mgmt->klad_fd, CMD_KLAD_SET_ATTR, &ctl_attr);
    ot_klad_func_fail_return(klad_ioctl, ret != TD_SUCCESS, ret);

    ot_klad_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_klad_get_attr(td_handle klad, ot_klad_attr *attr)
{
    td_s32 ret;
    klad_ctl_attr ctl_attr;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    _mpi_klad_not_init_return(mgmt->ref_count);

    ot_klad_formula_fail_return(attr == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    (td_void)memset_s(&ctl_attr, sizeof(klad_ctl_attr), 0, sizeof(klad_ctl_attr));
    ctl_attr.klad = klad;

    ret = klad_ioctl(mgmt->klad_fd, CMD_KLAD_GET_ATTR, &ctl_attr);
    ot_klad_func_fail_return(klad_ioctl, ret != TD_SUCCESS, ret);

    ret = memcpy_s(attr, sizeof(ot_klad_attr), &(ctl_attr.attr), sizeof(ot_klad_attr));
    ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);

    ot_klad_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_klad_set_session_key(td_handle klad, const ot_klad_session_key *key)
{
    td_s32 ret;
    klad_ctl_session_key session_key;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    _mpi_klad_not_init_return(mgmt->ref_count);

    ot_klad_formula_fail_return(key == TD_NULL, OT_ERR_KLAD_NULL_PTR);
    /* 16 byte:logic only supports 128bit session key */
    ot_klad_formula_fail_return(key->key_size != 16, OT_ERR_KLAD_INVALID_PARAM);

    (td_void)memset_s(&session_key, sizeof(klad_ctl_session_key), 0, sizeof(klad_ctl_session_key));
    session_key.klad = klad;
    ret = memcpy_s(&session_key.key, sizeof(ot_klad_session_key), key, sizeof(ot_klad_session_key));
    ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);

    ret = klad_ioctl(mgmt->klad_fd, CMD_KLAD_SESSION_KEY, &session_key);
    ot_klad_func_fail_return(klad_ioctl, ret != TD_SUCCESS, ret);

    /* clean key */
    (td_void)memset_s(&session_key.key, sizeof(ot_klad_session_key), 0, sizeof(ot_klad_session_key));

    ot_klad_exit();

    return TD_SUCCESS;
}

td_s32 ot_mpi_klad_set_content_key(td_handle klad, const ot_klad_content_key *key)
{
    td_s32 ret;
    klad_ctl_content_key content_key;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    _mpi_klad_not_init_return(mgmt->ref_count);

    ot_klad_formula_fail_return(key == TD_NULL, OT_ERR_KLAD_NULL_PTR);
    ot_klad_formula_fail_return(key->alg != OT_KLAD_ALG_TYPE_AES &&
        key->alg != OT_KLAD_ALG_TYPE_SM4, OT_ERR_KLAD_INVALID_PARAM);
    ot_klad_formula_fail_return(key->crypto_alg != OT_KLAD_CRYPTO_ALG_AES &&
        key->crypto_alg != OT_KLAD_CRYPTO_ALG_SM4, OT_ERR_KLAD_INVALID_PARAM);

    (td_void)memset_s(&content_key, sizeof(klad_ctl_content_key), 0, sizeof(klad_ctl_content_key));
    content_key.klad = klad;
    ret = memcpy_s(&content_key.key, sizeof(ot_klad_content_key), key, sizeof(ot_klad_content_key));
    ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);

    ret = klad_ioctl(mgmt->klad_fd, CMD_KLAD_CONTENT_KEY, &content_key);
    ot_klad_func_fail_goto(klad_ioctl, ret != TD_SUCCESS, ret, exit__);

exit__:

    /* clean key */
    (td_void)memset_s(&content_key.key, sizeof(ot_klad_content_key), 0, sizeof(ot_klad_content_key));

    ot_klad_exit();

    return ret;
}

td_s32 ot_mpi_klad_set_clear_key(td_handle klad, const ot_klad_clear_key *key)
{
    td_s32 ret;
    klad_ctl_clear_key clear_key;
    mpi_klad_mgmt *mgmt = _mpi_klad_get_mgmt();

    ot_klad_enter();

    _mpi_klad_not_init_return(mgmt->ref_count);

    ot_klad_formula_fail_return(key == TD_NULL, OT_ERR_KLAD_NULL_PTR);

    (td_void)memset_s(&clear_key, sizeof(klad_ctl_clear_key), 0, sizeof(klad_ctl_clear_key));
    clear_key.klad = klad;
    ret = memcpy_s(&clear_key.key, sizeof(ot_klad_clear_key), key, sizeof(ot_klad_clear_key));
    ot_klad_func_fail_return(memcpy_s, ret != EOK, OT_ERR_KLAD_FAILED_SEC_FUNC);

    ret = klad_ioctl(mgmt->klad_fd, CMD_KLAD_CLEAR_KEY, &clear_key);
    ot_klad_func_fail_goto(klad_ioctl, ret != TD_SUCCESS, ret, exit__);

exit__:
    /* clean key */
    (td_void)memset_s(&clear_key.key, sizeof(ot_klad_clear_key), 0, sizeof(ot_klad_clear_key));

    ot_klad_exit();

    return ret;
}
