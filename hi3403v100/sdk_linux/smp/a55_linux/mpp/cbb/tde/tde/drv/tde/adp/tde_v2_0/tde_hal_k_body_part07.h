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

#ifndef CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART07_H_
#define CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART07_H_


#if (TDE_CAPABILITY & ROTATE)
td_void tde_hal_node_set_rotate(tde_hw_node *hw_node, drv_tde_rotate_angle rotate_angle)
{
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }
    hw_node->src2_rtt_ctrl.bits.s2_rtt_en = 1;
    /* set bitmap attribute info */
    if (TDE_DRV_COLOR_FMT_YCBCR422 == hw_node->src2_ctrl.bits.src2_fmt) {
        hw_node->src2_rtt_ctrl.bits.s2_rtt_fmt = 0;
    }
    if ((hw_node->src2_ctrl.bits.src2_fmt == TDE_DRV_COLOR_FMT_RGB888) ||
        (TDE_DRV_COLOR_FMT_YCBCR888 == hw_node->src2_ctrl.bits.src2_fmt)) {
        hw_node->src2_rtt_ctrl.bits.s2_rtt_fmt = 1;
    }
    if ((hw_node->src2_ctrl.bits.src2_fmt == TDE_DRV_COLOR_FMT_ARGB4444) ||
        (hw_node->src2_ctrl.bits.src2_fmt == TDE_DRV_COLOR_FMT_ARGB1555)) {
        hw_node->src2_rtt_ctrl.bits.s2_rtt_fmt = 2; /* 2 ARGB4444 ARGB1555 */
    }
    if (hw_node->src2_ctrl.bits.src2_fmt == TDE_DRV_COLOR_FMT_ARGB8888) {
        hw_node->src2_rtt_ctrl.bits.s2_rtt_fmt = 3; /* 3 ARGB8888 */
    }
    if (rotate_angle == DRV_TDE_ROTATE_CLOCKWISE_90) {
        hw_node->src2_rtt_ctrl.bits.s2_rtt_dir = 0;
    } else {
        hw_node->src2_rtt_ctrl.bits.s2_rtt_dir = 1;
    }

    return;
}
#endif

static td_s32 tde_hal_node_file(tde_hw_node *hw_node, tde_base_opt_mode mode, tde_alu_mode alu,
                                const tde_color_fill *color_fill, td_u32 capability)
{
    if (alu > TDE_BUTT) {
        tde_error("alu invalid\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (mode == TDE_QUIKE_COPY) {
        if (!tde_hal_is_operation_support(alu, capability, QUICKCOPY)) {
            tde_error("It does not support QuickCopy\n");
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }
        hw_node->src1_ctrl.bits.src1_en = 1;
        hw_node->src1_ctrl.bits.src1_dma = 1;
    }
    if (mode == TDE_NORM_FILL_1OPT) {
        if (color_fill == TD_NULL) {
            tde_error("stColorFill is null !\n");
            return DRV_ERR_TDE_NULL_PTR;
        }
        tde_fill_data_by_fmt(hw_node, color_fill->fill_data, color_fill->drv_color_fmt, 1);
        hw_node->src2_ctrl.bits.src2_en = 1;
        hw_node->src2_ctrl.bits.src2_mode = 1;
        hw_node->cbmctrl.bits.cbm_en = 1;
        hw_node->cbmctrl.bits.cbm_mode = 1;
        hw_node->cbmctrl.bits.alu_mode = 1;
    }
    if (mode == TDE_NORM_FILL_2OPT) {
        if (color_fill == TD_NULL) {
            tde_error("stColorFill is null !\n");
            return DRV_ERR_TDE_NULL_PTR;
        }
        tde_fill_data_by_fmt(hw_node, color_fill->fill_data, color_fill->drv_color_fmt, 1);
        hw_node->src1_ctrl.bits.src1_en = 1;
        hw_node->src1_ctrl.bits.src1_mode = 0;
        hw_node->src2_ctrl.bits.src2_en = 1;
        hw_node->src2_ctrl.bits.src2_mode = 1;
        hw_node->cbmctrl.bits.cbm_en = 1;
        hw_node->cbmctrl.bits.cbm_mode = 1;

        hw_node->cbmctrl.bits.alu_mode = g_cbmctrl_alu_mode[alu];
    }
    return TD_SUCCESS;
}

static td_s32 tde_hal_node_opt(tde_hw_node *hw_node, tde_base_opt_mode mode, tde_alu_mode alu, td_u32 capability)
{
    if (alu < 0 || alu >= TDE_BUTT) {
        tde_error("alu invalid\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (mode == TDE_SINGLE_SRC_PATTERN_FILL_OPT) {
        if (!tde_hal_is_operation_support(alu, capability, PATTERFILL)) {
            tde_error("It deos not support PatternFill\n");
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }

        hw_node->src2_ctrl.bits.src2_en = 1;
        hw_node->src2_ctrl.bits.src2_mode = 2; /* 2 mode */
        hw_node->cbmctrl.bits.cbm_en = 1;
        hw_node->cbmctrl.bits.cbm_mode = 1;

        hw_node->cbmctrl.bits.alu_mode = g_cbmctrl_alu_mode[alu];
    }
    if (mode == TDE_DOUBLE_SRC_PATTERN_FILL_OPT) {
        if (!tde_hal_is_operation_support(alu, capability, PATTERFILL)) {
            tde_error("It deos not support PatternFill\n");
            return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
        }

        hw_node->src1_ctrl.bits.src1_en = 1;
        hw_node->src1_ctrl.bits.src1_mode = 0;

        hw_node->src2_ctrl.bits.src2_en = 1;
        hw_node->src2_ctrl.bits.src2_mode = 2; /* 2 mode */
        hw_node->cbmctrl.bits.cbm_en = 1;
        hw_node->cbmctrl.bits.cbm_mode = 1;

        hw_node->cbmctrl.bits.alu_mode = g_cbmctrl_alu_mode[alu];
    }
    return TD_SUCCESS;
}

static td_s32 tde_hal_node_set_base_hw(tde_hw_node *hw_node, tde_base_opt_mode mode, tde_alu_mode alu)
{
    if (alu < 0 || alu >= TDE_BUTT) {
        tde_error("alu invalid\n");
        return DRV_ERR_TDE_INVALID_PARA;
    }
    if (mode == TDE_QUIKE_FILL) {
        hw_node->src1_ctrl.bits.src1_en = 1;
        hw_node->src1_ctrl.bits.src1_mode = 1;
        hw_node->src1_ctrl.bits.src1_dma = 1;
    }
    if (mode == TDE_NORM_BLIT_1OPT) {
        hw_node->src2_ctrl.bits.src2_en = 1;
        hw_node->src2_ctrl.bits.src2_mode = 0;
        hw_node->cbmctrl.bits.cbm_en = 1;
        hw_node->cbmctrl.bits.cbm_mode = 1;
        hw_node->cbmctrl.bits.alu_mode = 1;
    }

    if (mode == TDE_NORM_BLIT_2OPT) {
        hw_node->src1_ctrl.bits.src1_en = 1;
        hw_node->src1_ctrl.bits.src1_mode = 0;

        hw_node->src2_ctrl.bits.src2_en = 1;
        hw_node->src2_ctrl.bits.src2_mode = 0;

        hw_node->cbmctrl.bits.cbm_en = 1;
        hw_node->cbmctrl.bits.cbm_mode = 1;

        hw_node->cbmctrl.bits.alu_mode = g_cbmctrl_alu_mode[alu];
    }
    if (mode == TDE_MB_2OPT) {
        hw_node->src2_ctrl.bits.src2_en = 1;
        hw_node->src2_ctrl.bits.src2_mode = 0;

        hw_node->cbmctrl.bits.cbm_en = 1;
        hw_node->cbmctrl.bits.cbm_mode = 1;
        hw_node->cbmctrl.bits.alu_mode = 0x1;
    }
    return TD_SUCCESS;
}

static td_s32 tde_hal_quick_fill(tde_hw_node *hw_node, tde_base_opt_mode mode, tde_alu_mode alu,
    const tde_color_fill *color_fill, td_u32 capability)
{
    if (color_fill == TD_NULL) {
        tde_error("stColorFill is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (!tde_hal_is_operation_support(alu, capability, QUICKFILL)) {
        tde_error("It deos not support QuickFill\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    tde_fill_data_by_fmt(hw_node, color_fill->fill_data, color_fill->drv_color_fmt, 0);
    if (tde_hal_node_set_base_hw(hw_node, mode, alu) != TD_SUCCESS) {
        return DRV_ERR_TDE_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_s32 tde_hal_node_operation_support(tde_alu_mode alu, td_u32 capability)
{
#if (TDE_CAPABILITY & ROP)
    if (!tde_hal_is_operation_support(alu, capability, MASKROP)) {
        tde_error("It deos not support MaskRop\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
#endif
#if (TDE_CAPABILITY & MASKBLEND)
    if (!tde_hal_is_operation_support(alu, capability, MASKBLEND)) {
        tde_error("It deos not support MaskBlend\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
#endif
    return TD_SUCCESS;
}

td_s32 tde_hal_node_set_base_operate(tde_hw_node *hw_node, tde_base_opt_mode mode, tde_alu_mode alu,
                                     const tde_color_fill *color_fill)
{
    td_u32 capability = 0;
    td_s32 ret;

    if (hw_node == TD_NULL) {
        tde_error("hw_node is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    tde_hal_get_capability(&capability);

    switch (mode) {
        case TDE_QUIKE_FILL: /* quick file */
            ret = tde_hal_quick_fill(hw_node, mode, alu, color_fill, capability);
            if (ret != TD_SUCCESS) {
                return ret;
            }
            break;
        case TDE_QUIKE_COPY:
        case TDE_NORM_FILL_1OPT:
        case TDE_NORM_FILL_2OPT:
            ret = tde_hal_node_file(hw_node, mode, alu, color_fill, capability);
            if (ret != TD_SUCCESS) {
                return ret;
            }
            break;
        case TDE_SINGLE_SRC_PATTERN_FILL_OPT:
        case TDE_DOUBLE_SRC_PATTERN_FILL_OPT:
            ret = tde_hal_node_opt(hw_node, mode, alu, capability);
            if (ret != TD_SUCCESS) {
                return ret;
            }
            break;
        case TDE_NORM_BLIT_2OPT:
            ret = tde_hal_node_operation_support(alu, capability);
            if (ret != TD_SUCCESS) {
                return ret;
            }
            fallthrough;
        case TDE_NORM_BLIT_1OPT:
            fallthrough;
        case TDE_MB_2OPT:
            ret = tde_hal_node_set_base_hw(hw_node, mode, alu);
            if (ret != TD_SUCCESS) {
                return ret;
            }
            break;
        default:
            break;
    }
    return TD_SUCCESS;
}

static td_bool tde_hal_is_operation_support(tde_alu_mode alu_mode, td_u32 capability, td_u32 oper_mode)
{
    if (!((td_u32)MASKBLEND & oper_mode) && !((td_u32)MASKROP & oper_mode)) {
        return (td_bool)((capability & oper_mode) ? TD_TRUE : TD_FALSE);
    }

#if (TDE_CAPABILITY & MASKBLEND)
    if ((td_u32)MASKBLEND & oper_mode) {
        return (td_bool)(!((alu_mode == TDE_ALU_MASK_BLEND) && (!(capability & oper_mode))));
    }
#endif

#if (TDE_CAPABILITY & ROP)
    if ((td_u32)MASKROP & oper_mode) {
        return (td_bool)(!(((alu_mode == TDE_ALU_MASK_ROP1) || (alu_mode == TDE_ALU_MASK_ROP2)) &&
                         (!(capability & oper_mode))));
    }
#endif

    return TD_TRUE;
}

td_void tde_hal_node_set_global_alpha(tde_hw_node *hw_node, td_u8 alpha, td_bool enable)
{
    ot_unused(enable);

    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }

    /* set node */ /* CNcomment:配置缓存节点 */
    hw_node->src2_cbmpara.bits.s2_galpha = alpha;

    return;
}

td_void tde_hal_node_set_src1_alpha(tde_hw_node *hw_node)
{
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }

    /* set alpha0 alpha1, CNcomment:alpha0, alpha1 */
    hw_node->src1_alpha.bits.src1_alpha0 = 0;
    hw_node->src1_alpha.bits.src1_alpha1 = 0xff;

    return;
}

td_void tde_hal_node_set_src2_alpha(tde_hw_node *hw_node)
{
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }

    /* set alpha0 alpha1, CNcomment:alpha0, alpha1 */
    hw_node->src2_alpha.bits.src2_alpha0 = 0;
    hw_node->src2_alpha.bits.src2_alpha1 = 0xff;

    return;
}

/*
 * Function:      tde_hal_node_set_exp_alpha
 * Description:   extend to alpha0 and alpha1 operation when extend alpha for RGB5551
 * Input:         hw_node:pointer of node
 *                alpha0: Alpha0 value
 *                alpha1: Alpha1 value
 */
td_void tde_hal_node_set_exp_alpha(tde_hw_node *hw_node, tde_src_mode src, td_u8 alpha0, td_u8 alpha1)
{
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return;
    }

    /* set alpha0 alpha1 */ /* CNcomment:配置alpha0, alpha1 */
    if (TDE_DRV_SRC_S1 & (td_u32)src) {
        hw_node->src1_alpha.bits.src1_alpha0 = alpha0;
        hw_node->src1_alpha.bits.src1_alpha1 = alpha1;
        hw_node->src1_ctrl.bits.src1_rgb_exp = 3; /* 3 set alpha0 alpha1 */
    }

    if (TDE_DRV_SRC_S2 & (td_u32)src) {
        hw_node->src2_alpha.bits.src2_alpha0 = alpha0;
        hw_node->src2_alpha.bits.src2_alpha1 = alpha1;
        hw_node->src2_ctrl.bits.src2_rgb_exp = 3; /* 3 set alpha0 alpha1 */
    }

    return;
}

#if (TDE_CAPABILITY & ROP)
td_s32 tde_hal_node_set_rop(tde_hw_node *hw_node, drv_tde_rop_mode rgb_rop, drv_tde_rop_mode alpha_rop)
{
    td_u32 capability;
    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    tde_hal_get_capability(&capability);
    if (!(capability & ROP)) {
        tde_error("It deos not support Rop\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }
    /* set node, CNcomment: Configure the cache node */
    hw_node->cbmctrl.bits.cbm_en = 1;
    hw_node->cbmctrl.bits.cbm_mode = 1;

    if (rgb_rop < 0 || rgb_rop >= DRV_TDE_ROP_MAX || alpha_rop < 0 || alpha_rop >= DRV_TDE_ROP_MAX) {
        tde_error("rgb_rop(%d) and alpha_rop(%d) invalid !should be in [%d, %d]",
            rgb_rop, alpha_rop, 0, DRV_TDE_ROP_WHITE);
        return DRV_ERR_TDE_INVALID_PARA;
    }
    hw_node->cbmalupara.bits.rgb_rop = (td_u32)rgb_rop;
    hw_node->cbmalupara.bits.a_rop = (td_u32)alpha_rop;

    return TD_SUCCESS;
}
#endif

static td_void tde_hal_node_blend_mode(tde_hw_node *hw_node, const drv_tde_blend_opt *blend_opt)
{
    if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_NONE) {
        /* fs: sa      fd: 1.0-sa */
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_INVSRC2ALPHA, DRV_TDE_BLEND_SRC2ALPHA);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_CLEAR) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_ZERO, DRV_TDE_BLEND_ZERO);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_SRC) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_ZERO, DRV_TDE_BLEND_ONE);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_SRCOVER) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_INVSRC2ALPHA, DRV_TDE_BLEND_ONE);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_DSTOVER) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_ONE, DRV_TDE_BLEND_INVSRC1ALPHA);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_SRCIN) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_ZERO, DRV_TDE_BLEND_SRC1ALPHA);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_DSTIN) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_SRC2ALPHA, DRV_TDE_BLEND_ZERO);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_SRCOUT) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_ZERO, DRV_TDE_BLEND_INVSRC1ALPHA);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_DSTOUT) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_INVSRC2ALPHA, DRV_TDE_BLEND_ZERO);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_SRCATOP) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_INVSRC2ALPHA, DRV_TDE_BLEND_SRC1ALPHA);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_DSTATOP) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_SRC2ALPHA, DRV_TDE_BLEND_INVSRC1ALPHA);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_ADD) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_ONE, DRV_TDE_BLEND_ONE);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_XOR) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_INVSRC2ALPHA, DRV_TDE_BLEND_INVSRC1ALPHA);
    } else if (blend_opt->blend_cmd == DRV_TDE_BLEND_CMD_DST) {
        drv_tde_node_blend_mode(hw_node, DRV_TDE_BLEND_ONE, DRV_TDE_BLEND_ZERO);
    } else {
        /* user parameter, CNcomment: The user's own configuration parameters */
        drv_tde_node_blend_mode(hw_node, blend_opt->src1_blend_mode, blend_opt->src2_blend_mode);
    }
}

td_s32 tde_hal_node_set_blend(tde_hw_node *hw_node, const drv_tde_blend_opt *blend_opt)
{
    td_u32 capability;

    if (hw_node == TD_NULL) {
        tde_error("HWNode is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    if (blend_opt == TD_NULL) {
        tde_error("stBlendOpt is null !\n");
        return DRV_ERR_TDE_NULL_PTR;
    }
    tde_hal_get_capability(&capability);
    if (!(capability & ALPHABLEND)) {
        tde_error("It deos not support Blend\n");
        return DRV_ERR_TDE_UNSUPPORTED_OPERATION;
    }

    hw_node->src1_cbmpara.bits.s1_palphaen = TD_TRUE;
    hw_node->src1_cbmpara.bits.s1_galphaen = TD_FALSE;
    hw_node->src1_cbmpara.bits.s1_multiglobalen = TD_FALSE;


#endif /* CBB_TDE_TDE_DRV_TDE_ADP_TDE_V2_0_TDE_HAL_K_BODY_PART07_H_ */
