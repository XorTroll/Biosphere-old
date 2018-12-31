#include <bio/biocoenosis/flora/hid/Debug.hpp>

namespace bio::hid::dbg
{
    ResultWrap<HidDbgService*> Initialize(sm::ServiceManager *SM)
    {
        auto srv = SM->GetService("hid:dbg");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<HidDbgService*>(rc, static_cast<HidDbgService*>(osrv));
    }
}