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

#include "sys_ctrl.h"
#include "sys_cfg.h"

static void set_vi_online_video_norm_vpss_online_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                                    npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x1000, 0x44777755);
    sys_writel(misc_base + 0x1004, 0x45457077);
    sys_writel(misc_base + 0x1008, 0x60050055);
    sys_writel(misc_base + 0x100c, 0x45433306);
    sys_writel(misc_base + 0x1010, 0x33333366);
    sys_writel(misc_base + 0x1014, 0x33503333);
    sys_writel(misc_base + 0x1018, 0x00044466);

    /*
     * read_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                       npu_mmu_pref npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x101c, 0x44777765);
    sys_writel(misc_base + 0x1020, 0x55557077);
    sys_writel(misc_base + 0x1024, 0x60050056);
    sys_writel(misc_base + 0x1028, 0x46433306);
    sys_writel(misc_base + 0x102c, 0x66555377);
    sys_writel(misc_base + 0x1030, 0x33503663);
    sys_writel(misc_base + 0x1034, 0x00055577);
}

static void set_vi_online_video_norm_vpss_offline_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                                    npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x1000, 0x44677755);
    sys_writel(misc_base + 0x1004, 0x45457077);
    sys_writel(misc_base + 0x1008, 0x60050055);
    sys_writel(misc_base + 0x100c, 0x45433306);
    sys_writel(misc_base + 0x1010, 0x33333366);
    sys_writel(misc_base + 0x1014, 0x33503333);
    sys_writel(misc_base + 0x1018, 0x00044466);

    /*
     * read_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                       npu_mmu_pref npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x101c, 0x44677765);
    sys_writel(misc_base + 0x1020, 0x55557077);
    sys_writel(misc_base + 0x1024, 0x60050056);
    sys_writel(misc_base + 0x1028, 0x46433306);
    sys_writel(misc_base + 0x102c, 0x66555377);
    sys_writel(misc_base + 0x1030, 0x33503663);
    sys_writel(misc_base + 0x1034, 0x00055577);
}

static void set_vi_online_video_advanced_vpss_offline_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                                    npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x1000, 0x44557755);
    sys_writel(misc_base + 0x1004, 0x45457077);
    sys_writel(misc_base + 0x1008, 0x60050055);
    sys_writel(misc_base + 0x100c, 0x45433306);
    sys_writel(misc_base + 0x1010, 0x33333366);
    sys_writel(misc_base + 0x1014, 0x33504334);
    sys_writel(misc_base + 0x1018, 0x00044466);

    /*
     * read_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                       npu_mmu_pref npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x101c, 0x44557765);
    sys_writel(misc_base + 0x1020, 0x55557077);
    sys_writel(misc_base + 0x1024, 0x60050056);
    sys_writel(misc_base + 0x1028, 0x46433306);
    sys_writel(misc_base + 0x102c, 0x66555377);
    sys_writel(misc_base + 0x1030, 0x33506666);
    sys_writel(misc_base + 0x1034, 0x00055577);
}

static void set_vi_offline_video_norm_vpss_online_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                                    npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x1000, 0x44556755);
    sys_writel(misc_base + 0x1004, 0x45457077);
    sys_writel(misc_base + 0x1008, 0x60050055);
    sys_writel(misc_base + 0x100c, 0x45433306);
    sys_writel(misc_base + 0x1010, 0x33333366);
    sys_writel(misc_base + 0x1014, 0x33503333);
    sys_writel(misc_base + 0x1018, 0x00044466);

    /*
     * read_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                       npu_mmu_pref npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x101c, 0x44666755);
    sys_writel(misc_base + 0x1020, 0x55557077);
    sys_writel(misc_base + 0x1024, 0x60050056);
    sys_writel(misc_base + 0x1028, 0x46433306);
    sys_writel(misc_base + 0x102c, 0x66555377);
    sys_writel(misc_base + 0x1030, 0x33503663);
    sys_writel(misc_base + 0x1034, 0x00055577);
}

static void set_vi_offline_video_norm_vpss_offline_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                                    npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x1000, 0x44566755);
    sys_writel(misc_base + 0x1004, 0x45457077);
    sys_writel(misc_base + 0x1008, 0x60050055);
    sys_writel(misc_base + 0x100c, 0x45433306);
    sys_writel(misc_base + 0x1010, 0x33333366);
    sys_writel(misc_base + 0x1014, 0x33503333);
    sys_writel(misc_base + 0x1018, 0x00044466);

    /*
     * read_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                       npu_mmu_pref npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x101c, 0x44566766);
    sys_writel(misc_base + 0x1020, 0x55557077);
    sys_writel(misc_base + 0x1024, 0x60050056);
    sys_writel(misc_base + 0x1028, 0x46433306);
    sys_writel(misc_base + 0x102c, 0x66555377);
    sys_writel(misc_base + 0x1030, 0x33503663);
    sys_writel(misc_base + 0x1034, 0x00055577);
}

static void set_vi_offline_video_advanced_vpss_offline_qos(void)
{
    void *misc_base = sys_config_get_reg_misc();

    /*
     * write_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                                    npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x1000, 0x44566755);
    sys_writel(misc_base + 0x1004, 0x45457077);
    sys_writel(misc_base + 0x1008, 0x60050055);
    sys_writel(misc_base + 0x100c, 0x45433306);
    sys_writel(misc_base + 0x1010, 0x33333366);
    sys_writel(misc_base + 0x1014, 0x33504334);
    sys_writel(misc_base + 0x1018, 0x00044466);

    /*
     * read_qos:
     * 30:28        26:24        22:20       18:16        14:12        10:8         6:4          2:0
     * ive1_dpu     ive0         vpss        viproc_post  viproc_pre   vicap        vdh          vedu
     * tde          vgs0         jpgd        jpge         mcu_axim     mcu_ahbm     dsp1         dps0
     * gsf0         sdio1        sdio0       edma         spacc        fmc          vgs1         gdc
     * coresight    cpu          gzip        usb3_1       usb3_0       pcie                      gsf1
     *                                       npu_mmu_pref npu_hwts     npu_data     vdp          aiao
     * dsp1_idma    dsp0_idma    mcu_edma    emmc
     *              spacc_mmu    spacc_sym   avsp_m2      avsp_m1      avsp_m0      usb3_1_uvc   usb3_0_uvc
     */
    sys_writel(misc_base + 0x101c, 0x44566766);
    sys_writel(misc_base + 0x1020, 0x55557077);
    sys_writel(misc_base + 0x1024, 0x60050056);
    sys_writel(misc_base + 0x1028, 0x46433306);
    sys_writel(misc_base + 0x102c, 0x66555377);
    sys_writel(misc_base + 0x1030, 0x33506666);
    sys_writel(misc_base + 0x1034, 0x00055577);
}

void sys_ctl(int online_flag)
{
    switch (online_flag) {
        case VI_ONLINE_VIDEO_NORM_VPSS_ONLINE:
            set_vi_online_video_norm_vpss_online_qos();
            break;
        case VI_ONLINE_VIDEO_NORM_VPSS_OFFLINE:
            set_vi_online_video_norm_vpss_offline_qos();
            break;
        case VI_ONLINE_VIDEO_ADVANCED_VPSS_OFFLINE:
        case VI_ONLINE_VIDEO_ADVANCED_VPSS_ONLINE:
            set_vi_online_video_advanced_vpss_offline_qos();
            break;
        case VI_OFFLINE_VIDEO_NORM_VPSS_ONLINE:
            set_vi_offline_video_norm_vpss_online_qos();
            break;
        case VI_OFFLINE_VIDEO_NORM_VPSS_OFFLINE:
            set_vi_offline_video_norm_vpss_offline_qos();
            break;
        case VI_OFFLINE_VIDEO_ADVANCED_VPSS_OFFLINE:
        case VI_OFFLINE_VIDEO_ADVANCED_VPSS_ONLINE:
            set_vi_offline_video_advanced_vpss_offline_qos();
            break;
        default:
            return;
    }
}

EXPORT_SYMBOL(sys_ctl);
