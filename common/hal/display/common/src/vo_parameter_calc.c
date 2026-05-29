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

#include <math.h>
#include <stdint.h>
#include "display_layer_internal.h"
#include "disp_common.h"

#define PIXEL_CLK_16P33M    16.33
#define PIXEL_CLK_16M       16
#define PIXEL_CLK_66M       66
#define PIXEL_CLK_264M      264
#define PIXEL_CLK_792M      792
#define PIXEL_CLK_3200M     3200

#define FACTOR_1            6.0 // pixel clock calculate factor from <<RGB_MIPI panel clock timming calculate>>
#define FACTOR_2            49.0 // pixel clock calculate factor from <<RGB_MIPI panel clock timming calculate>>
#define FACTOR_3            4.0 // pixel clock calculate factor from <<RGB_MIPI panel clock timming calculate>>
#define DEF_FRAC            16777215 // default fraction from <<RGB_MIPI panel clock timming calculate>>
#define BASE_NUM            2 // base number from <<RGB_MIPI panel clock timming calculate>>
#define EXP_NUM             24 // exponent number from <<RGB_MIPI panel clock timming calculate>>

static uint32_t CalcFbDiv(double pixClk, double pclkInt)
{
    const double baseNumber = 0.12244898; // base fraction number from <<RGB_MIPI panel clock timming calculate>>

    if (pixClk >= PIXEL_CLK_16P33M  && pclkInt <= PIXEL_CLK_66M) {
        double val = (pixClk - baseNumber) / baseNumber;
        HDF_LOGI("%s: pClkInt = %f", __func__, ceil(val));
        return (uint32_t)ceil(val);
    }
    if (pclkInt > PIXEL_CLK_66M && pclkInt <= PIXEL_CLK_3200M) {
        return (uint32_t)pclkInt;
    }
    HDF_LOGE("%s: FbDiv error", __func__);
    return 0;
}

static uint32_t CalcMipiFrac(double pixClk, double pclkInt, double pclkFrac, uint32_t fbDiv)
{
    double val;

    if (pixClk >= PIXEL_CLK_16P33M && pclkInt <= PIXEL_CLK_66M) {
        val = ((pixClk - (FACTOR_1 / FACTOR_2 * fbDiv)) * FACTOR_3 * FACTOR_2) / (EXP_NUM / pow(BASE_NUM, EXP_NUM));
        return (uint32_t)floor(val);
    }
    if (pclkInt > PIXEL_CLK_66M && pclkInt <= PIXEL_CLK_3200M) {
        val = pclkFrac * pow(BASE_NUM, EXP_NUM);
        if (val > DEF_FRAC) {
            return DEF_FRAC;
        }
        return (uint32_t)val;
    }
    HDF_LOGE("%s: Frac error", __func__);
    return 0;
}

static uint32_t CalcMipiRefDiv(double pclkInt)
{
    const uint32_t refDiv4 = 4;   // refer clock 4 div number from <<RGB_MIPI panel clock timming calculate.xlsx>>
    const uint32_t refDiv2 = 2;   // refer clock 2 div number from <<RGB_MIPI panel clock timming calculate.xlsx>>
    const uint32_t refDiv8 = 8;   // refer clock 8 div number from <<RGB_MIPI panel clock timming calculate.xlsx>>
    const uint32_t refDiv24 = 24; // refer clock 24 div number from <<RGB_MIPI panel clock timming calculate.xlsx>>

    if (pclkInt >= PIXEL_CLK_16M && pclkInt <= PIXEL_CLK_66M) {
        return refDiv4;
    }
    if (pclkInt > PIXEL_CLK_66M && pclkInt <= PIXEL_CLK_264M) {
        return refDiv2;
    }
    if (pclkInt > PIXEL_CLK_264M && pclkInt <= PIXEL_CLK_792M) {
        return refDiv8;
    }
    if (pclkInt > PIXEL_CLK_792M && pclkInt <= PIXEL_CLK_3200M) {
        return refDiv24;
    }
    HDF_LOGE("%s: RefDiv error", __func__);
    return 0;
}

static uint32_t CalcPostDiv1(double pclkInt)
{
    const uint32_t refDiv7 = 7; // refer clock 7 div number from <<RGB_MIPI panel clock timming calculate.xlsx>>
    const uint32_t refDiv4 = 4; // refer clock 4 div number from <<RGB_MIPI panel clock timming calculate.xlsx>>
    const uint32_t refDiv3 = 3; // refer clock 3 div number from <<RGB_MIPI panel clock timming calculate.xlsx>>
    const uint32_t refDiv1 = 1; // refer clock 1 div number from <<RGB_MIPI panel clock timming calculate.xlsx>>

    if (pclkInt >= PIXEL_CLK_16M && pclkInt <= PIXEL_CLK_66M) {
        return refDiv7;
    }
    if (pclkInt > PIXEL_CLK_66M && pclkInt <= PIXEL_CLK_264M) {
        return refDiv4;
    }
    if (pclkInt > PIXEL_CLK_264M && pclkInt <= PIXEL_CLK_792M) {
        return refDiv3;
    }
    if (pclkInt > PIXEL_CLK_792M && pclkInt <= PIXEL_CLK_3200M) {
        return refDiv1;
    }
    HDF_LOGE("%s: PostDiv1 error", __func__);
    return 0;
}

static uint32_t CalcPostDiv2(double pclkInt)
{
    const uint32_t refDiv7 = 7; // refer clock 7 div number from <<RGB_MIPI panel clock timming calculate>>
    const uint32_t refDiv3 = 3; // refer clock 3 div number from <<RGB_MIPI panel clock timming calculate>>
    const uint32_t refDiv1 = 1; // refer clock 1 div number from <<RGB_MIPI panel clock timming calculate>>

    if (pclkInt >= PIXEL_CLK_16M && pclkInt <= PIXEL_CLK_66M) {
        return refDiv7;
    }
    if (pclkInt > PIXEL_CLK_66M && pclkInt <= PIXEL_CLK_264M) {
        return refDiv3;
    }
    if (pclkInt > PIXEL_CLK_264M && pclkInt <= PIXEL_CLK_792M) {
        return refDiv1;
    }
    if (pclkInt > PIXEL_CLK_792M && pclkInt <= PIXEL_CLK_3200M) {
        return refDiv1;
    }
    HDF_LOGE("%s: PostDiv2 error", __func__);
    return 0;
}

static uint32_t CalcRgbFrac(double pixClk, double pclkInt, uint32_t fbDiv)
{
    double val;

    if (pixClk >= PIXEL_CLK_16P33M && pclkInt <= PIXEL_CLK_66M) {
        val = ((pixClk - (FACTOR_1 / FACTOR_2 * fbDiv)) * FACTOR_3 * FACTOR_2) / (EXP_NUM / pow(BASE_NUM, EXP_NUM));
        return (uint32_t)floor(val);
    }
    if (pclkInt > PIXEL_CLK_66M && pclkInt <= PIXEL_CLK_3200M) {
        val = pclkInt * pow(BASE_NUM, EXP_NUM);
        if (val > DEF_FRAC) {
            return DEF_FRAC;
        }
        return (uint32_t)val;
    }
    HDF_LOGE("%s: Frac error", __func__);
    return 0;
}

static uint32_t CalcRgbRefDiv(double pclkInt)
{
    const uint32_t refDiv1 = 1;   // refer clock 1 div number from <<RGB_MIPI panel clock timming calculate>>
    const uint32_t refDiv2 = 2;   // refer clock 2 div number from <<RGB_MIPI panel clock timming calculate>>
    const uint32_t refDiv8 = 8;   // refer clock 8 div number from <<RGB_MIPI panel clock timming calculate>>
    const uint32_t refDiv24 = 24; // refer clock 24 div number from <<RGB_MIPI panel clock timming calculate>>

    if (pclkInt >= PIXEL_CLK_16P33M && pclkInt <= PIXEL_CLK_66M) {
        return refDiv1;
    }
    if (pclkInt > PIXEL_CLK_66M && pclkInt <= PIXEL_CLK_264M) {
        return refDiv2;
    }
    if (pclkInt > PIXEL_CLK_264M && pclkInt <= PIXEL_CLK_792M) {
        return refDiv8;
    }
    if (pclkInt > PIXEL_CLK_792M && pclkInt <= PIXEL_CLK_3200M) {
        return refDiv24;
    }
    HDF_LOGE("%s: RefDiv error", __func__);
    return 0;
}

int32_t GetVoClkParameter(const struct DispInfo *info, VO_USER_INTFSYNC_PLL_S *clk)
{
    double pixClk;
    double hpixel;
    double vline;
    double pclkInt = 0.0;
    double pclkFrac;

    if (info == NULL || clk == NULL) {
        HDF_LOGE("%s: null pointer", __func__);
        return DISPLAY_FAILURE;
    }
    hpixel = info->width + info->hbp + info->hfp + info->hsw;
    vline = info->height + info->vbp + info->vfp + info->vsw;
    /* calculate pixel clock, 10^6 from <<RGB_MIPI panel clock timming calculate>> */
    pixClk = (hpixel * vline * info->frameRate) / pow(10, 6);
    pclkFrac = modf(pixClk, &pclkInt);
    clk->fb_div = CalcFbDiv(pixClk, pclkInt);
    if (clk->fb_div == 0) {
        HDF_LOGE("%s: calc fbdiv error", __func__);
        return DISPLAY_FAILURE;
    }
    clk->post_div1 = CalcPostDiv1(pclkInt);
    clk->post_div2 = CalcPostDiv2(pclkInt);
    if (info->intfType == VO_INTF_MIPI) {
        clk->frac = CalcMipiFrac(pixClk, pclkInt, pclkFrac, clk->fb_div);
        clk->ref_div = CalcMipiRefDiv(pclkInt);
    } else {
        clk->frac = CalcRgbFrac(pixClk, pclkInt, clk->fb_div);
        clk->ref_div = CalcRgbRefDiv(pclkInt);
    }
    HDF_LOGI("%s: pixClk = %f, pclkInt = %f, pclkFrac = %f", __func__, pixClk, pclkInt, pclkFrac);
    HDF_LOGI("%s: Fbdiv = %u, Frac = 0x%x, Refdiv = %u, Postdiv1 = %u, Postdiv2 = %u ", __func__,
        clk->fb_div, clk->frac, clk->ref_div, clk->post_div1, clk->post_div2);
    return DISPLAY_SUCCESS;
}
