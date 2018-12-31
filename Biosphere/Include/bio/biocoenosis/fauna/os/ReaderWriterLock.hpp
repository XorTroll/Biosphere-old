
#pragma once
#include <bio/biocoenosis/fauna/os/Mutex.hpp>

namespace bio::os
{
    enum class LockUnlockMode
    {
        Read,
        Write
    };

    class ReaderWriterLock
    {
        public:
            ReaderWriterLock();
            void Lock(LockUnlockMode Mode);
            void Unlock(LockUnlockMode Mode);
        private:
            RecursiveMutex *read;
            RecursiveMutex *write;
            u64 cnt;
    };
}