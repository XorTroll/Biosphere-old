#include <bio/biocoenosis/fauna/os/Mutex.hpp>

namespace bio::os
{
    Mutex::Mutex()
    {
        this->mutex = 0;
    }

    void Mutex::Lock()
    {
        u32 self = Thread::GetThreadVariables()->Handle;
        while(true)
        {
            u32 cur = __sync_val_compare_and_swap((u32*)&this->mutex, 0, self);
            if(cur == 0) return;
            if((cur &~ 0x40000000) == self) return;
            if(cur & 0x40000000) svc::ArbitrateLock((cur &~ 0x40000000), (u32*)&this->mutex, self);
            else
            {
                u32 old = __sync_val_compare_and_swap((u32*)&this->mutex, cur, (cur | 0x40000000));
                if(old == cur) svc::ArbitrateLock(cur, (u32*)&this->mutex, self);
            }
        }
    }

    bool Mutex::TryLock()
    {
        u32 self = Thread::GetThreadVariables()->Handle;
        u32 cur = __sync_val_compare_and_swap((u32*)&this->mutex, 0, self);
        if(cur == 0) return true;
        if((cur &~ 0x40000000) == self) return true;
        return false;
    }

    void Mutex::Unlock()
    {
        u32 old = __sync_val_compare_and_swap((u32*)&this->mutex, Thread::GetThreadVariables()->Handle, 0);
        if(old & 0x40000000) svc::ArbitrateUnlock((u32*)&this->mutex);
    }

    _LOCK_T *Mutex::GetNativeLock()
    {
        return &this->mutex;
    }

    RecursiveMutex::RecursiveMutex()
    {
        this->rmutex.lock = 0;
        this->rmutex.thread_tag = 0;
        this->rmutex.counter = 0;
    }

    void RecursiveMutex::Lock()
    {
        if(this->rmutex.thread_tag != Thread::GetThreadVariables()->Handle)
        {
            u32 self = Thread::GetThreadVariables()->Handle;
            while(true)
            {
                u32 cur = __sync_val_compare_and_swap((u32*)&this->rmutex.lock, 0, self);
                if(cur == 0) return;
                if((cur &~ 0x40000000) == self) return;
                if(cur & 0x40000000) svc::ArbitrateLock(cur &~ 0x40000000, (u32*)&this->rmutex.lock, self);
                else
                {
                    u32 old = __sync_val_compare_and_swap((u32*)&this->rmutex.lock, cur, (cur | 0x40000000));
                    if(old == cur) svc::ArbitrateLock(cur, (u32*)&this->rmutex.lock, self);
                }
            }
            this->rmutex.thread_tag = Thread::GetThreadVariables()->Handle;
        }
        this->rmutex.counter++;
    }

    bool RecursiveMutex::TryLock()
    {
        if(this->rmutex.thread_tag != Thread::GetThreadVariables()->Handle)
        {
            bool tlock = false;
            u32 self = Thread::GetThreadVariables()->Handle;
            u32 cur = __sync_val_compare_and_swap((u32*)&this->rmutex.lock, 0, self);
            if(cur == 0) tlock = true;
            if((cur &~ 0x40000000) == self) tlock = true;
            if(!tlock) return false;
            this->rmutex.thread_tag = Thread::GetThreadVariables()->Handle;
        }
        this->rmutex.counter++;
        return true;
    }

    void RecursiveMutex::Unlock()
    {
        if(--this->rmutex.counter == 0)
        {
            this->rmutex.thread_tag = 0;
            u32 old = __sync_val_compare_and_swap((u32*)&this->rmutex.lock, Thread::GetThreadVariables()->Handle, 0);
            if(old & 0x40000000) svc::ArbitrateUnlock((u32*)&this->rmutex.lock);
        }
    }

    _LOCK_RECURSIVE_T *RecursiveMutex::GetNativeRecursiveLock()
    {
        return &this->rmutex;
    }

    ConditionVariable::ConditionVariable()
    {
        this->cvar = 0;
    }

    Result ConditionVariable::WaitTimeout(Mutex *WaitMutex, u64 Timeout)
    {
        u32 rc = svc::WaitProcessWideKeyAtomic((u32*)(WaitMutex->GetNativeLock()), &this->cvar, Thread::GetThreadVariables()->Handle, Timeout);
        if(rc == 0xea01) WaitMutex->Lock();
        return rc;
    }

    Result ConditionVariable::Wait(Mutex *WaitMutex)
    {
        return this->WaitTimeout(WaitMutex, UINT64_MAX);
    }

    Result ConditionVariable::Wake(s32 ThreadNumber)
    {
        return svc::SignalProcessWideKey(&this->cvar, ThreadNumber);
    }

    Result ConditionVariable::WakeOne()
    {
        return this->Wake(1);
    }

    Result ConditionVariable::WakeAll()
    {
        return this->Wake(-1);
    }
}