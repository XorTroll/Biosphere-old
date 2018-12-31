#include <bio/biocoenosis/fauna/os/ReaderWriterLock.hpp>

namespace bio::os
{
    ReaderWriterLock::ReaderWriterLock()
    {
        this->read = new RecursiveMutex();
        this->write = new RecursiveMutex();
        this->cnt = 0;
    }

    void ReaderWriterLock::Lock(LockUnlockMode Mode)
    {
        switch(Mode)
        {
            case LockUnlockMode::Read:
                this->read->Lock();
                if(this->cnt++ == 0) this->write->Lock();
                this->read->Unlock();
                break;
            case LockUnlockMode::Write:
                this->write->Lock();
                break;
        }
    }

    void ReaderWriterLock::Unlock(LockUnlockMode Mode)
    {
        switch(Mode)
        {
            case LockUnlockMode::Read:
                this->read->Lock();
                if(this->cnt-- == 1) this->write->Unlock();
                this->read->Unlock();
                break;
            case LockUnlockMode::Write:
                this->write->Unlock();
                break;
        }
    }
}