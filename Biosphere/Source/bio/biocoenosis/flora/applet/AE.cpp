#include <bio/biocoenosis/flora/applet/AE.hpp>

namespace bio::applet::ae
{
    ResultWrap<CommonStateGetter*> SystemAppletProxy::GetCommonStateGetter()
    {
        u32 ocsg = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutObjectId<0>(ocsg));
        return ResultWrap<CommonStateGetter*>(rc, new CommonStateGetter(this, ocsg));
    }

    ResultWrap<SelfController*> SystemAppletProxy::GetSelfController()
    {
        u32 osch = 0;
        Result rc = this->ProcessRequest<1>(hipc::OutObjectId<0>(osch));
        return ResultWrap<SelfController*>(rc, new SelfController(this, osch));
    }

    ResultWrap<LibraryAppletCreator*> SystemAppletProxy::GetLibraryAppletCreator()
    {
        u32 olac = 0;
        Result rc = this->ProcessRequest<11>(hipc::OutObjectId<0>(olac));
        return ResultWrap<LibraryAppletCreator*>(rc, new LibraryAppletCreator(this, olac));
    }

    ResultWrap<ApplicationCreator*> SystemAppletProxy::GetApplicationCreator()
    {
        u32 oac = 0;
        Result rc = this->ProcessRequest<22>(hipc::OutObjectId<0>(oac));
        return ResultWrap<ApplicationCreator*>(rc, new ApplicationCreator(this, oac));
    }

    ResultWrap<WindowController*> SystemAppletProxy::GetWindowController()
    {
        u32 owc = 0;
        Result rc = this->ProcessRequest<2>(hipc::OutObjectId<0>(owc));
        return ResultWrap<WindowController*>(rc, new WindowController(this, owc));
    }

    ResultWrap<HomeMenuFunctions*> SystemAppletProxy::GetHomeMenuFunctions()
    {
        u32 ohmf = 0;
        Result rc = this->ProcessRequest<20>(hipc::OutObjectId<0>(ohmf));
        return ResultWrap<HomeMenuFunctions*>(rc, new HomeMenuFunctions(this, ohmf));
    }

    ResultWrap<SelfController*> LibraryAppletProxy::GetSelfController()
    {
        u32 osch = 0;
        Result rc = this->ProcessRequest<1>(hipc::OutObjectId<0>(osch));
        return ResultWrap<SelfController*>(rc, new SelfController(this, osch));
    }

    ResultWrap<CommonStateGetter*> LibraryAppletProxy::GetCommonStateGetter()
    {
        u32 ocsg = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutObjectId<0>(ocsg));
        return ResultWrap<CommonStateGetter*>(rc, new CommonStateGetter(this, ocsg));
    }

    ResultWrap<LibraryAppletCreator*> LibraryAppletProxy::GetLibraryAppletCreator()
    {
        u32 olac = 0;
        Result rc = this->ProcessRequest<11>(hipc::OutObjectId<0>(olac));
        return ResultWrap<LibraryAppletCreator*>(rc, new LibraryAppletCreator(this, olac));
    }

    ResultWrap<SystemAppletProxy*> AeService::OpenSystemAppletProxy(u64 Reserved)
    {
        u32 osap = 0;
        Result rc = this->ProcessRequest<100>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutObjectId<0>(osap));
        return ResultWrap<SystemAppletProxy*>(rc, new SystemAppletProxy(this, osap));
    }

    ResultWrap<LibraryAppletProxy*> AeService::OpenLibraryAppletProxyOld(u64 Reserved)
    {
        u32 olap = 0;
        Result rc = this->ProcessRequest<200>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutObjectId<0>(olap));
        return ResultWrap<LibraryAppletProxy*>(rc, new LibraryAppletProxy(this, olap));
    }

    ResultWrap<OverlayAppletProxy*> AeService::OpenOverlayAppletProxy(u64 Reserved)
    {
        u32 ooap = 0;
        Result rc = this->ProcessRequest<300>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutObjectId<0>(ooap));
        return ResultWrap<OverlayAppletProxy*>(rc, new OverlayAppletProxy(this, ooap));
    }

    ResultWrap<ApplicationProxy*> AeService::OpenSystemApplicationProxy(u64 Reserved)
    {
        u32 osap = 0;
        Result rc = this->ProcessRequest<350>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutObjectId<0>(osap));
        return ResultWrap<ApplicationProxy*>(rc, new ApplicationProxy(this, osap));
    }

    ResultWrap<AeService*> Initialize()
    {
        auto srv = sm::GetService("appletAE");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        if(rc.IsSuccess()) rc = osrv->ConvertToDomain();
        return ResultWrap<AeService*>(rc, static_cast<AeService*>(osrv));
    }
}