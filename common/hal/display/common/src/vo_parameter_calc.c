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

#define PLL_VCO_MIN          800   // VCO floor in MHz
#define PLL_VCO_MAX          3200  // VCO ceiling in MHz
#define PLL_FBDIV_MAX        4095  // fb_div must not exceed this
#define PLL_FRAC_DENOM       24.0  // 24 MHz reference clock
#define PLL_REF_DIV          1     // hi3403v100 ref_div is always 1
#define PLL_PRE_DIV          1     // pre_div default
#define PLL_DEV_DIV          1     // dev_div default

/*
 * Grid search for (post_div1, post_div2). Returns the first (a,b) pair
 * where srcClk * a * b falls in (PLL_VCO_MIN, PLL_VCO_MAX).
 * Search order matches MIPI clock calculator spreadsheet.
 */
static void FindPostDiv(double srcClk, uint32_t *postDiv1, uint32_t *postDiv2)
{
    /* ordered (a,b) pairs: (1,1)(2,1)(3,1)(2,2)(5,1)(3,2)(7,1)(4,2)(3,3)
     *                       (5,2)(4,3)(7,2)(5,3)(4,4)(6,3)(5,4)(7,3)(6,4)
     *                       (5,5)(7,4)(6,5)(7,5)(6,6)(7,6)(7,7) */
    static const uint8_t grid[][2] = {
        {1,1},{2,1},{3,1},{2,2},{5,1},{3,2},{7,1},{4,2},{3,3},
        {5,2},{4,3},{7,2},{5,3},{4,4},{6,3},{5,4},{7,3},{6,4},
        {5,5},{7,4},{6,5},{7,5},{6,6},{7,6},{7,7}
    };
    const int gridSize = sizeof(grid) / sizeof(grid[0]);
    int i;

    for (i = 0; i < gridSize; i++) {
        uint32_t a = grid[i][0];
        uint32_t b = grid[i][1];
        double vco = srcClk * a * b;
        if (vco > PLL_VCO_MIN && vco < PLL_VCO_MAX) {
            *postDiv1 = a;
            *postDiv2 = b;
            return;
        }
    }
    /* Should never reach here for valid pixel clocks */
    HDF_LOGE("%s: no valid (post_div1, post_div2) found for srcClk=%f", __func__, srcClk);
    *postDiv1 = 1;
    *postDiv2 = 1;
}

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

static __attribute__((unused)) uint32_t CalcMipiFrac(double pixClk, double pclkInt, double pclkFrac, uint32_t fbDiv)
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

static __attribute__((unused)) uint32_t CalcMipiRefDiv(double pclkInt)
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
    double srcClk;
    double pclkInt = 0.0;
    double pclkFrac;
    uint32_t postDiv1;
    uint32_t postDiv2;
    double fbDivInt;
    double fracPart;

    if (info == NULL || clk == NULL) {
        HDF_LOGE("%s: null pointer", __func__);
        return DISPLAY_FAILURE;
    }

    hpixel = info->width + info->hbp + info->hfp + info->hsw;
    vline = info->height + info->vbp + info->vfp + info->vsw;
    /* calculate pixel clock, 10^6 from <<RGB_MIPI panel clock timming calculate>> */
    pixClk = (hpixel * vline * info->frameRate) / pow(10, 6);
    pclkFrac = modf(pixClk, &pclkInt);

    if (info->intfType == VO_INTF_MIPI || info->intfType == 0) {
        /* VO_INTF_MIPI=0x400 from MPP SDK, or 0=MIPI_DSI from HDF enum */
        /*
         * PLL formula (hi3403v100 manual Table 3-4):
         *   FOUTVCO    = pixel_clk * pstdiv1 * pstdiv2
         *   fb_div_int = FOUTVCO * refdiv / FREF
         *   FREF = 24MHz, refdiv = 1 (recommended)
         */
        srcClk = pixClk * PLL_PRE_DIV * PLL_DEV_DIV;

        FindPostDiv(srcClk, &postDiv1, &postDiv2);
        clk->post_div1 = postDiv1;
        clk->post_div2 = postDiv2;
        clk->ref_div = PLL_REF_DIV;

        /* FOUTVCO * refdiv / FREF = srcClk * postDiv1 * postDiv2 * refdiv / 24 */
        fbDivInt = srcClk * PLL_REF_DIV * postDiv1 * postDiv2 / PLL_FRAC_DENOM;
        fracPart = fbDivInt - floor(fbDivInt);

        if (fracPart > 0.999999) {
            /* fractional part rounds up to next integer */
            uint32_t fbCeil = (uint32_t)ceil(fbDivInt);
            if (fbCeil > PLL_FBDIV_MAX) {
                HDF_LOGE("%s: fb_div %u exceeds max %u", __func__, fbCeil, PLL_FBDIV_MAX);
                return DISPLAY_FAILURE;
            }
            clk->fb_div = fbCeil;
            clk->frac = 0;
        } else {
            uint32_t fbFloor = (uint32_t)floor(fbDivInt);
            if (fbFloor > PLL_FBDIV_MAX) {
                HDF_LOGE("%s: fb_div %u exceeds max %u", __func__, fbFloor, PLL_FBDIV_MAX);
                return DISPLAY_FAILURE;
            }
            clk->fb_div = fbFloor;
            clk->frac = (uint32_t)ceil(fracPart * pow(2, 24));
        }
    } else {
        /* RGB path — use existing functions (unchanged) */
        clk->fb_div = CalcFbDiv(pixClk, pclkInt);
        if (clk->fb_div == 0) {
            HDF_LOGE("%s: calc fbdiv error", __func__);
            return DISPLAY_FAILURE;
        }
        clk->post_div1 = CalcPostDiv1(pclkInt);
        clk->post_div2 = CalcPostDiv2(pclkInt);
        clk->frac = CalcRgbFrac(pixClk, pclkInt, clk->fb_div);
        clk->ref_div = CalcRgbRefDiv(pclkInt);
    }

    HDF_LOGI("%s: pixClk = %f, pclkInt = %f, pclkFrac = %f", __func__, pixClk, pclkInt, pclkFrac);
    HDF_LOGI("%s: Fbdiv = %u, Frac = 0x%x, Refdiv = %u, Postdiv1 = %u, Postdiv2 = %u ", __func__,
        clk->fb_div, clk->frac, clk->ref_div, clk->post_div1, clk->post_div2);
    return DISPLAY_SUCCESS;
}
