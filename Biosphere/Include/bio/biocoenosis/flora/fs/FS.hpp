
#pragma once
#include <bio/biocoenosis/flora/fs/FS.hpp>
#include <bio/biocoenosis/flora/fsp/FSP.hpp>

namespace bio::fs
{
    void Initialize(sm::ServiceManager *SM);
    Result Mount(fsp::FileSystem *FS, const char *DeviceName);
    Result MountSdCard(const char *DeviceName);
    Result MountRom(const char *DeviceName);
    Result MountBis(fsp::Partition PartitionId, const char *DeviceName);
    bool CheckMountName(const char *DeviceName);
    Result Unmount(const char *DeviceName);
    fsp::FileSystem *GetFileSystem(const char *DeviceName);
    Result UnmountAllDevices();
}