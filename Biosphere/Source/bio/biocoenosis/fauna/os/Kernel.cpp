#include <bio/biocoenosis/fauna/os/Kernel.hpp>

extern int __argc;
extern char **__argv;
extern void *heapaddr;
extern bool lnso;
extern char *fake_heap_end;
extern u64 heapsize;

std::vector<bio::os::FinalizeCheckInfo> fchecks;

namespace bio::os
{
    static bool hpov = false;
    static void *cheapaddr;

    HandleObject::HandleObject(u32 Handle)
    {
        this->handle = Handle;
    }

    u32 HandleObject::GetHandle()
    {
        return this->handle;
    }

    int GetHostArgc()
    {
        return __argc;
    }

    char **GetHostArgv()
    {
        return __argv;
    }

    FirmwareMajor GetFirmwareMajorVersion()
    {
        u64 info = 0;
        bool av = (svc::GetInfo(&info, 21, 0, 0) != 0xf001);
        if(av) return FirmwareMajor::Major6;
        av = (svc::GetInfo(&info, 20, 0, 0) != 0xf001);
        if(av) return FirmwareMajor::Major5;
        av = (svc::GetInfo(&info, 19, 0, 0) != 0xf001);
        if(av) return FirmwareMajor::Major4;
        av = (svc::GetInfo(&info, 18, 0, 0) != 0xf001);
        if(av) return FirmwareMajor::Major3;
        av = (svc::GetInfo(&info, 12, 0, 0) != 0xf001);
        if(av) return FirmwareMajor::Major2;
        return FirmwareMajor::Major1;
    }

    u32 GetCurrentProcessHandle()
    {
        return 0xffff8001;
    }

    u64 EncodeString(const char *Text)
    {
        u64 enc = 0;
        for(u32 i = 0; i < 8; i++)
        {
            if(Text[i] == '\0') break;
            enc |= (((u64)Text[i]) << (8 * i));
        }
        return enc;
    }

    Executable GetExecutableType()
    {
        return (lnso ? Executable::NSO : Executable::NRO);
    }

    Result OverrideHeap(u64 CustomSize)
    {
        if(IsHeapOverrided()) RestoreOverridedHeap();
        Result rc = svc::SetHeapSize(&cheapaddr, CustomSize);
        if(rc.IsFailure()) return rc;
        fake_heap_end = (char*)cheapaddr + CustomSize;
        hpov = true;
        return rc;
    }

    bool IsHeapOverrided()
    {
        return hpov;
    }

    void RestoreOverridedHeap()
    {
        if(hpov)
        {
            svc::SetHeapSize(&cheapaddr, ((u8*)heapaddr + heapsize) - (u8*)cheapaddr);
            hpov = false;
        }
    }

    void AddFinalizeCheckingFor(std::function<bool()> CheckCallback, std::function<void()> FinalizeCallback)
    {
        fchecks.push_back({ CheckCallback, FinalizeCallback });
    }
}