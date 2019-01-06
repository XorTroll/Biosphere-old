#include <bio/biocoenosis/flora/fsp/FSP.hpp>

namespace bio::fsp
{
    ResultWrap<u64> File::Read(u64 Offset, void *Buffer, size_t Size)
    {
        u64 read = 0;
        Result rc = this->ProcessRequest<0>(hipc::InRaw<u64>(0), hipc::InRaw<u64>(Offset), hipc::InRaw<u64>(Size), hipc::OutBuffer(Buffer, Size, 1), hipc::OutRaw<u64>(read));
        return ResultWrap<u64>(rc, read);
    }

    Result File::Write(u64 Offset, void *Buffer, size_t Size)
    {
        return this->ProcessRequest<1>(hipc::InRaw<u64>(0), hipc::InRaw<u64>(Offset), hipc::InRaw<u64>(Size), hipc::InBuffer(Buffer, Size, 1));
    }

    Result File::Flush()
    {
        return this->ProcessRequest<2>(hipc::Simple());
    }

    Result File::SetSize(u64 Size)
    {
        return this->ProcessRequest<3>(hipc::InRaw<u64>(Size));
    }

    ResultWrap<u64> File::GetSize()
    {
        u64 size = 0;
        Result rc = this->ProcessRequest<4>(hipc::OutRaw<u64>(size));
        return ResultWrap<u64>(rc, size);
    }

    ResultWrap<u64> Directory::Read(DirectoryEntry *Buffer, size_t MaxEntries)
    {
        u64 total = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutBuffer(Buffer, (sizeof(DirectoryEntry) * MaxEntries), 0), hipc::InRaw<u64>(0), hipc::OutRaw<u64>(total));
        return ResultWrap<u64>(rc, total);
    }

    ResultWrap<u64> Directory::GetEntryCount()
    {
        u64 count = 0;
        Result rc = this->ProcessRequest<1>(hipc::OutRaw<u64>(count));
        return ResultWrap<u64>(rc, count);
    }

    Result FileSystem::CreateFile(u32 Flags, u64 Size, const char *Path)
    {
        return this->ProcessRequest<0>(hipc::InStaticBuffer((char*)Path, 0x301, 0), hipc::InRaw<u64>(0), hipc::InRaw<u64>(Size), hipc::InRaw<u32>(Flags));
    }

    Result FileSystem::DeleteFile(const char *Path)
    {
        return this->ProcessRequest<1>(hipc::InStaticBuffer((char*)Path, 0x301, 0));
    }

    Result FileSystem::CreateDirectory(const char *Path)
    {
        return this->ProcessRequest<2>(hipc::InStaticBuffer((char*)Path, 0x301, 0));
    }

    Result FileSystem::DeleteDirectory(const char *Path)
    {
        return this->ProcessRequest<3>(hipc::InStaticBuffer((char*)Path, 0x301, 0));
    }

    Result FileSystem::DeleteDirectoryRecursively(const char *Path)
    {
        return this->ProcessRequest<4>(hipc::InStaticBuffer((char*)Path, 0x301, 0));
    }

    Result FileSystem::RenameFile(const char *Path, const char *NewPath)
    {
        return this->ProcessRequest<5>(hipc::InStaticBuffer((char*)Path, 0x301, 0), hipc::InStaticBuffer((char*)NewPath, 0x301, 1));
    }

    Result FileSystem::RenameDirectory(const char *Path, const char *NewPath)
    {
        return this->ProcessRequest<6>(hipc::InStaticBuffer((char*)Path, 0x301, 0), hipc::InStaticBuffer((char*)NewPath, 0x301, 1));
    }

    ResultWrap<DirectoryEntryType> FileSystem::GetEntryType(const char *Path)
    {
        u32 detype = 0;
        Result rc = this->ProcessRequest<7>(hipc::InStaticBuffer((char*)Path, 0x301, 0), hipc::OutRaw<u32>(detype));
        return ResultWrap<DirectoryEntryType>(rc, static_cast<DirectoryEntryType>(detype));
    }

    ResultWrap<File*> FileSystem::OpenFile(u32 Mode, const char *Path)
    {
        u32 fh = 0;
        Result rc = this->ProcessRequest<8>(hipc::InRaw<u32>(Mode), hipc::InStaticBuffer((char*)Path, 0x301, 0), hipc::OutHandle<0>(fh));
        return ResultWrap<File*>(rc, new File(fh));
    }

    ResultWrap<Directory*> FileSystem::OpenDirectory(u32 Filter, const char *Path)
    {
        u32 dh = 0;
        Result rc = this->ProcessRequest<9>(hipc::InRaw<u32>(Filter), hipc::InStaticBuffer((char*)Path, 0x301, 0), hipc::OutHandle<0>(dh));
        return ResultWrap<Directory*>(rc, new Directory(dh));
    }

    Result FileSystem::Commit()
    {
        return this->ProcessRequest<10>(hipc::Simple());
    }

    ResultWrap<u64> FileSystem::GetFreeSpaceSize(const char *Path)
    {
        u64 fss = 0;
        Result rc = this->ProcessRequest<11>(hipc::InStaticBuffer((char*)Path, 0x301, 0), hipc::OutRaw<u64>(fss));
        return ResultWrap<u64>(rc, fss);
    }

    ResultWrap<u64> FileSystem::GetTotalSpaceSize(const char *Path)
    {
        u64 tss = 0;
        Result rc = this->ProcessRequest<12>(hipc::InStaticBuffer((char*)Path, 0x301, 0), hipc::OutRaw<u64>(tss));
        return ResultWrap<u64>(rc, tss);
    }

    Result FileSystem::CleanDirectoryRecursively(const char *Path)
    {
        if(os::GetFirmwareMajorVersion() < os::FirmwareMajor::Major3) return 0;
        return this->ProcessRequest<13>(hipc::InStaticBuffer((char*)Path, 0x301, 0));
    }

    Result FspService::Initialize()
    {
        return this->ProcessRequest<1>(hipc::InProcessId(), hipc::InRaw<u64>(0));
    }

    ResultWrap<FileSystem*> FspService::OpenSdCardFileSystem()
    {
        u32 fsh = 0;
        Result rc = this->ProcessRequest<18>(hipc::OutHandle<0>(fsh));
        return ResultWrap<FileSystem*>(rc, new FileSystem(fsh));
    }

    ResultWrap<FspService*> Initialize(sm::ServiceManager *SM)
    {
        auto srv = SM->GetService("fsp-srv");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<FspService*>(rc, static_cast<FspService*>(osrv));
    }
}