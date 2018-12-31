
#pragma once
#include <bio/root.hpp>

namespace bio::arm
{
    namespace Atomic
    {
        u32 Increment32(u32 Value);
        u64 Increment64(u64 Value);
        u32 Decrement32(u32 Value);
        u64 Decrement64(u64 Value);
    }
}