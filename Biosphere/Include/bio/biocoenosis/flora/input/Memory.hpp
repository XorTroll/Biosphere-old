
#pragma once
#include <bio/biocoenosis/flora/hid.hpp>

namespace bio::input
{
    enum class Controller
    {
        Player1,
        Player2,
        Player3,
        Player4,
        Player5,
        Player6,
        Player7,
        Player8,
        HandHeld,
        Unknown,
    };

    enum Key
    {
        A = BIO_BITMASK(0),
        B = BIO_BITMASK(1),
        X = BIO_BITMASK(2),
        Y = BIO_BITMASK(3),
        LStick = BIO_BITMASK(4),
        RStick = BIO_BITMASK(5),
        L = BIO_BITMASK(6),
        R = BIO_BITMASK(7),
        ZL = BIO_BITMASK(8),
        ZR = BIO_BITMASK(9),
        Plus = BIO_BITMASK(10),
        Minus = BIO_BITMASK(11),
        Left = BIO_BITMASK(12),
        Right = BIO_BITMASK(13),
        Up = BIO_BITMASK(14),
        Down = BIO_BITMASK(15),
        LStickLeft = BIO_BITMASK(16),
        LStickUp = BIO_BITMASK(17),
        LStickRight = BIO_BITMASK(18),
        LStickDown = BIO_BITMASK(19),
        RStickLeft = BIO_BITMASK(20),
        RStickUp = BIO_BITMASK(21),
        RStickRight = BIO_BITMASK(22),
        RStickDown = BIO_BITMASK(23),
        SLLeft = BIO_BITMASK(24),
        SRLeft = BIO_BITMASK(25),
        SLRight = BIO_BITMASK(26),
        SRRight = BIO_BITMASK(27),
        Touch = BIO_BITMASK(28),
    };

    struct TouchData
    {
        u64 Timestamp;
        u32 Pad1;
        u32 Index;
        u32 X;
        u32 Y;
        u32 DiameterX;
        u32 DiameterY;
        u32 Angle;
        u32 Pad2;
    };

    struct TouchEntry
    {
        u64 Timestamp;
        u64 Count;
        TouchData Touches[16];
        u64 Pad1;
    };

    struct TouchState
    {
        u64 TimestampTicks;
        u64 EntryCount;
        u64 LatestIndex;
        u64 MaxIndex;
        u64 Timestamp;
        TouchEntry Entries[17];
    };

    struct JoystickPosition
    {
        u32 DX;
        u32 DY;
    };

    struct ControllerStateEntry
    {
        u64 Timestamp;
        u64 Timestamp2;
        u64 ButtonState;
        JoystickPosition LeftPosition;
        JoystickPosition RightPosition;
        u64 ConnectionState;
    };

    struct ControllerState
    {
        u64 Timestamp;
        u64 EntryCount;
        u64 LatestIndex;
        u64 MaxIndex;
        ControllerStateEntry Entries[17];
    };

    struct ControllerMACAddress
    {
        u8 Address[0x10];
    };

    struct ControllerColor
    {
        u32 Body;
        u32 Buttons;
    };

    struct ControllerData
    {
        u32 Status;
        u32 IsJoyConHalf;
        u32 SingleColorsDescriptor;
        ControllerColor SingleColor;
        u32 SplitColorsDescriptor;
        ControllerColor RightColor;
        ControllerColor LeftColor;
        ControllerState ProController;
        ControllerState HandheldJoined;
        ControllerState Joined;
        ControllerState Left;
        ControllerState Right;
        ControllerState MainNoAnalog;
        ControllerState Main;
        u8 Unknown[0x2a78];
        ControllerMACAddress MAC[0x2];
        u8 Unknown2[0xe10];
    };

    struct InputMemory
    {
        u8 Header[0x400];
        TouchState Touch;
        u8 Pad[0x3c0];
        u8 Mouse[0x400];
        u8 Keyboard[0x400];
        u8 Unknown[0x400];
        u8 Unknown2[0x400];
        u8 Unknown3[0x400];
        u8 Unknown4[0x400];
        u8 Unknown5[0x200];
        u8 Unknown6[0x200];
        u8 Unknown7[0x200];
        u8 Unknown8[0x800];
        u8 ControllerSerials[0x4000];
        ControllerData Controllers[10];
        u8 Unknown9[0x4600];
    };

    struct Touch
    {
        u32 X;
        u32 Y;
        u32 DiameterX;
        u32 DiameterY;
        u32 Angle;
    };
}