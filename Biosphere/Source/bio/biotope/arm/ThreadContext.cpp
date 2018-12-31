#include <bio/biotope/arm/ThreadContext.hpp>

namespace bio::arm
{
    bool ThreadContext::IsAArch64()
    {
        return ((this->psr & 0x10) == 0);
    }

    bool ThreadExceptionDump::IsAArch64()
    {
        return ((this->pstate & 0x10) == 0);
    }

    void *GetThreadLocalStorage()
    {
        void *tls;
        __asm__("mrs %x[data], tpidrro_el0" : [data] "=r" (tls));
        return tls;
    }
}