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
 * Description: OSAL Error Definition.
 */

#ifndef OSAL_ERRNO_H_
#define OSAL_ERRNO_H_

#define OS_HWI_PRIO_IGNORE 6

/* Basic common errno 1~34 */
#define OSAL_EINTR (-4)  /* Interrupted system call */
#define OSAL_EINVAL (-22)  /* Interrupted system call */
#define OSAL_ETIME (-62)  /* Timer expired */
#define OSAL_EOVERFLOW (-75)  /* Value too large for defined data type */

#define OSAL_ERESTARTSYS 512

#ifndef ERESTARTSYS
#define ERESTARTSYS 512
#endif

#define OSAL_NONEOS_DEFAULT_RET (-200) /* NoneOS's default return value */
#ifndef EOK
#define EOK 0
#endif

#endif /* OSAL_ERRNO_H_ */
