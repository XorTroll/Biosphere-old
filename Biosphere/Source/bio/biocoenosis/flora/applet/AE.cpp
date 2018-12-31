#include <bio/biocoenosis/flora/applet/AE.hpp>

namespace bio::applet::ae
{
    ResultWrap<CommonStateGetter*> SystemAppletProxy::GetCommonStateGetter()
    {
        u32 ch = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutHandle<0>(ch));
        return ResultWrap<CommonStateGetter*>(rc, new CommonStateGetter(ch));
    }

    ResultWrap<SelfController*> SystemAppletProxy::GetSelfController()
    {
        u32 sch = 0;
        Result rc = this->ProcessRequest<1>(hipc::OutHandle<0>(sch));
        return ResultWrap<SelfController*>(rc, new SelfController(sch));
    }

    ResultWrap<SystemAppletProxy*> AeService::OpenSystemAppletProxy(u64 Reserved)
    {
        u32 aph = 0;
        Result rc = this->ProcessRequest<100>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutHandle<0>(aph));
        return ResultWrap<SystemAppletProxy*>(rc, new SystemAppletProxy(aph));
    }

    ResultWrap<LibraryAppletProxy*> AeService::OpenLibraryAppletProxyOld(u64 Reserved)
    {
        u32 aph = 0;
        Result rc = this->ProcessRequest<200>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutObjectId<0>(aph));
        return ResultWrap<LibraryAppletProxy*>(rc, new LibraryAppletProxy(this, aph));
    }

    ResultWrap<OverlayAppletProxy*> AeService::OpenOverlayAppletProxy(u64 Reserved)
    {
        u32 aph = 0;
        Result rc = this->ProcessRequest<300>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutHandle<0>(aph));
        return ResultWrap<OverlayAppletProxy*>(rc, new OverlayAppletProxy(aph));
    }

    ResultWrap<ApplicationProxy*> AeService::OpenSystemApplicationProxy(u64 Reserved)
    {
        u32 aph = 0;
        Result rc = this->ProcessRequest<350>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutHandle<0>(aph));
        return ResultWrap<ApplicationProxy*>(rc, new ApplicationProxy(aph));
    }

    ResultWrap<AeService*> Initialize(sm::ServiceManager *SM)
    {
        auto srv = SM->GetService("appletAE");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        if(rc.IsSuccess()) rc = osrv->ConvertToDomain();
        return ResultWrap<AeService*>(rc, static_cast<AeService*>(osrv));
    }
}