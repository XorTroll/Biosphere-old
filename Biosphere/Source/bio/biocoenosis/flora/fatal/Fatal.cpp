#include <bio/biocoenosis/flora/fatal/Fatal.hpp>

namespace bio::fatal
{
    Result FatalService::ThrowWithPolicy(Result Error, ThrowMode Mode)
    {
        return this->ProcessRequest<1>(hipc::InProcessId(), hipc::InRaw<u32>((u32)Error), hipc::InRaw<u32>(static_cast<u32>(Mode)), hipc::InRaw<u64>(0));
    }

    ResultWrap<FatalService*> Initialize()
    {
        auto srv = sm::GetService("fatal:u");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<FatalService*>(rc, static_cast<FatalService*>(osrv));
    }
}