
#pragma once
#include <bio/biocoenosis/fauna/sm.hpp>

namespace bio::hid::dbg
{
    class HidDbgService : public hipc::Object
    {
        public:
            using Object::Object;
    };

    ResultWrap<HidDbgService*> Initialize();
}