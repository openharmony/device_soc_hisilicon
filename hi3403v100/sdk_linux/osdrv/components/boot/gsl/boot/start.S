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
#include <arch/aarch64/asm_macros.S>
#include <platform.h>
#include <system.h>

.equ gsl_code_area_len_offset, 0x08
.equ gsl_code_offset, 0x40
.equ gsl_code_area_non_code_size, 0x440

.globl  runtime_exceptions
.globl  sync_exception_sp_el0
.globl  irq_sp_el0
.globl  fiq_sp_el0
.globl  serror_sp_el0

.globl  sync_exception_sp_elx
.globl  irq_sp_elx
.globl  fiq_sp_elx
.globl  serror_sp_elx

.globl  sync_exception_aarch64

.globl _start
_start:
	b reset
.align 3 /* data must be 8 bytes aligned*/

reset:
/* Initialize the Registers to avoid X startus */
	ldr x0, =0
	ldr x1, =0
	ldr x2, =0
	ldr x3, =0
	ldr x4, =0
	ldr x5, =0
	ldr x6, =0
	ldr x7, =0
	ldr x8, =0
	ldr x9, =0
	ldr x10, =0
	ldr x11, =0
	ldr x12, =0
	ldr x13, =0
	ldr x14, =0
	ldr x15, =0
	ldr x16, =0
	ldr x17, =0
	ldr x18, =0
	ldr x19, =0
	ldr x20, =0
	ldr x21, =0
	ldr x22, =0
	ldr x23, =0
	ldr x24, =0
	ldr x25, =0
	ldr x26, =0
	ldr x27, =0
	ldr x28, =0
	ldr x29, =0
	ldr x30, =0

	/*
	 * Could be EL3/EL2/EL1, Initial State:
	 * Little Endian, MMU Disabled, i/dCache Disabled
	 */
	adr x0, runtime_exceptions
	
	msr vbar_el3, x0
	mrs x0, scr_el3
	orr x0, x0, #0xf            /* SCR_EL3.NS|IRQ|FIQ|EA */
	msr scr_el3, x0
	msr cptr_el3, xzr            /* Enable FP/SIMD */

	/* set stack for C code  */
	ldr x0, =(STACK_ADDR)
	bic sp, x0, #0xf    /* 16-byte alignment for ABI compliance */
	
	/* clear_bss */
	ldr x0, =__bss_start
	mov x1, #0
	ldr x2, =__bss_end
	subs x2, x2, x0
	/* void * clear_data(void * s,int c,size_t count) */
	bl clear_data
	bl main_entry

	/* clear stack heap */
	ldr x0, =__bss_end
	ldr x2, =STACK_ADDR
	sub x2, x2, x0
	mov x1, #0
	bl clear_data

	/* keep bootloader address using x23, which will be used by REE-CPU later */
	ldr x1, _jump_addr
	ldr x23, [x1]

	/* check whether tee is enable */
	ldr     x0, =OTP_USER_LOCKABLE0
	ldr     w1, [x0]
	and     w1, w1, #0xff
	/* 0x42: tee is not enable */
	cmp     w1, #0x42
	beq non_tee_branch

tee_branch:
	/* keep the lengh of GSL code area using w20 */
	ldr x0, =GSL_KEY_AREA_ADDR
	mov x1, #gsl_code_area_len_offset
	add x1, x0, x1
	mov x20, #0
	ldr w20, [x1]

	/* keep the starting address of GSL code using x21 */
	ldr x0, =GSL_CODE_AREA_ADDR
	mov x1, #gsl_code_offset
	add x21, x0, x1

	/* keep the ending address of GSL code using x22 */
	mov x0, #gsl_code_area_non_code_size
	sub x0, x20, x0
	add x22, x21, x0

	/* make sercure bootram from bootram begin address to GSL code end address */
	ldr w0, =SRAM_BASE
	/* get secure bootram size in KB */
	sub w0, w22, w0
	lsr w0, w0, #10
	/* lower 10 bits is remainder of 1KB*/
	and w1, w22, #0x3ff
	cbz w1, set_secure_bootram
	/* round up */
	add w0, w0, #1
set_secure_bootram:
	ldr x2, =SEC_BOOTRAM_SEC_CFG
	ldr w3, [x2]
	and w3, w3, #0xffffff00
	orr w3, w3, w0
	str w3, [x2]
	
	/* copy the code to be executed later */
	ldr x0, =tee_copy_begin
	ldr x1, =GSL_LATER_CODE_ADDR
	ldr x2, =tee_copy_end
	sub x2, x2, x0  /* x2 <- size of the code */
	bl copy_code_to_sram

	/* Set EL3 return address and switch TEE-CPU to REE-CPU*/
	ldr x0, =GSL_LATER_CODE_ADDR
	b switch_from_el3_to_el1

non_tee_branch:
	/* copy the code to be executed later */
	ldr x0, =non_tee_copy_begin
	ldr x1, =GSL_LATER_CODE_ADDR
	ldr x2, =non_tee_copy_end
	sub x2, x2, x0  /* x2 <- size of the code */
	bl copy_code_to_sram
	/* jump to later code */
	ldr x21, =SRAM_BASE
	ldr x22, =GSL_LATER_CODE_ADDR
	br x22

.align 3 /* data must be 8 bytes aligned, the data will be copy to bootram */
non_tee_copy_begin:
	/* clear SRAM */
	mov x0, x21
	mov x1, x22
	mov w2, #0x0
	mov x3, #0x4
	do_code_clear_loop:
	str w2, [x0]
	add x0, x0, x3
	cmp x0, x1
	blt do_code_clear_loop
	b set_scs_finish

.align 3
tee_copy_begin:
set_scs_finish:
	/* get scs_finish register */
	ldr	x3, __REG_BASE_CA_MISC
	ldr	w1, [x3, #SCS_CTRL]

	/* set scs_finish register */
	and     w1, w1, #0xfffffff0
	mov     w2, #0x5
	orr     w1, w1, w2
	str     w1, [x3, #SCS_CTRL]

excute_bootloader:
	br x23

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

.align 3 /* data must be 8 bytes aligned, the data will be copy to bootram */
__REG_BASE_CA_MISC:
        .quad REG_BASE_CA_MISC

non_tee_copy_end:
tee_copy_end:

.align 3
_jump_addr:
	.quad jump_addr

/*
 * void copy_code_to_sram(x0:src, x1:dst, x2:size);
 */
copy_code_to_sram:
	add x2, x0, x2
	copy_code_to_sram_loop:
	ldp x10, x11, [x0], #16 /* copy from source address [x0] */
	stp x10, x11, [x1], #16 /* copy to   target address [x1] */
	cmp x0, x2
	ble copy_code_to_sram_loop
	ret

_do_error:
	/* reset */
	ldr x1, =REG_BASE_SCTL
	ldr w0, =1
	str w0, [x1, #REG_SC_SYSRES]

.globl _secure_failure_process
_secure_failure_process:
	ldr     x0, =OTP_USER_LOCKABLE0
	ldr     w1, [x0]
	and     w1, w1, #0xff
	/* 0x42: tee is not enable */
	cmp     w1, #0x42 
	beq     redundant_check
	/* if tee is enable, switch TEE-CPU to REE-CPU */
	ldr     x0, =copy_failure_code_to_sram
	b       switch_from_el3_to_el1

redundant_check:
	ldr     x0, =OTP_USER_LOCKABLE0
	ldr     w1, [x0]
	and     w1, w1, #0xff
	cmp     w1, #0x42 
	b.ne	_do_error

copy_failure_code_to_sram:
	ldr x0, =failure_code_begin
	ldr x1, =failure_code_end
	sub x21, x1, x0  /* code size */
	ldr x1, =SRAM_BASE
	mov x2, x21
	bl copy_code_to_sram
	mov x0, x21
	ldr lr, =SRAM_BASE
	ret

.align 3 /* data must be 8 bytes aligned, the data will be copy to bootram */
failure_code_begin:
clear_bootram:
	ldr x1, _SRAM_BASE
	add x0, x0, x1
	mov x1, #0
	ldr x2, _SRAM_END
	subs x2, x2, x0
	/* void * clear_data(void * s,int c,size_t count) */
	bl clear_data
	
set_addr_remap_clear:
	/* get scs_finish register */
	ldr	x3, _REG_BASE_CA_MISC
	ldr	w1, [x3, #SCS_CTRL]

	/* set scs_finish register */
	and     w1, w1, 0xfffffff0
	mov     w2, #0x5
	orr     w1, w1, w2
	str     w1, [x3, #SCS_CTRL]
	
	wfi
	wfi
	wfi
	wfi
	wfi

/*
 * void clear_data(x0, x1, x2);
 * x0: address of the area to fill
 * x1: fill byte
 * x2: number of bytes to fill
 */
clear_data:
	add x2, x0, x2
	mov x3, #0x8
	clear_data_loop:
	str x1, [x0]
	add x0, x0, x3
	cmp x0, x2
	blt clear_data_loop
	ret

.align 3 /* data must be 8 bytes aligned, the data will be copy to bootram */	
_REG_BASE_SCTL_R:
	.quad REG_BASE_SCTL
_REG_BASE_CA_MISC:
	.quad REG_BASE_CA_MISC
_SRAM_BASE:
	.quad SRAM_BASE
_SRAM_END:
	.quad SRAM_END
failure_code_end:

/*
 * void switch_from_el3_to_el1(x0);
 * x0: EL3 return address
 */
switch_from_el3_to_el1:
	/* SCTLR_EL1 with an unknown reset value must be configured */
	ldr x1, =(SCTLR_EL1_UCI_DIS | SCTLR_EL1_EE_LE | SCTLR_EL1_WXN_DIS |\
			 SCTLR_EL1_NTWE_DIS | SCTLR_EL1_NTWI_DIS | SCTLR_EL1_UCT_DIS |\
			 SCTLR_EL1_DZE_DIS | SCTLR_EL1_ICACHE_DIS | SCTLR_EL1_UMA_DIS |\
			 SCTLR_EL1_SED_EN | SCTLR_EL1_ITD_EN | SCTLR_EL1_CP15BEN_DIS |\
			 SCTLR_EL1_SA0_DIS | SCTLR_EL1_SA_DIS | SCTLR_EL1_DCACHE_DIS |\
			 SCTLR_EL1_ALIGN_DIS | SCTLR_EL1_MMU_DIS | SCTLR_EL1_RES1)
	msr sctlr_el1, x1

	/* Initialize HCR_EL2 */
	ldr x1, =HCR_EL2_RW_AARCH64
	msr hcr_el2, x1

	/* AArch64 64bit | SMD ENABLE | Non-secure */
	ldr x1, =(SCR_EL3_RW_AARCH64 & (~SCR_EL3_SMD_DIS) | SCR_EL3_NS_EN)
	msr scr_el3, x1

	/* Return to the EL1_SP1 mode from EL3 */
	ldr x1, =(SPSR_EL_DEBUG_MASK | SPSR_EL_SERR_MASK |\
			  SPSR_EL_IRQ_MASK | SPSR_EL_FIQ_MASK |\
			  SPSR_EL_M_AARCH64 | SPSR_EL_M_EL1H)
	msr spsr_el3, x1

	/* set exception return address */
	msr elr_el3, x0
	eret

/***********************************************************************/
/* Jumping from U-Boot to GSL Through the smc Service  */
/***********************************************************************/
vector_base runtime_exceptions
/* ---------------------------------------------------------------------
 * Current EL with SP_EL0 : 0x0 - 0x200
 * ---------------------------------------------------------------------
 */
vector_entry sync_exception_sp_el0
end_vector_entry sync_exception_sp_el0
vector_entry irq_sp_el0
end_vector_entry irq_sp_el0
vector_entry fiq_sp_el0
end_vector_entry fiq_sp_el0
vector_entry serror_sp_el0
end_vector_entry serror_sp_el0
	
/* ---------------------------------------------------------------------
 * Current EL with SP_ELx: 0x200 - 0x400
 * ---------------------------------------------------------------------
 */
vector_entry sync_exception_sp_elx
end_vector_entry sync_exception_sp_elx
vector_entry irq_sp_elx
end_vector_entry irq_sp_elx
vector_entry fiq_sp_elx
end_vector_entry fiq_sp_elx
vector_entry serror_sp_elx
end_vector_entry serror_sp_elx

/* ---------------------------------------------------------------------
 * Lower EL using AArch64 : 0x400 - 0x600
 * ---------------------------------------------------------------------
 */
vector_entry sync_exception_aarch64
/*
 * This exception vector will be the entry point for SMCs and traps
 * that are unhandled at lower ELs most commonly. SP_EL3 should point
 * to a valid cpu context where the general purpose and system register
 * state can be saved.
 */
	bl secure_os_entry
end_vector_entry sync_exception_aarch64

