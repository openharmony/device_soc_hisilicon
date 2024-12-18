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
 * Description: BARRIER
 *
 * Create: 2021-07-01
 */

#ifndef ARCH_BARRIER_H
#define ARCH_BARRIER_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef dsb
#undef dsb
#endif
#define dsb() __asm__ __volatile__("fence":::"memory")

#ifdef isb
#undef isb
#endif
#define isb() __asm__ __volatile__("fence":::"memory")

#ifdef nop
#undef nop
#endif
#define nop() __asm__ __volatile__("nop");

#ifdef wfi
#undef wfi
#endif
#define wfi() __asm__ __volatile__("wfi")

#define get_int_status()  !(read_csr(mstatus) & MSTATUS_MIE)
#define int_enter_lock()  clear_csr(mstatus, MSTATUS_MIE)
#define int_exit_lock()   set_csr(mstatus, MSTATUS_MIE)

#define int_disable_fault_exception()
#define int_enable_fault_exception()

#define get_temp_lr(temp_lr) __asm volatile("mv %0, x1" : "=r"(temp_lr))
#define get_temp_pc(temp_pc) __asm volatile("csrr %0, mepc" : "=r"(temp_pc))
#define get_temp_sp(temp_sp) __asm volatile("mv %0, sp" : "=r"(temp_sp))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif  // BARRIER_H