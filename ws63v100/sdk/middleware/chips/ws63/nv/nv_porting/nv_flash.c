/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: NV on different flash
 */

#include "nv_porting.h"
#include "sfc.h"
#include "osal_inner.h"
#include "tcxo.h"

errcode_t kv_flash_read(const uint32_t flash_offset, const uint32_t read_size, uint8_t *read_buffer)
{
    errcode_t ret = uapi_sfc_reg_read(flash_offset, read_buffer, read_size);
    if (ret != ERRCODE_SUCC) {
        return ERRCODE_FAIL;
    }
    return ERRCODE_SUCC;
}

errcode_t kv_flash_write(const uint32_t flash_offset, uint32_t write_size, const uint8_t *write_data, bool do_erase)
{
    errcode_t ret = ERRCODE_FAIL;
    if (do_erase == true) {
        ret = kv_flash_erase(flash_offset, write_size);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
    }

    uint8_t *cmp_data = kv_malloc(write_size);
    if (cmp_data == NULL) {
        return ERRCODE_MALLOC;
    }
    ret = uapi_sfc_reg_write(flash_offset, (uint8_t *)write_data, write_size);
    if (ret != EOK) {
        goto write_failed;
    }
    /* 回读比较 */
    ret = uapi_sfc_reg_read(flash_offset, cmp_data, write_size);
    if (ret != EOK) {
        goto write_failed;
    }
    ret = (errcode_t)memcmp(cmp_data, write_data, write_size);
    if (ret != EOK) {
        goto write_failed;
    }
write_failed:
    kv_free(cmp_data);
    return ret;
}

errcode_t kv_flash_erase(const uint32_t flash_addr, uint32_t size)
{
    errcode_t ret = ERRCODE_FAIL;
    ret = uapi_sfc_reg_erase(flash_addr, size);
    return ret;
}