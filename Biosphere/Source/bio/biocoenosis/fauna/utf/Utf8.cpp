#include <bio/biocoenosis/fauna/utf/Utf8.hpp>
#include <bio/biocoenosis/fauna/utf/Utf16.hpp>
#include <sys/types.h>

namespace bio::utf
{
    ssize_t EncodeUtf8(u32 In, u8 *Out)
    {
        if(In < 0x80)
        {
            if(Out != NULL) *Out++ = In;
            return 1;
        }
        else if(In < 0x800)
        {
            if(Out != NULL)
            {
                *Out++ = (In >> 6) + 0xC0;
                *Out++ = (In & 0x3F) + 0x80;
            }
            return 2;
        }
        else if(In < 0x10000)
        {
            if(Out != NULL)
            {
                *Out++ = (In >> 12) + 0xE0;
                *Out++ = ((In >> 6) & 0x3F) + 0x80;
                *Out++ = (In & 0x3F) + 0x80;
            }
            return 3;
        }
        else if(In < 0x110000)
        {
            if(Out != NULL)
            {
                *Out++ = (In >> 18) + 0xF0;
                *Out++ = ((In >> 12) & 0x3F) + 0x80;
                *Out++ = ((In >> 6) & 0x3F) + 0x80;
                *Out++ = (In & 0x3F) + 0x80;
            }
            return 4;
        }
        return -1;
    }

    ssize_t DecodeUtf8(const u8 *In, u32 *Out)
    {
        uint8_t code1, code2, code3, code4;
        code1 = *In++;
        if(code1 < 0x80)
        {
            *Out = code1;
            return 1;
        }
        else if(code1 < 0xC2) return -1;
        else if(code1 < 0xE0)
        {
            code2 = *In++;
            if((code2 & 0xC0) != 0x80) return -1;
            *Out = (code1 << 6) + code2 - 0x3080;
            return 2;
        }
        else if(code1 < 0xF0)
        {
            code2 = *In++;
            if((code2 & 0xC0) != 0x80) return -1;
            if(code1 == 0xE0 && code2 < 0xA0) return -1;
            code3 = *In++;
            if((code3 & 0xC0) != 0x80) return -1;
            *Out = (code1 << 12) + (code2 << 6) + code3 - 0xE2080;
            return 3;
        }
        else if(code1 < 0xF5)
        {
            code2 = *In++;
            if((code2 & 0xC0) != 0x80) return -1;
            if(code1 == 0xF0 && code2 < 0x90) return -1;
            if(code1 == 0xF4 && code2 >= 0x90) return -1;
            code3 = *In++;
            if((code3 & 0xC0) != 0x80) return -1;
            code4 = *In++;
            if((code4 & 0xC0) != 0x80) return -1;
            *Out = (code1 << 18) + (code2 << 12) + (code3 << 6) + code4 - 0x3C82080;
            return 4;
        }
        return -1;
    }

    ssize_t Utf8To16(const u8 *In, u16 *Out, size_t Length)
    {
        ssize_t rc = 0;
        ssize_t units;
        uint32_t code;
        uint16_t encoded[2];
        do
        {
            units = DecodeUtf8(In, &code);
            if(units == -1) return -1;
            if(code > 0)
            {
                In += units;
                units = EncodeUtf16(code, encoded);
                if(units == -1) return -1;
                if(Out != NULL)
                {
                    if(rc + units <= Length)
                    {
                        *Out++ = encoded[0];
                        if(units > 1) *Out++ = encoded[1];
                    }
                }
                if(SSIZE_MAX - units >= rc) rc += units;
                else return -1;
            }
        } while(code > 0);
        return rc;
    }

    ssize_t Utf8To32(const u8 *In, u32 *Out, size_t Length)
    {
        ssize_t rc = 0;
        ssize_t units;
        uint32_t code;
        do
        {
            units = DecodeUtf8(In, &code);
            if(units == -1) return -1;
            if(code > 0)
            {
                In += units;
                if(Out != NULL)
                {
                    if(rc < Length) *Out++ = code;
                }
                if(SSIZE_MAX - 1 >= rc) ++rc;
                else return -1;
            }
        } while(code > 0);
        return rc;
    }
}