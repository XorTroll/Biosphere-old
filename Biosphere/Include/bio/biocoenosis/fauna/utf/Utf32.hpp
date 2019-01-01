

#pragma once
#include <bio/root.hpp>
#include <sys/types.h>

namespace bio::utf
{
    ssize_t Utf32To8(const u32 *In, u8 *Out, size_t Length);
    ssize_t Utf32To16(const u32 *In, u16 *Out, size_t Length);
}