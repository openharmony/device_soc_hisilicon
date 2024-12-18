/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: common_log_dbg头文件
 * Date: 2022-03-08
 */

#ifndef COMMON_LOG_DBG_H
#define COMMON_LOG_DBG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define common_log_dbg0(vap_id, oam_sf, fmt)             \
    do {                                      \
        oam_warning_log0(vap_id, oam_sf, fmt); \
    } while (0)

#define common_log_dbg1(vap_id, oam_sf, fmt, p1)             \
    do {                                      \
        oam_warning_log1(vap_id, oam_sf, fmt, p1); \
    } while (0)

#define common_log_dbg2(vap_id, oam_sf, fmt, p1, p2)             \
    do {                                      \
        oam_warning_log2(vap_id, oam_sf, fmt, p1, p2); \
    } while (0)

#define common_log_dbg3(vap_id, oam_sf, fmt, p1, p2, p3)             \
    do {                                      \
        oam_warning_log3(vap_id, oam_sf, fmt, p1, p2, p3); \
    } while (0)

#define common_log_dbg4(vap_id, oam_sf, fmt, p1, p2, p3, p4)             \
    do {                                      \
        oam_warning_log4(vap_id, oam_sf, fmt, p1, p2, p3, p4); \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of common_log_dbg.h */
