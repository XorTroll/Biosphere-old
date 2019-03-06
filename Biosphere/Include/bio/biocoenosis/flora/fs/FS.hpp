
#pragma once
#include <bio/biocoenosis/flora/fs/Device.hpp>

namespace bio::fs
{
    void Initialize();
    void Finalize();
    Result Mount(fsp::FileSystem *FS, const char *DeviceName);
    Result MountSdCard(const char *DeviceName);
    Result MountRom(const char *DeviceName);
    Result MountBis(fsp::Partition PartitionId, const char *DeviceName);
    bool CheckMountName(const char *DeviceName);
    void Unregister(const char *DeviceName);
    void Unmount(const char *DeviceName);
    fsp::FileSystem *GetFileSystem(const char *DeviceName);
    void UnmountAllDevices();
}