#include <bio/biocoenosis/fauna/utf/Utf32.hpp>
#include <bio/biocoenosis/fauna/utf/Utf8.hpp>
#include <bio/biocoenosis/fauna/utf/Utf16.hpp>
#include <sys/types.h>

namespace bio::utf
{
    ssize_t Utf32To8(const u32 *In, u8 *Out, size_t Length)
    {
        ssize_t rc = 0;
        ssize_t units;
        uint8_t encoded[4];
        while(*In > 0)
        {
            units = EncodeUtf8(*In++, encoded);
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
        return rc;
    }

    ssize_t Utf32To16(const u32 *In, u16 *Out, size_t Length)
    {
        ssize_t rc = 0;
        ssize_t units;
        uint16_t encoded[2];
        while(*In > 0)
        {
            units = EncodeUtf16(*In++, encoded);
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
        return rc;
    }
}