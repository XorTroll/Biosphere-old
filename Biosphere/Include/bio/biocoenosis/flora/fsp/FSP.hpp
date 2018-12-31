
#pragma once
#include <bio/biocoenosis/flora/fsp/Types.hpp>

namespace bio::fsp
{
    class File : public hipc::Object
    {
        public:
            using Object::Object;
            Result SetSize(u64 Size);
            ResultWrap<u64> GetSize();
    };

    class Directory : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class FileSystem : public hipc::Object
    {
        public:
            using Object::Object;
            Result CreateFile(u32 Flags, u64 Size, const char *Path);
            Result DeleteFile(const char *Path);
            Result CreateDirectory(const char *Path);
            Result DeleteDirectory(const char *Path);
            Result DeleteDirectoryRecursively(const char *Path);
            Result RenameFile(const char *Path, const char *NewPath);
            Result RenameDirectory(const char *Path, const char *NewPath);
            ResultWrap<DirectoryEntryType> GetEntryType(const char *Path);
            ResultWrap<File*> OpenFile(u32 Mode, const char *Path);
            ResultWrap<Directory*> OpenDirectory(u32 Filter, const char *Path);
            Result Commit();
            ResultWrap<u64> GetFreeSpaceSize(const char *Path);
            ResultWrap<u64> GetTotalSpaceSize(const char *Path);
            Result CleanDirectoryRecursively(const char *Path);
    };

    class FspService : public hipc::Object
    {
        public:
            using Object::Object;
            Result Initialize();
            ResultWrap<FileSystem*> OpenSdCardFileSystem();
    };

    ResultWrap<FspService*> Initialize(sm::ServiceManager *SM);
}