
#pragma once
#include <bio/biocoenosis/fauna/os/Thread.hpp>
#include <sys/lock.h>

namespace bio::os
{
    class Mutex
    {
        public:
            Mutex();
            void Lock();
            bool TryLock();
            void Unlock();
            _LOCK_T *GetNativeLock();
        private:
            _LOCK_T mutex;
    };

    class RecursiveMutex
    {
        public:
            RecursiveMutex();
            void Lock();
            bool TryLock();
            void Unlock();
            _LOCK_RECURSIVE_T *GetNativeRecursiveLock();
        private:
            _LOCK_RECURSIVE_T rmutex;
    };

    class ConditionVariable
    {
        public:
            ConditionVariable();
            Result WaitTimeout(Mutex *WaitMutex, u64 Timeout);
            Result Wait(Mutex *WaitMutex);
            Result Wake(s32 ThreadNumber);
            Result WakeOne();
            Result WakeAll();
        private:
            u32 cvar;
    };
}