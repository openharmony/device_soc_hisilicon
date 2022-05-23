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

#ifndef __HI_REG_IO_H__
#define __HI_REG_IO_H__

/* Define the union U_ioshare_reg0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int ioshare_reg : 32; /* [31..0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_ioshare_reg;

// ==============================================================================
/* Define the global struct */
typedef struct {
    volatile u_ioshare_reg ioshare_reg0;   /* 0x0 */
    volatile u_ioshare_reg ioshare_reg1;   /* 0x4 */
    volatile u_ioshare_reg ioshare_reg2;   /* 0x8 */
    volatile u_ioshare_reg ioshare_reg3;   /* 0xc */
    volatile u_ioshare_reg ioshare_reg4;   /* 0x10 */
    volatile u_ioshare_reg ioshare_reg5;   /* 0x14 */
    volatile u_ioshare_reg ioshare_reg6;   /* 0x18 */
    volatile u_ioshare_reg ioshare_reg7;   /* 0x1c */
    volatile u_ioshare_reg ioshare_reg8;   /* 0x20 */
    volatile u_ioshare_reg ioshare_reg9;   /* 0x24 */
    volatile u_ioshare_reg ioshare_reg10;  /* 0x28 */
    volatile u_ioshare_reg ioshare_reg11;  /* 0x2c */
    volatile u_ioshare_reg ioshare_reg12;  /* 0x30 */
    volatile u_ioshare_reg ioshare_reg13;  /* 0x34 */
    volatile u_ioshare_reg ioshare_reg14;  /* 0x38 */
    volatile u_ioshare_reg ioshare_reg15;  /* 0x3c */
    volatile u_ioshare_reg ioshare_reg16;  /* 0x40 */
    volatile u_ioshare_reg ioshare_reg17;  /* 0x44 */
    volatile u_ioshare_reg ioshare_reg18;  /* 0x48 */
    volatile u_ioshare_reg ioshare_reg19;  /* 0x4c */
    volatile u_ioshare_reg ioshare_reg20;  /* 0x50 */
    volatile u_ioshare_reg ioshare_reg21;  /* 0x54 */
    volatile u_ioshare_reg ioshare_reg22;  /* 0x58 */
    volatile u_ioshare_reg ioshare_reg23;  /* 0x5c */
    volatile u_ioshare_reg ioshare_reg24;  /* 0x60 */
    volatile u_ioshare_reg ioshare_reg25;  /* 0x64 */
    volatile u_ioshare_reg ioshare_reg26;  /* 0x68 */
    volatile u_ioshare_reg ioshare_reg27;  /* 0x6c */
    volatile u_ioshare_reg ioshare_reg28;  /* 0x70 */
    volatile u_ioshare_reg ioshare_reg29;  /* 0x74 */
    volatile u_ioshare_reg ioshare_reg30;  /* 0x78 */
    volatile u_ioshare_reg ioshare_reg31;  /* 0x7c */
    volatile u_ioshare_reg ioshare_reg32;  /* 0x80 */
    volatile u_ioshare_reg ioshare_reg33;  /* 0x84 */
    volatile u_ioshare_reg ioshare_reg34;  /* 0x88 */
    volatile u_ioshare_reg ioshare_reg35;  /* 0x8c */
    volatile u_ioshare_reg ioshare_reg36;  /* 0x90 */
    volatile u_ioshare_reg ioshare_reg37;  /* 0x94 */
    volatile u_ioshare_reg ioshare_reg38;  /* 0x98 */
    volatile u_ioshare_reg ioshare_reg39;  /* 0x9c */
    volatile u_ioshare_reg ioshare_reg40;  /* 0xa0 */
    volatile u_ioshare_reg ioshare_reg41;  /* 0xa4 */
    volatile u_ioshare_reg ioshare_reg42;  /* 0xa8 */
    volatile u_ioshare_reg ioshare_reg43;  /* 0xac */
    volatile u_ioshare_reg ioshare_reg44;  /* 0xb0 */
    volatile u_ioshare_reg ioshare_reg45;  /* 0xb4 */
    volatile u_ioshare_reg ioshare_reg46;  /* 0xb8 */
    volatile u_ioshare_reg ioshare_reg47;  /* 0xbc */
    volatile u_ioshare_reg ioshare_reg48;  /* 0xc0 */
    volatile u_ioshare_reg ioshare_reg49;  /* 0xc4 */
    volatile u_ioshare_reg ioshare_reg50;  /* 0xc8 */
    volatile u_ioshare_reg ioshare_reg51;  /* 0xcc */
    volatile u_ioshare_reg ioshare_reg52;  /* 0xd0 */
    volatile u_ioshare_reg ioshare_reg53;  /* 0xd4 */
    volatile u_ioshare_reg ioshare_reg54;  /* 0xd8 */
    volatile u_ioshare_reg ioshare_reg55;  /* 0xdc */
    volatile u_ioshare_reg ioshare_reg56;  /* 0xe0 */
    volatile u_ioshare_reg ioshare_reg57;  /* 0xe4 */
    volatile u_ioshare_reg ioshare_reg58;  /* 0xe8 */
    volatile u_ioshare_reg ioshare_reg59;  /* 0xec */
    volatile u_ioshare_reg ioshare_reg60;  /* 0xf0 */
    volatile u_ioshare_reg ioshare_reg61;  /* 0xf4 */
    volatile u_ioshare_reg ioshare_reg62;  /* 0xf8 */
    volatile u_ioshare_reg ioshare_reg63;  /* 0xfc */
    volatile u_ioshare_reg ioshare_reg64;  /* 0x100 */
    volatile u_ioshare_reg ioshare_reg65;  /* 0x104 */
    volatile u_ioshare_reg ioshare_reg66;  /* 0x108 */
    volatile u_ioshare_reg ioshare_reg67;  /* 0x10c */
    volatile u_ioshare_reg ioshare_reg68;  /* 0x110 */
    volatile u_ioshare_reg ioshare_reg69;  /* 0x114 */
    volatile u_ioshare_reg ioshare_reg70;  /* 0x118 */
    volatile u_ioshare_reg ioshare_reg71;  /* 0x11c */
    volatile u_ioshare_reg ioshare_reg72;  /* 0x120 */
    volatile u_ioshare_reg ioshare_reg73;  /* 0x124 */
    volatile u_ioshare_reg ioshare_reg74;  /* 0x128 */
    volatile u_ioshare_reg ioshare_reg75;  /* 0x12c */
    volatile u_ioshare_reg ioshare_reg76;  /* 0x130 */
    volatile u_ioshare_reg ioshare_reg77;  /* 0x134 */
    volatile u_ioshare_reg ioshare_reg78;  /* 0x138 */
    volatile u_ioshare_reg ioshare_reg79;  /* 0x13c */
    volatile u_ioshare_reg ioshare_reg80;  /* 0x140 */
    volatile u_ioshare_reg ioshare_reg81;  /* 0x144 */
    volatile u_ioshare_reg ioshare_reg82;  /* 0x148 */
    volatile u_ioshare_reg ioshare_reg83;  /* 0x14c */
    volatile u_ioshare_reg ioshare_reg84;  /* 0x150 */
    volatile u_ioshare_reg ioshare_reg85;  /* 0x154 */
    volatile u_ioshare_reg ioshare_reg86;  /* 0x158 */
    volatile u_ioshare_reg ioshare_reg87;  /* 0x15c */
    volatile u_ioshare_reg ioshare_reg88;  /* 0x160 */
    volatile u_ioshare_reg ioshare_reg89;  /* 0x164 */
    volatile u_ioshare_reg ioshare_reg90;  /* 0x168 */
    volatile u_ioshare_reg ioshare_reg91;  /* 0x16c */
    volatile u_ioshare_reg ioshare_reg92;  /* 0x170 */
    volatile u_ioshare_reg ioshare_reg93;  /* 0x174 */
    volatile u_ioshare_reg ioshare_reg94;  /* 0x178 */
    volatile u_ioshare_reg ioshare_reg95;  /* 0x17c */
    volatile u_ioshare_reg ioshare_reg96;  /* 0x180 */
    volatile u_ioshare_reg ioshare_reg97;  /* 0x184 */
    volatile u_ioshare_reg ioshare_reg98;  /* 0x188 */
    volatile u_ioshare_reg ioshare_reg99;  /* 0x18c */
    volatile u_ioshare_reg ioshare_reg100; /* 0x190 */
    volatile u_ioshare_reg ioshare_reg101; /* 0x194 */
    volatile u_ioshare_reg ioshare_reg102; /* 0x198 */
    volatile u_ioshare_reg ioshare_reg103; /* 0x19c */
    volatile u_ioshare_reg ioshare_reg104; /* 0x1a0 */
    volatile u_ioshare_reg ioshare_reg105; /* 0x1a4 */
    volatile u_ioshare_reg ioshare_reg106; /* 0x1a8 */
    volatile u_ioshare_reg ioshare_reg107; /* 0x1ac */
    volatile u_ioshare_reg ioshare_reg108; /* 0x1b0 */
    volatile u_ioshare_reg ioshare_reg109; /* 0x1b4 */
    volatile u_ioshare_reg ioshare_reg110; /* 0x1b8 */
    volatile u_ioshare_reg ioshare_reg111; /* 0x1bc */
    volatile u_ioshare_reg ioshare_reg112; /* 0x1c0 */
    volatile u_ioshare_reg ioshare_reg113; /* 0x1c4 */
    volatile u_ioshare_reg ioshare_reg114; /* 0x1c8 */
    volatile u_ioshare_reg ioshare_reg115; /* 0x1cc */
    volatile u_ioshare_reg ioshare_reg116; /* 0x1d0 */
    volatile u_ioshare_reg ioshare_reg117; /* 0x1d4 */
    volatile u_ioshare_reg ioshare_reg118; /* 0x1d8 */
    volatile u_ioshare_reg ioshare_reg119; /* 0x1dc */
    volatile u_ioshare_reg ioshare_reg120; /* 0x1e0 */
    volatile u_ioshare_reg ioshare_reg121; /* 0x1e4 */
    volatile u_ioshare_reg ioshare_reg122; /* 0x1e8 */
    volatile u_ioshare_reg ioshare_reg123; /* 0x1ec */
    volatile u_ioshare_reg ioshare_reg124; /* 0x1f0 */
    volatile u_ioshare_reg ioshare_reg125; /* 0x1f4 */
    volatile u_ioshare_reg ioshare_reg126; /* 0x1f8 */
    volatile u_ioshare_reg ioshare_reg127; /* 0x1fc */
    volatile u_ioshare_reg ioshare_reg128; /* 0x200 */
    volatile u_ioshare_reg ioshare_reg129; /* 0x204 */
    volatile u_ioshare_reg ioshare_reg130; /* 0x208 */
    volatile u_ioshare_reg ioshare_reg131; /* 0x20c */
    volatile u_ioshare_reg ioshare_reg132; /* 0x210 */
} s_io_regs_type;

#endif /* __HI_REG_IO_H__ */

