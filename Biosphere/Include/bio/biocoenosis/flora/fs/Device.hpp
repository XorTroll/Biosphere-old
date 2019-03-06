
#pragma once
#include <bio/biocoenosis/flora/fsp/FSP.hpp>
#include <sys/dirent.h>
#include <sys/iosupport.h>
#include <sys/param.h>

namespace bio::fs
{
    struct Device
    {
        s32 Id;
        bool Valid;
        devoptab_t InternalDevice;
        fsp::FileSystem *Session;
        char Name[0x20];
    };

    struct DeviceFile
    {

    };
    
    Device *FindDevice(const char *Name);
}