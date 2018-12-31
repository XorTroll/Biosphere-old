
#pragma once
#include <bio/root.hpp>
#include <bio/biotope/arm.hpp>
#include <bio/biocoenosis/fauna/os/Memory.hpp>
#include <malloc.h>
#include <cstring>

namespace bio::os
{
    struct Thread;

    struct ThreadVariables
    {
        u32 Magic;
        u32 Handle;
        Thread *Pointer;
        struct _reent *Reent;
        void *ThreadLocalSegment;
    };

    struct Thread
    {
        Thread(ThreadFunction Entry, void *Arguments, size_t StackSize, int Priority, int CPUId);
        ~Thread();
        Result Start();
        Result WaitForExit();
        Result Close();
        Result Pause();
        Result Resume();
        u32 Handle;
        void *StackMemory;
        void *StackMemoryMirror;
        size_t StackSize;
        static ThreadVariables *GetThreadVariables();
    };
}