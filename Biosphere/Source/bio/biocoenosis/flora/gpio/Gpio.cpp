#include <bio/biocoenosis/flora/gpio/Gpio.hpp>

namespace bio::gpio
{
    Result PadSession::SetDirection(Direction Dir)
    {
        return this->ProcessRequest<0>(hipc::InRaw<u32>(static_cast<u32>(Dir)));
    }

    ResultWrap<Direction> PadSession::GetDirection()
    {
        u8 dir = 0;
        Result rc = this->ProcessRequest<1>(hipc::OutRaw<u8>(dir));
        return ResultWrap<Direction>(rc, static_cast<Direction>(dir));
    }

    Result PadSession::SetValue(Value Val)
    {
        return this->ProcessRequest<8>(hipc::InRaw<u32>(static_cast<u32>(Val)));
    }

    ResultWrap<Value> PadSession::GetValue()
    {
        u8 val = 0;
        Result rc = this->ProcessRequest<9>(hipc::OutRaw<u8>(val));
        return ResultWrap<Value>(rc, static_cast<Value>(val));
    }

    ResultWrap<PadSession*> GpioService::OpenSession(PadName Name)
    {
        u32 pdh = 0;
        Result rc = this->ProcessRequest<1>(hipc::InRaw<u32>(static_cast<u32>(Name)), hipc::OutHandle<0>(pdh));
        return ResultWrap<PadSession*>(rc, new PadSession(pdh));
    }

    ResultWrap<GpioService*> Initialize(sm::ServiceManager *SM)
    {
        auto srv = SM->GetService("gpio");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<GpioService*>(rc, static_cast<GpioService*>(osrv));
    }
}