
#pragma once
#include <bio/biocoenosis/flora/gpio/Types.hpp>

namespace bio::gpio
{
    class PadSession : public hipc::Object
    {
        public:
            using Object::Object;
            Result SetDirection(Direction Dir);
            ResultWrap<Direction> GetDirection();
            Result SetValue(Value Val);
            ResultWrap<Value> GetValue();
    };

    class GpioService : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<PadSession*> OpenSession(PadName Name);
    };

    ResultWrap<GpioService*> Initialize(sm::ServiceManager *SM);

    static const u32 Module = 102;
}