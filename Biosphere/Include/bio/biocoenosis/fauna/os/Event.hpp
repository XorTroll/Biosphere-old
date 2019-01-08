
#pragma once
#include <bio/biocoenosis/fauna/os/Kernel.hpp>

namespace bio::os
{
    class Event
    {
        public:
            Event(bool AutoClear);
            Event(u32 Handle, bool AutoClear);
            ~Event();
            bool IsValid();
            bool AutoClears();
            void SetAutoClear(bool AutoClear);
            Result Wait(u64 Timeout);
            Result Fire();
            Result Clear();
            void Close();
        private:
            bool cauto;
            u32 whandle;
            u32 rhandle;
    };
}