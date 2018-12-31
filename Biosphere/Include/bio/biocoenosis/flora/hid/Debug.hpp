
#pragma once
#include <bio/biocoenosis/flora/sm.hpp>

namespace bio::hid::dbg
{
    class HidDbgService : public hipc::Object
    {
        public:
            using Object::Object;
    };

    ResultWrap<HidDbgService*> Initialize(sm::ServiceManager *SM);
}