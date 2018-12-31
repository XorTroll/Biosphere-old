
#pragma once
#include <bio/biocoenosis/flora/fatal/Types.hpp>

namespace bio::fatal
{
    class FatalService : public hipc::Object
    {
        public:
            using Object::Object;
            Result ThrowWithPolicy(Result Error, ThrowMode Mode);
    };

    ResultWrap<FatalService*> Initialize(sm::ServiceManager *SM);

    static const u32 Module = 163;
    static const Result ResultNoVolPlusHeld(Module, 5);
    static const Result ResultSpecialCartNotInserted(Module, 6);
}