
#pragma once
#include <bio/root.hpp>

namespace bio::arm
{
    union CPURegister
    {
        u64 x;
        u32 w;
        u32 r;
    };

    union FPURegister
    {
        u128 v;
        double d;
        float s; 
    };

    enum class RegisterGroup
    {
        CPUGeneralPurpose = BIO_BITMASK(0),
        CPUSpecialPurpose = BIO_BITMASK(1),
        FPUGeneralPurpose = BIO_BITMASK(2),
        FPUSpecialPurpose = BIO_BITMASK(3),
        CPUAll = (CPUGeneralPurpose | CPUSpecialPurpose),
        FPUAll = (FPUGeneralPurpose | FPUSpecialPurpose),
        All = (CPUAll  | FPUAll),
    };

    enum ThreadException
    {
        InstructionAbort = 0x100,
        MisalignedPC = 0x102,
        MisalignedSP = 0x103,
        SError = 0x106,
        BadSVC = 0x301,
        Trap = 0x104,
        Other = 0x101,
    };

    struct ThreadContext
    {
        CPURegister cpu_gprs[29];
        u64 fp;
        u64 lr;
        u64 sp;
        CPURegister pc;
        u32 psr;
        FPURegister fpu_gprs[32];
        u32 fpcr;
        u32 fpsr;
        u64 tpidr;

        bool IsAArch64();
    };

    struct ThreadExceptionDump
    {
        u32 error_desc;
        u32 pad[3];
        CPURegister cpu_gprs[29];
        CPURegister fp;
        CPURegister lr;
        CPURegister sp;
        CPURegister pc;
        u64 padding;
        FPURegister fpu_gprs[32];
        u32 pstate; 
        u32 afsr0;
        u32 afsr1;
        u32 esr;
        CPURegister far;

        bool IsAArch64();
    };

    struct ThreadExceptionFrame64
    {
        u64 cpu_gprs[9];
        u64 lr;
        u64 sp;
        u64 elr_el1;
        u32 pstate;
        u32 afsr0;
        u32 afsr1;
        u32 esr;
        u64 far;
    };

    struct ThreadExceptionFrame32
    {
        u32 cpu_gprs[8];
        u32 sp;
        u32 lr;
        u32 elr_el1;
        u32 tpidr_el0;
        u32 cpsr;
        u32 afsr0;
        u32 afsr1;
        u32 esr;
        u32 far;
    };

    void *GetThreadLocalStorage();
}