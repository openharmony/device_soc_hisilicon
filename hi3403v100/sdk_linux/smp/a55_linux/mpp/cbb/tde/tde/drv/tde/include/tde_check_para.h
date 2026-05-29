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
#ifndef TDE_CHECK_PARA_H
#define TDE_CHECK_PARA_H

#include "ot_type.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define tde_check_array_over_return_value(cur_index, max_index, ret) \
    do {                                                           \
        if ((cur_index) >= (max_index)) {                          \
            return (ret);                                          \
        }                                                          \
    } while (0)

#define tde_check_array_over_return_novalue(cur_index, max_index) \
    do {                                                        \
        if ((cur_index) >= (max_index)) {                       \
            return;                                             \
        }                                                       \
    } while (0)

#define tde_check_nullpointer_return_novalue(pointer) \
    do {                                              \
        if ((pointer) == TD_NULL) {                    \
            return;                                   \
        }                                             \
    } while (0)

#define tde_check_nullpointer_return_value(pointer, ret) \
    do {                                                 \
        if ((pointer) == TD_NULL) {                      \
            return (ret);                                \
        }                                                \
    } while (0)

#define tde_check_unequal_return_novalue(para1, para2) \
    do {                                               \
        if ((para1) != (para2)) {                      \
            return;                                    \
        }                                              \
    } while (0)

#define tde_check_unequal_return_value(para1, para2, ret) \
    do {                                                  \
        if ((para1) != (para2)) {                         \
            return (ret);                                 \
        }                                                 \
    } while (0)

#define tde_check_equal_return_value(para1, para2, ret) \
    do {                                                \
        if ((para1) == (para2)) {                       \
            return (ret);                               \
        }                                               \
    } while (0)

#define tde_check_equal_return_novalue(para1, para2) \
    do {                                             \
        if ((para1) == (para2)) {                    \
            return;                                  \
        }                                            \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* TDE_CHECK_PARA_H */
