#include <bio/biocoenosis/fauna/os/Memory.hpp>
#include <bio/biocoenosis/flora/diag.hpp>

extern bio::os::VirtualRegion addrspace;
extern bio::os::VirtualRegion regs[4];
bio::os::Mutex vmutex;
u64 curaddr;
u64 curmapaddr;

namespace bio::os
{
    void *VirtualMemory::Reserve(size_t Size)
    {
        u32 rc = 0;
        MemoryInfo meminfo;
        u32 pageinfo;
        u32 i = 0;
        Size = (Size + 0xfff) &~ 0xfff;
        vmutex.Lock();
        u64 addr = curaddr;
        while(true)
        {
            addr += 0x1000;
            if(!((addr >= addrspace.Start) && (addr < addrspace.End))) addr = addrspace.Start;
            rc = svc::QueryMemory(&meminfo, &pageinfo, addr);
            if(rc != 0)
            {
                if(!bio::diag::HasInitialized()) bio::diag::Initialize(bio::sm::Initialize().AssertOk());
                bio::diag::AssertResultOk(bio::ResultMemoryQuery);
            }
            if(meminfo.Type != 0)
            {
                addr = meminfo.Address + meminfo.Size;
                continue;
            }
            if(Size > meminfo.Size)
            {
                addr = meminfo.Address + meminfo.Size;
                continue;
            }
            for(i = 0; i < 3; i++)
            {
                u64 end = (addr + Size - 1);
                if((addr >= regs[i].Start) && (addr < regs[i].End)) break;
                if((end >= regs[i].Start) && (end < regs[i].End)) break;
            }
            if(i != 3)
            {
                addr = regs[i].End;
                continue;
            }
            break;
        }
        curaddr = addr + Size;
        vmutex.Unlock();
        return (void*)addr;
    }

    void VirtualMemory::Free(void *Address, size_t Size)
    {
        BIO_IGNORE(Address);
        BIO_IGNORE(Size);
    }

    void *VirtualMemory::ReserveMap(size_t Size)
    {
        u32 rc = 0;
        MemoryInfo meminfo;
        u32 pageinfo = 0;
        int regidx = ((static_cast<u8>(GetFirmwareMajorVersion()) > 2) ? static_cast<int>(Region::NewStack) : static_cast<int>(Region::Stack));
        Size = (Size + 0xfff) &~ 0xfff;
        vmutex.Lock();
        u64 addr = curaddr;
        while(true)
        {
            addr += 0x1000;
            if(!((addr >= regs[regidx].Start) && (addr < regs[regidx].End))) addr = regs[regidx].Start;
            rc = svc::QueryMemory(&meminfo, &pageinfo, addr);
            if(rc != 0)
            {
                if(!bio::diag::HasInitialized()) bio::diag::Initialize(bio::sm::Initialize().AssertOk());
                bio::diag::AssertResultOk(bio::ResultMemoryQuery);
            }
            if(meminfo.Type != 0)
            {
                addr = (meminfo.Address + meminfo.Size);
                continue;
            }
            if(Size > meminfo.Size)
            {
                addr = (meminfo.Address + meminfo.Size);
                continue;
            }
            break;
        }
        curaddr = (addr + Size);
        vmutex.Unlock();
        return (void*)addr;
    }

    void VirtualMemory::FreeMap(void *Address, size_t Size)
    {
        BIO_IGNORE(Address);
        BIO_IGNORE(Size);
    }

    SharedMemory::SharedMemory(size_t Size, Permission Local, Permission Remote)
    {
        this->handle = 0;
        this->size = Size;
        this->address = NULL;
        this->perms = Local;
        svc::CreateSharedMemory(&this->handle, this->size, static_cast<u32>(Local), static_cast<u32>(Remote));
    }

    SharedMemory::SharedMemory(u32 Handle, size_t Size, Permission Permissions)
    {
        this->handle = Handle;
        this->size = Size;
        this->address = NULL;
        this->perms = Permissions;
    }

    SharedMemory::~SharedMemory()
    {
        u32 rc = 0;
        if(this->address != NULL) rc = this->Unmap();
        if(rc == 0)
        {
            if(this->handle != 0) rc = svc::CloseHandle(this->handle);
            this->handle = 0;
        }
    }

    Result SharedMemory::Map()
    {
        u32 rc = 0;
        if(this->address == NULL)
        {
            void *add = VirtualMemory::Reserve(this->size);
            rc = svc::MapSharedMemory(this->handle, add, this->size, static_cast<u32>(this->perms));
            if(rc == 0) this->address = add;
            else VirtualMemory::Free(add, this->size);
        }
        return Result(rc);
    }

    Result SharedMemory::Unmap()
    {
        u32 rc = svc::UnmapSharedMemory(this->handle, this->address, this->size);
        if(rc == 0)
        {
            VirtualMemory::Free(this->address, this->size);
            this->address = NULL;
        }
        return Result(rc);
    }

    void *SharedMemory::GetAddress()
    {
        return this->address;
    }

    TransferMemory::TransferMemory(size_t Size, Permission Permissions)
    {
        this->handle = 0;
        this->size = Size;
        this->perms = Permissions;
        this->mapaddress = NULL;
        this->backaddress = memalign(0x1000, Size);
        u32 rc = 0;
        if(this->backaddress != NULL)
        {
            memset(this->backaddress, 0, Size);
            rc = svc::CreateTransferMemory(&this->handle, this->backaddress, Size, static_cast<u32>(Permissions));
        }
        if(rc != 0)
        {
            free(this->backaddress);
            this->backaddress = NULL;
        }
    }

    TransferMemory::TransferMemory(u32 Handle, size_t Size, Permission Permissions)
    {
        this->handle = Handle;
        this->size = Size;
        this->perms = Permissions;
        this->mapaddress = NULL;
        this->backaddress = NULL;
    }

    TransferMemory::~TransferMemory()
    {
        u32 rc = 0;
        if(this->mapaddress != NULL) rc = this->Unmap();
        if(rc == 0)
        {
            if(this->handle != 0) rc = svc::CloseHandle(this->handle);
            this->handle = 0;
        }
        if(this->backaddress != NULL)
        {
            free(this->backaddress);
            this->backaddress = NULL;
        }
    }

    Result TransferMemory::Map()
    {
        u32 rc = 0;
        if(this->mapaddress == NULL)
        {
            void *add = VirtualMemory::Reserve(this->size);
            rc = svc::MapTransferMemory(this->handle, add, this->size, static_cast<u32>(this->perms));
            if(rc == 0) this->mapaddress = add;
            else VirtualMemory::Free(add, this->size);
        }
        return rc;
    }

    Result TransferMemory::Unmap()
    {
        u32 rc = svc::UnmapTransferMemory(this->handle, this->mapaddress, this->size);
        if(rc == 0)
        {
            VirtualMemory::Free(this->mapaddress, this->size);
            this->mapaddress = NULL;
        }
        return rc;
    }

    void *TransferMemory::GetAddress()
    {
        return this->mapaddress;
    }
}