#include <bio/biocoenosis/fauna/sm/SM.hpp>
#include <vector>
#include <cstdlib>

std::vector<bio::sm::OverrideService> overrides;

namespace bio::sm
{
    static hipc::Object *smport;
    static bool sminit = false;

    Result Initialize()
    {
        Result rc;
        if(!sminit)
        {
            u32 hsm = 0;
            Result rc = svc::ConnectToNamedPort(&hsm, "sm:");
            if(rc.IsFailure()) return rc;
            smport = new hipc::Object(hsm);
            rc = smport->ProcessRequest<0>(hipc::InProcessId(), hipc::InRaw<u64>(0), hipc::InRaw<u64>(0), hipc::InRaw<u64>(0));
            sminit = true;
        }
        return rc;
    }

    bool HasInitialized()
    {
        return sminit;
    }

    ResultWrap<hipc::Object*> GetService(const char *Name)
    {
        u64 encname = os::EncodeString(Name);
        if(!overrides.empty()) for(u32 i = 0; i < overrides.size(); i++) if(overrides[i].EncodedName == encname) return ResultWrap<hipc::Object*>(0, new hipc::Object(overrides[i].Handle, true));
        u32 outh = 0;
        Result rc = smport->ProcessRequest<1>(hipc::InRaw<u64>(encname), hipc::InRaw<u64>(0), hipc::InRaw<u64>(0), hipc::OutHandle<0>(outh));
        return ResultWrap<hipc::Object*>(rc, new hipc::Object(outh));
    }

    void Finalize()
    {
        if(HasInitialized())
        {
            delete smport;
            sminit = false;
        }
    }
}