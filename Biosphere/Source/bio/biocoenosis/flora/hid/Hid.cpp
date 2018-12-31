#include <bio/biocoenosis/flora/hid/Hid.hpp>

namespace bio::hid
{
    ResultWrap<u32> AppletResource::GetSharedMemoryHandle()
    {
        u32 shh = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutHandle<0>(shh));
        return ResultWrap<u32>(rc, shh);
    }

    ResultWrap<AppletResource*> HidService::CreateAppletResource(u64 AppletResourceUserId)
    {
        u32 arh = 0;
        Result rc = this->ProcessRequest<0>(hipc::InProcessId(), hipc::InRaw<u64>(AppletResourceUserId), hipc::OutHandle<0>(arh));
        return ResultWrap<AppletResource*>(rc, new AppletResource(arh));
    }

    Result HidService::SetSupportedNpadStyleSet(u32 NpadStyleTag, u64 AppletResourceUserId)
    {
        return this->ProcessRequest<100>(hipc::InProcessId(), hipc::InRaw<u32>(NpadStyleTag), hipc::InRaw<u64>(AppletResourceUserId));
    }

    Result HidService::SetSupportedNpadIdType(u64 AppletResourceUserId, u32 *Controllers, size_t ControllerCount)
    {
        return this->ProcessRequest<102>(hipc::InProcessId(), hipc::InStaticBuffer(Controllers, (ControllerCount * sizeof(u32)), 0), hipc::InRaw<u64>(AppletResourceUserId));
    }

    Result HidService::ActivateNpad(u64 AppletResourceUserId)
    {
        return this->ProcessRequest<103>(hipc::InProcessId(), hipc::InRaw<u64>(AppletResourceUserId));
    }

    Result HidService::SetNpadJoyAssignmentModeSingle(u32 Controller, u64 AppletResourceUserId, u64 JoyType)
    {
        return this->ProcessRequest<123>(hipc::InProcessId(), hipc::InRaw<u32>(Controller), hipc::InRaw<u64>(AppletResourceUserId), hipc::InRaw<u64>(JoyType));
    }

    Result HidService::SetNpadJoyAssignmentModeDual(u32 Controller, u64 AppletResourceUserId)
    {
        return this->ProcessRequest<124>(hipc::InProcessId(), hipc::InRaw<u32>(Controller), hipc::InRaw<u64>(AppletResourceUserId));
    }

    ResultWrap<HidService*> Initialize(sm::ServiceManager *SM)
    {
        auto srv = SM->GetService("hid");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<HidService*>(rc, static_cast<HidService*>(osrv));
    }
}