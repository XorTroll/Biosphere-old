#include <bio/biocoenosis/fauna/utf/Utf8.hpp>

namespace bio::utf
{
    ssize_t EncodeUtf8(u32 In, u8 *Out)
    {
        if(in < 0x80)
        {
            if(out != NULL) *out++ = in;
            return 1;
        }
        else if(in < 0x800)
        {
            if(out != NULL)
            {
                *out++ = (in >> 6) + 0xC0;
                *out++ = (in & 0x3F) + 0x80;
            }
            return 2;
        }
        else if(in < 0x10000)
        {
            if(out != NULL)
            {
                *out++ = (in >> 12) + 0xE0;
                *out++ = ((in >> 6) & 0x3F) + 0x80;
                *out++ = (in & 0x3F) + 0x80;
            }
            return 3;
        }
        else if(in < 0x110000)
        {
            if(out != NULL)
            {
                *out++ = (in >> 18) + 0xF0;
                *out++ = ((in >> 12) & 0x3F) + 0x80;
                *out++ = ((in >> 6) & 0x3F) + 0x80;
                *out++ = (in & 0x3F) + 0x80;
            }
            return 4;
        }
        return -1;
    }

    ssize_t DecodeUtf8(const u8 *In, u32 *Out)
    {
        uint8_t code1, code2, code3, code4;
        code1 = *in++;
        if(code1 < 0x80)
        {
            *out = code1;
            return 1;
        }
        else if(code1 < 0xC2) return -1;
        else if(code1 < 0xE0)
        {
            code2 = *in++;
            if((code2 & 0xC0) != 0x80) return -1;
            *out = (code1 << 6) + code2 - 0x3080;
            return 2;
        }
        else if(code1 < 0xF0)
        {
            code2 = *in++;
            if((code2 & 0xC0) != 0x80) return -1;
            if(code1 == 0xE0 && code2 < 0xA0) return -1;
            code3 = *in++;
            if((code3 & 0xC0) != 0x80) return -1;
            *out = (code1 << 12) + (code2 << 6) + code3 - 0xE2080;
            return 3;
        }
        else if(code1 < 0xF5)
        {
            code2 = *in++;
            if((code2 & 0xC0) != 0x80) return -1;
            if(code1 == 0xF0 && code2 < 0x90) return -1;
            if(code1 == 0xF4 && code2 >= 0x90) return -1;
            code3 = *in++;
            if((code3 & 0xC0) != 0x80) return -1;
            code4 = *in++;
            if((code4 & 0xC0) != 0x80) return -1;
            *out = (code1 << 18) + (code2 << 12) + (code3 << 6) + code4 - 0x3C82080;
            return 4;
        }
        return -1;
    }

    ssize_t Utf8To16(const u8 *In, u16 *Out, size_t Length)
    {

    }

    ssize_t Utf8To32(const u8 *In, u32 *Out, size_t Length)
    {

    }
}