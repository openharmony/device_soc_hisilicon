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
#ifndef _HI_MUTEX_H
#define _HI_MUTEX_H

#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

class HiCondition;

class HiMutex {
public:
    HiMutex();
    virtual ~HiMutex();

    int32_t    lock();
    void        unlock();

    // return 0 if success, otherwise for error
    int32_t    tryLock();

    class HiAutolock {
    public:
        inline HiAutolock(HiMutex& mutex) : mLock(mutex)
        {
            mLock.lock();
        }
        inline HiAutolock(HiMutex* mutex) : mLock(*mutex)
        {
            mLock.lock();
        }
        inline ~HiAutolock()
        {
            mLock.unlock();
        }
    private:
        HiMutex& mLock;
    };

private:
    friend class HiCondition;
    // A mutex cannot be copied
    HiMutex(const HiMutex&);
    HiMutex&      operator = (const HiMutex&);
    pthread_mutex_t mMutex;
};

// ---------------------------------------------------------------------------
inline HiMutex::HiMutex()
{
    (void)pthread_mutex_init(&mMutex, NULL);
}

inline HiMutex::~HiMutex()
{
    (void)pthread_mutex_destroy(&mMutex);
}
inline int32_t HiMutex::lock()
{
    return -pthread_mutex_lock(&mMutex);
}
inline void HiMutex::unlock()
{
    (void)pthread_mutex_unlock(&mMutex);
}
inline int32_t HiMutex::tryLock()
{
    return -pthread_mutex_trylock(&mMutex);
}

typedef HiMutex::HiAutolock AutoMutex;

#endif // _HI_MUTEX_H
