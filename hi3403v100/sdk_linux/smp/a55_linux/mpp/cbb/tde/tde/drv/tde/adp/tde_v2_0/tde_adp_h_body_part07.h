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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART07_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART07_H_

    u_des_dither_ctrl dst_dither_ctrl;       /* offset 0x2d0 + 0x08 */
    u_des_dswm des_dswm;                     /* 0x2dc */
    td_u32 reserved_6[8];                    /* 8:0x2e0~0x2fc */
    u_cbmctrl cbmctrl;                       /* 0x300 */
    u_cbmbkg cbmbkg;                         /* 0x304 */
    u_cbmcolorize cbmcolorize;               /* 0x308 */
    u_cbmalupara cbmalupara;                 /* 0x30c */
    u_cbmkeypara cbmkeypara;                 /* 0x310 */
    u_cbmkeymin cbmkeymin;                   /* 0x314 */
    u_cbmkeymax cbmkeymax;                   /* 0x318 */
    u_cbmkeymask cbmkeymask;                 /* 0x31c */
    u_src1_cbmpara src1_cbmpara;             /* 0x320 */
    td_u32 src1_cbmstpos;                    /* 0x324 */
    u_src2_cbmpara src2_cbmpara;             /* 0x328 */
    td_u32 src2_cbmstpos;                    /* 0x32c */
    td_u32 gdc3_cbmpara;                     /* 0x330 */
    td_u32 gdc3_cbmstpos;                    /* 0x334 */
    td_u32 gdc4_cbmpara;                     /* 0x338 */
    td_u32 gdc4_cbmstpos;                    /* 0x33c */
    td_u32 gdc5_cbmpara;                     /* 0x340 */
    td_u32 gdc5_cbmstpos;                    /* 0x344 */
    td_u32 gdc6_cbmpara;                     /* 0x348 */
    td_u32 gdc6_cbmstpos;                    /* 0x34c */
    td_u32 gdc7_cbmpara;                     /* 0x350 */
    td_u32 gdc7_cbmstpos;                    /* 0x354 */
    td_u32 reserved_7[38];                   /* 38:0x358~0x3ec */
    u_src2_rtt_ctrl src2_rtt_ctrl;           /* 0x3f0 */
    td_u32 reserved_8[35];                   /* 35:0x3f4~0x47c */
    td_u32 tde_src1_zme_lhaddr_high;         /* 0x480 */
    td_u32 tde_src1_zme_lhaddr_low;          /* 0x484 */
    td_u32 tde_src1_zme_lvaddr_high;         /* 0x488 */
    td_u32 tde_src1_zme_lvaddr_low;          /* 0x48C */
    td_u32 tde_src1_zme_chaddr_high;         /* 0x490 */
    td_u32 tde_src1_zme_chaddr_low;          /* 0x494 */
    td_u32 tde_src1_zme_cvaddr_high;         /* 0x498 */
    td_u32 tde_src1_zme_cvaddr_low;          /* 0x49c */
    td_u32 tde_src2_zme_lhaddr_high;         /* 0x4A0 */
    td_u32 tde_src2_zme_lhaddr_low;          /* 0x4A4 */
    td_u32 tde_src2_zme_lvaddr_high;         /* 0x4A8 */
    td_u32 tde_src2_zme_lvaddr_low;          /* 0x4AC */
    td_u32 tde_src2_zme_chaddr_high;         /* 0x4B0 */
    td_u32 tde_src2_zme_chaddr_low;          /* 0x4B4 */
    td_u32 tde_src2_zme_cvaddr_high;         /* 0x4B8 */
    td_u32 tde_src2_zme_cvaddr_low;          /* 0x4Bc */
    td_u32 tde_clut_addr_high;               /* 0x4C0 */
    td_u32 tde_clut_addr_low;                /* 0x4C4 */
    td_u32 tde_axiid;                        /* 0x4C8 */
    td_u32 tde_nodeid;                       /* 0x4CC */
    u_tde_intmask tde_intmask;               /* 0x4D0 */
    td_u32 reserved_9[9];                    /* 9:0x3D4~0x4F4 */
    u_tde_pnext_hi tde_pnext_hi;             /* 0x4f8 */
    u_tde_pnext_low tde_pnext_low;           /* 0x4fc */
    u_tde_start tde_start;                   /* 0x500 */
    u_tde_intstate tde_intstate;             /* 0x504 */
    u_tde_intclr tde_intclr_;               /* 0x508 */
    u_tde_rawint tde_rawint;                 /* 0x50c */
    u_tde_pfcnt tde_pfcnt;                   /* 0x510 */
    u_tde_miscellaneous tde_miscellaneous_; /* 0x514 */
    u_tde_maccfg tde_maccfg;                 /* 0x518 */
    u_tde_timeout tde_timeout;               /* 0x51c */
    u_tde_eofcnt tde_eofcnt;                 /* 0x520 */
    u_tde_memctrl tde_memctrl;               /* 0x524 */
    td_u32 tde_memctrl1;                     /* 0x528 */
    td_u32 reserved_10[53]; /* 53:0x52c~0x5fc */
#ifdef CONFIG_TDE_GFBG_COMPRESS_V1
    u_tde_od_pic_osd_glb_info tde_od_pic_osd_glb_info;     /* 0x600 */
    u_tde_od_pic_osd_frame_size tde_od_pic_osd_frame_size; /* 0x604 */
    u_tde_od_pic_osd_rc_cfg0 tde_od_pic_osd_rc_cfg0;       /* 0x608 */
    u_tde_od_pic_osd_rc_cfg1 tde_od_pic_osd_rc_cfg1;       /* 0x60c */
    u_tde_od_pic_osd_rc_cfg2 tde_od_pic_osd_rc_cfg2;       /* 0x610 */
    u_tde_od_pic_osd_rc_cfg3 tde_od_pic_osd_rc_cfg3;       /* 0x614 */
    u_tde_od_pic_osd_rc_cfg4 tde_od_pic_osd_rc_cfg4;       /* 0x618 */
    u_tde_od_pic_osd_rc_cfg5 tde_od_pic_osd_rc_cfg5;       /* 0x61c */
    u_tde_od_pic_osd_rc_cfg6 tde_od_pic_osd_rc_cfg6;       /* 0x620 */
    u_tde_od_pic_osd_rc_cfg7 tde_od_pic_osd_rc_cfg7;       /* 0x624 */
    u_tde_od_pic_osd_rc_cfg8 tde_od_pic_osd_rc_cfg8;       /* 0x628 */
    u_tde_od_pic_osd_rc_cfg9 tde_od_pic_osd_rc_cfg9;       /* 0x62c */
    u_tde_od_pic_osd_rc_cfg10 tde_od_pic_osd_rc_cfg10;     /* 0x630 */
    td_u32 reserved_11[19]; /* 19:0x634~0x67c */
#endif
#ifdef CONFIG_TDE_GFBG_COMPRESS_V2
    u_tde_line_osd_cmp_glb_info tde_line_osd_cmp_glb_info; /* 0x600 */
    u_tde_line_osd_cmp_frame_size tde_line_osd_cmp_frame_size; /* 0x604 */
    u_tde_line_osd_cmp_rc_cfg0 tde_line_osd_cmp_rc_cfg0; /* 0x608 */
    u_tde_line_osd_cmp_rc_cfg1 tde_line_osd_cmp_rc_cfg1; /* 0x60c */
    u_tde_line_osd_cmp_rc_cfg2 tde_line_osd_cmp_rc_cfg2; /* 0x610 */
    u_tde_line_osd_cmp_rc_cfg3 tde_line_osd_cmp_rc_cfg3; /* 0x614 */
    u_tde_line_osd_cmp_rc_cfg4 tde_line_osd_cmp_rc_cfg4; /* 0x618 */
    u_tde_line_osd_cmp_rc_cfg5 tde_line_osd_cmp_rc_cfg5; /* 0x61c */
    u_tde_line_osd_cmp_rc_cfg6 tde_line_osd_cmp_rc_cfg6; /* 0x620 */
    u_tde_line_osd_cmp_rc_cfg7 tde_line_osd_cmp_rc_cfg7; /* 0x624 */
    u_tde_line_osd_cmp_rc_cfg8 tde_line_osd_cmp_rc_cfg8; /* 0x628 */
    u_tde_line_osd_cmp_rc_cfg9 tde_line_osd_cmp_rc_cfg9; /* 0x62c */
    u_tde_line_osd_cmp_rc_cfg10 tde_line_osd_cmp_rc_cfg10; /* 0x630 */
    u_tde_line_osd_cmp_rc_cfg11 tde_line_osd_cmp_rc_cfg11; /* 0x634 */
    u_tde_line_osd_cmp_rc_cfg12 tde_line_osd_cmp_rc_cfg12; /* 0x638 */
    u_tde_line_osd_cmp_rc_cfg13 tde_line_osd_cmp_rc_cfg13; /* 0x63c */
    u_tde_line_osd_cmp_rc_cfg14 tde_line_osd_cmp_rc_cfg14; /* 0x640 */
    u_tde_line_osd_cmp_rc_cfg15 tde_line_osd_cmp_rc_cfg15; /* 0x644 */
    u_v4r2_line_osd_cmp_rc_cfg16 v4r2_line_osd_cmp_rc_cfg16; /* 0x648 */
    u_tde_line_osd_cmp_rc_cfg17 tde_line_osd_cmp_rc_cfg17; /* 0x64c */
    u_tde_line_osd_cmp_glb_st tde_line_osd_cmp_glb_st; /* 0x650 */
    u_tde_line_osd_cmp_dbg_reg tde_line_osd_cmp_dbg_reg; /* 0x654 */
    td_u32 reserved_11[10]; /* 10:0x658~0x67c */
#endif
#ifdef CONFIG_TDE_DRD_LINE_SUPPORT
    u_tde_drd_mask tde_drd_mask;                   /* 0x680 */
    u_tde_drd_line_width tde_drd_line_width;       /* 0x684 */
    u_tde_drd_line0_color_cfg tde_drd_line0_color_cfg; /* 0x688 */
    u_tde_drd_line0_st tde_drd_line0_st; /* 0x68c */
    u_tde_drd_line0_ed tde_drd_line0_ed; /* 0x690 */
    u_tde_drd_line1_color_cfg tde_drd_line1_color_cfg; /* 0x694 */
    u_tde_drd_line1_st tde_drd_line1_st; /* 0x698 */
    u_tde_drd_line1_ed tde_drd_line1_ed; /* 0x69c */
    u_tde_drd_line2_color_cfg tde_drd_line2_color_cfg; /* 0x6a0 */
    u_tde_drd_line2_st tde_drd_line2_st; /* 0x6a4 */
    u_tde_drd_line2_ed tde_drd_line2_ed; /* 0x6a8 */
    u_tde_drd_line3_color_cfg tde_drd_line3_color_cfg; /* 0x6ac */
    u_tde_drd_line3_st tde_drd_line3_st; /* 0x6b0 */
    u_tde_drd_line3_ed tde_drd_line3_ed; /* 0x6b4 */
#endif
} tde_hw_node;

typedef enum {
    TDE_DRV_INT_NODE = 0x1,
    TDE_DRV_INT_TIMEOUT = 0x2,
    TDE_DRV_INT_ERROR = 0x4,
    TDE_DRV_INT_NODE_COMP_AQ = 0x8,
} tde_drv_int;

typedef enum {
    OT_GFX_TDE_ID = 0, /* TDE ID */
    OT_GFX_JPGDEC_ID,  /* JPEG DECODE ID */
    OT_GFX_JPGENC_ID,  /* JPEG_ENCODE ID */
    OT_GFX_FB_ID,      /*  FRAMEBUFFER ID */
    OT_GFX_PNG_ID,     /* PNG ID */
    OT_GFX_HIGO_ID,
    OT_GFX_GFX2D_ID,
    OT_GFX_BUTT_ID,
} gfx_mode_id;

#define conver_id(module_id) ((module_id) + OT_ID_TDE - OT_GFX_TDE_ID)

#define CONFIG_TDE_TDE_EXPORT_FUNC

#define TDE_NO_SCALE_VSTEP     0x1000
#define TDE_NO_SCALE_HSTEP     0x100000
#define TDE_FLOAT_BITLEN       12
#define TDE_HAL_HSTEP_FLOATLEN 20
#define TDE_HAL_VSTEP_FLOATLEN 12
#define TDE_MAX_SLICE_WIDTH    256
#define TDE_MAX_SLICE_NUM      20
#define TDE_MAX_SURFACE_PITCH  0xffff
#define TDE_MAX_ZOOM_OUT_STEP  8
#define TDE_MAX_RECT_WIDTH_EX  0x2000
#define TDE_MAX_RECT_HEIGHT_EX 0x2000

#define TDE_MAX_RECT_WIDTH  0x1000
#define TDE_MAX_RECT_HEIGHT 0x1000

#define TDE_MAX_SLICE_RECT_WIDTH  0xfff
#define TDE_MAX_SLICE_RECT_HEIGHT 0xfff

#define TDE_MAX_MINIFICATION_H 255
#define TDE_MAX_MINIFICATION_V 255

#define ROP 0x1                /* Rop */
#define ALPHABLEND (0x1 << 1)  /* AlphaBlend */
#define COLORIZE   (0x1 << 2)  /* Colorize */
#define CLUT       (0x1 << 3)  /* Clut */
#define COLORKEY   (0x1 << 4)  /* ColorKey */
#define CLIP       (0x1 << 5)  /* Clip */
#define DEFLICKER  (0x1 << 6)  /* Deflicker */
#define RESIZE     (0x1 << 7)  /* Resize */
#define MIRROR     (0x1 << 8)  /* Mirror */
#define CSCCOVERT  (0x1 << 9)  /* CSC */
#define QUICKCOPY  (0x1 << 10) /* copy */
#define QUICKFILL  (0x1 << 11) /* fill */
#define PATTERFILL (0x1 << 12) /* patterfill */
#define MASKROP    (0x1 << 13) /* MaskRop */
#define MASKBLEND  (0x1 << 14) /* MaskBlend */
#define ROTATE     (0x1 << 15) /* Rotate */
#define COMPRESS   (0x1 << 16) /* Compress */
#define SYNC       (0x1 << 17) /* Sync */
#define SLICE      (0x1 << 18) /* Slice */

#define ROP_MASK        0xffffffff
#define ALPHABLEND_MASK 0xffffffff
#define COLORIZE_MASK   0xffffffff
#define CLUT_MASK       0xffffffff
#define COLORKEY_MASK   0xffffffff
#define CLIP_MASK       0xffffffff
#define DEFLICKER_MASK  0xffffffff
#define RESIZE_MASK     0xffffffff
#define MIRROR_MASK     0xffffffff
#define CSCCOVERT_MASK  0xffffffff
#define QUICKCOPY_MASK  0xffffffff
#define QUICKFILL_MASK  0xffffffff
#define PATTERFILL_MASK 0xffffffff
#define MASKROP_MASK    0xffffffff
#define MASKBLEND_MASK  0xffffffff
#ifdef CONFIG_TDE_ROTATE_SUPPORT
#define ROTATE_MASK     0xffffffff
#else
#define ROTATE_MASK     0x0
#endif
#define COMPRESS_MASK   0xffffffff
#ifdef CONFIG_GFBG_LOW_DELAY_SUPPORT
#define SYNC_MASK       0xffffffff
#else
#define SYNC_MASK       0x0
#endif
#define SLICE_MASK      0x0

td_void tde_hal_get_capability(td_u32 *capability);

#define TDE_CAPABILITY (((ROP) & (ROP_MASK)) | \
                        ((ALPHABLEND) & (ALPHABLEND_MASK)) | \
                        ((COLORIZE) & (COLORIZE_MASK)) | \
                        ((CLUT) & (CLUT_MASK)) | \
                        ((COLORKEY) & (COLORKEY_MASK)) | \
                        ((CLIP) & (CLIP_MASK)) | \
                        ((DEFLICKER) & (DEFLICKER_MASK)) | \
                        ((RESIZE) & (RESIZE_MASK)) | \
                        ((MIRROR) & (MIRROR_MASK)) | \
                        ((CSCCOVERT) & (CSCCOVERT_MASK)) | \
                        ((QUICKCOPY) & (QUICKCOPY_MASK)) | \
                        ((QUICKFILL) & (QUICKFILL_MASK)) | \
                        ((PATTERFILL) & (PATTERFILL_MASK)) | \
                        ((MASKROP) & (MASKROP_MASK)) | \
                        ((MASKBLEND) & (MASKBLEND_MASK)) | \
                        ((ROTATE) & (ROTATE_MASK)) | \
                        ((COMPRESS) & (COMPRESS_MASK)) | \
                        ((SYNC) & (SYNC_MASK)) | \
                        ((SLICE) & (SLICE_MASK)))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_ADP_H_BODY_PART07_H_ */
