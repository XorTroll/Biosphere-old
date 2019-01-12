
#pragma once
#include <bio/biocoenosis/fauna/sm.hpp>

namespace bio::gpio
{
    enum class PadName
    {
        AudioCodec = 0x1,
        JoyConLeftAttach = 0xc,
        Power = 0x18,
        VolumeUp = 0x19,
        VolumeDown = 0x1a,
        JoyConRightAttach = 0x34,
    };

    enum class Direction
    {
        Input,
        Output,
    };

    enum class Value
    {
        Low,
        High,  
    };
}