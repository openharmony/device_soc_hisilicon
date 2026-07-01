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

#ifndef SAMPLE_LOG_H
#define SAMPLE_LOG_H

#define sample_log(format, arg...)  printf(format, ##arg)
#define sample_err(format, arg...) printf("Error: [%s-%d]" format, __FUNCTION__, __LINE__, ##arg)

#ifndef sample_chk_return
#define sample_chk_return(cond, err_ret) do {   \
    if (cond) { \
        sample_err("%s\n", ##cond);   \
        return err_ret; \
    }   \
} while (0)
#endif

#ifndef sample_chk_expr_return
#define sample_chk_expr_return(expr, expected_ret) do { \
    int _ret = expr;  \
    if (_ret != expected_ret) { \
        sample_err("%s return 0x%x\n", #expr, _ret);    \
        return _ret;    \
    }   \
} while (0)
#endif

#ifndef sample_chk_expr_with_return
#define sample_chk_expr_with_return(expr, expected_ret, err_ret) do {   \
    int _ret = expr;    \
    if (_ret != expected_ret) { \
        sample_err("%s return 0x%x\n", #expr, _ret);    \
        return err_ret;    \
    }   \
} while (0)
#endif

#ifndef sample_chk_expr_goto
#define sample_chk_expr_goto(expr, expected_ret, label) do { \
    int _ret = expr;  \
    if (_ret != (expected_ret)) { \
        sample_err("%s return 0x%x\n", #expr, _ret);    \
        goto label;    \
    }   \
} while (0)
#endif

#ifndef sample_chk_expr_goto_with_ret
#define sample_chk_expr_goto_with_ret(expr, expected_ret, ret, err_ret, label) do { \
    int _ret = expr;  \
    if (_ret != (expected_ret)) { \
        sample_err("%s return 0x%x\n", #expr, _ret);    \
        ret = err_ret;  \
        goto label;    \
    }   \
} while (0)
#endif

#endif