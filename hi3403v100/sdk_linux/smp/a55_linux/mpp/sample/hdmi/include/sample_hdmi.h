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

#ifndef SAMPLE_HDMI_H
#define SAMPLE_HDMI_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define sample_if_failure_return_void(tmp)                                                     \
    do {                                                                                       \
        if ((tmp) != TD_SUCCESS) {                                                             \
            printf("\033[31;1m%s:[%d]:%s is failure!\033[0m\n", __FUNCTION__, __LINE__, #tmp); \
            return;                                                                            \
        }                                                                                      \
    } while (0)

#define sample_if_failure_return(tmp, ret)                                                     \
    do {                                                                                       \
        if ((tmp) != TD_SUCCESS) {                                                             \
            printf("\033[31;1m%s:[%d]:%s is failure!\033[0m\n", __FUNCTION__, __LINE__, #tmp); \
            return (ret);                                                                      \
        }                                                                                      \
    } while (0)

#define sample_prt(fmt...)                           \
    do {                                             \
        printf("[%s]-%d: ", __FUNCTION__, __LINE__); \
        printf(fmt);                                 \
    } while (0)

#define NUM_0                0
#define NUM_1                1
#define NUM_2                2
#define NUM_3                3
#define NUM_4                4
#define NUM_5                5
#define NUM_6                6
#define NUM_7                7
#define NUM_8                8
#define NUM_9                9
#define NUM_10               10
#define NUM_15               15
#define NUM_16               16
#define NUM_20               20
#define NUM_21               21
#define NUM_25               25
#define NUM_30               30
#define NUM_32               32
#define NUM_33               33
#define NUM_34               34
#define NUM_36               36
#define NUM_39               39
#define NUM_40               40
#define NUM_49               49
#define NUM_64               64
#define NUM_120              120
#define NUM_255              255
#define NUM_256              256
#define NUM_320              320
#define NUM_1000             1000
#define NUM_1024             1024
#define FMT_CIF_WIDTH        352
#define FMT_CIF_HEIGHT       288
#define FMT_576P_WIDTH       720
#define FMT_576P_HEIGHT      576
#define FMT_640_480_WIDTH    640
#define FMT_640_480_HEIGHT   480
#define FMT_800_600_WIDTH    800
#define FMT_800_600_HEIGHT   600
#define FMT_1366_768_WIDTH   1366
#define FMT_1366_768_HEIGHT  768
#define FMT_1440_900_WIDTH   1440
#define FMT_1440_900_HEIGHT  900
#define FMT_1280_800_WIDTH   1280
#define FMT_1280_800_HEIGHT  800
#define FMT_1920_2160_WIDTH  1920
#define FMT_1920_2160_HEIGHT 2160
#define FMT_2560_1600_WIDTH  2560
#define FMT_2560_1600_HEIGHT 1600
#define FMT_720_480_WIDTH    720
#define FMT_720_480_HEIGHT   480
#define FMT_1280_720_WIDTH   1280
#define FMT_1280_720_HEIGHT  720
#define FMT_1920_1080_WIDTH  1920
#define FMT_1920_1080_HEIGHT 1080
#define FMT_2592_1520_WIDTH  2592
#define FMT_2592_1520_HEIGHT 1520
#define FMT_2592_1944_WIDTH  2592
#define FMT_2592_1944_HEIGHT 1994
#define FMT_3840_2160_WIDTH  3840
#define FMT_3840_2160_HEIGHT 2160
#define FMT_3000_3000_WIDTH  3000
#define FMT_3000_3000_HEIGHT 3000
#define FMT_4000_3000_WIDTH  4000
#define FMT_4000_3000_HEIGHT 3000
#define FMT_4096_2160_WIDTH  4096
#define FMT_4096_2160_HEIGHT 2160
#define FMT_7680_4320_WIDTH  7680
#define FMT_7680_4320_HEIGHT 4320
#define FMT_3840_8640_WIDTH  3840
#define FMT_3840_8640_HEIGHT 8640
#define FMT_960H_PAL_WIDTH   960
#define FMT_960H_PAL_HEIGHT  576
#define FMT_960H_NTSC_WIDTH  960
#define FMT_960H_NTSC_HEIGHT 480
#define FMT_800_600_WIDTH    800
#define FMT_800_600_HEIGHT   600
#define FMT_1024_768_WIDTH   1024
#define FMT_1024_768_HEIGHT  768
#define FMT_1280_800_WIDTH   1280
#define FMT_1280_800_HEIGHT  800
#define FMT_1280_1024_WIDTH  1280
#define FMT_1280_1024_HEIGHT 1024
#define FMT_1400_1050_WIDTH  1440
#define FMT_1400_1050_HEIGHT 1050
#define FMT_1400_900_WIDTH   1440
#define FMT_1400_900_HEIGHT  900
#define FMT_1600_1200_WIDTH  1600
#define FMT_1600_1200_HEIGHT 1200
#define FMT_1680_1050_WIDTH  1680
#define FMT_1680_1050_HEIGHT 1050
#define FMT_1920_1200_WIDTH  1920
#define FMT_1920_1200_HEIGHT 1200
#define FMT_1920_2160_WIDTH  1920
#define FMT_1920_2160_HEIGHT 2160
#define FMT_2560_1440_WIDTH  2560
#define FMT_2560_1440_HEIGHT 1440
#define FMT_240_320_WIDTH    240
#define FMT_240_320_HEIGHT   320
#define FMT_320_240_WIDTH    320
#define FMT_320_240_HEIGHT   240
#define FMT_720_1280_WIDTH   720
#define FMT_720_1280_HEIGHT  1280
#define FMT_1082_1920_WIDTH  1080
#define FMT_1082_1920_HEIGHT 1920
#define FMT_720_576_WIDTH    720
#define FMT_720_576_HEIGHT   576
#define FRAME_RATE_24        24
#define FRAME_RATE_25        25
#define FRAME_RATE_30        30
#define FRAME_RATE_50        50
#define FRAME_RATE_60        60

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif

