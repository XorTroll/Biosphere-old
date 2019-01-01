

#pragma once
#include <bio/root.hpp>
#include <sys/types.h>

namespace bio::utf
{
    ssize_t EncodeUtf16(u32 In, u16 *Out);
    ssize_t DecodeUtf16(const u16 *In, u32 *Out);
    ssize_t Utf16To8(const u16 *In, u8 *Out, size_t Length);
    ssize_t Utf16To32(const u16 *In, u32 *Out, size_t Length);
}