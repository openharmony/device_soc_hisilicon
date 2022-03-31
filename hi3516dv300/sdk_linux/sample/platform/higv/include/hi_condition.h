/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _HI_CONDITION_H
#define _HI_CONDITION_H

#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <hi_mutex.h>

#define SEC_TO_NSEC 1000000000

class HiCondition {
public:
    enum WakeUpType {
        WAKE_UP_ONE = 0,
        WAKE_UP_ALL = 1
    };

    HiCondition();
    virtual ~HiCondition();
    int32_t wait(HiMutex& mutex);
    int32_t waitRelative(HiMutex& mutex, long reltime);
    void signal();
    void signal(WakeUpType type)
    {
        if (type == WAKE_UP_ONE) {
            signal();
        } else {
            broadcast();
        }
    }
    void broadcast();

private:
    pthread_cond_t mCond;
};

inline HiCondition::HiCondition()
{
    (void)pthread_cond_init(&mCond, NULL);
}

inline HiCondition::~HiCondition()
{
    (void)pthread_cond_destroy(&mCond);
}

inline int32_t HiCondition::wait(HiMutex& mutex)
{
    return -pthread_cond_wait(&mCond, &mutex.mMutex);
}

inline int32_t HiCondition::waitRelative(HiMutex& mutex, long reltime)
{
    struct timespec ts;
#ifndef __HuaweiLite__
    clock_gettime(CLOCK_REALTIME, &ts);
#else
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
#endif
    ts.tv_sec += reltime / SEC_TO_NSEC;
    ts.tv_nsec += reltime % SEC_TO_NSEC;
    if (ts.tv_nsec >= SEC_TO_NSEC) {
        ts.tv_nsec -= SEC_TO_NSEC;
        ts.tv_sec  += 1;
    }
    return -pthread_cond_timedwait(&mCond, &mutex.mMutex, &ts);
}

inline void HiCondition::signal()
{
    (void)pthread_cond_signal(&mCond);
}

inline void HiCondition::broadcast()
{
    (void)pthread_cond_broadcast(&mCond);
}

#endif
