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
#ifndef _CONDITION_H
#define _CONDITION_H

#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <ot_mutex.h>

#define SEC_TO_NSEC 1000000000

class OtCondition {
public:
    enum WakeUpType {
        WAKE_UP_ONE = 0,
        WAKE_UP_ALL = 1
    };

    OtCondition();
    virtual ~OtCondition();
    int32_t wait(OtMutex& mutex);
    int32_t waitRelative(OtMutex& mutex, long reltime);
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

inline OtCondition::OtCondition()
{
    (void)pthread_cond_init(&mCond, NULL);
}

inline OtCondition::~OtCondition()
{
    (void)pthread_cond_destroy(&mCond);
}

inline int32_t OtCondition::wait(OtMutex& mutex)
{
    return -pthread_cond_wait(&mCond, &mutex.mMutex);
}

inline int32_t OtCondition::waitRelative(OtMutex& mutex, long reltime)
{
    struct timespec ts;
#ifndef __HuaweiLite__
    (void)clock_gettime(CLOCK_REALTIME, &ts);
#else
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
#endif
    ts.tv_sec += reltime / SEC_TO_NSEC;
    ts.tv_nsec += reltime % SEC_TO_NSEC;
    if (ts.tv_nsec >= SEC_TO_NSEC) {
        ts.tv_nsec -= SEC_TO_NSEC;
        ts.tv_sec += 1;
    }
    return -pthread_cond_timedwait(&mCond, &mutex.mMutex, &ts);
}

inline void OtCondition::signal()
{
    (void)pthread_cond_signal(&mCond);
}

inline void OtCondition::broadcast()
{
    (void)pthread_cond_broadcast(&mCond);
}

#endif // _CONDITION_H
