#include <bio/biocoenosis/fauna/os/Semaphore.hpp>

namespace bio::os
{
    Semaphore::Semaphore(u64 InitialCount)
    {
        this->count = InitialCount;
        this->mtx = new Mutex();
        this->cvar = new ConditionVariable();
    }

    void Semaphore::Signal()
    {
        this->mtx->Lock();
        this->count++;
        this->cvar->WakeOne();
        this->mtx->Unlock();
    }

    void Semaphore::Wait()
    {
        this->mtx->Lock();
        while(!this->count) this->cvar->Wait(this->mtx);
        this->count--;
        this->mtx->Unlock();
    }

    bool Semaphore::TryWait()
    {
        this->mtx->Lock();
        bool ok = false;
        if(this->count)
        {
            this->count--;
            ok = true;
        }
        this->mtx->Unlock();
        return ok;
    }
}