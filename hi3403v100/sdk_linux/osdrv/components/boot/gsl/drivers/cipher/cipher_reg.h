/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 */
#ifndef _CIPHER_REG_H_
#define _CIPHER_REG_H_

#define SPACC_REG_BASE   0x10100000

#define SPACC_INT_RAW_SYM_CLEAR_FINISH      (SPACC_REG_BASE + 0x0008)
#define SPACC_SYM_CHN_CLEAR_REQ             (SPACC_REG_BASE + 0x0060)

#define SPACC_INT_RAW_HASH_CLEAR_FINISH     (SPACC_REG_BASE + 0x0014)
#define SPACC_HASH_CHN_CLEAR_REQ            (SPACC_REG_BASE + 0x0068)

#define SPACC_SYM_CHN_LOCK                  (SPACC_REG_BASE + 0x0020)
#define SPACC_HASH_CHN_LOCK                 (SPACC_REG_BASE + 0x0040)

#define SPACC_MMU_IN_SYM_SEC_EADDR_H        (SPACC_REG_BASE + 0x2100)
#define SPACC_MMU_IN_SYM_SEC_EADDR_L        (SPACC_REG_BASE + 0x2104)
#define SPACC_MMU_IN_SYM_NOSEC_EADDR_L      (SPACC_REG_BASE + 0x2114)

#define SPACC_MMU_OUT_SYM_SEC_EADDR_H       (SPACC_REG_BASE + 0x2500)
#define SPACC_MMU_OUT_SYM_SEC_EADDR_L       (SPACC_REG_BASE + 0x2504)
#define SPACC_MMU_OUT_SYM_NOSEC_EADDR_L     (SPACC_REG_BASE + 0x2514)

#define in_sym_chn_ctrl_fun(id)                 (SPACC_REG_BASE + 0x4000 + (id) * 0x100)
#define in_sym_out_ctrl_fun(id)                 (SPACC_REG_BASE + 0x4004 + (id) * 0x100)
#define in_sym_chn_key_ctrl_fun(id)             (SPACC_REG_BASE + 0x4010 + (id) * 0x100)
#define in_sym_chn_node_start_addr_l_fun(id)    (SPACC_REG_BASE + 0x4024 + (id) * 0x100)
#define in_sym_chn_node_length_fun(id)          (SPACC_REG_BASE + 0x402c + (id) * 0x100)
#define in_sym_chn_node_wr_point_fun(id)        (SPACC_REG_BASE + 0x4030 + (id) * 0x100)

#define in_hash_chn_ctrl_fun(id)                (SPACC_REG_BASE + 0x5000 + (id) * 0x100)
#define in_hash_chn_key_ctrl_fun(id)            (SPACC_REG_BASE + 0x5010 + (id) * 0x100)
#define in_hash_chn_node_start_fun(id)          (SPACC_REG_BASE + 0x5024 + (id) * 0x100)
#define in_hash_chn_node_length_fun(id)         (SPACC_REG_BASE + 0x502c + (id) * 0x100)
#define in_hash_chn_node_wr_point_fun(id)       (SPACC_REG_BASE + 0x5030 + (id) * 0x100)
#define in_hash_chn_node_rd_point_fun(id)       (SPACC_REG_BASE + 0x5034 + (id) * 0x100)

#define HASH_CHANN_RAW_INT                  (SPACC_REG_BASE + 0x8600)

#define chn_hash_state_val(id)            (SPACC_REG_BASE + 0xa000 + (id) * 0x80)
#define chn_hash_state_val_addr(id)       (SPACC_REG_BASE + 0xa004 + (id) * 0x80)

#define OUT_SYM_CHAN_RAW_LAST_NODE_INT      (SPACC_REG_BASE + 0xc000)

#define out_sym_chn_node_start_addr_h(id)   (SPACC_REG_BASE + 0xcf20 + (id) * 0x100)
#define out_sym_chn_node_start_addr_l(id)   (SPACC_REG_BASE + 0xcf24 + (id) * 0x100)
#define out_sym_chn_node_length(id)         (SPACC_REG_BASE + 0xcf28 + (id) * 0x100)
#define out_sym_chn_node_wr_point(id)       (SPACC_REG_BASE + 0xcf30 + (id) * 0x100)
#define out_sym_chn_node_rd_point(id)       (SPACC_REG_BASE + 0xcf34 + (id) * 0x100)
#define out_sym_chn_node_ctrl(id)           (SPACC_REG_BASE + 0xcf40 + (id) * 0x100)

#endif

