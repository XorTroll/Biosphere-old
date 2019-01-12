
#pragma once
#include <bio/biocoenosis/flora/applet/Types.hpp>

namespace bio::applet::oe
{
    class OeService : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<ApplicationProxy*> OpenApplicationProxy(u64 Reserved);
    };

    ResultWrap<OeService*> Initialize();
}