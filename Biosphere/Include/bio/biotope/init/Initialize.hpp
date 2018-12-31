
#pragma once
#include <bio/root.hpp>

namespace bio::init
{
    struct HomebrewContext
    {
        u32 Key;
        u32 Flags;
        u64 Value[2];
    };

    Result InitializeHomebrewEnvironment(void *Context, u32 MainThread, void *SavedLoader);
    Result InitializeStandardSysCalls();
    Result InitializeStandardFileSystem();
    Result InitializeStandardConstructors();
    Result InitializeKernel();
    Result InitializeArguments();
}