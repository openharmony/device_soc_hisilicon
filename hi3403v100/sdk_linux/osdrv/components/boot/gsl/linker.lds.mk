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

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)
SECTIONS
{
	. = TEXT_BASE;
	. = ALIGN(8);
	.text	:
	{
	  __text_start = .;
	  boot/start.o  (.text)
	  drivers/common/malloc.o   (.text)
	  drivers/common/lib.o    (.text)
	  drivers/common/timer.o    (.text)
	  drivers/otp/otp.o    (.text)
	  drivers/ddr_init/ddr_training_custom.o    (.text)
	  drivers/ddr_init/ddr_training_ctl.o	(.text)
	  drivers/ddr_init/init_regs.o    (.text)
	  drivers/ddr_init/ddr_training_impl.o    (.text)
	  drivers/ddr_init/ddr_training_boot.o    (.text)
	  drivers/ddr_init/ddr_training_console.o    (.text)
	  drivers/ddr_init/ddr_ddrt_training.o    (.text)
	  drivers/ddr_init/ddr_gate_training.o    (.text)
	  drivers/ddr_init/ddr_lpca_training.o    (.text)
	  drivers/ddr_init/ddr_mpr_training.o    (.text)
	  drivers/ddr_init/ddr_dcc_training.o    (.text)
	  drivers/ddr_init/ddr_wl_training.o    (.text)
	  drivers/klad/ot_klad.o    (.text)
	  drivers/klad/klad.o    (.text)
	  drivers/klad/rkp_com.o    (.text)
	  drivers/pke/drv_pke_v200.o    (.text)
	  drivers/securelib/memmove_s.o    (.text)
	  drivers/securelib/memcpy_s.o    (.text)
	  drivers/securelib/memset_s.o    (.text)
	  __init_end = .;
	  ASSERT(((__init_end - __text_start) < 0x5f00), "sections too big!");
	  . = 0x60c0;
	  . = ALIGN(8);
	  boot/main.o   (.text)
	  boot/checkup.o  (.text)
	  drivers/cipher/cipher.o    (.text)
	  drivers/uart/uart.o    (.text)
	  drivers/uart/serial_pl011.o    (.text)
	  drivers/emmc/emmc.o    (.text)
	  drivers/sdio/sdhci.o    (.text)
	  drivers/tzasc/tzasc.o    (.text)
	  drivers/klad/hkl.o    (.text)
	  drivers/pke/drv_pke.o    (.text)
	  drivers/pke/rsa_pad_pss.o    (.text)
	}
	. = ALIGN(8);
	.data : { *(.data) }
	. = ALIGN(8);

	.rodata :
	{
	  *(.rodata)
	  *(.version)
	}
	. = ALIGN(8);
	. = RAM_BASE;
	__bss_start = .;
	.bss : { *(.bss) }
	. = ALIGN(8);
	__bss_end = .;

	_end = .;
}
