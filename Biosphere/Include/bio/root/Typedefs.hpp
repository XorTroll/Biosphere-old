
/*

    Biosphere library

    @file Typedefs.hpp
    @brief Basic type definitions (from standard libraries) for the library.
    @author XorTroll

    @copyright Biosphere project - Bringing life to homebrew!

*/

#pragma once
#include <cstdint>
#include <cstring>

typedef uint8_t Bit8;
typedef uint16_t Bit16;
typedef uint32_t Bit32;
typedef uint64_t Bit64;
typedef __uint128_t Bit128;
typedef Bit8 u8;
typedef Bit16 u16;
typedef Bit32 u32;
typedef Bit64 u64;
typedef Bit128 u128;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef __int128_t s128;
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile u128 vu128;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;
typedef volatile s128 vs128;

namespace bio
{
    typedef void (*ThreadFunction)(void*);
}