#include <bio/biocoenosis/flora/fsp/FSP.hpp>

namespace bio::fsp
{
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