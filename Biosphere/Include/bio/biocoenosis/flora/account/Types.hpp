
#pragma once
#include <bio/biocoenosis/fauna/sm.hpp>

namespace bio::account
{
    typedef u128 Uid;

    struct UserData
    {
        u32 Unknown;
        u32 IconId;
        u8 IconBgColorId;
        u8 Unknown2[0x7];
        u8 MiiData[0x10];
        u8 Unknown3[0x60];
    } BIO_PACKED;

    struct ProfileBase
    {
        Uid UserId;
        u64 LastEditTimeStamp;
        char Nickname[0x20];
    } BIO_PACKED;

    class Profile : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class ProfileEditor : public hipc::Object
    {
        public:
            using Object::Object;
    };
}