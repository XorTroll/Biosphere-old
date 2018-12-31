

#pragma once
#include <bio/biotope/svc.hpp>
#include <bio/biocoenosis/fauna/os/Kernel.hpp>
#include <bio/biocoenosis/fauna/os/Mutex.hpp>

namespace bio::os
{
    enum class Region
    {
        Stack = 0,
        Heap,
        NewStack,
        Max
    };

    struct VirtualRegion
    {
        u64 Start;
        u64 End;
    };

    namespace VirtualMemory
    {
        void *Reserve(size_t Size);
        void Free(void *Address, size_t Size);
        void *ReserveMap(size_t Size);
        void FreeMap(void *Address, size_t Size);
    }

    class SharedMemory
    {
        public:
            SharedMemory(size_t Size, Permission Local, Permission Remote);
            SharedMemory(u32 Handle, size_t Size, Permission Permissions);
            ~SharedMemory();
            Result Map();
            Result Unmap();
            void *GetAddress();
        private:
            u32 handle;
            size_t size;
            Permission perms;
            void *address;
    };

    class TransferMemory
    {
        public:
            TransferMemory(size_t Size, Permission Permissions);
            TransferMemory(u32 Handle, size_t Size, Permission Permissions);
            ~TransferMemory();
            Result Map();
            Result Unmap();
            void *GetAddress();
        private:
            u32 handle;
            size_t size;
            Permission perms;
            void *backaddress;
            void *mapaddress;
    };
}