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
#ifndef _MUTEX_H
#define _MUTEX_H

#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

class OtCondition;

class OtMutex {
public:
    OtMutex() noexcept;
    virtual ~OtMutex();

    int32_t    lock();
    void        unlock();

    // return 0 if success, otherwise for error
    int32_t    tryLock();

    class OtAutolock {
    public:
        inline OtAutolock(OtMutex& mutex) : mLock(mutex)
        {
            mLock.lock();
        }
        inline OtAutolock(OtMutex* mutex) : mLock(*mutex)
        {
            mLock.lock();
        }
        inline ~OtAutolock()
        {
            mLock.unlock();
        }
    private:
        OtMutex& mLock;
    };

private:
    friend class OtCondition;
    // A mutex cannot be copied
    OtMutex(const OtMutex&);
    OtMutex&      operator = (const OtMutex&);
    pthread_mutex_t mMutex;
};

// ---------------------------------------------------------------------------
inline OtMutex::OtMutex() noexcept
{
    (void)pthread_mutex_init(&mMutex, NULL);
}

inline OtMutex::~OtMutex()
{
    (void)pthread_mutex_destroy(&mMutex);
}
inline int32_t OtMutex::lock()
{
    return -pthread_mutex_lock(&mMutex);
}
inline void OtMutex::unlock()
{
    (void)pthread_mutex_unlock(&mMutex);
}
inline int32_t OtMutex::tryLock()
{
    return -pthread_mutex_trylock(&mMutex);
}

typedef OtMutex::OtAutolock AutoMutex;

#endif // _MUTEX_H
