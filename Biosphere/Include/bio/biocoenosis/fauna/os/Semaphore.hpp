
#pragma once
#include <bio/biocoenosis/fauna/os/Mutex.hpp>

namespace bio::os
{
    class Semaphore
    {
        public:
            Semaphore(u64 InitialCount);
            void Signal();
            void Wait();
            bool TryWait();
        private:
            Mutex *mtx;  
            ConditionVariable *cvar;
            u64 count;
    };
}