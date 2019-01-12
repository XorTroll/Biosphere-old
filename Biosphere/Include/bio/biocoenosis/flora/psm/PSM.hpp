
#pragma once
#include <bio/biocoenosis/flora/psm/Types.hpp>

namespace bio::psm
{
    class PsmService : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<u32> GetBatteryChargePercentage();
            ResultWrap<ChargerType> GetChargerType();
    };

    ResultWrap<PsmService*> Initialize();

    static const u32 Module = 136;
}