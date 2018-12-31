#include <bio/biocoenosis/flora/applet/OE.hpp>

namespace bio::applet::oe
{
    ResultWrap<ApplicationProxy*> OeService::OpenApplicationProxy(u64 Reserved)
    {
        u32 aph = 0;
        Result rc = this->ProcessRequest<0>(hipc::InProcessId(), hipc::InHandle<hipc::HandleMode::Copy>(os::GetCurrentProcessHandle()), hipc::InRaw<u64>(Reserved), hipc::OutHandle<0>(aph));
        return ResultWrap<ApplicationProxy*>(rc, new ApplicationProxy(aph));
    }

    ResultWrap<OeService*> Initialize(sm::ServiceManager *SM)
    {
        auto srv = SM->GetService("appletOE");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<OeService*>(rc, static_cast<OeService*>(osrv));
    }
}