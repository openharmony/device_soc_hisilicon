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
#include "ot_mpi_hdmi.h"
#include <string.h>
#include <malloc.h>
#include "drv_hdmi_ioctl.h"
#include "mpi_hdmi_com.h"
#include "ot_common.h"
#include "ot_common_hdmi.h"
#include "mkp_hdmi.h"

/* Constants for reported magic-number values. */
#define OT_MPI_HDMI_MFRS_NAME_INDEX_2          2
#define OT_MPI_HDMI_MFRS_NAME_INDEX_3          3

#define hdmi_check_eok_return(ret, err_code)                     \
    do {                                                         \
        if ((ret) != EOK) {                                      \
            hdmi_err_trace("secure function error:%d\n", (ret)); \
            return (err_code);                                   \
        }                                                        \
    } while (0)

#define hdmi_check_max_return_invalid(tmp, max)                   \
    do {                                                          \
        if ((tmp) > (max)) {                                      \
            hdmi_err_trace("%s error:%u\n", #tmp, (td_u32)(tmp)); \
            return OT_ERR_HDMI_INVALID_PARA;                      \
        }                                                         \
    } while (0)

#define hdmi_check_id_return_invalid(hdmi)                               \
    do {                                                                 \
        if ((hdmi) >= HDMI_MPI_ID_MAX) {                                 \
            hdmi_err_trace("Invalid HDMI DEV ID:%u!\n", (td_u32)(hdmi)); \
            return OT_ERR_HDMI_INVALID_PARA;                             \
        }                                                                \
    } while (0)

#define hdmi_check_failure_return(ret)             \
    do {                                           \
        if ((ret) != TD_SUCCESS) {                 \
            hdmi_err_trace("return 0x%x!\n", ret); \
            return ret;                            \
        }                                          \
    } while (0)

#define hdmi_check_boolval_return(val)                           \
    do {                                                         \
        if (((val) != TD_FALSE) && ((val) != TD_TRUE)) {         \
            hdmi_err_trace("Invalid BOOL type data:%u!\n", val); \
            return OT_ERR_HDMI_INVALID_PARA;                     \
        }                                                        \
    } while (0)

#define hdmi_ycc_quantization_user2drv(uyccquantization, kyccquantization) \
    do {                                                                   \
        if ((uyccquantization) == OT_HDMI_YCC_QUANT_LIMITED_RANGE) {       \
            kyccquantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;        \
        } else if ((uyccquantization) == OT_HDMI_YCC_QUANT_FULL_RANGE) {   \
            kyccquantization = HDMI_YCC_QUANTIZATION_RANGE_FULL;           \
        } else {                                                           \
            kyccquantization = HDMI_YCC_QUANTIZATION_RANGE_BUTT;           \
        }                                                                  \
    } while (0)

#define hdmi_ycc_quantization_drv2user(kyccquantization, uyccquantization)   \
    do {                                                                     \
        if ((kyccquantization) == HDMI_YCC_QUANTIZATION_RANGE_LIMITED) {     \
            uyccquantization = OT_HDMI_YCC_QUANT_LIMITED_RANGE;              \
        } else if ((kyccquantization) == HDMI_YCC_QUANTIZATION_RANGE_FULL) { \
            uyccquantization = OT_HDMI_YCC_QUANT_FULL_RANGE;                 \
        } else {                                                             \
            uyccquantization = OT_HDMI_YCC_QUANT_BUTT;                       \
        }                                                                    \
    } while (0)

#define UMAP_DEVNAME_HDMI         "hdmi"
#define MAX_DELAY_TIME_MS         10000
#define HDMI_FEILD_RATE_DEVIATION 50
#define HDMI_PIXL_FREQ_DEVIATION  100
#define HDMI_INVALID_PIXFREQ      0xffffffff
#define HDMI_VIC_PC_BASE          ((HDMI_VIC_VIRTUAL_BASE) + 0x100)
#define hdmi_array_size(a)        ((sizeof(a)) / (sizeof((a)[0])))
#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
#define MAX_HDMI_HW_PARAM         63
#else
#define MAX_HDMI_HW_PARAM         43
#endif
#define MAX_HDMI_FT_PARAM         4
#define REFRESH_RATE_MULTIPLE     100

typedef struct {
    td_u32 etablish_code;
    td_u32 format;
} hdmi_estab_format;

typedef struct {
    td_u32 vic_code;
    td_u32 pix_freq;   /* KHz */
    td_u32 hor_active;
    td_u32 ver_active;
    td_u32 field_rate; /* 0.01Hz */
    td_u32 format;
} hdmi_format_param;

typedef struct {
    ot_hdmi_video_format format;
    hdmi_video_timing timing;
} hdmi_format_to_timing;

typedef struct {
    ot_hdmi_sample_rate usr_sample_rate;
    hdmi_sample_rate drv_sample_rate;
} sample_rate_transform;

static td_u32 g_avi_video_format;

static const hdmi_estab_format g_format_estab_table[] = {
    { HDMI_EDID_ESTABTIMG_VESA_800X600_60,        OT_HDMI_VIDEO_FORMAT_VESA_800X600_60  },
    { HDMI_EDID_ESTABTIMG_VESA_800X600_56,        OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_640X480_75,        OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_640X480_72,        OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_IBM_VGA_640X480_67,     OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_IBM_XGA_640X480_60,     OT_HDMI_VIDEO_FORMAT_VESA_800X600_60  },
    { HDMI_EDID_ESTABTIMG_IBM_XGA2_720X400_88,    OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_IBM_XGA_720X400_70,     OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_1280X1024_75,      OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_1024X768_75,       OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_1024X768_70,       OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_1024X768_60,       OT_HDMI_VIDEO_FORMAT_VESA_1024X768_60 },
    { HDMI_EDID_ESTABTIMG_IBM_1024X768_87,        OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_APPLE_MACII_832X624_75, OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_800X600_75,        OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_800X600_72,        OT_HDMI_VIDEO_FORMAT_BUTT             },
    { HDMI_EDID_ESTABTIMG_VESA_1152X870_75,       OT_HDMI_VIDEO_FORMAT_BUTT             }
};

static const hdmi_format_param g_format_param_table[] = {
    /* VIC  PixFreq  HACT  VACT  FldRate           EncFmt */
    { 0,    106500, 1440,  900,  6000,  OT_HDMI_VIDEO_FORMAT_VESA_1440X900_60  },
    { 0,    121750, 1400, 1050,  6000,  OT_HDMI_VIDEO_FORMAT_VESA_1400X1050_60 },
    { 0,     83500, 1280,  800,  6000,  OT_HDMI_VIDEO_FORMAT_VESA_1280X800_60  },
    { 0,    146250, 1680, 1560,  6000,  OT_HDMI_VIDEO_FORMAT_VESA_1680X1050_60 },
    { 0,    162000, 1600, 1200,  6000,  OT_HDMI_VIDEO_FORMAT_VESA_1600X1200_60 },
    { 0,    268500, 2560, 1600,  6000,  OT_HDMI_VIDEO_FORMAT_2560x1600_60      },
    { 0,    119375, 2560, 1440,  3000,  OT_HDMI_VIDEO_FORMAT_2560x1440_30      },
    { 0,    238750, 2560, 1440,  6000,  OT_HDMI_VIDEO_FORMAT_2560x1440_60      },
    { 0,    148500, 1920, 2160,  3000,  OT_HDMI_VIDEO_FORMAT_1920x2160_30      },
    { 1,     25175,  640,  480,  6000,  OT_HDMI_VIDEO_FORMAT_861D_640X480_60   },
    { 2,     27000,  720,  480,  6000,  OT_HDMI_VIDEO_FORMAT_480P_60           },
    { 3,     27000,  720,  480,  6000,  OT_HDMI_VIDEO_FORMAT_480P_60           },
    { 4,     74250, 1280,  720,  6000,  OT_HDMI_VIDEO_FORMAT_720P_60           },
    { 5,     74250, 1920, 1080,  6000,  OT_HDMI_VIDEO_FORMAT_1080i_60          },
    { 6,     27000,  720,  480,  6000,  OT_HDMI_VIDEO_FORMAT_NTSC              },
    { 7,     27000,  720,  480,  6000,  OT_HDMI_VIDEO_FORMAT_NTSC              },
    { 16,   148500, 1920, 1080,  6000,  OT_HDMI_VIDEO_FORMAT_1080P_60          },
    { 17,    27000,  720,  576,  5000,  OT_HDMI_VIDEO_FORMAT_576P_50           },
    { 18,    27000,  720,  576,  5000,  OT_HDMI_VIDEO_FORMAT_576P_50           },
    { 19,    74250, 1280,  720,  5000,  OT_HDMI_VIDEO_FORMAT_720P_50           },
    { 20,    74250, 1920, 1080,  5000,  OT_HDMI_VIDEO_FORMAT_1080i_50          },
    { 21,    27000, 1440,  576,  5000,  OT_HDMI_VIDEO_FORMAT_PAL               },
    { 22,    27000, 1440,  576,  5000,  OT_HDMI_VIDEO_FORMAT_PAL               },
    { 31,   148500, 1920, 1080,  5000,  OT_HDMI_VIDEO_FORMAT_1080P_50          },
    { 32,    74250, 1920, 1080,  2400,  OT_HDMI_VIDEO_FORMAT_1080P_24          },
    { 33,    74250, 1920, 1080,  2500,  OT_HDMI_VIDEO_FORMAT_1080P_25          },
    { 34,    74250, 1920, 1080,  3000,  OT_HDMI_VIDEO_FORMAT_1080P_30          },
    { 93,   297000, 3840, 2160,  2400,  OT_HDMI_VIDEO_FORMAT_3840X2160P_24     },
    { 94,   297000, 3840, 2160,  2500,  OT_HDMI_VIDEO_FORMAT_3840X2160P_25     },
    { 95,   297000, 3840, 2160,  3000,  OT_HDMI_VIDEO_FORMAT_3840X2160P_30     },
    { 96,   594000, 3840, 2160,  5000,  OT_HDMI_VIDEO_FORMAT_3840X2160P_50     },
    { 97,   594000, 3840, 2160,  6000,  OT_HDMI_VIDEO_FORMAT_3840X2160P_60     },
    { 98,   297000, 4096, 2160,  2400,  OT_HDMI_VIDEO_FORMAT_4096X2160P_24     },
    { 99,   297000, 4096, 2160,  2500,  OT_HDMI_VIDEO_FORMAT_4096X2160P_25     },
    { 100,  297000, 4096, 2160,  3000,  OT_HDMI_VIDEO_FORMAT_4096X2160P_30     },
    { 101,  594000, 4096, 2160,  5000,  OT_HDMI_VIDEO_FORMAT_4096X2160P_50     },
    { 102,  594000, 4096, 2160,  6000,  OT_HDMI_VIDEO_FORMAT_4096X2160P_60     },
    { 103,  297000, 3840, 2160,  2400,  OT_HDMI_VIDEO_FORMAT_3840X2160P_24     },
    { 104,  297000, 3840, 2160,  2500,  OT_HDMI_VIDEO_FORMAT_3840X2160P_25     },
    { 105,  297000, 3840, 2160,  3000,  OT_HDMI_VIDEO_FORMAT_3840X2160P_30     },
    { 106,  594000, 3840, 2160,  5000,  OT_HDMI_VIDEO_FORMAT_3840X2160P_50     },
    { 107,  594000, 3840, 2160,  6000,  OT_HDMI_VIDEO_FORMAT_3840X2160P_60     },
    { 118, 1188000, 3840, 2160, 12000,  OT_HDMI_VIDEO_FORMAT_3840X2160P_120    },
    { 219, 1188000, 4096, 2160, 12000,  OT_HDMI_VIDEO_FORMAT_4096X2160P_120    },
#if defined(HDMI_PRODUCT_SS626V100)
    // The 8K30 timing in SS626V100 is non-standard timing, and the pixel clock is 594 MHz.
    { 196,  594000, 7680, 4320,  3000,  OT_HDMI_VIDEO_FORMAT_7680X4320P_30     },
#else
    { 196, 1188000, 7680, 4320,  3000,  OT_HDMI_VIDEO_FORMAT_7680X4320P_30     },
#endif
#if defined(DVI_SUPPORT)
    /* VIC  PixFreq  HACT  VACT  FldRate           EncFmt */
    { HDMI_VIC_PC_BASE +  9,  40000,  800,  600, 6000, OT_HDMI_VIDEO_FORMAT_VESA_800X600_60   },
    { HDMI_VIC_PC_BASE + 13,  65000, 1024,  768, 6000, OT_HDMI_VIDEO_FORMAT_VESA_1024X768_60  },
    { HDMI_VIC_PC_BASE + 18, 162000, 1600, 1200, 6000, OT_HDMI_VIDEO_FORMAT_VESA_1600X1200_60 },
    { HDMI_VIC_PC_BASE + 25, 108000, 1280, 1024, 6000, OT_HDMI_VIDEO_FORMAT_VESA_1280X1024_60 },
    { HDMI_VIC_PC_BASE + 28,  85500, 1360,  768, 6000, OT_HDMI_VIDEO_FORMAT_VESA_1366X768_60  },
    { HDMI_VIC_PC_BASE + 41, 154000, 1920, 1200, 6000, OT_HDMI_VIDEO_FORMAT_VESA_1920X1200_60 },
#endif
    /* for 2k & 4k, we use VIRTUAL VIC */
    { HDMI_VIC_VIRTUAL_BASE + 1, 297000, 3840, 2160, 3000, OT_HDMI_VIDEO_FORMAT_3840X2160P_30 },
    { HDMI_VIC_VIRTUAL_BASE + 2, 297000, 3840, 2160, 2500, OT_HDMI_VIDEO_FORMAT_3840X2160P_25 },
    { HDMI_VIC_VIRTUAL_BASE + 3, 297000, 3840, 2160, 2400, OT_HDMI_VIDEO_FORMAT_3840X2160P_24 },
    { HDMI_VIC_VIRTUAL_BASE + 4, 297000, 4096, 2160, 2400, OT_HDMI_VIDEO_FORMAT_4096X2160P_24 }
};

static hdmi_format_to_timing g_format_to_timing[] = {
    { OT_HDMI_VIDEO_FORMAT_1080P_60,             HDMI_VIDEO_TIMING_1920X1080P_60000  },
    { OT_HDMI_VIDEO_FORMAT_1080P_50,             HDMI_VIDEO_TIMING_1920X1080P_50000  },
    { OT_HDMI_VIDEO_FORMAT_1080P_30,             HDMI_VIDEO_TIMING_1920X1080P_30000  },
    { OT_HDMI_VIDEO_FORMAT_1080P_25,             HDMI_VIDEO_TIMING_1920X1080P_25000  },
    { OT_HDMI_VIDEO_FORMAT_1080P_24,             HDMI_VIDEO_TIMING_1920X1080P_24000  },
    { OT_HDMI_VIDEO_FORMAT_1080i_60,             HDMI_VIDEO_TIMING_1920X1080I_60000  },
    { OT_HDMI_VIDEO_FORMAT_1080i_50,             HDMI_VIDEO_TIMING_1920X1080I_50000  },
    { OT_HDMI_VIDEO_FORMAT_720P_60,              HDMI_VIDEO_TIMING_1280X720P_60000   },
    { OT_HDMI_VIDEO_FORMAT_720P_50,              HDMI_VIDEO_TIMING_1280X720P_50000   },
    { OT_HDMI_VIDEO_FORMAT_576P_50,              HDMI_VIDEO_TIMING_720X576P_50000    },
    { OT_HDMI_VIDEO_FORMAT_480P_60,              HDMI_VIDEO_TIMING_720X480P_60000    },
    { OT_HDMI_VIDEO_FORMAT_PAL,                  HDMI_VIDEO_TIMING_1440X576I_50000   },
    { OT_HDMI_VIDEO_FORMAT_NTSC,                 HDMI_VIDEO_TIMING_1440X480I_60000   },
    { OT_HDMI_VIDEO_FORMAT_861D_640X480_60,      HDMI_VIDEO_TIMING_640X480P_60000    },
    { OT_HDMI_VIDEO_FORMAT_VESA_800X600_60,      HDMI_VIDEO_TIMING_VESA_800X600_60   },
    { OT_HDMI_VIDEO_FORMAT_VESA_1024X768_60,     HDMI_VIDEO_TIMING_VESA_1024X768_60  },
    { OT_HDMI_VIDEO_FORMAT_VESA_1280X800_60,     HDMI_VIDEO_TIMING_VESA_1280X800_60  },
    { OT_HDMI_VIDEO_FORMAT_VESA_1280X1024_60,    HDMI_VIDEO_TIMING_VESA_1280X1024_60 },
    { OT_HDMI_VIDEO_FORMAT_VESA_1366X768_60,     HDMI_VIDEO_TIMING_USER_1366X768_60  },
    { OT_HDMI_VIDEO_FORMAT_VESA_1440X900_60,     HDMI_VIDEO_TIMING_VESA_1440X900_60  },
    { OT_HDMI_VIDEO_FORMAT_VESA_1400X1050_60,    HDMI_VIDEO_TIMING_VESA_1400X1050_60 },
    { OT_HDMI_VIDEO_FORMAT_VESA_1600X1200_60,    HDMI_VIDEO_TIMING_VESA_1600X1200_60 },
    { OT_HDMI_VIDEO_FORMAT_VESA_1680X1050_60,    HDMI_VIDEO_TIMING_VESA_1680X1050_60 },
    { OT_HDMI_VIDEO_FORMAT_VESA_1920X1200_60,    HDMI_VIDEO_TIMING_VESA_1920X1200_60 },
    { OT_HDMI_VIDEO_FORMAT_2560x1440_30,         HDMI_VIDEO_TIMING_USER_2560X1440_30 },
    { OT_HDMI_VIDEO_FORMAT_2560x1440_60,         HDMI_VIDEO_TIMING_USER_2560X1440_60 },
    { OT_HDMI_VIDEO_FORMAT_2560x1600_60,         HDMI_VIDEO_TIMING_VESA_2560X1600_60 },
    { OT_HDMI_VIDEO_FORMAT_1920x2160_30,         HDMI_VIDEO_TIMING_USER_1920X2160_30 },
    { OT_HDMI_VIDEO_FORMAT_3840X2160P_24,        HDMI_VIDEO_TIMING_3840X2160P_24000  },
    { OT_HDMI_VIDEO_FORMAT_3840X2160P_25,        HDMI_VIDEO_TIMING_3840X2160P_25000  },
    { OT_HDMI_VIDEO_FORMAT_3840X2160P_30,        HDMI_VIDEO_TIMING_3840X2160P_30000  },
    { OT_HDMI_VIDEO_FORMAT_3840X2160P_50,        HDMI_VIDEO_TIMING_3840X2160P_50000  },
    { OT_HDMI_VIDEO_FORMAT_3840X2160P_60,        HDMI_VIDEO_TIMING_3840X2160P_60000  },
    { OT_HDMI_VIDEO_FORMAT_4096X2160P_24,        HDMI_VIDEO_TIMING_4096X2160P_24000  },
    { OT_HDMI_VIDEO_FORMAT_4096X2160P_25,        HDMI_VIDEO_TIMING_4096X2160P_25000  },
    { OT_HDMI_VIDEO_FORMAT_4096X2160P_30,        HDMI_VIDEO_TIMING_4096X2160P_30000  },
    { OT_HDMI_VIDEO_FORMAT_4096X2160P_50,        HDMI_VIDEO_TIMING_4096X2160P_50000  },
    { OT_HDMI_VIDEO_FORMAT_4096X2160P_60,        HDMI_VIDEO_TIMING_4096X2160P_60000  },
    { OT_HDMI_VIDEO_FORMAT_3840X2160P_120,       HDMI_VIDEO_TIMING_3840X2160P_120000 },
    { OT_HDMI_VIDEO_FORMAT_4096X2160P_120,       HDMI_VIDEO_TIMING_4096X2160P_120000 },
    { OT_HDMI_VIDEO_FORMAT_7680X4320P_30,        HDMI_VIDEO_TIMING_7680X4320P_30000  },
    { OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE, HDMI_VIDEO_TIMING_USER_DEFINE       }
};

static sample_rate_transform g_sample_rate_transform_table[] = {
    { OT_HDMI_SAMPLE_RATE_UNKNOWN, HDMI_SAMPLE_RATE_UNKNOWN },
    { OT_HDMI_SAMPLE_RATE_8K, HDMI_SAMPLE_RATE_8K           },
    { OT_HDMI_SAMPLE_RATE_11K, HDMI_SAMPLE_RATE_11K         },
    { OT_HDMI_SAMPLE_RATE_12K, HDMI_SAMPLE_RATE_12K         },
    { OT_HDMI_SAMPLE_RATE_16K, HDMI_SAMPLE_RATE_16K         },
    { OT_HDMI_SAMPLE_RATE_22K, HDMI_SAMPLE_RATE_22K         },
    { OT_HDMI_SAMPLE_RATE_24K, HDMI_SAMPLE_RATE_24K         },
    { OT_HDMI_SAMPLE_RATE_32K, HDMI_SAMPLE_RATE_32K         },
    { OT_HDMI_SAMPLE_RATE_44K, HDMI_SAMPLE_RATE_44K         },
    { OT_HDMI_SAMPLE_RATE_48K, HDMI_SAMPLE_RATE_48K         },
    { OT_HDMI_SAMPLE_RATE_88K, HDMI_SAMPLE_RATE_88K         },
    { OT_HDMI_SAMPLE_RATE_96K, HDMI_SAMPLE_RATE_96K         },
    { OT_HDMI_SAMPLE_RATE_176K, HDMI_SAMPLE_RATE_176K       },
    { OT_HDMI_SAMPLE_RATE_192K, HDMI_SAMPLE_RATE_192K       }
};

static hdmi_extended_colormetry ext_colorimetry_user2drv(ot_hdmi_ex_colorimetry user_ext_colorimetry)
{
    hdmi_extended_colormetry kernel_ext_colorimetry;

    switch (user_ext_colorimetry) {
        case OT_HDMI_COMMON_COLORIMETRY_XVYCC_601:
            kernel_ext_colorimetry = HDMI_EXTENDED_COLORIMETRY_XV_YCC_601;
            break;
        case OT_HDMI_COMMON_COLORIMETRY_XVYCC_709:
            kernel_ext_colorimetry = HDMI_EXTENDED_COLORIMETRY_XV_YCC_709;
            break;
        case OT_HDMI_COMMON_COLORIMETRY_S_YCC_601:
            kernel_ext_colorimetry = HDMI_EXTENDED_COLORIMETRY_S_YCC_601;
            break;
        case OT_HDMI_COMMON_COLORIMETRY_ADOBE_YCC_601:
            kernel_ext_colorimetry = HDMI_EXTENDED_COLORIMETRY_ADOBE_YCC_601;
            break;
        case OT_HDMI_COMMON_COLORIMETRY_ADOBE_RGB:
            kernel_ext_colorimetry = HDMI_EXTENDED_COLORIMETRY_ADOBE_RGB;
            break;
        case OT_HDMI_COMMON_COLORIMETRY_2020_CONST_LUMINOUS:
            kernel_ext_colorimetry = HDMI_EXTENDED_COLORIMETRY_2020_CONST_LUMINOUS;
            break;
        case OT_HDMI_COMMON_COLORIMETRY_2020_NON_CONST_LUMINOUS:
            kernel_ext_colorimetry = HDMI_EXTENDED_COLORIMETRY_2020_NON_CONST_LUMINOUS;
            break;
        default:
            kernel_ext_colorimetry = HDMI_EXTENDED_COLORIMETRY_BUTT;
            break;
    }

    return kernel_ext_colorimetry;
}

static ot_hdmi_ex_colorimetry ext_colorimetry_drv2user(hdmi_extended_colormetry kext_colorimetry)
{
    ot_hdmi_ex_colorimetry user_ext_colorimetry;

    switch (kext_colorimetry) {
        case HDMI_EXTENDED_COLORIMETRY_XV_YCC_601:
            user_ext_colorimetry = OT_HDMI_COMMON_COLORIMETRY_XVYCC_601;
            break;
        case HDMI_EXTENDED_COLORIMETRY_XV_YCC_709:
            user_ext_colorimetry = OT_HDMI_COMMON_COLORIMETRY_XVYCC_709;
            break;
        case HDMI_EXTENDED_COLORIMETRY_S_YCC_601:
            user_ext_colorimetry = OT_HDMI_COMMON_COLORIMETRY_S_YCC_601;
            break;
        case HDMI_EXTENDED_COLORIMETRY_ADOBE_YCC_601:
            user_ext_colorimetry = OT_HDMI_COMMON_COLORIMETRY_ADOBE_YCC_601;
            break;
        case HDMI_EXTENDED_COLORIMETRY_ADOBE_RGB:
            user_ext_colorimetry = OT_HDMI_COMMON_COLORIMETRY_ADOBE_RGB;
            break;
        case HDMI_EXTENDED_COLORIMETRY_2020_CONST_LUMINOUS:
            user_ext_colorimetry = OT_HDMI_COMMON_COLORIMETRY_2020_CONST_LUMINOUS;
            break;
        case HDMI_EXTENDED_COLORIMETRY_2020_NON_CONST_LUMINOUS:
            user_ext_colorimetry = OT_HDMI_COMMON_COLORIMETRY_2020_NON_CONST_LUMINOUS;
            break;
        default:
            user_ext_colorimetry = OT_HDMI_COMMON_COLORIMETRY_EXT_BUTT;
            break;
    }

    return user_ext_colorimetry;
}

static hdmi_colorspace color_space_user2drv(ot_hdmi_color_space ucolorspace)
{
    hdmi_colorspace kcolorspace;

    switch (ucolorspace) {
        case OT_HDMI_COLOR_SPACE_RGB444:
            kcolorspace = HDMI_COLORSPACE_RGB;
            break;
        case OT_HDMI_COLOR_SPACE_YCBCR422:
            kcolorspace = HDMI_COLORSPACE_YCBCR422;
            break;
        case OT_HDMI_COLOR_SPACE_YCBCR444:
            kcolorspace = HDMI_COLORSPACE_YCBCR444;
            break;
        case OT_HDMI_COLOR_SPACE_YCBCR420:
            kcolorspace = HDMI_COLORSPACE_YCBCR420;
            break;
        default:
            kcolorspace = HDMI_COLORSPACE_BUTT;
            break;
    }

    return kcolorspace;
}

static ot_hdmi_color_space color_space_drv2user(hdmi_colorspace kcolorspace)
{
    ot_hdmi_color_space ucolorspace;

    switch (kcolorspace) {
        case HDMI_COLORSPACE_YCBCR422:
            ucolorspace = OT_HDMI_COLOR_SPACE_YCBCR422;
            break;
        case HDMI_COLORSPACE_YCBCR444:
            ucolorspace = OT_HDMI_COLOR_SPACE_YCBCR444;
            break;
        case HDMI_COLORSPACE_YCBCR420:
            ucolorspace = OT_HDMI_COLOR_SPACE_YCBCR420;
            break;
        case HDMI_COLORSPACE_RGB:
        default:
            ucolorspace = OT_HDMI_COLOR_SPACE_RGB444;
            break;
    }

    return ucolorspace;
}

static hdmi_active_aspect act_aspect_user2drv(ot_hdmi_active_aspect_ratio user_active_aspect)
{
    hdmi_active_aspect kernel_active_aspect;

    switch (user_active_aspect) {
        case OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_TOP:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_16_9_TOP;
            break;
        case OT_HDMI_ACTIVE_ASPECT_RATIO_14TO9_TOP:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_14_9_TOP;
            break;
        case OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_BOX_CENTER:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_16_9_CENTER;
            break;
        case OT_HDMI_ACTIVE_ASPECT_RATIO_SAME_PIC:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_PICTURE;
            break;
        case OT_HDMI_ACTIVE_ASPECT_RATIO_4TO3_CENTER:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_4_3;
            break;
        case OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_CENTER:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_16_9;
            break;
        case OT_HDMI_ACTIVE_ASPECT_RATIO_14TO9_CENTER:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_14_9;
            break;
        case OT_HDMI_ACTIVE_ASPECT_RATIO_4TO3_14_9:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_4_3_SP_14_9;
            break;
        case OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_14_9:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_16_9_SP_14_9;
            break;
        case OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_4_3:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_16_9_SP_4_3;
            break;
        default:
            kernel_active_aspect = HDMI_ACTIVE_ASPECT_BUTT;
            break;
    }

    return kernel_active_aspect;
}

static ot_hdmi_active_aspect_ratio active_aspect_drv2user(hdmi_active_aspect kernel_active_aspect)
{
    ot_hdmi_active_aspect_ratio user_active_aspect;

    switch (kernel_active_aspect) {
        case HDMI_ACTIVE_ASPECT_16_9_TOP:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_TOP;
            break;
        case HDMI_ACTIVE_ASPECT_14_9_TOP:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_14TO9_TOP;
            break;
        case HDMI_ACTIVE_ASPECT_16_9_CENTER:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_BOX_CENTER;
            break;
        case HDMI_ACTIVE_ASPECT_PICTURE:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_SAME_PIC;
            break;
        case HDMI_ACTIVE_ASPECT_4_3:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_4TO3_CENTER;
            break;
        case HDMI_ACTIVE_ASPECT_16_9:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_CENTER;
            break;
        case HDMI_ACTIVE_ASPECT_14_9:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_14TO9_CENTER;
            break;
        case HDMI_ACTIVE_ASPECT_4_3_SP_14_9:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_4TO3_14_9;
            break;
        case HDMI_ACTIVE_ASPECT_16_9_SP_14_9:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_14_9;
            break;
        case HDMI_ACTIVE_ASPECT_16_9_SP_4_3:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_4_3;
            break;
        default:
            user_active_aspect = OT_HDMI_ACTIVE_ASPECT_RATIO_BUTT;
            break;
    }

    return user_active_aspect;
}

static enum hdmi_scan_mode scan_mode_user2drv(ot_hdmi_scan_info user_scan_info)
{
    enum hdmi_scan_mode kernel_scan_mode;

    switch (user_scan_info) {
        case OT_HDMI_SCAN_INFO_NO_DATA:
            kernel_scan_mode = HDMI_SCAN_MODE_NONE;
            break;
        case OT_HDMI_SCAN_INFO_OVERSCANNED:
            kernel_scan_mode = HDMI_SCAN_MODE_OVERSCAN;
            break;
        case OT_HDMI_SCAN_INFO_UNDERSCANNED:
            kernel_scan_mode = HDMI_SCAN_MODE_UNDERSCAN;
            break;
        default:
            kernel_scan_mode = HDMI_SCAN_MODE_BUTT;
            break;
    }

    return kernel_scan_mode;
}

static ot_hdmi_scan_info scan_mode_drv2user(enum hdmi_scan_mode kernel_scan_mode)
{
    ot_hdmi_scan_info uscaninfo;

    switch (kernel_scan_mode) {
        case HDMI_SCAN_MODE_NONE:
            uscaninfo = OT_HDMI_SCAN_INFO_NO_DATA;
            break;
        case HDMI_SCAN_MODE_OVERSCAN:
            uscaninfo = OT_HDMI_SCAN_INFO_OVERSCANNED;
            break;
        case HDMI_SCAN_MODE_UNDERSCAN:
            uscaninfo = OT_HDMI_SCAN_INFO_UNDERSCANNED;
            break;
        default:
            uscaninfo = OT_HDMI_SCAN_INFO_BUTT;
            break;
    }

    return uscaninfo;
}

static ot_hdmi_rgb_quant_range rgb_quantization_drv2user(hdmi_quantization_range kernel_rgb_quantization)
{
    ot_hdmi_rgb_quant_range user_rgb_quantization;

    switch (kernel_rgb_quantization) {
        case HDMI_QUANTIZATION_RANGE_DEFAULT:
            user_rgb_quantization = OT_HDMI_RGB_QUANT_DEFAULT_RANGE;
            break;
        case HDMI_QUANTIZATION_RANGE_LIMITED:
            user_rgb_quantization = OT_HDMI_RGB_QUANT_LIMITED_RANGE;
            break;
        case HDMI_QUANTIZATION_RANGE_FULL:
            user_rgb_quantization = OT_HDMI_RGB_QUANT_FULL_RANGE;
            break;
        default:
            user_rgb_quantization = OT_HDMI_RGB_QUANT_FULL_BUTT;
            break;
    }

    return user_rgb_quantization;
}

static ot_hdmi_coding_type coding_type_drv2user(enum hdmi_audio_code kernel_coding_type)
{
    ot_hdmi_coding_type user_coding_type;

    if (kernel_coding_type >= HDMI_AUDIO_CODING_TYPE_RESERVED) {
        user_coding_type = OT_HDMI_AUDIO_CODING_BUTT;
    } else {
        user_coding_type = (ot_hdmi_coding_type)kernel_coding_type;
    }

    return user_coding_type;
}

static enum hdmi_audio_code coding_type_user2drv(ot_hdmi_coding_type user_coding_type)
{
    enum hdmi_audio_code kernel_coding_type;

    if (user_coding_type > OT_HDMI_AUDIO_CODING_BUTT) {
        kernel_coding_type = HDMI_AUDIO_CODING_TYPE_BUTT;
    } else {
        kernel_coding_type = (enum hdmi_audio_code)user_coding_type;
    }

    return kernel_coding_type;
}

static ot_pic_aspect_ratio aspect_ratio_drv2user(hdmi_picture_aspect kernel_aspect_ratio)
{
    ot_pic_aspect_ratio  user_aspect_ratio;

    switch (kernel_aspect_ratio) {
        case HDMI_PICTURE_ASPECT_NONE:
            user_aspect_ratio = OT_HDMI_PIC_ASPECT_RATIO_NO_DATA;
            break;
        case HDMI_PICTURE_ASPECT_4_3:
            user_aspect_ratio = OT_HDMI_PIC_ASPECT_RATIO_4TO3;
            break;
        case HDMI_PICTURE_ASPECT_16_9:
            user_aspect_ratio = OT_HDMI_PIC_ASPECT_RATIO_16TO9;
            break;
        case HDMI_PICTURE_ASPECT_64_27:
            user_aspect_ratio = OT_HDMI_PIC_ASPECT_RATIO_64TO27;
            break;
        case HDMI_PICTURE_ASPECT_256_135:
            user_aspect_ratio = OT_HDMI_PIC_ASPECT_RATIO_256TO135;
            break;
        default:
            user_aspect_ratio = OT_HDMI_PIC_ASPECT_RATIO_BUTT;
            break;
    }

    return user_aspect_ratio;
}

static hdmi_picture_aspect aspect_ratio_user2drv(ot_pic_aspect_ratio user_aspect_ratio)
{
    hdmi_picture_aspect kernel_aspect_ratio;

    switch (user_aspect_ratio) {
        case OT_HDMI_PIC_ASPECT_RATIO_NO_DATA:
            kernel_aspect_ratio = HDMI_PICTURE_ASPECT_NONE;
            break;
        case OT_HDMI_PIC_ASPECT_RATIO_4TO3:
            kernel_aspect_ratio = HDMI_PICTURE_ASPECT_4_3;
            break;
        case OT_HDMI_PIC_ASPECT_RATIO_16TO9:
            kernel_aspect_ratio = HDMI_PICTURE_ASPECT_16_9;
            break;
        case OT_HDMI_PIC_ASPECT_RATIO_64TO27:
            kernel_aspect_ratio = HDMI_PICTURE_ASPECT_64_27;
            break;
        case OT_HDMI_PIC_ASPECT_RATIO_256TO135:
            kernel_aspect_ratio = HDMI_PICTURE_ASPECT_256_135;
            break;
        default:
            kernel_aspect_ratio = HDMI_PICTURE_ASPECT_FUTURE;
            break;
    }

    return kernel_aspect_ratio;
}

static ot_hdmi_audio_format_code aud_fmt_code_drv2user(hdmi_audio_format_code kernel_aud_fmt_code)
{
    ot_hdmi_audio_format_code user_aud_fmt_code;

    if (kernel_aud_fmt_code > HDMI_AUDIO_CODING_TYPE_WMA_PRO) {
        user_aud_fmt_code = OT_HDMI_AUDIO_FORMAT_CODE_BUTT;
    } else {
        user_aud_fmt_code = (ot_hdmi_audio_format_code)kernel_aud_fmt_code;
    }

    return user_aud_fmt_code;
}

static hdmi_sample_rate sample_rate_user2drv(ot_hdmi_sample_rate user_sample_rate)
{
    td_u32 i;

    for (i = 0; i < hdmi_array_size(g_sample_rate_transform_table); i++) {
        if (user_sample_rate == g_sample_rate_transform_table[i].usr_sample_rate) {
            return g_sample_rate_transform_table[i].drv_sample_rate;
        }
    }

    return HDMI_SAMPLE_RATE_UNKNOWN;
}

static ot_hdmi_sample_rate sample_rate_drv2user(hdmi_sample_rate kernel_sample_rate)
{
    td_u32 i;

    for (i = 0; i < hdmi_array_size(g_sample_rate_transform_table); i++) {
        if (kernel_sample_rate == g_sample_rate_transform_table[i].drv_sample_rate) {
            return g_sample_rate_transform_table[i].usr_sample_rate;
        }
    }

    return OT_HDMI_SAMPLE_RATE_BUTT;
}

static hdmi_audio_bit_depth bit_depth_user2drv(ot_hdmi_bit_depth user_sample_depth)
{
    hdmi_audio_bit_depth kernel_sample_depth;

    switch (user_sample_depth) {
        case OT_HDMI_BIT_DEPTH_UNKNOWN:
            kernel_sample_depth = HDMI_AUDIO_BIT_DEPTH_UNKNOWN;
            break;
        case OT_HDMI_BIT_DEPTH_8:
            kernel_sample_depth = HDMI_AUDIO_BIT_DEPTH_8;
            break;
        case OT_HDMI_BIT_DEPTH_16:
            kernel_sample_depth = HDMI_AUDIO_BIT_DEPTH_16;
            break;
        case OT_HDMI_BIT_DEPTH_18:
            kernel_sample_depth = HDMI_AUDIO_BIT_DEPTH_18;
            break;
        case OT_HDMI_BIT_DEPTH_20:
            kernel_sample_depth = HDMI_AUDIO_BIT_DEPTH_20;
            break;
        case OT_HDMI_BIT_DEPTH_24:
            kernel_sample_depth = HDMI_AUDIO_BIT_DEPTH_24;
            break;
        case OT_HDMI_BIT_DEPTH_32:
            kernel_sample_depth = HDMI_AUDIO_BIT_DEPTH_32;
            break;
        case OT_HDMI_BIT_DEPTH_BUTT:
            kernel_sample_depth = HDMI_AUDIO_BIT_DEPTH_BUTT;
            break;
        default:
            kernel_sample_depth = HDMI_AUDIO_BIT_DEPTH_BUTT;
            break;
    }

    return kernel_sample_depth;
}

static ot_hdmi_bit_depth bit_depth_drv2user(hdmi_audio_bit_depth kernel_sample_depth)
{
    ot_hdmi_bit_depth user_sample_depth;

    switch (kernel_sample_depth) {
        case HDMI_AUDIO_BIT_DEPTH_UNKNOWN:
            user_sample_depth = OT_HDMI_BIT_DEPTH_UNKNOWN;
            break;
        case HDMI_AUDIO_BIT_DEPTH_8:
            user_sample_depth = OT_HDMI_BIT_DEPTH_8;
            break;
        case HDMI_AUDIO_BIT_DEPTH_16:
            user_sample_depth = OT_HDMI_BIT_DEPTH_16;
            break;
        case HDMI_AUDIO_BIT_DEPTH_18:
            user_sample_depth = OT_HDMI_BIT_DEPTH_18;
            break;
        case HDMI_AUDIO_BIT_DEPTH_20:
            user_sample_depth = OT_HDMI_BIT_DEPTH_20;
            break;
        case HDMI_AUDIO_BIT_DEPTH_24:
            user_sample_depth = OT_HDMI_BIT_DEPTH_24;
            break;
        case HDMI_AUDIO_BIT_DEPTH_32:
            user_sample_depth = OT_HDMI_BIT_DEPTH_32;
            break;
        case HDMI_AUDIO_BIT_DEPTH_BUTT:
            user_sample_depth = OT_HDMI_BIT_DEPTH_BUTT;
            break;
        default:
            user_sample_depth = OT_HDMI_BIT_DEPTH_BUTT;
            break;
    }

    return user_sample_depth;
}

static td_u32 hdmi_etablish_to_format(td_u32 etablish_code)
{
    td_u32 i;
    hdmi_estab_format *estab_timing = TD_NULL;

    for (i = 0; i < hdmi_array_size(g_format_estab_table); i++) {
        estab_timing = (hdmi_estab_format *)&g_format_estab_table[i];
        if (estab_timing != TD_NULL && estab_timing->etablish_code == etablish_code) {
            return estab_timing->format;
        }
    }

    return OT_HDMI_VIDEO_FORMAT_BUTT;
}

static td_u32 hdmi_vic_to_format(td_u32 vic_code)
{
    td_u32 i;
    hdmi_format_param *format_param = TD_NULL;

    for (i = 0; i < hdmi_array_size(g_format_param_table); i++) {
        format_param = (hdmi_format_param *)&g_format_param_table[i];
        if ((format_param != TD_NULL) && (format_param->vic_code == vic_code)) {
            return format_param->format;
        }
    }

    return OT_HDMI_VIDEO_FORMAT_BUTT;
}

static td_u32 hdmi_std_timing_to_format(const hdmi_edid_std_timing *std_timing)
{
    td_u32 i;
    hdmi_format_param *format_param = TD_NULL;

    for (i = 0; i < hdmi_array_size(g_format_param_table); i++) {
        format_param = (hdmi_format_param *)&g_format_param_table[i];
        if ((format_param != TD_NULL) &&
            (format_param->hor_active == std_timing->hor_active) &&
            (format_param->ver_active == std_timing->ver_active) &&
            (format_param->field_rate >= std_timing->refresh_rate * REFRESH_RATE_MULTIPLE) &&
            ((format_param->field_rate - HDMI_FEILD_RATE_DEVIATION) <=
            (std_timing->refresh_rate * REFRESH_RATE_MULTIPLE))) {
            return format_param->format;
        }
    }

    return OT_HDMI_VIDEO_FORMAT_BUTT;
}

static td_u32 hdmi_pre_timing_to_format(const hdmi_edid_pre_timing *pre_timing)
{
    td_u32 i;
    hdmi_format_param *format_param = TD_NULL;

    for (i = 0; i < hdmi_array_size(g_format_param_table); i++) {
        format_param = (hdmi_format_param *)&g_format_param_table[i];
        if ((format_param != TD_NULL) &&
            (format_param->hor_active == pre_timing->hact) &&
            (format_param->ver_active == pre_timing->vact) &&
            (format_param->pix_freq >= pre_timing->pixel_clk) &&
            ((format_param->pix_freq - HDMI_PIXL_FREQ_DEVIATION) <= pre_timing->pixel_clk)) {
            return format_param->format;
        }
    }

    return OT_HDMI_VIDEO_FORMAT_BUTT;
}

static hdmi_video_timing hdmi_format_to_hdmi_timing(td_u32 format)
{
    td_u32 i;

    for (i = 0; i < hdmi_array_size(g_format_to_timing); i++) {
        if (format == g_format_to_timing[i].format) {
            return g_format_to_timing[i].timing;
        }
    }
    hdmi_info_trace("Error video fmt:%u\n", format);

    return HDMI_VIDEO_TIMING_UNKNOWN;
}

static ot_hdmi_video_format hdmi_timing_to_format(td_u32 timing)
{
    td_u32 i;

    for (i = 0; i < hdmi_array_size(g_format_to_timing); i++) {
        if (timing == g_format_to_timing[i].timing) {
            return g_format_to_timing[i].format;
        }
    }
    hdmi_info_trace("Error video timing:%u\n", timing);

    return OT_HDMI_VIDEO_FORMAT_BUTT;
}

static td_s32 hdmi_avi_active_aspect_check(ot_hdmi_active_aspect_ratio act_aspect)
{
    if (act_aspect >= OT_HDMI_ACTIVE_ASPECT_RATIO_BUTT || act_aspect < OT_HDMI_ACTIVE_ASPECT_RATIO_16TO9_TOP) {
        return OT_ERR_HDMI_INVALID_PARA;
    }
    return TD_SUCCESS;
}

static td_s32 hdmi_avi_param_check(const ot_hdmi_avi_infoframe *user_avi_infoframe)
{
#ifdef OT_HDMI_SUPPORT_1_4
    td_bool support;

    support = (user_avi_infoframe->timing_mode == OT_HDMI_VIDEO_FORMAT_3840X2160P_50 ||
               user_avi_infoframe->timing_mode == OT_HDMI_VIDEO_FORMAT_3840X2160P_60 ||
               user_avi_infoframe->timing_mode == OT_HDMI_VIDEO_FORMAT_4096X2160P_50 ||
               user_avi_infoframe->timing_mode == OT_HDMI_VIDEO_FORMAT_4096X2160P_60);

    if (support) {
        hdmi_warn_trace("Param is invalid, This chip is not support HDMI2.0\n");
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }
    if (user_avi_infoframe->color_space == OT_HDMI_COLOR_SPACE_YCBCR420) {
        hdmi_warn_trace("Param is invalid, This chip is not support YCBCR420 \n");
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }
#endif
    hdmi_check_max_return_invalid(user_avi_infoframe->timing_mode, OT_HDMI_VIDEO_FORMAT_7680X4320P_30);
    hdmi_check_max_return_invalid(user_avi_infoframe->color_space, OT_HDMI_COLOR_SPACE_YCBCR420);

    if (user_avi_infoframe->active_info_present != TD_FALSE && user_avi_infoframe->active_info_present != TD_TRUE) {
        hdmi_err_trace("active_infor_present error:%u\n", user_avi_infoframe->active_info_present);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    hdmi_check_max_return_invalid(user_avi_infoframe->bar_info, OT_HDMI_BAR_INFO_VH);
    hdmi_check_max_return_invalid(user_avi_infoframe->scan_info, (OT_HDMI_SCAN_INFO_BUTT - 1));
    hdmi_check_max_return_invalid(user_avi_infoframe->colorimetry, OT_HDMI_COMMON_COLORIMETRY_BUTT);
    hdmi_check_max_return_invalid(user_avi_infoframe->ex_colorimetry,
        OT_HDMI_COMMON_COLORIMETRY_2020_NON_CONST_LUMINOUS);
    hdmi_check_max_return_invalid(user_avi_infoframe->aspect_ratio, OT_HDMI_PIC_ASPECT_RATIO_256TO135);

    if (hdmi_avi_active_aspect_check(user_avi_infoframe->active_aspect_ratio) != TD_SUCCESS) {
        hdmi_err_trace("active_aspect_ratio error:%u\n", user_avi_infoframe->active_aspect_ratio);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    hdmi_check_max_return_invalid(user_avi_infoframe->pic_scaling, OT_HDMI_PIC_SCALING_HV);
    hdmi_check_max_return_invalid(user_avi_infoframe->rgb_quant, OT_HDMI_RGB_QUANT_FULL_RANGE);
    if (user_avi_infoframe->is_it_content != TD_FALSE && user_avi_infoframe->is_it_content != TD_TRUE) {
        hdmi_err_trace("is_it_content error:%u\n", user_avi_infoframe->is_it_content);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    hdmi_check_max_return_invalid(user_avi_infoframe->pixel_repetition, OT_HDMI_PIXEL_REPET_10_TIMES);
    hdmi_check_max_return_invalid(user_avi_infoframe->content_type, OT_HDMI_CONTNET_GAME);
    hdmi_check_max_return_invalid(user_avi_infoframe->ycc_quant, OT_HDMI_YCC_QUANT_FULL_RANGE);

    return TD_SUCCESS;
}

static td_s32 hdmi_audio_speaker_param_check(const ot_hdmi_audio_infoframe *user_audio_infoframe)
{
    td_s32 ret;

    if (user_audio_infoframe->chn_alloc > 0x1f) { /* 0x1f, channel_alloc max effective size for CEA861-D */
        hdmi_err_trace("channel_alloc error:%u, should in [0, 31]\n", user_audio_infoframe->chn_alloc);
        ret = OT_ERR_HDMI_INVALID_PARA;
        goto err;
    }
    if (user_audio_infoframe->level_shift >= OT_HDMI_LEVEL_SHIFT_VAL_BUTT) {
        hdmi_err_trace("level_shift error:%u, should in [%d, %d)\n", user_audio_infoframe->level_shift,
            OT_HDMI_LEVEL_SHIFT_VAL_0_DB, OT_HDMI_LEVEL_SHIFT_VAL_BUTT);
        ret = OT_ERR_HDMI_INVALID_PARA;
        goto err;
    }
    if (user_audio_infoframe->lfe_playback_level >= OT_HDMI_LFE_PLAYBACK_BUTT) {
        hdmi_err_trace("lfe_playback_level error:%u, shold in [%d, %d)\n", user_audio_infoframe->lfe_playback_level,
            OT_HDMI_LFE_PLAYBACK_NO, OT_HDMI_LFE_PLAYBACK_BUTT);
        ret = OT_ERR_HDMI_INVALID_PARA;
        goto err;
    }
    if (user_audio_infoframe->down_mix_inhibit != TD_FALSE && user_audio_infoframe->down_mix_inhibit != TD_TRUE) {
        hdmi_err_trace("down_mix_inhibit error:%u\n", user_audio_infoframe->down_mix_inhibit);
        ret = OT_ERR_HDMI_INVALID_PARA;
        goto err;
    }

    ret = TD_SUCCESS;

err:
    return ret;
}

static td_s32 hdmi_audio_infoframe_param_check(const ot_hdmi_audio_infoframe *user_audio_infoframe)
{
    td_s32 ret;

    if (user_audio_infoframe->chn_cnt > OT_HDMI_AUDIO_CHN_CNT_8) {
        hdmi_err_trace("chn_cnt error:%u should in [%d, %d)\n", user_audio_infoframe->chn_cnt,
            OT_HDMI_AUDIO_CHN_CNT_STREAM, OT_HDMI_AUDIO_CHN_CNT_BUTT);
        ret = OT_ERR_HDMI_INVALID_PARA;
        goto err;
    }
    if (user_audio_infoframe->coding_type > (OT_HDMI_AUDIO_CODING_BUTT - 1)) {
        hdmi_err_trace("coding_type error:%u, should in [%d, %d)\n", user_audio_infoframe->coding_type,
            OT_HDMI_AUDIO_CODING_REFER_STREAM_HEAD, OT_HDMI_AUDIO_CODING_WMA_PRO);
        ret = OT_ERR_HDMI_INVALID_PARA;
        goto err;
    }
    if (user_audio_infoframe->sample_size > OT_HDMI_AUDIO_SAMPLE_SIZE_24) {
        hdmi_err_trace("sample_size error:%u, should in [%d, %d]\n", user_audio_infoframe->sample_size,
            OT_HDMI_AUDIO_SAMPLE_SIZE_STREAM, OT_HDMI_AUDIO_SAMPLE_SIZE_24);
        ret = OT_ERR_HDMI_INVALID_PARA;
        goto err;
    }
    if (user_audio_infoframe->sampling_freq > OT_HDMI_AUDIO_SAMPLE_FREQ_192000) {
        hdmi_err_trace("sampling_frequency error:%u, should in [%d, %d]\n", user_audio_infoframe->sampling_freq,
            OT_HDMI_AUDIO_SAMPLE_FREQ_STREAM, OT_HDMI_AUDIO_SAMPLE_FREQ_192000);
        ret = OT_ERR_HDMI_INVALID_PARA;
        goto err;
    }

    ret = hdmi_audio_speaker_param_check(user_audio_infoframe);

err:
    return ret;
}

static td_void hdmi_infoframe_drv_to_user_avi(ot_hdmi_infoframe *dest_infoframe,
    const drv_hdmi_infoframe *src_infoframe)
{
    ot_hdmi_avi_infoframe *user_avi_infoframe = &dest_infoframe->infoframe_unit.avi_infoframe;
    const hdmi_avi_infoframe *drv_avi_infoframe = &src_infoframe->infoframe.avi_infoframe;
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    dest_infoframe->infoframe_type = OT_INFOFRAME_TYPE_AVI;
    g_avi_video_format = hdmi_timing_to_format(drv_avi_infoframe->video_timing);
    user_avi_infoframe->timing_mode = g_avi_video_format;
    user_avi_infoframe->color_space = color_space_drv2user(drv_avi_infoframe->colorspace);
    user_avi_infoframe->active_info_present = drv_avi_infoframe->active_info_valid;
    user_avi_infoframe->scan_info           = scan_mode_drv2user(drv_avi_infoframe->scan_mode);
    user_avi_infoframe->colorimetry         = drv_avi_infoframe->colorimetry;
    user_avi_infoframe->ex_colorimetry      = ext_colorimetry_drv2user(drv_avi_infoframe->extended_colorimetry);
    user_avi_infoframe->aspect_ratio        = aspect_ratio_drv2user(drv_avi_infoframe->picture_aspect);
    user_avi_infoframe->active_aspect_ratio = active_aspect_drv2user(drv_avi_infoframe->active_aspect);
    user_avi_infoframe->pic_scaling         = drv_avi_infoframe->nups;
    user_avi_infoframe->rgb_quant           = rgb_quantization_drv2user(drv_avi_infoframe->quantization_range);
    user_avi_infoframe->is_it_content       = drv_avi_infoframe->itc;
    user_avi_infoframe->content_type        = drv_avi_infoframe->content_type;
    hdmi_ycc_quantization_drv2user(drv_avi_infoframe->ycc_quantization_range, user_avi_infoframe->ycc_quant);
    user_avi_infoframe->pixel_repetition           = drv_avi_infoframe->pixel_repeat;
    user_avi_infoframe->pixel_repetition          -= 1;
    user_avi_infoframe->line_n_end_of_top_bar      = drv_avi_infoframe->top_bar;
    user_avi_infoframe->line_n_start_of_bot_bar    = drv_avi_infoframe->bottom_bar;
    user_avi_infoframe->pixel_n_end_of_left_bar    = drv_avi_infoframe->left_bar;
    user_avi_infoframe->pixel_n_start_of_right_bar = drv_avi_infoframe->right_bar;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    if (drv_avi_infoframe->horizontal_bar_valid && drv_avi_infoframe->vertical_bar_valid) {
        user_avi_infoframe->bar_info = OT_HDMI_BAR_INFO_VH;
    } else if (drv_avi_infoframe->horizontal_bar_valid) {
        user_avi_infoframe->bar_info = OT_HDMI_BAR_INFO_H;
    } else if (drv_avi_infoframe->vertical_bar_valid) {
        user_avi_infoframe->bar_info = OT_HDMI_BAR_INFO_V;
    } else {
        user_avi_infoframe->bar_info = OT_HDMI_BAR_INFO_NOT_VALID;
    }

    return;
}

static td_s32 hdmi_infoframe_drv_to_user(ot_hdmi_infoframe *dest_infoframe, const drv_hdmi_infoframe *src_infoframe)
{
    errno_t rc;

    switch (src_infoframe->infoframe_id) {
        case HDMI_INFOFRAME_TYPE_AVI:
            hdmi_infoframe_drv_to_user_avi(dest_infoframe, src_infoframe);
            break;
        case HDMI_INFOFRAME_TYPE_AUDIO: {
            ot_hdmi_audio_infoframe *user_audio_infoframe = &dest_infoframe->infoframe_unit.audio_infoframe;
            const hdmi_audio_infoframe *drv_audio_infoframe = &src_infoframe->infoframe.audio_infoframe;
            dest_infoframe->infoframe_type = OT_INFOFRAME_TYPE_AUDIO;
            user_audio_infoframe->chn_cnt            = drv_audio_infoframe->channels;
            user_audio_infoframe->coding_type        = coding_type_drv2user(drv_audio_infoframe->coding_type);
            user_audio_infoframe->sample_size        = (ot_hdmi_audio_sample_size)drv_audio_infoframe->sample_size;
            user_audio_infoframe->sampling_freq      = (ot_hdmi_audio_sample_freq)drv_audio_infoframe->sample_frequency;
            user_audio_infoframe->chn_alloc          = drv_audio_infoframe->channel_allocation;
            user_audio_infoframe->level_shift        = drv_audio_infoframe->level_shift_value;
            user_audio_infoframe->lfe_playback_level = \
                (ot_hdmi_lfe_playback_level)drv_audio_infoframe->lfe_playback_level;
            user_audio_infoframe->down_mix_inhibit   = drv_audio_infoframe->downmix_inhibit;
            break;
        }
        case HDMI_INFOFRAME_TYPE_VENDOR:{
            ot_hdmi_vendorspec_infoframe *des_vendor = &dest_infoframe->infoframe_unit.vendor_spec_infoframe;
            const hdmi_vendor_infoframe *src_vendor = &src_infoframe->infoframe.vendor_infoframe;

            dest_infoframe->infoframe_type = OT_INFOFRAME_TYPE_VENDORSPEC;
            des_vendor->data_len = src_vendor->vsif_content.cea861_vsif.len;
            rc = memcpy_s(des_vendor->user_data, OT_HDMI_VENDOR_USER_DATA_MAX_LEN,
                src_vendor->vsif_content.cea861_vsif.data, des_vendor->data_len);
            if (rc != EOK) {
                hdmi_err_trace("memcpy_s run error! rc=%d\n", rc);
                return OT_ERR_HDMI_INVALID_PARA;
            }
            break;
        }
        case HDMI_INFOFRAME_TYPE_SPD:
        case HDMI_INFOFRAME_TYPE_MPEG:
        case HDMI_INFOFRAME_TYPE_GBD:
        case HDMI_INFOFRAME_TYPE_DRM:
            hdmi_warn_trace("not support infoframe type:%x\n", src_infoframe->infoframe_id);
            return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
        default:
            hdmi_warn_trace("invalid infoframe type:%x\n", src_infoframe->infoframe_id);
            return OT_ERR_HDMI_INVALID_PARA;
    }

    return TD_SUCCESS;
}

static td_s32 hdmi_infoframe_user2drv_avi(drv_hdmi_infoframe *dest_infoframe, const ot_hdmi_infoframe *src_infoframe)
{
    td_s32 ret;

    const ot_hdmi_avi_infoframe *user_avi_infoframe = &src_infoframe->infoframe_unit.avi_infoframe;
    hdmi_avi_infoframe *drv_avi_infoframe = &dest_infoframe->infoframe.avi_infoframe;

    ret = hdmi_avi_param_check(user_avi_infoframe);
    hdmi_check_failure_return(ret);
    dest_infoframe->infoframe_id = HDMI_INFOFRAME_TYPE_AVI;
    g_avi_video_format = user_avi_infoframe->timing_mode;
    drv_avi_infoframe->video_timing      = hdmi_format_to_hdmi_timing(g_avi_video_format);
    drv_avi_infoframe->colorspace        = color_space_user2drv(user_avi_infoframe->color_space);
    drv_avi_infoframe->active_info_valid = user_avi_infoframe->active_info_present;
    drv_avi_infoframe->scan_mode         = scan_mode_user2drv(user_avi_infoframe->scan_info);
    drv_avi_infoframe->colorimetry       = (enum hdmi_colorimetry)user_avi_infoframe->colorimetry;
    drv_avi_infoframe->extended_colorimetry = ext_colorimetry_user2drv(user_avi_infoframe->ex_colorimetry);
    drv_avi_infoframe->picture_aspect       = aspect_ratio_user2drv(user_avi_infoframe->aspect_ratio);
    drv_avi_infoframe->active_aspect        = act_aspect_user2drv(user_avi_infoframe->active_aspect_ratio);
    drv_avi_infoframe->nups                 = (enum hdmi_nups)user_avi_infoframe->pic_scaling;
    drv_avi_infoframe->quantization_range   = (hdmi_quantization_range)user_avi_infoframe->rgb_quant;
    drv_avi_infoframe->itc                  = user_avi_infoframe->is_it_content;
    drv_avi_infoframe->pixel_repeat         = user_avi_infoframe->pixel_repetition;
    drv_avi_infoframe->pixel_repeat        += 1;
    drv_avi_infoframe->content_type         = (enum hdmi_content_type)user_avi_infoframe->content_type;
    hdmi_ycc_quantization_user2drv(user_avi_infoframe->ycc_quant, drv_avi_infoframe->ycc_quantization_range);
    drv_avi_infoframe->top_bar    = user_avi_infoframe->line_n_end_of_top_bar;
    drv_avi_infoframe->bottom_bar = user_avi_infoframe->line_n_start_of_bot_bar;
    drv_avi_infoframe->left_bar   = user_avi_infoframe->pixel_n_end_of_left_bar;
    drv_avi_infoframe->right_bar  = user_avi_infoframe->pixel_n_start_of_right_bar;
    switch (user_avi_infoframe->bar_info) {
        case OT_HDMI_BAR_INFO_VH:
            drv_avi_infoframe->horizontal_bar_valid = TD_TRUE;
            drv_avi_infoframe->vertical_bar_valid = TD_TRUE;
            break;
        case OT_HDMI_BAR_INFO_V:
            drv_avi_infoframe->vertical_bar_valid = TD_TRUE;
            break;
        case OT_HDMI_BAR_INFO_H:
            drv_avi_infoframe->horizontal_bar_valid = TD_TRUE;
            break;
        default:
            drv_avi_infoframe->horizontal_bar_valid = TD_FALSE;
            drv_avi_infoframe->vertical_bar_valid = TD_FALSE;
            break;
    }

    return TD_SUCCESS;
}

static td_s32 hdmi_infoframe_user2drv_audio(drv_hdmi_infoframe *dest_infoframe, const ot_hdmi_infoframe *src_infoframe)
{
    td_s32 ret;

    const ot_hdmi_audio_infoframe *user_audio_infoframe = &src_infoframe->infoframe_unit.audio_infoframe;
    hdmi_audio_infoframe *drv_audio_infoframe = &dest_infoframe->infoframe.audio_infoframe;
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    ret = hdmi_audio_infoframe_param_check(user_audio_infoframe);
    hdmi_check_failure_return(ret);
    dest_infoframe->infoframe_id            = HDMI_INFOFRAME_TYPE_AUDIO;
    drv_audio_infoframe->channels           = user_audio_infoframe->chn_cnt;
    drv_audio_infoframe->coding_type        = coding_type_user2drv(user_audio_infoframe->coding_type);
    drv_audio_infoframe->sample_size        = user_audio_infoframe->sample_size;
    drv_audio_infoframe->sample_frequency   = user_audio_infoframe->sampling_freq;
    drv_audio_infoframe->channel_allocation = user_audio_infoframe->chn_alloc;
    drv_audio_infoframe->level_shift_value  = user_audio_infoframe->level_shift;
    drv_audio_infoframe->lfe_playback_level = user_audio_infoframe->lfe_playback_level;
    drv_audio_infoframe->downmix_inhibit    = user_audio_infoframe->down_mix_inhibit;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    return TD_SUCCESS;
}

static td_s32 hdmi_infoframe_user2drv(drv_hdmi_infoframe *dest_infoframe, const ot_hdmi_infoframe *src_infoframe)
{
    errno_t rc;
    td_s32 ret;

    switch (src_infoframe->infoframe_type) {
        case OT_INFOFRAME_TYPE_AVI:
            ret = hdmi_infoframe_user2drv_avi(dest_infoframe, src_infoframe);
            if (ret != TD_SUCCESS) {
                hdmi_err_trace("return 0x%x!\n", ret);
                return ret;
            }
            break;
        case OT_INFOFRAME_TYPE_AUDIO:
            ret = hdmi_infoframe_user2drv_audio(dest_infoframe, src_infoframe);
            if (ret != TD_SUCCESS) {
                hdmi_err_trace("return 0x%x!\n", ret);
                return ret;
            }
            break;
        case OT_INFOFRAME_TYPE_VENDORSPEC: {
            td_u8 *src_data = TD_NULL;
            td_u8 *dest_data = TD_NULL;
            hdmi_vendor_infoframe *drv_infoframe  = &dest_infoframe->infoframe.vendor_infoframe;
            const ot_hdmi_vendorspec_infoframe *user_infoframe = &src_infoframe->infoframe_unit.vendor_spec_infoframe;

            if (user_infoframe->data_len > OT_HDMI_VENDOR_USER_DATA_MAX_LEN) {
                hdmi_err_trace("data_len:%u is invalid! should in [0, %d]\n", user_infoframe->data_len,
                    OT_HDMI_VENDOR_USER_DATA_MAX_LEN);
                return OT_ERR_HDMI_INVALID_PARA;
            }
            dest_infoframe->infoframe_id = HDMI_INFOFRAME_TYPE_VENDOR;
            drv_infoframe->vsif_content.cea861_vsif.len = user_infoframe->data_len;
            src_data  = (td_u8 *)&user_infoframe->user_data[0];
            dest_data = (td_u8 *)&drv_infoframe->vsif_content.cea861_vsif.data[0];
            /* copy user data for vendor infoframe */
            rc = memcpy_s(dest_data, HDMI_VENDOR_USER_DATA_MAX_LEN, src_data, user_infoframe->data_len);
            if (rc != EOK) {
                hdmi_err_trace("memcpy_s run error! rc=%d\n", rc);
                return OT_ERR_HDMI_INVALID_PARA;
            }
            break;
        }
        default:
            hdmi_warn_trace("infoframe type:%u invalid! should in [%d, %d)\n", src_infoframe->infoframe_type,
                OT_INFOFRAME_TYPE_AVI, OT_INFOFRAME_TYPE_BUTT);
            return OT_ERR_HDMI_INVALID_PARA;
    }

    return TD_SUCCESS;
}

static td_void hdmi_capability_drv2user_fmt(ot_hdmi_sink_capability *user_cap, const hdmi_sink_capability *drv_cap)
{
    td_u32 i;
    ot_hdmi_video_format tmp_format;

    for (i = 0; i < drv_cap->estab_num; i++) {
        tmp_format = hdmi_etablish_to_format(drv_cap->estab_timing[i]);
        if (tmp_format != OT_HDMI_VIDEO_FORMAT_BUTT) {
            user_cap->support_video_format[tmp_format] = TD_TRUE;
        }
    }
    for (i = 0; i < HDMI_EDID_MAX_STDTIMNG_COUNT; i++) {
        tmp_format = hdmi_std_timing_to_format(&drv_cap->std_timing[i]);
        if (tmp_format != OT_HDMI_VIDEO_FORMAT_BUTT) {
            user_cap->support_video_format[tmp_format] = TD_TRUE;
        }
    }
    for (i = 0; i < drv_cap->perfer_timing_num; i++) {
        tmp_format = hdmi_pre_timing_to_format(&drv_cap->perfer_timing[i]);
        if (tmp_format != OT_HDMI_VIDEO_FORMAT_BUTT) {
            user_cap->support_video_format[tmp_format] = TD_TRUE;
        }
    }
    for (i = 0; i < drv_cap->support_vic_num; i++) {
        tmp_format = hdmi_vic_to_format(drv_cap->support_format[i]);
        if (tmp_format != OT_HDMI_VIDEO_FORMAT_BUTT) {
            user_cap->support_video_format[tmp_format] = TD_TRUE;
        }
    }

    return;
}

static td_void hdmi_capability_drv2user_audio(ot_hdmi_sink_capability *user_cap, const hdmi_sink_capability *drv_cap)
{
    td_u32 i;
    td_u32 j;

    for (i = 0; i < OT_HDMI_MAX_AUDIO_CAPBILITY_CNT; i++) {
        user_cap->audio_info[i].audio_format_code = aud_fmt_code_drv2user(drv_cap->audio_info[i].aud_fmt_code);
        user_cap->audio_info[i].audio_chn = drv_cap->audio_info[i].aud_channel;
        user_cap->audio_info[i].support_bit_depth_num = drv_cap->audio_info[i].support_bit_depth_num;
        user_cap->audio_info[i].max_bit_rate = drv_cap->audio_info[i].max_bit_rate;
        for (j = 0; j < OT_HDMI_MAX_SAMPLE_RATE_NUM; j++) {
            user_cap->audio_info[i].support_sample_rate[j] =
                sample_rate_drv2user(drv_cap->audio_info[i].support_sample_rate[j]);
        }
        for (j = 0; j < OT_HDMI_MAX_BIT_DEPTH_NUM; j++) {
            user_cap->audio_info[i].support_bit_depth[j] =
                bit_depth_drv2user(drv_cap->audio_info[i].support_bit_depth[j]);
        }
    }
    user_cap->audio_info_num = drv_cap->audio_info_num;
    for (i = 0; i < OT_HDMI_AUDIO_SPEAKER_BUTT; i++) {
        user_cap->speaker[i] = drv_cap->support_audio_speaker[i];
    }

    return;
}

static td_void hdmi_capability_drv2user_base(ot_hdmi_sink_capability *user_cap, const hdmi_sink_capability *drv_cap)
{
    user_cap->manufacture_name[0]        = drv_cap->mfrs_info.mfrs_name[0];
    user_cap->manufacture_name[1]        = drv_cap->mfrs_info.mfrs_name[1];
    user_cap->manufacture_name[OT_MPI_HDMI_MFRS_NAME_INDEX_2] =
        drv_cap->mfrs_info.mfrs_name[OT_MPI_HDMI_MFRS_NAME_INDEX_2];
    user_cap->manufacture_name[OT_MPI_HDMI_MFRS_NAME_INDEX_3] =
        drv_cap->mfrs_info.mfrs_name[OT_MPI_HDMI_MFRS_NAME_INDEX_3];
    user_cap->pdt_code                   = drv_cap->mfrs_info.product_code;
    user_cap->serial_num                 = drv_cap->mfrs_info.serial_number;
    user_cap->week_of_manufacture        = drv_cap->mfrs_info.week;
    user_cap->year_of_manufacture        = drv_cap->mfrs_info.year;
    user_cap->version                    = drv_cap->version;
    user_cap->revision                   = drv_cap->revision;
    user_cap->edid_ex_blk_num            = drv_cap->ext_block_num;
    user_cap->is_phys_addr_valid         = drv_cap->cec_addr.phy_addr_valid;
    user_cap->phys_addr_a                = drv_cap->cec_addr.phy_addr_a;
    user_cap->phys_addr_b                = drv_cap->cec_addr.phy_addr_b;
    user_cap->phys_addr_c                = drv_cap->cec_addr.phy_addr_c;
    user_cap->phys_addr_d                = drv_cap->cec_addr.phy_addr_d;
    user_cap->support_dvi_dual           = drv_cap->support3d_dual_view;
    user_cap->support_deepcolor_ycbcr444 = drv_cap->deep_color.deep_color_y444;
    user_cap->support_deepcolor_30bit    = drv_cap->deep_color.deep_color30_bit;
    user_cap->support_deepcolor_36bit    = drv_cap->deep_color.deep_color36_bit;
    user_cap->support_deepcolor_48bit    = drv_cap->deep_color.deep_color48_bit;
    user_cap->support_ai                 = drv_cap->supports_ai;
    user_cap->max_tmds_clk               = drv_cap->max_tmds_clock;
    user_cap->i_latency_fields_present   = drv_cap->i_latency_fields_present;
    user_cap->latency_fields_present     = drv_cap->latency_fields_present;
    user_cap->hdmi_video_present         = drv_cap->hdmi_video_present;
    user_cap->video_latency              = drv_cap->video_latency;
    user_cap->audio_latency              = drv_cap->audio_latency;
    user_cap->interlaced_video_latency   = drv_cap->interlaced_video_latency;
    user_cap->interlaced_audio_latency   = drv_cap->interlaced_audio_latency;
    user_cap->support_y420_dc_30bit      = drv_cap->deep_color_y420.deep_color30_bit;
    user_cap->support_y420_dc_36bit      = drv_cap->deep_color_y420.deep_color36_bit;
    user_cap->support_y420_dc_48bit      = drv_cap->deep_color_y420.deep_color48_bit;
    user_cap->support_hdmi_2_0           = drv_cap->support_hdmi_20;

    return;
}

static td_s32 hdmi_y420_format_drv2user(ot_hdmi_sink_capability *user_cap, const hdmi_sink_capability *drv_cap)
{
    td_u32 i;
    ot_hdmi_video_format tmp_format;

    (td_void)memset_s(user_cap->support_y420_format, sizeof(user_cap->support_y420_format),
        TD_FALSE, sizeof(user_cap->support_y420_format));
    for (i = 0; i < drv_cap->support_y420_vic_num; i++) {
        tmp_format = hdmi_vic_to_format(drv_cap->support_y420_format[i]);
        if (tmp_format != OT_HDMI_VIDEO_FORMAT_BUTT) {
            user_cap->support_y420_format[tmp_format] = TD_TRUE;
        }
    }

    (td_void)memset_s(user_cap->only_support_y420_format, sizeof(user_cap->only_support_y420_format),
        TD_FALSE, sizeof(user_cap->only_support_y420_format));
    for (i = 0; i < drv_cap->only_support_y420_vic_num; i++) {
        tmp_format = hdmi_vic_to_format(drv_cap->only_support_y420_format[i]);
        if (tmp_format != OT_HDMI_VIDEO_FORMAT_BUTT) {
            user_cap->only_support_y420_format[tmp_format] = TD_TRUE;
        }
    }

    return TD_SUCCESS;
}

static td_s32 hdmi_capability_drv2user(ot_hdmi_sink_capability *user_cap, const hdmi_sink_capability *drv_cap)
{
    td_u32 i;
    errno_t ret;

    user_cap->support_hdmi = drv_cap->support_hdmi;
    user_cap->native_video_format = hdmi_vic_to_format(drv_cap->native_format);

    (td_void)memset_s(user_cap->support_video_format, sizeof(user_cap->support_video_format),
        TD_FALSE, sizeof(user_cap->support_video_format));
    hdmi_capability_drv2user_fmt(user_cap, drv_cap);
    user_cap->support_ycbcr = (drv_cap->color_space.ycbcr422 || drv_cap->color_space.ycbcr444) ? TD_TRUE : TD_FALSE;
    user_cap->support_xvycc601 = drv_cap->color_metry.xvycc601;
    user_cap->support_xvycc709 = drv_cap->color_metry.xvycc709;
    user_cap->md_bit           = drv_cap->md_bit;
    hdmi_capability_drv2user_audio(user_cap, drv_cap);
    hdmi_capability_drv2user_base(user_cap, drv_cap);
    ret = hdmi_y420_format_drv2user(user_cap, drv_cap);
    hdmi_check_eok_return(ret, TD_FAILURE);
    user_cap->rgb_quant_selectable = drv_cap->rgb_qrange_selectable;
    user_cap->ycc_quant_selectable = drv_cap->ycc_qrange_selectable;
    user_cap->detailed_timing.detail_timing_num = (drv_cap->perfer_timing_num <= HDMI_EDID_MAX_DETAIL_TIMING_COUNT) ?
        drv_cap->perfer_timing_num : HDMI_EDID_MAX_DETAIL_TIMING_COUNT;
    for (i = 0; i < user_cap->detailed_timing.detail_timing_num; i++) {
        ret = memcpy_s(&(user_cap->detailed_timing.detail_timing[i]), sizeof(ot_hdmi_timing_info),
            &(drv_cap->perfer_timing[i]), sizeof(hdmi_edid_pre_timing));
        if (ret != EOK) {
            hdmi_err_trace("secure function error:%d\n", ret);
            return OT_ERR_HDMI_INVALID_PARA;
        }
    }

    return TD_SUCCESS;
}

static td_u32 hdmi_pixel_freq_search(td_u32 format)
{
    td_u32 i;
    hdmi_format_param *format_param = TD_NULL;

    if (format >= OT_HDMI_VIDEO_FORMAT_BUTT) {
        return HDMI_INVALID_PIXFREQ;
    }
    for (i = 0; i < hdmi_array_size(g_format_param_table); i++) {
        format_param = (hdmi_format_param *)&g_format_param_table[i];
        if ((format_param != TD_NULL) && (format_param->format == format)) {
            return format_param->pix_freq;
        }
    }
    hdmi_warn_trace("Pix freq search fail, invalid format=%u\n", format);

    return HDMI_INVALID_PIXFREQ;
}

static td_s32 dvi_caps_check(const ot_hdmi_attr *attr)
{
    if (attr->hdmi_en == TD_FALSE) {
        if (attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_24 ||
            attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_25 ||
            attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_30 ||
            attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_50 ||
            attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_60 ||
            attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_24 ||
            attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_25 ||
            attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_30 ||
            attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_50 ||
            attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_60) {
            hdmi_warn_trace("param is invalid, not support 4K timing in DVI mode.\n");
            return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
        }
    }

    return TD_SUCCESS;
}

static td_s32 hdmi20_caps_check(const ot_hdmi_attr *attr)
{
#ifdef OT_HDMI_SUPPORT_1_4
    if (attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_50 ||
        attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_60 ||
        attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_50 ||
        attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_60) {
        hdmi_warn_trace("Param is invalid, This chip is not support HDMI2.0\n");
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }
    if ((attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_24 ||
        attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_25 ||
        attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_30) &&
        (attr->deep_color_mode == OT_HDMI_DEEP_COLOR_30BIT ||
        attr->deep_color_mode == OT_HDMI_DEEP_COLOR_36BIT)) {
        hdmi_warn_trace("Param is invalid, This chip is not support HDMI2.0\n");
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }
#else
    ot_unused(attr);
#endif

    return TD_SUCCESS;
}

static td_s32 hdmi21_caps_check(const ot_hdmi_attr *attr)
{
#ifndef HDMI_SUPPORT_2_1
    td_bool is_4k;

#if defined(HDMI_PRODUCT_SS626V100)
    // SS626V100 supports 8K30 YUV420 8-bit TMDS output.
    if (attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_120 ||
        attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_120) {
        hdmi_warn_trace("Param is invalid, This chip is not support HDMI2.1\n");
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }
#else
    if (attr->video_format == OT_HDMI_VIDEO_FORMAT_7680X4320P_30 ||
        attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_120 ||
        attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_120) {
        hdmi_warn_trace("Param is invalid, This chip is not support HDMI2.1\n");
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }
#endif

    is_4k = (attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_50) ||
            (attr->video_format == OT_HDMI_VIDEO_FORMAT_3840X2160P_60) ||
            (attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_50) ||
            (attr->video_format == OT_HDMI_VIDEO_FORMAT_4096X2160P_60) ||
            (attr->video_format == OT_HDMI_VIDEO_FORMAT_7680X4320P_30);

    if (is_4k == TD_TRUE &&
        (attr->deep_color_mode == OT_HDMI_DEEP_COLOR_30BIT || attr->deep_color_mode == OT_HDMI_DEEP_COLOR_36BIT)) {
        hdmi_warn_trace("Param is invalid, This chip is not support.\n");
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }
#else
    ot_unused(attr);
#endif

    return TD_SUCCESS;
}

static td_s32 hdmi_audio_sample_rate_check(const ot_hdmi_attr *attr)
{
    td_bool support;

    support = ((attr->sample_rate != OT_HDMI_SAMPLE_RATE_UNKNOWN) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_8K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_11K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_12K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_16K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_22K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_24K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_32K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_44K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_48K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_88K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_96K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_176K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_192K) &&
               (attr->sample_rate != OT_HDMI_SAMPLE_RATE_768K));

    if (support == TD_TRUE) {
        hdmi_warn_trace("sample_rate(%u) is invalid\n", attr->sample_rate);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    if ((attr->sample_rate != OT_HDMI_SAMPLE_RATE_32K) &&
        (attr->sample_rate != OT_HDMI_SAMPLE_RATE_44K) &&
        (attr->sample_rate != OT_HDMI_SAMPLE_RATE_48K)) {
        hdmi_warn_trace("sample_rate:%u not support! should in [%d, %d]\n", attr->sample_rate, OT_HDMI_SAMPLE_RATE_32K,
            OT_HDMI_SAMPLE_RATE_48K);
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }

    return TD_SUCCESS;
}

static td_s32 hdmi_audio_attr_check(const ot_hdmi_attr *attr)
{
    td_s32 ret;
    td_bool support;

    ret = hdmi_audio_sample_rate_check(attr);
    hdmi_check_failure_return(ret);
    support = ((attr->bit_depth != OT_HDMI_BIT_DEPTH_UNKNOWN) &&
               (attr->bit_depth != OT_HDMI_BIT_DEPTH_8) &&
               (attr->bit_depth != OT_HDMI_BIT_DEPTH_16) &&
               (attr->bit_depth != OT_HDMI_BIT_DEPTH_18) &&
               (attr->bit_depth != OT_HDMI_BIT_DEPTH_20) &&
               (attr->bit_depth != OT_HDMI_BIT_DEPTH_24) &&
               (attr->bit_depth != OT_HDMI_BIT_DEPTH_32));

    if (support == TD_TRUE) {
        hdmi_warn_trace("audio bit_depth(%u) is invalid! should in [%d, %d)\n", attr->bit_depth,
            OT_HDMI_BIT_DEPTH_UNKNOWN, OT_HDMI_BIT_DEPTH_BUTT);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    if (attr->bit_depth != OT_HDMI_BIT_DEPTH_16) {
        hdmi_warn_trace("audio bit_depth(%u) is invalid! should be %d\n", attr->bit_depth, OT_HDMI_BIT_DEPTH_16);
        return OT_ERR_HDMI_FEATURE_NO_SUPPORT;
    }
    /* DVI mode */
    if (attr->hdmi_en == TD_FALSE) {
        if (attr->audio_en == TD_TRUE || attr->deep_color_mode != OT_HDMI_DEEP_COLOR_24BIT) {
            hdmi_warn_trace("DVI mode, no audio output, relevant parameters can be set up, but may not be effective\n");
        }
    }

    return TD_SUCCESS;
}

static td_s32 hdmi_attr_check_valid(const ot_hdmi_attr *attr)
{
    td_s32 ret;
    td_bool support;

    ret = dvi_caps_check(attr);
    hdmi_check_failure_return(ret);
    ret = hdmi20_caps_check(attr);
    hdmi_check_failure_return(ret);
    ret = hdmi21_caps_check(attr);
    hdmi_check_failure_return(ret);

    hdmi_check_boolval_return(attr->hdmi_en);
    hdmi_check_boolval_return(attr->audio_en);
    hdmi_check_boolval_return(attr->auth_mode_en);
    hdmi_check_boolval_return(attr->deep_color_adapt_en);

    if (attr->video_format >= OT_HDMI_VIDEO_FORMAT_BUTT) {
        hdmi_warn_trace("envideofmt:%u invalid! should in [%d, %d)\n", attr->video_format,
            OT_HDMI_VIDEO_FORMAT_1080P_60, OT_HDMI_VIDEO_FORMAT_BUTT);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    support = (attr->deep_color_mode != OT_HDMI_DEEP_COLOR_24BIT &&
               attr->deep_color_mode != OT_HDMI_DEEP_COLOR_30BIT &&
               attr->deep_color_mode != OT_HDMI_DEEP_COLOR_36BIT);
    if (support == TD_TRUE) {
        hdmi_warn_trace("deep_color_mode:%u invalid! should in [%d, %d]\n", attr->deep_color_mode,
            OT_HDMI_DEEP_COLOR_24BIT, OT_HDMI_DEEP_COLOR_BUTT);
        return OT_ERR_HDMI_INVALID_PARA;
    }
    /* AO */
    ret = hdmi_audio_attr_check(attr);
    hdmi_check_failure_return(ret);

    return TD_SUCCESS;
}

static td_s32 hdmi_hw_spec_check_valid(const ot_hdmi_hw_spec *hw_spec)
{
    td_u8 i;

    for (i = 0; i < OT_HDMI_HW_PARAM_NUM; i++) {
#if defined(HDMI_PRODUCT_SS928V100) || defined(HDMI_PRODUCT_SS626V100)
        if ((hw_spec->hw_param[i].i_de_main_clk > MAX_HDMI_HW_PARAM) ||
            (hw_spec->hw_param[i].i_de_main_data > MAX_HDMI_HW_PARAM) ||
            (hw_spec->hw_param[i].i_main_clk > MAX_HDMI_HW_PARAM) ||
            (hw_spec->hw_param[i].i_main_data > MAX_HDMI_HW_PARAM) ||
            (hw_spec->hw_param[i].i_main_clk == 0) ||
            (hw_spec->hw_param[i].i_main_data == 0)) {
            hdmi_err_trace("stage(%u),u32i_de_main_clk:%u u32i_de_main_data:%u u32i_main_clk:%u u32i_main_data:%u"
                "Valid Range:0-%u\n", i, hw_spec->hw_param[i].i_de_main_clk, hw_spec->hw_param[i].i_de_main_data,
                hw_spec->hw_param[i].i_main_clk, hw_spec->hw_param[i].i_main_data, MAX_HDMI_HW_PARAM);
            return OT_ERR_HDMI_INVALID_PARA;
        }
#else
        if ((hw_spec->hw_param[i].i_de_main_clk > MAX_HDMI_HW_PARAM) ||
            (hw_spec->hw_param[i].i_de_main_data > MAX_HDMI_HW_PARAM) ||
            (hw_spec->hw_param[i].i_main_clk > MAX_HDMI_HW_PARAM) ||
            (hw_spec->hw_param[i].i_main_data > MAX_HDMI_HW_PARAM) ||
            (hw_spec->hw_param[i].ft_cap_clk > MAX_HDMI_FT_PARAM) ||
            (hw_spec->hw_param[i].ft_cap_data > MAX_HDMI_FT_PARAM) ||
            (hw_spec->hw_param[i].i_main_clk == 0) ||
            (hw_spec->hw_param[i].i_main_data == 0)) {
            hdmi_err_trace("stage(%u),u32i_de_main_clk:%u u32i_de_main_data:%u u32i_main_clk:%u u32i_main_data:%u"
                "Valid Range:0-%u\n", i, hw_spec->hw_param[i].i_de_main_clk, hw_spec->hw_param[i].i_de_main_data,
                hw_spec->hw_param[i].i_main_clk, hw_spec->hw_param[i].i_main_data, MAX_HDMI_HW_PARAM);
            hdmi_err_trace("stage(%u),ft_cap_clk:%u ft_cap_data:%u Valid Range:0-%u\n",
                i, hw_spec->hw_param[i].ft_cap_clk, hw_spec->hw_param[i].ft_cap_data, MAX_HDMI_FT_PARAM);
            return OT_ERR_HDMI_INVALID_PARA;
        }
#endif
    }

    return TD_SUCCESS;
}

td_s32 ot_mpi_hdmi_init(td_void)
{
    return mpi_hdmi_com_init();
}

td_s32 ot_mpi_hdmi_deinit(td_void)
{
    return mpi_hdmi_com_deinit();
}

td_s32 ot_mpi_hdmi_open(ot_hdmi_id hdmi)
{
    hdmi_open open_para = {0};
    open_para.default_mode = HDMI_DEFAULT_ACTION_HDMI;
    hdmi_check_id_return_invalid(hdmi);
    return mpi_hdmi_com_open(hdmi, &open_para);
}

td_s32 ot_mpi_hdmi_close(ot_hdmi_id hdmi)
{
    hdmi_check_id_return_invalid(hdmi);
    return mpi_hdmi_com_close(hdmi);
}

td_s32 ot_mpi_hdmi_get_sink_capability(ot_hdmi_id hdmi, ot_hdmi_sink_capability *capability)
{
    td_s32 ret;
    ot_hdmi_status tmp_stat = {0};
    drv_hdmi_sink_capability *drv_cap = TD_NULL;

    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(capability, OT_ERR_HDMI_NULL_PTR);

    ret = mpi_hdmi_com_get_status(hdmi, &tmp_stat);
    hdmi_check_failure_return(ret);

    capability->is_connected     = tmp_stat.is_connected;
    capability->is_sink_power_on = tmp_stat.is_sink_power_on;
    if (capability->is_connected == TD_FALSE) {
        return OT_ERR_HDMI_DEV_NOT_CONNECT;
    }
    drv_cap = (drv_hdmi_sink_capability *)malloc(sizeof(drv_hdmi_sink_capability));
    if (drv_cap != TD_NULL) {
        (td_void)memset_s(drv_cap, sizeof(drv_hdmi_sink_capability), 0, sizeof(drv_hdmi_sink_capability));
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
        drv_cap->hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
        ret = mpi_hdmi_com_get_sink_capability(hdmi, drv_cap);
        if (ret != TD_SUCCESS) {
            if (ret != OT_ERR_HDMI_DEV_NOT_OPEN) {
                ret = OT_ERR_HDMI_READ_SINK_FAILED;
            }
        } else {
            ret = hdmi_capability_drv2user(capability, &drv_cap->cap);
        }
        free(drv_cap);
    } else {
        ret = OT_ERR_HDMI_MALLOC_FAILED;
    }

    return ret;
}

td_s32 ot_mpi_hdmi_set_attr(ot_hdmi_id hdmi, const ot_hdmi_attr *attr)
{
    td_s32 ret;
    drv_hdmi_property property = {0};

    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(attr, OT_ERR_HDMI_NULL_PTR);

    ret = hdmi_attr_check_valid(attr);
    hdmi_check_failure_return(ret);

    property.prop.enable_hdmi           = attr->hdmi_en;
    property.prop.enable_video          = TD_TRUE;
    property.prop.enable_audio          = attr->audio_en;
    property.prop.auth_mode             = attr->auth_mode_en;
    property.prop.enable_avi_infoframe  = TD_TRUE;
    property.prop.enable_aud_infoframe  = TD_TRUE;
    property.prop.enable_deep_clr_adapt = attr->deep_color_adapt_en;
    property.prop.video_timing          = hdmi_format_to_hdmi_timing(attr->video_format);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    property.prop.disp_fmt              = attr->video_format;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    property.prop.pix_clk = (attr->video_format == OT_HDMI_VIDEO_FORMAT_VESA_CUSTOMER_DEFINE) ?
        attr->pix_clk : hdmi_pixel_freq_search(attr->video_format);
    property.prop.deep_color_mode = deep_color_user2drv(attr->deep_color_mode);
    property.prop.sample_rate     = sample_rate_user2drv(attr->sample_rate);
    property.prop.bit_depth       = bit_depth_user2drv(attr->bit_depth);

    ret = mpi_hdmi_com_set_attr(hdmi, &property);

    return ret;
}

td_s32 ot_mpi_hdmi_get_attr(ot_hdmi_id hdmi, ot_hdmi_attr *attr)
{
    td_s32 ret;
    drv_hdmi_property property = {0};

    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(attr, OT_ERR_HDMI_NULL_PTR);

    ret = mpi_hdmi_com_get_attr(hdmi, &property);
    hdmi_check_failure_return(ret);
    attr->hdmi_en             = property.prop.enable_hdmi;
    attr->audio_en            = property.prop.enable_audio;
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    attr->video_format        = property.prop.disp_fmt;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    attr->pix_clk             = property.prop.pix_clk;
    attr->deep_color_adapt_en = property.prop.enable_deep_clr_adapt;
    attr->auth_mode_en        = property.prop.auth_mode;
    attr->sample_rate         = sample_rate_drv2user(property.prop.sample_rate);
    attr->deep_color_mode     = deep_color_drv2user(property.prop.deep_color_mode);
    attr->bit_depth           = bit_depth_drv2user(property.prop.bit_depth);

    return TD_SUCCESS;
}

td_s32 ot_mpi_hdmi_start(ot_hdmi_id hdmi)
{
    hdmi_check_id_return_invalid(hdmi);
    return mpi_hdmi_com_start(hdmi);
}

td_s32 ot_mpi_hdmi_stop(ot_hdmi_id hdmi)
{
    hdmi_check_id_return_invalid(hdmi);
    return mpi_hdmi_com_stop(hdmi);
}

td_s32 ot_mpi_hdmi_force_get_edid(ot_hdmi_id hdmi, ot_hdmi_edid *edid_data)
{
    td_s32 ret;

    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(edid_data, OT_ERR_HDMI_NULL_PTR);

    (td_void)memset_s(edid_data, sizeof(ot_hdmi_edid), 0, sizeof(ot_hdmi_edid));
    ret = mpi_hdmi_com_force_get_edid(hdmi, edid_data->edid, &edid_data->edid_len);
    edid_data->edid_valid = (ret == TD_SUCCESS) ? TD_TRUE : TD_FALSE;

    return ret;
}

td_s32 ot_mpi_hdmi_set_infoframe(ot_hdmi_id hdmi, const ot_hdmi_infoframe *infoframe)
{
    td_s32 ret;
    drv_hdmi_infoframe tmp = {0};

    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(infoframe, OT_ERR_HDMI_NULL_PTR);
    ret = hdmi_infoframe_user2drv(&tmp, infoframe);
    hdmi_check_failure_return(ret);

    return mpi_hdmi_com_set_infoframe(hdmi, &tmp);
}

td_s32 ot_mpi_hdmi_get_infoframe(ot_hdmi_id hdmi, ot_hdmi_infoframe_type infoframe_type, ot_hdmi_infoframe *infoframe)
{
    td_s32 ret;
    drv_hdmi_infoframe tmp = {0};

    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(infoframe, OT_ERR_HDMI_NULL_PTR);
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wenum-conversion"
#endif
    tmp.hdmi_id = hdmi;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    switch (infoframe_type) {
        case OT_INFOFRAME_TYPE_AVI:
            tmp.infoframe_id = HDMI_INFOFRAME_TYPE_AVI;
            break;
        case OT_INFOFRAME_TYPE_AUDIO:
            tmp.infoframe_id = HDMI_INFOFRAME_TYPE_AUDIO;
            break;
        case OT_INFOFRAME_TYPE_VENDORSPEC:
            tmp.infoframe_id = HDMI_INFOFRAME_TYPE_VENDOR;
            break;
        default:
            hdmi_warn_trace("infoframe type:%d invalid! should in [%d, %d)\n", infoframe_type, OT_INFOFRAME_TYPE_AVI,
                OT_INFOFRAME_TYPE_BUTT);
            return OT_ERR_HDMI_INVALID_PARA;
    }
    ret = mpi_hdmi_com_get_infoframe(hdmi, &tmp);
    hdmi_check_failure_return(ret);
    ret = hdmi_infoframe_drv_to_user(infoframe, &tmp);
    hdmi_check_failure_return(ret);

    return ret;
}

td_s32 ot_mpi_hdmi_register_callback(ot_hdmi_id hdmi, const ot_hdmi_callback_func *callback_func)
{
    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(callback_func, OT_ERR_HDMI_NULL_PTR);
    return mpi_hdmi_com_reg_callback_func(hdmi, callback_func);
}

td_s32 ot_mpi_hdmi_unregister_callback(ot_hdmi_id hdmi, const ot_hdmi_callback_func *callback_func)
{
    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(callback_func, OT_ERR_HDMI_NULL_PTR);
    return mpi_hdmi_com_unreg_callback_func(hdmi, callback_func);
}

td_s32 ot_mpi_hdmi_set_hw_spec(ot_hdmi_id hdmi, const ot_hdmi_hw_spec *hw_spec)
{
    td_s32 ret;

    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(hw_spec, OT_ERR_HDMI_NULL_PTR);

    ret = hdmi_hw_spec_check_valid(hw_spec);
    hdmi_check_failure_return(ret);

    ret = mpi_hdmi_com_set_hw_spec(hdmi, hw_spec);

    return ret;
}

td_s32 ot_mpi_hdmi_get_hw_spec(ot_hdmi_id hdmi, ot_hdmi_hw_spec *hw_spec)
{
    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(hw_spec, OT_ERR_HDMI_NULL_PTR);
    return mpi_hdmi_com_get_hw_spec(hdmi, hw_spec);
}

td_s32 ot_mpi_hdmi_set_avmute(ot_hdmi_id hdmi, td_bool avmute_en)
{
    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_boolval_return(avmute_en);
    return mpi_hdmi_com_set_avmute(hdmi, avmute_en);
}

td_s32 ot_mpi_hdmi_set_mod_param(ot_hdmi_id hdmi, const ot_hdmi_mod_param *mod_param)
{
    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(mod_param, OT_ERR_HDMI_NULL_PTR);

    if (mod_param->emi_en != TD_TRUE && mod_param->emi_en != TD_FALSE) {
        hdmi_err_trace("emi_en:%d invalid! should in [%d, %d]\n", mod_param->emi_en, TD_FALSE, TD_TRUE);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    if (mod_param->trace_len >= OT_HDMI_TRACE_BUTT) {
        hdmi_err_trace("trace_len:%d invalid! should in [%d, %d)\n", mod_param->trace_len, OT_HDMI_TRACE_LEN_0,
            OT_HDMI_TRACE_BUTT);
        return OT_ERR_HDMI_INVALID_PARA;
    }

    return mpi_hdmi_com_mod_param_set(hdmi, mod_param);
}

td_s32 ot_mpi_hdmi_get_mod_param(ot_hdmi_id hdmi, ot_hdmi_mod_param *mod_param)
{
    hdmi_check_id_return_invalid(hdmi);
    hdmi_check_null_return(mod_param, OT_ERR_HDMI_NULL_PTR);
    return mpi_hdmi_com_mod_param_get(hdmi, mod_param);
}

