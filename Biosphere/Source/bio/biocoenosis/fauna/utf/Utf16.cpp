#include <bio/biocoenosis/fauna/utf/Utf16.hpp>
#include <bio/biocoenosis/fauna/utf/Utf8.hpp>
#include <sys/types.h>

namespace bio::utf
{
    ssize_t EncodeUtf16(u32 In, u16 *Out)
    {
        if(In < 0x10000)
        {
            if(Out != NULL) *Out++ = In;
            return 1;
        }
        else if(In < 0x110000)
        {
            if(Out != NULL)
            {
                *Out++ = (In >> 10) + 0xD7C0;
                *Out++ = (In & 0x3FF) + 0xDC00;
            }
            return 2;
        }
        return -1;
    }

    ssize_t DecodeUtf16(const u16 *In, u32 *Out)
    {
        uint16_t code1, code2;
        code1 = *In++;
        if(code1 >= 0xD800 && code1 < 0xDC00)
        {
            code2 = *In++;
            if(code2 >= 0xDC00 && code2 < 0xE000)
            {
                *Out = (code1 << 10) + code2 - 0x35FDC00;
                return 2;
            }
            return -1;
        }
        *Out = code1;
        return 1;
    }

    ssize_t Utf16To8(const u16 *In, u8 *Out, size_t Length)
    {
        ssize_t rc = 0;
        ssize_t units;
        uint32_t code;
        uint8_t encoded[4];
        do
        {
            units = DecodeUtf16(In, &code);
            if(units == -1) return -1;
            if(code > 0)
            {
                In += units;
                units = EncodeUtf8(code, encoded);
                if(units == -1) return -1;
                if(Out != NULL)
                {
                    if(rc + units <= Length)
                    {
                        *Out++ = encoded[0];
                        if(units > 1) *Out++ = encoded[1];
                        if(units > 2) *Out++ = encoded[2];
                        if(units > 3) *Out++ = encoded[3];
                    }
                }
                if(SSIZE_MAX - units >= rc) rc += units;
                else return -1;
            }
        } while(code > 0);
        return rc;
    }

    ssize_t Utf16To32(const u16 *In, u32 *Out, size_t Length)
    {
        ssize_t rc = 0;
        ssize_t units;
        uint32_t code;
        do
        {
            units = DecodeUtf16(In, &code);
            if(units == -1) return -1;
            if(code > 0)
            {
                In += units;
                if(Out != NULL)
                {
                    if(rc < Length)
                    *Out++ = code;
                }
                if(SSIZE_MAX - 1 >= rc) ++rc;
                else return -1;
            }
        } while(code > 0);
        return rc;
    }
}