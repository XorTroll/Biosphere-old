

#pragma once
#include <bio/root.hpp>
#include <sys/types.h>

namespace bio::utf
{
    ssize_t EncodeUtf8(u32 In, u8 *Out);
    ssize_t DecodeUtf8(const u8 *In, u32 *Out);
    ssize_t Utf8To16(const u8 *In, u16 *Out, size_t Length);
    ssize_t Utf8To32(const u8 *In, u32 *Out, size_t Length);
}