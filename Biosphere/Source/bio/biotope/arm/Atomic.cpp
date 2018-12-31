#include <bio/biotope/arm/Atomic.hpp>

namespace bio::arm
{
    u32 Atomic::Increment32(u32 Value)
    {
        return __atomic_fetch_add(&Value, 1, __ATOMIC_SEQ_CST);
    }

    u64 Atomic::Increment64(u64 Value)
    {
        return __atomic_fetch_add(&Value, 1, __ATOMIC_SEQ_CST);
    }

    u32 Atomic::Decrement32(u32 Value)
    {
        return __atomic_sub_fetch(&Value, 1, __ATOMIC_SEQ_CST);
    }

    u64 Atomic::Decrement64(u64 Value)
    {
        return __atomic_sub_fetch(&Value, 1, __ATOMIC_SEQ_CST);
    }
}