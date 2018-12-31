#include <bio/biotope/arm/Tick.hpp>

namespace bio::arm
{
    u64 GetSystemTick()
    {
        u64 tick;
        __asm__ __volatile__ ("mrs %x[data], cntpct_el0" : [data] "=r" (tick));
        return tick;
    }

    u64 GetSystemTickFrequency()
    {
        u64 freq;
        __asm__ ("mrs %x[data], cntfrq_el0" : [data] "=r" (freq));
        return freq;
    }
}