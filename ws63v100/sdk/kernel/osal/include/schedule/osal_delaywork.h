/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: OS Abstract Layer.
 */

/**
 * @defgroup osal_delayedwork osal_delayedwork
 */
#ifndef __OSAL_DELAYWORK_H__
#define __OSAL_DELAYWORK_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct osal_delayedwork_ {
    void *work;
    void (*handler)(struct osal_delayedwork_ *delayedwork);
} osal_delayedwork;
typedef void (*osal_delayedwork_handler)(osal_delayedwork *delayedwork);

/**
 * @ingroup osal_delayedwork
 * @brief This API is used to initialization of delayedwork.
 *
 * @par Description:
 * This API is used to initialization of delayedwork.
 *
 * @attention Must be freed with osal_delayedwork_destroy.
 *
 * @param work [in/out] The delayedwork to be initialized.
 * @param handler [in/out] The delayedwork callback handler function.
 *
 * @return OSAL_SUCCESS/OSAL_FAILURE
 *
 * @par Support System:
 * linux.
 */
int osal_delayedwork_init(osal_delayedwork *work, osal_delayedwork_handler handler);

/**
 * @ingroup osal_delayedwork
 * @brief This API is used to destroy the delayedwork.
 *
 * @par Description:
 * This API is used to destroy the delayedwork.
 *
 * @param work [in] The delayedwork to be destroyed.
 *
 * @attention this api may free memory, work should be from osal_delayedwork_init.
 *
 * @par Support System:
 * linux.
 */
void osal_delayedwork_destroy(osal_delayedwork *work);

/**
 * @ingroup osal_delayedwork
 * @brief put work task in global workqueue after delay.
 *
 * @par Description:
 * put work task in global workqueue after delay.
 * After waiting for a given time this puts a job in the kernel-global workqueue.
 *
 * @param work [in] Job to be done.
 * @param timeout [in] Number of jiffies to wait or 0 for immediate execution.
 *
 * @return OSAL_SUCCESS/OSAL_FAILURE
 *
 * @par Support System:
 * linux.
 */
int osal_delayedwork_schedule(osal_delayedwork *work, int timeout);

/**
 * @ingroup osal_delayedwork
 * @brief cancel a delayed work and wait for it to finish.
 *
 * @par Description:
 * cancel a delayed work and wait for it to finish.
 *
 * @param work [in] The delayed work cancel.
 *
 * @return OSAL_SUCCESS/OSAL_FAILURE
 *
 * @par Support System:
 * linux.
 */
int osal_delayedwork_cancel_sync(osal_delayedwork *work);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif /* __OSAL_DELAYWORK_H__ */