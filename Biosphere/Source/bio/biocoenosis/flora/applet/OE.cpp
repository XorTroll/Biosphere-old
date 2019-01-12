#include <bio/biocoenosis/flora/applet/OE.hpp>

namespace bio::applet::oe
{
    ResultWrap<ApplicationProxy*> OeService::OpenApplicationProxy(u64 Reserved)
    {
        u32 oap = 0;
        Result rc = this->ProcessRequest<0>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutObjectId<0>(oap));
        return ResultWrap<ApplicationProxy*>(rc, new ApplicationProxy(this, oap));
    }

    ResultWrap<OeService*> Initialize()
    {
        auto srv = sm::GetService("appletOE");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        if(rc.IsSuccess()) rc = osrv->ConvertToDomain();
        return ResultWrap<OeService*>(rc, static_cast<OeService*>(osrv));
    }
}