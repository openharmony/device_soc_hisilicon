/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <securec.h>
#include "los_task.h"
#include "los_task_pri.h"

#define POLLING_INTERVAL_FOR_JOIN 1000
#define PTHREAD_NAMELEN 16

struct PthreadData {
    CHAR name[PTHREAD_NAMELEN];
    BOOL detached;
    BOOL exited;
    VOID *exitCode;
};

/**
 * pthread id and native task id translation is use for preventing
 * wrongly passing pthread id to LOS_TaskXXX API as a native task id.
 */

/* translate pthread_t id to taskID */
static inline UINT32 P2T(pthread_t id)
{
    return (UINT32)id - g_taskMaxNum;
}

/* translate taskID to pthread_t id */
static inline pthread_t T2P(UINT32 id)
{
    return (pthread_t)(id + g_taskMaxNum);
}

/**
 * Only pthreads can be set name by pthread_setname_np.
 * Native threads are detached by default and its name can't be set.
 */

static inline BOOL IsValidTask(UINT32 taskID)
{
    return (taskID < g_taskMaxNum) &&
           !(OS_TCB_FROM_TID(taskID)->taskStatus & OS_TASK_STATUS_UNUSED);
}

static void *PthreadEntry(UINT32 param1, UINT32 param2, UINT32 param3, UINT32 param4);
static inline BOOL IsPthread(UINT32 taskID)
{
    return IsValidTask(taskID) &&
           (OS_TCB_FROM_TID(taskID)->taskEntry == PthreadEntry);
}

static inline struct PthreadData *GetPthreadData(UINT32 taskID)
{
    return IsPthread(taskID) ?
           (struct PthreadData *)(UINTPTR)(OS_TCB_FROM_TID(taskID)->taskName) : NULL;
}

static void *PthreadEntry(UINT32 param1, UINT32 param2, UINT32 param3, UINT32 param4)
{
    void *(*startRoutine)(void *) = (void *)(UINTPTR)param1;
    void *param = (void *)(UINTPTR)param2;
    void *retVal = NULL;
    UINT32 taskID = LOS_CurTaskIDGet();
    struct PthreadData *pthreadData = NULL;

    (void)param3;
    (void)param4;

    retVal = startRoutine(param);

    pthreadData = GetPthreadData(taskID);
    if (pthreadData->detached) {
        free(pthreadData);
        pthreadData = NULL;
    } else {
        pthreadData->exitCode = retVal;
        pthreadData->exited = TRUE;
        if (LOS_ERRNO_TSK_SUSPEND_LOCKED == LOS_TaskSuspend(taskID)) {
            LOS_TaskUnlock();
        }
        while (LOS_ERRNO_TSK_SUSPEND_LOCKED != LOS_TaskSuspend(taskID)) {
        }
    }

    return retVal;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*startRoutine)(void *), void *arg)
{
    TSK_INIT_PARAM_S taskInitParam = {0};
    UINT32 taskID;
    struct PthreadData *pthreadData = NULL;

    if ((thread == NULL) || (startRoutine == NULL)) {
        return EINVAL;
    }

    pthreadData = malloc(sizeof(struct PthreadData));
    if (pthreadData == NULL) {
        return ENOMEM;
    }

    (void)memset_s(pthreadData, sizeof(struct PthreadData), 0, sizeof(struct PthreadData));

    taskInitParam.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    if (attr) {
        if (attr->detachstate == PTHREAD_CREATE_DETACHED) {
            pthreadData->detached = TRUE;
        }
        if (attr->stackaddr_set) {
            free(pthreadData);
            return ENOTSUP;
        }
        if (attr->stacksize_set) {
            taskInitParam.uwStackSize = attr->stacksize;
        }
        taskInitParam.usTaskPrio = (UINT16)attr->schedparam.sched_priority;
    }

    taskInitParam.pcName       = pthreadData->name;
    taskInitParam.pfnTaskEntry = PthreadEntry;
    taskInitParam.auwArgs[0]   = (UINT32)(UINTPTR)startRoutine;
    taskInitParam.auwArgs[1]   = (UINT32)(UINTPTR)arg;

    if (LOS_TaskCreateOnly(&taskID, &taskInitParam) != LOS_OK) {
        free(pthreadData);
        return EAGAIN;
    }

    /* set pthread default name */
    (void)sprintf_s(taskInitParam.pcName, PTHREAD_NAMELEN, "pthread%u", (UINT32)T2P(taskID));

    (void)LOS_TaskResume(taskID);

    *thread = T2P(taskID);
    return 0;
}

int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param)
{
    UINT32 taskID = P2T(thread);

    if (param == NULL) {
        return EINVAL;
    }

    if (!IsValidTask(taskID)) {
        return ESRCH;
    }

    /* Only support SCHED_RR policy now */
    if (policy != SCHED_RR) {
        return ENOTSUP;
    }

    if ((param->sched_priority < OS_TASK_PRIORITY_HIGHEST) ||
        (param->sched_priority >= OS_TASK_PRIORITY_LOWEST)) {
        return EINVAL;
    }

    if (LOS_TaskPriSet(taskID, (UINT16)param->sched_priority) != LOS_OK) {
        return EPERM;
    }

    return 0;
}

int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param)
{
    UINT32 prio;
    UINT32 taskID = P2T(thread);

    if ((policy == NULL) || (param == NULL)) {
        return EINVAL;
    }

    if (!IsValidTask(taskID)) {
        return ESRCH;
    }

    prio = LOS_TaskPriGet(taskID);
    if (prio == OS_INVALID) {
        return EINVAL;
    }

    *policy = SCHED_RR;
    param->sched_priority = prio;
    return 0;
}

pthread_t pthread_self(void)
{
    UINT32 taskID = LOS_CurTaskIDGet();
    return T2P(taskID);
}

int pthread_cancel(pthread_t thread)
{
    UINT32 taskID = P2T(thread);
    struct PthreadData *pthreadData = NULL;

    if (!IsValidTask(taskID)) {
        return ESRCH;
    }

    if (thread == pthread_self()) {
        return EDEADLK;
    }

    if (!IsPthread(taskID)) {
        switch (LOS_TaskDelete(taskID)) {
            case LOS_OK:
                return 0;
            case LOS_ERRNO_TSK_NOT_CREATED:
                return ESRCH;
            case LOS_ERRNO_TSK_OPERATE_IDLE:
            case LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED:
                return EPERM;
            default:
                break;
        }
        return EINVAL;
    }

    pthreadData = GetPthreadData(taskID);
    if (pthreadData->detached) {
        (void)LOS_TaskDelete(taskID);
        free(pthreadData);
        return 0;
    }

    if (pthreadData->exited) {
        return 0;
    }

    pthreadData->exitCode = PTHREAD_CANCELED;
    pthreadData->exited = TRUE;
    LOS_TaskSuspend(taskID);
    return 0;
}

static void *VoidTask(UINT32 param1, UINT32 param2, UINT32 param3, UINT32 param4)
{
    (void)param1;
    (void)param2;
    (void)param3;
    (void)param4;
    return 0;
}

static void CleanupTaskResource(void)
{
    TSK_INIT_PARAM_S taskInitParam = {0};
    UINT32 taskID;

    taskInitParam.pcName       = "void";
    taskInitParam.pfnTaskEntry = VoidTask;
    taskInitParam.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE;

    (void)LOS_TaskCreate(&taskID, &taskInitParam);
}

int pthread_join(pthread_t thread, void **retval)
{
    UINT32 taskStatus;
    struct PthreadData *pthreadData = NULL;
    UINT32 taskID = P2T(thread);
    if (!IsValidTask(taskID)) {
        return ESRCH;
    }

    if (!IsPthread(taskID)) {
        /* native thread is always detached so it can't be join */
        return EINVAL;
    }

    if (thread == pthread_self()) {
        return EDEADLK;
    }

    pthreadData = GetPthreadData(taskID);

    while (LOS_TaskStatusGet(taskID, &taskStatus) == LOS_OK) {
        if (pthreadData->detached) {
            /* detached thread can't be join */
            return EINVAL;
        }
        if ((taskStatus & OS_TASK_STATUS_SUSPEND) && pthreadData->exited) {
            if (retval) {
                *retval = pthreadData->exitCode;
            }
            (void)LOS_TaskDelete(taskID);
            free(pthreadData);
            pthreadData = NULL;
            CleanupTaskResource();
            return 0;
        }
        /* we use usleep to simplify this implementation or we need an extra semaphore for each pthread */
        usleep(POLLING_INTERVAL_FOR_JOIN);
    }

    return ESRCH;
}

int pthread_detach(pthread_t thread)
{
    UINT32 taskID = P2T(thread);
    struct PthreadData *pthreadData = NULL;

    if (!IsValidTask(taskID)) {
        return ESRCH;
    }

    if (!IsPthread(taskID)) {
        /* native thread is always detached so it can't be detach again */
        return EINVAL;
    }

    pthreadData = GetPthreadData(taskID);
    if (pthreadData->detached) {
        /* detached thread can't be detach again */
        return EINVAL;
    }

    if (pthreadData->exited) {
        (void)LOS_TaskDelete(taskID);
        free(pthreadData);
        return 0;
    }
    pthreadData->detached = TRUE;
    return 0;
}

void pthread_exit(void *retVal)
{
    UINT32 taskID = LOS_CurTaskIDGet();
    struct PthreadData *pthreadData = NULL;

    if (!IsPthread(taskID)) {
        /* native thread just delete self */
        while (LOS_OK != LOS_TaskDelete(taskID)) {
        }
    }

    pthreadData = GetPthreadData(taskID);
    if (pthreadData->detached) {
        free(pthreadData);
        while (LOS_OK != LOS_TaskDelete(taskID)) {
        }
    } else {
        pthreadData->exitCode = retVal;
        pthreadData->exited = TRUE;
        while (LOS_ERRNO_TSK_SUSPEND_LOCKED != LOS_TaskSuspend(taskID)) {
        }
    }
}

int pthread_setname_np(pthread_t thread, const char *name)
{
    UINT32 taskID = P2T(thread);
    if (!IsValidTask(taskID)) {
        return ESRCH;
    }

    if (!IsPthread(taskID)) {
        /* native thread's name is readonly so it can not be set */
        return EPERM;
    }

    /* ensure the following strcpy_s not fail and making unneeded side effect (set src to empty string) */
    if (strlen(name) >= PTHREAD_NAMELEN) {
        return ERANGE;
    }

    if (strcpy_s(LOS_TaskNameGet(taskID), PTHREAD_NAMELEN, name) != EOK) {
        return ERANGE;
    }
    return 0;
}

int pthread_getname_np(pthread_t thread, char *buf, size_t buflen)
{
    const char *name = NULL;
    UINT32 taskID = P2T(thread);
    if (!IsValidTask(taskID)) {
        return ESRCH;
    }

    name = LOS_TaskNameGet(taskID);
    if (name == NULL) {
        name = "";
    }

    /* ensure the following strcpy_s not fail and making unneeded side effect (set src to empty string) */
    if (strlen(name) >= PTHREAD_NAMELEN) {
        return ERANGE;
    }

    if (strcpy_s(buf, buflen, name) != EOK) {
        return ERANGE;
    }
    return 0;
}
