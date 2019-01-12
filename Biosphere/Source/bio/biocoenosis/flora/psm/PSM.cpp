#include <bio/biocoenosis/flora/psm/PSM.hpp>

namespace bio::psm
{
    ResultWrap<u32> PsmService::GetBatteryChargePercentage()
    {
        u32 bcp = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutRaw<u32>(bcp));
        return ResultWrap<u32>(rc, bcp);
    }

    ResultWrap<ChargerType> PsmService::GetChargerType()
    {
        u32 cht = 0;
        Result rc = this->ProcessRequest<1>(hipc::OutRaw<u32>(cht));
        return ResultWrap<ChargerType>(rc, static_cast<ChargerType>(cht));
    }

    ResultWrap<PsmService*> Initialize()
    {
        auto srv = sm::GetService("psm");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<PsmService*>(rc, static_cast<PsmService*>(osrv));
    }
}