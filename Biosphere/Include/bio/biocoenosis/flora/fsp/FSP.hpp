
#pragma once
#include <bio/biocoenosis/flora/fsp/Types.hpp>

namespace bio::fsp
{
    class File : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<u64> Read(u64 Offset, void *Buffer, size_t Size);
            Result Write(u64 Offset, void *Buffer, size_t Size);
            Result Flush();
            Result SetSize(u64 Size);
            ResultWrap<u64> GetSize();
    };

    class Directory : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<u64> Read(DirectoryEntry *Buffer, size_t MaxEntries);
            ResultWrap<u64> GetEntryCount();
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

    ResultWrap<FspService*> Initialize();

    static const u32 Module = 2;
    static const Result ResultPathDoesntExist(Module, 1);
    static const Result ResultPathAlreadyExists(Module, 2);
    static const Result ResultResourceBusy(Module, 7);
    static const Result ResultInvalidInput(Module, 6001);
    static const Result ResultPathTooLong(Module, 6003);
}