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

#ifndef HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART04_H_
#define HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART04_H_

    volatile gen2_sub_pkt0_low    gen2_pkt0l;       /* 18D0 */
    volatile gen2_sub_pkt0_high   gen2_pkt0h;       /* 18D4 */
    volatile gen2_sub_pkt1_low    gen2_pkt1l;       /* offset 0x18D0 + 0x08 */
    volatile gen2_sub_pkt1_high   gen2_pkt1h;       /* 18DC */
    volatile gen2_sub_pkt2_low    gen2_pkt2l;       /* 18E0 */
    volatile gen2_sub_pkt2_high   gen2_pkt2h;       /* 18E4 */
    volatile gen2_sub_pkt3_low    gen2_pkt3l;       /* 18E8 */
    volatile gen2_sub_pkt3_high   gen2_pkt3h;       /* 18EC */
    volatile gen3_pkt_header      gen3_pkt_head;    /* 18F0 */
    volatile gen3_sub_pkt0_low    gen3_pkt0l;       /* 18F4 */
    volatile gen3_sub_pkt0_high   gen3_pkt0h;       /* 18F8 */
    volatile gen3_sub_pkt1_low    gen3_pkt1l;       /* 18FC */
    volatile gen3_sub_pkt1_high   gen3_pkt1h;       /* 1900 */
    volatile gen3_sub_pkt2_low    gen3_pkt2l;       /* 1904 */
    volatile gen3_sub_pkt2_high   gen3_pkt2h;       /* 1908 */
    volatile gen3_sub_pkt3_low    gen3_pkt3l;       /* 190C */
    volatile gen3_sub_pkt3_high   gen3_pkt3h;       /* 1910 */
    volatile gen4_pkt_header      gen4_pkt_head;    /* 1914 */
    volatile gen4_sub_pkt0_low    gen4_pkt0l;       /* 1918 */
    volatile gen4_sub_pkt0_high   gen4_pkt0h;       /* 191C */
    volatile gen4_sub_pkt1_low    gen4_pkt1l;       /* 1920 */
    volatile gen4_sub_pkt1_high   gen4_pkt1h;       /* 1924 */
    volatile gen4_sub_pkt2_low    gen4_pkt2l;       /* 1928 */
    volatile gen4_sub_pkt2_high   gen4_pkt2h;       /* 192C */
    volatile gen4_sub_pkt3_low    gen4_pkt3l;       /* 1930 */
    volatile gen4_sub_pkt3_high   gen4_pkt3h;       /* 1934 */
    volatile gen5_pkt_header      gen5_pkt_head;    /* 1938 */
    volatile gen5_sub_pkt0_low    gen5_pkt0l;       /* 193C */
    volatile gen5_sub_pkt0_high   gen5_pkt0h;       /* 1940 */
    volatile gen5_sub_pkt1_low    gen5_pkt1l;       /* 1944 */
    volatile gen5_sub_pkt1_high   gen5_pkt1h;       /* 1948 */
    volatile gen5_sub_pkt2_low    gen5_pkt2l;       /* 194C */
    volatile gen5_sub_pkt2_high   gen5_pkt2h;       /* 1950 */
    volatile gen5_sub_pkt3_low    gen5_pkt3l;       /* 1954 */
    volatile gen5_sub_pkt3_high   gen5_pkt3h;       /* 1958 */
    volatile gamut_pkt_header     gamut_pkt_head;   /* 195C */
    volatile gamut_sub_pkt0_low   gamut_pkt0l;      /* 1960 */
    volatile gamut_sub_pkt0_high  gamut_pkt0h;      /* 1964 */
    volatile gamut_sub_pkt1_low   gamut_pkt1l;      /* 1968 */
    volatile gamut_sub_pkt1_high  gamut_pkt1h;      /* 196C */
    volatile gamut_sub_pkt2_low   gamut_pkt2l;      /* 1970 */
    volatile gamut_sub_pkt2_high  gamut_pkt2h;      /* 1974 */
    volatile gamut_sub_pkt3_low   gamut_pkt3l;      /* 1978 */
    volatile gamut_sub_pkt3_high  gamut_pkt3h;      /* 197C */
    volatile vsif_pkt_header      vsif_pkt_head;    /* 1980 */
    volatile vsif_sub_pkt0_low    vsif_pkt0l;       /* 1984 */
    volatile vsif_sub_pkt0_high   vsif_pkt0h;       /* 1988 */
    volatile vsif_sub_pkt1_low    vsif_pkt1l;       /* 198C */
    volatile vsif_sub_pkt1_high   vsif_pkt1h;       /* 1990 */
    volatile vsif_sub_pkt2_low    vsif_pkt2l;       /* 1994 */
    volatile vsif_sub_pkt2_high   vsif_pkt2h;       /* 1998 */
    volatile vsif_sub_pkt3_low    vsif_pkt3l;       /* 199C */
    volatile vsif_sub_pkt3_high   vsif_pkt3h;       /* 19A0 */
    volatile cea_avi_cfg          avi_cfg;          /* 19A4 */
    volatile cea_spf_cfg          spf_cfg;          /* 19A8 */
    volatile cea_aud_cfg          aud_cfg;          /* 19AC */
    volatile cea_mpeg_cfg         mpeg_cfg;         /* 19B0 */
    volatile cea_gen_cfg          gen_cfg;          /* 19B4 */
    volatile cea_cp_cfg           cp_cfg;           /* 19B8 */
    volatile cea_gen2_cfg         gen2_cfg;         /* 19BC */
    volatile cea_gen3_cfg         gen3_cfg;         /* 19C0 */
    volatile cea_gen4_cfg         gen4_cfg;         /* 19C4 */
    volatile cea_gen5_cfg         gen5_cfg;         /* 19C8 */
    volatile cea_gamut_cfg        gamut_cfg;        /* 19CC */
    volatile cea_vsif_cfg         vsif_cfg;         /* 19D0 */
    unsigned int                  reserved_1[3];    /* 19D4-19DC */
    volatile cea_pktf_state       pkt_stats;        /* 19E0 */
    unsigned int                  reserved_2[9];    /* 19E4-1A04 */
    volatile cea_avmixer_config   avmixer_cfg;      /* 1A08 */
    volatile cp_pkt_avmute        pkt_avmute;       /* 1A0C */
    volatile video_blank_cfg      vblank_cfg;       /* 1A10 */
    unsigned int                  reserved_3[16];   /* 1A14-1A50 */
    volatile tmds_bist_ctrl       bist_ctrl;        /* 1A54 */
    unsigned int                  reserved_4[2];    /* 1A58-1A5C */
    volatile hdmi_enc_ctrl        enc_ctrl;         /* 1A60 */
    volatile enc_ck_sharp         enc_sharp;        /* 1A64 */
} tx_hdmi_reg_regs_type;

int hdmi_reg_tx_hdmi_regs_init(td_u32 id, const td_char *addr);
int hdmi_reg_tx_hdmi_regs_deinit(td_u32 id);
td_void hdmi_reg_tmds_pack_mode_set(td_u32 id, td_u8 tmds_pack_mode);
td_void hdmi_reg_avi_pkt_header_hb_set(td_u32 id, td_u8 hb0, td_u8 hb1, td_u8 hb2);
td_void hdmi_reg_avi_pkt0_low_set(td_u32 id, td_u8 avi_pkt0_pb0,
    td_u8 avi_pkt0_pb1, td_u8 avi_pkt0_pb2, td_u8 avi_pkt0_pb3);
td_void hdmi_reg_avi_pkt0_high_set(td_u32 id, td_u8 avi_pkt0_pb4, td_u8 avi_pkt0_pb5, td_u8 avi_pkt0_pb6);
td_void hdmi_reg_avi_pkt1_low_set(td_u32 id, td_u8 avi_pkt1_pb0,
    td_u8 avi_pkt1_pb1, td_u8 avi_pkt1_pb2, td_u8 avi_pkt1_pb3);
td_void hdmi_reg_avi_pkt1_high_set(td_u32 id, td_u8 avi_pkt1_pb4, td_u8 avi_pkt1_pb5, td_u8 avi_pkt1_pb6);
td_void hdmi_reg_avi_pkt2_low_set(td_u32 id, td_u8 avi_pkt2_pb0,
    td_u8 avi_pkt2_pb1, td_u8 avi_pkt2_pb2, td_u8 avi_pkt2_pb3);
td_void hdmi_reg_avi_pkt2_high_set(td_u32 id, td_u8 avi_pkt2_pb4, td_u8 avi_pkt2_pb5, td_u8 avi_pkt2_pb6);
td_void hdmi_reg_avi_pkt3_low_set(td_u32 id, td_u8 avi_pkt3_pb0,
    td_u8 avi_pkt3_pb1, td_u8 avi_pkt3_pb2, td_u8 avi_pkt3_pb3);
td_void hdmi_reg_avi_pkt3_high_set(td_u32 id, td_u8 avi_pkt3_pb4, td_u8 avi_pkt3_pb5, td_u8 avi_pkt3_pb6);
td_void hdmi_reg_avi_pkt_header_hb_get(td_u32 id, avi_pkt_header *pkt_header);
td_void hdmi_reg_avi_pkt0_low_get(td_u32 id, avi_sub_pkt0_low *sub_pkt0_low);
td_void hdmi_reg_avi_pkt0_high_get(td_u32 id, avi_sub_pkt0_high *sub_pkt0_high);
td_void hdmi_reg_avi_pkt1_low_get(td_u32 id, avi_sub_pkt1_low *sub_pkt1_low);
td_void hdmi_reg_avi_pkt1_high_get(td_u32 id, avi_sub_pkt1_high *sub_pkt1_high);
td_void hdmi_reg_avi_pkt2_low_get(td_u32 id, avi_sub_pkt2_low *sub_pkt2_low);
void hdmi_reg_avi_pkt2_high_get(td_u32 id, avi_sub_pkt2_high *sub_pkt2_high);
void hdmi_reg_avi_pkt3_low_get(td_u32 id, avi_sub_pkt3_low *sub_pkt3_low);
void hdmi_reg_avi_pkt3_high_get(td_u32 id, avi_sub_pkt3_high *sub_pkt3_high);
td_void hdmi_reg_audio_pkt_header_set(td_u32 id, td_u8 hb0, td_u8 hb1, td_u8 hb2);
td_void hdmi_reg_audio_pkt0_low_set(td_u32 id, td_u8 avi_pkt0_pb0,
    td_u8 avi_pkt0_pb1, td_u8 avi_pkt0_pb2, td_u8 avi_pkt0_pb3);
td_void hdmi_reg_audio_pkt0_high_set(td_u32 id, td_u8 avi_pkt0_pb4,
    td_u8 avi_pkt0_pb5, td_u8 avi_pkt0_pb6);
td_void hdmi_reg_audio_pkt1_low_set(td_u32 id, td_u8 avi_pkt1_pb0,
    td_u8 avi_pkt1_pb1, td_u8 avi_pkt1_pb2, td_u8 avi_pkt1_pb3);
td_void hdmi_reg_audio_pkt1_high_set(td_u32 id, td_u8 avi_pkt1_pb4, td_u8 avi_pkt1_pb5, td_u8 avi_pkt1_pb6);
td_void hdmi_reg_audio_pkt2_low_set(td_u32 id, td_u8 avi_pkt2_pb0,
    td_u8 avi_pkt2_pb1, td_u8 avi_pkt2_pb2, td_u8 avi_pkt2_pb3);
td_void hdmi_reg_audio_pkt2_high_set(td_u32 id, td_u8 avi_pkt2_pb4, td_u8 avi_pkt2_pb5, td_u8 avi_pkt2_pb6);
td_void hdmi_reg_audio_pkt3_low_set(td_u32 id, td_u8 avi_pkt3_pb0,
    td_u8 avi_pkt3_pb1, td_u8 avi_pkt3_pb2, td_u8 avi_pkt3_pb3);
td_void hdmi_reg_audio_pkt3_high_set(td_u32 id, td_u8 avi_pkt3_pb4, td_u8 avi_pkt3_pb5, td_u8 avi_pkt3_pb6);
td_void hdmi_reg_aif_pkt_header_get(td_u32 id, aif_pkt_header *pkt_header);
td_void hdmi_reg_aif_pkt0_low_get(td_u32 id, aif_sub_pkt0_low *sub_pkt0_low);
td_void hdmi_reg_aif_pkt0_high_get(td_u32 id, aif_sub_pkt0_high *sub_pkt0_high);
td_void hdmi_reg_aif_pkt1_low_get(td_u32 id, aif_sub_pkt1_low *sub_pkt1_low);
td_void hdmi_reg_aif_pkt1_high_get(td_u32 id, aif_sub_pkt1_high *sub_pkt1_high);
td_void hdmi_reg_aif_pkt2_low_get(td_u32 id, aif_sub_pkt2_low *sub_pkt2_low);
td_void hdmi_reg_aif_pkt2_high_get(td_u32 id, aif_sub_pkt2_high *sub_pkt2_high);
td_void hdmi_reg_aif_pkt3_low_get(td_u32 id, aif_sub_pkt3_low *sub_pkt3_low);
td_void hdmi_reg_aif_pkt3_high_get(td_u32 id, aif_sub_pkt3_high *sub_pkt3_high);
td_void hdmi_reg_gamut_pkt_header_get(td_u32 id, gamut_pkt_header *pkt_header);
td_void hdmi_reg_gamut_pkt0_low_get(td_u32 id, gamut_sub_pkt0_low *sub_pkt0_low);
td_void hdmi_reg_gamut_pkt0_high_get(td_u32 id, gamut_sub_pkt0_high *sub_pkt0_high);
td_void hdmi_reg_gamut_pkt1_low_get(td_u32 id, gamut_sub_pkt1_low *sub_pkt1_low);
td_void hdmi_reg_gamut_pkt1_high_get(td_u32 id, gamut_sub_pkt1_high *sub_pkt1_high);
td_void hdmi_reg_gamut_pkt2_low_get(td_u32 id, gamut_sub_pkt2_low *sub_pkt2_low);
td_void hdmi_reg_gamut_pkt2_high_get(td_u32 id, gamut_sub_pkt2_high *sub_pkt2_high);
td_void hdmi_reg_gamut_pkt3_low_get(td_u32 id, gamut_sub_pkt3_low *sub_pkt3_low);
td_void hdmi_reg_gamut_pkt3_high_get(td_u32 id, gamut_sub_pkt3_high *sub_pkt3_high);
td_void hdmi_reg_vsif_header_set(td_u32 id, td_u8 hb0, td_u8 hb1, td_u8 hb2);
td_void hdmi_reg_vsif_pkt0_low_set(td_u32 id, td_u8 vsif_pkt0_pb0,
    td_u8 vsif_pkt0_pb1, td_u8 vsif_pkt0_pb2, td_u8 vsif_pkt0_pb3);
td_void hdmi_reg_vsif_pkt0_high_set(td_u32 id, td_u8 vsif_pkt0_pb4, td_u8 vsif_pkt0_pb5, td_u8 vsif_pkt0_pb6);
td_void hdmi_reg_vsif_pkt1_low_set(td_u32 id, td_u8 vsif_pkt1_pb0,
    td_u8 vsif_pkt1_pb1, td_u8 vsif_pkt1_pb2, td_u8 vsif_pkt1_pb3);
td_void hdmi_reg_vsif_pkt1_high_set(td_u32 id, td_u8 vsif_pkt1_pb4, td_u8 vsif_pkt1_pb5, td_u8 vsif_pkt1_pb6);
td_void hdmi_reg_vsif_pkt2_low_set(td_u32 id, td_u8 vsif_pkt2_pb0,
    td_u8 vsif_pkt2_pb1, td_u8 vsif_pkt2_pb2, td_u8 vsif_pkt2_pb3);
td_void hdmi_reg_vsif_pkt2_high_set(td_u32 id, td_u8 vsif_pkt2_pb4, td_u8 vsif_pkt2_pb5, td_u8 vsif_pkt2_pb6);
td_void hdmi_reg_vsif_pkt3_low_set(td_u32 id, td_u8 vsif_pkt3_pb0,
    td_u8 vsif_pkt3_pb1, td_u8 vsif_pkt3_pb2, td_u8 vsif_pkt3_pb3);
td_void hdmi_reg_vsif_pkt3_high_set(td_u32 id, td_u8 vsif_pkt3_pb4, td_u8 vsif_pkt3_pb5, td_u8 vsif_pkt3_pb6);
td_void hdmi_reg_vsif_pkt_header_get(td_u32 id, vsif_pkt_header *pkt_header);
td_void hdmi_reg_vsif_pkt0_low_get(td_u32 id, vsif_sub_pkt0_low *sub_pkt0_low);
td_void hdmi_reg_vsif_pkt0_high_get(td_u32 id, vsif_sub_pkt0_high *sub_pkt0_high);
td_void hdmi_reg_vsif_pkt1_low_get(td_u32 id, vsif_sub_pkt1_low *sub_pkt1_low);
td_void hdmi_reg_vsif_pkt1_high_get(td_u32 id, vsif_sub_pkt1_high *sub_pkt1_high);
td_void hdmi_reg_vsif_pkt2_low_get(td_u32 id, vsif_sub_pkt2_low *sub_pkt2_low);
td_void hdmi_reg_vsif_pkt2_high_get(td_u32 id, vsif_sub_pkt2_high *sub_pkt2_high);
td_void hdmi_reg_vsif_pkt3_low_get(td_u32 id, vsif_sub_pkt3_low *sub_pkt3_low);
td_void hdmi_reg_vsif_pkt3_high_get(td_u32 id, vsif_sub_pkt3_high *sub_pkt3_high);
td_void hdmi_reg_cea_avi_en_set(td_u32 id, td_u8 cea_avi_en);
td_void hdmi_reg_cea_avi_rpt_en_set(td_u32 id, td_u8 cea_avi_rpt_en);
td_void hdmi_reg_cea_aud_en_set(td_u32 id, td_u8 cea_aud_en);
td_void hdmi_reg_cea_aud_rpt_en_set(td_u32 id, td_u8 cea_aud_rpt_en);
td_void hdmi_reg_cea_cp_en_set(td_u32 id, td_u8 cea_cp_en);
td_void hdmi_reg_cea_cp_rpt_cnt_set(td_u32 id, td_u8 cea_cp_rpt_cnt);
td_void hdmi_reg_cea_cp_rpt_en_set(td_u32 id, td_u8 cea_cp_rpt_en);
td_void hdmi_reg_cea_gamut_en_set(td_u32 id, td_u8 cea_gamut_en);
td_void hdmi_reg_cea_gamut_rpt_en_set(td_u32 id, td_u8 cea_gamut_rpt_en);
td_void hdmi_reg_cea_vsif_en_set(td_u32 id, td_u8 cea_vsif_en);
td_void hdmi_reg_cea_vsif_rpt_en_set(td_u32 id, td_u8 cea_vsif_rpt_en);
td_void hdmi_reg_hdmi_mode_set(td_u32 id, td_u8 hdmi_mode);
td_void hdmi_reg_dc_pkt_en_set(td_u32 id, td_u8 dc_pkt_en);
td_void hdmi_reg_null_pkt_en_set(td_u32 id, td_u8 null_pkt_en);
td_void hdmi_reg_hdmi_dvi_sel_set(td_u32 id, td_u8 hdmi_dvi_sel);
td_void hdmi_reg_eess_mode_en_set(td_u32 id, td_u8 eess_mode_en);
td_void hdmi_reg_cp_set_avmute_set(td_u32 id, td_u8 cp_set_avmute);
td_void hdmi_reg_cp_clr_avmute_set(td_u32 id, td_u8 cp_clr_avmute);
td_void hdmi_reg_enc_hdmi2_on_set(td_u32 id, td_u8 enc_hdmi2_on);
td_void hdmi_reg_enc_scr_on_set(td_u32 id, td_u8 enc_scr_on);
td_void hdmi_reg_enc_bypass_set(td_u32 id, td_u8 enc_bypass);
td_u8 hdmi_reg_tmds_pack_mode_get(td_u32 id);
td_u8 hdmi_reg_pclk2tclk_stable_get(td_u32 id);
td_u8 hdmi_reg_cea_avi_en_get(td_u32 id);
td_u8 hdmi_reg_cea_aud_en_get(td_u32 id);
td_u8 hdmi_reg_cea_cp_rpt_en_get(td_u32 id);
td_u8 hdmi_reg_cea_gamut_en_get(td_u32 id);
td_u8 hdmi_reg_cea_vsif_rpt_en_get(td_u32 id);
td_u8 hdmi_reg_hdmi_mode_get(td_u32 id);
td_u8 hdmi_reg_dc_pkt_en_get(td_u32 id);
td_u8 hdmi_reg_cp_set_avmute_get(td_u32 id);
td_u8 hdmi_reg_enc_hdmi2_on_get(td_u32 id);
td_u8 hdmi_reg_enc_scr_on_get(td_u32 id);
td_u8 hdmi_reg_null_pkt_en_get(td_u32 id);


#endif /* HDMI_SRC_MKP_HAL_CTRL_V200_REGS_HDMI_REG_TX_H_BODY_PART04_H_ */
