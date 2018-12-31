#include <bio/biocoenosis/flora/sm/SM.hpp>
#include <vector>
#include <cstdlib>

std::vector<std::pair<u64, u32>> overrides;

namespace bio::sm
{
    Result ServiceManager::Initialize()
    {
        return this->ProcessRequest<0>(hipc::InProcessId(), hipc::InRaw<u64>(0), hipc::InRaw<u64>(0), hipc::InRaw<u64>(0));
    }

    ResultWrap<hipc::Object*> ServiceManager::GetService(const char *Name)
    {
        u64 encname = os::EncodeString(Name);
        if(!overrides.empty()) for(u32 i = 0; i < overrides.size(); i++) if(overrides[i].first == encname) return ResultWrap<hipc::Object*>(0, new Object(overrides[i].second));
        u32 outh = 0;
        Result rc = this->ProcessRequest<1>(hipc::InRaw<u64>(encname), hipc::InRaw<u64>(0), hipc::InRaw<u64>(0), hipc::OutHandle<0>(outh));
        return ResultWrap<hipc::Object*>(rc, new Object(outh));
    }

    ResultWrap<ServiceManager*> Initialize()
    {
        u32 handle = 0;
        Result rc = svc::ConnectToNamedPort(&handle, "sm:");
        return ResultWrap<ServiceManager*>(rc, new ServiceManager(handle));
    }
}