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
 */


#ifndef	_ERRNO_H
#define _ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#include <bits/errno.h>

#ifdef __LITEOS__
/**
 * @ingroup  errno
 *
 * @par Description:
 * The set_errno() function sets the error code.
 *
 * @attention
 * <ul>
 * <li>The set_errno() function is not a standard C Library Function.</li>
 * </ul>
 *
 * @retval #void None.
 *
 * @par Dependency:
 * <ul><li>errno.h</li></ul>
 * @see get_errno
 */
void set_errno(int errcode);

/**
 * @ingroup  errno
 *
 * @par Description:
 * The get_errno() function gets the error code.
 *
 * @attention
 * <ul>
 * <li>The get_errno() function is not a standard C Library Function.</li>
 * </ul>
 *
 * @retval #int The get_errno() returns the error code.
 *
 * @par Dependency:
 * <ul><li>errno.h</li></ul>
 * @see set_errno
 */
int get_errno(void);
#endif

#ifdef __GNUC__
__attribute__((const))
#endif
int *__errno_location(void);
#define errno (*__errno_location())

#ifdef _GNU_SOURCE
extern char *program_invocation_short_name, *program_invocation_name;
#endif

#ifdef __cplusplus
}
#endif

#endif

