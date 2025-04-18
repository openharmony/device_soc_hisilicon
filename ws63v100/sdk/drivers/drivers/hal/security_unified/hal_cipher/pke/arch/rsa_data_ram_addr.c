/**
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
 * Description: define the data store address of RSA in the PKE DRAM.
 *
 * Create: 2022-08-19
*/

#include "rom_lib.h"

/* modulus storage space */
const td_u32 rsa_addr_n  = 0;
/* storage space for base number ofr the modular exponentiation */
const td_u32 rsa_addr_a  = 16;
/* storage sapce for montgomery parameter */
const td_u32 rsa_addr_rr  = 32;
/* storage sapce for constant value 1 */
const td_u32 rsa_addr_const_1  = 48;
/* storage sapce of result */
const td_u32 rsa_addr_s  = 64;
/* storage sapce for intermediate variable of CRT mode */
const td_u32 rsa_addr_m2  = 40;
const td_u32 rsa_addr_q_inv  = 56;
/* storage sapce for common intermediate variable */
const td_u32 rsa_addr_tp  = 80;
/* storage sapce for input data */
const td_u32 rsa_addr_t0  = 96;
const td_u32 rsa_addr_t1  = 112;